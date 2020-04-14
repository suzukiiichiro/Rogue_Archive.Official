package crl.monster;

import java.util.Enumeration;
import java.util.Vector;

import sz.csi.textcomponents.ListItem;
import sz.util.*;
import zrl.player.GameSessionInfo;
import zrl.player.Player;

import crl.action.*;
import crl.item.*;
import crl.level.Cell;
import crl.level.Emerger;
import crl.level.EmergerAI;
import crl.level.Level;
import crl.feature.*;
import crl.game.SFXManager;
import crl.ui.*;
import crl.ui.effects.EffectFactory;
//import crl.ui.effects.DoubleSplashEffect;
import crl.ai.*;
import crl.ai.monster.BasicMonsterAI;
import crl.actor.*;

public class Monster extends Actor implements Cloneable{
	//Attributes
	private transient MonsterDefinition definition;
	private String defID;

	private int evadePoints;
	private int evadePointsMax;
	private int evasion;
	private int frozenCounter;
	private int attack, leaping, sightRange;
	private int integrityMax;
	private int integrityPoints;
	
	private String featurePrize;
	private boolean visible = true;
	private boolean wasSeen = false;
	
	public String getWavOnHit(){
		return getDefinition().getWavOnHit();
	}
	
	public void setWasSeen(boolean value){
		wasSeen = true;
	}
	
	public boolean wasSeen(){
		return wasSeen;
	}
	public void updateStatus(){
		Enumeration countersKeys = hashCounters.keys();
		while (countersKeys.hasMoreElements()){
			String key = (String) countersKeys.nextElement();
			Integer counter = (Integer)hashCounters.get(key);
			if (counter.intValue() == 0){
				hashCounters.remove(key);
			} else {
				hashCounters.put(key, new Integer(counter.intValue()-1));
			}
		}
		
		if (isFrozen()){
			frozenCounter--;
		}
		
		if (hasCounter("POISON")){
			if (Util.chance(80)){
				level.addMessage("The "+getDescription()+" is injured by the poison!",getPosition());
				damageIntegrity(1);
			}
		}
		
		
		justEvaded = false;
	}

	public void act(){
		if (isFrozen() || hasCounter("SLEEP") || hasCounter("STUN")){
			setNextTime(50);
			updateStatus();
			return;
		}
		super.act();
		wasSeen = false;
	}

	public boolean canSwim(){
		return false;
	}
	
	public boolean isInWater(){
		if (level.getMapCell(getPosition())!= null)
			return level.getMapCell(getPosition()).isWater();
		else
			return false;
	}

	public boolean isFrozen(){
		return frozenCounter > 0;
	}

	public void freeze(int cont){
		frozenCounter = cont;
	}

	public int getFreezeResistance(){
		return 0; //placeholder
	}

	public Appearance getAppearance(){
		return getDefinition().getAppearance();
	}

	public Object clone(){
		try {
        	return super.clone();
		} catch (Exception x)
		{
			return null;
		}

	}

	public boolean playerInRow(){
		Position pp = level.getPlayer().getPosition();
		/*if (!playerInRange())
			return false;*/
		//Debug.say("pp"+pp);
		//Debug.say(getPosition());
		if (pp.x == getPosition().x || pp.y == getPosition().y)
			return true;
		if (pp.x - getPosition().x == pp.y - getPosition().y)
			return true;
		return false;
	}

	public int starePlayer(){
		/** returns the direction in which the player is seen */
		if (level.getPlayer() == null || level.getPlayer().getPosition().z != getPosition().z)
			return -1;
		if (Position.flatDistance(level.getPlayer().getPosition(), getPosition()) <= getDefinition().getSightRange()){
			Position pp = level.getPlayer().getPosition();
			if (pp.x == getPosition().x){
				if (pp.y > getPosition().y){
					return Action.DOWN;
				} else {
                     return Action.UP;
				}
			} else
			if (pp.y == getPosition().y){
				if (pp.x > getPosition().x){
					return Action.RIGHT;
				} else {
					return Action.LEFT;
				}
			} else
			if (pp.x < getPosition().x){
				if (pp.y > getPosition().y)
					return Action.DOWNLEFT;
				else
					return Action.UPLEFT;
			} else {
                if (pp.y > getPosition().y)
					return Action.DOWNRIGHT;
				else
					return Action.UPRIGHT;
			}
		}
		return -1;
	}
	
	public Monster getNearestMonster(){
		VMonster monsters = level.getMonsters();
		Monster nearMonster = null;
		int minDist = 150;
		for (int i = 0; i < monsters.size(); i++){
			Monster monster = (Monster) monsters.elementAt(i);
			int distance = Position.flatDistance(getPosition(), monster.getPosition());
			if (monster != this && distance < minDist){
				minDist = distance;
				nearMonster = monster;
			}
		}
		return nearMonster;
	}
	
	/** Returns the direction in which the nearest monster is seen */
	public int stareMonster(){
		return stareMonster(getNearestMonster());

	}
	
	public int stareMonster(Monster who){
		if (who == null)
			return -1;
		if (who.getPosition().z != getPosition().z)
			return -1;
		if (Position.flatDistance(who.getPosition(), getPosition()) <= getDefinition().getSightRange()){
			Position pp = who.getPosition();
			if (pp.x == getPosition().x){
				if (pp.y > getPosition().y){
					return Action.DOWN;
				} else {
                     return Action.UP;
				}
			} else
			if (pp.y == getPosition().y){
				if (pp.x > getPosition().x){
					return Action.RIGHT;
				} else {
					return Action.LEFT;
				}
			} else
			if (pp.x < getPosition().x){
				if (pp.y > getPosition().y)
					return Action.DOWNLEFT;
				else
					return Action.UPLEFT;
			} else {
                if (pp.y > getPosition().y)
					return Action.DOWNRIGHT;
				else
					return Action.UPRIGHT;
			}
		}
		return -1;
	}
	
	public void damageIntegrity(Player damager, int damage){
		//return;
		if (isFrozen())
			damage *= 2;
		integrityPoints -= damage;
		
		if (damage <= 0){
			level.addMessage("The "+getDescription()+" shuns the attack!");
			return;
		}
		checkDeath(damager);
	}
	
	public void damageIntegrity(int damage){
		//return;
		integrityPoints -= damage;
		
		if (damage <= 0){
			level.addMessage("The "+getDescription()+" shuns the attack!");
			return;
		}
		if (isDead()){
			level.addMessage("The "+getDescription()+" dies!");
			die();
		}
	}
	
	public void checkDeath(Player damager){
		if (isDead()){
			level.addMessage("The "+getDescription()+" is dead!");
			SFXManager.play("wav/LTTP_Enemy_Kill.wav");
			die();
			if (damager.getGameSessionInfo() != null)
				damager.getGameSessionInfo().addDeath(definition);
			//damager.addScore(definition.getScore());
		}
	}
	

	public int getScore(){
		return getDefinition().getScore();
		
	}
	public boolean isDead(){
		return integrityPoints <= 0;
	}

	public String getDescription(){
	//This may be flavored with specific monster daya
		return getDefinition().getDescription();
	}
	
	public String getExamineDescription(){
		return getDefinition().getDescription() + 
			(hasCounter("CHARM") ? "{Charmed}" : "")+
			(getEnemy() != null ? getEnemy().getDescription() : "");
	}

	private MonsterDefinition getDefinition(){
		if (definition == null){
			definition = MonsterFactory.getFactory().getDefinition(defID);
		}
		return definition;
	}
	
	public boolean hasFireLine(){
		if (Position.flatDistance(getPosition(), level.getPlayer().getPosition()) <= getDefinition().getSightRange()){
			Line sight = new Line(getPosition(), level.getPlayer().getPosition());
			Position point = sight.next();
			while(true){
				//System.out.println("2");
				if (point.equals(level.getPlayer().getPosition()))
					break;
				if (level.isSolid(point) || (level.getMonsterAt(point) != null && level.getMonsterAt(point)!= this)){
					return false;
				}
				
				
				point = sight.next();
			}
			return true;
		} else {
			return false;
		}
	}
	public boolean seesPlayer(){
		//if (wasSeen()){
		if (Position.flatDistance(getPosition(), level.getPlayer().getPosition()) <= getDefinition().getSightRange()){
			Line sight = new Line(getPosition(), level.getPlayer().getPosition());
			Position point = sight.next();
			while(true){
				//System.out.println("2");
				if (level.getMapCell(point).isOpaque()){
					return false;
				}
				if (point.equals(level.getPlayer().getPosition()))
					break;
				point = sight.next();
			}
			return true;
		} else {
			return false;
		}
	}
	
	public boolean isUndead(){
		return getDefinition().isUndead();
	}

	
 	public Monster (MonsterDefinition md){
 		definition = md;
 		defID = md.getID();
 		//selector = md.getDefaultSelector();
 		selector = md.getDefaultSelector().derive();
 		
 		attack = definition.getBaseAttack();
		leaping = definition.getBaseLeaping();
		sightRange = definition.getSightRange();
		integrityPoints = definition.getBaseIntegrityPoints();
		integrityMax = definition.getBaseIntegrityPoints();
		evadePoints = definition.getBaseEvadePoints();
		evadePointsMax = definition.getBaseEvadePoints();
		evasion = definition.getBaseEvasion();
	}

	/*public ActionSelector getSelector(){
		return selector;
		//return definition.getDefaultSelector();
	}*/

	public String getFeaturePrize() {
		return featurePrize;
	}

	public void setFeaturePrize(String value) {
		featurePrize = value;
	}

	
	
	public boolean waitsPlayer(){
		return false;
	}

	/*public ListItem getSightListItem(){
		return definition.getSightListItem();
	}*/

	private void setPrize(){
		Player p = level.getPlayer();
		String [] prizeList = null;
		
        if (Util.chance(50))
	    if (Util.chance(50))
    	if (Util.chance(40))
        if (Util.chance(40))
	    if (Util.chance(40))
	    	prizeList = new String[]{"WHITE_MONEY_BAG"};
		else
			prizeList = new String[]{"POT_ROAST"};
		else
			prizeList = new String[]{"INVISIBILITY_POTION", "ROSARY", "BLUE_MONEY_BAG"};
		else
			prizeList = new String[]{"RED_MONEY_BAG"};
		else
			prizeList = new String[]{"BIGHEART"};
		else
			prizeList = new String[]{"SMALLHEART", "COIN"};    	
    	

		if (prizeList != null)
			setFeaturePrize(Util.randomElementOf(prizeList));
	}
	
	public void die(){
		super.die();
		
		if (getAutorespawncount() > 0){
			Emerger em = new Emerger(MonsterFactory.getFactory().buildMonster(getDefinition().getID()), getPosition(), getAutorespawncount());
			level.addActor(em);
			em.setSelector(new EmergerAI());
			em.setLevel(getLevel());
		}
		Vector drops = definition.getDrops();
		out: for (int i = 0; i < drops.size(); i++){
			Drop drop = (Drop) drops.elementAt(i);
			if (Util.chance(drop.getDropProb())){
				if (drop.getDropType().equals("ITEM")){
					level.addItem(getPosition(), ItemFactory.getItemFactory().createItem(drop.getDropID()));
				} else if (drop.getDropType().equals("FEATURE")){
					level.addFeature(drop.getDropID(), getPosition());
				}
				break out;
			}
		}
		level.removeMonster(this);
	}
	
	public void setVisible(boolean value){
		visible = value;
	}
	
	public boolean isVisible(){
		return visible;
	}
	
	public int getAttackCost() {
		return getDefinition().getAttackCost();
	}

	public int getWalkCost() {
		return getDefinition().getWalkCost();
	}
	
	public String getID(){
		return getDefinition().getID();
	}
	
	public int getEvadeChance(){
		return getDefinition().getEvadeChance();
	}
	
	public String getEvadeMessage(){
		return getDefinition().getEvadeMessage();
	}
	
	public int getAutorespawncount(){
		return getDefinition().getAutorespawnCount();
	}
	
	private int getEvasion(){
		return evasion;
	}
	
	public boolean tryMagicHit(Player attacker, int magicalDamage, int magicalHit, boolean showMsg, String attackDesc, boolean isWeaponAttack, Position attackOrigin){
		int hitChance = 100 - getEvadeChance();
		hitChance = (int)Math.round((hitChance + magicalHit)/2.0d);
		int penalty = 0;
		if (isWeaponAttack){
			penalty = (int)(Position.distance(getPosition(), attackOrigin)/4);
			if (attacker.getWeapon().isHarmsUndead() && isUndead())
				magicalDamage *= 2;
		}
			
		magicalDamage -= penalty;
		int evasion = 100 - hitChance;
			
		if (evasion < 0)
			evasion = 0;
		
		if (hasCounter("CHARM"))
			setCounter("CHARM", 0);
		if (hasCounter("SLEEP"))
			evasion = 0;
		//see if evades it
		if (Util.chance(evasion)){
			if (showMsg)
				level.addMessage("The "+getDescription()+" evades the "+attackDesc+"!");
			//moveRandomly();
			return false;
		} else {
			if (hasCounter("SLEEP")){
				level.addMessage("You wake up the "+getDescription()+ "!");
				setCounter("SLEEP", 0);
			}
			int baseDamage = magicalDamage;
			double damageMod = 1;
			String hitDesc = "";
			int damage = (int)(baseDamage * damageMod);
			double percent = (double)damage/(double)getDefinition().getBaseIntegrityPoints();
			if (percent > 1.0d)
				hitDesc = "The "+attackDesc+ " whacks the "+getDescription()+ " appart!!";
			else if (percent > 0.7d)
				hitDesc = "The "+attackDesc+ " smashes the "+getDescription()+ "!";
			else if (percent > 0.5d)
				hitDesc = "The "+attackDesc+ " grievously hits the "+getDescription()+ "!";
			else if (percent> 0.3d)
				hitDesc = "The "+attackDesc+ " hits the "+getDescription()+ ".";
			else
				hitDesc = "The "+attackDesc+ " barely scratches the "+getDescription()+ "...";
			if (showMsg)
				level.addMessage(hitDesc);
			damageIntegrity((int)(baseDamage*damageMod));
			//attacker.setLastWalkingDirection(Action.SELF);
			return true;
		}
	}
	
	
	public boolean tryHit(Player attacker, Item weapon, boolean cornered, boolean cinetic, int direction){
		boolean isValidCharge = false;
		if (attacker.isCharging()){
			Position attack = Action.directionToVariation(direction);
			Position previous = Action.directionToVariation(attacker.getLastWalkingDirection());
			isValidCharge = (attack.x == previous.x && Math.abs(attack.y - previous.y) < 2) || 
							(attack.y == previous.y && Math.abs(attack.x - previous.x) < 2);
		}
		
		if (hasCounter("CHARM"))
			setCounter("CHARM", 0);
		
		int weaponAttack = attacker.getAttackBonus();
		if (weapon != null)
			weaponAttack = weapon.getAttack();
		
		attacker.doHit(true);
		if (hasCounter("SLEEP")){
			level.addMessage("You wake up the "+getDescription()+ "!");
			setCounter("SLEEP", 0);
		}
		if (isValidCharge)
			level.addMessage("You charge against the "+getDescription()+ "!");
		/*else
			level.addMessage("The "+getDescription()+ " is hit!");*/
		int baseDamage = weaponAttack;
		double damageMod = 1 +
			(isValidCharge ? 2 : 0)+
			(cornered && attacker.getFlag("COMBAT_CORNER")? 1 : 0)+
			
			(cinetic ? 0.5d:0)+
			(attacker.hasStoredEnergy() ? 0.5d:0);
		 
		String hitDesc = "";
		int damage = (int)(baseDamage * damageMod);
		double percent = (double)damage/(double)getDefinition().getBaseIntegrityPoints();
		if (percent > 1.0d)
			hitDesc = "You whack the "+getDescription()+ " appart!!";
		else if (percent > 0.7d)
			hitDesc = "You smash the "+getDescription()+ "!";
		else if (percent > 0.5d)
			hitDesc = "You grievously hit the "+getDescription()+ "!";
		else if (percent> 0.3d)
			hitDesc = "You hit the "+getDescription()+ ".";
		else
			hitDesc = "You barely scratch the "+getDescription()+ "...";
		level.addMessage(hitDesc);
		if (attacker.getFlag("COMBAT_STUN") && Util.chance(30) && !hasCounter("STUN")){
			level.addMessage("The "+getDescription()+" is stunned!");
			setCounter("STUN", 4);
		}
		SFXManager.play("wav/LTTP_Enemy_Hit.wav");
		damageIntegrity(attacker, (int)(baseDamage*damageMod));
		
		//level.addMessage("Damage Base: "+baseDamage);
		//level.addMessage("Damage Mod: "+damageMod);
		attacker.setLastWalkingDirection(Action.SELF);
		return true;
		
	}
	
	private Monster enemy;
	
	public Monster getEnemy(){
		return enemy;
	}
	
	public void setEnemy(Monster who){
		enemy = who;
	}
	
	public boolean tryHit(Monster attacker){
		return tryHit(attacker, attacker.getDefinition().getBaseAttack());
	}
	
	public boolean tryHit(Monster attacker, int weaponAttack){
		setEnemy(attacker);
		int evasion = getEvasion();
		//level.addMessage("Evasion "+evasion);
		if (hasCounter("SLEEP"))
			evasion = 0;
		//level.addMessage("Evasion "+evasion);
		//see if evades it
		int evadeBreak = attacker.getDefinition().getBaseBreak();
		//int weaponAttack = attacker.getDefinition().getBaseAttack();
		if (evadePoints > 0 && Util.chance(evasion)){
			level.addMessage("The "+getDescription()+ " dodges the "+attacker.getDescription()+" attack!");
			evadePoints -= evadeBreak;
			if (evadePoints < 0)
				evadePoints = 0;
			moveRandomly();
			return false;
		} else {
			if (hasCounter("SLEEP")){
				level.addMessage("The "+attacker.getDescription()+" wakes up the "+getDescription()+ "!");
				setCounter("SLEEP", 0);
			}
			int baseDamage = weaponAttack;
			double damageMod = 1 ;
			 
			String hitDesc = "";
			int damage = (int)(baseDamage * damageMod);
			double percent = (double)damage/(double)getDefinition().getBaseIntegrityPoints();
			if (percent > 1.0d)
				hitDesc = "The "+attacker.getDescription()+" whacks the "+getDescription()+ " appart!!";
			else if (percent > 0.7d)
				hitDesc = "The "+attacker.getDescription()+" smashes the "+getDescription()+ "!";
			else if (percent > 0.5d)
				hitDesc = "The "+attacker.getDescription()+" grievously hits the "+getDescription()+ "!";
			else if (percent> 0.3d)
				hitDesc = "The "+attacker.getDescription()+" hits the "+getDescription()+ ".";
			else
				hitDesc = "The "+attacker.getDescription()+" barely scratches the "+getDescription()+ "...";
			level.addMessage(hitDesc);
			damageIntegrity((int)(baseDamage*damageMod));
			return true;
		}
	}
	
	private void moveRandomly(){
		int direction = Util.rand(0,7);
		Position var = Action.directionToVariation(direction);
		Position dest = Position.add(getPosition(), var);
		if (level.isWalkable(dest) && level.getActorAt(dest)== null && !level.getPlayer().getPosition().equals(dest))
			//setPosition(dest);
			tryLand(dest);
		justEvaded = true;
	}
	private boolean justEvaded;
	
	public boolean justEvaded(){
		return justEvaded;
	}
	
	public int getBreak(){
		return definition.getBaseBreak();
	}
	
	public int getDamage(){
		return definition.getBaseAttack();
	}
	
	public boolean canBeVanished(){
		return true;
	}
	
	public void tryLand(Position destinationPoint){
		Level aLevel = getLevel();
		Cell destinationCell = aLevel.getMapCell(destinationPoint);
        //boolean destinationWalkable = aLevel.isWalkable(destinationPoint);
		boolean destinationWalkable = aLevel.isValidCoordinate(destinationPoint) && !aLevel.isSolid(destinationPoint) /*&& !aLevel.isDoor(destinationPoint)*/;
        Cell currentCell = aLevel.getMapCell(getPosition());
        /*if (currentCell.isWater() && isFloating())
        	destinationWalkable = true;*/
        Monster destinationMonster = aLevel.getMonsterAt(destinationPoint);
        Feature destinationFeature = aLevel.getFeatureAt(destinationPoint);
        SmartFeature standing = aLevel.getSmartFeature(getPosition());
        
        if (standing != null) {
        	if (standing.getEffectOnStep() != null){
		        String[] effects = standing.getEffectOnStep().split(" ");
		        if (effects[0].equals("TRAP") && this != aLevel.getBoss()){
			        aLevel.addMessage("The "+getDescription()+" is trapped!");
		        	return;
		        }
		    }
        }
        
        if (destinationFeature != null && destinationFeature.isSolid()){
        	if (wasSeen())
        		aLevel.addMessage("The "+getDescription()+" hits the "+destinationFeature.getDescription());
        	destinationFeature.damage(this);
        }
        
        if (destinationCell == null || !destinationWalkable) 
        	if (getSelector() instanceof BasicMonsterAI)
        		if (((BasicMonsterAI)getSelector()).getPatrolRange() > 0)
        			((BasicMonsterAI)getSelector()).setChangeDirection(true);
        
        if (destinationCell != null){
        	if (destinationFeature != null && !destinationWalkable){
        		
        	}else {
				if (destinationWalkable) {
					if (destinationMonster == null) {
						if (currentCell == null || destinationCell.getHeight() <= currentCell.getHeight() +1)
							if (aLevel.getPlayer().getPosition().equals(destinationPoint)){
								aLevel.getPlayer().tryHit(this);
							} else {
								setPosition(destinationPoint);
								if (destinationCell.getDamageOnStep() > 0){
									if (!(getDefinition().isFireResistant() || getDefinition().isFloating())){
										getLevel().addMessage("The "+getDescription()+" is injured by the "+destinationCell.getDescription(), getPosition());
										damageIntegrity(destinationCell.getDamageOnStep());
									}
								}
							}
					} else {
						if (hasCounter("CHARM") || getEnemy() == destinationMonster){
							destinationMonster.tryHit(this);
						}
					}
				}
        	}
        }
		
	}
	
	public boolean isFloating(){
		return getDefinition().isFloating();
	}
	
	public boolean isFireResistant(){
		return getDefinition().isFireResistant();
	}
	
	public boolean isHeavy(){
		return getDefinition().isHeavy();
	}
	
	public boolean isMagus(){
		return getDefinition().isMagus();
	}
	
	public boolean isBoss(){
		return getDefinition().isBoss();
	}
}