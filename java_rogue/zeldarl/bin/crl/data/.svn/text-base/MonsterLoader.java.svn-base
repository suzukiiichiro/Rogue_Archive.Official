package crl.data;

import crl.ai.ActionSelector;
import crl.ai.monster.BasicMonsterAI;
import crl.ai.monster.MonsterAI;
import crl.ai.monster.RangedAI;
import crl.ai.monster.RangedAttack;
import crl.ai.monster.StationaryAI;

import crl.ai.monster.WanderToPlayerAI;

import crl.game.CRLException;
import crl.monster.*;
import crl.ui.AppearanceFactory;

import java.text.ParseException;
import java.util.*;
import java.io.*;

import org.xml.sax.AttributeList;
import org.xml.sax.ContentHandler;
import org.xml.sax.DocumentHandler;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.XMLReaderFactory;

import sz.util.*;
import sz.crypt.*;
import uk.co.wilson.xml.MinML;


public class MonsterLoader {
	
	public static MonsterDefinition[] getMonsterDefinitions(String monsterFile) throws CRLException{
		try{
	        MonsterDocumentHandler handler = new MonsterDocumentHandler();
	        //XMLReader parser = XMLReaderFactory.createXMLReader("org.apache.xerces.parsers.SAXParser");
	        MinML parser = new MinML();
	        
	        DESEncrypter encrypter = new DESEncrypter("If you can see this, you are the one to go ahead and get the monsters info");
	        //parser.setContentHandler(handler);
	        parser.setDocumentHandler(handler);
	        parser.parse(new InputSource(encrypter.decrypt(new FileInputStream(monsterFile))));
	        Vector monsters = handler.getMonsterDefinitions();
	        MonsterDefinition[] ret = new MonsterDefinition[monsters.size()];
	        for (int i = 0; i < monsters.size(); i++)
	        	ret[i] = (MonsterDefinition) monsters.elementAt(i);
	        return ret;
		} catch (IOException ioe){
			throw new CRLException("Error while loading data from monster file");
		} catch (SAXException sax){
			sax.printStackTrace();
			throw new CRLException("Error while loading data from monster file");
		}
    }

}



class MonsterDocumentHandler implements DocumentHandler{
    private Vector defVector = new Vector (10);
   
    private MonsterDefinition currentMD;
    private ActionSelector currentSelector;
    private Vector currentRangedAttacks;
    
    public Vector getMonsterDefinitions (){
        return defVector;
    }
    
    public void startDocument() throws org.xml.sax.SAXException {}
    

    public void startElement(String localName, AttributeList at) throws org.xml.sax.SAXException {
        if (localName.equals("monster")){
        	currentMD = new MonsterDefinition(at.getValue("id"));
        	currentMD.setAppearance(AppearanceFactory.getAppearanceFactory().getAppearance(at.getValue("appearance")));
        	currentMD.setDescription(at.getValue("description"));
        	if (at.getValue("effectWav")!= null)
        		currentMD.setWavOnHit(at.getValue("effectWav"));
        	if (at.getValue("bloodContent") != null)
        		currentMD.setBloodContent(inte(at.getValue("bloodContent")));
        	if (at.getValue("undead") != null)
        		currentMD.setUndead(at.getValue("undead").equals("true"));
        	if (at.getValue("walkCost") != null){
        		currentMD.setWalkCost(inte(at.getValue("walkCost")));
        		currentMD.setAttackCost(inte(at.getValue("walkCost")));
        	}
        	if (at.getValue("evadeChance")!=null){
        		currentMD.setEvadeChance(inte(at.getValue("evadeChance")));
        		currentMD.setEvadeMessage(at.getValue("evadeMessage"));
        	}
        	if (at.getValue("autorespawns")!=null){
        		currentMD.setAutorespawnCount(inte(at.getValue("autorespawns")));
        	}
        	if (at.getValue("baseEvadePoints")!= null){
        		currentMD.setBaseEvadePoints(inte(at.getValue("baseEvadePoints")));
        	}
        	if (at.getValue("baseIntegrityPoints")!= null){
        		currentMD.setBaseIntegrityPoints(inte(at.getValue("baseIntegrityPoints")));
        	}
        	if (at.getValue("baseEvasion")!= null){
        		currentMD.setBaseEvasion(inte(at.getValue("baseEvasion")));
        	}
        	if (at.getValue("baseAttack")!= null){
        		currentMD.setBaseAttack(inte(at.getValue("baseAttack")));
        	}
        	if (at.getValue("baseBreak")!= null){
        		currentMD.setBaseBreak(inte(at.getValue("baseBreak")));
        	}
        	if (at.getValue("baseSightRange")!= null){
        		currentMD.setSightRange(inte(at.getValue("baseSightRange")));
        	}
        	if (at.getValue("floating")!= null){
        		currentMD.setFloating(at.getValue("floating").equals("true"));
        	}
        	if (at.getValue("fireResistant")!= null){
        		currentMD.setFireResistant(at.getValue("fireResistant").equals("true"));
        	}
        	if (at.getValue("heavy")!= null){
        		currentMD.setHeavy(at.getValue("heavy").equals("true"));
        	}
        	if (at.getValue("magus")!= null){
        		currentMD.setMagus(at.getValue("magus").equals("true"));
        	}
        	if (at.getValue("boss")!= null){
        		currentMD.setBoss(at.getValue("boss").equals("true"));
        	}
        } else
        if (localName.equals("sel_wander")){
        	currentSelector = new WanderToPlayerAI();
        } else
    	if (localName.equals("sel_stationary")){
        	currentSelector = new StationaryAI();
        }else
    	if (localName.equals("sel_basic")){
        	currentSelector = new BasicMonsterAI();
        	if (at.getValue("stationary") != null)
        		((BasicMonsterAI)currentSelector).setStationary(at.getValue("stationary").equals("true"));
        	if (at.getValue("approachLimit") != null)
        		((BasicMonsterAI)currentSelector).setApproachLimit(inte(at.getValue("approachLimit")));
        	if (at.getValue("waitPlayerRange") != null)
        		((BasicMonsterAI)currentSelector).setWaitPlayerRange(inte(at.getValue("waitPlayerRange")));
        	if (at.getValue("patrolRange") != null)
        		((BasicMonsterAI)currentSelector).setPatrolRange(inte(at.getValue("patrolRange")));
        }else if (localName.equals("drop")){
        	currentMD.addDrop(new Drop(at.getValue("type"), at.getValue("id"), inte(at.getValue("prob"))));
        }else
        if (localName.equals("sel_ranged")){
        	currentSelector = new RangedAI();
        	((RangedAI)currentSelector).setApproachLimit(inte(at.getValue("approachLimit")));
        }else
    	if (localName.equals("rangedAttacks")){
    		currentRangedAttacks = new Vector(10);
    	} else 
		if (localName.equals("rangedAttack")){
			int damage = 0;
			try {
				damage = Integer.parseInt(at.getValue("damage")); 
			} catch (NumberFormatException nfe){
				
			}
				
			RangedAttack ra = new RangedAttack(
					at.getValue("id"),
					at.getValue("type"),
					at.getValue("status_effect"),
					Integer.parseInt(at.getValue("range")), 
					Integer.parseInt(at.getValue("frequency")),
					at.getValue("message"),
					at.getValue("effectType"),
					at.getValue("effectID"),
					damage
					
					//color
					);
			if (at.getValue("effectWav") != null)
				ra.setEffectWav(at.getValue("effectWav"));
			if (at.getValue("summonMonsterId") != null)
				ra.setSummonMonsterId(at.getValue("summonMonsterId"));
			if (at.getValue("charge") != null)
				ra.setChargeCounter(inte(at.getValue("charge")));
			
			currentRangedAttacks.add(ra);
		}
    }
    
    public void endElement(String localName) throws org.xml.sax.SAXException {
        if (localName.equals("rangedAttacks")){
        	((MonsterAI)currentSelector).setRangedAttacks(currentRangedAttacks);
        }
        else
        if (localName.equals("selector")){
            currentMD.setDefaultSelector(currentSelector);
        }
        else
        if (localName.equals("monster")){
            defVector.add(currentMD);
        }
    }
    
    public void characters(char[] values, int param, int param2) throws org.xml.sax.SAXException {}

    public void endDocument() throws org.xml.sax.SAXException {}

    public void endPrefixMapping(String str) throws org.xml.sax.SAXException {}

    public void ignorableWhitespace(char[] values, int param, int param2) throws org.xml.sax.SAXException {}

    public void processingInstruction(String str, String str1) throws org.xml.sax.SAXException {}

    public void setDocumentLocator(org.xml.sax.Locator locator) {}

    public void skippedEntity(String str) throws org.xml.sax.SAXException {}

    public void startPrefixMapping(String str, String str1) throws org.xml.sax.SAXException {}
    
    private int inte(String s){
    	return Integer.parseInt(s);
    }
}
	