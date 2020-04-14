package crl.ui;

import java.util.*;

import sz.csi.CharKey;
import sz.csi.ConsoleSystemInterface;
import sz.csi.textcomponents.ListBox;
import sz.csi.textcomponents.TextBox;
import sz.csi.textcomponents.TextInformBox;
import sz.util.*;
import sz.csi.*;
import zrl.*;
import zrl.player.*;

import crl.action.*;
//import crl.action.vkiller.Whip;
import crl.ui.effects.*;
import crl.item.*;
import crl.level.*;
import crl.monster.*;
import crl.feature.*;
import crl.ai.*;
import crl.actor.*;


/** 
 *  Shows the level
 *  Informs the Actions and Commands of the player.
 * 	Must be listening to a System Interface
 */

public abstract class UserInterface implements CommandListener, Runnable{
	//Attributes
	//private String[] quitMessages;
	protected String[] quitMessages = new String[]{
		"Do you really want to abandon Hyrule?"
	};
	
	protected Hashtable gameActions = new Hashtable();
	protected Hashtable gameCommands = new Hashtable(); 
	
	//Status
	protected Vector monstersOnSight = new Vector();
	protected Vector featuresOnSight = new Vector();
	protected Vector itemsOnSight = new Vector();
	protected Action actionSelectedByCommand;
	
	//Components
	private Vector commandListeners = new Vector (5); // Class CommandListener

	protected Action advance;
	protected Action attack;
	protected Action target;
	//protected Position defaultTarget;

	protected boolean eraseOnArrival; // Erase the buffer upon the arrival of a new msg
   	
	protected String lastMessage; 
	protected Level level;
    // Relations
	protected Player player;

	
    private transient ConsoleSystemInterface si;

	// Setters
	/** Sets the object which will be informed of the player commands.
     * this corresponds to the Game object */
	public void addCommandListener(CommandListener pCl) {
		commandListeners.add(pCl);
    }
	
	public void removeCommandListener(CommandListener pCl){
		commandListeners.remove(pCl);
	}
	
	//Getters
    public Player getPlayer() {
		return player;
	}

    // Smart Getters
	protected Action getRelatedAction(int keyCode){
    	Debug.enterMethod(this, "getRelatedAction", keyCode+"");
    	UserAction ua = (UserAction) gameActions.get(keyCode+"");
    	if (ua == null){
    		Debug.exitMethod("null");
    		return null;
    	}
    	Action ret = ua.getAction();
		Debug.exitMethod(ret);
		return ret;
	}

	protected int getRelatedCommand(int keyCode){
		Debug.enterMethod(this, "getRelatedCommand", keyCode+"");
    	UserCommand uc = (UserCommand ) gameCommands.get(keyCode+"");
    	if (uc == null){
    		Debug.exitMethod(CommandListener.NONE);
    		return CommandListener.NONE;
    	}

    	int ret = uc.getCommand();
    	Debug.exitMethod(ret+"");
    	return ret;
	}
    
    //  Final attributes
    
    public final static String verboseSkills[] = new String []{
		"Unskilled", "Mediocre", "Trained", "Skilled", "Master"};

    private final int WEAPONCODE = CharKey.SPACE;

    private boolean [][] FOVMask;
    //Interactive Methods
    public abstract void doLook();
    


    // Drawing Methods
	public abstract void drawEffect(Effect what);
	
	public boolean isOnFOVMask(int x, int y){
		return FOVMask[x][y];
	}

	public abstract void addMessage(Message message);
	public abstract Vector getMessageBuffer();

    protected void informPlayerCommand(int command) {
	    Debug.enterMethod(this, "informPlayerCommand", command+"");
	    for (int i =0; i < commandListeners.size(); i++){
	    	((CommandListener)commandListeners.elementAt(i)).commandSelected(command);
	    }
		Debug.exitMethod();
    }
	
	public void setPlayer(Player pPlayer){
		player = pPlayer;
		level = player.getLevel();
	}

	public void init(UserAction[] gameActions, UserCommand[] gameCommands,
		Action advance, Action target, Action attack){
		Debug.enterMethod(this, "init");
		this.advance = advance;
		this.target = target;
		this.attack = attack;
		for (int i = 0; i < gameActions.length; i++){
			this.gameActions.put(gameActions[i].getKeyCode()+"", gameActions[i]);
		}
		for (int i = 0; i < gameCommands.length; i++)
			this.gameCommands.put(gameCommands[i].getKeyCode()+"", gameCommands[i]);
		
		addCommandListener(this);
		
		FOVMask = new boolean[80][25];
		Debug.exitMethod();
	}

	public abstract boolean isDisplaying(Actor who);

    public void levelChange(){
		level = player.getLevel();
	}
    
    /*
     * Prompts for Yes or NO
     */
    public abstract boolean prompt ();

	public abstract void refresh();


 	/**
     * Shows a message inmediately; useful for system
     * messages.
     *  
     * @param x the message to be shown
     */
	public abstract void showMessage(String x);

	/**
     * Shows a message inmediately; useful for system
     * messages. Waits for a key press or something.
     *  
     * @param x the message to be shown
     */
	public abstract void showSystemMessage(String x);
	

	
	private Action selectCommand (CharKey input){
		Debug.enterMethod(this, "selectCommand", input);
		int com = getRelatedCommand(input.code);
		informPlayerCommand(com);
		Action ret = actionSelectedByCommand;
		actionSelectedByCommand = null;
		Debug.exitMethod(ret);
		return ret;
	}
	
	public abstract ActionSelector getSelector();
	
	public abstract void processQuit();
	
	public abstract void processSave();
	
	public abstract void showPlayerStats();
	
	public abstract Action showInventory() throws ActionCancelException;
	
	public abstract Action showSkills() throws ActionCancelException;
	
	public void commandSelected (int commandCode){
		switch (commandCode){
			case CommandListener.PROMPTQUIT:
				processQuit();
				break;
			case CommandListener.PROMPTSAVE:
				processSave();
				break;
			case CommandListener.HELP:
				si.saveBuffer();
				Display.thus.showHelp();
				si.restore();
				si.refresh();
				break;
			case CommandListener.LOOK:
				doLook();
				break;
			case CommandListener.SHOWSTATS:
				showPlayerStats();
				break;
			case CommandListener.SHOWINVEN:
				try {
					actionSelectedByCommand = showInventory();
				} catch (ActionCancelException ace){

				}
				break;
			case CommandListener.SHOWSKILLS:
				try {
					actionSelectedByCommand = showSkills();
				} catch (ActionCancelException ace){

				}
				break;
		}
	}
	
	protected boolean gameOver;
	
	public void setGameOver(boolean bal){
		gameOver = bal;
	}
	
	//	 Singleton
	private static UserInterface singleton;
	
	public static void setSingleton(UserInterface ui){
		singleton = ui;
	}
	public static UserInterface getUI (){
		return singleton;
	}
	
	public abstract void showMessageHistory();
	
	public abstract Item pickUnderlyingItem(String prompt) throws ActionCancelException;
	public abstract Position pickPosition(String prompt) throws ActionCancelException;
	public abstract Item pickItem(String prompt) throws ActionCancelException;
	public abstract Item pickEquipedItem(String prompt) throws ActionCancelException;
	public abstract int pickDirection(String prompt) throws ActionCancelException;
}