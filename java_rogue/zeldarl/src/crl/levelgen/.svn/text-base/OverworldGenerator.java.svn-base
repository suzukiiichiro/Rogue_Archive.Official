package crl.levelgen;

import java.awt.Color;
import java.awt.Graphics;
import java.util.Hashtable;

import javax.swing.JFrame;
import javax.swing.JPanel;

import sz.ca.CARandomInitializer;
import sz.ca.CARule;
import sz.ca.Matrix;
import sz.ca.SZCA;
import sz.util.Position;
import sz.util.Util;
import crl.game.CRLException;
import crl.game.Game;
import crl.level.Cell;
import crl.level.Level;
import crl.level.MapCellFactory;
import crl.level.SpawnPoint;
import crl.levelgen.cave.Wisp;
import crl.levelgen.cave.WispSim;

public class OverworldGenerator extends LevelGenerator{
	private int xsize, ysize;
	//private int[] dungeons, towns;
	
	
	private final int
		GRASS = 0,
		WATER = 1,
		FOREST = 2,
		MOUNTAIN = 3,
		DESERT = 4,
		FORESTDUNGEON = 5,
		DESERTDUNGEON = 6,
		RUINSDUNGEON = 7,
		KAKARIKO = 8,
		LINKS = 9,
		SHALLOW_WATER = 10,
		FOREST_TREE = 11,
		CACTUS = 12,
		PLAINS_TREE = 13,
		ANCIENTTEMPLE = 14,
		BUSH = 15,
		HEARTCONTAINER = 16;
	
	
	private void setup(){
		xsize = 128;
		ysize = 128;
		/*dungeons = new int[]{FORESTDUNGEON, DESERTDUNGEON, RUINSDUNGEON};
		towns = new int[]{KAKARIKO, LINKS};*/
	}
	
	public Level generateLevel(){
		int[][] matrix = generateLevelMatrix();
		String[] strCells = new String[matrix[0].length];
		for (int y = 0; y < matrix.length; y++){
			strCells[y] = "";
			for (int x = 0; x < matrix[0].length; x++) {
				String strCell = null;
				switch (matrix[x][y]){
				case GRASS:
					strCell = "."; break;
				case FOREST:
					strCell = "&"; break;
				case MOUNTAIN:
					strCell = "^"; break;
				case DESERT:
					strCell = "x"; break;
				case WATER:
					strCell = "~"; break;
				case FORESTDUNGEON:
					strCell = "1"; break;
				case DESERTDUNGEON:
					strCell = "2"; break;
				case RUINSDUNGEON:
					strCell = "3"; break;
				case KAKARIKO:
					strCell = "4"; break;
				case LINKS:
					strCell = "5"; break;
				case SHALLOW_WATER:
					strCell = "s"; break;
				case FOREST_TREE:
					strCell = "f"; break;
				case CACTUS:
					strCell = "c"; break;
				case PLAINS_TREE:
					strCell = "p"; break;
				case ANCIENTTEMPLE:
					strCell = "6"; break;
				case BUSH:
					strCell = "*"; break;
				case HEARTCONTAINER:
					strCell = "H"; break;
				}
				strCells[y] += strCell;
			}
		}
		Level level = new Level();
		level.setCells(new Cell[1][xsize][ysize]);
		StaticGenerator.getGenerator().renderOverLevel(level, strCells, charMap, new Position(0, 0, 0), 0);
		
		//Wisp Sims
		Cell GRASSCELL = null;
		Cell SWALLOWCELL = null;
		
		try {
			GRASSCELL = MapCellFactory.getMapCellFactory().getMapCell("GRASS");
			SWALLOWCELL = MapCellFactory.getMapCellFactory().getMapCell("SHALLOW_WATER");
		} catch (CRLException crle){
			Game.crash("OverworldGenerator", crle);
		}
			WispSim.setWisps(new Wisp(linkPos, 120,30,2),new Wisp(dungeon3Pos, 100,30,3));
			WispSim.run(level.getCells()[0], GRASSCELL, SWALLOWCELL);
			WispSim.setWisps(new Wisp(dungeon3Pos, 120,30,2),new Wisp(ancientPos, 100,30,3));
			WispSim.run(level.getCells()[0], GRASSCELL, SWALLOWCELL);
			WispSim.setWisps(new Wisp(ancientPos, 120,30,2),new Wisp(dungeon2Pos, 100,30,3));
			WispSim.run(level.getCells()[0], GRASSCELL, SWALLOWCELL);
			WispSim.setWisps(new Wisp(dungeon2Pos, 120,30,2),new Wisp(dungeon1Pos, 100,30,3));
			WispSim.run(level.getCells()[0], GRASSCELL, SWALLOWCELL);
			WispSim.setWisps(new Wisp(dungeon1Pos, 120,30,2),new Wisp(kakarikoPos, 100,30,3));
			WispSim.run(level.getCells()[0], GRASSCELL, SWALLOWCELL);
		

		//Spawn Points
		int spawnPoints = (int)((xsize*ysize)/100.0D);
		for (int i = 0; i < spawnPoints; i++){
			int x = Util.rand(5, xsize-5);
			int y = Util.rand(5, ysize-5);
			switch (matrix[x][y]){
			case GRASS:
				level.addSpawnPoint(new SpawnPoint(new String[]{"CROW", "LEEVER", "MOBLIN", "OCTOROC"}, new Position(x,y)));
				break;
			case WATER:
				level.addSpawnPoint(new SpawnPoint(new String[]{"ZOLA"}, new Position(x,y)));
				break;
			case FOREST:
				level.addSpawnPoint(new SpawnPoint(new String[]{"CROW", "LEEVER", "ARCHER"}, new Position(x,y)));
				break;
			case MOUNTAIN:
				level.addSpawnPoint(new SpawnPoint(new String[]{"CROW"}, new Position(x,y)));
				break;
			case DESERT:
				level.addSpawnPoint(new SpawnPoint(new String[]{"TEKTITE", "PEAHAT", "LYNEL"}, new Position(x,y)));
				break;
			}
		}
		
		// Debug
		/*JFrame jf = new JFrame();
		jf.getContentPane().add(new PanelMatrixCell(level.getCells()[0]));
		jf.setSize(800,600);
		jf.setVisible(true);
		jf.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);*/
		
		return level;
	}
	
	private Hashtable charMap = new Hashtable();
	{
		charMap.put(".", "GRASS");
		charMap.put("*", "GRASS FEATURE BUSH");
		charMap.put("&", "FOREST");
		charMap.put("^", "MOUNTAIN");
		charMap.put("x", "DESERT");
		charMap.put("~", "LAKE");
		charMap.put("1", "FORESTDUNGEON EXIT_FEATURE FORESTDUNGEON FORESTDUNGEON");
		charMap.put("2", "DESERTDUNGEON EXIT_FEATURE DESERTDUNGEON DESERTDUNGEON");
		charMap.put("3", "RUINSDUNGEON EXIT_FEATURE RUINSDUNGEON RUINSDUNGEON");
		charMap.put("4", "KAKARIKO FEATURE KAKARIKO");
		charMap.put("5", "LINKS EXIT_FEATURE LINKS LINKS");
		charMap.put("6", "ANCIENTTEMPLE EXIT_FEATURE ANCIENTTEMPLE ANCIENTTEMPLE");
		charMap.put("s", "SHALLOW_WATER");
		charMap.put("f", "FOREST_TREE");
		charMap.put("c", "CACTUS");
		charMap.put("p", "PLAINS_TREE");
		charMap.put("H", "MOUNTAIN FEATURE HEARTCONTAINER");
	}
	private Position dungeon1Pos = null;
	private Position dungeon2Pos = null;
	private Position dungeon3Pos = null;
	private Position ancientPos = null;
	private Position kakarikoPos = null;
	private Position linkPos = null;
	
	public int[][] generateLevelMatrix(){
		setup();
		/** Uses ZeldaOverworld Cellular automata, by SZ
		 
		 */
		
		CARandomInitializer vInit = new CARandomInitializer(new double [] {0.65, 0.0, 0.05, 0.2}, MOUNTAIN);
		//GRASS, WATER, FOREST, MOUNTAIN, DESERT,
		CARule [] vRules = new CARule []{
			new CARule(GRASS, CARule.MORE_THAN, 2, FOREST, FOREST),
			new CARule(GRASS, CARule.MORE_THAN, 2, WATER, WATER),
			new CARule(GRASS, CARule.MORE_THAN, 1, DESERT, DESERT),
			new CARule(GRASS, CARule.MORE_THAN, 3, MOUNTAIN, MOUNTAIN),
			
			new CARule(WATER, CARule.MORE_THAN, 5, GRASS, GRASS),
			
			//new CARule(DESERT, CARule.MORE_THAN, 3, DESERT, GRASS),
			new CARule(DESERT, CARule.MORE_THAN, 0, WATER, GRASS),
			new CARule(DESERT, CARule.MORE_THAN, 5, GRASS, GRASS),
			new CARule(DESERT, CARule.MORE_THAN, 7, MOUNTAIN, MOUNTAIN),
			
			new CARule(FOREST, CARule.MORE_THAN, 4, MOUNTAIN, MOUNTAIN),
			
			new CARule(MOUNTAIN, CARule.LESS_THAN,1, MOUNTAIN, GRASS),
			new CARule(DESERT, CARule.LESS_THAN, 1, DESERT, GRASS),
			new CARule(WATER, CARule.LESS_THAN, 1, WATER, GRASS),
			new CARule(FOREST, CARule.LESS_THAN, 1, FOREST, GRASS),
			
			new CARule(GRASS, CARule.MORE_THAN, 0, DESERTDUNGEON, DESERT),
			new CARule(FOREST, CARule.MORE_THAN, 0, DESERTDUNGEON, DESERT),
			new CARule(MOUNTAIN, CARule.MORE_THAN, 0, DESERTDUNGEON, DESERT),
			new CARule(WATER, CARule.MORE_THAN, 0, DESERTDUNGEON, DESERT),
			
			
			new CARule(MOUNTAIN, CARule.MORE_THAN, 0, FORESTDUNGEON, GRASS),
			//new CARule(MOUNTAIN, CARule.MORE_THAN, 0, DESERTDUNGEON, GRASS),
			new CARule(MOUNTAIN, CARule.MORE_THAN, 0, RUINSDUNGEON, GRASS),
			new CARule(MOUNTAIN, CARule.MORE_THAN, 0, KAKARIKO, GRASS),
			new CARule(MOUNTAIN, CARule.MORE_THAN, 0, LINKS, GRASS),
		};
		Matrix map = new Matrix(xsize,ysize);
		vInit.init(map);
		int [][] ret = map.getArrays();
		
		
		out: do {
			dungeon1Pos = new Position(Util.rand(20,xsize-20), Util.rand(20,ysize-20));
			dungeon2Pos = new Position(Util.rand(20,xsize-20), Util.rand(20,ysize-20));
			dungeon3Pos = new Position(Util.rand(20,xsize-20), Util.rand(20,ysize-20));
			ancientPos = new Position(Util.rand(20,xsize-20), Util.rand(20,ysize-20));
			kakarikoPos = new Position(Util.rand(20,xsize-20), Util.rand(20,ysize-20));
			linkPos = new Position(Util.rand(20,xsize-20), Util.rand(20,ysize-20));
			Position[] checks = new Position[]{dungeon1Pos,dungeon2Pos,dungeon3Pos,kakarikoPos, linkPos, ancientPos};
			for (int i = 0; i < checks.length; i++){
				if (ret[checks[i].x][checks[i].y] == WATER)
					continue out;
				in: for (int j = 0; j < checks.length; j++){
					if (i == j)
						continue in;
					if (Position.distance(checks[i], checks[j]) < 40){
						continue out;
					}
				}
			}
			break;
		} while (true);
		
		map.addShoweredHotSpot(DESERT, DESERT, 200, 70);
		map.addShoweredHotSpot(WATER, WATER, 200, 20);
		map.addShoweredHotSpot(WATER, WATER, 400, 20);
		map.addShoweredHotSpot(WATER, WATER, 400, 30);
		map.addShoweredHotSpot(FOREST, FOREST, 300, 10);
		map.addShoweredHotSpot(dungeon1Pos, DESERTDUNGEON, DESERT, 300, 15);
		map.addShoweredHotSpot(dungeon2Pos, FORESTDUNGEON, FOREST, 400, 20);
		map.addHotSpot(dungeon3Pos, RUINSDUNGEON);
		map.addHotSpot(ancientPos, ANCIENTTEMPLE);
		map.addHotSpot(kakarikoPos, KAKARIKO);
		map.addHotSpot(linkPos, LINKS);
		
		SZCA.runCA(map, vRules, 10, false);
		
		CARule [] cleanupRules = new CARule []{
				new CARule(MOUNTAIN, CARule.LESS_THAN,2, MOUNTAIN, GRASS),
				//new CARule(DESERT, CARule.LESS_THAN, 2, DESERT, GRASS),
				//new CARule(WATER, CARule.LESS_THAN, 2, WATER, GRASS),
				new CARule(FOREST, CARule.LESS_THAN, 2, FOREST, GRASS),
			};
		SZCA.runCA(map, cleanupRules, 1, false);
		
		cleanupRules = new CARule []{
				new CARule(GRASS, CARule.MORE_THAN, 0, DESERT, DESERT),
				new CARule(GRASS, CARule.MORE_THAN, 0, FOREST, FOREST),
				new CARule(GRASS, CARule.MORE_THAN, 0, WATER, WATER),
			};
		SZCA.runCA(map, cleanupRules, 1, false);
		
		CARule [] detailRules = new CARule []{
			new CARule(WATER, CARule.MORE_THAN, 2, GRASS, SHALLOW_WATER),
			new CARule(WATER, CARule.MORE_THAN, 2, DESERT, SHALLOW_WATER),
			new CARule(WATER, CARule.MORE_THAN, 2, MOUNTAIN, SHALLOW_WATER),
			new CARule(WATER, CARule.MORE_THAN, 2, FOREST, SHALLOW_WATER),
			new CARule(WATER, CARule.MORE_THAN, 2, SHALLOW_WATER, SHALLOW_WATER),
		};
		SZCA.runCA(map, detailRules, 3, false);
		
		// Trees, Cactuses, Bushes
		
		int trees = (int)((xsize*ysize)/25.0D);
		for (int i = 0; i < trees; i++){
			int x = Util.rand(5, xsize-5);
			int y = Util.rand(5, ysize-5);
			if (ret[x][y] == FOREST)
				ret[x][y] = FOREST_TREE;
			else
				i--;
		}
		int cactus = (int)((xsize*ysize)/300.0D);
		for (int i = 0; i < cactus; i++){
			int x = Util.rand(5, xsize-5);
			int y = Util.rand(5, ysize-5);
			if (ret[x][y] == DESERT)
				ret[x][y] = CACTUS;
			else
				i--;
		}
		trees = (int)((xsize*ysize)/50.0D);
		for (int i = 0; i < trees; i++){
			int x = Util.rand(5, xsize-5);
			int y = Util.rand(5, ysize-5);
			if (ret[x][y] == GRASS)
				ret[x][y] = PLAINS_TREE;
			else
				i--;
		}
		
		trees = (int)((xsize*ysize)/200.0D);
		for (int i = 0; i < trees; i++){
			int x = Util.rand(5, xsize-5);
			int y = Util.rand(5, ysize-5);
			if (ret[x][y] == GRASS)
				ret[x][y] = BUSH;
			else
				i--;
		}
		
		trees = 10;
		for (int i = 0; i < trees; i++){
			int x = Util.rand(5, xsize-5);
			int y = Util.rand(5, ysize-5);
			if (ret[x][y] == MOUNTAIN)
				ret[x][y] = HEARTCONTAINER;
			else
				i--;
		}
		
		return ret;
	}
	
	public static void main(String args[]){
		OverworldGenerator owg = new OverworldGenerator();
		int res [][] = owg.generateLevelMatrix();
		JFrame jf = new JFrame();
		jf.getContentPane().add(new PanelMatrix(res));
		jf.setSize(800,600);
		jf.setVisible(true);
		jf.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	
}

class PanelMatrix extends JPanel{
	private int[][] m;
	PanelMatrix(int[][] m){
		this.m = m;
	}
	
	public void paintComponent(Graphics g){
		for (int x = 0; x < m.length; x++)
			for (int y = 0; y < m[0].length; y++){
				g.setColor(COLORS[m[x][y]]);
				if (m[x][y] >= 5)
					System.out.println("KAKARIKO!");
				g.fillRect(x*3,y*3,3,3);
			}
	}
	
	private Color[] COLORS = new Color[]{
		new Color(0,255,0), //GRASS
		new Color(0,0,150), //WATER
		new Color(0,100,0), //FOREST
		new Color(150,150,0), //MOUNTAIN
		new Color(250,250,0), //DESERT
		new Color(254,0,0), //FORESTDUNGEON
		new Color(254,0,0),
		new Color(254,0,0),
		new Color(254,0,0),
		new Color(254,0,0),
		new Color(0,0,200), //WATER
		new Color(0,200,0), //FOREST_TREE
		new Color(0,200,0), //CACTUS
		new Color(0,200,0), //PLAINS_TREE
		new Color(0,200,0), //PLAINS_TREE
		
		
	};
	
}

class PanelMatrixCell extends JPanel{
	private Cell[][] m;
	PanelMatrixCell(Cell[][] m){
		this.m = m;
	}
	
	public void paintComponent(Graphics g){
		for (int x = 0; x < m.length; x++)
			for (int y = 0; y < m[0].length; y++){
				g.setColor(getColor(m[x][y]));
				g.fillRect(x*3,y*3,3,3);
			}
	}
	
	private Color getColor(Cell c){
		if (c.getID().equals("GRASS"))
			return COLORS[0];
		if (c.getID().equals("LAKE"))
			return COLORS[1];
		if (c.getID().equals("FOREST"))
			return COLORS[2];
		if (c.getID().equals("MOUNTAIN"))
			return COLORS[3];
		if (c.getID().equals("DESERT"))
			return COLORS[4];
		if (c.getID().equals("SHALLOW_WATER"))
			return COLORS[10];
		if (c.getID().equals("FOREST_TREE"))
			return COLORS[11];
		if (c.getID().equals("CACTUS"))
			return COLORS[12];
		if (c.getID().equals("PLAINS_TREE"))
			return COLORS[13];
		return COLORS[7];
		
	}
	
	private Color[] COLORS = new Color[]{
		new Color(0,255,0), //GRASS
		new Color(0,0,150), //WATER
		new Color(0,100,0), //FOREST
		new Color(150,150,0), //MOUNTAIN
		new Color(250,250,0), //DESERT
		new Color(254,0,0), //FORESTDUNGEON
		new Color(254,0,0),
		new Color(254,0,0),
		new Color(254,0,0),
		new Color(254,0,0),
		new Color(0,0,200), //WATER
		new Color(0,200,0), //FOREST_TREE
		new Color(0,200,0), //CACTUS
		new Color(0,200,0), //PLAINS_TREE
		new Color(0,200,0), //PLAINS_TREE
		
		
	};
	
}