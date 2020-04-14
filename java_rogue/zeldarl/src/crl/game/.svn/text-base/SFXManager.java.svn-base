package crl.game;

import sz.wav.WavPlayer;

public class SFXManager {
	private static Thread currentThread;
	private static boolean enabled;
	
	public static void setEnabled(boolean value){
		enabled = value;
	}
	
	public static void play(String fileName){
		if (!enabled)
			return;
		if (currentThread != null)
			currentThread.interrupt();
		if (fileName.endsWith(".wav")){
			WavPlayer wavPlayer = new WavPlayer(fileName);
			currentThread = new Thread(wavPlayer);
		} 
		currentThread.start();
	}
}
