/* display.c
 * 
 * Copyright 2005 Martin Read
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define DISPLAY_C
#include "dunbash.h"
#include "monsters.h"
#include <curses.h>
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

int wall_colour;
int you_colour;
int status_updated;
int map_updated;
int show_terrain;
int hard_redraw;

chtype colour_attrs[15] =
{
    0,
    COLOR_PAIR(DBCLR_D_GREY) | A_BOLD,
    COLOR_PAIR(DBCLR_RED),
    COLOR_PAIR(DBCLR_BLUE),
    COLOR_PAIR(DBCLR_GREEN),
    COLOR_PAIR(DBCLR_PURPLE),
    COLOR_PAIR(DBCLR_BROWN),
    COLOR_PAIR(DBCLR_CYAN),
    A_BOLD,
    COLOR_PAIR(DBCLR_RED) | A_BOLD,
    COLOR_PAIR(DBCLR_BLUE) | A_BOLD,
    COLOR_PAIR(DBCLR_GREEN) | A_BOLD,
    COLOR_PAIR(DBCLR_PURPLE) | A_BOLD,
    COLOR_PAIR(DBCLR_BROWN) | A_BOLD,
    COLOR_PAIR(DBCLR_CYAN) | A_BOLD
};

#define DISP_HEIGHT 21
#define DISP_WIDTH 21

chtype back_buffer[DUN_HEIGHT][DUN_WIDTH];
chtype front_buffer[DISP_HEIGHT][DISP_WIDTH];

/* Prototypes for static funcs */
static chtype object_char(int object_id);
static chtype monster_char(int monster_id);
static chtype terrain_char(enum terrain_num terrain_type);
static void draw_status_line(void);
static void draw_world(void);

/* Static funcs */
static void draw_status_line(void)
{
    mvwprintw(status_window, 0, 0, "%-16.16s", u.name);
    mvwprintw(status_window, 0, 17, "HP: %03d/%03d", u.hpcur, u.hpmax);
    mvwprintw(status_window, 0, 30, "XL: %d", u.level);
    mvwprintw(status_window, 0, 47, "Body: %02d/%02d", u.body - u.bdam, u.body);
    mvwprintw(status_window, 0, 62, "Gold: %d", u.gold);
    mvwprintw(status_window, 1, 0, "Defence: %02d", u.defence);
    mvwprintw(status_window, 1, 15, "Food: %06d", u.food);
    mvwprintw(status_window, 1, 30, "Depth: %d", depth);
    mvwprintw(status_window, 1, 62, "XP: %d", u.experience);
    mvwprintw(status_window, 1, 44, "Agility: %02d/%02d", u.agility - u.adam, u.agility);
}

static chtype terrain_char(enum terrain_num terrain_type)
{
    switch (terrain_type)
    {
    case STAIRS:
	return '>';
    case FLOOR:
	return '.';
    case WALL:
	return '#' | colour_attrs[wall_colour];
    case DOOR:
	return '+';
    default:
	return '*';
    }
}

static chtype monster_char(int monster_id)
{
    return (permons[monster_id].sym) | colour_attrs[permons[monster_id].colour];
}

static chtype object_char(int object_id)
{
    return permobjs[object_id].sym;
}

void touch_back_buffer(void)
{
    int y;
    int x;
    for (y = 0; y < DUN_HEIGHT; y++)
    {
	for (x = 0; x < DUN_WIDTH; x++)
	{
	    newsym(y, x);
	}
    }
    map_updated = 1;
    hard_redraw = 1;
}

void newsym(int y, int x)
{
    chtype ch;

    ch = back_buffer[y][x];
    if ((y == u.y) && (x == u.x))
    {
	back_buffer[y][x] = '@' | colour_attrs[you_colour];
    }
    else if (!show_terrain && (mapmonster[y][x] != -1) && mon_visible(mapmonster[y][x]))
    {
	back_buffer[y][x] = monster_char(monsters[mapmonster[y][x]].mon_id);
    }
    else if (mapflags[y][x] & MAPFLAG_EXPLORED)
    {
	if (!show_terrain && (mapobject[y][x] != -1))
	{
	    back_buffer[y][x] = object_char(objects[mapobject[y][x]].obj_id);
	}
	else
	{
	    back_buffer[y][x] = terrain_char(terrain[y][x]);
	}
    }
    else
    {
	back_buffer[y][x] = ' ';
    }
    if (ch != back_buffer[y][x])
    {
	map_updated = 1;
    }
}

static void draw_world(void)
{
    int i;
    int j;
    int x;
    int y;

    for (i = 0; i < 21; i++)
    {
	y = u.y + i - 10;
	for (j = 0; j < 21; j++)
	{
	    x = u.x + j - 10;
	    if ((y < 0) || (x < 0) ||
		(y >= DUN_HEIGHT) || (x >= DUN_WIDTH))
	    {
		if ((front_buffer[i][j] != ' ') || hard_redraw)
		{
		    mvwaddch(world_window, i, j, ' ');
		}
		front_buffer[i][j] = ' ';
	    }
	    else if (hard_redraw || (front_buffer[i][j] != back_buffer[y][x]))
	    {
		mvwaddch(world_window, i, j, back_buffer[y][x]);
		front_buffer[i][j] = back_buffer[y][x];
	    }
	}
    }
}

/* extern funcs */

void press_enter(void)
{
    int ch;
    print_msg("Press RETURN or SPACE to continue\n");
    while (1)
    {
	ch = wgetch(message_window);
	if ((ch == ' ') || (ch == '\n') || (ch == '\r'))
	{
	    break;
	}
    }
}

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
    start_color();
    init_pair(DBCLR_BROWN, COLOR_YELLOW, COLOR_BLACK);
    init_pair(DBCLR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(DBCLR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(DBCLR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(DBCLR_D_GREY, COLOR_BLACK, COLOR_BLACK);
    init_pair(DBCLR_PURPLE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(DBCLR_CYAN, COLOR_CYAN, COLOR_BLACK);
    wall_colour = DBCLR_BROWN;
    you_colour = DBCLR_WHITE;
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
    idcok(status_window, FALSE);
    idcok(world_window, FALSE);
    idcok(message_window, FALSE);
    mvwprintw(world_window, 6, 5, "  Martin's");
    mvwprintw(world_window, 7, 5, "Dungeon Bash");
    mvwprintw(world_window, 9, 5, "Version %d.%d", MAJVERS, MINVERS);
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

void show_discoveries(void)
{
    int i, j;
    print_msg("You recognise the following items:\n");
    for (i = 0, j = 1; i < PO_REAL_COUNT; i++)
    {
	if (permobjs[i].known)
	{
	    print_msg("%s\n", permobjs[i].name);
	    j++;
	}
	if (j == 19)
	{
	    press_enter();
	    j = 0;
	}
    }
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

int inv_select(enum poclass_num filter, const char *action, int accept_blank)
{
    int selection;
    int ch;
    int i;
    int items = 0;
    for (i = 0; i < 19; i++)
    {
	if ((u.inventory[i] != -1) && ((filter == POCLASS_NONE) || (permobjs[objects[u.inventory[i]].obj_id].poclass == filter)))
	{
	    items++;
	}
    }
    if (items == 0)
    {
	print_msg("You have nothing to %s.\n", action);
	return -1;
    }
    print_msg("Items available to %s\n", action);
    print_inv(filter);
    if (accept_blank)
    {
	print_msg("-) no item\n");
    }
    print_msg("[ESC or SPACE to cancel]\n");
tryagain:
    print_msg("What do you want to %s? ", action);
    ch = wgetch(message_window);
    switch (ch)
    {
    case '-':
	if (accept_blank)
	{
	    print_msg("\n");
	    return -2;
	}
    case 'x':
    case '\x1b':
    case ' ':
	print_msg("\nNever mind.\n");
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
	/* I am assuming that we're in a place where the character
	 * set is a strict superset of ASCII. IF we're not, the
	 * following code may break. */
	print_msg("\n");
	selection = ch - 'a';
	if ((u.inventory[selection] != -1) && ((filter == POCLASS_NONE) || (permobjs[objects[u.inventory[selection]].obj_id].poclass == filter)))
	{
	    return selection;
	}
	/* Fall through */
    default:
	print_msg("\nBad selection\n");
	goto tryagain;
    }
}

int select_dir(int *psy, int *psx)
{
    int ch;
    int done = 0;
    print_msg("Select a direction with movement keys.\n[ESC or space to cancel].\n");
    while (!done)
    {
	ch = wgetch(message_window);
	switch (ch)
	{
	case 'h':
	case '4':
	    *psx = -1;
	    *psy = 0;
	    done = 1;
	    break;
	case 'j':
	case '2':
	    *psx = 0;
	    *psy = 1;
	    done = 1;
	    break;
	case 'k':
	case '8':
	    *psx = 0;
	    *psy = -1;
	    done = 1;
	    break;
	case 'l':
	case '6':
	    *psx = 1;
	    *psy = 0;
	    done = 1;
	    break;
	case 'y':
	case '7':
	    *psx = -1;
	    *psy = -1;
	    done = 1;
	    break;
	case 'u':
	case '9':
	    *psx = 1;
	    *psy = -1;
	    done = 1;
	    break;
	case 'b':
	case '1':
	    *psx = -1;
	    *psy = 1;
	    done = 1;
	    break;
	case 'n':
	case '3':
	    *psx = 1;
	    *psy = 1;
	    done = 1;
	    break;
	case '\x1b':
	case ' ':
	    return -1;	/* cancelled. */
	default:
	    print_msg("Bad direction (use movement keys).\n");
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
	ch = wgetch(message_window);
	switch (ch)
	{
	case 'a':
	    return ATTACK;
	case '0':
	case ',':
	case 'g':
	    return GET_ITEM;
	case 'd':
	    return DROP_ITEM;
	case 'D':
	    return DUMP_CHARA;
	case 'S':
	    return SAVE_GAME;
	case 'X':
	    return QUIT;
	case 'i':
	    return SHOW_INVENTORY;
	case 'I':
	    return INSPECT_ITEM;
	case ';':
	    return EXAMINE_MONSTER;
	case '#':
	    return SHOW_TERRAIN;
	case '\\':
	    return SHOW_DISCOVERIES;
	case '\x12':
	    return RNG_TEST;
	case '4':
	case 'h':
	    return MOVE_WEST;
	case '2':
	case 'j':
	    return MOVE_SOUTH;
	case '8':
	case 'k':
	    return MOVE_NORTH;
	case '6':
	case 'l':
	    return MOVE_EAST;
	case '7':
	case 'y':
	    return MOVE_NW;
	case '9':
	case 'u':
	    return MOVE_NE;
	case '1':
	case 'b':
	    return MOVE_SW;
	case '3':
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
	case 'e':
	    return EAT_FOOD;
	case '5':
	case '.':
	    return STAND_STILL;
	case '\x04':
	    return WIZARD_DESCEND;
	case '\x05':
	    return WIZARD_LEVELUP;
	}
    }
    return 0;
}

int display_shutdown(void)
{
    display_update();
    press_enter();
    clear();
    refresh();
    endwin();
    return 0;
}

void pressanykey(void)
{
    print_msg("Press any key to continue.\n");
    wgetch(message_window);
}

int getYN(const char *msg)
{
    int ch;
    print_msg("%s", msg);
    print_msg("Press capital Y to confirm, any other key to cancel\n");
    ch = wgetch(message_window);
    if (ch == 'Y')
    {
	return 1;
    }
    return 0;
}

int getyn(const char *msg)
{
    int ch;
    print_msg("%s", msg);
    while (1)
    {
	ch = wgetch(message_window);
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
    print_msg("y  k  u  7  8  9\n");
    print_msg(" \\ | /    \\ | / \n");
    print_msg("  \\|/      \\|/  \n");
    print_msg("h--*--l  4--*--6\n");
    print_msg("  /|\\      /|\\  \n");
    print_msg(" / | \\    / | \\ \n");
    print_msg("b  j  n  1  2  3\n");
    print_msg("Attack monsters in melee by bumping into them.\n");
    print_msg("Doors do not have to be opened before you go through.\n");
    print_msg("Turn on NUM LOCK to use the numeric keypad for movement.\n");
    print_msg("\nPress any key to continue...\n");
    wgetch(message_window);
    print_msg("ACTIONS\n");
    print_msg("a   make an attack (used to fire bows)\n");
    print_msg("P   put on a ring\n");
    print_msg("R   remove a ring\n");
    print_msg("W   wear armour\n");
    print_msg("T   take off armour\n");
    print_msg("r   read a scroll\n");
    print_msg("w   wield a weapon\n");
    print_msg("q   quaff a potion\n");
    print_msg("g   pick up an item (also 0 or comma)\n");
    print_msg("d   drop an item\n");
    print_msg("e   eat something edible\n");
    print_msg(">   go down stairs\n");
    print_msg("5   do nothing (wait until next action)\n");
    print_msg(".   do nothing (wait until next action)\n");
    print_msg("\nPress any key to continue...\n");
    wgetch(message_window);
    print_msg("OTHER COMMANDS\n");
    print_msg("S   save and exit\n");
    print_msg("X   quit without saving\n");
    print_msg("i   print your inventory\n");
    print_msg("I   examine an item you are carrying\n");
    print_msg("#   show underlying terrain of occupied squares\n");
    print_msg("\\   list all recognised items\n");
    print_msg("D   dump your character's details to <name>.dump\n");
    print_msg("?   print this message\n");
    print_msg("\nPress any key to continue...\n");
    wgetch(message_window);
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
    print_msg("%%   some food\n");
    print_msg("&   a demon\n");
    print_msg("\nMost other monsters are shown as letters.\n");
    print_msg("\nThis is all the help you get. Good luck!\n");
}

/* display.c */
// あ
