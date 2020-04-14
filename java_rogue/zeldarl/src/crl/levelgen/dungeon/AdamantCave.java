package crl.levelgen.dungeon;

import java.util.ArrayList;
import java.util.Hashtable;

import crl.levelgen.MonsterSpawnInfo;

import sz.util.Util;

public class AdamantCave implements DungeonData{
	
	public ArrayList getFills() {
		return fills;
	}
	
	public ArrayList getFirstObstacles() {
		return obstacles;
	}
	
	public ArrayList getFirstPrizes() {
		return prizes;
	}
	
	public MonsterSpawnInfo[] getInhabitants() {
		return inhabitants;
	}
	
	private MonsterSpawnInfo[] inhabitants = new MonsterSpawnInfo[]{
			new MonsterSpawnInfo("MOUSE", MonsterSpawnInfo.UNDERGROUND, 30),
			new MonsterSpawnInfo("GEL", MonsterSpawnInfo.UNDERGROUND, 30),
			new MonsterSpawnInfo("KEESE", MonsterSpawnInfo.UNDERGROUND, 30)
	};
	
	public String[] getDwellers(){
		return INHABITANTS;
	}
	
	private String[] INHABITANTS = new String[]{
		"MOUSE", "GEL", "KEESE"	
	};
	
	private ArrayList fills = new ArrayList();
	private ArrayList obstacles = new ArrayList();
	private ArrayList prizes = new ArrayList();
	
	{
		/*obstacles.add("D_LOCKEDROOM");
		obstacles.add("D_LOCKEDROOM");
		obstacles.add("BOOMERANGROOM");
		
		prizes.add("KEYROOM");
		prizes.add("KEYROOM");
		prizes.add("EMPTYROOM");
		
		fills.add("COMPASSROOM");
		int empties = Util.rand(1,2);
		for (int i = 0; i < empties; i++){
			fills.add("EMPTYROOM");
		}*/
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
	charMap.put("#", "ADAMANTWALL");
	charMap.put(".", "ADAMANTFLOOR");
	charMap.put("=", "ADAMANTFLOOR");
	charMap.put("/", "DUNGEON_DOOR");
	charMap.put("W", "WATER");
	charMap.put("B", "ADAMANTWALL");
	charMap.put("P", "ADAMANTWALL");
	charMap.put(">", "STAIRSDOWN");
	charMap.put("<", "STAIRSUP");
	
	charMap.put("S", "ADAMANTFLOOR EXIT _START");
	
	charMap.put("D", "ADAMANTFLOOR MONSTER LIGHT_SHADE");
	charMap.put("i", "FIREPIT");
	
	charMap.put("$", "DUNGEON_DOOR FEATURE LOCKED_DOOR");
	charMap.put("%", "DUNGEON_DOOR FEATURE BOSS_DOOR");
	charMap.put("&", "DUNGEON_DOOR FEATURE TREASURE_DOOR");
	
	charMap.put("k", "ADAMANTFLOOR FEATURE SMALL_KEY");
	charMap.put("K", "ADAMANTFLOOR FEATURE BIG_KEY");
	charMap.put(")", "ADAMANTFLOOR FEATURE SWORD");
	charMap.put("(", "ADAMANTFLOOR FEATURE BOOMERANG");
	charMap.put("C", "ADAMANTFLOOR FEATURE COMPASS");
	charMap.put("H", "ADAMANTFLOOR FEATURE HEARTCONTAINER");
	charMap.put("c", "ADAMANTFLOOR FEATURE CANE_BYRNA");
	}
	
	public String[] startRoom = new String []{
			
				"#####################",
				"#########...##WWWWW##",
				"#..###...........WWW#",
				"#.......#####.......#",
				"#.......#...#.......#",
				"#.........S.#.......#",
				"#.......#...#.......#",
				"#.......#####WWWW...#",
				"#....WW..WWWWW###...#",
				"##..WW##......#######",
				"#####################",
			
	};
	
	public String[] bossRoom = new String []{
			
			"#####################",
			"##.................##",
			"#......i...i........#",
			"#...................#",
			"#...i.........i.....#",
			"#........D..........#",
			"#...i.........i.....#",
			"#...................#",
			"#......i...i........#",
			"##.................##",
			"#####################",
		
	};
	
	public String[] prizeRoom = new String []{
			
			"#####################",
			"##.................##",
			"#..===============..#",
			"#..===============..#",
			"#..===============..#",
			"#..=======c=======..#",
			"#..===============..#",
			"#..=======H=======..#",
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

