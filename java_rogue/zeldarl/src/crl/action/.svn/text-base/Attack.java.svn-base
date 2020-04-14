package crl.action;

import sz.util.Position;
import sz.util.Util;
import zrl.player.Player;
import crl.feature.Feature;
import crl.item.Item;
import crl.item.ItemDefinition;
import crl.level.Cell;
import crl.level.Level;
import crl.monster.Monster;
import crl.ui.effects.Effect;
import crl.ui.effects.EffectFactory;

public class Attack extends Action{
	public String getID(){
		return "Attack";
	}
	
	public boolean needsDirection(){
		return true;
	}

	private int reloadTime;
	private Item weapon;
	public void execute(){
        Position var = directionToVariation(targetDirection);
        Player player = null;
        reloadTime = 0;
		try {
			player = (Player) performer;
		} catch (ClassCastException cce){
			return;
		}
		
		weapon = player.getWeapon();
		Level aLevel = performer.getLevel();
		
		if (targetDirection == Action.SELF && aLevel.getMonsterAt(player.getPosition()) == null){
			aLevel.addMessage("That's a coward way to give up!");
			return;
		}
		
		targetPosition = Position.add(performer.getPosition(), Action.directionToVariation(targetDirection));
		int startHeight = aLevel.getMapCell(player.getPosition()).getHeight();
		ItemDefinition weaponDef = weapon.getDefinition();

		if (weapon.getReloadTurns() > 0)
			if (weapon.getRemainingTurnsToReload() == 0){
				if (!reload(weapon, player))
					return;
			}
		String [] sfx = weaponDef.getAttackSFX().split(" ");
		if (sfx.length > 0)
			if (sfx[0].equals("MELEE")){
				Effect me = EffectFactory.getSingleton().createDirectionalEffect(performer.getPosition(), targetDirection, Integer.parseInt(sfx[1]), "SFX_WP_"+weaponDef.getID()); 
				aLevel.addEffect(me);
			}
			else if (sfx[0].equals("BEAM")){
				Effect me = EffectFactory.getSingleton().createDirectedEffect(performer.getPosition(), targetPosition, "SFX_WP_"+weaponDef.getID(), Integer.parseInt(sfx[1]));
				aLevel.addEffect(me);
			}else if (sfx[0].equals("MISSILE")){
				Effect me = EffectFactory.getSingleton().createDirectedEffect(performer.getPosition(), targetPosition, "SFX_WP_"+weaponDef.getID(), weaponDef.getRange());
				if (!(weapon.isSlicesThrough() || player.getFlag("COMBAT_PIERCE"))){
					int i = 0;
					for (i=0; i<weapon.getRange(); i++){
						Position destinationPoint = Position.add(performer.getPosition(),
							Position.mul(var, i+1));
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
		for (int i=0; i<weapon.getRange(); i++){
			Position destinationPoint = Position.add(performer.getPosition(),
				Position.mul(var, i+1));
			Position nextPoint = Position.add(destinationPoint,var);
        	Cell destinationCell = aLevel.getMapCell(destinationPoint);
        	/*aLevel.addMessage("You hit the "+destinationCell.getID());*/

			String message = "";
        	

			Monster targetMonster = performer.getLevel().getMonsterAt(destinationPoint);
			message = "";

			if (targetMonster != null){
				if (destinationCell.getHeight() < startHeight-1){
					if (targetMonster.wasSeen())
						aLevel.addMessage("The attack passes over the "+targetMonster.getDescription());
				} else {
					if (destinationCell.getHeight() > startHeight + 1){
						if (weapon.getVerticalRange()>0){
							hits = true;
						} else {
							if (targetMonster.wasSeen()){
								aLevel.addMessage("The attack passes under the "+targetMonster.getDescription());
							}
						}
					} else {
						hits = true;
					}
				}
			}
			boolean cornered = false;
			boolean cinetic = false;
			if (player.getFlag("COMBAT_CINETIC")){
				if (targetDirection != Action.SELF && targetDirection == Action.oppositeFrom(player.getLastAttackDirection())){
					aLevel.addMessage("You swing your weapon with all your might!");
					cinetic = true;
				} else {
					if (i==0)aLevel.addMessage("You swing your weapon!");
				}
			} else {
				if (i==0) aLevel.addMessage("You swing your weapon!");
			}
			
			if (targetMonster != null){
				if (player.getFlag("COMBAT_CORNER") && aLevel.isSolid(nextPoint)){
					aLevel.addMessage("The "+targetMonster.getDescription()+" is cornered!");
					cornered = true;
				}
				if (targetMonster.tryHit(player, player.getWeapon(), cornered, cinetic, targetDirection)){
					if (!targetMonster.isHeavy() && player.getFlag("COMBAT_KNOCKBACK") && Util.chance(75)){
						//Push the monster
						if (!aLevel.isSolid(nextPoint) && aLevel.getMonsterAt(nextPoint)==null){
							aLevel.addMessage("You knock the "+targetMonster.getDescription()+" back!");
							targetMonster.tryLand(nextPoint);
							targetMonster.setCounter("STUN", 1);
						}
					}
					hits = false;
					hitsSomebody = true;
				}
			}
			
			
			Feature destinationFeature = aLevel.getFeatureAt(destinationPoint);
        	if (destinationFeature != null && destinationFeature.isDestroyable()){
        		hitsSomebody = true;
        		if (player.sees(destinationPoint))
        			message = "You hit the "+destinationFeature.getDescription();
        		else
        			message = "You hit something";
				destinationFeature.damage(player, weapon.getAttack()+player.getAttackBonus());
				aLevel.addMessage(message);
			}
        	
			if (aLevel.isSolid(destinationPoint)){
				hitsSomebody = true;
				//aLevel.addMessage("You hit the "+targetMapCell.getShortDescription());
         	}
			
			if (hitsSomebody && !(weapon.isSlicesThrough() || player.getFlag("COMBAT_PIERCE")))
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
		player.setIsAttacking();
		player.setLastAttackDirection(targetDirection);
		
		
	}

	public String getPromptDirection(){
		return "Where do you want to attack?";
	}

	public int getDirection(){
		return targetDirection;
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
	
	public int getCost(){
		Player player = (Player) performer;
		if (weapon != null){
			return player.getAttackCost()+reloadTime;
		} else {
			return (int)(player.getAttackCost() * 1.5);
		}
	}

	public String getSFX(){
		
			return "wav/LTTP_Sword1.wav";
		
	}

}