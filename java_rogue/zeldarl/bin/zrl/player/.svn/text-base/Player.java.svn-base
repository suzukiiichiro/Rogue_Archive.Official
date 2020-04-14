package zrl.player;

import sz.fov.FOV;
import sz.util.*;

import java.util.*;

import crl.item.*;
import crl.action.*;
import crl.actor.*;
import crl.monster.*;
import crl.ui.*;
import crl.level.*;
import crl.feature.*;
import crl.game.Game;
import crl.game.SFXManager;


public class Player extends Actor {
	private Game game;
	private boolean doNotRecordScore = false;
	public final static int
	SEX_MALE = 1,
	SEX_FEMALE = 2;
	
	private String name;
	private int sex;
	
	private int baseSightRange;
	private int heartsMax;
	private int hearts;
	private int magic;
	private int magicMax;
	
	private int carryMax;
	
	private int walkCost = 50;
	private int attackCost = 50;
	private int castCost = 50;
	
	private GameSessionInfo gameSessionInfo;
	
	private int smallKeys;
	
	private int arrows;
	
	//Status Auxiliars
	
	private Hashtable hashFlags = new Hashtable();
	public int getArrows() {
		return arrows;
	}

	public void setArrows(int arrows) {
		this.arrows = arrows;
	}

	public int getSmallKeys() {
		return smallKeys;
	}

	public void setSmallKeys(int smallKeys) {
		this.smallKeys = smallKeys;
	}

	public void setFlag(String flagID, boolean value){
		hashFlags.put(flagID, new Boolean(value));
	}
	
	public boolean getFlag(String flagID){
		Boolean val =(Boolean)hashFlags.get(flagID); 
		return val != null && val.booleanValue();
	}
	
	
	//Relationships
	private transient PlayerEventListener playerEventListener;

	public void addHistoricEvent(String description){
		gameSessionInfo.addHistoryItem(description);
	}
	
	private int prayerCount;
	
	public void informPlayerEvent(int code){
		if (playerEventListener != null)
			playerEventListener.informEvent(code);
	}

	public void informPlayerEvent(int code, Object param){
		playerEventListener.informEvent(code, param);
	}
	
	
	public void selfDamage(int damageType, int dam){
		damageIntegrity(dam);
		if (isDead()){
			switch (damageType){
				case DAMAGE_WALKED_ON_LAVA:
					gameSessionInfo.setDeathCause(GameSessionInfo.BURNED_BY_LAVA);
					break;
				case DAMAGE_SHOCKED:
					gameSessionInfo.setDeathCause(GameSessionInfo.SHOCKED);
					break;
				case DAMAGE_POISON:
					gameSessionInfo.setDeathCause(GameSessionInfo.POISONED_TO_DEATH);
					break;
				case DAMAGE_LIFEBURST:
					gameSessionInfo.setDeathCause(GameSessionInfo.LIFEBURST);
					break;
				case DAMAGE_KEG:
					gameSessionInfo.setDeathCause(GameSessionInfo.KEG);
					break;
			}
			gameSessionInfo.setDeathLevel(getLevel().getLevelNumber());
			gameSessionInfo.setDeathLevelDescription(getLevel().getDescription());
			informPlayerEvent (DEATH);
		}

	}

	public void checkDeath(){
		if (isDead()) {
			SFXManager.play("wav/LTTP_Link_Dying.wav");
			
			informPlayerEvent (DEATH);
		}
	}

	private Hashtable inventory = new Hashtable();

	public String getSecondaryWeaponDescription(){
		if (getSecondaryWeapon() != null)
			return getSecondaryWeapon().getAttributesDescription();
		else
			return "";
	}
	
	public String getEquipedWeaponDescription(){
		if (weapon != null)
			return weapon.getAttributesDescription();
		else
			return "Nothing";
	}

	public String getArmorDescription(){
		if (armor != null)
			return armor.getAttributesDescription();
		else
			return "Nothing";
	}

	public String getAccDescription(){
		if (accesory == null)
			return "Nothing";
		else
			return accesory.getAttributesDescription();
	}

	public void addItem(Item toAdd){
		if (!canCarry()){
			if (level != null){
				level.addMessage("You can't carry anything more");
				level.addItem(getPosition(), toAdd);
			}
			return;
		}
		
		String toAddID = toAdd.getFullID();
		Equipment equipmentx = (Equipment) inventory.get(toAddID);
		if (equipmentx == null)
			inventory.put(toAddID, new Equipment(toAdd, 1));
		else
			equipmentx.increaseQuantity();
		
		
	}

	private Item weapon;
	private Item secondaryWeapon;
	private Item armor;
	private Item accesory;

	public int getItemCount(){
		int eqCount = 0;
		Enumeration en = inventory.elements();
		while (en.hasMoreElements())
			eqCount += ((Equipment)en.nextElement()).getQuantity();
		return eqCount;
	}
	public boolean canCarry(){
		return getItemCount() < carryMax;
		//return true;
	}

	private void removeItem(Equipment toRemove){
		
		inventory.remove(toRemove.getItem().getFullID());
	}
	
	public boolean hasItem (Item item){
		return inventory.containsKey(item.getFullID());
	}

	/*public void removeItem(Item toRemove){
		inventory.remove(toRemove.getDefinition().getID());
	}*/

	public Vector getInventory(){
		Vector ret = new Vector();
		Enumeration x = inventory.elements();
		while (x.hasMoreElements()){
			ret.add(x.nextElement());
		}
		return ret;
	}


	public String getName() {
		return name;
	}

	private String classString;
	public String getClassString(){
		return classString;
	}

	public void setName(String value) {
		name = value;
	}

	public PlayerEventListener getPlayerEventListener() {
		return playerEventListener;
	}

	public void setPlayerEventListener(PlayerEventListener value) {
		playerEventListener = value;
	}

	public void bounceBack(Position variation, int dep){
		if (level.getMapCell(getPosition()).isStair() || isInvincible() )
			return;
		out: for (int run = 0; run < dep; run++){
			Position landingPoint = Position.add(getPosition(), variation);
			Cell landingCell = getLevel().getMapCell(landingPoint);
			
			if (landingCell == null){
				if (run < dep-1){
					continue out;
				} else {
					if (!level.isValidCoordinate(landingPoint))
						break out;
					landingPoint = level.getDeepPosition(landingPoint);
					if (landingPoint == null){
						level.addMessage("You are thrown into a endless pit!");
						gameSessionInfo.setDeathCause(GameSessionInfo.ENDLESS_PIT);
						hearts = -1;
						informPlayerEvent(Player.DEATH);
						return;
					} else {
						landOn(landingPoint);
						break out;
					}
				}
			}
			
			if (landingCell != null && !landingCell.isSolid()
			 && landingCell.getHeight() <= getLevel().getMapCell(getPosition()).getHeight())
				setPosition(landingPoint);
			else
				break;
		}
	}

	public GameSessionInfo getGameSessionInfo() {
		return gameSessionInfo;
	}

	public void setGameSessionInfo(GameSessionInfo value) {
		gameSessionInfo = value;
	}

	public int getSex() {
		return sex;
	}

	public void setSex(int value) {
		sex = value;
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
		
		if (!isRunning){
			setLastWalkingDirection(Action.SELF);
			resetChargeCounter();
			//setIsDodging(false);
		}
		if (!isAttacking){
			consecutiveHits = 0;
			lastAttackDirection = Action.SELF;
		}
		isRunning = false;
		isAttacking = false;
		
    	if (hasIncreasedJumping()) jumpingCounter--;
    	if (isInvincible()) invincibleCount--;
    	if (isPoisoned()){
    		if (Util.chance(50)){
    			selfDamage(DAMAGE_POISON, 3);
    		}
    		if (!isPoisoned())
    			level.addMessage("The poison leaves your blood.");
    	}
    	
    	if (isPoisoned()){
    		if (Util.chance(40)){
    			level.addMessage("You feel the poison coursing through your veins!");
    			selfDamage(Player.DAMAGE_POISON, 3);
    		}
    	}
        
    	
	}
	
	public void act(){
		if (hasCounter("PARALIZED")){
			//if (Util.chance(40)){
				level.addMessage("You cannot move!");
				updateStatus();
			/*}
			else
				super.act();*/
		} else {
			setPreviousPosition(getPosition());
			super.act();
		}
		 
	}

	public void land(){
		landOn (getPosition());
	}

	private Position getFreeSquareAround(Position destinationPoint){
		Position tryP = Position.add(destinationPoint, Action.directionToVariation(Action.UP));
		if (level.getMapCell(tryP) != null && !level.getMapCell(tryP).isSolid()){
			return tryP;
		} 
		
		tryP = Position.add(destinationPoint, Action.directionToVariation(Action.DOWN));
		if (level.getMapCell(tryP) != null && !level.getMapCell(tryP).isSolid()){
			return tryP;
		}
		
		tryP = Position.add(destinationPoint, Action.directionToVariation(Action.LEFT));
		if (level.getMapCell(tryP) != null && !level.getMapCell(tryP).isSolid()){
			return tryP;
		}
					
		tryP = Position.add(destinationPoint, Action.directionToVariation(Action.RIGHT));
		if (level.getMapCell(tryP) != null && !level.getMapCell(tryP).isSolid()){
			return tryP;
		}
		return null;
	}
		
	public void landOn (Position destinationPoint){
		Debug.enterMethod(this, "landOn", destinationPoint);
		Cell destinationCell = level.getMapCell(destinationPoint);
		
        if (destinationCell == null){
        	destinationPoint = level.getDeepPosition(destinationPoint);
        	if (destinationPoint == null) {
        		level.addMessage("You fall into a endless pit!");
				gameSessionInfo.setDeathCause(GameSessionInfo.ENDLESS_PIT);
				hearts = -1;
				informPlayerEvent(Player.DEATH);
				Debug.exitMethod();
				return;
        	}else {
        		destinationCell = level.getMapCell(destinationPoint);
        	}
        }
        
        setPosition(destinationPoint);
        
		if (destinationCell.getDamageOnStep() > 0){
			if (!isInvincible()){
				level.addMessage("You are injured by the "+destinationCell.getShortDescription()+"!");
				selfDamage(Player.DAMAGE_WALKED_ON_LAVA, 2);
			}
		}

		if (destinationCell.getHeightMod() != 0){
			setPosition(Position.add(destinationPoint, new Position(0,0, destinationCell.getHeightMod())));
		}
		
		

		Vector destinationItems = level.getItemsAt(destinationPoint);
		if (destinationItems != null){
			if (destinationItems.size() == 1)
				level.addMessage("There is a "+((Item)destinationItems.elementAt(0)).getDescription()+" here");
			else 
				level.addMessage("There are several items here");
		}
		Feature destinationFeature = level.getFeatureAt(destinationPoint);
		while (destinationFeature != null){
			if (destinationFeature.getHeightMod() != 0){
				setPosition(Position.add(destinationPoint, new Position(0,0, destinationFeature.getHeightMod())));
			}
			if (destinationFeature.getScorePrize() > 0){
				level.addMessage("You pickup the "+destinationFeature.getDescription()+".");
				level.destroyFeature(destinationFeature);
			}
			
			if (destinationFeature.getGoldPrize() > 0){
				level.addMessage("You pickup the "+destinationFeature.getDescription()+".");
				SFXManager.play("wav/LTTP_Rupee1.wav");
				
				gold += destinationFeature.getGoldPrize();
				level.destroyFeature(destinationFeature);
			}
			if (destinationFeature.getHealPrize() > 0){
				level.addMessage("You feel much better!");
				SFXManager.play("wav/LTTP_Item.wav");
				recoverHearts(destinationFeature.getHealPrize());
				level.destroyFeature(destinationFeature);
			}
			if (destinationFeature.getManaPrize() > 0){
				level.addMessage("You feel power running into your body!");
				recoverMagic(destinationFeature.getManaPrize());
				level.destroyFeature(destinationFeature);
			}
			if (destinationFeature.getID().equals("ALTARBYRNA") && getFlag("CANE_BYRNA")){
				level.addMessage("You put the cane of Byrna on the altar!");
				level.destroyFeature(destinationFeature);
				level.addFeature("ALTARBYRNA_X", destinationFeature.getPosition());
			}
			if (destinationFeature.getID().equals("ALTARSOMARIA") && getFlag("CANE_SOMARIA")){
				level.addMessage("You put the cane of Somaria on the altar!");
				level.destroyFeature(destinationFeature);
				level.addFeature("ALTARSOMARIA_X", destinationFeature.getPosition());
			}
			if (destinationFeature.getID().equals("ALTARYENDOR") && getFlag("CANE_YENDOR")){
				level.addMessage("You put the cane of Yendor on the altar!");
				level.destroyFeature(destinationFeature);
				level.addFeature("ALTARYENDOR_X", destinationFeature.getPosition());
			}
			
			if (destinationFeature.getEffect() != null){
				
				if (destinationFeature.getEffect().equals("SMALL_KEY")){
					setSmallKeys(getSmallKeys()+1);
					level.addMessage("You get a small key!");
					SFXManager.play("wav/LTTP_Get_Key.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("COMPASS")){
					setFlag("COMPASS"+level.getID(), true);
					level.addMessage("You get the compass!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("ADD_BOW")){
					setFlag("ITEM_BOW", true);
					level.addMessage("You get the bow!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("BIG_KEY")){
					setFlag("BIGKEY"+level.getID(), true);
					level.addMessage("You get the master key!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("TREASURE_KEY")){
					setFlag("TREASURE_KEY"+level.getID(), true);
					level.addMessage("You get the treasure key!");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("ADD_SWORD")){
					setFlag("SWORD", true);
					level.addMessage("You get the sword!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("ADD_BOOMERANG")){
					setFlag("BOOMERANG", true);
					level.addMessage("You get the boomerang!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("ADD_HOOKSHOOT")){
					setFlag("HOOKSHOOT", true);
					level.addMessage("You get the hookshoot!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("ADD_FLIPPERS")){
					setFlag("FLIPPERS", true);
					level.addMessage("You get the flippers!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("CANE_BYRNA")){
					setFlag("CANE_BYRNA", true);
					level.addMessage("You get The Cane of Byrna!!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("CANE_SOMARIA")){
					setFlag("CANE_SOMARIA", true);
					level.addMessage("You get The Cane of Somaria!!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("CANE_YENDOR")){
					setFlag("CANE_YENDOR", true);
					level.addMessage("You get The Cane of Yendor!!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("CANE_HYLIA")){
					setFlag("CANE_HYLIA", true);
					level.addMessage("You get The Cane of Hylia!!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("HEART_CONTAINER")){
					setHeartsMax(getHeartsMax()+2);
					setHearts(getHeartsMax());
					level.addMessage("You get a heart container!!");
					SFXManager.play("wav/LTTP_Get_HeartContainer.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("ADD_RAFT")){
					setFlag("RAFT", true);
					level.addMessage("You get the raft!");
					SFXManager.play("wav/LTTP_ItemFanfare.wav");
					level.destroyFeature(destinationFeature);
				}else if (destinationFeature.getEffect().equals("NOTE1")){
					level.addMessage("You read the note: \"I will be here, waiting for you.\"");
					level.destroyFeature(destinationFeature);
				}
			}
			
			Feature pred = destinationFeature;
			destinationFeature = level.getFeatureAt(destinationPoint);
			if (destinationFeature == pred)
				destinationFeature = null;
		}
		if (level.isExit(getPosition())){
			String exit = level.getExitOn(getPosition());
			if (exit.equals("_START")){
				//Do nothing. This must be changed with startsWith("_");
			} else if (exit.equals("_NEXT")){
				informPlayerEvent(Player.EVT_NEXT_LEVEL);
			} else if (exit.equals("_BACK")){
				//informPlayerEvent(Player.EVT_BACK_LEVEL);
			} else {
				informPlayerEvent(Player.EVT_GOTO_LEVEL, exit);
			}
			
		}
		Debug.exitMethod();
	}

	public boolean isPoisoned(){
		return getCounter("POISON") > 0;
	}
	
	private int invincibleCount;

	public boolean isInvincible(){
		return invincibleCount > 0;
	}

	public void setInvincible(int counter) {
		invincibleCount = counter;
	}

	private int jumpingCounter;

	public void increaseJumping(int counter) {
		jumpingCounter = counter;
    }

    public boolean hasIncreasedJumping(){
    	return jumpingCounter > 0;
    }

	public void reduceQuantityOf(Item what){
		String toAddID = what.getFullID();
		//System.out.println("ID "+toAddID);
		Equipment equipment = (Equipment)inventory.get(toAddID);
		equipment.reduceQuantity();
		if (equipment.isEmpty())
			removeItem(equipment);
	}

	public void setCarryMax(int value) {
		carryMax = value;
	}

    public Appearance getAppearance(){
		Appearance ret = super.getAppearance();
		if (ret == null){
			if (getSex()==Player.MALE)
				setAppearance(AppearanceFactory.getAppearanceFactory().getAppearance("AVATAR"));
			else
				setAppearance(AppearanceFactory.getAppearanceFactory().getAppearance("AVATAR_F"));
			ret = super.getAppearance();
		}
		return ret; 
    }

	private Vector availableSkills = new Vector(10);

	public synchronized Vector getAvailableSkills(){
		availableSkills.removeAllElements();
		if (getFlag("CANE_BYRNA")){
			availableSkills.add(allSkills.get("BYRNA"));
		}
		if (getFlag("CANE_SOMARIA")){
			availableSkills.add(allSkills.get("SOMARIA"));
		}
		if (getFlag("CANE_YENDOR")){
			availableSkills.add(allSkills.get("YENDOR"));
		}
		return availableSkills;
	}

	public String getWeaponDescription(){
		if (getWeapon() != null)
			if (getWeapon().getReloadTurns() > 0)
				return getWeapon().getDescription()+"("+getWeapon().getRemainingTurnsToReload()+")";
			else
				return getWeapon().getDescription();
		else
			return "None";

	}

	private Hashtable allSkills = new Hashtable();{
			allSkills.put("BYRNA", new Skill("Protection of Byrna", new Byrna(), 3));
			allSkills.put("SOMARIA", new Skill("Relief of Somaria", new Somaria(), 3));
			allSkills.put("YENDOR", new Skill("Stop of Yendor", new Yendor(), 3));
	};
	public final static int DEATH = 0, WIN = 1, DROWNED = 2, KEYINMINENT = 3;

	public final static int
		EVT_FO23RWARD = 7,
		EVT_RE23TURN = 8, 
		EVT_MERCHANT = 9, 
		EVT_SMASHED = 10, 
		EVT_CHAT = 11, 
		EVT_LEVELUP = 12,
		EVT_NEXT_LEVEL = 13,
		EVT_BACK_LEVEL = 14,
		EVT_GOTO_LEVEL = 15;
	
	public final static int MALE = 1, FEMALE = 2;



	public final static int
		DAMAGE_MORPHED_WITH_STRONG_ARMOR = 0,
		DAMAGE_WALKED_ON_LAVA = 1,
		DAMAGE_USING_ITEM = 2,
		DAMAGE_POISON = 3,
		DAMAGE_JINX = 4,
		DAMAGE_SHOCKED = 5,
		DAMAGE_LIFEBURST = 6,
		DAMAGE_KEG = 7;

	public final static String 
		STATUS_STUN = "STUN",
		STATUS_POISON = "POISON",
		STATUS_PETRIFY = "PETRIFY",
		STATUS_FAINTED = "FAINTED";

	public Item getWeapon() {
		return weapon;
	}

	public void setWeapon(Item value) {
		weapon = value;
	}
	
	public Item getSecondaryWeapon() {
		return secondaryWeapon;
	}

	public void setSecondaryWeapon(Item value) {
		secondaryWeapon = value;
	}

	public Item getArmor() {
		return armor;
	}

	public void setArmor(Item value) {
		armor = value;
	}

	public Item getAccesory() {
		return accesory;
	}

	public void setAccesory(Item value) {
		accesory = value;
	}

	public String getStatusString(){
		String status = "";
		if (hasCounter("INCREASE_SIGHT"))
			status +="Light";
		if (hasCounter("INCREASE_SIGHT2"))
			status +="Light2";
		if (isCharging())
			status +="Charge";
		if (hasStoredEnergy())
			status +="Power";
		if (isDodging())
			status +="Dodging";
		if (isPoisoned())
    		status +="Poison";
		if (isProtected())
    		status +="Protect";
		if (getCounter("QUICK") > 0)
			status +="Quick";
		if (hasCounter("PARALIZED"))
			status +="Paralyzed";
		
	   	if (hasIncreasedJumping())
    		status +="Spring";
    	if (isInvincible())
    		status +="Invin";
    	return status;
    }
	
	public int getSightRange(){
		return baseSightRange  + 
			(hasCounter("INCREASE_SIGHT2") ? 5 : (hasCounter("INCREASE_SIGHT") ||  hasCounter("TORCH") ? 2 : 0))+
			(getAccesory() != null && getAccesory().getDefinition().getEffectOnWear().equals("SIGHT2") ? 2 : 0)
			- level.getDarkness()
			;
	}

	public int getBaseSightRange() {
		return baseSightRange;
	}

	public void setBaseSightRange(int baseSightRange) {
		this.baseSightRange = baseSightRange;
	}


	public void setFOV(FOV fov){
		this.fov = fov;
	}
	
	private transient FOV fov;
	
	public void see(){
		fov.startCircle(getLevel(), getPosition().x, getPosition().y, getSightRange());
	}
	
	public void darken(){
		level.darken();
	}
	
	public Position getNearestMonsterPosition(){
		VMonster monsters = level.getMonsters();
		Monster nearMonster = null;
		int minDist = 150;
		for (int i = 0; i < monsters.size(); i++){
			Monster monster = (Monster) monsters.elementAt(i);
			int distance = Position.flatDistance(level.getPlayer().getPosition(), monster.getPosition());
			if (distance < minDist){
				minDist = distance;
				nearMonster = monster;
			}
		}
		if (nearMonster != null)
			return nearMonster.getPosition();
		else
			return null;
	}

	public int getAttackCost() {
		int accesoryBonus = 0;
		if (getAccesory() != null){
			if (getAccesory().getEffectOnWear().equals("COMBAT1"))
				accesoryBonus -= 10;
			else if (getAccesory().getEffectOnWear().equals("COMBAT2SOUL2")){
				accesoryBonus -= 20;
			}
		}
		int attackCosts = attackCost + 
		(weapon != null ? weapon.getAttackCost() : 0)+
		accesoryBonus;
		return attackCosts > 1  ? attackCosts : 1;
	
	}
	
	public int getBaseAttackCost(){
		return attackCost;
	}

	public void setAttackCost(int attackCost) {
		this.attackCost = attackCost;
	}

	public int getBaseCastCost(){
		return castCost;
	}
	
	public int getCastCost() {
		return castCost;
	}

	public void setCastCost(int castCost) {
		this.castCost = castCost;
	}

	public int getBaseWalkCost(){
		return walkCost;
	}
	
	public int getWalkCost() {
		int accesoryBonus = 0;
		if (getAccesory() != null){
			if (getAccesory().getEffectOnWear().equals("SPEED1"))
				accesoryBonus -= 5;
		}
		
		int walkCosts = walkCost - 
			(isCharging() ? 10 : 0) - 
			(getCounter("QUICK") > 0 ? 10 : 0) +
			accesoryBonus;
		return walkCosts > 1  ? walkCosts : 1;
	}

	public void setWalkCost(int walkCost) {
		this.walkCost = walkCost;
	}
	
	public void reduceCastCost(int param){
		this.castCost -= param;
	}

	public void reduceWalkCost(int param){
		this.walkCost -= param;
	}
	
	public void increaseCarryMax(int param){
		this.carryMax += param;
	}
	
	
	public void reduceAttackCost(int param){
		this.attackCost -= param;
	}
	
	public int getCarryMax() {
		return carryMax;
	}
	
	public boolean sees(Position p){
		 return level.isVisible(p.x, p.y);
	}
	 
	 public boolean sees(Monster m){
		 return sees(m.getPosition());
	 }
	 
	public boolean isDoNotRecordScore() {
		return doNotRecordScore;
	}

	public void setDoNotRecordScore(boolean doNotRecordScore) {
		this.doNotRecordScore = doNotRecordScore;
	}
	
	public Game getGame() {
		return game;
	}

	public void setGame(Game game) {
		this.game = game;
	}
	
	
	public boolean tryHit(Monster attacker){
		return tryHit(attacker, 0);
	}
	
	public final static int TACTIC_AGRESSIVE = 1, TACTIC_NORMAL = 0;
	private int currentTactic;
	
	private Position previousPosition;
	
	public boolean tryHit(Monster attacker, int damagePlus){
		if (this instanceof Player){
			Player me = (Player) this;
			int damage = attacker.getDamage()+damagePlus;
			damage -= me.getArmor().getDefense();
			int blockChance = 0;
			if (me.getSecondaryWeapon() != null)
				me.getSecondaryWeapon().getDefense();
			if (getCurrentTactic() == TACTIC_AGRESSIVE)
				blockChance = (int)(blockChance / 2.0D);
			if (isProtected())
				blockChance += (int)(blockChance / 2.0D);
			if (blockChance > 95)
				blockChance = 95;
			if (me.getSecondaryWeapon() != null && me.getSecondaryWeapon().getDefinition().getEquipCategory().equals(ItemDefinition.CAT_SHIELD) && Util.chance(blockChance)){
				level.addMessage("You block the attack with your "+me.getSecondaryWeapon().getDescription()+"!");
				SFXManager.play("wav/LTTP_Shield.wav");
			} else{
				level.addMessage("The "+attacker.getDescription()+" hits you!");
				SFXManager.play("wav/LTTP_Link_Hurt.wav");
				damageIntegrity(attacker, damage);
			}
			if (getFlag("COMBAT_COUNTER") && Util.chance(40)){
				level.addMessage("You counter attack!");
				attacker.tryHit(this, getWeapon(), false, false, Action.SELF);
			}
			return true;
		} else {
			damageIntegrity(attacker, attacker.getDamage()+damagePlus);
			return true;
		}
			
		
	}
	
	public void checkDeath(Monster damager){
		if (isDead()){
			die();
			gameSessionInfo.setDeathCause(GameSessionInfo.KILLED);
			gameSessionInfo.setKillerMonster(damager);
			gameSessionInfo.setDeathLevel(level.getLevelNumber());
			informPlayerEvent(Player.DEATH);
		}
	}
	
	private boolean isDead(){
		return hearts < 1;
	}

	private void damageIntegrity(Monster damager, int damage){
		if (isProtected()){
			level.addMessage("You feel protected!");
			damage = (int)((double)damage * 0.5d);
		}
		
		if (damage <= 0){
			level.addMessage("You shun the attack!");
			return;
		}
		//SFXManager.play("wav/jump.wav");
		hearts -= damage;
		/*if (getHearts() < 10){
			level.addMessage("*** You are almost dead! ***");
		}*/
		checkDeath(damager);
	}
	
	private void damageIntegrity(int damage){
		
		
		if (isProtected()){
			level.addMessage("You feel protected!");
			damage = (int)((double)damage * 0.5d);
		}
		
		if (damage <= 0){
			level.addMessage("You shun the attack!");
			return;
		}
		//SFXManager.play("wav/jump.wav");
		hearts -= damage;
		//checkDeath();
	}
	
	
	public int getHeartsMax() {
		return heartsMax;
	}
	
	public void setHeartsMax(int integrityMax) {
		this.heartsMax = integrityMax;
	}

	public int getHearts() {
		return hearts;
	}

	public void setHearts(int integrityPoints) {
		this.hearts = integrityPoints;
	}

	public void recoverHearts(int i){
		hearts += i;
		if (hearts > getHeartsMax())
			hearts = getHeartsMax();
    }
	
	public void recoverHeartsP(int i){
		recoverHearts(getHeartsMax()*i / 100);
    }
	
	public void recoverMagicP(int i){
		recoverMagic(getMagicMax()*i / 100);
    }

	public void recoverMagic(int i){
		magic += i;
		if (magic > getMagicMax())
			magic = getMagicMax();
    }
	
	public void reduceMagic(int i){
		magic -= i;
		if (magic < 0)
			magic = 0;
	}
	
	public int getMagicMax() {
		int bonus =0;
		Item equip = null;
		
		for (int i = 0; i < 3; i++){
			switch (i){
			case 0:
				equip = getAccesory();
				break;
			case 1:
				equip = getWeapon();
				break;
			case 2:
				equip = getArmor();
				break;
			}
			if (equip != null){
				if (equip.getDefinition().getEffectOnWear().equals("SOUL2MANA10"))
					bonus +=10;
				else if (equip.getDefinition().getEffectOnWear().equals("SOUL3MANA20"))
					bonus +=20;
			}
		}
		return magicMax+bonus;
	}

	public void setMagicMax(int manaMax) {
		this.magicMax = manaMax;
	}

	public int getMagic() {
		return magic;
	}

	public void setMagic(int magic) {
		this.magic = magic;
	}
	
	private int chargeCounter;
	private int lastWalkingDirection;
	private boolean isRunning;
	
	public void setRunning(boolean val){
		isRunning = val;
	}
	
	public void increaseChargeCounter(){
		chargeCounter++;
	}
	
	public void resetChargeCounter(){
		chargeCounter = 0;
	}

	public int getLastWalkingDirection() {
		return lastWalkingDirection;
	}

	public void setLastWalkingDirection(int lastWalkingDirection) {
		this.lastWalkingDirection = lastWalkingDirection;
	}
	
	public boolean isCharging(){
		return getFlag("COMBAT_CHARGE") && chargeCounter > 0;
		
	}
	
	public boolean isProtected(){
		return getCounter("PROTECTION") > 0;
	}
	
	public boolean isDodging(){
		return hasCounter("DODGE");
	}
	
	private int consecutiveHits;
	public void doHit(boolean hit){
		if (hit){
			consecutiveHits++;
		} else {
			consecutiveHits = 0;
		}
	}
	
	private int lastAttackDirection;
	public void setLastAttackDirection(int value){
		lastAttackDirection = value;
	}
	public int getLastAttackDirection(){
		return lastAttackDirection;
	}
	private boolean isAttacking;
	public void setIsAttacking(){
		isAttacking = true;
	}
	//private boolean storeEnergy;
	
	public void doNothing(){
		if (getFlag("COMBAT_POWER"))
			setCounter("STORE_ENERGY", 2);
	}
	
	public boolean hasStoredEnergy(){
		return hasCounter("STORE_ENERGY");
	}
	
	/*private boolean isDodging;
	
	public void setIsDodging(boolean val){
		isDodging = val;
	}
	
	public boolean isDodging(){
		return isDodging;
	}*/
	
	private int gold;
	
	public void addGold(int val){
		gold += val;
	}
	
	public int getGold(){
		return gold;
	}
	
	public void reduceGold(int val){
		gold -= val;
		if (gold <0)
			gold = 0;
	}
	
	public void setParalized(int count){
		if (getAccesory() != null && getAccesory().getDefinition().getEffectOnWear().equals("PROTECTPARA"))
			return;
		setCounter("PARALIZED", count);
	}
	
	public boolean isParalized(){
		return hasCounter("PARALIZED");
	}
	
	public int getCompletion(){
		return 0;
	}
	
	public boolean checkKeys(Feature door){
		if (getSmallKeys() < door.getKeyCost())
			return false;
		else {
			setSmallKeys(getSmallKeys()-door.getKeyCost());
			level.destroyFeature(door);
			return true;
		}
			
	}
	
	public boolean checkBossDoor(Feature door){
		if (!door.getID().equals("BOSS_DOOR"))
			return false;
		if (!getFlag("BIGKEY"+level.getID()))
			return false;
		else {
			level.destroyFeature(door);
			return true;
		}
			
	}
	
	public boolean checkTreasureDoor(Feature door){
		if (!door.getID().equals("TREASURE_DOOR"))
			return false;
		if (!getFlag("TREASURE_KEY"+level.getID()))
			return false;
		else {
			level.destroyFeature(door);
			return true;
		}
			
	}

	public int getCurrentTactic() {
		return currentTactic;
	}

	public void setCurrentTactic(int currentTactic) {
		this.currentTactic = currentTactic;
	}
	
	public void changeTactics(){
		if (getCurrentTactic() == TACTIC_AGRESSIVE)
			currentTactic = TACTIC_NORMAL;
		else
			currentTactic = TACTIC_AGRESSIVE;
	}
	
	public int getAttackBonus(){
		if (getCurrentTactic() == TACTIC_AGRESSIVE)
			return 1;
		else
			return 0;
	}

	public Position getPreviousPosition() {
		return previousPosition;
	}

	public void setPreviousPosition(Position previousPosition) {
		this.previousPosition = previousPosition;
	}
	
	
}