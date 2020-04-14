package crl.levelgen.patterns;


public class Space extends StaticPattern{
	public String getMapKey(){
		return "CRASHING";
	}
	
	public String getDescription() {
		return "Planet X";
	}

	public String getMusicKeyMorning() {
		return "AVATAR";
	}

	public String getMusicKeyNoon() {
		return null;
	}

	public Space () {
		this.cellMap = new String [][]{{
			"ttttttttttttttttttttttttttttttttttttttttttttttttttttttttt",
			"t......................wwwwwww..........................t",
			"t..t...................w..E..w...t......................t",
			"t......................w.....w..................N.......t",
			"t........mmmm..........w.....w..........................t",
			"t.......mmxxmm.t.......w.....w..........................t",
			"t.......mxxxxm.........www.www..........................t",
			"t.......mmxxmm..........................................t",
			"t........mxxm........................................t..t",
			"t......t................................................t",
			"t................t.................t....................t",
			"t.......................................................t",
			"t.......................................................t",
			"t.......................................................t",
			"t.......................................................t",
			"t...................................................t...t",
			"t........t................S.............................t",
			"ttttttttttttttttttttttttttttttttttttttttttttttttttttttttt",

			
		}};
		
		charMap.put(".", "GRASS_FLOOR");
		charMap.put("t", "FOREST_TREE");
		charMap.put("m", "TIME_MACHINE_WALL");
		charMap.put("x", "TIME_MACHINE_FLOOR");
		charMap.put("w", "WATER");
		charMap.put("E", "GRASS_FLOOR ITEM ENILNO");
		charMap.put("S", "GRASS_FLOOR EXIT _BACK");
		charMap.put("N", "DDOOR EXIT _NEXT");
	}
	
	/*public String[] getDwellers(){
		return new String[] {"VIPER"};
	}*/


}