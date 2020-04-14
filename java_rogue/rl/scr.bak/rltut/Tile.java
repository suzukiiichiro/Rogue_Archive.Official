package rltut;

import java.awt.Color;
import asciiPanel.AsciiPanel;

public enum Tile {
	FLOOR((char)250, AsciiPanel.yellow, "A dirt and rock cave floor."),
	WALL((char)177, AsciiPanel.yellow, "A dirt and rock cave wall."),
	BOUNDS('x', AsciiPanel.brightBlack, "Beyond the edge of the world."),
	STAIRS_DOWN('>', AsciiPanel.white, "A stone staircase that goes down."),
	STAIRS_UP('<', AsciiPanel.white, "A stone staircase that goes up."),
	UNKNOWN(' ', AsciiPanel.white, "(unknown)");
	
	private char glyph;
	public char glyph() { return glyph; }
	
	private Color color;
	public Color color() { return color; }

	private String description;
	public String details(){ return description; }
	
	Tile(char glyph, Color color, String description){
		this.glyph = glyph;
		this.color = color;
		this.description = description;
	}

	public boolean isGround() {
		return this != WALL && this != BOUNDS;
	}

	public boolean isDiggable() {
		return this == Tile.WALL;
	}
}
