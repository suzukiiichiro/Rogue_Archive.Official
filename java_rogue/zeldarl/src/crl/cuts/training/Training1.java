package crl.cuts.training;

import crl.cuts.Unleasher;
import crl.game.Game;
import crl.level.Level;
import crl.ui.Display;

public class Training1 extends Unleasher {
	public void unleash(Level level, Game game){
		Display.thus.showScreen("Dracula has been defeated, and Christopher Belmont lives a peaceful life with his family; his son Solieyu is growing strong and is about to begin his training.");
		Display.thus.showScreen("Use the directional keys to move, 'L' to look at the different sign posts, '?' for help. Press 'Q' to exit this training.");
		enabled = false;
	}
}
