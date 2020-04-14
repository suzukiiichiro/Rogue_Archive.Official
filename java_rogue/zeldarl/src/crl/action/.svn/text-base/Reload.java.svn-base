package crl.action;

import zrl.player.Player;
import crl.actor.Actor;
import crl.item.Item;

public class Reload extends Action{
	private transient Item weapon;
	public int getCost() {
		if (weapon != null)
			return 10 * weapon.getDefinition().getReloadCostGold();
		else
			return 50;
	}

	public String getID(){
		return "Reload";
	}
	
	public void execute(){
		Player aPlayer = (Player) performer;
		weapon = aPlayer.getWeapon();
		if (weapon != null){
			if (aPlayer.getGold() < weapon.getDefinition().getReloadCostGold())
				aPlayer.getLevel().addMessage("You can't reload the " + weapon.getDescription());
			else {
				weapon.reload();
				aPlayer.reduceGold(weapon.getDefinition().getReloadCostGold());
				aPlayer.getLevel().addMessage("You reload the " + weapon.getDescription()+" ("+weapon.getDefinition().getReloadCostGold()+" gold)");
			}
		} else
			aPlayer.getLevel().addMessage("You can't reload yourself");
 	}
	
	public boolean canPerform(Actor a){
		Player aPlayer = (Player) a;
		Item weapon = aPlayer.getWeapon();
		if (weapon != null){
			if (weapon.getReloadTurns()>0){
				if (aPlayer.getGold() < weapon.getDefinition().getReloadCostGold()){
					invalidationMessage = "You need "+weapon.getDefinition().getReloadCostGold()+" gold to reload the " + weapon.getDescription();
					return false;
				}
				else {
					return true;
				}
			} else {
				invalidationMessage = "The " + weapon.getDescription()+" cannot be reloaded";
				return false;
			}
		} else {
			invalidationMessage = "You can't reload yourself";
			return false;
		}
	}
}