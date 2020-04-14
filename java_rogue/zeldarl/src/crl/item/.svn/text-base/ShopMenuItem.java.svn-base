package crl.item;

import sz.csi.textcomponents.MenuItem;
import crl.ui.*;
import crl.ui.consoleUI.CharAppearance;


public class ShopMenuItem implements MenuItem{
	private transient ItemDefinition x;
	private String defId;
	public ShopMenuItem(ItemDefinition x){
		this.x = x;
		defId = x.getID();
	}

	public String getMenuDescription(){
		return getItemDefinition().getAttributesDescription() + " : "+getItemDefinition().getMenuDescription() + " ($"+getItemDefinition().getGoldPrice()+")";
	}

	/*Unsafe, Coupled*/
	public char getMenuChar() {
		return ((CharAppearance)getItemDefinition().getAppearance()).getChar();
	}
	
	/*Unsafe, Coupled*/
	public int getMenuColor() {
		return ((CharAppearance)getItemDefinition().getAppearance()).getColor();
	}

	public ItemDefinition getItem(){
		return getItemDefinition();
	}
	
	private ItemDefinition getItemDefinition(){
		if (x == null)
			x = ItemFactory.getItemFactory().getDefinition(defId);
		return x;
	}
}