/* move.c */

#ifndef __CYGWIN__
#include <ncurses.h>
#else
#include <ncurses/ncurses.h>
#endif
#include <string.h>
#include "rogue.h"
#include "hit.h"
#include "inventory.h"
#include "keys.h"
#include "message.h"
#include "monster.h"
#include "move.h"
#include "object.h"
#include "pack.h"
#include "random.h"
#include "room.h"
#include "score.h"
#include "trap.h"
#include "use.h"

short m_moves = 0;
boolean jump = 1;
char *you_can_move_again = "You can move again.";

extern int cur_room;
extern short blind, levitate;
extern int cur_level;
extern short max_level;
extern int bear_trap;
extern short halluc, haste_self, confused;
extern short e_rings, regeneration, auto_search;
extern char hunger_str[];
extern boolean being_held, interrupted, r_teleport, multimove;
extern fighter rogue;
extern unsigned short dungeon[DROWS][DCOLS];
extern object level_monsters;
extern object level_objects;

short gr_dir(void);
int next_to_something(int, int);
void heal(void);

int move_rogue(short dirch, short pickup)
{
	if (multimove)
	{
		multiple_move_rogue(dirch-('a'-1));
		multimove = 0;
		return 0;
	} else {
		return one_move_rogue(dirch, pickup);
	}
}

int one_move_rogue(short dirch, short pickup)
{
	short row, col;
	object *obj;
	char desc[DCOLS];
	short n, status;

	row = rogue.row;
	col = rogue.col;

	if (confused)
	{
		dirch = gr_dir();
	}
	get_dir_rc(dirch, &row, &col, 1);

	if (!can_move(rogue.row, rogue.col, row, col))
	{
		return(MOVE_FAILED);
	}
	if (being_held || bear_trap)
	{
		if (!(dungeon[row][col] & MONSTER))
		{
			if (being_held)
			{
				message("You are being held.", 1);
			}
			else
			{
				message("You are still stuck in the bear trap.", 0);
				(void) reg_move();
			}
			return(MOVE_FAILED);
		}
	}
	if (r_teleport)
	{
		if (rand_percent(R_TELE_PERCENT))
		{
			tele();
			return(STOPPED_ON_SOMETHING);
		}
	}
	if (dungeon[row][col] & MONSTER)
	{
		rogue_hit(object_at(&level_monsters, row, col), 0);
		(void) reg_move();
		return(MOVE_FAILED);
	}
	if (dungeon[row][col] & DOOR) {
		if (cur_room == PASSAGE)
		{
			cur_room = get_room_number(row, col);
			light_up_room(cur_room);
			wake_room(cur_room, 1, row, col);
		}
		else
		{
			light_passage(row, col);
		}
	} else if ((dungeon[rogue.row][rogue.col] & DOOR)
	           && (dungeon[row][col] & TUNNEL)) {
		light_passage(row, col);
		wake_room(cur_room, 0, rogue.row, rogue.col);
		darken_room(cur_room);
		cur_room = PASSAGE;
	} else if (dungeon[row][col] & TUNNEL) {
			light_passage(row, col);
	}
	mvaddch(rogue.row, rogue.col, get_dungeon_char(rogue.row, rogue.col));
	mvaddch(row, col, rogue.fchar);

	if (!jump)
	{
		refresh();
	}
	rogue.row = row;
	rogue.col = col;
	if (dungeon[row][col] & OBJECT)
	{
		if (levitate && pickup)
		{
			return(STOPPED_ON_SOMETHING);
		}
		if (pickup && !levitate)
		{
			if ((obj = pick_up(row, col, &status))) {
				get_desc(obj, desc);
				if (obj->what_is == GOLD)
				{
					free_object(obj);
					goto NOT_IN_PACK;
				}
			} else if (!status) {
				goto MVED;
			} else {
				goto MOVE_ON;
			}
		}
		else
		{
MOVE_ON:
			obj = object_at(&level_objects, row, col);
			(void) strcpy(desc, "moved onto ");
			get_desc(obj, desc+11);
			goto NOT_IN_PACK;
		}
		n = strlen(desc);
		desc[n] = '(';
		desc[n+1] = obj->ichar;
		desc[n+2] = ')';
		desc[n+3] = 0;
NOT_IN_PACK:
		message(desc, 1);
		(void) reg_move();
		return(STOPPED_ON_SOMETHING);
	}
	if (dungeon[row][col] & (DOOR | STAIRS | TRAP))
	{
		if ((!levitate) && (dungeon[row][col] & TRAP))
		{
			trap_player(row, col);
		}
		(void) reg_move();
		return(STOPPED_ON_SOMETHING);
	}
MVED:	if (reg_move())
	{			/* fainted from hunger */
			return(STOPPED_ON_SOMETHING);
	}
	return((confused ? STOPPED_ON_SOMETHING : MOVED));
}

void multiple_move_rogue(int dirch)
{
	short row, col;
	short m;
	char tmpdirch;

	switch(dirch)
	{
	case ROGUE_KEY_WEST_CTRL:
	case ROGUE_KEY_SOUTH_CTRL:
	case ROGUE_KEY_NORTH_CTRL:
	case ROGUE_KEY_EAST_CTRL:
	case ROGUE_KEY_NORTHWEST_CTRL:
	case ROGUE_KEY_NORTHEAST_CTRL:
	case ROGUE_KEY_SOUTHEAST_CTRL:
	case ROGUE_KEY_SOUTHWEST_CTRL:
		tmpdirch = dirch + 96;
		do
		{
again:
			row = rogue.row;
			col = rogue.col;
			m = one_move_rogue(tmpdirch, 1);
			if (m == MOVE_FAILED)
			{
				int havemove = 0;
				int dir1, dir2;
				if (dungeon[row][col] & TUNNEL)
				{
					switch (tmpdirch)
					{
					case ROGUE_KEY_WEST:
					case ROGUE_KEY_EAST:
						dir1 = !!is_passable(row - 1, col);
						dir2 = !!is_passable(row + 1, col);
						if ((dir1 + dir2) == 1)
						{
							havemove = 1;
							tmpdirch = dir1 ?  'k' : 'j';
						}
						break;
					case ROGUE_KEY_NORTH:
					case ROGUE_KEY_SOUTH:
						dir1 = !!is_passable(row, col - 1);
						dir2 = !!is_passable(row, col + 1);
						if ((dir1 + dir2) == 1)
						{
							havemove = 1;
							tmpdirch = dir1 ?  'h' : 'l';
						}
						break;
					}
				}
				if (!havemove)
					break;
				else
					goto again;
			}
			if ((m == STOPPED_ON_SOMETHING) || interrupted)
			{
				break;
			}
		} while (!next_to_something(row, col));
		break;
	case ROGUE_KEY_WEST_SHIFT:
	case ROGUE_KEY_SOUTH_SHIFT:
	case ROGUE_KEY_NORTH_SHIFT:
	case ROGUE_KEY_EAST_SHIFT:
	case ROGUE_KEY_NORTHWEST_SHIFT:
	case ROGUE_KEY_NORTHEAST_SHIFT:
	case ROGUE_KEY_SOUTHEAST_SHIFT:
	case ROGUE_KEY_SOUTHWEST_SHIFT:
		while (	(!interrupted) &&
				(one_move_rogue((dirch + 32), 1) == MOVED)) ;
		break;
	}
}

int is_passable(int row, int col)
{
	if ((row < MIN_ROW) || (row > (DROWS - 2)) || (col < 0)
	    || (col > (DCOLS-1)))
	{
		return(0);
	}
	if (dungeon[row][col] & HIDDEN)
	{
		return((dungeon[row][col] & TRAP) ? 1 : 0);
	}
	return(dungeon[row][col] & (FLOOR | TUNNEL | DOOR | STAIRS | TRAP));
}

int next_to_something(int drow, int dcol)
{
	short i, j, i_end, j_end, row, col;
	short pass_count = 0;
	unsigned short s;

	if (confused)
	{
		return(1);
	}
	if (blind)
	{
		return(0);
	}
	i_end = (rogue.row < (DROWS-2)) ? 1 : 0;
	j_end = (rogue.col < (DCOLS-1)) ? 1 : 0;

	for (i = ((rogue.row > MIN_ROW) ? -1 : 0); i <= i_end; i++)
	{
		for (j = ((rogue.col > 0) ? -1 : 0); j <= j_end; j++)
		{
			if ((i == 0) && (j == 0))
			{
				continue;
			}
			if (((rogue.row+i) == drow) && ((rogue.col+j) == dcol))
			{
				continue;
			}
			row = rogue.row + i;
			col = rogue.col + j;
			s = dungeon[row][col];
			if (s & HIDDEN)
			{
				continue;
			}
			/* If the rogue used to be right, up, left, down, or right of
			 * row,col, and now isn't, then don't stop */
			if (s & (MONSTER | OBJECT | STAIRS))
			{
				if (((row == drow) || (col == dcol))
				    && (!((row == rogue.row)
				    || (col == rogue.col))))
				{
					continue;
				}
				return(1);
			}
			if (s & TRAP)
			{
				if (!(s & HIDDEN))
				{
					if (((row == drow) || (col == dcol))
					    && (!((row == rogue.row)
					    || (col == rogue.col))))
					{
						continue;
					}
					return(1);
				}
			}
			if ((((i - j) == 1) || ((i - j) == -1)) && (s & TUNNEL))
			{
				if (++pass_count > 1)
				{
					return(1);
				}
			}
			if ((s & DOOR) && ((i == 0) || (j == 0)))
			{
					return(1);
			}
		}
	}
	return(0);
}

int can_move(int row1, int col1, int row2, int col2)
{
	if (!is_passable(row2, col2))
	{
		return(0);
	}
	if ((row1 != row2) && (col1 != col2))
	{
		if ((dungeon[row1][col1]&DOOR)||(dungeon[row2][col2]&DOOR))
		{
			return(0);
		}
		if ((!dungeon[row1][col2]) || (!dungeon[row2][col1]))
		{
			return(0);
		}
	}
	return(1);
}

void move_onto(void)
{
	short ch;
	boolean first_miss = 1;

	while (!is_direction(ch = rgetchar()))
	{
		sound_bell();
		if (first_miss)
		{
			message("Direction? ", 0);
			first_miss = 0;
		}
	}
	check_message();
	if (ch != ROGUE_KEY_CANCEL)
	{
		(void) one_move_rogue(ch, 0);
	}
}

boolean is_direction(char c)
{
	return(
		(c == ROGUE_KEY_WEST) ||
		(c == ROGUE_KEY_SOUTH) ||
		(c == ROGUE_KEY_NORTH) ||
		(c == ROGUE_KEY_EAST) ||
		(c == ROGUE_KEY_SOUTHWEST) ||
		(c == ROGUE_KEY_NORTHWEST) ||
		(c == ROGUE_KEY_NORTHEAST) ||
		(c == ROGUE_KEY_SOUTHEAST) ||
		(c == ROGUE_KEY_CANCEL)
		);
}

boolean check_hunger(boolean messages_only)
{
	short i, n;
	boolean fainted = 0;

	if (rogue.moves_left == HUNGRY)
	{
		(void) strcpy(hunger_str, "hungry");
		message("You are beginning to feel hungry.", 0);
		print_stats(STAT_HUNGER);
	}
	if (rogue.moves_left == WEAK)
	{
		(void) strcpy(hunger_str, "weak");
		message("You're getting weak from hunger.", 1);
		print_stats(STAT_HUNGER);
	}
	if (rogue.moves_left <= FAINT)
	{
		if (rogue.moves_left == FAINT)
		{
			(void) strcpy(hunger_str, "faint");
			message("Your hunger is making you feel faint.", 1);
			print_stats(STAT_HUNGER);
		}
		n = get_rand(0, (FAINT - rogue.moves_left));
		if (n > 0)
		{
			fainted = 1;
			if (rand_percent(40))
			{
				rogue.moves_left++;
			}
			message("you faint", 1);
			for (i = 0; i < n; i++)
			{
				if (coin_toss())
				{
					mv_mons();
				}
			}
			message(you_can_move_again, 1);
		}
	}
	if (messages_only)
	{
		return(fainted);
	}
	if (rogue.moves_left <= STARVE)
	{
		killed_by((object *) 0, STARVATION);
	}

	switch(e_rings)
	{
	/*case -2:
		Subtract 0, i.e. do nothing.
		break;*/
	case -1:
		rogue.moves_left -= (rogue.moves_left % 2);
		break;
	case 0:
		rogue.moves_left--;
		break;
	case 1:
		rogue.moves_left--;
		(void) check_hunger(1);
		rogue.moves_left -= (rogue.moves_left % 2);
		break;
	case 2:
		rogue.moves_left--;
		(void) check_hunger(1);
		rogue.moves_left--;
		break;
	}
	return(fainted);
}

boolean reg_move(void)
{
	boolean fainted;

	if ((rogue.moves_left <= HUNGRY) || (cur_level >= max_level))
	{
		fainted = check_hunger(0);
	}
	else
	{
		fainted = 0;
	}

	mv_mons();

	if (++m_moves >= 120)
	{
		m_moves = 0;
		wanderer();
	}
	if (halluc)
	{
		if (!(--halluc))
		{
			unhallucinate();
		}
		else
		{
			hallucinate();
		}
	}
	if (blind)
	{
		if (!(--blind))
		{
			unblind();
		}
	}
	if (confused)
	{
		if (!(--confused))
		{
			unconfuse();
		}
	}
	if (bear_trap)
	{
		bear_trap--;
	}
	if (levitate)
	{
		if (!(--levitate))
		{
			message("You float gently to the ground.", 1);
			if (dungeon[rogue.row][rogue.col] & TRAP)
			{
				trap_player(rogue.row, rogue.col);
			}
		}
	}
	if (haste_self)
	{
		if (!(--haste_self))
		{
			message("You feel yourself slowing down.", 0);
		}
	}
	heal();
	if (auto_search > 0)
	{
		search(auto_search, auto_search);
	}
	return(fainted);
}

void rest(int count)
{
	int i;

	interrupted = 0;

	for (i = 0; i < count; i++)
	{
		if (interrupted)
		{
			break;
		}
		(void) reg_move();
	}
}

short gr_dir(void)
{
	short d;

	d = get_rand(1, 8);

	switch(d)
	{
		case 1:
			d = ROGUE_KEY_SOUTH;
			break;
		case 2:
			d = ROGUE_KEY_NORTH;
			break;
		case 3:
			d = ROGUE_KEY_EAST;
			break;
		case 4:
			d = ROGUE_KEY_WEST;
			break;
		case 5:
			d = ROGUE_KEY_NORTHWEST;
			break;
		case 6:
			d = ROGUE_KEY_NORTHEAST;
			break;
		case 7:
			d = ROGUE_KEY_SOUTHWEST;
			break;
		case 8:
			d = ROGUE_KEY_SOUTHEAST;
			break;
	}
	return(d);
}

void heal(void)
{
	static short heal_exp = -1, n, c = 0;
	static boolean alt;

	if (rogue.hp_current == rogue.hp_max)
	{
		c = 0;
		return;
	}
	if (rogue.exp != heal_exp)
	{
		heal_exp = rogue.exp;

		switch(heal_exp)
		{
		case 1:
			n = 20;
			break;
		case 2:
			n = 18;
			break;
		case 3:
			n = 17;
			break;
		case 4:
			n = 14;
			break;
		case 5:
			n = 13;
			break;
		case 6:
			n = 10;
			break;
		case 7:
			n = 9;
			break;
		case 8:
			n = 8;
			break;
		case 9:
			n = 7;
			break;
		case 10:
			n = 4;
			break;
		case 11:
			n = 3;
			break;
		case 12:
		default:
			n = 2;
		}
	}
	if (++c >= n)
	{
		c = 0;
		rogue.hp_current++;
		if ((alt = !alt))
		{ /* ?????? ashwin */
			rogue.hp_current++;
		}
		if ((rogue.hp_current += regeneration) > rogue.hp_max)
		{
			rogue.hp_current = rogue.hp_max;
		}
		print_stats(STAT_HP);
	}
}
