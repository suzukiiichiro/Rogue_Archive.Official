/* ring.c */

#include <stdio.h>
#include <string.h>
#include "rogue.h"
#include "inventory.h"
#include "keys.h"
#include "message.h"
#include "move.h"
#include "object.h"
#include "pack.h"
#include "random.h"
#include "ring.h"
#include "save.h"
#include "use.h"

char *left_or_right = "Left or right hand?";
char *no_ring = "There's no ring on that hand.";
short stealthy, r_rings, add_strength, e_rings, regeneration, ring_exp;
short auto_search;
boolean r_teleport, r_see_invisible, sustain_strength, maintain_armor;

extern char *curse_message;
extern boolean wizard;
extern fighter rogue;

void put_on_ring(void)
{
	short ch;
	char desc[DCOLS];
	object *ring;

	if (r_rings == 2)
	{
		message("Wearing two rings already.", 0);
		return;
	}
	if ((ch = pack_letter("Put on what?", RING)) == ROGUE_KEY_CANCEL)
	{
		return;
	}
	if (!(ring = get_letter_object(ch)))
	{
		message("No such item.", 0);
		return;
	}
	if (!(ring->what_is & RING))
	{
		message("That's not a ring!", 0);
		return;
	}
	if (ring->in_use_flags & (ON_LEFT_HAND | ON_RIGHT_HAND))
	{
		message("That ring is already being worn.", 0);
		return;
	}
	if (r_rings == 1)
	{
		ch = (rogue.left_ring ? 'r' : 'l');
	}
	else
	{
		message(left_or_right, 0);
		do
		{
			ch = rgetchar();
		} while ((ch != ROGUE_KEY_CANCEL) && (ch != 'l') && (ch != 'r')
		         && (ch != '\n') && (ch != '\r'));
	}
	if ((ch != 'l') && (ch != 'r'))
	{
		check_message();
		return;
	}
	if (((ch == 'l') && rogue.left_ring)||((ch == 'r') && rogue.right_ring))
	{
		check_message();
		message("There's already a ring on that hand.", 0);
		return;
	}
	if (ch == 'l')
	{
		do_put_on(ring, 1);
	}
	else
	{
		do_put_on(ring, 0);
	}
	ring_stats(1);
	check_message();
	get_desc(ring, desc);
	message(desc, 0);
	(void) reg_move();
}

/*
 * Do not call ring_stats() from within do_put_on().  It will cause
 * serious problems when do_put_on() is called from read_pack() in restore().
 */

void do_put_on(object *ring, boolean on_left)
{
	if (on_left)
	{
		ring->in_use_flags |= ON_LEFT_HAND;
		rogue.left_ring = ring;
	}
	else
	{
		ring->in_use_flags |= ON_RIGHT_HAND;
		rogue.right_ring = ring;
	}
}

void remove_ring(void)
{
	boolean left = 0, right = 0;
	short ch;
	char buf[DCOLS];
	object *ring = NULL;

	if (r_rings == 0) {
		inv_rings();
	} else if (rogue.left_ring && !rogue.right_ring) {
		left = 1;
	} else if (!rogue.left_ring && rogue.right_ring) {
		right = 1;
	} else {
		message(left_or_right, 0);
		do
		{
			ch = rgetchar();
		} while ((ch != ROGUE_KEY_CANCEL)
			 && (ch != 'l') && (ch != 'r') &&
		         (ch != '\n') && (ch != '\r'));
		left = (ch == 'l');
		right = (ch == 'r');
		check_message();
	}
	if (left || right)
	{
		if (left)
		{
			if (rogue.left_ring)
			{
				ring = rogue.left_ring;
			}
			else
			{
				message(no_ring, 0);
			}
		}
		else
		{
			if (rogue.right_ring)
			{
				ring = rogue.right_ring;
			}
			else
			{
				message(no_ring, 0);
			}
		}
		if (ring->is_cursed)
		{
			message(curse_message, 0);
		}
		else
		{
			un_put_on(ring);
			(void) strcpy(buf, "Removed ");
			get_desc(ring, buf + 8);
			message(buf, 0);
			(void) reg_move();
		}
	}
}

void un_put_on(object *ring)
{
	if (ring && (ring->in_use_flags & ON_LEFT_HAND)) {
		ring->in_use_flags &= (~ON_LEFT_HAND);
		rogue.left_ring = 0;
	} else if (ring && (ring->in_use_flags & ON_RIGHT_HAND)) {
		ring->in_use_flags &= (~ON_RIGHT_HAND);
		rogue.right_ring = 0;
	}
	ring_stats(1);
}

void gr_ring(object *ring, boolean assign_wk)
{
	ring->what_is = RING;
	if (assign_wk)
	{
		ring->which_kind = get_rand(0, (RINGS - 1));
	}
	ring->oclass = 0;

	switch(ring->which_kind)
	{
	/*
	case STEALTH:
		break;
	case SLOW_DIGEST:
		break;
	case REGENERATION:
		break;
	case R_SEE_INVISIBLE:
		break;
	case SUSTAIN_STRENGTH:
		break;
	case R_MAINTAIN_ARMOR:
		break;
	case SEARCHING:
		break;
	*/
	case R_TELEPORT:
		ring->is_cursed = 1;
		break;
	case ADD_STRENGTH:
	case DEXTERITY:
		while ((ring->oclass = (get_rand(0, 4) - 2)) == 0) ;
		ring->is_cursed = (ring->oclass < 0);
		break;
	case ADORNMENT:
		ring->is_cursed = coin_toss();
		break;
	}
}

void inv_rings(void)
{
	char buf[DCOLS];

	if (r_rings == 0)
	{
		message("Not wearing any rings.", 0);
	}
	else
	{
		if (rogue.left_ring)
		{
			get_desc(rogue.left_ring, buf);
			message(buf, 0);
		}
		if (rogue.right_ring)
		{
			get_desc(rogue.right_ring, buf);
			message(buf, 0);
		}
	}
	/*if (wizard)
	{
		sprintf(buf, "ste %d, r_r %d, e_r %d, r_t %d, s_s %d, a_s %d, reg %d, r_e %d, s_i %d, m_a %d, aus %d",
			stealthy, r_rings, e_rings, r_teleport, sustain_strength,
			add_strength, regeneration, ring_exp, r_see_invisible,
			maintain_armor, auto_search);
		message(buf, 0);
	}*/
}

void wiz_ring_info()
{
	char buf[DCOLS];

	sprintf(buf, "ste %d, r_r %d, e_r %d, r_t %d, s_s %d, a_s %d, reg %d, r_e %d, s_i %d, m_a %d, aus %d",
		stealthy, r_rings, e_rings, r_teleport, sustain_strength,
		add_strength, regeneration, ring_exp, r_see_invisible,
		maintain_armor, auto_search);
	message(buf, 0);
}

void ring_stats(boolean pr)
{
	short i;
	object *ring;

	stealthy = 0;
	r_rings = 0;
	e_rings = 0;
	r_teleport = 0;
	sustain_strength = 0;
	add_strength = 0;
	regeneration = 0;
	ring_exp = 0;
	r_see_invisible = 0;
	maintain_armor = 0;
	auto_search = 0;

	for (i = 0; i < 2; i++)
	{
		if (!(ring = ((i == 0) ? rogue.left_ring : rogue.right_ring)))
		{
			continue;
		}
		r_rings++;
		e_rings++;
		switch(ring->which_kind)
		{
		case STEALTH:
			stealthy++;
			break;
		case R_TELEPORT:
			r_teleport = 1;
			break;
		case REGENERATION:
			regeneration++;
			break;
		case SLOW_DIGEST:
			e_rings -= 2;
			break;
		case ADD_STRENGTH:
			add_strength += ring->oclass;
			break;
		case SUSTAIN_STRENGTH:
			sustain_strength = 1;
			break;
		case DEXTERITY:
			ring_exp += ring->oclass;
			break;
		case ADORNMENT:
			break;
		case R_SEE_INVISIBLE:
			r_see_invisible = 1;
			break;
		case MAINTAIN_ARMOR:
			maintain_armor = 1;
			break;
		case SEARCHING:
			auto_search += 2;
			break;
		}
	}
	if (pr)
	{
		print_stats(STAT_STRENGTH);
		relight();
	}
}
