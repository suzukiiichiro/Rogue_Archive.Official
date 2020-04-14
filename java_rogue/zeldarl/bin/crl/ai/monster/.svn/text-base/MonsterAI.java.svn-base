package crl.ai.monster;

import java.util.Vector;

import sz.util.Debug;
import crl.action.Action;
import crl.actor.Actor;
import crl.ai.ActionSelector;
import crl.game.Game;

public abstract class MonsterAI  implements ActionSelector, Cloneable{
	protected Vector rangedAttacks;
	
	public void setRangedAttacks(Vector pRangedAttacks){
   	 rangedAttacks = pRangedAttacks;
    }
	
	public abstract Action selectAction(Actor who);
	public abstract String getID();

	public ActionSelector derive(){
		try {
			return (ActionSelector) clone();
		} catch (Exception e) {
			Game.crash("Failed to clone MonsterAI "+getID(), e);
			return null;
		}
	}
}
