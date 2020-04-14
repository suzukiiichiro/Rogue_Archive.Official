package crl.game;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.Vector;

import sz.util.Debug;
import sz.util.FileUtil;
import zrl.player.Equipment;
import zrl.player.GameSessionInfo;
import zrl.player.HiScore;
import zrl.player.MonsterDeath;
import zrl.player.Player;
import zrl.player.Skill;
import crl.ui.UserInterface;

public class GameFiles {
	public static HiScore[] loadScores(){
		Debug.enterStaticMethod("GameFiles", "loadScores");
		HiScore[] ret = new HiScore[10];
        try{
            BufferedReader lectorArchivo = FileUtil.getReader("hiscore.tbl");
            for (int i = 0; i < 10; i++) {
            	String line = lectorArchivo.readLine();
            	String [] regs = line.split(";");
            	HiScore x = new HiScore();
            	x.setName(regs[0]);
            	x.setScore(Integer.parseInt(regs[1]));
            	x.setDate(regs[2]);
            	x.setTurns(regs[3]);
            	x.setDeathString(regs[4]);
            	x.setDeathLevel(Integer.parseInt(regs[5]));
            	ret[i] = x;
            }
            Debug.exitMethod(ret);
            return ret;
        }catch(IOException ioe){
        	Game.crash("Invalid or corrupt hiscore table", ioe);
    	}
    	return null;
	}
	
	public static void saveHiScore (Player player){
		Debug.enterStaticMethod("GameFiles", "saveHiscore");
		int score = player.getCompletion();
		String name = player.getName();
		SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy");
		String now = sdf.format(new Date());

		HiScore [] scores = loadScores();

		try{
			BufferedWriter fileWriter = FileUtil.getWriter("hiscore.tbl");
			for (int i = 0; i < 10; i++){
				if (score > scores[i].getScore()){
           			fileWriter.write(name+";"+score+";"+now+";"+player.getGameSessionInfo().getTurns()+";"+player.getGameSessionInfo().getShortDeathString()+";"+player.getGameSessionInfo().getDeathLevel());
           			fileWriter.newLine();
            		score = -1;
            		if (i == 9)
	            		break;
            	}
            	fileWriter.write(scores[i].getName()+";"+scores[i].getScore()+";"+scores[i].getDate()+";"+scores[i].getTurns()+";"+scores[i].getDeathString()+";"+scores[i].getDeathLevel());
            	fileWriter.newLine();
            }
            fileWriter.close();
            Debug.exitMethod();
        }catch(IOException ioe){
        	ioe.printStackTrace(System.out);
			Game.crash("Invalid or corrupt hiscore table", ioe);
        }
	}

	public static void saveMemorialFile(Player player){
		try{
			SimpleDateFormat sdf = new SimpleDateFormat("dd-MM-yyyy hh-mm-ss");
			String now = sdf.format(new Date());
			BufferedWriter fileWriter = FileUtil.getWriter("memorials/"+player.getName()+" ("+now+").life");
			GameSessionInfo gsi = player.getGameSessionInfo();
			gsi.setDeathLevelDescription(player.getLevel().getDescription());
			String heshe = (player.getSex() == Player.MALE ? "He" : "She");
			
			fileWriter.write(" ZeldaRL"+Game.getVersion()+ " Player Record");fileWriter.newLine();
			fileWriter.newLine();fileWriter.newLine();
			fileWriter.write(player.getName()+ ", "+gsi.getDeathString()+" on the "+gsi.getDeathLevelDescription()+"...");fileWriter.newLine();
			fileWriter.write(heshe+" survived for "+gsi.getTurns()+" turns and completed "+player.getCompletion()+".");fileWriter.newLine();
			fileWriter.newLine();
			/*fileWriter.write(heshe +" had the following skills:");fileWriter.newLine();
			Vector skills = player.getAvailableSkills();
			for (int i = 0; i < skills.size(); i++){
				fileWriter.write(((Skill)skills.elementAt(i)).getMenuDescription());fileWriter.newLine();
			}
			fileWriter.newLine();*/
			Vector history = gsi.getHistory();
			for (int i = 0; i < history.size(); i++){
				fileWriter.write(heshe + " " + history.elementAt(i));
				fileWriter.newLine();
			}
			fileWriter.newLine();
			fileWriter.write(heshe +" killed "+gsi.getTotalDeathCount()+" monsters");fileWriter.newLine();
			
			int i = 0;
			Enumeration monsters = gsi.getDeathCount().elements();
			while (monsters.hasMoreElements()){
				MonsterDeath mons = (MonsterDeath) monsters.nextElement();
				fileWriter.write(mons.getTimes() +" "+mons.getMonsterDescription());fileWriter.newLine();
				
				i++;
			}
			fileWriter.newLine();
			fileWriter.write("-- Final Stats --");fileWriter.newLine();fileWriter.newLine();
			
			fileWriter.write(player.getName()+" "+player.getStatusString()+" Gold: "+player.getGold()+" Completion: "+player.getCompletion());fileWriter.newLine();
			fileWriter.write("Sex: "+ (player.getSex() == Player.MALE ? "M" : "F"));fileWriter.newLine();
			
			fileWriter.write("Hearts: "+player.getHearts()+ "/"+player.getHeartsMax());fileWriter.newLine();
			fileWriter.write("Magic: "+player.getMagic()+ "/"+player.getMagicMax());fileWriter.newLine();
			fileWriter.write("Carrying: "+player.getItemCount()+"/"+player.getCarryMax());fileWriter.newLine();
			fileWriter.write("Sight: "+player.getBaseSightRange()+" ("+player.getSightRange()+")");fileWriter.newLine();
			fileWriter.write("Speed: "+((player.getBaseWalkCost()-50)*-1)+ " ("+((player.getWalkCost()-50)*-1)+")");fileWriter.newLine();
		    
		    
			Vector inventory = player.getInventory();
			fileWriter.newLine();
			fileWriter.write("-- Inventory --");fileWriter.newLine();
			fileWriter.write("Weapon     "+player.getEquipedWeaponDescription());fileWriter.newLine();
			fileWriter.write("Readied    "+player.getSecondaryWeaponDescription());fileWriter.newLine();
			fileWriter.write("Armor      "+player.getArmorDescription());fileWriter.newLine();
			fileWriter.write("Item       "+player.getAccDescription());fileWriter.newLine();fileWriter.newLine();
			
			for (Iterator iter = inventory.iterator(); iter.hasNext();) {
				Equipment element = (Equipment) iter.next();
				fileWriter.write(element.getQuantity()+ " - "+ element.getMenuDescription());fileWriter.newLine();
			}
			Vector stuff = new Vector();
		    if (player.getFlag("RAFT"))
		    	stuff.add("Raft");
		    if (player.getFlag("BOOMERANG"))
		    	stuff.add("Blue Boomerang");
		    if (player.getFlag("HOOKSHOOT"))
		    	stuff.add("Hookshoot");
		    if (player.getFlag("FLIPPERS"))
		    	stuff.add("Flippers");
		    if (player.getFlag("CANE_BYRNA"))
		    	stuff.add("The Cane of Byrna");
		    if (player.getFlag("CANE_SOMARIA"))
		    	stuff.add("The Cane of Somaria");
		    if (player.getFlag("CANE_YENDOR"))
		    	stuff.add("The Cane of Yendor");
		    
		    for (int j = 0; j < stuff.size(); j++){
		    	fileWriter.write(stuff.elementAt(j).toString());fileWriter.newLine();
		    }
		    	
			fileWriter.newLine();
			fileWriter.write("-- Last Messages --");fileWriter.newLine();
			Vector messages = UserInterface.getUI().getMessageBuffer();
			for (int j = 0; j < messages.size(); j++){
				fileWriter.write(messages.elementAt(j).toString());fileWriter.newLine();
			}
			
			fileWriter.close();
		} catch (IOException ioe){
			Game.crash("Error writing the memorial file", ioe);
		}
		
	}
	
	public static void saveGame(Game g, Player p){
		String filename = "savegame/"+p.getName()+".sav";
		try {
			ObjectOutputStream os = new ObjectOutputStream(new FileOutputStream(filename));
			p.setSelector(null);
			os.writeObject(g);
			os.close();
		} catch (IOException ioe){
			Game.crash("Error saving the game", ioe);
		}
	}
	
	public static void saveGame(Game g, Player p, boolean dontCrash){
		String filename = "savegame/"+p.getName()+".sav";
		try {
			ObjectOutputStream os = new ObjectOutputStream(new FileOutputStream(filename));
			p.setSelector(null);
			os.writeObject(g);
			os.close();
		} catch (IOException ioe){
			if (dontCrash)
				System.out.println("Error saving the game");
			else
				Game.crash("Error saving the game", ioe);
		}
	}
	
	public static void permadeath(Player p){
		String filename = "savegame/"+p.getName()+".sav";
		if (FileUtil.fileExists(filename)) {
			FileUtil.deleteFile(filename);
		}
	}
	
	public static void saveChardump(Player player){
		try{
			SimpleDateFormat sdf = new SimpleDateFormat("dd-MM-yyyy hh-mm-ss");
			String now = sdf.format(new Date());
			BufferedWriter fileWriter = FileUtil.getWriter("memorials/"+player.getName()+" {Alive}("+now+").life");
			GameSessionInfo gsi = player.getGameSessionInfo();
			gsi.setDeathLevelDescription(player.getLevel().getDescription());
			String heshe = (player.getSex() == Player.MALE ? "He" : "She");
			String hisher = (player.getSex() == Player.MALE ? "his" : "her");
			
			fileWriter.write(" Mt Drash "+Game.getVersion()+ " Character Dump");fileWriter.newLine();
			fileWriter.newLine();fileWriter.newLine();
			fileWriter.write(player.getName()+ ", fights for "+hisher+" freedom on the Garrintroot Arena");fileWriter.newLine();
			fileWriter.write(heshe+" has survived for "+gsi.getTurns()+" turns and has completed "+player.getCompletion()+".");fileWriter.newLine();
			fileWriter.newLine();
			fileWriter.write(heshe +" has the following skills:");fileWriter.newLine();
			Vector skills = player.getAvailableSkills();
			for (int i = 0; i < skills.size(); i++){
				fileWriter.write(((Skill)skills.elementAt(i)).getMenuDescription());fileWriter.newLine();
			}
			fileWriter.newLine();
			Vector history = gsi.getHistory();
			for (int i = 0; i < history.size(); i++){
				fileWriter.write(heshe + " " + history.elementAt(i));
				fileWriter.newLine();
			}
			fileWriter.newLine();
			fileWriter.write(heshe +" has vanquished "+gsi.getTotalDeathCount()+" monsters");fileWriter.newLine();
			
			int i = 0;
			Enumeration monsters = gsi.getDeathCount().elements();
			while (monsters.hasMoreElements()){
				MonsterDeath mons = (MonsterDeath) monsters.nextElement();
				fileWriter.write(mons.getTimes() +" "+mons.getMonsterDescription());fileWriter.newLine();
				
				i++;
			}
			fileWriter.newLine();
			fileWriter.write("-- Current Stats --");fileWriter.newLine();fileWriter.newLine();
			
			fileWriter.write(player.getName()+" "+player.getStatusString()+" Gold: "+player.getGold()+" Completion: "+player.getCompletion());fileWriter.newLine();
			fileWriter.write("Sex: "+ (player.getSex() == Player.MALE ? "M" : "F"));fileWriter.newLine();
			
			fileWriter.write("Hearts: "+player.getHearts()+ "/"+player.getHeartsMax());fileWriter.newLine();
			fileWriter.write("Magic: "+player.getMagic()+ "/"+player.getMagicMax());fileWriter.newLine();
			fileWriter.write("arrying: "+player.getItemCount()+"/"+player.getCarryMax());fileWriter.newLine();
			fileWriter.write("Sight: "+player.getBaseSightRange()+" ("+player.getSightRange()+")");fileWriter.newLine();
			fileWriter.write("Speed: "+((player.getBaseWalkCost()-50)*-1)+ " ("+((player.getWalkCost()-50)*-1)+")");fileWriter.newLine();

		    
			Vector inventory = player.getInventory();
			fileWriter.newLine();
			fileWriter.write("-- Inventory --");fileWriter.newLine();
			fileWriter.write("Weapon     "+player.getEquipedWeaponDescription());fileWriter.newLine();
			fileWriter.write("Readied    "+player.getSecondaryWeaponDescription());fileWriter.newLine();
			fileWriter.write("Armor      "+player.getArmorDescription());fileWriter.newLine();
			fileWriter.write("Item       "+player.getAccDescription());fileWriter.newLine();fileWriter.newLine();
			
			for (Iterator iter = inventory.iterator(); iter.hasNext();) {
				Equipment element = (Equipment) iter.next();
				fileWriter.write(element.getQuantity()+ " - "+ element.getMenuDescription());fileWriter.newLine();
			}
			Vector stuff = new Vector();
		    if (player.getFlag("RAFT"))
		    	stuff.add("Raft");
		    if (player.getFlag("BOOMERANG"))
		    	stuff.add("Blue Boomerang");
		    if (player.getFlag("HOOKSHOOT"))
		    	stuff.add("Hookshoot");
		    if (player.getFlag("FLIPPERS"))
		    	stuff.add("Flippers");
		    if (player.getFlag("CANE_BYRNA"))
		    	stuff.add("The Cane of Byrna");
		    if (player.getFlag("CANE_SOMARIA"))
		    	stuff.add("The Cane of Somaria");
		    if (player.getFlag("CANE_YENDOR"))
		    	stuff.add("The Cane of Yendor");
		    
		    for (int j = 0; j < stuff.size(); j++){
		    	fileWriter.write(stuff.elementAt(j).toString());fileWriter.newLine();
		    }
		    	
			fileWriter.newLine();
			fileWriter.write("-- Latest Messages --");fileWriter.newLine();
			Vector messages = UserInterface.getUI().getMessageBuffer();
			for (int j = 0; j < messages.size(); j++){
				fileWriter.write(messages.elementAt(j).toString());fileWriter.newLine();
			}
			
			fileWriter.close();
		} catch (IOException ioe){
			Game.crash("Error writing the character dump", ioe);
		}
		
	}
}
