package crl.conf.console.data;

import java.util.Vector;

import sz.csi.ConsoleSystemInterface;
import sz.util.Position;
import crl.ui.consoleUI.effects.CharAnimatedMissileEffect;
import crl.ui.consoleUI.effects.CharBeamMissileEffect;
import crl.ui.consoleUI.effects.CharDirectionalMissileEffect;
import crl.ui.consoleUI.effects.CharEffect;
import crl.ui.consoleUI.effects.CharFlashEffect;
import crl.ui.consoleUI.effects.CharIconEffect;
import crl.ui.consoleUI.effects.CharIconMissileEffect;
import crl.ui.consoleUI.effects.CharMeleeEffect;
import crl.ui.consoleUI.effects.CharSequentialEffect;
import crl.ui.consoleUI.effects.CharSplashEffect;

public class CharEffects {
	private CharEffect [] effects = new CharEffect[]{
		//Animated Missile Effects			
		new CharDirectionalMissileEffect("SFX_FIRE_MISSILE", "\\|/--/|\\", ConsoleSystemInterface.RED, 80), 
		new CharAnimatedMissileEffect("SFX_ICE_BALL", "*", ConsoleSystemInterface.CYAN, 80),
		new CharDirectionalMissileEffect("SFX_LIGHTING", "\\|/--/|\\", ConsoleSystemInterface.CYAN, 80),
		new CharDirectionalMissileEffect("SFX_LIGHT_MISSILE", "\\|/--/|\\", ConsoleSystemInterface.WHITE, 80),
		new CharDirectionalMissileEffect("SFX_MAGIC_MISSILE", "\\|/--/|\\", ConsoleSystemInterface.BLUE, 80),
		new CharDirectionalMissileEffect("SFX_MIND_MISSILE", "\\|/--/|\\", ConsoleSystemInterface.YELLOW, 80),
		
		//Splash Effects
		new CharSplashEffect("SFX_EXPLOTION","*~,",ConsoleSystemInterface.YELLOW,40),
		new CharSplashEffect("SFX_KEGBLAST","*~,",ConsoleSystemInterface.RED,40),
		
		//Melee Effects
		//En MonsterMissile.java  : 45 "SFX_MONSTER_ID_"+aMonster.getID()
		//En Attack.java : 116 "SFX_"+weaponDef.getID()
		
		//Sequential Effects
		
		//Tile Effects
		new CharIconEffect("SFX_PLACEBLAST",'X', ConsoleSystemInterface.RED, 40),
		
				
		//Flash Effects
		new CharFlashEffect("SFX_THUNDER_FLASH", ConsoleSystemInterface.WHITE),
		
		//Weapons
		new CharMeleeEffect("SFX_WP_FIGHTERSWORD","||--/\\\\/", ConsoleSystemInterface.GRAY),
		new CharMeleeEffect("SFX_WP_SWORD","||--/\\\\/", ConsoleSystemInterface.GRAY),
		new CharMeleeEffect("SFX_WP_ENILNO","||--/\\\\/", ConsoleSystemInterface.CYAN),
		new CharMeleeEffect("SFX_WP_GREAT_SWORD","||--/\\\\/", ConsoleSystemInterface.GRAY),
		new CharMeleeEffect("SFX_WP_BATTLE_AXE","||--/\\\\/", ConsoleSystemInterface.GRAY),
		new CharMeleeEffect("SFX_WP_ROPE_SPIKE","||--/\\\\/", ConsoleSystemInterface.BROWN),
		new CharMeleeEffect("SFX_WP_LIGHT_SWORD","||--/\\\\/", ConsoleSystemInterface.CYAN),
		new CharIconMissileEffect("SFX_WP_PISTOL", '\'', ConsoleSystemInterface.GRAY, 40),
		new CharIconMissileEffect("SFX_WP_BOW_ARROWS", '\'', ConsoleSystemInterface.GRAY, 40),
		new CharIconMissileEffect("SFX_WP_RONDORIN_BOW", '\'', ConsoleSystemInterface.GRAY, 40),
		new CharAnimatedMissileEffect("SFX_WP_PHAZER", ".*", ConsoleSystemInterface.CYAN, 40),
		new CharAnimatedMissileEffect("SFX_WP_BLASTER", ".*", ConsoleSystemInterface.RED, 150),
		
		new CharAnimatedMissileEffect("SFX_BOOMERANG", "<V>^", ConsoleSystemInterface.BLUE, 40),
		new CharDirectionalMissileEffect("SFX_WP_BOW", "\\|/--/|\\", ConsoleSystemInterface.GRAY, 40),
		
		new CharAnimatedMissileEffect("SFX_LIGHTBALL", "o-", ConsoleSystemInterface.WHITE, 40),
		new CharAnimatedMissileEffect("SFX_FIREBALL", "o-", ConsoleSystemInterface.RED, 40),
		new CharDirectionalMissileEffect("SFX_SPEAR", "\\|/--/|\\", ConsoleSystemInterface.WHITE, 40),
		new CharDirectionalMissileEffect("SFX_ARROW", "\\|/--/|\\", ConsoleSystemInterface.WHITE, 40),
		
		new CharMeleeEffect("SFX_WP_BLACK_SWORD","||--/\\\\/", ConsoleSystemInterface.RED),
		new CharMeleeEffect("SFX_WP_DWARVEN_AXE","||--/\\\\/", ConsoleSystemInterface.RED),
		
		//Monsters
		new CharMeleeEffect("SFX_SWORD_SLASH","||--/\\\\/", ConsoleSystemInterface.GRAY),
		new CharMeleeEffect("SFX_AXE_SLASH","||--/\\\\/", ConsoleSystemInterface.GRAY),
		new CharAnimatedMissileEffect("SFX_RCP89", ".", ConsoleSystemInterface.GRAY, 20),
		new CharAnimatedMissileEffect("SFX_THROWN_ROCK", "*", ConsoleSystemInterface.GRAY, 120),
		new CharAnimatedMissileEffect("SFX_FIREBALL", "*", ConsoleSystemInterface.RED, 40),
		new CharAnimatedMissileEffect("SFX_MAGIC_MISSILE", "*", ConsoleSystemInterface.BLUE, 60),
		new CharAnimatedMissileEffect("SFX_SLIME_BLOB", "*", ConsoleSystemInterface.GREEN, 100),
		new CharAnimatedMissileEffect("SFX_PHANTOM_STARE", "X", ConsoleSystemInterface.BLUE, 80),
		new CharAnimatedMissileEffect("SFX_SPIT_VENOM", "*", ConsoleSystemInterface.PURPLE, 20),
		new CharDirectionalMissileEffect("SFX_ARROW_SHOT", "\\|/--/|\\", ConsoleSystemInterface.WHITE, 40),
		new CharAnimatedMissileEffect("SFX_ORB_STARE", "*", ConsoleSystemInterface.YELLOW, 60),
		
	};

	public CharEffect[] getEffects() {
		return effects;
	}


}
