
package crl.game;

import java.util.Hashtable;

import sz.util.Util;
import zrl.player.Player;
import crl.item.Item;
import crl.item.ItemFactory;
import crl.ui.AppearanceFactory;

public abstract class PlayerGenerator {
	public static PlayerGenerator thus;
	public abstract Player generatePlayer();
	
	private Hashtable SPECIAL_PLAYERS = new Hashtable();
	private void initSpecialPlayers(){
		SPECIAL_PLAYERS.clear();
}
	public Player createSpecialPlayer(String playerID){
		initSpecialPlayers();
		return (Player) SPECIAL_PLAYERS.get(playerID);
	}
	
	public Player getPlayer(String name, int sex){
		Player player = new Player();
		player.setSex(sex);
		if (name.trim().equals("")){
			if (sex == Player.MALE)
				player.setName(Util.randomElementOf(MALE_NAMES));
			else
				player.setName(Util.randomElementOf(FEMALE_NAMES));
		} else {
			player.setName(name);
		}
		
		player.setBaseSightRange(8);
		
		player.setWalkCost(50);
		player.setAttackCost(50);
		player.setCastCost(50);
		player.setCarryMax(20);
		
		player.setHeartsMax(12);
		player.setHearts(12);
		
		player.setMagicMax(9);
		player.setMagic(9);
		
		ItemFactory itf = ItemFactory.getItemFactory();
		AppearanceFactory apf = AppearanceFactory.getAppearanceFactory();
		if (player.getSex()==Player.MALE)
			player.setAppearance(apf.getAppearance("AVATAR"));
		else
			player.setAppearance(apf.getAppearance("AVATAR_F"));
		
		player.setWeapon(itf.createItem("FIGHTERSWORD"));
		player.setArmor(itf.createItem("GREENMAIL"));
		player.setArrows(5);
		player.setSecondaryWeapon(itf.createItem("WARRIOR_SHIELD"));

		return player;

	}
	
	protected String [] MALE_NAMES = new String [] {"Link", "Slash", "Adam", "Bjorn", "Anubis", "Adral"};
	protected String [] FEMALE_NAMES = new String [] {"Zelda","Maiden", "Valentina"};


}
