package crl.levelgen.cave;

import crl.level.Cell;

public class WispSim {
	//private static int[][] level;
	private static Wisp lastWisp;
	private static Wisp wisp1, wisp2;
	
	public static void setWisps(Wisp w1, Wisp w2){
		//System.out.println("set wispys " +w1.getPosition() + " " + w2.getPosition());
		wisp1=w1;
		wisp2=w2;
		w1.setFriend(w2);
		w2.setFriend(w1);
		lastWisp = w1;
	}
	
	public static void run (Cell[][] map, Cell noSolidCell, Cell swallowWaterCell){
		//System.out.println("Running");
		wisp1.setLevel(map, noSolidCell, swallowWaterCell);
		wisp2.setLevel(map, noSolidCell, swallowWaterCell);
		//level = map;
		while (!wisp1.getPosition().equals(wisp2.getPosition())){
			if (lastWisp == wisp1){
				wisp2.go();
				lastWisp = wisp2;
			}
			else{
				//wisp2.go();
				wisp1.go();
				lastWisp = wisp1;
			}
		}
	}
}
