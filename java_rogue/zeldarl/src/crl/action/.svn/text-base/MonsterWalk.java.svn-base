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

public class MonsterWalk extends Action{
	public String getID(){
		return "MonsterWalk";
	}
	
	public boolean needsDirection(){
		return true;
	}

	public void execute(){
		Debug.doAssert(performer instanceof Monster, "The player tried to MonsterWalk...");
		Monster aMonster = (Monster) performer;
        Position var = directionToVariation(targetDirection);
        Position destinationPoint = Position.add(performer.getPosition(), var);
        Level aLevel = performer.getLevel();
        if (!aLevel.isValidCoordinate(destinationPoint))
        	return;
        /*if (aLevel.isDoor(destinationPoint) && !destinationPoint.equals(aLevel.getPlayer().getPosition()))
        	return;*/
        aMonster.tryLand(destinationPoint);
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