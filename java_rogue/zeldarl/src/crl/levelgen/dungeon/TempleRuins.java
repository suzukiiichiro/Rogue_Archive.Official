package crl.levelgen.dungeon;

import java.util.ArrayList;
import java.util.Hashtable;

import crl.levelgen.MonsterSpawnInfo;

import sz.util.Util;

public class TempleRuins implements DungeonData{
	
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
		"ZOL", "MOUSE", "GEL", "KEESE"	
	};
	
	public MonsterSpawnInfo[] getInhabitants() {
		return inhabitants;
	}
	
	private MonsterSpawnInfo[] inhabitants = new MonsterSpawnInfo[]{
			new MonsterSpawnInfo("ZOL", MonsterSpawnInfo.UNDERGROUND, 20),
			new MonsterSpawnInfo("GEL", MonsterSpawnInfo.UNDERGROUND, 30),
			new MonsterSpawnInfo("KEESE", MonsterSpawnInfo.UNDERGROUND, 30),
			new MonsterSpawnInfo("MOUSE", MonsterSpawnInfo.UNDERGROUND, 30)
	};
	
	public String[] getDwellers(){
		return INHABITANTS;
	}
	
	private ArrayList fills = new ArrayList();
	private ArrayList obstacles = new ArrayList();
	private ArrayList prizes = new ArrayList();
	
	{
		obstacles.add("D_LOCKEDROOM");
		obstacles.add("D_LOCKEDROOM");
		obstacles.add("BOWROOM");
		
		prizes.add("KEYROOM");
		prizes.add("KEYROOM");
		prizes.add("EMPTYROOM");
		
		fills.add("COMPASSROOM");
		int empties = Util.rand(9,11);
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
	charMap.put("#", "RUINSWALL");
	charMap.put(".", "RUINSFLOOR");
	charMap.put("=", "REDCARPET");
	charMap.put("/", "DUNGEON_DOOR");
	charMap.put("W", "WATER");
	charMap.put("B", "STATUE");
	charMap.put("P", "RUINEDPILLAR");
	charMap.put(">", "STAIRSDOWN");
	charMap.put("<", "STAIRSUP");
	charMap.put("!", "RUINSFLOOR PRIZE POTION");
	
	charMap.put("S", "REDCARPET EXIT _START");
	charMap.put("O", "REDCARPET EXIT OVERWORLD");
	if (Util.chance(50))
		charMap.put("D", "RUINSFLOOR MONSTER GOLEM_GUARD");
	else
		charMap.put("D", "RUINSFLOOR MONSTER ARMOS_KNIGHT");
	
	charMap.put("$", "DUNGEON_DOOR FEATURE LOCKED_DOOR");
	charMap.put("%", "DUNGEON_DOOR FEATURE BOSS_DOOR");
	charMap.put("&", "DUNGEON_DOOR FEATURE TREASURE_DOOR");
	
	charMap.put("k", "RUINSFLOOR FEATURE SMALL_KEY");
	charMap.put("K", "REDCARPET FEATURE BIG_KEY");
	charMap.put("H", "REDCARPET FEATURE HEARTCONTAINER");
	charMap.put(")", "REDCARPET ITEM BOW");
	charMap.put("C", "RUINSFLOOR FEATURE COMPASS");
	charMap.put("c", "RUINSFLOOR FEATURE CANE_BYRNA");
	}
	
	public String[] startRoom = new String []{
			
				"#####################",
				"##..B...B...B...B..##",
				"#...................#",
				"#.....P.=====.P.....#",
				"#.......=====.......#",
				"#.......=====.......#",
				"#.....P.=====.P.....#",
				"#.........=.........#",
				"#........#S#........#",
				"##......##O##......##",
				"#####################",
			
	};
	
	public String[] bossRoom = new String []{
			
			"#####################",
			"##.................##",
			"#..===============..#",
			"#..=.P.........P.=..#",
			"#..=.............=..#",
			"#..=.....D.......=..#",
			"#..=.............=..#",
			"#..=.P.........P.=..#",
			"#..===============..#",
			"##.................##",
			"#####################",
		
	};
	
	public String[] prizeRoom = new String []{
			
			"#####################",
			"##.................##",
			"#..===============..#",
			"#..===============..#",
			"#..===============..#",
			"#..==H====c=======..#",
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
			"##.................##",
			"#....B.B.....B.B....#",
			"#...B...B...B...B...#",
			"#....B.B.....B.B....#",
			"#...................#",
			"#....B.B.....B.B....#",
			"#...B...B...B...B...#",
			"#....B.B.....B.B....#",
			"##.................##",
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
