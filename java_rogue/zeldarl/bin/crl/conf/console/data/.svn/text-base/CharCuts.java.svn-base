package crl.conf.console.data;

import java.util.Hashtable;

import crl.game.Game;
import crl.ui.consoleUI.cuts.CharChat;

public class CharCuts {
	public static CharCuts thus;
	
	public static void initializeSingleton(){
		thus = new CharCuts();
	}
	private Hashtable hashCuts = new Hashtable();
	{
		CharChat temp = null;
		temp = new CharChat();
		temp.add("Count Dracula", "We meet again Vampire Killer. You are old now.");
		temp.add("Christopher Belmont", "I came here to fulfill my fate as a Belmont; age bears no relevance.");
		temp.add("Count Dracula", "Look at your own self! And look at me, just reborn from warm innocent blood, you stand no chance against my power!");
		temp.add("Christopher Belmont", "It is for that one same blood that my whip shall seek revenge against thee, dark lord.");
		temp.add("Count Dracula", "HAHAHAHA! Don't make me laugh, pitiful excuse for a warrior, you shall regret these words!");
		hashCuts.put("PRELUDE_DRACULA1", temp);
		temp = new CharChat();
		temp.add("Solieyu Belmont", "Father! I finally understand, I am here to confront my destiny, the destiny marked by my legacy!");
		temp.add("Solieyu Belmont", "... Father? FATHER!");
		temp.add("Count Dracula", "Your father belongs to me now, you are late, son of a Belmont");
		temp.add("Solieyu Belmont", "No! NO! this.. this cannot be! You miserable monster! Die!");
		temp.add("Count Dracula", "HAHAHAHA!");
		hashCuts.put("PRELUDE_DRACULA2", temp);
		temp = new CharChat();
		temp.add("Count Dracula", "Don't be such a fool! Your soul has always been my possession! ");
		temp.add("Solieyu Belmont", "What! What is this! What happens to my body! ARGH!");
		temp.add("Solieyu Belmont", "WARGH!!! WARRRGH!");
		temp.add("Count Dracula", "HAHAHAHA! Now, get out of my sight shameful creature! I have important things to do");
		hashCuts.put("PRELUDE_DRACULA3", temp);
		temp = new CharChat();
		temp.add("Melduck", "We couldn't be on a worse situation... with the charriot like this we cannot make it to Petra, and this creepy forest...");
		temp.add("Melduck", ". . . wait . . . Did you hear that?");
		temp.add("%NAME", "This forest... I can feel... it is suffering...");
		temp.add("%NAME", "We are running out of time, do you have a clue about the castle location?");
		temp.add("Melduck", "Petra is to the northwest, and the castle of Dracula is just east of there... you better proceed on foot... but quickly, I'm afraid I can't get past the forest with my feet like this...");
		temp.add("%NAME", "I will come back with help, dont worry");
		
		hashCuts.put("INTRO_1", temp);
		temp = new CharChat();
		temp.add("???", "Who are you?");
		temp.add("%NAME", "I am %NAME");
		temp.add("???", "Leave this castle while you can, this is not a place for humans!");
		temp.add("%NAME", "I can't, my fate is to go in and confront the dark count!");
		temp.add("???", "Don't make me laugh! You don't even know what awaits you inside stranger!");
		temp.add("???", "I will go in, we will meet inside, if you survive!");
		hashCuts.put("CLARA1", temp);
		temp = new CharChat();
		temp.add("%NAME", "What is this, a garden inside this foul castle?");
		temp.add("%NAME", "And here we have yet another evil forger of darkness whom soul must be freed!");
		temp.add("???", ". . . Stop where you are, son of a Belmont!");
		temp.add("%NAME", "What? How do you...");
		temp.add("???", "You have come pretty far on the castle; this place is safe for you, for now.");
		temp.add("%NAME", "But... who are ---");
		temp.add("???", "I am known as Heliann, the blacksmith maiden. I inhabit the villa of Castlevania, I am here to help the Belmonts to find their path."); 
		temp.add("Heliann", "We are running out of time though... the count is using the souls from the grand Belmonts to perform a ritual that will be catastrophic for the world, to open the portal to hell!");
		temp.add("%NAME", "If they couldn't stop him, I doubt I will be able to!");
		temp.add("Heliann", "Only way to know if you are ready is to confront your fate; death will be the price to pay if you are not the chosen one to carry on with the Belmont legacy!");
		temp.add("Heliann", "Take this key, it opens the castle dungeon, from there you can reach the clock tower, and finally, the castle keep. That is the only way to go. Be careful %NAME Belmont.");
		hashCuts.put("MAIDEN1", temp);
		temp = new CharChat();
		temp.add("%NAME", "Your reign of blood ends here, Count Dracula!");
		temp.add("Count Dracula", "The color of your soul... Amusing... A Belmont!");
		temp.add("%NAME", "And I am here to vanquish you for good. Prepare to fight!");
		temp.add("Count Dracula", "HAHAHA! Humans! Mankind! Carrying hope as their standard, is it true that you cannot see everything is doomed?");
		temp.add("Count Dracula", "Can you not see that you are not the heir of the night hunters? They are mine, already!");
		temp.add("%NAME", "No! our blood will never be yours, our fate will never dissapear, as long as you are a threat to men!");
		temp.add("Count Dracula", "You already rennounced everything when you ran away from your destiny!");
		temp.add("Count Dracula", "Yes, the son of the Belmont, the one true hunter, with his demise he condemned this world!");
		temp.add("%NAME", "Stop talking! It's time to fight!");
		temp.add("Count Dracula", "HAHAHA!");
		hashCuts.put("DRACULA1", temp);
		temp = new CharChat();
		temp.add("Count Dracula", "Argh! You are strong Belmont! But not enough... HAHAHA!");
		temp.add("%NAME", "What?");
		temp.add("Count Dracula", "TASTE MY TRUE POWER!");
		hashCuts.put("DRACULA2", temp);
		temp = new CharChat();
		temp.add("Count Dracula", "HAHAHA! It is worthless! As long as light exists on this world, I shall return as darkness made flesh!");
		temp.add("%NAME", "And my heir, the new Belmonts, we will be there to vanquish you again. DIE!");
		temp.add("Count Dracula", "This cannot be! NOOOOOOOOOOOOOOOOOOOO!!!!!!");
		hashCuts.put("DRACULA3", temp);

	}

	public CharChat get(String ID){
		CharChat ret = (CharChat) hashCuts.get(ID);
		if (ret == null)
			Game.crash("Couldnt find CharChat "+ID, new Exception());
		return ret;
	}
	

}
