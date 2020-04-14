package crl.data;

import crl.feature.Feature;
import crl.ui.AppearanceFactory;

public class Features {
	public static Feature[] getFeatureDefinitions(AppearanceFactory apf){
        Feature [] ret = new Feature [46];
		ret[0] = new Feature("MAGIC_BARRIER", apf.getAppearance("MAGIC_BARRIER"), 15, "Magic Barrier");
		ret[1] = new Feature("ROCK", apf.getAppearance("ROCK"), 20, "Rock");
		ret[2] = new Feature("COIN", apf.getAppearance("COIN"), 9999, "Coin");
		ret[3] = new Feature("BIGCOIN", apf.getAppearance("BIGCOIN"), 9999, "Big Coin");
		
		ret[4] = new Feature("RED_POOL", apf.getAppearance("RFOUNTAIN_F"), 9999, "Mana water");
		ret[5] = new Feature("YELLOW_POOL", apf.getAppearance("YFOUNTAIN_F"), 9999, "Health water");
		ret[6] = new Feature("RED_WATER", apf.getAppearance("RSFOUNTAIN_F"), 9999, "Mana water");
		ret[7] = new Feature("YELLOW_WATER", apf.getAppearance("YSFOUNTAIN_F"), 9999, "Health water");
		ret[8] = new Feature("FORGE", apf.getAppearance("FORGE"), 9999, "Magic Anvil");
		ret[9] = new Feature("ANKH", apf.getAppearance("ANKH"), 9999, "Ankh");
		
		ret[10] = new Feature("SMALL_KEY", apf.getAppearance("SMALL_KEY"), 9999, "Small Key");
		ret[11] = new Feature("LOCKED_DOOR", apf.getAppearance("LOCKED_DOOR"), 9999, "Locked Door");
		ret[12] = new Feature("COMPASS", apf.getAppearance("COMPASS"), 9999, "Compass");
		ret[13] = new Feature("BOW", apf.getAppearance("BOW"), 9999, "Bow");
		
		ret[14] = new Feature("BIG_KEY", apf.getAppearance("BIG_KEY"), 9999, "Big Key");
		ret[15] = new Feature("BOSS_DOOR", apf.getAppearance("BOSS_DOOR"), 9999, "Master's Door");
		
		ret[16] = new Feature("CANE_BYRNA", apf.getAppearance("CANE_BYRNA"), 9999, "The Cane of Byrna");
		ret[17] = new Feature("CANE_SOMARIA", apf.getAppearance("CANE_SOMARIA"), 9999, "The Cane of Somaria");
		ret[18] = new Feature("CANE_YENDOR", apf.getAppearance("CANE_YENDOR"), 9999, "The Cane of Yendor");
		ret[19] = new Feature("CANE_HYLIA", apf.getAppearance("CANE_HYLIA"), 9999, "The Cane of Hylia");
		
		ret[20] = new Feature("TREASURE_KEY", apf.getAppearance("TREASURE_KEY"), 9999, "Treasure Key");
		ret[21] = new Feature("TREASURE_DOOR", apf.getAppearance("TREASURE_DOOR"), 9999, "Locked Door");
		
		ret[22] = new Feature("SWORD", apf.getAppearance("SWORD"), 9999, "Sword");
		ret[23] = new Feature("BOOMERANG", apf.getAppearance("BOOMERANG"), 9999, "Boomerang");
		
		ret[24] = new Feature("RUPEE", apf.getAppearance("RUPEE"), 9999, "Rupee"); ret[24].setGoldPrize(1);
		ret[25] = new Feature("BLUE_RUPEE", apf.getAppearance("BLUE_RUPEE"), 9999, "Blue Rupee"); ret[25].setGoldPrize(5);
		ret[26] = new Feature("RED_RUPEE", apf.getAppearance("RED_RUPEE"), 9999, "Red Rupee"); ret[26].setGoldPrize(20);
		
		ret[27] = new Feature("HEART", apf.getAppearance("HEART"), 9999, "Heart"); ret[27].setHealPrize(2);
		
		ret[28] = new Feature("HOOKSHOOT", apf.getAppearance("HOOKSHOOT"), 9999, "HookShoot");
		
		ret[29] = new Feature("FLIPPERS", apf.getAppearance("FLIPPERS"), 9999, "Flippers");
		
		ret[30] = new Feature("ALTARBYRNA", apf.getAppearance("ALTARBYRNA"), 9999, "The Altar of Byrna");
		ret[31] = new Feature("ALTARSOMARIA", apf.getAppearance("ALTARSOMARIA"), 9999, "The Altar of Somaria");
		ret[32] = new Feature("ALTARYENDOR", apf.getAppearance("ALTARYENDOR"), 9999, "The Altar of Yendor");
		
		ret[33] = new Feature("ALTARBYRNA_X", apf.getAppearance("ALTARBYRNA_X"), 9999, "The Cane of Byrna");
		ret[34] = new Feature("ALTARSOMARIA_X", apf.getAppearance("ALTARSOMARIA_X"), 9999, "The Cane of Somaria");
		ret[35] = new Feature("ALTARYENDOR_X", apf.getAppearance("ALTARYENDOR_X"), 9999, "The Cane of Yendor");
		
		ret[36] = new Feature("BUSH", apf.getAppearance("BUSH"), 0, "Bush");
		ret[36].setSolid(true);
		ret[36].setDestroyable(true);
		ret[36].setRelevant(false);
		
		ret[37] = new Feature("HEARTCONTAINER", apf.getAppearance("HEARTCONTAINER"), 9999, "Heart Container");
		ret[38] = new Feature("RAFT", apf.getAppearance("RAFT"), 9999, "Raft");
				
		
		ret[39] = new Feature("FORESTDUNGEON", apf.getAppearance("FORESTDUNGEON"), 9999, "Forest Monastery");
		ret[40] = new Feature("DESERTDUNGEON", apf.getAppearance("DESERTDUNGEON"), 9999, "Temple of Sands");
		ret[41] = new Feature("RUINSDUNGEON", apf.getAppearance("RUINSDUNGEON"), 9999, "Temple Ruins");
		ret[42] = new Feature("KAKARIKO", apf.getAppearance("KAKARIKO"), 9999, "Kakariko Village");
		ret[43] = new Feature("LINKS", apf.getAppearance("LINKS"), 9999, "Link's House");
		ret[44] = new Feature("ANCIENTTEMPLE", apf.getAppearance("ANCIENTTEMPLE"), 9999, "Ancient Temple");
		
		ret[45] = new Feature("NOTE1", apf.getAppearance("NOTE"), 9999, "Note");
				
		ret[0].setSolid(true);
		ret[0].setDestroyable(true);
		ret[0].setRelevant(false);
		
		ret[11].setRelevant(false);
		ret[11].setKeyCost(1);
		ret[11].setOpaque(true);
		
		ret[14].setRelevant(false);
		
		ret[20].setRelevant(false);
		ret[20].setOpaque(true);
		
		ret[15].setRelevant(false);
		ret[15].setOpaque(true);

		ret[21].setRelevant(false);
		ret[21].setOpaque(true);
		
		ret[10].setEffect("SMALL_KEY");
		ret[12].setEffect("COMPASS");
		ret[13].setEffect("ADD_BOW");
		ret[14].setEffect("BIG_KEY");
		ret[16].setEffect("CANE_BYRNA");
		ret[17].setEffect("CANE_SOMARIA");
		ret[18].setEffect("CANE_YENDOR");
		ret[19].setEffect("CANE_HYLIA");
		ret[20].setEffect("TREASURE_KEY");
		ret[22].setEffect("ADD_SWORD");
		ret[23].setEffect("ADD_BOOMERANG");
		ret[28].setEffect("ADD_HOOKSHOOT");
		ret[29].setEffect("ADD_FLIPPERS");
		ret[37].setEffect("HEART_CONTAINER");
		ret[38].setEffect("ADD_RAFT");
		ret[45].setEffect("NOTE1");
		
		ret[1].setSolid(true);
		ret[1].setDestroyable(true);
		ret[1].setRelevant(false);
		ret[2].setGoldPrize(10);
		ret[3].setGoldPrize(50);
		ret[4].setRelevant(false);
		ret[5].setRelevant(false);
		ret[6].setRelevant(false);
		ret[7].setRelevant(false);
		ret[4].setManaPrize(200);
		ret[5].setHealPrize(200);
		ret[6].setManaPrize(20);
		ret[7].setHealPrize(20);
		ret[8].setEffect("REPAIR");
		ret[9].setEffect("ANKH");
		return ret;
	}
}
