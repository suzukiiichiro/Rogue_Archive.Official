package crl.cuts;

import zrl.player.Player;
import crl.conf.console.data.CharCuts;
import crl.cuts.Unleasher;
import crl.game.Game;
import crl.game.STMusicManagerNew;
import crl.level.Level;
import crl.monster.Monster;
import crl.ui.Display;
import crl.ui.UserInterface;

public class EndingTrigger extends Unleasher{

	public void unleash(Level level, Game game) {
		Monster dracula = level.getMonsterByID("CHIMERA");
		if (dracula != null)
			return;
		STMusicManagerNew.thus.stopMusic();
		STMusicManagerNew.thus.playKey("VICTORY");
		game.wonGame();
		enabled = false;
	}
}