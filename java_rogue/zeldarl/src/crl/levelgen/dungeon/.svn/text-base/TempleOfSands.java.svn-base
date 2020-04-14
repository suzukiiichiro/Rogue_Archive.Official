package crl.levelgen.dungeon;

import java.util.ArrayList;
import java.util.Hashtable;

import crl.levelgen.MonsterSpawnInfo;

import sz.util.Util;

public class TempleOfSands implements DungeonData{
	
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
		"MOLDORM", "ROPE", "RED_DARKNUT", "LIKELIKE", "BLUE_GORIRA", "SKELETON", "LANDMOLA"
	};
	
	public MonsterSpawnInfo[] getInhabitants() {
		return inhabitants;
	}
	
	private MonsterSpawnInfo[] inhabitants = new MonsterSpawnInfo[]{
			new MonsterSpawnInfo("MOLDORM", MonsterSpawnInfo.UNDERGROUND, 20),
			new MonsterSpawnInfo("ROPE", MonsterSpawnInfo.UNDERGROUND, 70),
			new MonsterSpawnInfo("RED_DARKNUT", MonsterSpawnInfo.UNDERGROUND, 10),
			new MonsterSpawnInfo("BLUE_GORIRA", MonsterSpawnInfo.UNDERGROUND, 50),
			new MonsterSpawnInfo("SKELETON", MonsterSpawnInfo.UNDERGROUND, 60),
			new MonsterSpawnInfo("LANDMOLA", MonsterSpawnInfo.UNDERGROUND, 30)
	};
	
	public String[] getDwellers(){
		return INHABITANTS;
	}
	
	private ArrayList fills = new ArrayList();
	private ArrayList obstacles = new ArrayList();
	private ArrayList prizes = new ArrayList();
	
	{
		int keys = Util.rand(3,5);
		for (int i = 0; i < keys; i++){
			obstacles.add("D_LOCKEDROOM");
			prizes.add("KEYROOM");
		}
		
		obstacles.add("FLIPPERSROOM");
		prizes.add("EMPTYROOM");
		
		fills.add("COMPASSROOM");
		int empties = Util.rand(6,7);
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
	charMap.put("#", "SANDTEMPLEWALL");
	charMap.put(".", "SANDTEMPLEFLOOR");
	charMap.put("=", "YELLOWCARPET");
	charMap.put("/", "DUNGEON_DOOR");
	charMap.put("W", "SAND");
	charMap.put("B", "SANDTEMPLESTATUE");
	charMap.put("P", "SANDTEMPLEPILLAR");
	charMap.put(">", "STAIRSDOWN");
	charMap.put("<", "STAIRSUP");
	charMap.put("!", "SANDTEMPLEFLOOR PRIZE POTION");
	
	charMap.put("S", "YELLOWCARPET EXIT _START");
	charMap.put("O", "YELLOWCARPET EXIT OVERWORLD");
	if (Util.chance(50))
		charMap.put("D", "SANDTEMPLEFLOOR MONSTER GIANT_MOLDORM");
	else
		charMap.put("D", "SANDTEMPLEFLOOR MONSTER GIANT_LANDMOLA");
	
	charMap.put("$", "DUNGEON_DOOR FEATURE LOCKED_DOOR");
	charMap.put("%", "DUNGEON_DOOR FEATURE BOSS_DOOR");
	charMap.put("&", "DUNGEON_DOOR FEATURE TREASURE_DOOR");
	
	charMap.put("k", "SANDTEMPLEFLOOR FEATURE SMALL_KEY");
	charMap.put("K", "YELLOWCARPET FEATURE BIG_KEY");
	charMap.put("H", "YELLOWCARPET FEATURE HEARTCONTAINER");
	charMap.put(")", "YELLOWCARPET FEATURE FLIPPERS");
	charMap.put("C", "SANDTEMPLEFLOOR FEATURE COMPASS");
	charMap.put("c", "YELLOWCARPET FEATURE CANE_YENDOR");
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
			"#..=======H=======..#",
			"#..===============..#",
			"#..=======c=======..#",
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
