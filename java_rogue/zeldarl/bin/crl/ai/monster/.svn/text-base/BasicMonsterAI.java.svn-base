package crl.ai.monster;

import java.util.Iterator;

import sz.util.Position;
import sz.util.Util;
import crl.action.Action;
import crl.action.ActionFactory;
import crl.action.MonsterCharge;
import crl.action.MonsterMissile;
import crl.action.MonsterWalk;
import crl.action.SummonMonster;
import crl.actor.Actor;
import crl.ai.ActionSelector;
import crl.level.Cell;
import crl.monster.Monster;

public class BasicMonsterAI extends MonsterAI{
	private boolean isStationary;
	private int waitPlayerRange;
	private int approachLimit = 0;
	private int patrolRange = 0;
	private int chargeCounter = 99;
	private int lastDirection = -1;
	private boolean changeDirection;
	
	public Action selectAction(Actor who){
		Monster aMonster = (Monster) who;
		if (aMonster.getEnemy() != null){
			if (!aMonster.getLevel().getMonsters().contains((Monster)aMonster.getEnemy())){
				aMonster.setEnemy(null);
			}
		}
		if (aMonster.getEnemy() != null || aMonster.hasCounter("CHARM")){
			int directionToMonster = -1;
			if (aMonster.getEnemy() != null){
				directionToMonster = aMonster.stareMonster((Monster)aMonster.getEnemy());
			} else {
				directionToMonster = aMonster.stareMonster();
			}
			
			if (directionToMonster == -1) {
				return null; 
			} else {
				if (isStationary){
					return null;
				} else {
					Action ret = new MonsterWalk();
					if (!who.getLevel().isWalkable(Position.add(who.getPosition(), Action.directionToVariation(directionToMonster)))){
						ret.setDirection(Util.rand(0,7));
					} else {
						ret.setDirection(directionToMonster);
					}
					return ret;
				}
			} 
		}
		int directionToPlayer = aMonster.starePlayer();
		int playerDistance = Position.flatDistance(aMonster.getPosition(), aMonster.getLevel().getPlayer().getPosition());
		if (patrolRange >0 && playerDistance > patrolRange){
			if (lastDirection == -1 || changeDirection){
				lastDirection = Util.rand(0,7);
				changeDirection = false;
			}
			Action ret = new MonsterWalk();
	     	ret.setDirection(lastDirection);
	     	return ret;
		}
		
		if (directionToPlayer == -1) {
			/*if (isStationary || waitPlayerRange > 0) {
				return null;
			} else {
				int direction = Util.rand(0,7);
				Action ret = new MonsterWalk();
	            ret.setDirection(direction);
	            return ret;
			}*/
			return null; 
		} else {
			if (waitPlayerRange > 0 && playerDistance > waitPlayerRange){
				return null;
			}
			
			
			if (playerDistance < approachLimit){
				//get away from player
				int direction = Action.toIntDirection(Position.mul(Action.directionToVariation(directionToPlayer), -1));
				Action ret = new MonsterWalk();
	            ret.setDirection(direction);
	            return ret;
			} else {
				//System.out.println("Randomly decide if will approach the player or attack");
				//System.out.println("Sees player "+ ( aMonster.seesPlayer() ? "Yeah":"non"));
				if (aMonster.seesPlayer() && aMonster.hasFireLine() && rangedAttacks != null && Util.chance(80)){
					//System.out.println("Try to attack the player");
					for (Iterator iter = rangedAttacks.iterator(); iter.hasNext();) {
						RangedAttack element = (RangedAttack) iter.next();
						if (element.getRange() >= playerDistance && Util.chance(element.getFrequency())){
							Action ret = ActionFactory.getActionFactory().getAction(element.getAttackId());
							if (element.getChargeCounter() > 0){
								if (chargeCounter == 0){
									chargeCounter = element.getChargeCounter();
								}else{
									chargeCounter --;
									continue;
								}
							}
							
							if (ret instanceof MonsterMissile){
								((MonsterMissile)ret).set(
										element.getAttackType(),
										element.getStatusEffect(),
										element.getRange(),
										element.getAttackMessage(),
										element.getEffectType(),
										element.getEffectID(),
										element.getDamage(),
										element.getEffectWav()
										);
							}else if (ret instanceof MonsterCharge){
								((MonsterCharge)ret).set(element.getRange(), element.getAttackMessage(), element.getDamage(),element.getStatusEffect(), element.getEffectWav());
							}else if (ret instanceof SummonMonster){
								((SummonMonster)ret).set(element.getSummonMonsterId(), element.getAttackMessage());
							}
							//ret.setPosition(who.getLevel().getPlayer().getPosition());
							ret.setPosition(who.getLevel().getPlayer().getPreviousPosition());
							
							return ret;
						}
					}
				}
				// Couldnt attack the player, so walk to him
				if (isStationary){
					return null;
				} else {
					//System.out.println("Lets move it");
					Action ret = new MonsterWalk();
					int tries = 10;
					int tryDirection = directionToPlayer;
					boolean walkable = false;
					Position tryPosition;
					do {
						tryPosition = Position.add(who.getPosition(), Action.directionToVariation(tryDirection));
						Cell targetMapCell = who.getLevel().getMapCell(tryPosition);
						walkable =  targetMapCell != null && !targetMapCell.isSolid() && 
									(!who.getLevel().isDoor(tryPosition) || (who.getLevel().isDoor(tryPosition) && aMonster.getLevel().getPlayer().getPosition().equals(tryPosition))) && 
									(!targetMapCell.isWater() || (targetMapCell.isWater() && aMonster.isFloating())) &&  
									(targetMapCell.getDamageOnStep() == 0 || (targetMapCell.getDamageOnStep() > 0 && (aMonster.isFloating() || aMonster.isFireResistant())));
						if (!walkable){
							tryDirection = Util.rand(0,7);
							//System.out.println("A shame");
						}
						tries--;
					} while (!walkable && tries > 0);
						
					if (walkable) {
						ret.setDirection(tryDirection);
						return ret;
					} else {
						return null;
					}
				}
			}
		}
	 }

	 public String getID(){
		 return "BASIC_MONSTER_AI";
	 }

	 public ActionSelector derive(){
 		try {
	 		return (ActionSelector) clone();
	 	} catch (CloneNotSupportedException cnse){
			return null;
	 	}
 	}

	public void setApproachLimit(int limit){
		 approachLimit = limit;
	}
	
	public void setWaitPlayerRange(int limit){
		 waitPlayerRange = limit;
	}
	
	public void setPatrolRange(int limit){
		 patrolRange = limit;
	}
	
	public int getPatrolRange(){
		return patrolRange;
	}

	public void setStationary(boolean isStationary) {
		this.isStationary = isStationary;
	}

	public void setChangeDirection(boolean value) {
		changeDirection = value;
	}
	 
}