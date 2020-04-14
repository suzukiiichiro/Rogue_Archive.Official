/* zap.c */

#ifndef __CYGWIN__
#include <ncurses.h>
#else
#include <ncurses/ncurses.h>
#endif
#include <string.h>
#include "rogue.h"
#include "hit.h"
#include "keys.h"
#include "message.h"
#include "monster.h"
#include "move.h"
#include "object.h"
#include "pack.h"
#include "random.h"
#include "room.h"
#include "score.h"
#include "special_hit.h"
#include "use.h"
#include "zap.h"

boolean wizard = 0;

extern boolean being_held;
extern boolean score_only;
extern boolean detect_monster;
extern fighter rogue;
extern unsigned short dungeon[DROWS][DCOLS];
extern object level_monsters;

object * get_zapped_monster(short, short *, short *);
void tele_away(object *);

void zapp(void)
{
	short wch;
	boolean first_miss = 1;
	object *wand;
	short dir, row, col;
	object *monster;

	while (!is_direction(dir = rgetchar()))
	{
		sound_bell();
		if (first_miss)
		{
			message("Direction? ", 0);
			first_miss = 0;
		}
	}
	check_message();
	if (dir == ROGUE_KEY_CANCEL)
	{
		return;
	}
	if ((wch = pack_letter("Zap with what?", WAND)) == ROGUE_KEY_CANCEL)
	{
		return;
	}
	check_message();

	if (!(wand = get_letter_object(wch)))
	{
		message("no such item.", 0);
		return;
	}
	if (wand->what_is != WAND)
	{
		message("you can't zap with that", 0);
		return;
	}
	if (wand->oclass <= 0)
	{
		message("nothing happens", 0);
	}
	else
	{
		wand->oclass--;
		row = rogue.row; col = rogue.col;
		monster = get_zapped_monster(dir, &row, &col);
		if (monster)
		{
			wake_up(monster);
			zap_monster(monster, wand->which_kind);
			relight();
		}
	}
	(void) reg_move();
}

object * get_zapped_monster(short dir, short *row, short *col)
{
	short orow, ocol;

	for (;;)
	{
		orow = *row; ocol = *col;
		get_dir_rc(dir, row, col, 0);
		if (((*row == orow) && (*col == ocol))
		    || (dungeon[*row][*col] & (HORWALL | VERTWALL))
			|| (dungeon[*row][*col] == NOTHING))
		{
			return(0);
		}
		if (dungeon[*row][*col] & MONSTER)
		{
			if (!imitating(*row, *col))
			{
				return(object_at(&level_monsters, *row, *col));
			}
		}
	}
}

void zap_monster(object *monster, unsigned short kind)
{
	short row, col;
	object *nm;
	short tc;

	row = monster->row;
	col = monster->col;

	switch(kind)
	{
	case SLOW_MONSTER:
		if (monster->m_flags & HASTED)
		{
			monster->m_flags &= (~HASTED);
		}
		else
		{
			monster->slowed_toggle = 0;
			monster->m_flags |= SLOWED;
		}
		break;
	case HASTE_MONSTER:
		if (monster->m_flags & SLOWED)
		{
			monster->m_flags &= (~SLOWED);
		}
		else
		{
			monster->m_flags |= HASTED;
		}
		break;
	case TELE_AWAY:
		tele_away(monster);
		break;
	case CONFUSE_MONSTER:
		monster->m_flags |= CONFUSED;
		monster->moves_confused += get_rand(12, 22);
		break;
	case INVISIBILITY:
		monster->m_flags |= INVISIBLE;
		break;
	case POLYMORPH:
		if (monster->m_flags & HOLDS)
		{
			being_held = 0;
		}
		nm = monster->next_monster;
		tc = monster->trail_char;
		(void) gr_monster(monster, get_rand(0, MONSTERS-1));
		monster->row = row;
		monster->col = col;
		monster->next_monster = nm;
		monster->trail_char = tc;
		if (!(monster->m_flags & IMITATES))
		{
			wake_up(monster);
		}
		break;
	case PUT_TO_SLEEP:
		monster->m_flags |= (ASLEEP | NAPPING);
		monster->nap_length = get_rand(3, 6);
		break;
	case MAGIC_MISSILE:
		rogue_hit(monster, 1);
		break;
	case CANCELLATION:
		if (monster->m_flags & HOLDS)
		{
			being_held = 0;
		}
		if (monster->m_flags & STEALS_ITEM)
		{
			monster->drop_percent = 0;
		}
		monster->m_flags &= (~(FLIES | FLITS | SPECIAL_HIT | INVISIBLE
		                       | FLAMES | IMITATES | CONFUSES
							   | SEEKS_GOLD | HOLDS));
		break;
	case DO_NOTHING:
		message("nothing happens", 0);
		break;
	}
}

void tele_away(object *monster)
{
    int row, col;

    if (monster->m_flags & HOLDS)
	{
        being_held = 0;
    }
    gr_row_col(&row, &col, (FLOOR | TUNNEL | STAIRS | OBJECT));
    mvaddch(monster->row, monster->col, monster->trail_char);
    dungeon[monster->row][monster->col] &= ~MONSTER;
    monster->row = row; monster->col = col;
    dungeon[row][col] |= MONSTER;
    monster->trail_char = mvinch(row, col);
    if (detect_monster || rogue_can_see(row, col))
	{
        mvaddch(row, col, gmc(monster));
    }
}

void wizardize(void)
{
	char buf[100];

	if (wizard)
	{
		wizard = 0;
		message("Not wizard anymore.", 0);
	}
	else
	{
		if (get_input_line("Your score will not be saved! Become Wizard anyway (y/N)?", buf, "", "", 0, 1))
		{
			if (!strncmp(buf, "Y", 1) || !strncmp(buf, "y", 1))
			{
				wizard = 1;
				score_only = 1;
				message("Welcome, mighty wizard!", 0);
			}
			else
			{
				message("Returning to game...", 0);
			}
		}
	}
}
