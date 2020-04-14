package crl.level;

import sz.util.*;
import zrl.*;
import crl.action.*;

public class SpawnMonster extends Action{
	private static SpawnMonster singleton = new SpawnMonster();
	public String getID(){
		return "SpawnMonster";
	}

	public void execute(){
		Level level = performer.getLevel();
		Respawner perf = (Respawner) performer;
		if (Util.chance(perf.getProb()))
			level.respawn();
	}

	public static SpawnMonster getAction(){
		return singleton;
	}
}