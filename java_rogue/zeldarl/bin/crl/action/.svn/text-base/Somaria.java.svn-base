package crl.action;

import zrl.player.Player;
import crl.action.Action;
import crl.actor.Actor;
import crl.item.Item;
import crl.item.ItemDefinition;


public class Somaria extends Spell{
	public int getManaCost() {
		return 3;
	}
	
	public String getID(){
		return "Recover";
	}
	
	public void execute(){
		super.execute();
		Player player = (Player)performer;
		player.getLevel().addMessage("You invoke the powers of Somaria!");
		player.recoverHeartsP(50);
	}
	
	public double getTimeCostModifier() {
		return 2;
	}
	
	
}