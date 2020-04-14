package zrl.player;

import java.awt.Image;

import sz.csi.textcomponents.MenuItem;
//import sz.gadgets.GFXMenuItem;
import zrl.*;
import crl.item.*;
import crl.ui.*;
import crl.ui.consoleUI.CharAppearance;


public class Equipment implements MenuItem/* GFXMenuItem*/{
	private Item item;
	private int quantity;

	public Equipment (Item pItem, int pQuantity){
		item = pItem;
		quantity = pQuantity;
 	}

 	public boolean isEmpty(){
 		return quantity == 0;
    }
 	
 	public static boolean eqMode = false;

 	public String getMenuDescription(){
 		if (quantity == 1)
 			return item.getAttributesDescription();
 		else
 			return item.getAttributesDescription() +" x"+quantity;
 	}
 	
 	public String getMenuDetail(){
 		if (eqMode)
 			return "  "+item.getDefinition().getMenuDescription();
 		else
 			return null;
 	}

 	/*Unsafe, Coupled*/
	public char getMenuChar() {
		return ((CharAppearance)item.getAppearance()).getChar();
	}
	
	/*Unsafe, Coupled*/
	public int getMenuColor() {
		return ((CharAppearance)item.getAppearance()).getColor();
	}


	public Item getItem() {
		return item;
	}

	public int getQuantity() {
		return quantity;
	}

	public void setQuantity(int value) {
		quantity = value;
	}

	public void increaseQuantity (){
		quantity ++;
	}

	public void reduceQuantity(){
		quantity --;
	}

	public Image getMenuImage() {
		return null;
		//return ((GFXAppearance)item.getAppearance()).getImage();
	}
}