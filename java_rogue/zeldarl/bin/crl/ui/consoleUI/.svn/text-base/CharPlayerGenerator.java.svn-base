package crl.ui.consoleUI;

import sz.csi.CharKey;
import sz.csi.ConsoleSystemInterface;
import sz.csi.textcomponents.TextBox;
import zrl.player.Player;
import crl.game.PlayerGenerator;
import crl.ui.AppearanceFactory;
import crl.ui.Display;

public class CharPlayerGenerator extends PlayerGenerator{
	public CharPlayerGenerator(ConsoleSystemInterface si){
		this.si = si;
	}
	private ConsoleSystemInterface si;
	
	public Player generatePlayer(){
		si.cls();
		si.print(2,3, "Hero name:", ConsoleSystemInterface.WHITE);
		si.refresh();
		si.locateCaret(3 +"Hero name:".length(), 3);
		String name = si.input(10);
		si.print(2,4, "Sex: [m/f]", ConsoleSystemInterface.WHITE);
		si.refresh();
		CharKey x = new CharKey(CharKey.NONE);
		while ( x.code != CharKey.M &&
				x.code != CharKey.m &&
				x.code != CharKey.F &&
				x.code != CharKey.f)
			x = si.inkey();
		int sex = 0;
		if (x.code == CharKey.M || x.code == CharKey.m)
			sex = Player.MALE;
		else
			sex = Player.FEMALE;
		
    	return getPlayer(name, sex);
	}
}