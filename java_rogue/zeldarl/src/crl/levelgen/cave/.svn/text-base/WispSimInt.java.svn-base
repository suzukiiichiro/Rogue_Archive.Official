package crl.levelgen.cave;

public class WispSimInt {
	//private static int[][] level;
	private static WispInt lastWisp;
	private static WispInt wisp1, wisp2;
	
	public static void setWisps(WispInt w1, WispInt w2){
		//System.out.println("set wispys " +w1.getPosition() + " " + w2.getPosition());
		wisp1=w1;
		wisp2=w2;
		w1.setFriend(w2);
		w2.setFriend(w1);
		lastWisp = w1;
	}
	
	public static void run (int[][] map){
		//System.out.println("Running");
		wisp1.setLevel(map);
		wisp2.setLevel(map);
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
