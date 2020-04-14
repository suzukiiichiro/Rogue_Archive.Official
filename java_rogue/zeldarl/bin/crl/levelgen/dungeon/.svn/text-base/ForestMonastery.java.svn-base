package crl.levelgen.dungeon;

import java.util.ArrayList;
import java.util.Hashtable;

import crl.levelgen.MonsterSpawnInfo;

import sz.util.Util;

public class ForestMonastery implements DungeonData{
	
	public ArrayList getFills() {
		return fills;
	}
	
	public ArrayList getFirstObstacles() {
		return obstacles;
	}
	
	public ArrayList getFirstPrizes() {
		return prizes;
	}
	
	private String[] INHABITANTS = new String[]{
		"ZOL", "VIRE", "GEL", "KEESE", "RED_GORIRA", "POLS", "SKELETON"
	};
	
	public MonsterSpawnInfo[] getInhabitants() {
		return inhabitants;
	}
	
	private MonsterSpawnInfo[] inhabitants = new MonsterSpawnInfo[]{
			new MonsterSpawnInfo("ZOL", MonsterSpawnInfo.UNDERGROUND, 20),
			new MonsterSpawnInfo("GEL", MonsterSpawnInfo.UNDERGROUND, 30),
			new MonsterSpawnInfo("KEESE", MonsterSpawnInfo.UNDERGROUND, 30),
			new MonsterSpawnInfo("VIRE", MonsterSpawnInfo.UNDERGROUND, 30),
			new MonsterSpawnInfo("RED_GORIRA", MonsterSpawnInfo.UNDERGROUND, 30),
			new MonsterSpawnInfo("SKELETON", MonsterSpawnInfo.UNDERGROUND, 30)
	};
	
	public String[] getDwellers(){
		return INHABITANTS;
	}
	
	private ArrayList fills = new ArrayList();
	private ArrayList obstacles = new ArrayList();
	private ArrayList prizes = new ArrayList();
	
	{
		int keys = Util.rand(2,4);
		for (int i = 0; i < keys; i++){
			obstacles.add("D_LOCKEDROOM");
			prizes.add("KEYROOM");
		}
		
		obstacles.add("HOOKSHOOTROOM");
		prizes.add("EMPTYROOM");
		
		fills.add("COMPASSROOM");
		
		int empties = Util.rand(7,9);
		for (int i = 0; i < empties; i++){
			fills.add("EMPTYROOM");
		}
		int potions = Util.rand(2,3);
		for (int i = 0; i < potions; i++){
			fills.add("POTIONROOM");
		}
	}
	
	
	public String[][] getEmptyRooms() {
		return emptyRooms;
	}
	
	public String[] getStartRoom() {
		return startRoom;
	}
	
	public String[] getBossRoom() {
		return bossRoom;
	}
	
	public Hashtable getCharMap() {
		return charMap;
	}
	
	public String[] getPrizeRoom() {
		return prizeRoom;
	}
	private Hashtable charMap = new Hashtable();
	{
	charMap.put("#", "FORESTTEMPLEWALL");
	charMap.put(".", "FORESTTEMPLEFLOOR");
	charMap.put("=", "GREENCARPET");
	charMap.put("!", "FORESTTEMPLEFLOOR PRIZE POTION");
	charMap.put("/", "DUNGEON_DOOR");
	charMap.put("W", "FORESTTEMPLEWATER");
	charMap.put("B", "FORESTTEMPLESTATUE");
	charMap.put("P", "FORESTTEMPLEPILLAR");
	charMap.put(">", "STAIRSDOWN");
	charMap.put("<", "STAIRSUP");
	
	charMap.put("S", "GREENCARPET EXIT _START");
	charMap.put("O", "GREENCARPET EXIT OVERWORLD");
	if (Util.chance(50))
		charMap.put("D", "FORESTTEMPLEFLOOR MONSTER DODONGO");
	else
		charMap.put("D", "FORESTTEMPLEFLOOR MONSTER MOSS_SHADE");
	
	charMap.put("$", "DUNGEON_DOOR FEATURE LOCKED_DOOR");
	charMap.put("%", "DUNGEON_DOOR FEATURE BOSS_DOOR");
	charMap.put("&", "DUNGEON_DOOR FEATURE TREASURE_DOOR");
	
	charMap.put("k", "FORESTTEMPLEFLOOR FEATURE SMALL_KEY");
	charMap.put("K", "GREENCARPET FEATURE BIG_KEY");
	charMap.put("H", "GREENCARPET FEATURE HEARTCONTAINER");
	charMap.put(")", "GREENCARPET FEATURE HOOKSHOOT");
	charMap.put("C", "FORESTTEMPLEFLOOR FEATURE COMPASS");
	charMap.put("c", "FORESTTEMPLEFLOOR FEATURE CANE_SOMARIA");
	}
	
	public String[] startRoom = new String []{
			
				"#####################",
				"##WWWWWWWWWWWWWWWWW##",
				"#WWWW...........WWWW#",
				"#...................#",
				"#...................#",
				"#......WWWWWWW......#",
				"#.....WWWWWWWWW.....#",
				"#.....WWW#.#WWW.....#",
				"#.....WWW#S#WWW.....#",
				"##...WWWW#O#WWWW...##",
				"#####################",
			
	};
	
	public String[] bossRoom = new String []{
			
			"#####################",
			"##...B.........B...##",
			"#..WWWWWWWWWWWWWWW..#",
			"#..WWWWWWWWWWWWWWW..#",
			"#B.WWWWWWWWWWWWWWW.B#",
			"#..WWWWWWWDWWWWWWW..#",
			"#B.WWWWWWWWWWWWWWW.B#",
			"#..WWWWWWWWWWWWWWW..#",
			"#..WWWWWWWWWWWWWWW..#",
			"##...B.........B...##",
			"#####################",
		
	};
	
	public String[] prizeRoom = new String []{
			
			"#####################",
			"##.................##",
			"#..===============..#",
			"#..===============..#",
			"#..===============..#",
			"#..=======c====H==..#",
			"#..===============..#",
			"#..===============..#",
			"#..===============..#",
			"##.................##",
			"#####################",
		
	};
	
	public String [][] emptyRooms = new String [][]{
		{
			"#####################",
			"##.................##",
			"#...................#",
			"#...................#",
			"#...................#",
			"#.........B.........#",
			"#...................#",
			"#...................#",
			"#...................#",
			"##.................##",
			"#####################",
		},{
			"#####################",
			"##.................##",
			"#...................#",
			"#..B.............B..#",
			"#...................#",
			"#...................#",
			"#...................#",
			"#..B.............B..#",
			"#...................#",
			"##.................##",
			"#####################",
		},{
			"#####################",
			"##.................##",
			"#...BBBB.....BBBB...#",
			"#...................#",
			"#...................#",
			"#...................#",
			"#...................#",
			"#...................#",
			"#...BBBB.....BBBB...#",
			"##.................##",
			"#####################",
		},{
			"#####################",
			"##.................##",
			"#...................#",
			"#.......B...B.......#",
			"#...................#",
			"#.........P.........#",
			"#...................#",
			"#.......B...B.......#",
			"#...................#",
			"##.................##",
			"#####################",
		},{
			"#####################",
			"##WWWWWWWWWWWWWWWWW##",
			"#WWWWWWWWWWWWWWWWWWW#",
			"#WWWWWWWWWWWWWWWWWWW#",
			"#WWWWWWWWWWWWWWWWWWW#",
			"#WWWWWWWWWWWWWWWWWWW#",
			"#WWWWWWWWWWWWWWWWWWW#",
			"#WWWWWWWWWWWWWWWWWWW#",
			"#WWWWWWWWWWWWWWWWWWW#",
			"##WWWWWWWWWWWWWWWWW##",
			"#####################",
		},{
			"#####################",
			"##.................##",
			"#...................#",
			"#...PP.........PP...#",
			"#...PB.........BP...#",
			"#...................#",
			"#...PB.........BP...#",
			"#...PP.........PP...#",
			"#...................#",
			"##.................##",
			"#####################",
		},{
			"#####################",
			"#B.................B#",
			"#...................#",
			"#..WWWWWW...WWWWWW..#",
			"#..W.............W..#",
			"#..W.WWWWWWWWWWW.W..#",
			"#..W.............W..#",
			"#..WWWWWW...WWWWWW..#",
			"#...................#",
			"#B.................B#",
			"#####################",
		},{
			"#####################",
			"##.................##",
			"#...................#",
			"#....###########....#",
			"#...................#",
			"#...................#",
			"#...................#",
			"#....###########....#",
			"#...................#",
			"##.................##",
			"#####################",
		},{
			"#####################",
			"##....#............##",
			"#.....#.............#",
			"#.....#...#.........#",
			"#.....#...#.........#",
			"#.........#.........#",
			"#.........#...#.....#",
			"#.........#...#.....#",
			"#.............#.....#",
			"##............#....##",
			"#####################",
		},{
			"#####################",
			"##.................##",
			"#...................#",
			"#....P####P.........#",
			"#...................#",
			"#...................#",
			"#...................#",
			"#.........P####P....#",
			"#...................#",
			"##.................##",
			"#####################",
		},{
			"#####################",
			"##.................##",
			"#.B...............B.#",
			"#......WWWWWWW......#",
			"#...................#",
			"#.......WWWWW.......#",
			"#...................#",
			"#......WWWWWWW......#",
			"#.B...............B.#",
			"##.................##",
			"#####################",
		},{
			"#####################",
			"##.................##",
			"#.B...............B.#",
			"#...................#",
			"#.......WWWWW.......#",
			"#......WWWWWWW......#",
			"#.......WWWWW.......#",
			"#...................#",
			"#.B...............B.#",
			"##.................##",
			"#####################",
		},{
			"#####################",
			"##.................##",
			"#....WWW............#",
			"#....WPW............#",
			"#....WWW............#",
			"#...................#",
			"#.............WWW...#",
			"#.............WPW...#",
			"#.............WWW...#",
			"##.................##",
			"#####################",
		},{
			"#####################",
			"##.................##",
			"#...................#",
			"#...................#",
			"#...................#",
			"#...................#",
			"#...................#",
			"#...................#",
			"#...................#",
			"##.................##",
			"#####################",
		}
		};
}
