package crl.cuts;

import zrl.player.Player;
import crl.cuts.Unleasher;
import crl.game.Game;
import crl.level.Level;
import crl.monster.Monster;
import crl.ui.Display;
import crl.ui.UserInterface;

public class EndOfAdamantCave extends Unleasher{

	public void unleash(Level level, Game game) {
		Monster dracula = level.getMonsterByID("LIGHT_SHADE");
		if (dracula != null)
			return;
		
		Display.thus.showScreen("Suddenly, the White spirit vanishes in a puff of smoke, and you hear a voice... XXX XXX   \"You are brave... Go on and find the three ancient canes if you want to save Hyrule!...\"  XXX XXX XXX You wake up in your house... What is this about? There is only one way to find out...");
		level.getPlayer().informPlayerEvent(Player.EVT_GOTO_LEVEL, "LINKS");
		level.getPlayer().see();
		UserInterface.getUI().refresh();
		
		enabled = false;
	}
}