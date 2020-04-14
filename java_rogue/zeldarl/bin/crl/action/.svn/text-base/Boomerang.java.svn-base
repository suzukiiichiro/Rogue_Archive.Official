package crl.action;

import crl.actor.Actor;

public class Boomerang extends ProjectileSkill{
	public int getDamage() {
		// TODO Auto-generated method stub
		return 0;
	}
	
	public int getHit() {
		// TODO Auto-generated method stub
		return 0;
	}
	
	public String getID() {
		return "BOOMERANG";
	}
	
	public int getPathType() {
		return PATH_LINEAR;
	}
	
	public String getPromptPosition() {
		return "Where do you want to throw the boomerang?";
	}
	
	public int getRange() {
		return 6;
	}
	
	public String getSelfTargettedMessage() {
		return "";
	}
	
	public String getSFXID() {
		return "SFX_BOOMERANG";
	}
	
	public String getShootMessage() {
		return "You launch a boomerang!";
	}
	
	public String getSpellAttackDesc() {
		return "boomerang";
	}
	
	public boolean allowsSelfTarget() {
		return false;
	}
	
	public boolean canPerform(Actor a) {
		boolean ret = getPlayer().getFlag("BOOMERANG");
		if (!ret)
			invalidationMessage = "Huh?";
		return ret;
	}
}
