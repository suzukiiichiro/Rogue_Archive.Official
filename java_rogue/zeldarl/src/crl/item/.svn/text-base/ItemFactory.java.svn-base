package crl.item;

import java.util.*;

import crl.game.Game;
import crl.ui.UserInterface;

import sz.util.*;

public class ItemFactory {
	private static ItemFactory singleton = new ItemFactory();

	private Hashtable definitions = new Hashtable();
	private Vector vDefinitions;

	public static ItemFactory getItemFactory(){
		return singleton;
	}

	public void init(ItemDefinition[] defs) {
		vDefinitions = new Vector();
		for (int i = 0; i < defs.length; i++){
			definitions.put(defs[i].getID(), defs[i]);
			vDefinitions.add(defs[i]);
		}
		
	}

	public ItemDefinition getDefinition(String ID){
		ItemDefinition def = (ItemDefinition)definitions.get(ID);
		if (def == null)
			Debug.doAssert(false, "Invalid Item ID "+ID);
		return def;
	}
	
	public Item createItem(String ID){
		ItemDefinition def = (ItemDefinition)definitions.get(ID);
		if (def == null)
			Debug.doAssert(false, "Invalid Item ID "+ID);
		if (def.isUnique())
			if (Game.getCurrentGame().wasUniqueGenerated(ID)){
				return null;
			} else {
				Game.getCurrentGame().registerUniqueGenerated(ID);
			}
		return new Item(def, Game.getCurrentGame().getLastGeneratedItemIndex());
	}
	
	public Item createItemForLevel(int levelNumber){
		int tolerance = 100;
		while (tolerance>0){
			ItemDefinition def = (ItemDefinition) definitions.values().toArray()[Util.rand(0,definitions.size()-1)];
			if (def.getMinLevel() <= levelNumber && Util.chance(def.getRarity())){
				if (def.isUnique())
					if (Game.getCurrentGame().wasUniqueGenerated(def.getID())){
						return null;
					} else {
						Game.getCurrentGame().registerUniqueGenerated(def.getID());
					}
				return new Item(def, Game.getCurrentGame().getLastGeneratedItemIndex());
			}
			tolerance--;  
		}
		return null;
		/*
		if (Util.chance((int)(1+(double)level.getLevelNumber()/10.0D))){
			//Pick a random modifier
			if (def.getAttack() > 0) {//Is weapon
				Modifier mod = (Modifier)Util.randomElementOf(Item.WEAPON_MODIFIERS);
				return new Item(def,mod);
			} else if (def.getDefense() > 0) { //Is Armor
				Modifier mod = (Modifier)Util.randomElementOf(Item.ARMOR_MODIFIERS);
				return new Item(def,mod);
			} else { //Isnormal item
				return new Item(def);
			}
		} else {
			return new Item(def);
		}*/
	}
}