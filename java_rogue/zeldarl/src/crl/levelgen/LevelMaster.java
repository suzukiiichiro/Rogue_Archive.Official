package crl.levelgen;

import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import crl.levelgen.cave.*;
import crl.levelgen.dungeon.AdamantCave;
import crl.levelgen.dungeon.DungeonData;
import crl.levelgen.dungeon.ForestMonastery;
import crl.levelgen.dungeon.TempleOfAncients;
import crl.levelgen.dungeon.TempleOfSands;
import crl.levelgen.dungeon.TempleRuins;
import crl.levelgen.patterns.*;
import crl.item.*;
import sz.util.*;
import zrl.player.*;
import crl.level.*;
import crl.monster.Monster;
import crl.monster.MonsterFactory;
import crl.ui.Display;
import crl.ui.UserInterface;


import crl.cuts.EndOfAdamantCave;
import crl.cuts.EndOfTempleAncient;
import crl.cuts.Unleasher;
import crl.feature.Feature;
import crl.feature.FeatureFactory;
import crl.game.*;

public class LevelMaster {
	private static Dispatcher currentDispatcher;
	private static boolean firstCave = true;
	public static Level createLevel(String levelID, String previousLevelID, int levelNumber) throws CRLException{
		
		/*Prueba*/
		//JOptionPane.showMessageDialog(null, "createLevel "+levelID+" with former "+previousLevelID);
		Debug.enterStaticMethod("LevelMaster", "createLevel");
		Debug.say("levelID "+levelID);
		boolean overrideLevelNumber = false;
		boolean blockTime = false;
		Level ret = null;
		PatternGenerator.getGenerator().resetFeatures();
		Respawner x = new Respawner(30, 80);
		x.setSelector(new RespawnAI());
		boolean isStatic = false;
		StaticPattern pattern = null;
		if (levelID.startsWith("LINKS")){
			isStatic = true;
			pattern = new LinkHouse();
		} 
		
		if (isStatic){
			pattern.setup(StaticGenerator.getGenerator());
			ret = StaticGenerator.getGenerator().createLevel();
			if (pattern.isHaunted()){
				ret.setHaunted(true);
				ret.setNightRespawner(x);
				ret.savePop();
				ret.getMonsters().removeAll();
				ret.getDispatcher().removeAll();
				
			}
			ret.setRespawner(x);
			ret.setInhabitants(pattern.getSpawnInfo());
			ret.setDwellerIDs(pattern.getDwellers());
			ret.setSpawnItemIDs(pattern.getItems());
			ret.setDescription(pattern.getDescription());
			ret.setHostageSafe(pattern.isHostageSafe());
			ret.setMusicKeyMorning(pattern.getMusicKeyMorning());
			ret.setMusicKeyNoon(pattern.getMusicKeyNoon());
			
			if (pattern.getBoss() != null){
				Monster monsBoss = MonsterFactory.getFactory().buildMonster(pattern.getBoss());
				monsBoss.setPosition(pattern.getBossPosition());
				ret.setBoss(monsBoss);
			}
			if (pattern.getUnleashers() != null){
				ret.setUnleashers(pattern.getUnleashers());
			}
			ret.setMapLocationKey(pattern.getMapKey());
		} 
		
		
		if (levelID.startsWith("OVERWORLD")){
			OverworldGenerator og = new OverworldGenerator();
			ret = og.generateLevel();
			ret.setDescription("Hyrule");
			String musicKey = null;
			musicKey = "OVERWORLD";
			ret.setMusicKeyMorning(musicKey);
			ret.setMusicKeyNoon(musicKey);
			ret.setDispatcher(new Dispatcher());
			Respawner overWorldRespawner = new Respawner(2, 100);
			overWorldRespawner.setSelector(new RespawnAI());
			ret.setRespawner(overWorldRespawner);
			
			//Draw, Debug
			
			
		} else if (levelID.endsWith("ADAMANTCAVE")){
			DungeonGenerator dg = new DungeonGenerator();
			DungeonData dungeon = new AdamantCave();
			ret = dg.generateLevel(dungeon);
			ret.setDescription("Adamant Cave");
			String musicKey = null;
			musicKey = "ADAMANTCAVE";
			ret.setMusicKeyMorning(musicKey);
			ret.setMusicKeyNoon(musicKey);
			ret.setRespawner(x);
			ret.setInhabitants(dungeon.getInhabitants());
			ret.setDwellerIDs(dungeon.getDwellers());
			ret.setUnleashers(new Unleasher[]{new EndOfAdamantCave()});
			
			//ret.setDispatcher(new Dispatcher());
			ret.setNeedsCompass(true);
		} else if (levelID.startsWith("FORESTDUNGEON")){
		//} else if (levelID.endsWith("DUNGEON")){
			DungeonGenerator dg = new DungeonGenerator();
			DungeonData dungeon = new ForestMonastery();
			ret = dg.generateLevel(dungeon);
			ret.setDescription("Forest Monastery");
			String musicKey = null;
			musicKey = "DUNGEON2";
			ret.setMusicKeyMorning(musicKey);
			ret.setMusicKeyNoon(musicKey);
			//ret.setDispatcher(new Dispatcher());
			ret.setRespawner(x);
			ret.setInhabitants(dungeon.getInhabitants());
			ret.setDwellerIDs(dungeon.getDwellers());
			ret.setNeedsCompass(true);
		}else if (levelID.startsWith("RUINSDUNGEON")){
		//} else if (levelID.endsWith("DUNGEON")){
			DungeonGenerator dg = new DungeonGenerator();
			DungeonData dungeon = new TempleRuins();
			ret = dg.generateLevel(dungeon);
			ret.setDescription("Temple Ruins");
			String musicKey = null;
			musicKey = "DUNGEON1";
			ret.setMusicKeyMorning(musicKey);
			ret.setMusicKeyNoon(musicKey);
			//ret.setDispatcher(new Dispatcher());
			ret.setRespawner(x);
			ret.setInhabitants(dungeon.getInhabitants());
			ret.setDwellerIDs(dungeon.getDwellers());
			ret.setNeedsCompass(true);
		} else if (levelID.startsWith("DESERTDUNGEON")){
			DungeonGenerator dg = new DungeonGenerator();
			DungeonData dungeon = new TempleOfSands();
			ret = dg.generateLevel(dungeon);
			ret.setDescription("Temple of Sands");
			String musicKey = null;
			musicKey = "DUNGEON3";
			ret.setMusicKeyMorning(musicKey);
			ret.setMusicKeyNoon(musicKey);
			//ret.setDispatcher(new Dispatcher());
			ret.setRespawner(x);
			ret.setInhabitants(dungeon.getInhabitants());
			ret.setDwellerIDs(dungeon.getDwellers());
			ret.setNeedsCompass(true);
		}else if (levelID.startsWith("ANCIENTTEMPLE")){
			DungeonGenerator dg = new DungeonGenerator();
			DungeonData dungeon = new TempleOfAncients();
			ret = dg.generateLevel(dungeon);
			ret.setDescription("Temple of Ancients");
			String musicKey = null;
			musicKey = "TEMPLE";
			ret.setMusicKeyMorning(musicKey);
			ret.setMusicKeyNoon(musicKey);
			//ret.setDispatcher(new Dispatcher());
			ret.setRespawner(x);
			ret.setInhabitants(dungeon.getInhabitants());
			ret.setDwellerIDs(dungeon.getDwellers());
			ret.setUnleashers(new Unleasher[]{new EndOfTempleAncient()});
			ret.setNeedsCompass(true);
		}
		
		
		ret.setID(levelID);
		if (!overrideLevelNumber)
			ret.setLevelNumber(levelNumber);
		//if (levelID.startsWith("AVATARCELL")){
		if (isStatic){
			ret.removeActor(x);
		}
		ret.setTimeStopBlocked(blockTime);
		Debug.exitMethod(ret);
		return ret;

	}

	public static Dispatcher getCurrentDispatcher() {
		return currentDispatcher;
	}

	protected int placeKeys(Level ret){
		Debug.enterMethod(this, "placeKeys");
		//Place the magic Keys
		int keys = Util.rand(1,4);
		Position tempPosition = new Position(0,0);
		for (int i = 0; i < keys; i++){
			int keyx = Util.rand(1,ret.getWidth()-1);
			int keyy = Util.rand(1,ret.getHeight()-1);
			int keyz = Util.rand(0, ret.getDepth()-1);
			tempPosition.x = keyx;
			tempPosition.y = keyy;
			tempPosition.z = keyz;
			if (ret.isWalkable(tempPosition)){
				Feature keyf = FeatureFactory.getFactory().buildFeature("KEY");
				keyf.setPosition(tempPosition.x, tempPosition.y, tempPosition.z);
				ret.addFeature(keyf);
			} else {
				i--;
			}
		}
		Debug.exitMethod(keys);
		return keys;
		
	}
	public static void placeItems(Level ret, Player p, int levelNo){
		int items = Util.rand(2,4);
		//int items = 300;
		for (int i = 0; i < items; i++){
			Item item = ItemFactory.getItemFactory().createItemForLevel(levelNo);
			if (item == null)
				continue;
			int xrand = 0;
			int yrand = 0;
			Position pos = null;
			do {
				xrand = Util.rand(1, ret.getWidth()-1);
				yrand = Util.rand(1, ret.getHeight()-1);
				pos = new Position(xrand, yrand);
			} while (!ret.isWalkable(pos));
			//System.out.println("Placing "+item.getAttributesDescription());
			ret.addItem(pos, item);
		}
	}
	
	public static void lightCandles(Level l){
		int candles = Util.rand(25,35);
		for (int i = 0; i < candles; i++){
			int xrnd = Util.rand(1, l.getWidth() -1);
			int yrnd = Util.rand(1, l.getHeight() -1);
			if (l.getMapCell(xrnd, yrnd, 0).isSolid()){
				i--;
				continue;
			}
				
			Feature vFeature = FeatureFactory.getFactory().buildFeature("CANDLE");
			vFeature.setPosition(xrnd,yrnd,0);
			l.addFeature(vFeature);
			
		}
	}
	
}