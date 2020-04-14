package sz.mp3;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.FloatControl;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.SourceDataLine;
import javax.sound.sampled.UnsupportedAudioFileException;

import javazoom.jl.decoder.JavaLayerException;
import javazoom.jl.player.Player;

import crl.game.Game;
import crl.game.STMusicManagerNew;

public class JLayerMP3Player implements Runnable{
	private static String currentMP3File = "__noneYet";
	private static int currentInstruction;
	
	public static final int INS_LOAD = 1;
	public static final int INS_STOP = 0;
	public static final int INS_DIE = 2;
	
	public static void setMP3(String pMP3File){
		currentMP3File = pMP3File;
	}
	
	public static void setInstruction(int instruction){
		currentInstruction = instruction;
	}
	
	public synchronized void run(){
		boolean leave = false;
		out: while (true){
			if (currentInstruction == INS_DIE){
				break out;
			}
			if (currentInstruction == INS_STOP){
				currentMP3File= "__noneYet";
			}
			if (currentMP3File.equals("__noneYet")){
	    		try {
	    			this.wait();
	    		} catch (InterruptedException ie) {
	    			continue;
	    		}
	    	}
			File soundFile = new File(currentMP3File);
        	if(!soundFile.exists() || soundFile.isDirectory() || !soundFile.canRead()) {
        		Game.addReport("Invalid MP3 file: "+currentMP3File);
        		 try {
        			this.wait();
        		} catch (InterruptedException ie) {
        			continue;
        		}
        	}
        	leave = false;
        	Player player = null;
        	while (!leave) {
				try {
					player = new Player(new FileInputStream(soundFile));
					in: while (true) {
						try {
	                        Thread.sleep(1000); // Check every second
	                    } catch(InterruptedException ignore) {
	                    	leave = true;
	                    	break;
	                    }
	                    player.play(100);
	                    if (player.isComplete()){
							player.close();
							break in;
						}
					}
				} catch (IOException ioe) {
					Game.addReport("I/O Error for "+currentMP3File);
					leave = true;
				} catch (JavaLayerException uafe) {
					Game.addReport("UnsupportedAudioFileException for "+currentMP3File);
					STMusicManagerNew.thus.setEnabled(false);
					leave = true;
				}
        	}
        	player.close();
		}
	}
}
