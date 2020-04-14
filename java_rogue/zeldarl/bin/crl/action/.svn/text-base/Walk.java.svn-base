package crl.action;

import sz.util.Debug;
import sz.util.Position;
import sz.util.Util;
import zrl.player.Player;
import crl.actor.Actor;
import crl.feature.Feature;

import crl.level.Cell;
import crl.level.Level;
import crl.monster.Monster;

public class Walk extends Action{
	private Player aPlayer;
	
	public String getID(){
		return "Walk";
	}
	
	public boolean needsDirection(){
		return true;
	}

	public void execute(){
		Debug.doAssert(performer instanceof Player, "An actor different from the player tried to execute Walk action");
		Debug.enterMethod(this, "execute");
		aPlayer = (Player) performer;
		if (targetDirection == Action.SELF){
			aPlayer.getLevel().addMessage("You walk around yourself");
			return;
		}
        Position var = directionToVariation(targetDirection);
        Position destinationPoint = Position.add(performer.getPosition(), var);
        Level aLevel = performer.getLevel();
        Cell destinationCell = aLevel.getMapCell(destinationPoint);
        Feature destinationFeature = aLevel.getFeatureAt(destinationPoint);
        Cell currentCell = aLevel.getMapCell(performer.getPosition());

        if (destinationCell == null){
        	if (!aLevel.isValidCoordinate(destinationPoint)){
        		aPlayer.land();
        		aPlayer.setLastWalkingDirection(Action.SELF);
        		Debug.exitMethod();
            	return;
        	}
        	
        	destinationPoint = aLevel.getDeepPosition(destinationPoint);
        	if (destinationPoint == null){
        		aPlayer.land();
        		Debug.exitMethod();
        		aPlayer.setLastWalkingDirection(Action.SELF);
            	return;
        	} else {
        		aLevel.addMessage("You fall!");
        		destinationCell = aLevel.getMapCell(destinationPoint);
        		currentCell = aLevel.getMapCell(destinationPoint);
        		aPlayer.setLastWalkingDirection(Action.SELF);
        	}
        }

        boolean bumped = true;
       	if (destinationCell.getHeight() > currentCell.getHeight()+2)
			aLevel.addMessage("You can't climb it.");
		else {
			if (destinationCell.getHeight() < currentCell.getHeight())
				aLevel.addMessage("You descend");
			if (destinationCell.isSolid())
				aLevel.addMessage("You bump into the "+destinationCell.getShortDescription());
			else
				if (destinationFeature != null && 
						(destinationFeature.isSolid() || 
								(destinationFeature.getKeyCost() > 0 && !aPlayer.checkKeys(destinationFeature)) ||
								(destinationFeature.getID().equals("BOSS_DOOR") && !aPlayer.checkBossDoor(destinationFeature)) ||
								(destinationFeature.getID().equals("TREASURE_DOOR") && !aPlayer.checkTreasureDoor(destinationFeature))
								
								))
					aLevel.addMessage("You bump into the "+destinationFeature.getDescription());
				else
					if (!aLevel.isWalkable(destinationPoint))
						aLevel.addMessage("Your way is blocked");
					else {
						if (aPlayer.getFlag("COMBAT_SLASH") || aPlayer.getFlag("COMBAT_HALFSLASH")){
							boolean once = !aPlayer.getFlag("COMBAT_SLASH");
							int otherDir1 = 0;
							int otherDir2 = 0;
							switch (targetDirection){
								case Action.UP:
									otherDir1 = Action.UPLEFT;
									otherDir2 = Action.UPRIGHT;
									break;
								case Action.DOWN:
									otherDir1 = Action.DOWNLEFT;
									otherDir2 = Action.DOWNRIGHT;
									break;
								case Action.LEFT:
									otherDir1 = Action.UPLEFT;
									otherDir2 = Action.DOWNLEFT;
									break;
								case Action.RIGHT:
									otherDir1 = Action.UPRIGHT;
									otherDir2 = Action.DOWNRIGHT;
									break;
								case Action.UPRIGHT:
									otherDir1 = Action.UP;
									otherDir2 = Action.RIGHT;
									break;
								case Action.UPLEFT:
									otherDir1 = Action.UP;
									otherDir2 = Action.LEFT;
									break;
								case Action.DOWNLEFT:
									otherDir1 = Action.LEFT;
									otherDir2 = Action.DOWN;
									break;
								case Action.DOWNRIGHT:
									otherDir1 = Action.RIGHT;
									otherDir2 = Action.DOWN;
									break;
							}
							Monster m1 = aLevel.getMonsterAt(Position.add(aPlayer.getPosition(), Action.directionToVariation(otherDir1)));
							Monster m2 = aLevel.getMonsterAt(Position.add(aPlayer.getPosition(), Action.directionToVariation(otherDir2)));
							boolean hit = false;
							if (m1 != null){
								aLevel.addMessage("You slash through the "+m1.getDescription()+"!");
								hit = m1.tryHit(aPlayer, aPlayer.getWeapon(), false, false, Action.SELF);
							}
							if (!(hit && once) || !hit){
								if (m2 != null){
									aLevel.addMessage("You slash through the "+m2.getDescription()+"!");
									m2.tryHit(aPlayer, aPlayer.getWeapon(), false, false, Action.SELF);
								}
							} 
						}
						Actor aActor = aLevel.getActorAt(destinationPoint);
						if (aActor != null){
							aLevel.addMessage("Your way is blocked");
						} else {
							if (aLevel.getBloodAt(aPlayer.getPosition()) != null)
								if (Util.chance(30)){
									aLevel.addBlood(destinationPoint, Util.rand(0,1));
								}
							aPlayer.landOn(destinationPoint);
							
							if (aPlayer.getLastWalkingDirection()!= Action.SELF && aPlayer.getLastWalkingDirection() == targetDirection){
								aPlayer.increaseChargeCounter(); 
								//aPlayer.setIsDodging(false);
							} else {
								if (aPlayer.getFlag("COMBAT_MIRAGE")){
									if (aPlayer.getLastWalkingDirection()!= Action.SELF){
										Position lastWalkingVar = Action.directionToVariation(aPlayer.getLastWalkingDirection());
										Position targetVar = Action.directionToVariation(targetDirection);
										if (lastWalkingVar.x == -targetVar.x || lastWalkingVar.y == -targetVar.y)
											aPlayer.setCounter("DODGE", 2);
											//aPlayer.getLastWalkingDirection() == oppositeFrom(targetDirection)){
									}
								} else {
									//aPlayer.setIsDodging(false);
								}
								aPlayer.setLastWalkingDirection(targetDirection);
								aPlayer.resetChargeCounter();
							}
							bumped = false;
							aPlayer.setRunning(true);
						}
					}
		}
       	if (bumped)
       		aPlayer.setLastWalkingDirection(Action.SELF);
       	Debug.exitMethod();
	}

	public int getCost(){
		//System.out.println("walkCost "+aPlayer.getWalkCost());
		return aPlayer.getWalkCost();
	}
}