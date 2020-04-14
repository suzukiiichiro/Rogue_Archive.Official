package crl.action;

import java.util.Vector;

import sz.util.Line;
import sz.util.Position;
import crl.feature.Feature;
import crl.level.Level;
import crl.monster.Monster;
import zrl.player.Player;
import crl.ui.effects.EffectFactory;

public abstract class ProjectileSkill extends Action{
	public abstract String getSelfTargettedMessage();
	
	public abstract String getShootMessage();
	
	public abstract int getRange();
	public boolean piercesThru(){
		return true;
	}
	
	public boolean showThrowMessage(){
		return true;
	}
	
	public abstract int getPathType();
	public static final int PATH_DIRECT = 0, PATH_CURVED = 1, PATH_LINEAR = 2;
	
	public abstract String getSFXID(); 
	
	public abstract int getDamage(); 
	
	public abstract String getSpellAttackDesc(); 
	public abstract String getPromptPosition();
	
	public abstract int getHit();
	
	public boolean needsPosition(){
		return true;
	}
	
	private Vector hitMonsters = new Vector(10);
	public Vector getHitMonsters(){
		return hitMonsters;
	}
	public void execute(){
		//super.execute();
		hitMonsters.clear();
        Level aLevel = performer.getLevel();
        Player aPlayer = aLevel.getPlayer();
        int attackHeight = aLevel.getMapCell(aPlayer.getPosition()).getHeight();
        if (targetPosition.equals(performer.getPosition()) && allowsSelfTarget()){
        	aLevel.addMessage(getSelfTargettedMessage());
        	if (getPathType() == PATH_CURVED){
				Feature destinationFeature = aLevel.getFeatureAt(getPlayer().getPosition());
	        	if (destinationFeature != null && destinationFeature.isDestroyable()){
		        	String message = "The " + getSpellAttackDesc() + " hits the "+destinationFeature.getDescription();
					Feature prize = destinationFeature.damage(aPlayer, getDamage());
		        	if (prize != null){
			        	message += " and destroys it.";
					}
					aLevel.addMessage(message);
					return;
				}
    			
    			Monster targetMonster = performer.getLevel().getMonsterAt(getPlayer().getPosition());
    			
    			if (targetMonster != null){
					if (targetMonster.tryMagicHit(aPlayer,getDamage(), getHit(), targetMonster.wasSeen(), getSpellAttackDesc(), isWeaponAttack(), performer.getPosition())){
						hitMonsters.add(targetMonster);
						return;
					};
    			}
        	}
        	return;
        }
	    if (showThrowMessage())
	    	aLevel.addMessage(getShootMessage());
		boolean hit = false;
		Line fireLine = new Line(performer.getPosition(), targetPosition);
		
		boolean curved = false;
		int flyStart = 0, flyEnd = 0;
		if (getPathType() == PATH_CURVED){
			curved = true;
			flyStart =  (int)Math.round(getRange() / 3.0D);
			flyEnd =  (int)Math.round(2* (getRange() / 3.0D));
		}
		int projectileHeight = attackHeight;
		for (int i=0; i<getRange(); i++){
			Position destinationPoint = fireLine.next();
			if (!aLevel.isValidCoordinate(destinationPoint))
				continue;
			if (curved){
				if (i >= flyStart && i <= flyEnd)
					projectileHeight = attackHeight + 1;
				else
					projectileHeight = attackHeight;
			} 
			if (aLevel.isSolid(destinationPoint)){
				if (!piercesThru()){
					drawEffect(EffectFactory.getSingleton().createDirectedEffect(performer.getPosition(), targetPosition, getSFXID(), i));
					return;
				}
			}
			
			String message = "";
			
			int destinationHeight = aLevel.getMapCell(destinationPoint).getHeight();

			if (destinationHeight == projectileHeight){
				Feature destinationFeature = aLevel.getFeatureAt(destinationPoint);
	        	if (destinationFeature != null && destinationFeature.isDestroyable()){
		        	message = "The " + getSpellAttackDesc() + " hits the "+destinationFeature.getDescription();
		        	if (!piercesThru())
		        		drawEffect(EffectFactory.getSingleton().createDirectedEffect(performer.getPosition(), targetPosition, getSFXID(), i));
					Feature prize = destinationFeature.damage(aPlayer, getDamage());
		        	if (prize != null){
			        	message += " and destroys it.";
					}
					aLevel.addMessage(message);
					if (!piercesThru())
						return;
				}
			}
			Monster targetMonster = performer.getLevel().getMonsterAt(destinationPoint);
			
			if (targetMonster != null){
				int monsterHeight = destinationHeight;
				if (projectileHeight == monsterHeight){
					if (targetMonster.tryMagicHit(aPlayer,getDamage(), getHit(), targetMonster.wasSeen(), getSpellAttackDesc(), isWeaponAttack(), performer.getPosition())){
						hit = true;
						hitMonsters.add(targetMonster);
						if (!piercesThru()){
							drawEffect(EffectFactory.getSingleton().createDirectedEffect(aPlayer.getPosition(), targetPosition, getSFXID(), i));
							return;
						}
							
					};
				} else if (projectileHeight < monsterHeight) {
					aLevel.addMessage("The "+getSpellAttackDesc()+" flies under the "+targetMonster.getDescription());
				} else {
					aLevel.addMessage("The "+getSpellAttackDesc()+" flies over the "+targetMonster.getDescription());
				}
			}
		}
		if (!hit || piercesThru())
			drawEffect(EffectFactory.getSingleton().createDirectedEffect(aPlayer.getPosition(), targetPosition, getSFXID(), getRange()));

	}

	public boolean allowsSelfTarget(){
		return true;
	}
	
	public boolean isWeaponAttack(){
		return false;
	}
	
	public Player getPlayer(){
		return (Player) performer;
	}
}