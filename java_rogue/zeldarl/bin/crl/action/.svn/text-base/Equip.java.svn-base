package crl.action;

import zrl.player.Player;
import crl.item.Item;
import crl.item.ItemDefinition;

public class Equip extends Action{
	public String getID(){
		return "Equip";
	}
	
	public boolean needsItem(){
		return true;
	}

	public String getPromptItem(){
		return "Wear what?";
	}

	public void execute(){
		ItemDefinition def = targetItem.getDefinition();
		Player player = (Player)performer;
		if (def.getEquipCategory().equals(ItemDefinition.CAT_ARMOR)){
				Item currentArmor = player.getArmor();
				player.reduceQuantityOf(targetItem);
				if (currentArmor != null)
					player.addItem(currentArmor);
				performer.getLevel().addMessage("You wear the "+def.getDescription());
				player.setArmor(targetItem);
				
		} else if (def.getEquipCategory().equals(ItemDefinition.CAT_WEAPON)){
				Item currentWeapon = player.getWeapon();
				player.reduceQuantityOf(targetItem);
				if (currentWeapon != null){
					player.addItem(currentWeapon);
				}
				performer.getLevel().addMessage("You wear the "+def.getDescription());
				player.setWeapon(targetItem);
		} else if (def.getEquipCategory().equals(ItemDefinition.CAT_ACCESORY)){
				Item currentAccesory = player.getAccesory();
				player.reduceQuantityOf(targetItem);
				if (currentAccesory != null)
					player.addItem(currentAccesory);
				performer.getLevel().addMessage("You wear the "+def.getDescription());
				player.setAccesory(targetItem);
		} else if (def.getEquipCategory().equals(ItemDefinition.CAT_SHIELD)){
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