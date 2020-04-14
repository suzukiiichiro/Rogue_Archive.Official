package crl.game;


import java.util.*;

import crl.ui.*;
//import crl.action.vkiller.Whip;
import crl.level.*;
import crl.levelgen.*;
import crl.actor.*;

import crl.item.*;
import sz.fov.FOV;
import sz.util.*;
import zrl.Main;
import zrl.player.*;
//import zrl.player.advancements.Advancement;


public class Game implements CommandListener, PlayerEventListener, java.io.Serializable{
	//Configuration
	//private transient ConsoleSystemInterface si;
	private transient UserInterface ui;
	
	private Dispatcher dispatcher;
	private Player player;
	private Level currentLevel;
	private boolean canSave;
	
	public void setCanSave(boolean vl){
		canSave = vl;
	}
	
	public boolean canSave(){
		return canSave;
	}
	
	private Hashtable /*Level*/  storedLevels = new Hashtable();
	private boolean endGame;
	private long turns;
	private boolean isDay = true;
	private long timeSwitch;
	private String[] levelPath;

	public void commandSelected (int commandCode){
		if (commandCode == CommandListener.QUIT){
			finishGame();
		} else if (commandCode == CommandListener.SAVE) {
			if (canSave()){
				GameFiles.saveGame(this, player);
				exitGame();
			}
		}
	}

	private void run(){
		//si.cls();
		player.setFOV(new FOV());
		player.getLevel().addMessage("Greetings "+player.getName()+", welcome to the game... Press '?' for Help");
		ui.refresh();
		while (!endGame){
			Actor actor = dispatcher.getNextActor();
            if (actor == player){
            	player.darken();
            	player.see();
            	ui.refresh();
				player.getGameSessionInfo().increaseTurns();
				player.checkDeath();
				player.getLevel().checkUnleashers(this);
				
			}
            if (endGame)
            	break;
			actor.act();
			if (endGame)
            	break;
			actor.getLevel().getDispatcher().returnActor(actor);
			
			if (actor == player){
				if (currentLevel != null)
					currentLevel.updateLevelStatus();
				//ui.refresh();
				turns++;
				//player.addScore(1);
				
			}
		}
	}
	 	
	public void resume(){
		Game.setCurrentGame(this);
		player.setSelector(ui.getSelector());
		ui.setPlayer(player);
		ui.addCommandListener(this);
		ui.setGameOver(false);
		player.getLevel().addActor(player);
		player.setPlayerEventListener(this);
		endGame = false;
		turns = player.getGameSessionInfo().getTurns();
		if (currentLevel.hasNoonMusic() && !currentLevel.isDay()){
			STMusicManagerNew.thus.playKey(currentLevel.getMusicKeyNoon());
		} else {
			STMusicManagerNew.thus.playKey(currentLevel.getMusicKeyMorning());
		}
		run();
	}
	
	public void setPlayer(Player p){
		player = p;
		player.setLevel(currentLevel);
		player.setFOV(new FOV());
		currentLevel.setPlayer(player);
		if (player.getGameSessionInfo() == null)
			player.setGameSessionInfo(new GameSessionInfo());
		player.setSelector(ui.getSelector());
		ui.setPlayer(player);
		player.setPlayerEventListener(this);
		player.setGame(this);
	}
	
	public void newGame(){
		Game.setCurrentGame(this);
		player = PlayerGenerator.thus.generatePlayer();
		player.setGame(this);
		player.setGameSessionInfo(new GameSessionInfo());
		player.setSelector(ui.getSelector());
		ui.setPlayer(player);
		ui.addCommandListener(this);
		ui.setGameOver(false);
		player.setPlayerEventListener(this);
		//generateLevelPath();
		
		Display.thus.showIntro2(player);
		
		//loadLevel("LINKS", 0);
		loadLevel("LINKS", 0);
		turns = 0;
		timeSwitch = 600;
		run();
	}
	
	public void training(){
		player = PlayerGenerator.thus.createSpecialPlayer("SOLIEYU_KID");
		player.setGame(this);
		player.setGameSessionInfo(new GameSessionInfo());
		player.setSelector(ui.getSelector());
		player.setDoNotRecordScore(true);
		ui.setPlayer(player);
		ui.addCommandListener(this);
		ui.setGameOver(false);
		player.setPlayerEventListener(this);
		//generatePrologueLevelPath();
		//Display.thus.showIntro(player);
		loadLevel("TRAINING");
		turns = 0;
		timeSwitch = 900;
		run();
	}
	
	public void prologue(){
		Game.setCurrentGame(this);
		player = PlayerGenerator.thus.generatePlayer();
		player.setGame(this);
		player.setGameSessionInfo(new GameSessionInfo());
		player.setSelector(ui.getSelector());
		ui.setPlayer(player);
		ui.addCommandListener(this);
		ui.setGameOver(false);
		player.setPlayerEventListener(this);
		//generateLevelPath();
		
		Display.thus.showIntro(player);
		
		//loadLevel("LINKS", 0);
		loadLevel("ADAMANTCAVE", 0);
		turns = 0;
		timeSwitch = 600;
		run();
		
	}
	
	public void arena(){
		player = PlayerGenerator.thus.createSpecialPlayer("SONIA");
		player.setGame(this);
		player.setGameSessionInfo(new GameSessionInfo());
		player.setSelector(ui.getSelector());
		player.setDoNotRecordScore(false);
		ui.setPlayer(player);
		ui.addCommandListener(this);
		ui.setGameOver(false);
		player.setPlayerEventListener(this);
		//generatePrologueLevelPath();
		//Display.thus.showIntro(player);
		loadLevel("PRELUDE_ARENA");
		turns = 0;
		timeSwitch = 50;
		run();
	}
	
		private void resumeScreen(){
		STMusicManagerNew.thus.playKey("GAME_OVER");
		//UserInterface.getUI().showMessageHistory();
		if (Display.thus.showResumeScreen(player)){
			GameFiles.saveMemorialFile(player);
		}
    }
	
    public void informEvent(int code){
    	informEvent(code, null);
    }

    private String [] DEATHMESSAGES = new String[]{
		"G A M E   O V E R",
		"Game Over",
		"Better luck next time, Hero of time"
    };
	public void informEvent(int code, Object param){
		switch (code){
			case Player.DEATH:
				ui.refresh();
				ui.showSystemMessage(Util.randomElementOf(DEATHMESSAGES) +  " [Press Space to continue]");
				finishGame();
				break;
			case Player.DROWNED:
				ui.refresh();
				ui.showSystemMessage("You fell into the water and drowned!  [Press Space to continue]");
				finishGame();
				break;
			case Player.EVT_SMASHED:
				ui.refresh();
				ui.showSystemMessage("Your body collapses!  [Press Space to continue]");
				finishGame();
				break;
			/*case Player.EVT_NEXT_LEVEL:
				loadNextLevel();
				break;
			case Player.EVT_BACK_LEVEL:
				loadBackLevel();
				break;*/
			case Player.EVT_GOTO_LEVEL:
				loadLevel((String)param);
				break;
			
			
		}
	}

	/*private void endGame(){
		Display.thus.showEndgame(player);
	}*/

	private void finishGame(){
		if (!player.isDoNotRecordScore()){
			GameFiles.saveHiScore(player);
			
			resumeScreen();
			//Display.thus.showHiscores(GameFiles.loadScores());
		}
		GameFiles.permadeath(player);
		exitGame();
	}
	
	public void exitGame(){
		//levelNumber = -1;
		currentLevel.disableTriggers();
		currentLevel = null;
		ui.removeCommandListener(this);
		ui.setGameOver(true);
		player.setPlayerEventListener(null);
		
		
		endGame = true;
	}

	/*private void loadNextLevel(){
		if (currentLevel.getLevelNumber() == -1)
			Game.crash("A level outside the mainstream path tried to continue it", new Exception("A level outside the mainstream path tried to continue it"));
		loadLevel(levelPath[currentLevel.getLevelNumber()+1], currentLevel.getLevelNumber()+1);
		if (currentLevel.getLevelNumber() % 2 != 0) {
			Advancement pick = Display.thus.levelUp(player);
			player.getGameSessionInfo().addHistoryItem("went for "+pick.getName());
			pick.advance(player);
		}
		
		
		if (player.getFlag("BLACK_GEM") && player.getFlag("WHITE_GEM") && player.getFlag("AMBER_GEM")){
			loadLevel("LAUNCHING", currentLevel.getLevelNumber());
			player.setFlag("BLACK_GEM", false);
			player.setFlag("WHITE_GEM", false);
			player.setFlag("AMBER_GEM", false);
		}
		
		if (player.getFlag("BLACK_GEM") && player.getFlag("WHITE_GEM")){
			loadLevel("CRASHING", currentLevel.getLevelNumber());
			player.setFlag("BLACK_GEM", false);
			player.setFlag("WHITE_GEM", false);
		}
		
		if (player.getFlag("BLACK_GEM")){
			Advancement pick = Display.thus.levelUp(player);
			player.getGameSessionInfo().addHistoryItem("went for "+pick.getName()+" using a black gem");
			pick.advance(player);
			player.setFlag("BLACK_GEM", false);
		}
		if (player.getFlag("WHITE_GEM")){
			if (currentLevel.getLevelNumber() < 17){
				player.setFlag("WHITE_GEM", false);
				player.getGameSessionInfo().addHistoryItem("went forward using a white gem");
				loadNextLevel();
			}
			player.setFlag("WHITE_GEM", false);
		}
		player.setPosition(currentLevel.getExitFor("_BACK"));
	}
	
	private void loadBackLevel(){
		if (currentLevel.getLevelNumber() == -1)
			Game.crash("A level outside the mainstream path tried to continue it", new Exception("A level outside the mainstream path tried to continue it"));
		loadLevel(levelPath[currentLevel.getLevelNumber()-1], currentLevel.getLevelNumber()-1);
		player.setPosition(currentLevel.getExitFor("_NEXT"));
	}
	*/
	private void loadLevel(String levelID) {
		loadLevel(levelID, -1);
	}
	
	public void wonGame(){
		Display.thus.showEndgame(player);
		player.getGameSessionInfo().setDeathCause(GameSessionInfo.ASCENDED);
		player.getGameSessionInfo().setDeathLevel(player.getLevel().getLevelNumber());
		finishGame();
		return;
	}
	
	private void loadLevel(String levelID, int targetLevelNumber) {
		//JOptionPane.showMessageDialog(null, "loadLevel "+levelID+" with "+targetLevelNumber);
		Debug.say("Load new level");
		String formerLevelID = null; 
		if (currentLevel != null){
			formerLevelID = currentLevel.getID();
			Level storedLevel = (Level) storedLevels.get(formerLevelID);
			if (storedLevel == null){
				storedLevels.put(formerLevelID, currentLevel);
			}
		} else {
			formerLevelID = "_BACK";
		}
		Level storedLevel = (Level)storedLevels.get(levelID);
		if (storedLevel != null) {
			currentLevel = storedLevel;
			player.setPosition(currentLevel.getExitFor(formerLevelID));
			currentLevel.setIsDay(isDay);
			if (currentLevel.isCandled()){
				currentLevel.destroyCandles();
				LevelMaster.lightCandles(currentLevel);
			}
		} else {
			try {
				currentLevel = LevelMaster.createLevel(levelID, formerLevelID, targetLevelNumber);
				currentLevel.setPlayer(player);
				ui.setPlayer(player);
				currentLevel.setIsDay(isDay);
				if (currentLevel.getPlayer() != null)
					currentLevel.getPlayer().addHistoricEvent("got to the "+currentLevel.getDescription());
			} catch (CRLException crle){
				crash("Error while creating level "+levelID, crle);
			}
		}
		//currentLevel.setLevelNumber(targetLevelNumber);
		player.setLevel(currentLevel);
		if(currentLevel.getExitFor(formerLevelID) != null){
			player.setPosition(currentLevel.getExitFor(formerLevelID));
		} else if(currentLevel.getExitFor("_START") != null) {
			player.setPosition(currentLevel.getExitFor("_START"));
		}
		
		if (currentLevel.isHostageSafe()){
			
		}
		dispatcher = currentLevel.getDispatcher();
		if (currentLevel.hasNoonMusic() && !currentLevel.isDay()){
			STMusicManagerNew.thus.playKey(currentLevel.getMusicKeyNoon());
		} else {
			STMusicManagerNew.thus.playKey(currentLevel.getMusicKeyMorning());
		}
		currentLevel.anihilateMonsters();
		currentLevel.populate();
		ui.levelChange();
	}
	
	public void setLevel(Level level){
		currentLevel = level;
		player.setLevel(level);
		dispatcher = currentLevel.getDispatcher();
		if (currentLevel.hasNoonMusic() && !currentLevel.isDay()){
			STMusicManagerNew.thus.playKey(currentLevel.getMusicKeyNoon());
		} else {
			STMusicManagerNew.thus.playKey(currentLevel.getMusicKeyMorning());
		}
		
		//STMusicManager.thus.playForLevel(levelNumber, levelPath[levelNumber], currentLevel.isDay());
		ui.levelChange();
		
		
	}
	public Player getPlayer(){
		return player;
	}
	
	public static String getVersion(){
		return "0.72a";
	}
	
	public void setInterfaces(UserInterface pui){
		ui = pui;
	}
	
	
	public static void crash(String message, Throwable exception){
		Main.crash(message, exception);
    }
	
	private static Vector reports = new Vector(20);
	public static void addReport(String report){
		reports.add(report);
	}
	
	public static Vector getReports(){
		return reports;
	}
	
	//private static Vector uniqueRegister = new Vector();
	
	private Vector uniqueRegister = new Vector();
	
	/*public void syncUniqueRegister(){
		uniqueRegister = uniqueRegisterObjectCopy;
	}*/
	
	/*public void freezeUniqueRegister(){
		uniqueRegisterObjectCopy = uniqueRegister;
	}*/
	
	public boolean wasUniqueGenerated(String itemID){
		return uniqueRegister.contains(itemID);
	}
	
	public void resetUniqueRegister(){
		uniqueRegister = new Vector();
	}
	
	public void registerUniqueGenerated (String itemID){
		uniqueRegister.add(itemID);
	}
	
	private static Game currentGame;
	
	public static Game getCurrentGame(){
		return currentGame;
	}
	
	public static void setCurrentGame(Game g){
		currentGame = g;
	}
	
	private int lastGeneratedItemIndex;
	
	public int getLastGeneratedItemIndex(){
		lastGeneratedItemIndex++;
		return lastGeneratedItemIndex;
	}
}