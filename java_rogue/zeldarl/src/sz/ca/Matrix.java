package sz.ca;

import sz.util.Position;
import sz.util.Util;

public class Matrix {
	private int [][] values;
	private int [][] futureValues;

	public void setFuture (int value, int x, int y){
		/** Sets the future state of cell(x,y) to value */
		futureValues [x][y] = value;
	}

	public void setPresent (int value, int x, int y){
		/** Sets the future state of cell(x,y) to value */
		values [x][y] = value;
	}

	public int get(int x, int y){
		/** returns the current state of cell(x,y) */
		return values[x][y];
	}

	public int getWidth(){
		return values.length;
	}

	public int getHeight(){
		return values[0].length;
	}

	public void advance(){
		/** All future values become current*/
		for (int x = 0; x < values.length; x++){
			for (int y = 0; y < values[0].length; y++){
				values[x][y] = futureValues [x][y];
			}
		}
	}

	public int getSurroundingCount(int x, int y, int type){
		/** Returns the number of cells of type type that surround
		 * the matrix at x,y, wrapping from the sides */
		 int upIndex = (y == 0 ? getHeight()-1 : y-1);
		 int downIndex = (y == getHeight()-1  ? 0 : y+1);
		 int rightIndex = (x == getWidth()-1  ? 0 : x+1);
		 int leftIndex = (x == 0 ? getWidth()-1 : x-1);
		 int count =
		 		(values[leftIndex][upIndex] == type ? 1 : 0) +
		 		(values[leftIndex][y] == type ? 1 : 0) +
		 		(values[leftIndex][downIndex] == type ? 1 : 0) +
		 		(values[rightIndex][upIndex] == type ? 1 : 0) +
		 		(values[rightIndex][y] == type ? 1 : 0) +
		 		(values[rightIndex][downIndex] == type ? 1 : 0) +
		 		(values[x][downIndex] == type ? 1 : 0) +
		 		(values[x][upIndex] == type ? 1 : 0) ;
		 return count;
	}
	
	public int getSurroundingCountNoWrap(int x, int y, int type){
		/** Returns the number of cells of type type that surround
		 * the matrix at x,y*/
		 int count =
		 		(y==0||x==0?0:(values[x-1][y-1] == type ? 1 : 0)) +
		 		(x==0?0:(values[x-1][y] == type ? 1 : 0)) +
		 		(x==0||y==getHeight()-1?0:(values[x-1][y+1] == type ? 1 : 0)) +
		 		(y==0||x==getWidth()-1?0:(values[x+1][y-1] == type ? 1 : 0)) +
		 		(x==getWidth()-1?0:(values[x+1][y] == type ? 1 : 0)) +
		 		(x==getWidth()-1||y==getHeight()-1?0:(values[x+1][y+1] == type ? 1 : 0)) +
		 		(y==getHeight()-1?0:(values[x][y+1] == type ? 1 : 0)) +
		 		(y==0?0:(values[x][y-1] == type ? 1 : 0)) ;
		 return count;
	}


	public void clean(){
		/** Sets all the values and old Values to 0 */
		values = new int[values.length][values[0].length];
		futureValues = new int[values.length][values[0].length];
	}

	public Matrix(int xdim, int ydim){
		values = new int[xdim][ydim];
		futureValues = new int[xdim][ydim];
	}

	public Matrix(int dim){
		this(dim, dim);
	}
	
	public int[][] getArrays(){
		return values;
	}
	
	public void addHotSpot(int value){
		int xpos = Util.rand(5,getWidth()-5);
		int ypos = Util.rand(5,getHeight()-5);
		values[xpos][ypos] = value;
		futureValues[xpos][ypos] = value;
	}
	
	public void addHotSpot(Position where, int value){
		int xpos = where.x;
		int ypos = where.y;
		values[xpos][ypos] = value;
		futureValues[xpos][ypos] = value;
	}
	
	public void addShoweredHotSpot(int value, int shower, int showers, int maxDist){
		addShoweredHotSpot(new Position(Util.rand(5,getWidth()-5), Util.rand(5,getHeight()-5)), value, shower, showers, maxDist);		
	}
	
	public void addShoweredHotSpot(Position where, int value, int shower, int showers, int maxDist){
		int xpos = where.x;
		int ypos = where.y;
		int xs = showers;
		for (int i = 0; i < xs; i++){
			int xdif = maxDist - Util.rand(0,maxDist*2);
			int ydif = maxDist - Util.rand(0,maxDist*2);
			if (isValid(xpos+xdif, ypos+ydif))
				futureValues[xpos+xdif][ypos+ydif] = shower;
		}
		futureValues[xpos][ypos] = value;
	}
	
	private boolean isValid(int x,int y){
		return x>=0 && y >= 0 && x < getWidth() && y < getHeight();
	}
}
