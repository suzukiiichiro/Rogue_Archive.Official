package crl.item;

import java.awt.Image;
import java.io.Serializable;

import sz.csi.textcomponents.BasicListItem;
import sz.csi.textcomponents.ListItem;
import sz.csi.textcomponents.MenuItem;
//import sz.gadgets.GFXMenuItem;

import crl.ui.*;
import crl.ui.consoleUI.CharAppearance;
//import crl.ui.graphicsUI.GFXAppearance;


public class Item implements Serializable, MenuItem/*, GFXMenuItem*/{
	private transient ItemDefinition definition;
	private String defID;
	//Status
	private int remainingTurnsToReload;
	private Modifier modifier;
	private int integrity;
	
	private int uniqueID;
	
	
	public boolean isModified (){
		return modifier != null;
	}
	
	public String getModifierID(){
		return modifier.getID();
	}
	public static final Modifier [] WEAPON_MODIFIERS = new Modifier[]{
		new Modifier("_WM", " of Might"),
		new Modifier("_WE", " of Energy"),
		new Modifier("_WL", " of Light"),
		new Modifier("_WS", " of Slashing"),
		new Modifier("_WD", " of Death"),
		new Modifier("_WT", " of Time")
		
	};
	static {
		WEAPON_MODIFIERS[0].setAtkBonus(5);
		WEAPON_MODIFIERS[1].setRangeBonus(1);
		WEAPON_MODIFIERS[2].setHarmsUndead(true);
		WEAPON_MODIFIERS[3].setVrangeBonus(1);
		WEAPON_MODIFIERS[4].setSlicesThru(true);
		WEAPON_MODIFIERS[5].setAtkCostBonus(-30);
	}
	
	public static final Modifier [] ARMOR_MODIFIERS = new Modifier[]{
		new Modifier("_AP", " of Protection"),
		new Modifier("_AS", " of Shell")
	};
	
	static {
		ARMOR_MODIFIERS[0].setDefenseBonus(1);
		ARMOR_MODIFIERS[0].setDefenseBonus(2);
	}
	
	
	public Item (ItemDefinition itemDef, Modifier mod, int pUniqueID){
		uniqueID = pUniqueID;
		definition = itemDef;
		defID = definition.getID();
		modifier = mod;
		integrity = definition.getBaseIntegrity();
	}
	
	public String getUniqueID(){
		return ""+uniqueID;
	}
	public Item (ItemDefinition itemDef, int pUniqueID){
		uniqueID = pUniqueID;
		definition = itemDef;
		defID = definition.getID();
		modifier = new Modifier("","");
		integrity = definition.getBaseIntegrity();
	}
	
	public ItemDefinition getDefinition() {
		if (definition == null){
			definition = ItemFactory.getItemFactory().getDefinition(defID);
		}
		return definition;
	}

	public int getRemainingTurnsToReload() {
		return remainingTurnsToReload;
	}

	public void setRemainingTurnsToReload(int value) {
		remainingTurnsToReload = value;
	}

	public void reload(){
		setRemainingTurnsToReload(getDefinition().getReloadTurns());
	}

	
	/*public ListItem getSightListItem() {
		((BasicListItem)getDefinition().getSightListItem()).setRow(getDescription());
		return getDefinition().getSightListItem();
	}*/

	public boolean isVisible(){
		return !getDefinition().getAppearance().getID().equals("VOID");
	}

	/*Unsafe, Coupled*/
	public char getMenuChar() {
		return ((CharAppearance)getDefinition().getAppearance()).getChar();
	}
	
	/*Unsafe, Coupled*/
	public int getMenuColor() {
		return ((CharAppearance)getDefinition().getAppearance()).getColor();
	}

	public String getMenuDescription() {
		return getAttributesDescription();
	}
	
	public String getDescription(){
		return getDefinition().getDescription() + modifier.getDescription();
	}
	
	public Appearance getAppearance() {
		return getDefinition().getAppearance();
	}

	public int getAttack() {
		return getDefinition().getAttack()+modifier.getAtkBonus();
	}

	public int getAttackCost() {
		return getDefinition().getAttackCost()+modifier.getAtkCostBonus();
	}

	public int getDefense() {
		return getDefinition().getDefense()+modifier.getDefenseBonus();
	}

	public String getEffectOnAcquire() {
		return getDefinition().getEffectOnAcquire();
	}

	/*public String getEffectOnStep() {
		return getDefinition().getEffectOnStep();
	}*/

	public String getEffectOnUse() {
		return getDefinition().getEffectOnUse();
	}

	public int getFeatureTurns() {
		return getDefinition().getFeatureTurns();
	}

	public boolean isHarmsUndead() {
		return getDefinition().isHarmsUndead() || modifier.isHarmsUndead();
	}

	public String getPlacedSmartFeature() {
		return getDefinition().getPlacedSmartFeature();
	}

	public int getRange() {
		return getDefinition().getRange() + modifier.getRangeBonus();
	}

	public int getReloadTurns() {
		return getDefinition().getReloadTurns();
	}

	public boolean isSlicesThrough() {
		return getDefinition().isSlicesThrough() || modifier.isSlicesThru();
	}

	public String getThrowMessage() {
		return getDefinition().getThrowMessage();
	}

	public int getThrowRange() {
		return getDefinition().getThrowRange();
	}

	public String getUseMessage() {
		return getDefinition().getUseMessage();
	}

	public int getVerticalRange() {
		return getDefinition().getVerticalRange() + modifier.getVrangeBonus();
	}
	
	public String getAttributesDescription(){
		String base = getDescription();
		if (getRemainingTurnsToReload()>0){
			base += " {"+getRemainingTurnsToReload()+"}";
		}
		if (getAttack() > 0 || getDefense() > 0 || getRange() > 1 || getVerticalRange() > 0)
			base+= " (";
		if (getAttack() > 0) {
			base+= "ATK"+getAttack()+" "+getHitChance()+"%";
		}
		if (getDefense() > 0)
			base+= "DEF"+getDefense();
		if (getRange() > 1 || getVerticalRange() > 0)
			if (getVerticalRange() > 0)
				base+= " RNG"+getRange()+","+getVerticalRange();
			else
				base+= " RNG"+getRange();
		if (definition.getReloadCostGold() > 0){
			base += " RLD"+definition.getReloadCostGold()+"$";
		}
		if (getDefense() > 0){
			base += " INT"+getIntegrity()+"/"+definition.getBaseIntegrity();
		}
		if (getAttack() > 0 || getDefense() > 0 || getRange() > 1 || getVerticalRange() > 0)
			base+= ")";
		return base;
	}
	
	public String getFullID(){
		String toAddID = getDefinition().getID();
		if (isModified())
			toAddID += getModifierID();
		if (!getDefinition().isStackable())
			toAddID += uniqueID+"" /*hashcode()*/;
		return toAddID;
	}
	
	public void damage(int dam){
		integrity -= dam;
		if (integrity < 0)
			integrity = 0;
	}
	
	public void fix(int dam){
		integrity += dam;
		if (integrity > getDefinition().getBaseIntegrity())
			integrity = getDefinition().getBaseIntegrity();
	}
	
	public int getCoverage(){
		return (int)Math.round(((double)integrity / (double)definition.getBaseIntegrity()) * (double)definition.getDefense());
	}
	
	

	public int getIntegrity() {
		return integrity;
	}
	
	public int getHitChance(){
		return getDefinition().getHitChance();
	}

	public Image getMenuImage() {
		return null;
		//return ((GFXAppearance)getAppearance()).getImage();
	}
	
	public String getEffectOnWear(){
		return getDefinition().getEffectOnWear();
	}

	public String getMenuDetail() {
		// TODO Auto-generated method stub
		return null;
	}
}