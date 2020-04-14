package sz.util;

public class Area extends Position{
	private Position dimension;
	public Area (Position location, Position dimension){
		super(location);
		this.dimension = dimension;
	}
	public Position getDimension() {
		return dimension;
	}
	
	public boolean overlaps(Area o){
		for (int xrun = x; xrun < x+dimension.x; xrun++)
			for (int yrun = y; yrun < y+dimension.y; yrun++)
				if (o.contains(xrun, yrun))
					return true;
		return false;
	}
	public boolean contains(int xd, int yd){
		if (xd >= x && xd <= x+dimension.x)
			if (yd >= y && yd <= y+dimension.y)
				return true;
		return false;
	}
}
