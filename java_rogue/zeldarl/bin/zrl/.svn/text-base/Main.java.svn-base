package zrl;

import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.Enumeration;
import java.util.Properties;

import javax.sound.midi.MidiSystem;
import javax.sound.midi.MidiUnavailableException;
import javax.sound.midi.Sequencer;

import sz.csi.CharKey;
import sz.csi.ConsoleSystemInterface;
import sz.csi.jcurses.JCursesConsoleInterface;
import sz.csi.wswing.WSwingConsoleInterface;
import sz.midi.STMidiPlayer;
import zrl.player.Player;

import crl.action.*;
import crl.ai.ActionSelector;
import crl.ai.SelectorFactory;
import crl.ai.monster.BasicMonsterAI;
import crl.ai.monster.RangedAI;

import crl.ai.monster.WanderToPlayerAI;
import crl.conf.console.data.CharAppearances;
import crl.conf.console.data.CharCuts;
import crl.conf.console.data.CharEffects;
/*import crl.conf.gfx.data.GFXAppearances;
import crl.conf.gfx.data.GFXCuts;
import crl.conf.gfx.data.GFXEffects;*/
import crl.data.Cells;

import crl.data.Features;
import crl.data.Items;
import crl.data.MonsterLoader;
import crl.data.SmartFeatures;

import crl.feature.CountDown;
import crl.feature.FeatureFactory;
import crl.feature.SmartFeatureFactory;
import crl.feature.ai.KegSelector;
import crl.feature.ai.NullSelector;
import crl.game.CRLException;
import crl.game.Game;
import crl.game.GameFiles;
import crl.game.PlayerGenerator;
import crl.game.SFXManager;
import crl.game.STMusicManagerNew;
import crl.item.ItemFactory;
import crl.level.MapCellFactory;
import crl.monster.MonsterFactory;
import crl.ui.Appearance;
import crl.ui.AppearanceFactory;
import crl.ui.CommandListener;
import crl.ui.Display;
import crl.ui.UserAction;
import crl.ui.UserCommand;
import crl.ui.UserInterface;
import crl.ui.consoleUI.CharDisplay;
import crl.ui.consoleUI.CharPlayerGenerator;
import crl.ui.consoleUI.ConsoleUserInterface;
import crl.ui.consoleUI.effects.CharEffectFactory;
import crl.ui.effects.EffectFactory;
/*import crl.ui.graphicsUI.GFXDisplay;
import crl.ui.graphicsUI.GFXPlayerGenerator;
import crl.ui.graphicsUI.GFXUserInterface;
import crl.ui.graphicsUI.SwingSystemInterface;
import crl.ui.graphicsUI.effects.GFXEffectFactory;*/

public class Main {
	//private static SystemInterface si;
	private final static int JCURSES_CONSOLE = 0, SWING_GFX = 1, SWING_CONSOLE = 2;
	private static String uiFile;
	
	private static UserInterface ui;
	
	private static Game currentGame;
	private static boolean createNew = true;
	private static int mode;

	private static void init(){
		System.out.println("ZeldaRL "+Game.getVersion());
		System.out.println("Slash ~ 2007");
		System.out.println("Reading configuration");
    	readConfiguration();
    	
		if (createNew){
			try {
    			
    			switch (mode){
				case SWING_GFX:
					System.out.println("Initializing Graphics Appearances");
					initializeGAppearances();
					break;
				case JCURSES_CONSOLE:
				case SWING_CONSOLE:
					System.out.println("Initializing Char Appearances");
					initializeCAppearances();
					break;
    			}
				System.out.println("Initializing Action Objects");
				initializeActions();
				initializeSelectors();
				System.out.println("Loading Data");
				initializeCells();
				initializeFeatures();
				initializeItems();
				initializeMonsters();
				switch (mode){
				case SWING_GFX:
					/*System.out.println("Initializing Swing GFX System Interface");
					SwingSystemInterface si = new SwingSystemInterface();
					System.out.println("Initializing Swing GFX User Interface");
					UserInterface.setSingleton(new GFXUserInterface());
					//GFXCuts.initializeSingleton();
					Display.thus = new GFXDisplay(si, UIconfiguration);
					PlayerGenerator.thus = new GFXPlayerGenerator(si, UIconfiguration);
					//PlayerGenerator.thus.initSpecialPlayers();
					EffectFactory.setSingleton(new GFXEffectFactory());
					((GFXEffectFactory)EffectFactory.getSingleton()).setEffects(new GFXEffects().getEffects(UIconfiguration));
					ui = UserInterface.getUI();
					initializeUI(si);*/
					break;
				case JCURSES_CONSOLE:
					System.out.println("Initializing JCurses System Interface");
					ConsoleSystemInterface csi = null;
					try{
						csi = new JCursesConsoleInterface();
					}
		            catch (ExceptionInInitializerError eiie){
		            	crash("Fatal Error Initializing JCurses", eiie);
		            	eiie.printStackTrace();
		                System.exit(-1);
		            }
		            System.out.println("Initializing Console User Interface");
					UserInterface.setSingleton(new ConsoleUserInterface());
					CharCuts.initializeSingleton();
					Display.thus = new CharDisplay(csi);
					PlayerGenerator.thus = new CharPlayerGenerator(csi);
					//PlayerGenerator.thus.initSpecialPlayers();
					EffectFactory.setSingleton(new CharEffectFactory());
					((CharEffectFactory)EffectFactory.getSingleton()).setEffects(new CharEffects().getEffects());
					ui = UserInterface.getUI();
					initializeUI(csi);
					break;
				case SWING_CONSOLE:
					System.out.println("Initializing Swing Console System Interface");
					csi = new WSwingConsoleInterface();
		            System.out.println("Initializing Console User Interface");
					UserInterface.setSingleton(new ConsoleUserInterface());
					CharCuts.initializeSingleton();
					Display.thus = new CharDisplay(csi);
					PlayerGenerator.thus = new CharPlayerGenerator(csi);
					//PlayerGenerator.thus.initSpecialPlayers();
					EffectFactory.setSingleton(new CharEffectFactory());
					((CharEffectFactory)EffectFactory.getSingleton()).setEffects(new CharEffects().getEffects());
					ui = UserInterface.getUI();
					initializeUI(csi);
				}
				
            } catch (CRLException crle){
            	crash("Error initializing", crle);
            }
            
            STMusicManagerNew.initManager();
        	if (configuration.getProperty("enableSound") != null && configuration.getProperty("enableSound").equals("true")){ // Sound
        		if (configuration.getProperty("enableMusic") == null || !configuration.getProperty("enableMusic").equals("true")){ // Music
    	    		STMusicManagerNew.thus.setEnabled(false);
    		    } else {
    		    	System.out.println("Initializing Midi Sequencer");
    	    		try {
    	    			STMidiPlayer.sequencer = MidiSystem.getSequencer ();
    	    			STMidiPlayer.sequencer.open();
    	    		} catch(MidiUnavailableException mue) {
    	            	Game.addReport("Midi device unavailable");
    	            	System.out.println("Midi Device Unavailable");
    	            	STMusicManagerNew.thus.setEnabled(false);
    	            	return;
    	            }
    	    		System.out.println("Initializing Music Manager");
    				
    		    	
    	    		Enumeration keys = configuration.keys();
    	    	    while (keys.hasMoreElements()){
    	    	    	String key = (String) keys.nextElement();
    	    	    	if (key.startsWith("mus_")){
    	    	    		String music = key.substring(4);
    	    	    		STMusicManagerNew.thus.addMusic(music, configuration.getProperty(key));
    	    	    	}
    	    	    }
    	    	    STMusicManagerNew.thus.setEnabled(true);
    		    }
    	    	if (configuration.getProperty("enableSFX") == null || !configuration.getProperty("enableSFX").equals("true")){
    		    	SFXManager.setEnabled(false);
    		    } else {
    		    	SFXManager.setEnabled(true);
    		    }
        	}
            createNew = false;
    	}
	}
	private static Properties configuration;
	private static Properties UIconfiguration;
	
	private static void readConfiguration(){
		configuration = new Properties();
	    try {
	    	configuration.load(new FileInputStream("zrl.cfg"));
	    } catch (IOException e) {
	    	System.out.println("Error loading configuration file, please confirm existence of zrl.cfg");
	    	System.exit(-1);
	    }
	    
	    if (mode == SWING_GFX){
		    UIconfiguration = new Properties();
		    try {
		    	UIconfiguration.load(new FileInputStream(uiFile));
		    } catch (IOException e) {
		    	System.out.println("Error loading configuration file, please confirm existence of zrl.cfg");
		    	System.exit(-1);
		    }
	    }
	}
	
	private static void	title() {
		STMusicManagerNew.thus.playKey("TITLE");
		int choice = Display.thus.showTitleScreen();
		switch (choice){
		case 0:
			newGame();
			break;
		case 1:
			prologue();
			break;
		case 2:
			
			loadGame();
			break;
/*			Display.thus.showHiscores(GameFiles.loadScores());
			title();
			break;*/
		case 3:
			System.out.println("ZeldaRL v"+Game.getVersion()+", clean Exit");
			System.out.println("Thank you for playing!");
			System.exit(0);
			break;
		
		}
		
			
	}
	
	private static void prologue(){
		if (currentGame != null){
			ui.removeCommandListener(currentGame);
			currentGame.resetUniqueRegister();
		}
		currentGame = new Game();
		currentGame.setCanSave(true);
		currentGame.setInterfaces(ui);
		currentGame.prologue();
		
		title();
	}
	
	private static void arena(){
		if (currentGame != null){
			ui.removeCommandListener(currentGame);
		}
		currentGame = new Game();
		currentGame.setCanSave(false);
		currentGame.setInterfaces(ui);
		//si.cls();
		currentGame.arena();
		title();
	}
	
	private static void training(){
		if (currentGame != null){
			ui.removeCommandListener(currentGame);
		}
		currentGame = new Game();
		currentGame.setCanSave(false);
		currentGame.setInterfaces(ui);
		//si.cls();
		currentGame.training();
		title();
	}
	
	private static void loadGame(){
		File saveDirectory = new File("saveGame");
		File[] saves = saveDirectory.listFiles(new SaveGameFilenameFilter() );
		
		int index = Display.thus.showSavedGames(saves);
		if (index == -1)
			title();
		try {
			ObjectInputStream ois = new ObjectInputStream(new FileInputStream(saves[index]));
			currentGame = (Game) ois.readObject();
			ois.close();
		} catch (IOException ioe){
 
			ioe.printStackTrace();
		} catch (ClassNotFoundException cnfe){
			crash("Invalid savefile or wrong version", new CRLException("Invalid savefile or wrong version"));
		}
		currentGame.setInterfaces(ui);
		if (currentGame.getPlayer().getLevel() == null){
			crash("Player wasnt loaded", new Exception("Player wasnt loaded"));
		}
		currentGame.setPlayer(currentGame.getPlayer());
		ui.setPlayer(currentGame.getPlayer());
		
		currentGame.resume();
		
		title();
	}
	
	private static void newGame(){
		if (currentGame != null){
			ui.removeCommandListener(currentGame);
			currentGame.resetUniqueRegister();
		}
		currentGame = new Game();
		currentGame.setCanSave(true);
		currentGame.setInterfaces(ui);
		currentGame.newGame();
		
		title();
	}

	private static void initializeUI(Object si){
		Action walkAction = new Walk();
		Action ready = new ReadyWeapon();
		Action use = new Use();
		Action equip = new Equip();
		Action unequip = new Unequip();
		Action attack = new Attack();
		Action reload = new Reload();
		Action target = new Target();
		Action switchWeapons = new SwitchWeapons();
		Action get = new Get();
		Action drop = new Drop();
		

		UserAction[] userActions = new UserAction[] {
		        new UserAction(attack, CharKey.CTRL),
		        new UserAction(attack, CharKey.a),
		        new UserAction(new Boomerang(), CharKey.z),
		        //new UserAction(thrown, CharKey.t),
		        new UserAction(equip, CharKey.e),
		        new UserAction(ready, CharKey.R),
  		        //new UserAction(reload, CharKey.r),
		        //new UserAction(unequip, CharKey.U),
  		        new UserAction(use, CharKey.u),
		        new UserAction(get, CharKey.g),
		        new UserAction(drop, CharKey.d),
		        new UserAction(target, CharKey.f),
		        new UserAction(switchWeapons, CharKey.x),
		        new UserAction(get, CharKey.COMMA)
		};
		

		UserCommand[] userCommands = new UserCommand[]{
				new UserCommand(CommandListener.PROMPTQUIT, CharKey.Q),
				new UserCommand(CommandListener.HELP, CharKey.F1),
				new UserCommand(CommandListener.LOOK, CharKey.l),
				new UserCommand(CommandListener.PROMPTSAVE, CharKey.S),
				new UserCommand(CommandListener.SHOWSKILLS, CharKey.s),
				new UserCommand(CommandListener.HELP, CharKey.QUESTION),
				new UserCommand(CommandListener.SHOWSTATS, CharKey.c),
				new UserCommand(CommandListener.SHOWMESSAGEHISTORY, CharKey.m),
				new UserCommand(CommandListener.SWITCHMUSIC, CharKey.T),
				new UserCommand(CommandListener.SHOWINVEN, CharKey.i),
				//new UserCommand(CommandListener.EXAMINELEVELMAP, CharKey.M),
				new UserCommand(CommandListener.CHANGE_TACTICS, CharKey.t),
				new UserCommand(CommandListener.CHAR_DUMP, CharKey.D),

		};
		switch (mode){
		case SWING_GFX:
			//((GFXUserInterface)ui).init((SwingSystemInterface)si, userActions, userCommands, walkAction, target, attack, UIconfiguration);
			break;
		case JCURSES_CONSOLE:
			((ConsoleUserInterface)ui).init((ConsoleSystemInterface)si, userActions, userCommands, walkAction, target, attack);
			break;
		case SWING_CONSOLE:
			((ConsoleUserInterface)ui).init((WSwingConsoleInterface)si, userActions, userCommands, walkAction, target, attack);
			break;
		}
		
		
	}
	
	public static void main(String args[]){
		
		mode = JCURSES_CONSOLE;
		uiFile = "slash-retrovga.ui";
		if (args!= null && args.length > 0){
			if (args[0].equalsIgnoreCase("sgfx")){
				mode = SWING_GFX;
				if (args.length > 1)
					uiFile = args[1];
				else
					uiFile = "slash-retrovga.ui";
			}
			else if (args[0].equalsIgnoreCase("jc"))
				mode = JCURSES_CONSOLE;
			else if (args[0].equalsIgnoreCase("sc"))
				mode = SWING_CONSOLE;
		}
		
		init();
		System.out.println("Launching game");
		try {
			title();
		} catch (Exception e){
			Game.crash("Unrecoverable Exception [Press Space]",e);
			//si.waitKey(CharKey.SPACE);
		}
	}

	private static void initializeGAppearances(){
		/*String tileSet = UIconfiguration.getProperty("TILESET");
		String tileSet_dark = UIconfiguration.getProperty("TILESET_DARK");
		Appearance[] definitions = new GFXAppearances(tileSet, tileSet_dark, Integer.parseInt(UIconfiguration.getProperty("TILESIZE"))).getAppearances();
		for (int i=0; i<definitions.length; i++){
			AppearanceFactory.getAppearanceFactory().addDefinition(definitions[i]);
		}*/
	}
	
	private static void initializeCAppearances(){
		Appearance[] definitions = new CharAppearances().getAppearances();
		for (int i=0; i<definitions.length; i++){
			AppearanceFactory.getAppearanceFactory().addDefinition(definitions[i]);
		}
	}
	
	private static void initializeActions(){
		ActionFactory af = ActionFactory.getActionFactory();
		Action[] definitions = new Action[]{
				new MonsterWalk(),
				new MonsterCharge(),
				new MonsterMissile(),
				new SummonMonster(),
		};
		for (int i = 0; i < definitions.length; i++)
			af.addDefinition(definitions[i]);
	}
	
	private static void initializeCells(){
		MapCellFactory.getMapCellFactory().init(Cells.getCellDefinitions(AppearanceFactory.getAppearanceFactory()));
	}
	
	private static void initializeFeatures(){
		FeatureFactory.getFactory().init(Features.getFeatureDefinitions(AppearanceFactory.getAppearanceFactory()));
		SmartFeatureFactory.getFactory().init(SmartFeatures.getSmartFeatures(SelectorFactory.getSelectorFactory()));
	}

	

	private static void initializeSelectors(){
		ActionSelector [] definitions = getSelectorDefinitions();
		for (int i=0; i<definitions.length; i++){
        	SelectorFactory.getSelectorFactory().addDefinition(definitions[i]);
		}
	}

	private static void initializeMonsters() throws CRLException{
		
		MonsterFactory.getFactory().init(MonsterLoader.getMonsterDefinitions("data/monsters.exml"));
	}

	

	private static void initializeItems(){
		ItemFactory.getItemFactory().init(Items.getItemDefinitions());
	}


	private static ActionSelector [] getSelectorDefinitions(){
		ActionSelector [] ret = new ActionSelector[]{
				new WanderToPlayerAI(),
				new RangedAI(),
				new CountDown(),
				new NullSelector(),
				new BasicMonsterAI(),
				new KegSelector()
		};
		return ret;
	}

    public static void crash(String message, Throwable exception){
    	System.out.println("ZeldaRL "+Game.getVersion()+": Error");
        System.out.println("");
        System.out.println("Unrecoverable error: "+message);
        exception.printStackTrace();
        if (currentGame != null){
        	System.out.println("Trying to save game");
        	try {
        		GameFiles.saveGame(currentGame, currentGame.getPlayer(), true);
        	} catch (Exception e){
        		e.printStackTrace();
        		System.out.println("Couldnt save game");
        	}
        }
        System.exit(-1);
    }
}

class SaveGameFilenameFilter implements FilenameFilter {

	public boolean accept(File arg0, String arg1) {
		//if (arg0.getName().endsWith(".sav"))
		if (arg1.endsWith(".sav"))
			return true;
		else
			return false;
	}
	
}