package crl.action;

import zrl.player.Player;
import crl.actor.Actor;
import crl.item.Item;
import crl.item.ItemDefinition;

public class SwitchWeapons extends Action{
	private transient Item weapon;
	public int getCost() {
		if (weapon != null)
			return 10 * weapon.getDefinition().getReloadCostGold();
		else
			return 50;
	}

	public String getID(){
		return "SwitchWeapons";
	}
	
	public void execute(){
		Player aPlayer = (Player) performer;
		Item secondary = aPlayer.getSecondaryWeapon();
		if (secondary == null){
			aPlayer.getLevel().addMessage("You don't have a secondary weapon");
			return;
		}
		Item primary = aPlayer.getWeapon();
		aPlayer.setWeapon(secondary);
		if (primary != null){
			aPlayer.setSecondaryWeapon(primary);
			aPlayer.getLevel().addMessage("You switch your "+primary.getDescription()+" for your "+secondary.getDescription());
		} else {
			aPlayer.setSecondaryWeapon(null);
			aPlayer.getLevel().addMessage("You equip your "+secondary.getDescription());
		}
 	}
	
	public boolean canPerform(Actor a){
		Player aPlayer = (Player) a;
		Item secondary = aPlayer.getSecondaryWeapon();
		if (secondary == null || secondary.getDefinition().getEquipCategory().equals(ItemDefinition.CAT_SHIELD)){
			invalidationMessage = "You don't have a secondary weapon";
			return false;
		}
		return true;
	}
}