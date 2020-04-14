package crl.action;

import zrl.player.Player;
import crl.action.Action;
import crl.actor.Actor;
import crl.item.Item;
import crl.item.ItemDefinition;

public abstract class Spell extends Action{
	public abstract int getManaCost();
	public abstract double getTimeCostModifier();
	
	public void execute(){
		Player aPlayer = (Player) performer;
		aPlayer.reduceMagic(getManaCost());
	}

	public final int getCost(){
		Player p = (Player) performer;
		return (int)(p.getCastCost() * getTimeCostModifier());
	}

	public final boolean canPerform(Actor a) {
		Player p = (Player) a;
		if (p.getMagic() >= getManaCost()){
			return true;
		}
		invalidationMessage = "Your magic is too low";
		return false;
	}
}