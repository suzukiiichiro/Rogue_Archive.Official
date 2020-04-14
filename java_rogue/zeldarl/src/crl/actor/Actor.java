package crl.actor;

import java.util.Hashtable;

import sz.util.Debug;
import sz.util.Position;
import sz.util.PriorityEnqueable;
import zrl.player.Player;
import crl.action.Action;
import crl.ai.ActionSelector;
import crl.game.SFXManager;
import crl.level.Level;
import crl.ui.Appearance;

public class Actor implements Cloneable, java.io.Serializable, PriorityEnqueable{
	protected /*transient*/ int positionx, positiony, positionz;
	protected transient Appearance appearance;
	
	protected ActionSelector selector;
	private /*transient*/ Position position = new Position(0,0,0);
	private /*transient*/ int nextTime=10;
	
	public int getCost(){
		//Debug.say("Cost of "+getDescription()+" "+ nextTime);
		return nextTime;
	}
	
	public void reduceCost(int value){
		//Debug.say("Reducing cost of "+getDescription()+"by"+value+" (from "+nextTime+")");
		nextTime = nextTime - value;
	}
	
	public void setNextTime(int value){
		//Debug.say("Next time for "+getDescription()+" "+ value);
		nextTime = value;
	}

	protected Level level;

	public void updateStatus(){}

	public String getDescription(){
		return "";
	}

	public void execute(Action x){
		//System.out.println(getDescription()+" executing "+x);
		if (x != null){
        	x.setPerformer(this);
        	if (x.canPerform(this)){
	        	if (x.getSFX() != null)
	        		SFXManager.play(x.getSFX());
				x.execute();
				//Debug.say("("+x.getCost()+")");
				setNextTime(x.getCost());
        	}
        	updateStatus();
		} else {
			/*if (this instanceof Player) {
				setNextTime(0);
			} else {*/
				setNextTime(50);
				updateStatus();
			//}
		}
		
	}
	public void act(){
		Action x = getSelector().selectAction(this);
		execute(x);
	}

	public void setPosition (int x, int y, int z){
		position.x = x;
		position.y = y;
		position.z = z;
	}

	public void die(){
		/** Request to be removed from any dispatcher or structure */
		aWannaDie = true;
	}

	public boolean wannaDie(){
		return aWannaDie;
	}

	private boolean aWannaDie;


	public void setPosition (Position p){
		position = p;
	}

	public Position getPosition(){
		return position;
	}

	public void setLevel(Level what){
		level = what;
	}

	public Level getLevel(){
		return level;
	}

	public ActionSelector getSelector() {
		return selector;
	}

	public void setSelector(ActionSelector value) {
		selector = value;
	}

	public Appearance getAppearance() {
		return appearance;
	}

	public void setAppearance(Appearance value) {
		appearance = value;
	}

	public Object clone(){
		try {
			Actor x = (Actor) super.clone();
			if (position != null)
				x.setPosition(new Position(position.x, position.y, position.z));
			return x;
		} catch (CloneNotSupportedException cnse){
			Debug.doAssert(false, "failed class cast, Feature.clone()");
		}
		return null;
	}


	public void message(String mess){
	}

	protected Hashtable hashCounters = new Hashtable();
	public void setCounter(String counterID, int turns){
		hashCounters.put(counterID, new Integer(turns));
	}
	
	public int getCounter(String counterID){
		Integer val = (Integer)hashCounters.get(counterID);
		if (val == null)
			return -1;
		else
			return val.intValue();
	}
	
	public boolean hasCounter(String counterID){
		return getCounter(counterID) > 0;
	}
}