package crl.feature;

import sz.csi.textcomponents.BasicListItem;
import sz.csi.textcomponents.ListItem;
import sz.util.*;
import sz.csi.textcomponents.*;
import zrl.player.*;

import crl.ui.*;
import crl.monster.Monster;

public class Feature implements Cloneable, java.io.Serializable {
	/** A feature is something that stays inside the level but may be moved,
	 * destroyed or otherwise affected. */
	private Feature prize;
	private int resistance; // How many blows til it gives the prize (max)
	private int currentResistance; // How many blows til it gives the prize
	private boolean destroyable, isSolid, isOpaque;
	private int heartPrize,
			mysticWeaponPrize = -1,
			keyPrize,
			upgradePrize, goldPrize;
	private Position position;
	private transient Appearance appearance;
	private String ID, description,appearanceID;
	private String trigger;
	private int heightMod;
	private int keyCost;
	private String effect;
	private int scorePrize;
	private int healPrize;
	private int manaPrize;
	private boolean relevant = true;

	public String getID(){
		return ID;
	}

	private Feature getPrizeFor(Player p){
		String [] prizeList = null;

        
	        if (Util.chance(50))
    	    if (Util.chance(50))
        	if (Util.chance(40))
	        if (Util.chance(40))
    	    if (Util.chance(40))
    	    	prizeList = new String[]{"RED_RUPEE"};
			else
				prizeList = new String[]{"BLUE_RUPEE"};
			else
				prizeList = new String[]{"RUPEE","RUPEE"};
			else
				prizeList = new String[]{"HEART","RUPEE"};
			else
				prizeList = new String[]{"HEART","RUPEE"};
			else
				prizeList = null;    	
    	
        if (prizeList != null)
        	return FeatureFactory.getFactory().buildFeature(Util.randomElementOf(prizeList));
        else
        	return null;
	}

	public Feature damage(Player p, int damage){
		currentResistance -= damage;
		if (currentResistance < 0){
			Feature pPrize = getPrizeFor(p);
			
			if (pPrize != null){
				pPrize.setPosition(position.x, position.y, position.z);
				p.getLevel().addFeature(pPrize);
			}
			p.getLevel().destroyFeature(this);
			return pPrize;
		}
		return null;
	}
	
	public void damage(Monster m){
		currentResistance -= m.getDamage();
		if (currentResistance < 0){
			m.getLevel().destroyFeature(this);
		}
	}

	public Object clone(){
		try {
			Feature x = (Feature) super.clone();

			if (position != null)
				x.setPosition(position.x, position.y, position.z);
			if (prize != null)
				x.setPrize((Feature)prize.clone());
			return x;
		} catch (CloneNotSupportedException cnse){
			Debug.doAssert(false, "failed class cast, Feature.clone()");
		}
		return null;
	}

	public void setPrize(Feature what){
		prize = what;
	}

	public Feature (String pID, Appearance pApp, int resistance, String pDescription){
		ID = pID;
		appearance = pApp;
		appearanceID = pApp.getID();
		this.resistance = resistance;
		description = pDescription;
		currentResistance = resistance;
		//sightListItem = new BasicListItem(appearance.getChar(), appearance.getColor(), description);
		Debug.doAssert(pApp != null, "No se especifico apariencia pa la featura");
	}

	public void setPosition(int x, int y, int z){
		position = new Position (x,y, z);
	}

	public Appearance getAppearance(){
		if (appearance == null){
			if (appearanceID != null)
				appearance = AppearanceFactory.getAppearanceFactory().getAppearance(appearanceID);
		}
		return appearance;
	}

	public String getDescription(){
		return description;
	}

	public Position getPosition(){
		return position;
	}

	public void setPrizesFor(Player p){
		heartPrize = 0;
		mysticWeaponPrize = -1;
		upgradePrize = 0;

		
    }

	public int getHeartPrize() {
		return heartPrize;
	}

	public void setHeartPrize(int value) {
		heartPrize = value;
	}

	public int getMysticWeaponPrize() {
		return mysticWeaponPrize;
	}

	public void setMysticWeaponPrize(int value) {
		mysticWeaponPrize = value;
	}


    public int getUpgradePrize() {
		return upgradePrize;
	}

	public void setUpgradePrize(int value) {
		upgradePrize = value;
	}


	public int getKeyPrize() {
		return keyPrize;
	}

	public void setKeyPrize(int value) {
		keyPrize = value;
	}

	public String getTrigger() {
		return trigger;
	}

	public void setTrigger(String value) {
		trigger = value;
	}

	public int getHeightMod() {
		return heightMod;
	}

	public void setHeightMod(int value) {
		heightMod = value;
	}

	public int getKeyCost() {
		return keyCost;
	}

	public void setKeyCost(int value) {
		keyCost = value;
	}

	public boolean isSolid() {
		return isSolid;
	}

	public void setSolid(boolean value) {
		isSolid = value;
	}

	public boolean isDestroyable() {
		return destroyable;
	}

	public void setDestroyable(boolean value) {
		destroyable = value;
	}

	public String getEffect() {
		return effect;
	}

	public void setEffect(String value) {
		effect = value;
	}

	public int getScorePrize() {
		return scorePrize;
	}

	public void setScorePrize(int value) {
		scorePrize = value;
	}

	public int getHealPrize() {
		return healPrize;
	}

	public void setHealPrize(int value) {
		healPrize = value;
	}

	public boolean isRelevant() {
		return relevant;
	}

	public void setRelevant(boolean relevant) {
		this.relevant = relevant;
	}

	public boolean isVisible(){
		return !getAppearance().getID().equals("VOID");
	}

	public int getGoldPrize() {
		return goldPrize;
	}

	public void setGoldPrize(int goldPrize) {
		this.goldPrize = goldPrize;
	}

	public int getManaPrize() {
		return manaPrize;
	}

	public void setManaPrize(int manaPrize) {
		this.manaPrize = manaPrize;
	}

	public boolean isOpaque() {
		return isOpaque;
	}

	public void setOpaque(boolean isOpaque) {
		this.isOpaque = isOpaque;
	}
}