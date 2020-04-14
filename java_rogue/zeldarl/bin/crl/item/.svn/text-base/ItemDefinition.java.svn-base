package crl.item;

import sz.csi.textcomponents.BasicListItem;
import sz.csi.textcomponents.ListItem;
import sz.csi.textcomponents.MenuItem;
import crl.ui.*;

public class ItemDefinition {
	// Weapon Categories
	public final static int
		CAT_UNARMED = 0,
		CAT_DAGGERS = 1,
		CAT_SWORDS = 2,
		CAT_SPEARS = 3,
		CAT_WHIPS = 4,
		CAT_STAVES = 5,
		CAT_RINGS = 6,
		CAT_PISTOLS = 7,
		CAT_PROJECTILES = 8;
	
	public final static String[] CAT_DESCRIPTIONS = new String [] {
		"fists",
		"daggers",
		"swords",
		"spears",
		"whips",
		"staves",
		"rings",
		"ranged weapons",
		"thrown weapons"
	};

	//Shop Categories
	public final static String
		CAT_WEAPON = "WEAPON",
		CAT_ARMOR = "ARMOR",
		CAT_SHIELD = "SHIELD",
		CAT_ACCESORY = "ACCESORY";

	//Attributes
	private String ID;
	private String description;
	private int goldPrice;
	private String effectOnUse;
	private String effectOnAcquire;
	private String effectOnStep;
	private boolean singleUse;
	private int throwRange;
	private Appearance appearance;
	private String useMessage;
	private String throwMessage;
	private int featureTurns;
	private String placedSmartFeature;
   	private int attack;
	private int range;
	private int reloadTurns;
	private boolean harmsUndead;
	private boolean slicesThrough;
	private MenuItem shopMenuItem;
	private int rarity;
	private int minLevel;
	private int defense;
	
	private String attackSFX;
	private String menuDescription;
	
	private String equipCategory;
	
	/*private ListItem sightListItem;
	 * Debe ser calculado por la UI, y guardado en esta
	 */
	private int lv1prob;
	private int verticalRange;
	private int attackCost;
	
	//private int evadeBreak;
	//private int damage;
	
	private int baseIntegrity;
	
	private int reloadCostGold;
	private int hitChance;
	
	public int getAttackCost() {
		return attackCost;
	}

	private boolean stackable;
	
	private String effectOnWear;
	private boolean isUnique;
	
	public ItemDefinition (String pID, String pDescription, String pAppearance,
		String pEffectOnUse, boolean pSingleUse, int pHit, int pDamage,
		int pRange, int pReloadTurns, int pAttackCost, int pReloadCost, boolean pSlicesThrough, String pUseMessage,
		int pMinLevel, int pRarity, String pMenuDescription, int pDefense, int pIntegrity, 
		String pAttackSFX, boolean pStackable, String pEffectOnWear, String pEquipCategory, boolean pIsUnique){
		ID = pID;
		description = pDescription;
		appearance = AppearanceFactory.getAppearanceFactory().getAppearance(pAppearance);
		effectOnUse = pEffectOnUse;
		singleUse = pSingleUse;
		attackCost = pAttackCost;
		
		attack = pDamage;
		range = pRange;
		reloadTurns = pReloadTurns;
		reloadCostGold = pReloadCost;
		slicesThrough = pSlicesThrough;
		useMessage = pUseMessage;
		minLevel = pMinLevel;
		rarity = pRarity;
		menuDescription = pMenuDescription;
		defense=pDefense;
		baseIntegrity = pIntegrity;
		attackSFX = pAttackSFX;
		shopMenuItem = new ShopMenuItem(this);
		stackable = pStackable;
		effectOnWear = pEffectOnWear;
		isUnique = pIsUnique;
		hitChance = pHit;
		equipCategory = pEquipCategory;
		//sightListItem = new BasicListItem(appearance.getChar(), appearance.getColor(), description);
	}


	public MenuItem getShopMenuItem(){
		return shopMenuItem;
    }

	public String getID() {
		return ID;
	}

	public String getDescription() {
		return description;
	}

	public int getGoldPrice() {
		return goldPrice;
	}

	public String getEffectOnUse() {
		return effectOnUse;
	}

	public String getEffectOnAcquire() {
		return effectOnAcquire;
	}

	public boolean isSingleUse() {
		return singleUse;
	}

	public int getThrowRange() {
		return throwRange;
	}

	public Appearance getAppearance() {
		return appearance;
	}

	public String getUseMessage() {
		return useMessage;
	}

	public String getThrowMessage() {
		return throwMessage;
	}

	public int getFeatureTurns() {
		return featureTurns;
	}

	public String getPlacedSmartFeature() {
		return placedSmartFeature;
	}

	public int getAttack() {
		return attack;
	}

	public int getRange() {
		return range;
	}

	public int getReloadTurns() {
		return reloadTurns;
	}

	public boolean isHarmsUndead() {
		return harmsUndead;
	}

	public boolean isSlicesThrough() {
		return slicesThrough;
	}

	/*public String getMenuDescription(){
		return getAttributesDescription();
	}
	
	public Appearance getMenuAppearance(){
	 	return getAppearance();
	}*/

	public int getRarity() {
		return rarity;
	}

	public String getEquipCategory() {
		return equipCategory;
	}

	public String getAttackSFX() {
		return attackSFX;
	}

	public int getDefense() {
		return defense;
	}




	/*public ListItem getSightListItem(){
		return sightListItem;
	}
	
	public void setSightListItem(ListItem sightListItem) {
		this.sightListItem = sightListItem;
	}
	*/
	/*public String getShopDescription(){
		return shopDescription;
	}*/

	public int getLv1Prob(){
		return lv1prob;
	}
	
	public int getVerticalRange(){
		return verticalRange;
	}
	
	public String getAttributesDescription(){
		String base = getDescription();
		if (getAttack() > 0 || getDefense() > 0 || getRange() > 1 || getVerticalRange() > 0)
			base+= " (";
		if (getAttack() > 0)
			base+= "ATK:"+getAttack()+" ";
		if (getDefense() > 0)
			base+= "DEF:"+getDefense()+" ";
		if (getRange() > 1 || getVerticalRange() > 0)
			if (getVerticalRange() > 0)
				base+= "RNG:"+getRange()+","+getVerticalRange();
			else
				base+= "RNG:"+getRange();
		if (getReloadCostGold() > 0){
			base += " RLD:"+getReloadCostGold();
		}
		if (getAttack() > 0 || getDefense() > 0 || getRange() > 1 || getVerticalRange() > 0)
			base+= ")";
		return base;
	}


	public String getMenuDescription() {
		return menuDescription;
	}


	public void setMenuDescription(String menuDescription) {
		this.menuDescription = menuDescription;
	}


	public int getReloadCostGold() {
		return reloadCostGold;
	}


	public void setReloadCostGold(int reloadCostGold) {
		this.reloadCostGold = reloadCostGold;
	}


	
	

	public int getBaseIntegrity() {
		return baseIntegrity;
	}


	public void setBaseIntegrity(int baseIntegrity) {
		this.baseIntegrity = baseIntegrity;
	}


	public boolean isStackable() {
		return stackable;
	}


	public String getEffectOnWear() {
		return effectOnWear;
	}


	public boolean isUnique() {
		return isUnique;
	}


	public int getHitChance() {
		return hitChance;
	}


	public int getMinLevel() {
		return minLevel;
	}
}