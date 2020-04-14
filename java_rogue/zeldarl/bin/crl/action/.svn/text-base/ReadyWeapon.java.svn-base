package crl.action;

import zrl.player.Player;
import crl.item.Item;
import crl.item.ItemDefinition;

public class ReadyWeapon extends Action{
	public String getID(){
		return "ReadyWeapon";
	}
	
	public boolean needsItem(){
		return true;
	}

	public String getPromptItem(){
		return "Ready what?";
	}

	public void execute(){
		ItemDefinition def = targetItem.getDefinition();
		Player player = (Player)performer;
		if (def.getEquipCategory().equals(ItemDefinition.CAT_WEAPON)){
			Item currentSecondaryWeapon = player.getSecondaryWeapon();
			player.reduceQuantityOf(targetItem);
			if (currentSecondaryWeapon != null){
				player.addItem(currentSecondaryWeapon);
			}
			performer.getLevel().addMessage("You ready the "+def.getDescription());
			player.setSecondaryWeapon(targetItem);
		}else if (def.getEquipCategory().equals(ItemDefinition.CAT_SHIELD)){
			Item currentSecondaryWeapon = player.getSecondaryWeapon();
			player.reduceQuantityOf(targetItem);
			if (currentSecondaryWeapon != null){
				player.addItem(currentSecondaryWeapon);
			}
			performer.getLevel().addMessage("You defend yourself with the "+def.getDescription());
			player.setSecondaryWeapon(targetItem);
		}
	}
}