package crl.monster;

import java.io.*;
import java.util.Vector;

import crl.ui.*;
import crl.ai.*;

import sz.csi.textcomponents.BasicListItem;
import sz.csi.textcomponents.ListItem;

public class MonsterDefinition {
	private String ID;
	private String description;
	private /*transient*/ Appearance appearance;
	private String wavOnHit;
	private ActionSelector defaultSelector;
	private int score;
	private int sightRange;
	private int baseEvadePoints;
	private int baseIntegrityPoints;
	private int baseEvasion;
	private int baseAttack;
	private int baseBreak;
	private int baseLeaping;
	private int baseCarryCapacity;
	private int baseSightRange;
	private int minLevel;
	private int bloodContent;
	private boolean undead;
	private int attackCost = 50, walkCost = 50;
	private int evadeChance;
	private String evadeMessage;
	private int autorespawnCount;
	private boolean floating;
	private boolean fireResistant;
	private boolean heavy;
	private boolean magus;
	private boolean boss;
	private Vector /*Drop*/ vecDrops = new Vector(5);

	public boolean isBoss() {
		return boss;
	}
	public void setBoss(boolean boss) {
		this.boss = boss;
	}
	public void addDrop(Drop d){
		vecDrops.add(d);
	}
	public void setWavOnHit(String value){
		wavOnHit = value;
	}
	
	public String getWavOnHit(){
		return wavOnHit;
	}
	
	public int getEvadeChance() {
		return evadeChance;
	}

	public void setEvadeChance(int evadeChance) {
		this.evadeChance = evadeChance;
	}

	public String getEvadeMessage() {
		return evadeMessage;
	}

	public void setEvadeMessage(String evadeMessage) {
		this.evadeMessage = evadeMessage;
	}

	public MonsterDefinition (String pID){
		//sightListItem = new BasicListItem(' ',0, "");
		ID = pID;
    }

	public String getID() {
		return ID;
	}

	public String getDescription() {
		return description;
	}



	public int getScore() {
		return score;
	}



	public int getBloodContent() {
		return bloodContent;
	}

	public boolean isBleedable(){
		return getBloodContent() > 0;
	}
	
	public boolean isUndead() {
		return undead;
	}


	public Appearance getAppearance() {
		return appearance;
	}

	public ActionSelector getDefaultSelector() {
		return defaultSelector;
	}

	public void setAppearance(Appearance appearance) {
		this.appearance = appearance;
		/*if (appearance != null){
			((BasicListItem)sightListItem).setIndex(appearance.getChar());
			((BasicListItem)sightListItem).setIndexColor(appearance.getColor());
		}*/
	}

	public void setBloodContent(int bloodContent) {
		this.bloodContent = bloodContent;
	}



	public void setDefaultSelector(ActionSelector defaultSelector) {
		this.defaultSelector = defaultSelector;
	}

	public void setDescription(String description) {
		this.description = description;
		/*if (appearance != null)
			((BasicListItem)sightListItem).setRow(getDescription());*/
	}



	public void setScore(int score) {
		this.score = score;
	}

	public void setUndead(boolean undead) {
		this.undead = undead;
	}

	/*public ListItem getSightListItem() {
		return sightListItem;
	}

	public void setSightListItem(ListItem sightListItem) {
		this.sightListItem = sightListItem;
	}*/

	

	public int getMinLevel() {
		return minLevel;
	}

	public void setMinLevel(int minLevel) {
		this.minLevel = minLevel;
	}

	public int getAttackCost() {
		return attackCost;
	}

	public void setAttackCost(int attackCost) {
		this.attackCost = attackCost;
	}

	public int getWalkCost() {
		return walkCost;
	}

	public void setWalkCost(int walkCost) {
		this.walkCost = walkCost;
	}

	public int getAutorespawnCount() {
		return autorespawnCount;
	}

	public void setAutorespawnCount(int autorespawnCount) {
		this.autorespawnCount = autorespawnCount;
	}

	public int getBaseAttack() {
		return baseAttack;
	}

	public void setBaseAttack(int baseAttack) {
		this.baseAttack = baseAttack;
	}

	public int getBaseBreak() {
		return baseBreak;
	}

	public void setBaseBreak(int baseBreak) {
		this.baseBreak = baseBreak;
	}

	public int getBaseCarryCapacity() {
		return baseCarryCapacity;
	}

	public void setBaseCarryCapacity(int baseCarryCapacity) {
		this.baseCarryCapacity = baseCarryCapacity;
	}

	public int getBaseEvadePoints() {
		return baseEvadePoints;
	}

	public void setBaseEvadePoints(int baseEvadePoints) {
		this.baseEvadePoints = baseEvadePoints;
	}

	public int getBaseEvasion() {
		return baseEvasion;
	}

	public void setBaseEvasion(int baseEvasion) {
		this.baseEvasion = baseEvasion;
	}

	public int getBaseIntegrityPoints() {
		return baseIntegrityPoints;
	}

	public void setBaseIntegrityPoints(int baseIntegrityPoints) {
		this.baseIntegrityPoints = baseIntegrityPoints;
	}

	public int getBaseLeaping() {
		return baseLeaping;
	}

	public void setBaseLeaping(int baseLeaping) {
		this.baseLeaping = baseLeaping;
	}

	public int getSightRange() {
		return sightRange;
	}

	public void setSightRange(int sightRange) {
		this.sightRange = sightRange;
	}
	public Vector getDrops(){
		return vecDrops;
	}
	public boolean isFireResistant() {
		return fireResistant;
	}
	public void setFireResistant(boolean fireResistant) {
		this.fireResistant = fireResistant;
	}
	public boolean isFloating() {
		return floating;
	}
	public void setFloating(boolean floating) {
		this.floating = floating;
	}
	public boolean isHeavy() {
		return heavy;
	}
	public void setHeavy(boolean heavy) {
		this.heavy = heavy;
	}
	public boolean isMagus() {
		return magus;
	}
	public void setMagus(boolean magus) {
		this.magus = magus;
	}

}

