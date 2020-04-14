package crl.action;

import sz.util.Debug;
import sz.util.Position;
import sz.util.Util;
import crl.action.Action;
import crl.ai.monster.BasicMonsterAI;
import crl.feature.Feature;
import crl.feature.SmartFeature;
import crl.game.SFXManager;
import crl.level.Cell;
import crl.level.Level;
import crl.monster.Monster;

public class CharmedWalk extends Action{
	public String getID(){
		return "CharmedWalk";
	}
	
	public boolean needsDirection(){
		return true;
	}

	public void execute(){
		Monster aMonster = (Monster) performer;
        Position var = directionToVariation(targetDirection);
        Position destinationPoint = Position.add(performer.getPosition(), var);
        Level aLevel = performer.getLevel();
        if (!aLevel.isValidCoordinate(destinationPoint))
        	return;
        Cell destinationCell = aLevel.getMapCell(destinationPoint);
        Cell currentCell = aLevel.getMapCell(performer.getPosition());
        
        Monster destinationMonster = aLevel.getMonsterAt(destinationPoint);
        Feature destinationFeature = aLevel.getFeatureAt(destinationPoint);
        SmartFeature standing = aLevel.getSmartFeature(performer.getPosition());
        if (standing != null)
        	if (standing.getEffectOnStep() != null){
		        String[] effects = standing.getEffectOnStep().split(" ");
		        if (effects[0].equals("TRAP") && aMonster != aLevel.getBoss()){
			        aLevel.addMessage("The "+aMonster.getDescription()+" is trapped!");
		        	return;
		        }
		    }

        if (destinationFeature != null && destinationFeature.isSolid()){
        	//if (Util.chance(50)){
        	if (aMonster.wasSeen())
        		aLevel.addMessage("The "+aMonster.getDescription()+" hits the "+destinationFeature.getDescription());
        	destinationFeature.damage(aMonster);
        	//}
        }
        if (aLevel.isSolid(destinationPoint))
        	if (aMonster.getSelector() instanceof BasicMonsterAI)
        		if (((BasicMonsterAI)aMonster.getSelector()).getPatrolRange() > 0)
        			((BasicMonsterAI)aMonster.getSelector()).setChangeDirection(true);
        if (destinationCell != null){
        	if (destinationFeature != null && destinationFeature.isSolid()){
        		
        	}else {
				if (!aLevel.isSolid(destinationPoint))
					if (destinationMonster == null)
						if (currentCell == null || destinationCell.getHeight() <= currentCell.getHeight() +1)
							if (aLevel.getPlayer().getPosition().equals(destinationPoint)){
								aLevel.getPlayer().tryHit((Monster)performer);
							} else {
								performer.setPosition(destinationPoint);
							}
        	}
        }

	}

	public int getCost(){
		Monster m = (Monster) performer;
		if (m.getWalkCost() == 0){
			Debug.say("quickie monster");
			return 10;
		}
		//System.out.println("m walkCost "+m.getWalkCost());
		return m.getWalkCost();
	}
}