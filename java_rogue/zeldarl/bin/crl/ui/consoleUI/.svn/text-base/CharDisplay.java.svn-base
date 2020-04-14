package crl.ui.consoleUI;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.util.Vector;


import crl.ui.CommandListener;
import crl.ui.Display;
import crl.ui.UserAction;
import crl.ui.UserCommand;
import crl.ui.UserInterface;
import crl.ui.consoleUI.cuts.CharChat;
import crl.conf.console.data.CharCuts;
import crl.game.Game;
import crl.game.STMusicManagerNew;
import sz.csi.CharKey;
import sz.csi.ConsoleSystemInterface;
import sz.csi.textcomponents.TextBox;
import sz.util.FileUtil;
import sz.util.ScriptUtil;
import zrl.player.GameSessionInfo;
import zrl.player.HiScore;
import zrl.player.Player;
//import zrl.player.advancements.Advancement;

public class CharDisplay extends Display{
	private ConsoleSystemInterface si;
	
	public CharDisplay(ConsoleSystemInterface si){
		this.si = si;
	}
	
	public int showTitleScreen(){
		si.cls();
		String tempString = "The Legend of Zelda - The Rainbow Maiden"; 
		si.print((80-tempString.length())/2,3, tempString, ConsoleSystemInterface.GREEN);
		tempString = "a. New Game";
		si.print((80-tempString.length())/2,9, tempString, ConsoleSystemInterface.WHITE);
		tempString = "b. Prelude";
		si.print((80-tempString.length())/2,10, tempString, ConsoleSystemInterface.WHITE);
		tempString = "c. Load Game";
		si.print((80-tempString.length())/2,11, tempString, ConsoleSystemInterface.WHITE);
		tempString = "d. Quit";
		si.print((80-tempString.length())/2,12, tempString, ConsoleSystemInterface.WHITE);
		tempString = "ZeldaRL v"+Game.getVersion()+", Slash ~ 2007";
		si.print((80-tempString.length())/2,19, tempString, ConsoleSystemInterface.WHITE);
		tempString = "Zelda and all used names are a copyright of Nintendo";
		si.print((80-tempString.length())/2,20, tempString, ConsoleSystemInterface.WHITE);
		tempString = "This project is not associated in any way to Nintendo Co.";
		si.print((80-tempString.length())/2,21, tempString, ConsoleSystemInterface.WHITE);
		
		si.refresh();
    	STMusicManagerNew.thus.playKey("TITLE");
    	CharKey x = new CharKey(CharKey.NONE);
		while (x.code != CharKey.A && x.code != CharKey.a &&
				x.code != CharKey.B && x.code != CharKey.b &&
				x.code != CharKey.C && x.code != CharKey.c &&
				x.code != CharKey.D && x.code != CharKey.d)
			x = si.inkey();
		si.cls();
		switch (x.code){
		case CharKey.A: case CharKey.a:
			return 0;
		case CharKey.B: case CharKey.b:
			return 1;
		case CharKey.C: case CharKey.c:
			return 2;
		case CharKey.D: case CharKey.d:
			return 3;
		}
		return 0;
		
	}
	
	public void showIntro(Player player){
		si.cls();
		showScreen("Seven days ago, the world changed.... XXX Yellow flames covered the skies of Hyrule, summoning "+
				"everything into darkness. You have been hidding in your house, as everybody else has, while "+
"monsters roam the countryside and the world seems to tear apart. XXX This night you have been rolling in your "+
"bed for what appears to be ages. Finally, you are asleep.");
		
		showScreen("You open your eyes to find yourself standing in front of a strange looking " +
				"cavern of white stone. Your sword and shield are in your hands, and you feel a voice " +
				"inside your head, welcoming you in. What is this about? you decide to enter the cavern, and " +
				"find it out by yourself.");
		si.cls();

	}
	
	public void showIntro2(Player player){
		si.cls();
		showScreen("Seven days ago, the world changed.... XXX XXX Yellow flames covered the skies of Hyrule, summoning "+
				"everything into darkness. You have been hidding in your house, as everybody else has, while "+
"monsters roam the countryside and the world seems to tear apart. XXX XXX This night you have been rolling in your "+
"bed for what appears to be ages. XXX XXX Finally, you are asleep.");
		
		showScreen("You open your eyes to find yourself standing in front of a strange looking " +
				"cavern of white stone. XXX XXX Your sword and shield are in your hands, and you feel a voice " +
				"inside your head, welcoming you in. What is this about? you decide to enter the cavern, and " +
				"find it out by yourself.");
		
		showScreen("Entering the cavern, you meet a faint white shadow, which after some " +
				"sword hits vanishes into thin smoke. Again, you can feel the strange voice, echoing in " +
				"your head... XXX XXX \"You are brave! It is now your fate, to go on and find the three ancient canes and save Hyrule!...\"  XXX XXX XXX You wake up in your house... What is this about? There is only one way to find out!");
		si.cls();

	}
	
	public boolean showResumeScreen(Player player){
		GameSessionInfo gsi = player.getGameSessionInfo();
		si.cls();
		
		si.print(2,5, player.getName()+" is dead.", ConsoleSystemInterface.GREEN);
		
		TextBox tb = new TextBox(si);
		tb.setPosition(2,7);
		tb.setHeight(3);
		tb.setWidth(70);
		tb.setForeColor(ConsoleSystemInterface.WHITE);
		tb.setText(player.getName() + ", "+gsi.getDeathString()+" on the "+player.getLevel().getDescription()+"... ");
		tb.draw();
		
		si.print(2,11, "Save memorial file? [Y/N]");
		si.refresh();
		return UserInterface.getUI().prompt();
	}

	public void showEndgame(Player player){
		si.cls();
		
		TextBox tb1 = new TextBox(si);
		TextBox tb2 = new TextBox(si);
		tb2.setForeColor(ConsoleSystemInterface.WHITE);
		tb1.setForeColor(ConsoleSystemInterface.WHITE);

		tb1.setPosition(1,3);
		tb1.setHeight(6);
		tb1.setWidth(79);

		tb2.setPosition(1,10);
		tb2.setHeight(5);
		tb2.setWidth(79);

		tb1.setText(
"After you put the three canes over the altars, a shimmering blast of light covers all the "+
"temple, and the monsters vanish... A voice thunders all around the place... \"The portal to Byrna has been opened, go in and find the answers to your questions\"");

		tb2.setText(
"You step into the portal and appear into a world devasted by cold and snow. The adventure has just begun.");

		tb1.draw();
		tb2.draw();
		si.print(2,18, "[Press Space]", ConsoleSystemInterface.WHITE);
		si.refresh();
		si.waitKey(CharKey.SPACE);
		si.cls();
		tb1.clear();tb2.clear();
		tb1.setText(
				"Congratulations! You have finished the 7DRL version of ZeldaRL. As you may have noticed, "+
				"there are lots of things to improve. All your comments at the official website, " +
				"http://www.santiagoz.com/web are more than welcome!");

						tb2.setText(
				"Let's work together to shape this into a great game! Thank you for playing!");

						tb1.draw();
						tb2.draw();
						si.print(2,18, "[Press Space]", ConsoleSystemInterface.WHITE);
						
						si.refresh();
						si.waitKey(CharKey.SPACE);				
		si.cls();
	}
	
	public void showHiscores(HiScore[] scores){
		si.cls();


		si.print(2,1, "DrashRL: ARENA OF DEATH "+Game.getVersion(), ConsoleSystemInterface.WHITE);
		si.print(2,2, "Legendary Challengers", ConsoleSystemInterface.WHITE);

		si.print(13,4, "Score");
		si.print(20,4, "Date");	
		si.print(31,4, "Turns");
		si.print(38,4, "Death");

		for (int i = 0; i < scores.length; i++){
			si.print(2,5+i, scores[i].getName(), ConsoleSystemInterface.WHITE);
			si.print(13,5+i, ""+scores[i].getScore());
			si.print(20,5+i, ""+scores[i].getDate());
			si.print(31,5+i, ""+scores[i].getTurns());
			si.print(38,5+i, ""+scores[i].getDeathString()+" on level "+scores[i].getDeathLevel());

		}
		si.print(2,23, "[Press Space]");
		si.refresh();
		si.waitKey(CharKey.SPACE);
		si.cls();
	}
	
	public void showHelp(){
		si.cls();
	    
		si.print(2,0, "Help", ConsoleSystemInterface.WHITE);
		si.print(2,1, "Actions", ConsoleSystemInterface.WHITE);
		si.print(2,2,  "a Attack: Uses a weapon in a given direction");
		si.print(2,3,  "d Drop: Discards an item");
		si.print(2,4,  "e Equip: Wears a weapon or armor");
		si.print(2,5,  "f Fire: Targets a weapon on a given position");
		si.print(2,6,  "g Get: Picks up an item");
		//si.print(2,7,  "r Reload: Reloads the equiped weapon");
		si.print(2,7,  "R Ready: Readies a secondary weapon");
		si.print(2,8, "s Skills: Shows the skill list");
		si.print(2,9, "t Tactics: Switches normal and agressive tactics");
		si.print(2,10,  "u Use: Uses an Item");
		//si.print(2,12,  "U Unequip: Take off an item");
		si.print(2,11,  "x Switch: Switched primary and secondary weapon");
		si.print(2,12,  "z Boomerang");
		

		si.print(2,14, "Commands", ConsoleSystemInterface.WHITE);

		si.print(2,15, "? Help: Shows Help (this screen)");
		si.print(2,16, "c Character: Shows the character stats");
		si.print(2,17, "D Dump: Dumps you character info to a file");
		si.print(2,18, "i Inventory: Shows the inventory");
		si.print(2,19, "l Look: Identifies map symbols and monsters");
		si.print(2,20, "m Messages: Shows the message history");
		//si.print(2,21, "M Map: Shows the level map in multiple screens");
		si.print(2,21, "T Music: Turns music on/off");
		si.print(2,22, "Q Quit: Exits game");
		si.print(2,23, "S Save: Saves game");
		
		
		//si.print(2,24,"[Press Space to continue]");

		si.refresh();
		si.waitKey(CharKey.SPACE);
		si.cls();
	}
	
	public void init(ConsoleSystemInterface syst){
		si = syst;
	}
	
	public int showSavedGames(File[] saveFiles){
		si.cls();
		if (saveFiles.length == 0){
			si.print(3,6, "Empty Data");
			si.print(4,8, "[Space to Cancel]");
			si.refresh();
			si.waitKey(CharKey.SPACE);
			return -1;
		}
			
		si.print(3,6, "Select a Slot");
		for (int i = 0; i < saveFiles.length; i++){
			si.print(5,7+i, (char)(CharKey.a+i+1)+ " - "+ saveFiles[i].getName());
		}
		si.print(3,9+saveFiles.length, "[Space to Cancel]");
		si.refresh();
		CharKey x = si.inkey();
		while ((x.code < CharKey.a || x.code > CharKey.a+saveFiles.length) && x.code != CharKey.SPACE){
			x = si.inkey();
		}
		si.cls();
		if (x.code == CharKey.SPACE)
			return -1;
		else
			return x.code - CharKey.a;
	}
	
	private int readAlphaToNumber(int numbers){
		while (true){
			CharKey key = si.inkey();
			if (key.code >= CharKey.A && key.code <= CharKey.A + numbers -1){
				return key.code - CharKey.A;
			}
			if (key.code >= CharKey.a && key.code <= CharKey.a + numbers -1){
				return key.code - CharKey.a;
			}
		}
	}
	
	public void showChat(String chatID, Game game){
		si.saveBuffer();
		CharChat chat = CharCuts.thus.get(chatID);
		TextBox tb = new TextBox(si);
		tb.setBounds(3,4,40,10);
		tb.setBorder(true);
		String[] marks = new String[] {"%NAME"};
		String[] replacements = new String[] {game.getPlayer().getName()};
		for (int i = 0; i < chat.getConversations(); i++){
			tb.clear();
			tb.setText(ScriptUtil.replace(marks, replacements, chat.getConversation(i)));
			tb.setTitle(ScriptUtil.replace(marks, replacements, chat.getName(i)));
			tb.draw();
			si.refresh();
			si.waitKey(CharKey.SPACE);
		}
		si.restore();
		si.refresh();
	}
	
	/*public void showScreen(Object pScreen){
		si.saveBuffer();
		si.cls();
		try {
			BufferedReader red = FileUtil.getReader((String)pScreen);
			String line = red.readLine();
			int i = 0;
			while (line != null){
				si.print(1, i+2, line);
				i++;
				line = red.readLine();
			}
		} catch (IOException ioe){
			Game.crash("Error loading screen "+pScreen, ioe);
		}
		si.refresh();
		si.waitKey(CharKey.SPACE);
		si.restore();
	}*/
	
	public void showScreen(Object pScreen){
		si.saveBuffer();
		si.cls();
		
		TextBox tb1 = new TextBox(si);
		
		tb1.setForeColor(ConsoleSystemInterface.WHITE);

		tb1.setPosition(1,3);
		tb1.setHeight(10);
		tb1.setWidth(78);

		
		tb1.setText(pScreen.toString());
		
		tb1.draw();
		si.refresh();
		si.waitKey(CharKey.SPACE);
		si.restore();
		si.refresh();
	}
	
	
	

	/*public Advancement levelUp(Player p) {
		return null;
	}*/
	

}
