package crl.level;

import java.io.Serializable;

import sz.util.Position;
import sz.util.Util;

public class SpawnPoint implements Serializable{
	private String[] monsterIDs;
	private Position position;
	public String[] getMonsterIDs() {
		return monsterIDs;
	}
	public void setMonsterIDs(String[] monsterIDs) {
		this.monsterIDs = monsterIDs;
	}
	public Position getPosition() {
		return position;
	}
	public void setPosition(Position position) {
		this.position = position;
	}
	
	public String getAMonsterID(){
		return Util.randomElementOf(monsterIDs);
	}
	
	public SpawnPoint(String[] monsterIDs, Position position){
		setMonsterIDs(monsterIDs);
		setPosition(position);
	}
}
