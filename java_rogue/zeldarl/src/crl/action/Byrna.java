package crl.action;

import zrl.player.Player;
import crl.action.Action;
import crl.actor.Actor;
import crl.item.Item;
import crl.item.ItemDefinition;

public class Byrna extends Spell{
	public int getManaCost() {
		return 3;
	}
	
	public String getID(){
		return "Protection";
	}
	
	public void execute(){
		super.execute();
		Player player = (Player)performer;
		player.getLevel().addMessage("You invoke the powers of Byrna!");
		player.setCounter("PROTECTION", 25);
	}
	
	public double getTimeCostModifier() {
		return 2;
	}
	
	
}