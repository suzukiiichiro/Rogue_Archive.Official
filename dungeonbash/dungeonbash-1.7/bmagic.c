/* bmagic.c 
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
#include "dunbash.h"
#include "bmagic.h"
#include "monsters.h"
#include "combat.h"

/* BLACK MAGIC
 *
 * Certain of the denizens of the dungeon have the power to use black magic
 * against the player.
 * 
 * The "ordinary" lich may unleash bolts of necromantic force against
 * the player, or smite him at close quarters with their staves of necromancy,
 * or invoke grim curses against him.
 *
 * The dreaded master liches can smite the player from a distance with
 * their necromantic powers without lying on a cardinal direction from
 * him, and can steal the player's vitality with a touch, as well as having
 * the spells of their lesser brethren.  Furthermore, they may attempt to
 * evade the player in the same manner as wizards.
 *
 * Itinerant wizards roaming the dungeon cast bolts of lightning, and strike
 * in hand-to-hand combat with staves wreathed with enchantments of shattering
 * force; if sorely pressed, they may invoke their powers to teleport across
 * the dungeon level, cheating the player of his victory.
 * 
 * Archmages, learned scholars of the Black Arts and veterans of many a
 * confrontation, have the powers of wizards. In addition, an archmage who
 * teleports away from the player to evade death may well leave him with a
 * group of summoned monsters.
 * 
 * The more potent order of demons known as defilers may cast curses against
 * the player, or call down a column of fire to smite him.
 *
 * Some forms of black magic may be defended against by wearing the proper
 * armour or putting on a suitable ring; others bypass all such defences to
 * strike the player directly, although some of these can be evaded by those
 * with high enough agility.
 */

int use_black_magic(int mon)
{
    /* Returns zero for no spell selected, -1 for unsupported spell
     * selected, 1 for supported spell selected. */
    struct mon *mptr = monsters + mon;
    int dy, dx;
    int sy, sx;
    enum monspell to_cast = MS_REJECT;
    int rval = 1;	/* Default to success; failure paths will force this
			 * to an appropriate value. */
    int dieroll;
    int meleerange;
    int oncardinal;
    int i;
    int cansee;
    int range;
    compute_directions(u.y, u.x, mptr->y, mptr->x, &dy, &dx, &sy, &sx, &meleerange, &oncardinal);
    cansee = inyourroom(mptr->y, mptr->x) || meleerange;
    if ((dy * sy) >= (dx * sx))
    {
	range = dy * sy;
    }
    else
    {
	range = dx * sx;
    }
    switch (monsters[mon].mon_id)
    {
    case PM_ARCHMAGE:
	if (cansee)
	{
	    /* We have LOS; choose a spell on that basis. */
	    if ((mptr->hpcur < (mptr->hpmax * 25 / 100)) && (zero_die(10) < 2))
	    {
		to_cast = zero_die(3) ? MS_TELEPORT_ESCAPE : MS_TELEPORT_AND_SUMMON;
	    }
	    else if (meleerange && (zero_die(10) > 3))
	    {
		to_cast = MS_STRIKE_STAFF;
	    }
	    else if (oncardinal)
	    {
		to_cast = MS_LIGHTNING;
	    }
	}
	else if (!zero_die(40))
	{
	    /* 
	     * We lack LOS, but pass the 1-in-40 chance; use
	     * black magic to relocate us to the player's location.
	     */
	    to_cast = MS_TELEPORT_ASSAULT;
	}
	break;

    case PM_WIZARD:
	if (cansee)
	{
	    if ((mptr->hpcur < (mptr->hpmax * 25 / 100)) && (zero_die(10) < 2))
	    {
		to_cast = MS_TELEPORT_ESCAPE;
	    }
	    else if (meleerange && (zero_die(10) > 2))
	    {
		to_cast = MS_STRIKE_STAFF;
	    }
	    else if (oncardinal)
	    {
		to_cast = MS_LIGHTNING;
	    }
	}
	else if (!zero_die(80))
	{
	    /* we lack LOS, but passed the 1-in-80 chance to
	     * close with the player by means of black magic. */
	    to_cast = MS_TELEPORT_ASSAULT;
	}
	break;

    case PM_MASTER_LICH:
	if (cansee)
	{
	    if ((mptr->hpcur < (mptr->hpmax * 25 / 100)) && (zero_die(10) < 4))
	    {
		to_cast = !zero_die(3) ? MS_TELEPORT_ESCAPE : MS_TELEPORT_AND_SUMMON;
	    }
	    else if (meleerange)
	    {
		switch (zero_die(7))
		{
		case 6:
		    if (!u.withering)
		    {
			to_cast = MS_CURSE_WITHERING;
			break;
		    }
		case 4:
		    if (!u.leadfoot)
		    {
			to_cast = MS_CURSE_LEADFOOT;
			break;
		    }
		    /* fall through */
		case 5:
		    if (!u.armourmelt)
		    {
			to_cast = MS_CURSE_ARMOURMELT;
			break;
		    }
		    /* fall through */
		default:
		    to_cast = zero_die(2) ? MS_CHILLING_TOUCH : MS_STRIKE_STAFF;
		    break;
		}
	    }
	    else if (range < 3)
	    {
		switch (zero_die(10))
		{
		case 9:
		    if (!u.withering)
		    {
			to_cast = MS_CURSE_WITHERING;
			break;
		    }
		case 8:
		    if (!u.leadfoot)
		    {
			to_cast = MS_CURSE_LEADFOOT;
			break;
		    }
		    /* fall through */
		case 7:
		    if (!u.armourmelt)
		    {
			to_cast = MS_CURSE_ARMOURMELT;
			break;
		    }
		    /* fall through */
		default:
		    to_cast = MS_NECRO_SMITE;
		    break;
		}
	    }
	    else if (range < 8)
	    {
		switch (zero_die(7))
		{
		case 6:
		    if (!u.withering)
		    {
			to_cast = MS_CURSE_WITHERING;
			break;
		    }
		case 4:
		    if (!u.leadfoot)
		    {
			to_cast = MS_CURSE_LEADFOOT;
			break;
		    }
		    /* fall through */
		case 5:
		    if (!u.armourmelt)
		    {
			to_cast = MS_CURSE_ARMOURMELT;
			break;
		    }
		    /* fall through */
		default:
		    to_cast = MS_NECRO_SMITE;
		    break;
		}
	    }
	}
	else if (!zero_die(40))
	{
	    /* we lack LOS, but passed the 1-in-40 chance to
	     * close with the player by means of black magic. */
	    to_cast = MS_TELEPORT_ASSAULT;
	}
	break;
    case PM_LICH:
	if (cansee)
	{
	    if (meleerange)
	    {
		dieroll = zero_die(6);
		switch (dieroll)
		{
		case 4:
		    if (!u.leadfoot)
		    {
			to_cast = MS_CURSE_LEADFOOT;
			break;
		    }
		    /* fall through */
		case 5:
		    if (!u.armourmelt)
		    {
			to_cast = MS_CURSE_ARMOURMELT;
			break;
		    }
		    /* fall through */
		default:
		    to_cast = MS_NECRO_STAFF;
		    break;
		}
	    }
	    else if (oncardinal)
	    {
		if (range < 3)
		{
		    switch (zero_die(6))
		    {
		    case 4:
			if (!u.leadfoot)
			{
			    to_cast = MS_CURSE_LEADFOOT;
			    break;
			}
			/* fall through */
		    case 5:
			if (!u.armourmelt)
			{
			    to_cast = MS_CURSE_ARMOURMELT;
			    break;
			}
			/* fall through */
		    default:
			to_cast = MS_NECRO_BOLT;
			break;
		    }
		}
		else
		{
		    to_cast = MS_NECRO_BOLT;
		}
	    }
	    break;
	}
	break;
    case PM_DEFILER:
	if (cansee)
	{
	    if (!meleerange)
	    {
		switch (zero_die(7))
		{
		case 6:
		    if (!u.withering)
		    {
			to_cast = MS_CURSE_WITHERING;
			break;
		    }
		case 4:
		    if (!u.leadfoot)
		    {
			to_cast = MS_CURSE_LEADFOOT;
			break;
		    }
		    /* fall through */
		case 5:
		    if (!u.armourmelt)
		    {
			to_cast = MS_CURSE_ARMOURMELT;
			break;
		    }
		    /* fall through */
		default:
		    to_cast = MS_FIRE_COLUMN;
		    break;
		}
	    }
	}
	break;

    default:
	break;
    }
    switch (to_cast)
    {
    default:
	/* If this happens, we're trying to cast an unimplemented
	 * spell. */
	print_msg("Can't happen: Bogus spell %d!\n", to_cast);
	rval = -1;
	break;

    case MS_REJECT:
	/* No usable spell available. */
	rval = 0;
	break;

    case MS_STRIKE_STAFF:
	mhitu(mon, DT_PHYS);
	break;

    case MS_NECRO_STAFF:
	mhitu(mon, DT_NECRO);
	break;

    case MS_CHILLING_TOUCH:
	mhitu(mon, DT_COLD);
	break;

    case MS_LIGHTNING:
    case MS_NECRO_BOLT:
	mshootu(mon);
	break;

    case MS_TELEPORT_AND_SUMMON:
	/* Do the summoning... */
	print_mon_name(mon, 3);
	print_msg(" calls for help...\n");
	/* (Try to) summon 2-6 monsters. */
	i = summoning(mptr->y, mptr->x, dice(2, 3));
	if (i == 0)
	{
	    print_msg("... luckily for you, help wasn't listening.\n");
	}
	else
	{
	    print_msg("... and gets it.\n");
	}
	/* ... and fall through. */
    case MS_TELEPORT_ESCAPE:
	print_mon_name(mon, 3);
	print_msg(" vanishes in a puff of smoke.\n");
	teleport_mon(mon);
	break;

    case MS_TELEPORT_ASSAULT:
	/* It is rare that a monster will cast this spell, but not
	 * unheard of. */
	teleport_mon_to_you(mon);
	break;

    case MS_CURSE_ARMOURMELT:
	mon_curses(mon);
	if (u.protection)
	{
	    malignant_aura();
	}
	else
	{
	    u.armourmelt = 10 + one_die(10);
	    print_msg("Your armour seems suddenly no stronger than dust!\n");
	}
	break;

    case MS_CURSE_LEADFOOT:
	mon_curses(mon);
	if (u.protection)
	{
	    malignant_aura();
	}
	else
	{
	    u.leadfoot = 10 + one_die(10);
	    print_msg("Your feet feel like lead!\n");
	}
	break;

    case MS_CURSE_WITHERING:
	mon_curses(mon);
	if (u.protection)
	{
	    malignant_aura();
	}
	else
	{
	    u.withering = 10 + one_die(10);
	    print_msg("Your limbs twist and wither!\n");
	}
	break;

    case MS_NECRO_SMITE:
	mon_curses(mon);
	if (player_resists_dtype(DT_NECRO))
	{
	    print_msg("Darkness reaches towards you, but dissolves.\n");
	}
	else
	{
	    print_msg("Soul-chilling darkness engulfs you!\n");
	    damage_u(dice(1, 20), DEATH_KILLED_MON, permons[monsters[mon].mon_id].name);
	}
	break;

    case MS_FIRE_COLUMN:
	mon_curses(mon);
	print_msg("The fires of hell ");
	if (player_resists_dtype(DT_FIRE))
	{
	    print_msg("lightly singe you.\n");
	    damage_u(dice(1, 5), DEATH_KILLED_MON, permons[monsters[mon].mon_id].name);
	}
	else
	{
	    print_msg("burn you!\n");
	    damage_u(dice(1, 20), DEATH_KILLED_MON, permons[monsters[mon].mon_id].name);
	}
	break;
    }
    return rval;
}

void malignant_aura()
{
    print_msg("A malignant aura surrounds you briefly.\n");
}

void mon_curses(int mon)
{
    print_mon_name(mon, 3);
    print_msg(" points at you and curses horribly.\n");
}

/* bmagic.c */
// あ
