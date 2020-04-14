/* play.c */

#ifndef __CYGWIN__
#include <ncurses.h>
#else
#include <ncurses/ncurses.h>
#endif
#include <ctype.h>
#include "rogue.h"
#include "hit.h"
#include "instruct.h"
#include "inventory.h"
#include "keys.h"
#include "level.h"
#include "message.h"
#include "monster.h"
#include "move.h"
#include "object.h"
#include "pack.h"
#include "play.h"
#include "ring.h"
#include "room.h"
#include "save.h"
#include "score.h"
#include "throw.h"
#include "trap.h"
#include "use.h"
#include "zap.h"

boolean interrupted = 0;
boolean multimove = 0;
char *unknown_command = "Unknown command.";

// extern short party_room, bear_trap;
extern char hit_message[];
extern boolean wizard, trap_door;
extern fighter rogue;
extern object level_objects;

void whatisit(void)
{
	int ch;
	char *msg;
	char messbuf[80];
	extern char *m_names[];

	check_message();
	message("What character would you like to know?", 0);
	do
	{
		ch = getchar();
		if (isprint(ch) && !iscntrl(ch))
			break;
		if (ch == ROGUE_KEY_CANCEL)
			break;
		sound_bell();
	} while (1);
	if (isupper(ch))
	{
		char *article;
		msg = m_names[ch - 'A'];
		switch (*msg)
		{
			case 'A':
			case 'a':
			case 'E':
			case 'e':
			case 'I':
			case 'i':
			case 'O':
			case 'o':
			case 'U':
			case 'u':
				article = "an";
				break;
			default:
				article = "a";
				break;
		}
		sprintf(messbuf, "<%c> is %s %s", ch, article, msg);
		msg = messbuf;
	}
	else
	{
		switch (ch)
		{
			case ROGUE_KEY_CANCEL:
				check_message();
				return;
				break;
			case '|':
			case '-':
				msg = "the wall";
				break;
			case '+':
				msg = "a door";
				break;
			case '#':
				msg = "a tunnel";
				break;
			case '.':
				msg = "a floor tile";
				break;
			case '!':
				msg = "a potion";
				break;
			case '?':
				msg = "a scroll";
				break;
			case ')':
				msg = "a weapon";
				break;
			case ']':
				msg = "a suit of armour";
				break;
			case '*':
				msg = "some gold";
				break;
			case ':':
				msg = "some food";
				break;
			case '/':
				msg = "a wand or staff";
				break;
			case '=':
				msg = "a ring";
				break;
			case ',':
				msg = "The Amulet of Yendor";
				break;
			case '^':
				msg = "a trap";
				break;
			case '%':
				msg = "stairs";
				break;
			case '@':
				msg = "you";
				break;
			default:
				msg = messbuf;
				sprintf(messbuf, "I don't know what <%c> is either", ch);
				break;
		}
		if (msg != messbuf)
		{
			sprintf(messbuf, "<%c> is %s", ch, msg);
			msg = messbuf;
		}
	}
	check_message();
	message(msg, 0);
}

void play_level(void)
{
	short ch;
	int count;

	for (;;)
	{
		interrupted = 0;
		if (hit_message[0])
		{
			message(hit_message, 1);
			hit_message[0] = 0;
		}
		if (trap_door)
		{
			trap_door = 0;
			return;
		}
		move(rogue.row, rogue.col);
		refresh();

		ch = rgetchar();
		check_message();
		count = 0;
CH:
		switch(ch)
		{
		case ROGUE_KEY_MULTIMOVE:
			multimove = ((multimove == 0) ? 1 : 0);
			break;
		case ROGUE_KEY_INSTRUCTIONS:
			Instructions();
			break;
		case ROGUE_KEY_REST:
			rest((count > 0) ? count : 1);
			break;
		case ROGUE_KEY_SEARCH:
			search(((count > 0) ? count : 1), 0);
			break;
		case ROGUE_KEY_INVENTORY:
			inventory(&rogue.pack, ALL_OBJECTS);
			break;
		case ROGUE_KEY_FIGHT:
			fight(0);
			break;
		case ROGUE_KEY_FIGHT_TO_DEATH:
			fight(1);
			break;
		case ROGUE_KEY_NORTH:
		case ROGUE_KEY_SOUTH:
		case ROGUE_KEY_EAST:
		case ROGUE_KEY_WEST:
		case ROGUE_KEY_NORTHEAST:
		case ROGUE_KEY_NORTHWEST:
		case ROGUE_KEY_SOUTHEAST:
		case ROGUE_KEY_SOUTHWEST:
			(void) move_rogue(ch, 1);
			break;
		case ROGUE_KEY_NORTH_SHIFT:
		case ROGUE_KEY_SOUTH_SHIFT:
		case ROGUE_KEY_EAST_SHIFT:
		case ROGUE_KEY_WEST_SHIFT:
		case ROGUE_KEY_NORTHEAST_SHIFT:
		case ROGUE_KEY_NORTHWEST_SHIFT:
		case ROGUE_KEY_SOUTHEAST_SHIFT:
		case ROGUE_KEY_SOUTHWEST_SHIFT:
		case ROGUE_KEY_NORTH_CTRL:
		case ROGUE_KEY_SOUTH_CTRL:
		case ROGUE_KEY_EAST_CTRL:
		case ROGUE_KEY_WEST_CTRL:
		case ROGUE_KEY_NORTHEAST_CTRL:
		case ROGUE_KEY_NORTHWEST_CTRL:
		case ROGUE_KEY_SOUTHEAST_CTRL:
		case ROGUE_KEY_SOUTHWEST_CTRL:
			multiple_move_rogue(ch);
			break;
		case ROGUE_KEY_EAT:
			eat();
			break;
		case ROGUE_KEY_QUAFF:
			quaff();
			break;
		case ROGUE_KEY_READ:
			read_scroll();
			break;
		case ROGUE_KEY_MOVE:
			move_onto();
			break;
		case ROGUE_KEY_DROP:
			drop();
			break;
		case ROGUE_KEY_PUT_ON_RING:
			put_on_ring();
			break;
		case ROGUE_KEY_REMOVE_RING:
			remove_ring();
			break;
		case ROGUE_KEY_REMESSAGE:
			remessage();
			break;
		case ROGUE_KEY_WIZARDIZE:
			wizardize();
			break;
		case ROGUE_KEY_DROP_CHECK:
			if (drop_check())
			{
				return;
			}
			break;
		case ROGUE_KEY_CHECK_UP:
			if (check_up())
			{
				return;
			}
			break;
		case ROGUE_KEY_INV_WEAPON:
		case ROGUE_KEY_INV_ARMOR:
			inv_armor_weapon(ch == ')');
			break;
		case ROGUE_KEY_INV_RINGS:
			inv_rings();
			break;
		case ROGUE_KEY_ID_TRAP:
			id_trap();
			break;
		case ROGUE_KEY_SINGLE_INV:
			single_inv(0);
			break;
		case ROGUE_KEY_TAKE_OFF:
			take_off();
			break;
		case ROGUE_KEY_WEAR:
			wear();
			break;
		case ROGUE_KEY_WIELD:
			wield();
			break;
		case ROGUE_KEY_CALL:
			call_it();
			break;
		case ROGUE_KEY_ZAPP:
			zapp();
			break;
		case ROGUE_KEY_THROW:
			throw();
			break;
		case ROGUE_KEY_VERSION:
			message("LinuxRogue v0.3.7: Rogue clone for Linux / coredumpcentral.org", 0);
			break;
		case ROGUE_KEY_QUIT:
			quit(0);
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			move(rogue.row, rogue.col);
			refresh();
			do
			{
				if (count < 100)
				{
					count = (10 * count) + (ch - '0');
				}
				ch = rgetchar();
			} while (is_digit(ch));
			if (ch != ROGUE_KEY_CANCEL)
			{
				goto CH;
			}
			break;
		case ROGUE_KEY_NOP:
			break;
		case ROGUE_KEY_WIZ_INVENTORY:
			if (wizard)
			{
				inventory(&level_objects, ALL_OBJECTS);
			}
			else
			{
				message(unknown_command, 0);
			}
			break;
		case ROGUE_KEY_WIZ_MAGIC_MAP:
			if (wizard)
			{
				draw_magic_map();
			}
			else
			{
				message(unknown_command, 0);
			}
			break;
		case ROGUE_KEY_WIZ_SHOW_TRAPS:
			if (wizard)
			{
				show_traps();
			}
			else
			{
				message(unknown_command, 0);
			}
			break;
		case ROGUE_KEY_WIZ_SHOW_OBJS:
			if (wizard)
			{
				show_objects();
			}
			else
			{
				message(unknown_command, 0);
			}
			break;
		case ROGUE_KEY_SHOW_AV_HP:
			show_average_hp();
			break;
		case ROGUE_KEY_WIZ_NEW_OBJ:
			if (wizard)
			{
				new_object_for_wizard();
			}
			else
			{
				message(unknown_command, 0);
			}
			break;
		case ROGUE_KEY_WIZ_SHOW_MONST:
			if (wizard)
			{
				show_monsters();
			}
			else
			{
				message(unknown_command, 0);
			}
			break;
		case ROGUE_KEY_WIZ_RING_INFO:
			if (wizard)
			{
				wiz_ring_info();
			}
			else
			{
				message(unknown_command, 0);
			}
			break;
		case ROGUE_KEY_SAVE_GAME:
			save_game();
			break;
		case ROGUE_KEY_PICK_UP:
			kick_into_pack();
			break;
		case ROGUE_KEY_WHATISIT:
			whatisit();
			break;
		default:
			message(unknown_command, 0);
			break;
		}
	}
}
