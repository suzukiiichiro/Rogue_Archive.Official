package crl.action;

import java.util.Vector;
import sz.util.Line;
import sz.util.Position;
import sz.util.Util;
import zrl.player.Equipment;
import zrl.player.Player;
import crl.feature.Feature;
import crl.item.ItemDefinition;
import crl.level.Cell;
import crl.level.Level;
import crl.monster.Monster;
import crl.monster.MonsterFactory;
import crl.ui.ActionCancelException;
import crl.ui.UserInterface;
import crl.ui.effects.EffectFactory;

public class Use extends Action{
	public String getID(){
		return "Use";
	}
	
	public boolean needsItem(){
		return true;
    }

    public String getPromptItem(){
    	return "What do you want to use?";
	}

	public void execute(){

		Player aPlayer = (Player) performer;
		ItemDefinition def = targetItem.getDefinition();
		String[] effect = def.getEffectOnUse().split(" ");
		
		if (def.getID().equals("SOUL_RECALL")){
			aPlayer.informPlayerEvent(Player.EVT_GOTO_LEVEL, "TOWN");
			aPlayer.getLevel().setLevelNumber(0);
			aPlayer.landOn(Position.add(aPlayer.getLevel().getExitFor("_NEXT"), new Position(-1,0,0)));
			aPlayer.reduceQuantityOf(targetItem);
			return;
		}
		
		if (def.getID().equals("BRASS_BUTTON")){
			if (aPlayer.getLevel().getMapCell(aPlayer.getPosition()).getID().equals("ROCKET")){
				Vector inventory = aPlayer.getInventory();
				for (int i = 0; i < inventory.size(); i++){
					Equipment eq = (Equipment)inventory.elementAt(i);
					if (eq.getItem().getDefinition().getID().equals("TRILITHIUM")){
						if (eq.getQuantity() >= 15){
							for (int ii = 0; ii < 15	; ii++){
								aPlayer.reduceQuantityOf(eq.getItem());
							}
							int levelNumber = aPlayer.getLevel().getLevelNumber();
							aPlayer.addHistoricEvent("reached planet X!");
							aPlayer.informPlayerEvent(Player.EVT_GOTO_LEVEL, "SPACE");
							aPlayer.getLevel().setLevelNumber(levelNumber);
							aPlayer.setPosition(aPlayer.getLevel().getExitFor("_BACK"));
							return;
						}
					}
				}
				
			}
		}
		
		if (def.getID().equals("KEG")){
			aPlayer.getLevel().addMessage("You light the Powder Keg! RUN!");
			aPlayer.getLevel().addSmartFeature("KEG", aPlayer.getPosition());
			aPlayer.reduceQuantityOf(targetItem);
			return;
		}
		
		if (effect[0].equals("")){
			performer.getLevel().addMessage("You don\'t find a use for the "+targetItem.getDescription());
			//aPlayer.addItem(targetItem);
			return;
		}       
		
		for (int cmd = 0; cmd < effect.length; cmd+=2){
			String message = targetItem.getUseMessage();
			if (message.equals(""))
				message = "You use the "+targetItem.getDescription();
			performer.getLevel().addMessage(message);
			if (effect[cmd].equals("INCREASE_DEFENSE"))
				aPlayer.setCounter("PROTECTION", Integer.parseInt(effect[cmd+1]));
			else
			if (effect[cmd].equals("INVINCIBILITY"))
				aPlayer.setInvincible(Integer.parseInt(effect[cmd+1]));
			else
			if (effect[cmd].equals("INCREASE_JUMPING"))
				aPlayer.increaseJumping(Integer.parseInt(effect[cmd+1]));
			else
			if (effect[cmd].equals("RECOVERP"))
				aPlayer.recoverHeartsP(Integer.parseInt(effect[cmd+1]));
			else
			if (effect[cmd].equals("RECOVER"))
				aPlayer.recoverHearts(Integer.parseInt(effect[cmd+1]));
			else
			if (effect[cmd].equals("RECOVERALL")){
				aPlayer.recoverMagicP(100);
				aPlayer.recoverHeartsP(100);
			}
			else
			if (effect[cmd].equals("RECOVERMANAP"))
				aPlayer.recoverMagicP(Integer.parseInt(effect[cmd+1]));
			else
			if (effect[cmd].equals("RECOVERMANA"))
				aPlayer.recoverMagic(Integer.parseInt(effect[cmd+1]));
			else
			if (effect[cmd].equals("SIGHT"))
				aPlayer.setCounter("TORCH", Integer.parseInt(effect[cmd+1]));
			else
			if (effect[cmd].equals("CAST")){
				
					if (effect[cmd+1].equals("VIEW")){
						Position runner = new Position(0,0);
						for (int i = 0; i < 100; i++){
							runner.x = Util.rand(1,performer.getLevel().getWidth()-1);
							runner.y = Util.rand(1,performer.getLevel().getHeight()-1);
							performer.getLevel().setSeen(runner.x, runner.y);
						}
					} 
				
			}else
			if (effect[cmd].equals("DAMAGE")) {
				if (aPlayer.isInvincible())
					aPlayer.getLevel().addMessage("The damage is repelled!");
				else
					aPlayer.selfDamage(Player.DAMAGE_USING_ITEM, Integer.parseInt(effect[cmd+1]));
			} else
			if (effect[cmd].equals("POISON")) {
				aPlayer.setCounter("POISON", 10);
			} else
			if (effect[cmd].equals("CURE")) {
				if (aPlayer.isPoisoned())
					aPlayer.setCounter("POISON", 0);
			}else 
			if (effect[cmd].equals("BLACK_GEM")){
				aPlayer.setFlag("BLACK_GEM", true);
			} else
			if (effect[cmd].equals("WHITE_GEM")){
				aPlayer.setFlag("WHITE_GEM", true);
			} else 
			if (effect[cmd].equals("AMBER_GEM")){
				aPlayer.setFlag("AMBER_GEM", true);
			}
		}
		if (def.isSingleUse())
			aPlayer.reduceQuantityOf(targetItem);


	}

	



}