package crl.levelgen;

import java.util.ArrayList;

import sz.util.Position;
import sz.util.Util;

public class RoomMetaData {
	private String roomType;
	private String roomID;
	private int xpos, ypos;
	private int depth;
	private String[] nextRooms = new String[4];
	private String[] nextDoors = new String[4];
	private String stairsUpRoom;
	private String stairsDownRoom;

	public final static int NORTHROOM = 0, WESTROOM = 1, EASTROOM = 2, SOUTHROOM = 3;
	
	public String getStairsDownRoom() {
		return stairsDownRoom;
	}

	public void setStairsDownRoom(String stairsDownRoom) {
		this.stairsDownRoom = stairsDownRoom;
	}

	public String getStairsUpRoom() {
		return stairsUpRoom;
	}

	public void setStairsUpRoom(String stairsUpRoom) {
		this.stairsUpRoom = stairsUpRoom;
	}

	public int getDepth() {
		return depth;
	}

	public void setDepth(int depth) {
		this.depth = depth;
	}

	public int getXpos() {
		return xpos;
	}

	public void setXpos(int xpos) {
		this.xpos = xpos;
	}

	public int getYpos() {
		return ypos;
	}

	public void setYpos(int ypos) {
		this.ypos = ypos;
	}

	public RoomMetaData(String roomID, String roomType){
		this.roomID = roomID;
		this.roomType = roomType;
	}
	
	public String getRoomType() {
		return roomType;
	}

	public void setRoomType(String roomType) {
		this.roomType = roomType;
	}

	public String getRoomID() {
		return roomID;
	}

	private int oposite(int direction){
		switch (direction){
		case NORTHROOM:
			return SOUTHROOM;
		case SOUTHROOM:
			return NORTHROOM;
		case EASTROOM:
			return WESTROOM;
		case WESTROOM:
			return EASTROOM;
		}
		return -1;
	}
	
	private final static Position
		NORTHVAR = new Position(0,-1),
		SOUTHVAR = new Position(0,1),
		WESTVAR = new Position(-1,0),
		EASTVAR = new Position(1,0);
	
	public static  Position getVar(int direction){
		switch (direction){
		case NORTHROOM:
			return NORTHVAR;
		case SOUTHROOM:
			return SOUTHVAR;
		case EASTROOM:
			return EASTVAR;
		case WESTROOM:
			return WESTVAR;
		}
		return null;
	}
	
	public void joinWith(RoomMetaData what, int direction, String doorType){
		setNextRoom(direction, what.getRoomID(), doorType);
		if (doorType.equals("?"))
			what.setNextRoom(oposite(direction), getRoomID(), "?");
		else
			what.setNextRoom(oposite(direction), getRoomID(), "/");
		Position var = getVar(direction);
		what.setPosition(xpos+var.x, ypos+var.y, depth);
	}
	
	public void joinWith(RoomMetaData what, int direction){
		if (Util.chance(20))
			joinWith(what, direction, "?");
		else
			joinWith(what, direction, "/");
	}

	public void setNextRoom(int direction, String roomID, String doorType){
		nextRooms[direction] = roomID;
		nextDoors[direction] = doorType;
	}
	
	public void setPosition(int xpos, int ypos, int depth){
		this.xpos = xpos;
		this.ypos = ypos;
		this.depth = depth;
	}
	
	private boolean [] tempAvailable = new boolean[4];
	public int getAvailableDirection(){
		tempAvailable[0] = nextRooms[0] == null;
		tempAvailable[1] = nextRooms[1] == null;
		tempAvailable[2] = nextRooms[2] == null;
		tempAvailable[3] = nextRooms[3] == null;
		if (!(tempAvailable[0] || tempAvailable[1] || tempAvailable[2] || tempAvailable[3]))
			return -1;
		int rand = Util.rand(0,3);
		while (true){
			if (nextRooms[rand] == null)
				return rand;
			else
				rand = Util.rand(0,3);
		}
	}

	public String getNextRoom(int direction){
		return nextRooms[direction];
	}
	
	public String getDoor(int direction){
		return nextDoors[direction];
	}
	
	public void joinWithStairsTo(RoomMetaData deeperRoom){
		stairsDownRoom = deeperRoom.getRoomID();
		deeperRoom.setStairsUpRoom(getRoomID());
	}
}
