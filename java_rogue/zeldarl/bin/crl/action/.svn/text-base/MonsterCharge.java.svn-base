package crl.action;

import sz.util.Debug;
import sz.util.Position;
import zrl.player.Player;
import crl.action.Action;
import crl.game.SFXManager;
import crl.level.Cell;
import crl.level.Level;
import crl.monster.Monster;

public class MonsterCharge extends Action{
	private int range;
	private String message;
	private String effectWav;
	private String statusEffect;
	private int damage;
	public String getID(){
		return "MONSTER_CHARGE";
	}
	
	public boolean needsDirection(){
		return true;
	}

	public void set(int pRange, String pMessage, int pDamage, String pStatusEffect, String pEffectWav){
		statusEffect = pStatusEffect;
		message = pMessage;
		range = pRange;
		damage = pDamage;
		effectWav = pEffectWav;
	}
	
	public void execute(){
		Debug.doAssert(performer instanceof Monster, "Someone not a monster tried to JumpOver");
		Monster aMonster = (Monster) performer;
		targetDirection = aMonster.starePlayer();
        Position var = directionToVariation(targetDirection);
        Level aLevel = performer.getLevel();
        Player aPlayer = aLevel.getPlayer();
        //aLevel.addMessage("The "+aMonster.getDescription()+" "+message+".");
        aLevel.addMessage("The "+aMonster.getDescription()+" "+message+".");
        Cell currentCell = aLevel.getMapCell(performer.getPosition());
        Position destinationPoint = null;
        if (effectWav != null)
        	SFXManager.play(effectWav);
        for (int i=0; i<range; i++){
			destinationPoint = Position.add(aMonster.getPosition(), var);
			Position deepPoint  = aLevel.getDeepPosition(destinationPoint);
			if (deepPoint == null){
				aLevel.addMessage("The "+aMonster.getDescription()+ " falls into an endless pit! at "+destinationPoint);
				performer.die();
				performer.getLevel().removeMonster(aMonster);
				break;
			}
			Cell destinationCell = aLevel.getMapCell(deepPoint);
			if (
					( aLevel.isSolid(deepPoint) || destinationCell.getHeight() > currentCell.getHeight() +1
					)
				) {
				aLevel.addMessage("The "+aMonster.getDescription()+ " bumps into the "+destinationCell.getShortDescription());
				break;
			}

			if (aPlayer.getPosition().equals(destinationPoint)){
				if (aPlayer.tryHit(aMonster, damage)){
					
					if (statusEffect != null){
						if (statusEffect.equals(Player.STATUS_STUN)){
							aLevel.addMessage("You are stunned!");
							aPlayer.setCounter("PARALIZED", 8);
						} else if (statusEffect.equals(Player.STATUS_POISON)){
							aLevel.addMessage("Your blood is poisoned!");
							aPlayer.setCounter("POISON", 15);
						} /*else if (statusEffect.equals(Player.STATUS_PETRIFY)){
							aLevel.addMessage("Your skin petrifies!");
							aPlayer.setPetrify(10);
						}*/
					}
				}
			} else {
				aMonster.tryLand(destinationPoint);
			}
		}
		
	}

	public String getPromptDirection(){
		return "Where do you want to whip?";
	}

	public int getCost(){
		Monster m = (Monster) performer;
		if (m.getAttackCost() == 0){
			Debug.say("quickie monster");
			return 10;
		}
		//System.out.println("m attackCost "+m.getAttackCost());
		return m.getAttackCost();
	}
	
}