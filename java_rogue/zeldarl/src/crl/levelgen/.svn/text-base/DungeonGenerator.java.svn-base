package crl.levelgen;

import java.awt.Color;
import java.awt.Graphics;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JPanel;

import sz.util.Position;
import sz.util.Util;

import crl.feature.Feature;
import crl.level.Cell;
import crl.level.Dispatcher;
import crl.level.Level;
import crl.levelgen.dungeon.DungeonData;

public class DungeonGenerator extends LevelGenerator{
	private DungeonData dungeonDataX;
	public Level generateLevel(DungeonData dungeon){
		this.dungeonDataX = dungeon;
		//System.out.println(dungeonDataX);
		ArrayList metadata = getRooms();
		
		RoomMetaData md0 = (RoomMetaData) metadata.get(0);
		int minx = md0.getXpos(), maxx = md0.getXpos();
		int miny = md0.getYpos(), maxy = md0.getYpos();
		int mindepth = md0.getDepth(), maxdepth = md0.getDepth();
		for (int i = 0; i < metadata.size(); i++){
			RoomMetaData md = (RoomMetaData) metadata.get(i);
			if (md.getXpos() > maxx)
				maxx = md.getXpos();
			if (md.getYpos() > maxy)
				maxy = md.getYpos();
			if (md.getXpos() < minx)
				minx = md.getXpos();
			if (md.getYpos() < miny)
				miny = md.getYpos();
			if (md.getDepth() < mindepth)
				mindepth = md.getDepth();
			if (md.getDepth() > maxdepth)
				maxdepth = md.getDepth();
		}
		/*System.out.println("minx "+minx);
		System.out.println("miny "+miny);
		System.out.println("maxx "+maxx);
		System.out.println("maxY "+maxy);
		System.out.println("maxDepth "+maxdepth);
		System.out.println("minDepth "+mindepth);*/
		
		int xrange = maxx - minx + 1;
		int yrange = maxy - miny + 1;
		int zrange = maxdepth - mindepth + 1;
		
		/*System.out.println("xrange "+xrange);
		System.out.println("yrange "+yrange);
		System.out.println("zrange "+zrange);*/
		
		RoomMetaData [][][] map = new RoomMetaData[xrange][yrange][zrange];
		for (int i = 0; i < metadata.size(); i++){
			RoomMetaData md = (RoomMetaData) metadata.get(i);
			md.setXpos(md.getXpos()-minx);
			md.setYpos(md.getYpos()-miny);
			md.setDepth(md.getDepth()-mindepth);
			map[md.getXpos()][md.getYpos()][md.getDepth()] = md;
		}
		Level level = new Level();
		level.setDispatcher(new Dispatcher());
		// Plot the rooms
		int roomWidth = 21;
		int roomHeight = 11;
		level.setCells(new Cell[zrange][xrange*roomWidth][yrange*roomHeight]);
		for (int z = 0; z < zrange; z++){
			for (int y = 0; y < yrange; y++){
				for (int x = 0; x < xrange; x++) {
					if (map[x][y][z] == null)
						continue;
					plotRoom(map[x][y][z], x*roomWidth, y*roomHeight, z, level);
				}
			}
		}
		//showLevel(level);
		return level;
	}
	
	private void showLevel(Level l){
		JFrame j = new JFrame();
		j.getContentPane().add(new DungeonPanelMatrix(l));
		j.setSize(800,800);
		j.setVisible(true);
		j.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	private void plotRoom(RoomMetaData room, int xpos, int ypos, int zpos, Level level){
		String [] stamp = getStampFor(room);
		Hashtable charMap = dungeonDataX.getCharMap();
		StaticGenerator.getGenerator().renderOverLevel(level, stamp, charMap, new Position(xpos, ypos, zpos), 0);
	}
	
	
	private String[] getStampFor(RoomMetaData room){
		String[] ret = null;
		//if (room.getRoomType().equals("EMPTYROOM") || room.getRoomType().equals("EMPTYROOM"))
		if (room.getRoomID().equals("START"))
			ret = dungeonDataX.getStartRoom();
		else if (room.getRoomType().equals("&BIGLOCK"))
			ret = dungeonDataX.getBossRoom();
		else if (room.getRoomType().equals("PRIZEROOM"))
			ret = dungeonDataX.getPrizeRoom();
		else {
			String[][] emptyRooms = dungeonDataX.getEmptyRooms();
			ret = (String[]) emptyRooms[Util.rand(0,emptyRooms.length-1)].clone();
		}
		
		if (room.getStairsDownRoom() != null){
			ret[3] = ret[3].substring(0,8)+"..>.."+ret[3].substring(13);
		}
		if (room.getStairsUpRoom() != null){
			ret[3] = ret[3].substring(0,8)+"..<.."+ret[3].substring(13);
		}
		
		if (room.getRoomType().equals("KEYROOM"))
			ret[5] = "#.........k.........#";
		if (room.getRoomType().equals("POTIONROOM"))
			ret[5] = "#.........!.........#";
		if (room.getRoomType().equals("COMPASSROOM"))
			ret[5] = "#.........C.........#";
		if (room.getRoomType().equals("BOWROOM")){
			ret[4] = "#.......=====.......#";
			ret[5] = "#.......==)==.......#";
			ret[6] = "#.......=====.......#";
		}
		
		if (room.getRoomType().equals("SWORDROOM")){
			ret[4] = "#.......=====.......#";
			ret[5] = "#.......==)==.......#";
			ret[6] = "#.......=====.......#";
		}
		if (room.getRoomType().equals("BOOMERANGROOM")){
			ret[4] = "#.......=====.......#";
			ret[5] = "#.......==(==.......#";
			ret[6] = "#.......=====.......#";
		}
		
		if (room.getRoomType().equals("HOOKSHOTROOM")){
			ret[4] = "#.......=====.......#";
			ret[5] = "#.......==)==.......#";
			ret[6] = "#.......=====.......#";
		}
		
		if (room.getRoomType().equals("FLIPPERSROOM")){
			ret[4] = "#.......=====.......#";
			ret[5] = "#.......==)==.......#";
			ret[6] = "#.......=====.......#";
		}
		
		if (room.getRoomType().equals("%BIGKEY")){
			ret[4] = "#.......=====.......#";
			ret[5] = "#.......==K==.......#";
			ret[6] = "#.......=====.......#";
		}
		if (room.getDoor(RoomMetaData.WESTROOM) != null){
			if (room.getDoor(RoomMetaData.WESTROOM).equals("?")){
				for (int i = 2; i < 9; i++){
					ret[i] = "."+ret[i].substring(1);
				}
			} else {
				ret[5] = room.getDoor(RoomMetaData.WESTROOM)+ret[5].substring(1);
			}
		}
		if (room.getDoor(RoomMetaData.EASTROOM) != null){
			if (room.getDoor(RoomMetaData.EASTROOM).equals("?")){
				for (int i = 2; i < 9; i++){
					ret[i] = ret[i].substring(0, ret[i].length()-1)+".";
				}
			} else {
				ret[5] = ret[5].substring(0, ret[5].length()-1)+room.getDoor(RoomMetaData.EASTROOM);
			}
		}
		if (room.getDoor(RoomMetaData.NORTHROOM) != null){
			if (room.getDoor(RoomMetaData.NORTHROOM).equals("?")){
				ret[0] =  "##.................##";
			} else {
				ret[0] =  "##########"+room.getDoor(RoomMetaData.NORTHROOM)+"##########";
			}
		}
		if (room.getDoor(RoomMetaData.SOUTHROOM) != null){
			if (room.getDoor(RoomMetaData.SOUTHROOM).equals("?")){
				ret[10] =  "##.................##";
			} else {
				ret[10] = "##########"+room.getDoor(RoomMetaData.SOUTHROOM)+"##########";
			}
		}
		return ret;
	}
	
	private ArrayList obstacles;
	private ArrayList prizes;
	private ArrayList fills;
	
	private void setup(){
		//System.out.println("dungeonData "+dungeonDataX);
		obstacles = dungeonDataX.getFirstObstacles();
		prizes = dungeonDataX.getFirstPrizes();
		fills = dungeonDataX.getFills();
	}
	
	public ArrayList getRooms(){
		setup();
		ArrayList dungeonRooms = new ArrayList();
		RoomMetaData firstRoom = new RoomMetaData("START","STARTROOM");
		firstRoom.setNextRoom(RoomMetaData.SOUTHROOM, "_EXIT", "#");
		dungeonRooms.add(firstRoom);
		registerOcupied(firstRoom);
		registerOcupied(firstRoom.getXpos(), firstRoom.getYpos()+1, firstRoom.getDepth());
		int fillCount = 1;
		int prizeCount = 1;
		int deepCount = 1;
		RoomMetaData nextRoom = firstRoom;
		
		while (obstacles.size() > 0 || fills.size() > 0){
			if (Util.chance(80) && fills.size() > 0){
				int availableDirection = nextRoom.getAvailableDirection();
				if (availableDirection == -1 || !checkOcupied(nextRoom, availableDirection)) {//Este cuarto no tiene salidas disponibles, o están bloqueadas 
//					Vamonos pa abajo
					int nextDepth = checkNextDepth(nextRoom);
					RoomMetaData deeperRoom = new RoomMetaData("DEEP"+deepCount, nextRoom.getRoomType());
					deeperRoom.setPosition(nextRoom.getXpos(), nextRoom.getYpos(), nextDepth);
					dungeonRooms.add(deeperRoom);
					registerOcupied(deeperRoom);
					deepCount++;
					nextRoom.joinWithStairsTo(deeperRoom);
					nextRoom = deeperRoom;
					
				} 
				String fill = (String)Util.randomElementOf(fills);
				//System.out.println("Adding "+fill);
				RoomMetaData newRoom = new RoomMetaData("Fill"+fillCount, fill); 
				nextRoom.joinWith(newRoom, availableDirection);
				dungeonRooms.add(newRoom);
				registerOcupied(newRoom);
				fills.remove(fill);
				fillCount++;
				nextRoom = newRoom;
			} else if (obstacles.size()>0){
				//int obstaclePrize = Util.rand(0, prizes.size()-1);
				int obstaclePrize = 0;
				String obstacle = (String)obstacles.get(obstaclePrize);
				String prize = (String)prizes.get(obstaclePrize);
				while (true){
					RoomMetaData randRoom = (RoomMetaData) Util.randomElementOf(dungeonRooms);
					int availableDirection = randRoom.getAvailableDirection();
					if (availableDirection == -1 || !checkOcupied(randRoom, availableDirection))
						continue;
					//System.out.println("Adding "+prize);
					RoomMetaData newRoom = new RoomMetaData("Prize"+prizeCount, prize); 
					randRoom.joinWith(newRoom, availableDirection);
					dungeonRooms.add(newRoom);
					registerOcupied(newRoom);
					break;
				}
				while (true){
					String doorType = "/";
					if (Util.chance(70))
						doorType = "?";
					if (obstacle.startsWith("D_LOCKEDROOM")){ //Door obstacle
						doorType = "$";
					} else if (obstacle.startsWith("&BIGLOCK")){ 
						doorType = "%";
					} else if (obstacle.startsWith("PRIZEROOM")){
						doorType = "&";
					}
					
					//nextRoom = (RoomMetaData) Util.randomElementOf(dungeonRooms);
					int availableDirection = nextRoom.getAvailableDirection();
					if (availableDirection == -1 || !checkOcupied(nextRoom, availableDirection)) {//Este cuarto no tiene salidas disponibles, o están bloqueadas 
						//Vamonos pa abajo
						int nextDepth = checkNextDepth(nextRoom);
						//RoomMetaData deeperRoom = new RoomMetaData("DEEP"+deepCount, nextRoom.getRoomType());
						RoomMetaData deeperRoom = new RoomMetaData("DEEP"+deepCount, "EMPTYROOM");
						deeperRoom.setPosition(nextRoom.getXpos(), nextRoom.getYpos(), nextDepth);
						dungeonRooms.add(deeperRoom);
						registerOcupied(deeperRoom);
						deepCount++;
						nextRoom.joinWithStairsTo(deeperRoom);
						nextRoom = deeperRoom;
					} 
					//System.out.println("Adding "+obstacle);
					RoomMetaData newRoom = new RoomMetaData("Obstacle"+prizeCount, obstacle);
					nextRoom.joinWith(newRoom, availableDirection, doorType);
					dungeonRooms.add(newRoom);
					registerOcupied(newRoom);
					nextRoom = newRoom;
					break;
				}
				prizeCount++;
				prizes.remove(obstaclePrize);
				obstacles.remove(obstaclePrize);
			}
		}
		obstacles.add("&BIGLOCK");
		obstacles.add("PRIZEROOM");
		prizes.add("%BIGKEY");
		prizes.add("EMPTYROOM");
		
		while (obstacles.size() > 0 || fills.size() > 0){
			int obstaclePrize = 0;
			String obstacle = (String)obstacles.get(obstaclePrize);
			String prize = (String)prizes.get(obstaclePrize);
			while (true){
				RoomMetaData randRoom = (RoomMetaData) Util.randomElementOf(dungeonRooms);
				int availableDirection = randRoom.getAvailableDirection();
				if (availableDirection == -1 || !checkOcupied(randRoom, availableDirection))
					continue;
				//System.out.println("Adding "+prize);
				RoomMetaData newRoom = new RoomMetaData("Prize"+prizeCount, prize); 
				randRoom.joinWith(newRoom, availableDirection);
				dungeonRooms.add(newRoom);
				registerOcupied(newRoom);
				break;
			}
			while (true){
				String doorType = "/";
				if (obstacle.startsWith("D_LOCKEDROOM")){ //Door obstacle
					doorType = "$";
				} else if (obstacle.startsWith("&BIGLOCK")){ 
					doorType = "%";
				} else if (obstacle.startsWith("PRIZEROOM")){
					doorType = "&";
				}
				//nextRoom = (RoomMetaData) Util.randomElementOf(dungeonRooms);
				int availableDirection = nextRoom.getAvailableDirection();
				if (availableDirection == -1 || !checkOcupied(nextRoom, availableDirection)){
					//Vamonos pa abajo
					int nextDepth = checkNextDepth(nextRoom);
					//RoomMetaData deeperRoom = new RoomMetaData("DEEP"+deepCount, nextRoom.getRoomType());
					RoomMetaData deeperRoom = new RoomMetaData("DEEP"+deepCount, "EMPTYROOM");
					deeperRoom.setPosition(nextRoom.getXpos(), nextRoom.getYpos(), nextDepth);
					dungeonRooms.add(deeperRoom);
					registerOcupied(deeperRoom);
					deepCount++;
					nextRoom.joinWithStairsTo(deeperRoom);
					nextRoom = deeperRoom;
				} 
				//System.out.println("Adding "+obstacle);
				RoomMetaData newRoom = new RoomMetaData("Obstacle"+prizeCount, obstacle);
				nextRoom.joinWith(newRoom, availableDirection, doorType);
				dungeonRooms.add(newRoom);
				registerOcupied(newRoom);
				nextRoom = newRoom;
				break;
			}
			prizeCount++;
			prizes.remove(obstaclePrize);
			obstacles.remove(obstaclePrize);
		}
		
		return dungeonRooms;
	}
	
	
	
	private Hashtable hashPositions = new Hashtable();
	private void registerOcupied(RoomMetaData newRoom){
		hashPositions.put("("+newRoom.getXpos()+","+newRoom.getYpos()+","+newRoom.getDepth()+")", newRoom.getRoomID());
	}
	
	private void registerOcupied(int x, int y, int z){
		hashPositions.put("("+x+","+y+","+z+")", "Dummie");
	}
	
	private boolean checkOcupied(RoomMetaData nextRoom, int direction){
		Position var = RoomMetaData.getVar(direction);
		return hashPositions.get("("+(nextRoom.getXpos()+var.x)+","+(nextRoom.getYpos()+var.y)+","+nextRoom.getDepth()+")") == null;
	}
	
	private int checkNextDepth(RoomMetaData highRoom){
		int startDepth = highRoom.getDepth();
		while (true){
			if (hashPositions.get("("+highRoom.getXpos()+","+highRoom.getYpos()+","+startDepth+")") != null){
				startDepth++;
			} else {
				return startDepth;
			}
		}
	}
	
	public static void main (String [] args){
		ArrayList metadata = new DungeonGenerator().getRooms();
		for (int i = 0; i < metadata.size(); i++){
			RoomMetaData md = (RoomMetaData) metadata.get(i);
			System.out.println("Room "+i+":"+md.getRoomID()+" ("+md.getRoomType()+") ["+md.getXpos()+","+md.getYpos()+"]");
			System.out.println("       "+md.getNextRoom(RoomMetaData.NORTHROOM));
			System.out.println(md.getNextRoom(RoomMetaData.WESTROOM)+"       "+md.getNextRoom(RoomMetaData.EASTROOM));
			System.out.println("       "+md.getNextRoom(RoomMetaData.SOUTHROOM));
			System.out.println();
		}
		RoomMetaData md0 = (RoomMetaData) metadata.get(0);
		int minx = md0.getXpos(), maxx = md0.getXpos();
		int miny = md0.getYpos(), maxy = md0.getYpos();
		int mindepth = md0.getDepth(), maxdepth = md0.getDepth();
		for (int i = 0; i < metadata.size(); i++){
			RoomMetaData md = (RoomMetaData) metadata.get(i);
			if (md.getXpos() > maxx)
				maxx = md.getXpos();
			if (md.getYpos() > maxy)
				maxy = md.getYpos();
			if (md.getXpos() < minx)
				minx = md.getXpos();
			if (md.getYpos() < miny)
				miny = md.getYpos();
			if (md.getDepth() < mindepth)
				mindepth = md.getDepth();
			if (md.getDepth() > maxdepth)
				maxdepth = md.getDepth();
		}
		System.out.println("minx "+minx);
		System.out.println("miny "+miny);
		System.out.println("maxx "+maxx);
		System.out.println("maxY "+maxy);
		System.out.println("maxDepth "+maxdepth);
		System.out.println("minDepth "+mindepth);
		
		int xrange = maxx - minx + 1;
		int yrange = maxy - miny + 1;
		int zrange = maxdepth - mindepth + 1;
		
		System.out.println("xrange "+xrange);
		System.out.println("yrange "+yrange);
		System.out.println("zrange "+zrange);
		
		char[][][] map = new char[(xrange)*3][(yrange)*3][zrange];
		for (int z = 0; z < zrange; z++)
			for (int y = 0; y < (yrange)*3; y++)
				for (int x = 0; x < (xrange)*3; x++)
					map[x][y][z] = ' ';
			
		for (int i = 0; i < metadata.size(); i++){
			RoomMetaData md = (RoomMetaData) metadata.get(i);
			md.setXpos(md.getXpos()-minx);
			md.setYpos(md.getYpos()-miny);
			md.setDepth(md.getDepth()-mindepth);
			//map[md.getXpos()*3 +1][md.getYpos()*3+1] = md.getRoomID().charAt(0);
			if (md.getStairsDownRoom() != null)
				map[md.getXpos()*3 +1][md.getYpos()*3+1][md.getDepth()] = 'v';
			else if (md.getStairsUpRoom() != null)
				map[md.getXpos()*3 +1][md.getYpos()*3+1][md.getDepth()] = '^';
			else
				map[md.getXpos()*3 +1][md.getYpos()*3+1][md.getDepth()] = md.getRoomType().charAt(0);
			map[md.getXpos()*3][md.getYpos()*3+1][md.getDepth()] = md.getDoor(RoomMetaData.WESTROOM) != null ? md.getDoor(RoomMetaData.WESTROOM).charAt(0) : ' ';
			map[md.getXpos()*3 +2][md.getYpos()*3+1][md.getDepth()] = md.getDoor(RoomMetaData.EASTROOM) != null ? md.getDoor(RoomMetaData.EASTROOM).charAt(0) : ' ';
			map[md.getXpos()*3 +1][md.getYpos()*3][md.getDepth()] = md.getDoor(RoomMetaData.NORTHROOM) != null ? md.getDoor(RoomMetaData.NORTHROOM).charAt(0) : ' ';
			map[md.getXpos()*3 +1][md.getYpos()*3+2][md.getDepth()] = md.getDoor(RoomMetaData.SOUTHROOM) != null ? md.getDoor(RoomMetaData.SOUTHROOM).charAt(0) : ' ';
		}
		for (int z = 0; z < zrange; z++){
			System.out.println("Level "+z);
			for (int y = 0; y < (yrange)*3; y++){
				for (int x = 0; x < (xrange)*3; x++)
					System.out.print(map[x][y][z]);
				System.out.println();
			}
		}
		
		
	}
}




class DungeonPanelMatrix extends JPanel{
	private Cell[][][] m;
	private Level l;
	DungeonPanelMatrix(Level l){
		this.m = l.getCells();
		this.l = l;
	}
	
	public void paintComponent(Graphics g){
		Position runner = new Position(0,0,0);
		for (runner.z = 0; runner.z < m.length; runner.z++)
			for (runner.x = 0; runner.x < m[0].length; runner.x++)
				for (runner.y = 0; runner.y < m[0][0].length; runner.y++){
					Color c = pickColor(runner);
					g.setColor(c);
					g.fillRect(runner.x*3,runner.y*3 + runner.z*(l.getHeight()*3+10),3,3);
				}
	}
	

	private Color pickColor(Position w){
		if (!l.isValidCoordinate(w))
			return Color.BLUE;
		else if (l.getMapCell(w) == null)
			return Color.GREEN;
		else if (l.getFeatureAt(w) != null){
			Feature f = l.getFeatureAt(w);
			if (f.getID().equals("LOCKED_DOOR"))
				return Color.BLUE;
			else if (f.getID().equals("SMALL_KEY"))
				return Color.YELLOW;
			else if (f.getID().equals("BIG_KEY"))
				return Color.RED;
			else if (f.getID().equals("BOSS_DOOR"))
				return Color.CYAN;
			else
				return Color.GREEN;
		}
		else if (l.isSolid(w))
			return Color.BLACK;
		else if (l.isDoor(w))
			return Color.RED;
		else if (l.getMapCell(w).getHeightMod() != 0)
			return Color.MAGENTA;
		else
			return Color.WHITE;
		
	}
	
}