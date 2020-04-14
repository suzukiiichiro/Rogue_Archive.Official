package crl.feature.ai;

import sz.util.Position;
import sz.util.Util;
import zrl.player.Player;
import crl.action.*;
import crl.ai.*;
import crl.actor.*;
import crl.feature.SmartFeature;
import crl.game.SFXManager;
import crl.monster.Monster;
import crl.monster.VMonster;
import crl.ui.effects.EffectFactory;

public class KegSelector implements ActionSelector, Cloneable{
	public String getID(){
	     return "POWDER_KEG";
	}
	private final static int KEGRANGE = 3;
	int turns = -1;
	public Action selectAction(Actor who){
		
		if (turns == -1)
			turns = 4;
		if (turns == 0){
			who.getLevel().addMessage("The powder keg explodes!!");
			who.getLevel().addEffect(EffectFactory.getSingleton().createLocatedEffect(who.getPosition(), "SFX_KEGBLAST"));
			int xstart = who.getPosition().x - KEGRANGE;
			int ystart = who.getPosition().y - KEGRANGE;
			int xend = who.getPosition().x + KEGRANGE;
			int yend = who.getPosition().y + KEGRANGE;
			if (xstart < 0) xstart = 0;
			if (ystart < 0) ystart = 0;
			if (xend > who.getLevel().getWidth()-2) xend = who.getLevel().getWidth()-2;
			if (yend > who.getLevel().getHeight()-2) yend = who.getLevel().getHeight()-2;
			for (int xrun = xstart; xrun <= xend; xrun++)
				for (int yrun = ystart; yrun <= yend; yrun++){
					Monster targetMonster = who.getLevel().getMonsterAt(xrun, yrun, 0);
					if (targetMonster != null){
						who.getLevel().addMessage("The "+targetMonster.getDescription()+" is hit by the explosion!");
						targetMonster.damageIntegrity(Util.rand(10, 15));
					}
					if (who.getLevel().getMapCell(xrun, yrun, 0).isSolid())
						if (Util.chance(70))
							who.getLevel().transformCell(new Position(xrun, yrun, 0), "CAVE_FLOOR");
					if (who.getLevel().getPlayer().getPosition().x == xrun && who.getLevel().getPlayer().getPosition().y == yrun){
						who.getLevel().addMessage("You are hit by the explosion!");
						who.getLevel().getPlayer().selfDamage(Player.DAMAGE_KEG, Util.rand(10,15));
						
					}
					
				}
			who.getLevel().removeSmartFeature((SmartFeature)who);
			who.die();
		}
		
		turns --;
		
		return null;
 	}

 	public ActionSelector derive(){
 		try {
	 		return (ActionSelector) clone();
	 	} catch (CloneNotSupportedException cnse){
			return null;
	 	}
 	}

}