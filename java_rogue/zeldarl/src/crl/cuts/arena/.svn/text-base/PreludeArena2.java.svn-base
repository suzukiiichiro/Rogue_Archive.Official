package crl.cuts.arena;

import crl.cuts.Unleasher;
import crl.game.Game;
import crl.level.Level;
import crl.levelgen.MonsterSpawnInfo;

public class PreludeArena2 extends Unleasher {
	int counter;
	int superCounter;
	
	private MonsterSpawnInfo[] doInhabitants(String[] ids){
		MonsterSpawnInfo[] ret = new MonsterSpawnInfo[ids.length];
		for (int i = 0; i < ids.length; i++){
			ret[i] = new MonsterSpawnInfo(ids[i], MonsterSpawnInfo.UNDERGROUND, 1);
		}
		return ret;
	}
	
	public void unleash(Level level, Game game){
		counter++;
		if (counter > 100){
			counter = 0;
			superCounter++;
			switch (superCounter){
			case 1:
				level.setInhabitants(new MonsterSpawnInfo[]{
					new MonsterSpawnInfo("SKELETON_PANTHER", MonsterSpawnInfo.UNDERGROUND, 1),
					new MonsterSpawnInfo("BLACK_KNIGHT", MonsterSpawnInfo.UNDERGROUND, 1),
					new MonsterSpawnInfo("SPEAR_KNIGHT", MonsterSpawnInfo.UNDERGROUND, 1),
					new MonsterSpawnInfo("SKULL_HEAD", MonsterSpawnInfo.UNDERGROUND, 1),
					new MonsterSpawnInfo("WEREBEAR", MonsterSpawnInfo.UNDERGROUND, 1)
				});
				break;
			case 2:
				level.setInhabitants(new MonsterSpawnInfo[]{
					new MonsterSpawnInfo("BONE_ARCHER", MonsterSpawnInfo.UNDERGROUND, 1),
					new MonsterSpawnInfo("SKELETON_PANTHER", MonsterSpawnInfo.UNDERGROUND, 1),
					new MonsterSpawnInfo("AXE_KNIGHT", MonsterSpawnInfo.UNDERGROUND, 1)
				});
				break;
			case 3:
				level.setInhabitants(doInhabitants(new String[]{"DURGA","BLADE_SOLDIER","BONE_HALBERD","CROW"}));
				break;
			case 4:
				level.setInhabitants(doInhabitants(new String[]{"COCKATRICE","COOPER_ARMOR","SALOME"}));
				break;
			case 5:
				level.setInhabitants(doInhabitants(new String[]{"LILITH","BONE_MUSKET","VAMPIRE_BAT"}));
				break;
			case 6:
				level.setInhabitants(doInhabitants(new String[]{"ZELDO","CAGNAZOO","KNIFE_MERMAN"}));
				break;
			case 7:
				level.setInhabitants(doInhabitants(new String[]{"GIANTBAT"}));
				break;
			}
		}
	}
}