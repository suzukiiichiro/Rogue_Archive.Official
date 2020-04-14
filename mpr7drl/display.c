/* display.c */

#define DISPLAY_C
#include "7drl.h"
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <panel.h>
#include <stdarg.h>
#include <string.h>

WINDOW *status_window;
WINDOW *world_window;
WINDOW *message_window;
PANEL *status_panel;
PANEL *world_panel;
PANEL *message_panel;

int status_updated;
int map_updated;

/* Prototypes for static funcs */
static int object_char(int object_id);
static int monster_char(int monster_id);
static int terrain_char(enum terrain_num terrain_type);
static void draw_status_line(void);
static void draw_world(void);

/* Static funcs */
static void draw_status_line(void)
{
	mvwprintw(status_window, 0, 0, "%-16.16s", u.name);
	mvwprintw(status_window, 0, 17, "HP: %03d/%03d", u.hpcur, u.hpmax);
	mvwprintw(status_window, 0, 30, "XL: %d", u.level);
	mvwprintw(status_window, 0, 50, "Body: %02d", u.body);
	mvwprintw(status_window, 0, 62, "Gold: %d", u.gold);
	mvwprintw(status_window, 1, 0, "Defence: %02d", u.defence);
	mvwprintw(status_window, 1, 30, "Depth: %d", depth);
	mvwprintw(status_window, 1, 62, "XP: %d", u.experience);
	mvwprintw(status_window, 1, 47, "Agility: %02d", u.body);
}

static int terrain_char(enum terrain_num terrain_type)
{
	switch (terrain_type)
	{
		case STAIRS:
			return '>';
		case FLOOR:
			return '.';
		case WALL:
			return '#';
		case DOOR:
			return '+';
		default:
			return '*';
	}
}

static int monster_char(int monster_id)
{
	return permons[monster_id].sym;
}

static int object_char(int object_id)
{
	return permobjs[object_id].sym;
}

static void draw_world(void)
{
	int i;
	int j;
	int x;
	int y;
	for (i = -10; i < 11; i++)
	{
		y = u.y + i;
		for (j = -10; j < 11; j++)
		{
			x = u.x + j;
			if ((i == 0) && (j == 0))
			{
				mvwaddch(world_window, 10, 10, '@');
			}
			else if ((y < 0) || (x < 0) ||
			    (y >= DUN_SIZE) || (x >= DUN_SIZE))
			{
				mvwaddch(world_window, i + 10, j + 10, ' ');
			}
			else if ((i < 2) && (i > -2) && (j < 2) && (j > -2) && (mapmonster[y][x] != -1))
			{
				mvwaddch(world_window, i + 10, j + 10, monster_char(monsters[mapmonster[y][x]].mon_id));
			}
			else if ((roomnums[u.y][u.x] != -1) && (roomnums[u.y][u.x] == roomnums[y][x]) && (mapmonster[y][x] != -1))
			{
				mvwaddch(world_window, i + 10, j + 10, monster_char(monsters[mapmonster[y][x]].mon_id));
			}
			else if (mapflags[y][x] & MAPFLAG_EXPLORED)
			{
				if (mapobject[y][x] != -1)
				{
					mvwaddch(world_window, i + 10, j + 10, object_char(objects[mapobject[y][x]].obj_id));
				}
				else
				{
					mvwaddch(world_window, i + 10, j + 10, terrain_char(terrain[y][x]));
				}
			}
			else
			{
				mvwaddch(world_window, i + 10, j + 10, ' ');
			}
		}
	}
}

/* extern funcs */
void display_update(void)
{
	if (status_updated)
	{
		status_updated = 0;
		draw_status_line();
	}
	if (map_updated)
	{
		map_updated = 0;
		draw_world();
	}
	update_panels();
	doupdate();
}

int display_init(void)
{
	initscr();
	noecho();
	cbreak();
	/* OK. We want a 21x21 viewport (player at centre), a 21x58 message
	 * window, and a 2x80 status line. */
	status_window = newwin(2, 80, 22, 0);
	status_panel = new_panel(status_window);
	world_window = newwin(21, 21, 0, 0);
	world_panel = new_panel(world_window);
	message_window = newwin(21, 58, 0, 22);
	message_panel = new_panel(message_window);
	wclear(status_window);
	wclear(world_window);
	wclear(message_window);
	scrollok(status_window, FALSE);
	scrollok(world_window, FALSE);
	scrollok(message_window, TRUE);
	mvwprintw(world_window, 1, 1, "Martin's 7-Day");
	mvwprintw(world_window, 2, 1, "Rogue-Like Game");
	wmove(message_window, 0, 0);
	map_updated = FALSE;
	status_updated = FALSE;
	update_panels();
	doupdate();
	return 0;
}

int read_input(char *buffer, int length)
{
	echo();
	display_update();
	buffer[0] = '\0';
	wgetnstr(message_window, buffer, length);
	noecho();
	return strlen(buffer);
}

void print_msg(const char *fmt, ...)
{
	va_list ap;
	/* For now, assume (1) that the player will never be so inundated
	 * with messages that it's dangerous to let them just fly past (2)
	 * that messages will be of sane length and nicely formatted. THIS
	 * IS VERY BAD CODING PRACTICE! */
	/* Note that every message forces a call to display_update().
	 * Events that cause changes to the map or the player should flag
	 * the change before calling printmsg. */
	va_start(ap, fmt);
	vw_printw(message_window, fmt, ap);
	va_end(ap);
	display_update();
}

void print_inv(enum poclass_num filter)
{
	int i;
	for (i = 0; i < 19; i++)
	{
		if ((u.inventory[i] != -1) && ((filter == POCLASS_NONE) || (permobjs[objects[u.inventory[i]].obj_id].poclass == filter)))
		{
			print_msg("%c) ", 'a' + i);
			print_obj_name(u.inventory[i]);
			if (u.ring == u.inventory[i])
			{
				print_msg(" (on finger)");
			}
			else if (u.weapon == u.inventory[i])
			{
				print_msg(" (in hand)");
			}
			else if (u.armour == u.inventory[i])
			{
				print_msg(" (being worn)");
			}
			print_msg("\n");
		}
	}
}

int inv_select(enum poclass_num filter, const char *action)
{
	int selection;
	int ch;
	print_msg("Items available to %s\n", action);
	print_inv(filter);
tryagain:
	print_msg("What do you want to %s?\n", action);
	ch = getch();
	switch (ch)
	{
	case 'x':
	case '\x1b':
	case ' ':
		print_msg("Never mind.\n");
		return -1;
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
		selection = ch - 'a';
		if (permobjs[objects[u.inventory[selection]].obj_id].poclass == filter)
		{
			return selection;
		}
		break;
	default:
		print_msg("Bad selection\n");
		goto tryagain;
	}
	return selection;
}

int select_dir(int *psy, int *psx)
{
	int ch;
	int done = 0;
	print_msg("Select a direction [ESC or space to cancel].\n");
	while (!done)
	{
		ch = getch();
		switch (ch)
		{
		case 'h':
			*psx = -1;
			*psy = 0;
			done = 1;
			break;
		case 'j':
			*psx = 0;
			*psy = 1;
			done = 1;
			break;
		case 'k':
			*psx = 0;
			*psy = -1;
			done = 1;
			break;
		case 'l':
			*psx = 1;
			*psy = 0;
			done = 1;
			break;
		case 'y':
			*psx = -1;
			*psy = -1;
			done = 1;
			break;
		case 'u':
			*psx = 1;
			*psy = -1;
			done = 1;
			break;
		case 'b':
			*psx = -1;
			*psy = 1;
			done = 1;
			break;
		case 'n':
			*psx = 1;
			*psy = 1;
			done = 1;
			break;
		case '\x1b':
		case ' ':
			return -1;	/* cancelled. */
		default:
			print_msg("Bad direction (choices are hjklyubn).\n");
			print_msg("[Press ESC or space to cancel.]\n");
			break;
		}
	}
	return 0;
}

enum game_cmd get_command(void)
{
	int ch;
	int done = 0;
	while (!done)
	{
		ch = getch();
		switch (ch)
		{
		case 'a':
			return ATTACK;
		case ',':
		case 'g':
			return GET_ITEM;
		case 'd':
			return DROP_ITEM;
		case 'S':
			return SAVE_GAME;
		case 'X':
			return QUIT;
		case 'i':
			return SHOW_INVENTORY;
		case 'h':
			return MOVE_WEST;
		case 'j':
			return MOVE_SOUTH;
		case 'k':
			return MOVE_NORTH;
		case 'l':
			return MOVE_EAST;
		case 'y':
			return MOVE_NW;
		case 'u':
			return MOVE_NE;
		case 'b':
			return MOVE_SW;
		case 'n':
			return MOVE_SE;
		case 'q':
			return QUAFF_POTION;
		case 'r':
			return READ_SCROLL;
		case 'w':
			return WIELD_WEAPON;
		case 'W':
			return WEAR_ARMOUR;
		case 'T':
			return TAKE_OFF_ARMOUR;
		case 'P':
			return PUT_ON_RING;
		case 'R':
			return REMOVE_RING;
		case '?':
			return GIVE_HELP;
		case '>':
			return GO_DOWN_STAIRS;
		case '.':
			return STAND_STILL;
		}
	}
	return 0;
}

int display_shutdown(void)
{
	display_update();
	print_msg("Press any key to exit.\n");
	getch();
	clear();
	refresh();
	endwin();
	return 0;
}

int getyn(const char *msg)
{
	int ch;
	print_msg("%s", msg);
	while (1)
	{
		ch = getch();
		switch (ch)
		{
		case 'y':
		case 'Y':
			return 1;
		case 'n':
		case 'N':
			return 0;
		case '\x1b':
		case ' ':
			return -1;
		default:
			print_msg("Invalid response. Press y or n (ESC or space to cancel)\n");
		}
	}
}

void print_help(void)
{
	print_msg("MOVEMENT\n");
	print_msg("y  k  u\n");
	print_msg(" \\ | /\n");
	print_msg("  \\|/ \n");
	print_msg("h--*--l\n");
	print_msg("  /|\\ \n");
	print_msg(" / | \\\n");
	print_msg("b  j  n\n");
	print_msg("Attack monsters in melee by bumping into them.\n");
	print_msg("Doors do not have to be opened before you go through.\n");
	print_msg("\nPress any key to continue...\n");
	getch();
	print_msg("ACTIONS\n");
	print_msg("a   make an attack (used to fire bows)\n");
	print_msg("P   put on a ring\n");
	print_msg("R   remove a ring\n");
	print_msg("W   wear armour\n");
	print_msg("T   take off armour\n");
	print_msg("r   read a scroll\n");
	print_msg("w   wield a weapon\n");
	print_msg("q   quaff a potion\n");
	print_msg("g   pick up an item\n");
	print_msg("d   drop an item\n");
	print_msg(">   go down stairs\n");
	print_msg(".   do nothing (wait until next action)\n");
	print_msg("\nPress any key to continue...\n");
	getch();
	print_msg("OTHER COMMANDS\n");
	print_msg("S   save and exit\n");
	print_msg("X   quit without saving\n");
	print_msg("i   print your inventory\n");
	print_msg("?   print this message\n");
	print_msg("\nPress any key to continue...\n");
	getch();
	print_msg("SYMBOLS\n");
	print_msg("@   you\n");
	print_msg(".   floor\n");
	print_msg(">   stairs down\n");
	print_msg("#   wall\n");
	print_msg("+   a door\n");
	print_msg(")   a weapon\n");
	print_msg("(   a missile weapon\n");
	print_msg("[   a suit of armour\n");
	print_msg("=   a ring\n");
	print_msg("?   a scroll\n");
	print_msg("!   a potion\n");
	print_msg("&   a demon\n");
	print_msg("\nMost other monsters are shown as letters.\n");
	print_msg("\nThis is all the help you get. Good luck!\n");
}

/* display.c */
