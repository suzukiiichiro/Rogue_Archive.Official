package crl.action;

import zrl.player.Player;
import crl.action.Action;
import crl.actor.Actor;
import crl.level.Level;

public class Yendor extends Spell{
	public String getID(){
		return "TimeStop";
	}
	
	public void execute(){
		super.execute();
		Player aPlayer = (Player) performer;
		Level x = performer.getLevel();
		x.addMessage("You invoke the powers of Yendor!");
		x.stopTime(25);
	}

	public String getSFX(){
		return "wav/clockbel.wav";
	}
	
	public int getManaCost() {
		return 3;
	}
	
	public double getTimeCostModifier() {
		return 1;
	}
}