package crl.levelgen.dungeon;

import java.util.ArrayList;
import java.util.Hashtable;

import crl.levelgen.MonsterSpawnInfo;

import sz.util.Util;

public class TempleOfAncients implements DungeonData{
	
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
		"BLUE_DARKNUT", "STALFOS", "BLUE_GORIRA", "MOLDORM", "LIKELIKE", "BLUE_WIZZROBE"
	};
	
	public MonsterSpawnInfo[] getInhabitants() {
		return inhabitants;
	}
	
	private MonsterSpawnInfo[] inhabitants = new MonsterSpawnInfo[]{
			new MonsterSpawnInfo("BLUE_DARKNUT", MonsterSpawnInfo.UNDERGROUND, 60),
			new MonsterSpawnInfo("STALFOS", MonsterSpawnInfo.UNDERGROUND, 40),
			new MonsterSpawnInfo("MOLDORM", MonsterSpawnInfo.UNDERGROUND, 60),
			new MonsterSpawnInfo("BLUE_GORIRA", MonsterSpawnInfo.UNDERGROUND, 70),
			new MonsterSpawnInfo("BLUE_WIZZROBE", MonsterSpawnInfo.UNDERGROUND, 60),
	};
	
	public String[] getDwellers(){
		return INHABITANTS;
	}
	
	private ArrayList fills = new ArrayList();
	private ArrayList obstacles = new ArrayList();
	private ArrayList prizes = new ArrayList();
	
	{
		int keys = 2;
		for (int i = 0; i < keys; i++){
			obstacles.add("D_LOCKEDROOM");
			prizes.add("KEYROOM");
		}
		
		int empties = Util.rand(6,7);
		for (int i = 0; i < empties; i++){
			fills.add("EMPTYROOM");
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
	charMap.put("#", "RUINSWALL");
	charMap.put(".", "RUINSFLOOR");
	charMap.put("=", "GREENCARPET");
	charMap.put("/", "DUNGEON_DOOR");
	charMap.put("W", "FORESTTEMPLEWATER");
	charMap.put("B", "SANDTEMPLESTATUE");
	charMap.put("P", "SANDTEMPLEPILLAR");
	charMap.put(">", "STAIRSDOWN");
	charMap.put("<", "STAIRSUP");
	
	charMap.put("S", "GREENCARPET EXIT _START");
	charMap.put("O", "GREENCARPET EXIT OVERWORLD");
	charMap.put("D", "GREENCARPET FEATURE TREASURE_KEY");
	
	charMap.put("$", "DUNGEON_DOOR FEATURE LOCKED_DOOR");
	charMap.put("%", "DUNGEON_DOOR FEATURE BOSS_DOOR");
	charMap.put("&", "DUNGEON_DOOR FEATURE TREASURE_DOOR");
	
	charMap.put("k", "RUINSFLOOR FEATURE SMALL_KEY");
	charMap.put("K", "GREENCARPET FEATURE BIG_KEY");
	charMap.put("H", "GREENCARPET FEATURE HEARTCONTAINER");
	charMap.put("C", "RUINSFLOOR FEATURE COMPASS");
	
	charMap.put("1", "GREENCARPET FEATURE ALTARBYRNA");
	charMap.put("2", "GREENCARPET FEATURE ALTARSOMARIA");
	charMap.put("3", "RUINSFLOOR FEATURE ALTARYENDOR");
	
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
			"##.................##",
			"#...WWWWWW.WWWWWW...#",
			"#..WWWWWWW.WWWWWWW..#",
			"#..WWWWWWWWWWWWWWW..#",
			"#........WDW........#",
			"#..WWWWWWWWWWWWWWW..#",
			"#..WWWWWWW.WWWWWWW..#",
			"#...WWWWWW.WWWWWW...#",
			"##.................##",
			"#####################",
		
	};
	
	public String[] prizeRoom = new String []{
			
			"#####################",
			"##.................##",
			"#..===============..#",
			"#..=======2=======..#",
			"#..===============..#",
			"#..=======H=======..#",
			"#..===1=======3===..#",
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
