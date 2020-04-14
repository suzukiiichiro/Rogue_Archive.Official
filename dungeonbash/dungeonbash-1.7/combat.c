/* combat.c 
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
#include "combat.h"
#include "monsters.h"

int player_attack(int dy, int dx)
{
    if ((objects[u.weapon].obj_id == PO_BOW) || (objects[u.weapon].obj_id == PO_CROSSBOW))
    {
	ushootm(dy, dx);
    }
    else if (mapmonster[u.y + dy][u.x + dx] != -1)
    {
	uhitm(mapmonster[u.y + dy][u.x + dx]);
    }
    else
    {
	print_msg("Nothing to attack.\n");
	return 0;
    }
    return 1;
}

int uhitm(int mon)
{
    struct mon *mp;
    struct obj *wep;
    struct permobj *pwep;
    struct obj *ring;
    struct permobj *pring;
    int tohit;
    int damage;
    int healing;
    mp = monsters + mon;
    tohit = zero_die(u.agility + u.level);
    if (tohit < mp->defence)
    {
	print_msg("You miss.\n");
	return 0;	/* Missed. */
    }
    print_msg("You hit ");
    print_mon_name(mon, 1);
    print_msg(".\n");
    if (u.weapon != -1)
    {
	wep = objects + u.weapon;
	pwep = permobjs + wep->obj_id;
	damage = one_die(pwep->power) + (u.body / 10);
    }
    else
    {
	damage = u.body / 10;
    }
    if (u.ring != -1)
    {
	ring = objects + u.ring;
	pring = permobjs + ring->obj_id;
	switch (ring->obj_id)
	{
	case PO_RING_FIRE:
	    if (!pmon_resists_fire(mp->mon_id))
	    {
		if (!pring->known)
		{
		    pring->known = 1;
		}
		print_msg("Your ring burns ");
		print_mon_name(mon, 1);
		print_msg("!\n");
		damage += (damage + 1) / 2 + dice(2, 4);
	    }
	    break;
	case PO_RING_VAMPIRE:
	    if (!pmon_is_undead(mp->mon_id))
	    {
		if (!pring->known)
		{
		    pring->known = 1;
		}
		print_msg("Your ring drains ");
		print_mon_name(mon, 1);
		print_msg("!\n");
		damage += (damage + 3) / 4 + dice(2, 4);
		healing = (damage + 5) / 6;
		heal_u(healing, 0, 1);
	    }
	    break;
	case PO_RING_FROST:
	    if (!pmon_resists_cold(mp->mon_id))
	    {
		if (!pring->known)
		{
		    pring->known = 1;
		}
		print_msg("Your ring freezes ");
		print_mon_name(mon, 1);
		print_msg("!\n");
		damage += (damage + 2) / 3 + dice(1, 6);
	    }
	}
    }
    print_msg("You do %d damage.\n", damage);
    damage_mon(mon, damage, 1);
    if (u.weapon != -1)
    {
	damage_obj(u.weapon);
    }
    return 1;	/* Hit. */
}

int ushootm(int sy, int sx)
{
    /* Propagate a missile in direction (sy,sx). Attack first target in
     * LOF. */
    int tohit;
    int range;
    int y, x;
    int done = 0;
    struct mon *mptr;
    struct obj *wep;
    struct permobj *pwep;
    int damage;
    wep = objects + u.weapon;
    pwep = permobjs + wep->obj_id;
    damage = one_die(pwep->power);
    y = u.y + sy;
    x = u.x + sx;
    range = 1;
    for ( ; !done; (y += sy), (x += sx))
    {
	if (mapmonster[y][x] != -1)
	{
	    done = 1;
	    mptr = monsters + mapmonster[y][x];
	    tohit = zero_die(u.agility + u.level - range);
	    if (range == 1)
	    {
		/* Shooting at point-blank is tricky */
		tohit = (tohit + 1) / 2;
	    }
	    if (tohit >= mptr->defence)
	    {
		if (mon_visible(mapmonster[y][x]))
		{
		    print_msg("You hit ");
		    print_mon_name(mapmonster[y][x], 1);
		    print_msg(".\n");
		    print_msg("You do %d damage.\n", damage);
		}
		damage_mon(mapmonster[y][x], damage, 1);
		return 1;
	    }
	    else
	    {
		print_msg("You miss ");
		print_mon_name(mapmonster[y][x], 1);
		print_msg(".\n");
		return 0;
	    }
	}
	else if ((terrain[y][x] == WALL) || (terrain[y][x] == DOOR))
	{
	    print_msg("Your %s hits the %s.\n", (wep->obj_id == PO_BOW) ? "arrow" : "bolt", (terrain[y][x] == WALL) ? "wall" : "door");
	    return 0;
	}
    }
    return 0;
}

int mhitu(int mon, enum damtyp dtype)
{
    int tohit;
    int damage;
    int unaffected;
    struct mon *mptr = monsters + mon;
    tohit = zero_die(mptr->mtohit + 5);
    if (tohit < u.defence)
    {
	/* Note: Yes, all attacks can damage your armour. Deal. */
	if ((u.armour != -1) && (tohit > agility_modifier()))
	{
	    /* Monster hit your armour. */
	    print_msg("Your armour deflects ");
	    print_mon_name(mon, 1);
	    print_msg("'s blow.\n");
	    damage_obj(u.armour);
	}
	else
	{
	    print_mon_name(mon, 3);
	    print_msg(" misses you.\n");
	}
	return 0;
    }
    damage = one_die(mptr->mdam);
    unaffected = player_resists_dtype(dtype);
    print_mon_name(mon, 3);
    print_msg(" hits you.\n");
    if (u.armourmelt && (!zero_die(3)))
    {
	/* If you're subject to armourmelt, it is decreed that one
	 * blow in three hits your dust-weak armour and rips a chunk
	 * out of it. */
	damage_obj(u.armour);
    }
test_unaffected:
    if (unaffected)
    {
	switch (dtype)
	{
	case DT_PHYS:
	    print_msg("Can't happen: player resisting physical damage\n");
	    unaffected = 0;
	    /* Turn off the player's resistance, because they're
	     * not supposed to have it! */
	    u.resistances[DT_PHYS] = 0;
	    goto test_unaffected;
	case DT_FIRE:
	    print_msg("The flames seem pleasantly warm.\n");
	    if (unaffected & RESIST_RING)
	    {
		print_msg("Your ring flashes red.\n");
		permobjs[PO_RING_FIRE].known = 1;
	    }
	    break;
	case DT_COLD:
	    print_msg("Its touch seems pleasantly cool.\n");
	    if (unaffected & RESIST_RING)
	    {
		print_msg("Your ring flashes blue.\n");
		permobjs[PO_RING_FROST].known = 1;
	    }
	    break;
	case DT_NECRO:
	    print_msg("Its touch makes you feel no deader.\n");
	    if (objects[u.ring].obj_id == PO_RING_VAMPIRE)
	    {
		permobjs[PO_RING_VAMPIRE].known = 1;
		print_msg("Your ring shrieks.\n");
	    }
	    break;
	default:
	    print_msg("Can't happen: bogus damage type.\n");
	    break;
	}
    }
    else
    {
	switch (dtype)
	{
	default:
	case DT_PHYS:
	    break;
	case DT_FIRE:
	    print_msg("You are engulfed in flames.\n");
	    break;
	case DT_COLD:
	    print_msg("You are covered in frost.\n");
	    break;
	case DT_NECRO:
	    print_msg("You feel your life force slipping away.\n");
	    break;
	}
	print_msg("You take %d damage.\n", damage);
	if ((mptr->mon_id == PM_VAMPIRE) && !player_resists_dtype(DT_NECRO))
	{
	    heal_mon(mon, damage * 2 / 5, 1);
	} else if ((tohit - u.defence >= 5) && (mptr->mon_id == PM_SNAKE))
	{
	    drain_body(1, "snake venom", 0);
	}
	damage_u(damage, DEATH_KILLED_MON, permons[mptr->mon_id].name);
	display_update();
    }
    return 1;
}

int mshootu(int mon)
{
    struct mon *mptr;
    struct mon *bystander;
    int y;
    int x;
    int dy;
    int dx;
    int sy, sx;
    int done;
    int unaffected = 0;
    int tohit;
    int damage;
    int evasion;
    int defence;
    enum damtyp dtype;
    mptr = monsters + mon;
    y = mptr->y;
    x = mptr->x;
    /* dy, dx == trajectory of missile */
    dy = u.y - y;
    dx = u.x - x;
    sy = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0);
    sx = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);
    /* Don't get the bonus that applies to melee attacks. */
    tohit = zero_die(mptr->rtohit);
    print_mon_name(mon, 3);
    dtype = permons[mptr->mon_id].rdtyp;
    if (dtype == DT_PHYS)
    {
	print_msg(" %s at you!\n", permons[mptr->mon_id].shootverb);
    }
    else
    {
	print_msg(" %s %s at you!\n", permons[mptr->mon_id].shootverb, damtype_names[dtype]);
    }
    if ((dtype == DT_NECRO) || (dtype == DT_ELEC))
    {
	/* Use agility-based defence for necromantic blasts and lightning
	 * bolts */
	evasion = u.agility * 100;
	if (u.armour != -1)
	{
	    evasion -= (u.agility * evasion_penalty(u.armour));
	}
	defence = evasion / 200;
    }
    else
    {
	defence = u.defence;
    }
    /* Move projectile one square before looking for targets. */
    for ((done = 0), (y = mptr->y + sy), (x = mptr->x + sx);
	 !done;
	 (y += sy), (x += sx))
    {
	if ((terrain[y][x] == WALL) || (terrain[y][x] == DOOR))
	{
	    done = 1;
	}
	if ((y == u.y) && (x == u.x))
	{
	    if (tohit >= defence)
	    {
		done = 1;
		print_msg("It hits you!\n");
		unaffected = player_resists_dtype(dtype);
		if (unaffected)
		{
		    /* For now, resistant armours are always known, so
		     * we only need to check for identification of rings. */
		    if (unaffected & RESIST_RING)
		    {
			switch (dtype)
			{
			case DT_COLD:
			    if (objects[u.ring].obj_id == PO_RING_FROST)
			    {
				permobjs[PO_RING_FROST].known = 1;
				print_msg("Your ring flashes blue.\n");
			    }
			    break;
			case DT_FIRE:
			    if (objects[u.ring].obj_id == PO_RING_FIRE)
			    {
				permobjs[PO_RING_FIRE].known = 1;
				print_msg("Your ring flashes red.\n");
			    }
			    break;
			case DT_NECRO:
			    if (objects[u.ring].obj_id == PO_RING_VAMPIRE)
			    {
				permobjs[PO_RING_VAMPIRE].known = 1;
				print_msg("Your ring shrieks.\n");
			    }
			    break;
			default:
			    break;
			}
		    }
		}
		if (!unaffected)
		{
		    damage = one_die(mptr->rdam);
		    print_msg("You take %d damage.\n", damage);
		    damage_u(damage, DEATH_KILLED_MON, permons[mptr->mon_id].name);
		}
		display_update();
		return 1;
	    }
	    else
	    {
		print_msg("It misses you.\n");
	    }
	}
	else if (mapmonster[y][x] != -1)
	{
	    done = 1;
	    bystander = monsters + mapmonster[y][x];
	    switch (dtype)
	    {
	    case DT_COLD:
		if (pmon_resists_cold(bystander->mon_id))
		{
		    unaffected = 1;
		}
		else
		{
		    unaffected = 0;
		}
		break;
	    case DT_FIRE:
		if (pmon_resists_fire(bystander->mon_id))
		{
		    unaffected = 1;
		}
		else
		{
		    unaffected = 0;
		}
		break;
	    case DT_NECRO:
		if (pmon_is_undead(bystander->mon_id))
		{
		    unaffected = 1;
		}
		else
		{
		    unaffected = 0;
		}
		break;
	    default:
		unaffected = 0;
		break;
	    }
	    if (tohit >= bystander->defence)
	    {
		damage = one_die(mptr->rdam);
		damage_mon(mapmonster[y][x], dtype, 0);
	    }
	}
    }
    return 0;
}

/* combat.c */
//あ
