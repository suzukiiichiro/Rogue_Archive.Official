package crl.action;

import sz.util.Line;
import sz.util.Position;
import zrl.player.Player;
import crl.actor.Actor;
import crl.feature.Feature;
import crl.item.Item;
import crl.item.ItemDefinition;
import crl.level.Cell;
import crl.level.Level;
import crl.monster.Monster;
import crl.ui.effects.Effect;
import crl.ui.effects.EffectFactory;

public class Target extends Action{
	private Player player;
	private Item weapon;
	private int reloadTime;
	public String getID(){
		return "Target";
	}
	
	public boolean needsPosition(){
		return true;
	}

	public void execute(){
		player = null;
        reloadTime = 0;
		try {
			player = (Player) performer;
		} catch (ClassCastException cce){
			return;
		}
		
		weapon = player.getWeapon();
		Level aLevel = performer.getLevel();
		int startHeight = aLevel.getMapCell(player.getPosition()).getHeight();
		
		/*if (targetPosition.equals(performer.getPosition()) && aLevel.getMonsterAt(targetPosition)==null){
			aLevel.addMessage("That's a coward way to give up!");
			return;
        }*/



		ItemDefinition weaponDef = weapon.getDefinition();

		if (weapon.getReloadTurns() > 0)
			if (weapon.getRemainingTurnsToReload() == 0){
				/*
				aLevel.addMessage("You must reload the "+weapon.getDescription()+"!");
				return;*/
				if (!reload(weapon, player))
					return;
			}

		
		String [] sfx = weaponDef.getAttackSFX().split(" ");
		if (sfx.length > 0)
			if (sfx[0].equals("MISSILE")){
				//AnimatedLineMissileEffect me = new AnimatedLineMissileEffect(performer.getPosition(), sfx[1], Integer.parseInt(sfx[3]), performer.getPosition(), targetPosition, Integer.parseInt(sfx[2]), 40);
				Effect me = EffectFactory.getSingleton().createDirectedEffect(performer.getPosition(), targetPosition, "SFX_WP_"+weaponDef.getID(), weaponDef.getRange());
				//me.setAnimationDelay(Integer.parseInt(sfx[4]));
				if (!weapon.isSlicesThrough()){
					int i = 0;
					Line path = new Line(performer.getPosition(), targetPosition);
					for (i=0; i<weapon.getRange(); i++){
						Position destinationPoint = path.next();
						Cell destinationCell = aLevel.getMapCell(destinationPoint);
			        	Feature destinationFeature = aLevel.getFeatureAt(destinationPoint);
	       	 			if (destinationFeature != null && destinationFeature.isDestroyable())
    	   	 				break;
						Monster targetMonster = performer.getLevel().getMonsterAt(destinationPoint);
						if (
							targetMonster != null &&
							!(targetMonster.isInWater() && false) &&
							(destinationCell.getHeight() == aLevel.getMapCell(player.getPosition()).getHeight() ||
							destinationCell.getHeight() -1  == aLevel.getMapCell(player.getPosition()).getHeight() ||
							destinationCell.getHeight() == aLevel.getMapCell(player.getPosition()).getHeight()-1)
						)
							break;
					}
					me = EffectFactory.getSingleton().createDirectedEffect(performer.getPosition(), targetPosition, "SFX_WP_"+weaponDef.getID(), i);
				}
				aLevel.addEffect(me);
			}


		boolean hitsSomebody = false;
		boolean hits = false;
		Line path = new Line(performer.getPosition(), targetPosition);
		for (int i=0; i<weapon.getRange(); i++){
			Position destinationPoint = path.next();
        	Cell destinationCell = aLevel.getMapCell(destinationPoint);
        	/*aLevel.addMessage("You hit the "+destinationCell.getID());*/

			String message = "";
        	Feature destinationFeature = aLevel.getFeatureAt(destinationPoint);
        	if (destinationFeature != null && destinationFeature.isDestroyable()){
        		hitsSomebody = true;
	        	message = "You hit the "+destinationFeature.getDescription();
				Feature prize = destinationFeature.damage(player, weapon.getAttack());
	        	if (prize != null){
		        	message += ", and destroy it!";
				}
				aLevel.addMessage(message);
			}

			Monster targetMonster = performer.getLevel().getMonsterAt(destinationPoint);
			message = "";

			if (targetMonster != null){
				if ((targetMonster.isInWater() && false)|| destinationCell.getHeight() < startHeight-1){
					if (targetMonster.wasSeen())
						aLevel.addMessage("The dagger flies over the "+targetMonster.getDescription());
				} else {
					if (destinationCell.getHeight() > startHeight + 1){
						if (weapon.getVerticalRange()>0){
							hits = true;
						} else {
							if (targetMonster.wasSeen()){
								aLevel.addMessage("The dagger flies under the "+targetMonster.getDescription());
							}
						}
					} else {
						hits = true;
					}
				}
			}
			
			if (targetMonster != null && targetMonster.tryHit(player, player.getWeapon(), false, false, Action.SELF)){
				hits = false;
				hitsSomebody = true;
			}
			
			if (aLevel.isSolid(destinationPoint)){
				//aLevel.addMessage("You hit the "+targetMapCell.getShortDescription());
				hitsSomebody = true;
         	}
			
			if (hitsSomebody && !weapon.isSlicesThrough())
				break;
		}
/*		if (!hitsSomebody)
			aLevel.addMessage("You swing at the air!");*/
		if (weapon.getReloadTurns() > 0 &&
			weapon.getRemainingTurnsToReload() > 0)
			weapon.setRemainingTurnsToReload(weapon.getRemainingTurnsToReload()-1);
		if (weaponDef.isSingleUse()){
			if (player.hasItem(weapon))
				player.reduceQuantityOf(weapon);
			else
				player.setWeapon(null);
		}
	}

	public String getPromptPosition(){
		return "Where do you want to attack?";
	}

	public Position getPosition(){
		return targetPosition;
	}


	public String getSFX(){
		return "wav/rich_yah.wav";
	}

	public int getCost(){
		return player.getAttackCost()+weapon.getDefinition().getAttackCost()+reloadTime;
	}
	
	private boolean reload(Item weapon, Player aPlayer){
		if (weapon != null){
			if (aPlayer.getArrows() < weapon.getDefinition().getReloadCostGold()){
				aPlayer.getLevel().addMessage("You don't have enough arrows");
				return false;
			}
			else {
				weapon.reload();
				aPlayer.reduceGold(weapon.getDefinition().getReloadCostGold());
				//aPlayer.getLevel().addMessage("You reload the " + weapon.getDescription()+" ("+weapon.getDefinition().getReloadCostGold()+" gold)");
				reloadTime = 10*weapon.getDefinition().getReloadTurns();
				return true;
			}
		}
		return false;
	}
	
	public boolean canPerform(Actor a){
        player = null;
		try {
			player = (Player) a;
		} catch (ClassCastException cce){
			return false;
		}
		
		Item weapon = player.getWeapon();
		
		if (weapon == null) {
			invalidationMessage = "It is useless to target your own blows...";
			
			return false;
		}
		
		if (weapon.getRange() < 2){
			invalidationMessage = "You can´t target your "+weapon.getDescription();
			
			return false;
		}
/*
		ItemDefinition weaponDef = weapon.getDefinition();

		/*if (weapon.getReloadTurns() > 0)
			if (weapon.getRemainingTurnsToReload() == 0){
				aLevel.addMessage("You must reload the "+weapon.getDescription()+"!");
				return false;
			}*/
		
		return true;
	}
}