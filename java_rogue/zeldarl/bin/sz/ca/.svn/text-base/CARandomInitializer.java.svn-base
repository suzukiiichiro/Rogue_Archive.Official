package sz.ca;

import sz.util.*;

public class CARandomInitializer {
	private double [] proportions;
	private int border;
	public CARandomInitializer(double[] proportions, int border){
		this.proportions = proportions;
		this.border = border;
	}
	
	public void init(Matrix map){
		boolean wasVisited[][] = new boolean[map.getWidth()][map.getHeight()];
		/*for (int x = 0; x < map.getWidth(); x++)
			for (int y = 0; y < map.getHeight(); y++)
				map.setFuture(0, x, y);

		if (border){
			for (int x = 0; x < map.getWidth(); x++){
				map.setFuture(1,x,0);
				map.setFuture(1,x,map.getHeight()-1);
			}
			for (int y = 0; y < map.getHeight(); y++){
				map.setFuture(1, 0, y);
				map.setFuture(1, map.getWidth()-1, y);
			}
		}
		
		int cellCount = map.getHeight() * map.getWidth();
		for (int i = 0; i < proportions.length; i++){
			for (int j = 0; j < (int) (proportions[i] * cellCount); j++){
				int xgo = Util.rand(0, map.getWidth()-1);
				int ygo = Util.rand(0, map.getHeight()-1);
				if (map.get(xgo, ygo) == 0)
					map.setFuture(i+1, xgo, ygo);
				else
					j--;
			}
		}
		map.advance();*/
		for (int x = 0; x < map.getWidth(); x++)
			for (int y = 0; y < map.getHeight(); y++)
				map.setFuture(0, x, y);
		if (border != 0){
			for (int x = 0; x < map.getWidth(); x++){
				map.setFuture(border,x,0);
				map.setFuture(border,x,map.getHeight()-1);
			}
			for (int y = 0; y < map.getHeight(); y++){
				map.setFuture(border, 0, y);
				map.setFuture(border, map.getWidth()-1, y);
			}
		}
		
		int cellCount = map.getHeight() * map.getWidth();
		for (int i = 1; i < proportions.length; i++){
			for (int j = 0; j < (int) (proportions[i] * cellCount); j++){
				int xgo = Util.rand(0, map.getWidth()-1);
				int ygo = Util.rand(0, map.getHeight()-1);
				if (!wasVisited[xgo][ygo]){
					map.setFuture(i, xgo, ygo);
					wasVisited[xgo][ygo] = true;
				}
				else
					j--;
			}
		}
		map.advance();
	}
}
