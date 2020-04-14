package crl.cuts;

import zrl.player.Player;
import crl.cuts.Unleasher;
import crl.feature.Feature;
import crl.game.Game;
import crl.game.STMusicManagerNew;
import crl.level.Level;
import crl.monster.Monster;
import crl.ui.Display;
import crl.ui.UserInterface;

public class EndOfTempleAncient extends Unleasher{

	public void unleash(Level level, Game game) {
		Feature a1 = level.getFeatureByID("ALTARBYRNA");
		Feature a2 = level.getFeatureByID("ALTARSOMARIA");
		Feature a3 = level.getFeatureByID("ALTARYENDOR");
		if (a1 == null && a2 == null && a3 == null) {
			STMusicManagerNew.thus.stopMusic();
			Display.thus.showScreen("The Gate to Byrna has been Opened!");
			STMusicManagerNew.thus.playKey("VICTORY");
			game.wonGame();
			enabled = false;
		}
	}
}