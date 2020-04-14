package sz.util;

import java.io.Serializable;

public class Counter implements Serializable{

	private int value;
	public Counter(int initialCoun){
		value = initialCoun;
	}

	public void reduce(){
		value--;
	}

	public boolean isOver(){
		return value < 0;
	}
}