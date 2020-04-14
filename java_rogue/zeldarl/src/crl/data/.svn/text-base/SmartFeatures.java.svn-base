package crl.data;

import crl.ai.SelectorFactory;
import crl.feature.SmartFeature;
import crl.ui.AppearanceFactory;

public class SmartFeatures {
	public static SmartFeature[] getSmartFeatures(SelectorFactory sf){
		AppearanceFactory apf = AppearanceFactory.getAppearanceFactory();
		SmartFeature [] ret = new SmartFeature [1];
		ret[0] = new SmartFeature("KEG", "Powder Keg", apf.getAppearance("KEG"));
		
		ret[0].setSelector(sf.getSelector("POWDER_KEG"));
		return ret;
	}
}
