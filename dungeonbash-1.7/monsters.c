/* monsters.c
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

#define MONSTERS_C
#include "dunbash.h"
#include "monsters.h"

struct mon monsters[100];
static int reject_mon(int pm);

int summoning(int y, int x, int how_many)
{
    int i;
    int dy;
    int dx;
    int tryct;
    int mon;
    int created = 0;
    int pmon;
    for (i = 0; i < how_many; i++)
    {
	for (tryct = 0; tryct < 20; tryct++)
	{
	    dy = zero_die(3) - 1;
	    dx = zero_die(3) - 1;
	    if ((terrain[y + dy][x + dx] == FLOOR) &&
		(mapmonster[y + dy][x + dx] == -1) &&
		((y + dy != u.y) || (x + dx != u.x)))
	    {
		pmon = get_random_pmon();
		if (pmon_is_magician(pmon))
		{
		    /* Never summon magicians! */
		    continue;
		}
		mon = create_mon(-1, y + dy, x + dx);
		if (mon != -1)
		{
		    created++;
		    break;
		}
	    }
	}
    }
    if (created > 0)
    {
	map_updated = 1;
	display_update();
    }
    return created;
}

int ood(int power, int ratio)
{
    return (depth - power + ratio - 1) / ratio;
}

int get_random_pmon(void)
{
    int tryct;
    int pm;
    for (tryct = 0; tryct < 200; tryct++)
    {
	pm = zero_die(PM_REAL_COUNT);
	if (reject_mon(pm))
	{
	    pm = -1;
	    continue;
	}
	break;
    }
    return pm;
}

int create_mon(int pm_idx, int y, int x)
{
    int mon;
    if (mapmonster[y][x] != -1)
    {
	print_msg("Attempt to create monster at occupied space %d %d\n", y, x);
	return -1;
    }
    if (pm_idx == -1)
    {
	pm_idx = get_random_pmon();
	if (pm_idx == -1)
	{
	    return -1;
	}
    }
    for (mon = 0; mon < 100; mon++)
    {
	if (monsters[mon].used == 0)
	{
	    monsters[mon].mon_id = pm_idx;
	    monsters[mon].used = 1;
	    monsters[mon].y = y;
	    monsters[mon].x = x;
	    monsters[mon].ai_lasty = -1;
	    monsters[mon].ai_lastx = -1;
	    monsters[mon].hpmax = permons[pm_idx].hp + ood(permons[pm_idx].power, 1);
	    monsters[mon].hpcur = monsters[mon].hpmax;
	    monsters[mon].mtohit = permons[pm_idx].mtohit + ood(permons[pm_idx].power, 3);
	    monsters[mon].defence = permons[pm_idx].defence + ood(permons[pm_idx].power, 3);
	    monsters[mon].mdam = permons[pm_idx].mdam + ood(permons[pm_idx].power, 5);
	    if (permons[pm_idx].rdam != -1)
	    {
		monsters[mon].rtohit = permons[pm_idx].rtohit + ood(permons[pm_idx].power, 3);
		monsters[mon].rdam = permons[pm_idx].rdam + ood(permons[pm_idx].power, 5);
	    }
	    else
	    {
		monsters[mon].rtohit = -1;
		monsters[mon].rdam = -1;
	    }
	    monsters[mon].awake = 0;
	    mapmonster[y][x] = mon;
	    newsym(y, x);
	    return mon;
	}
    }
    return -1;
}

void death_drop(int mon)
{
    int pm = monsters[mon].mon_id;
    int y = monsters[mon].y;
    int x = monsters[mon].x;
    int dy, dx;
    int tryct;
    tryct=0;
    while (((mapobject[y][x] != -1) || (terrain[y][x] != FLOOR)) && tryct < 100)
    {
	dy = zero_die(3) - 1;
	dx = zero_die(3) - 1;
	tryct++;
	y += dy;
	x += dx;
    }
    if (tryct >= 100)
    {
	return;
    }
    switch (pm)
    {
    case PM_GOBLIN:
	if (!zero_die(4))
	{
	    create_obj(PO_DAGGER, 1, 0, y, x);
	}
	break;
    case PM_THUG:
    case PM_GOON:
	if (!zero_die(4))
	{
	    create_obj(PO_MACE, 1, 0, y, x);
	}
	else if (!zero_die(3))
	{
	    create_obj(PO_LEATHER_ARMOUR, 1, 0, y, x);
	}
	break;
    case PM_HUNTER:
	if (!zero_die(6))
	{
	    create_obj(PO_BOW, 1, 0, y, x);
	}
	break;
    case PM_DUELLIST:
	if (!zero_die(6))
	{
	    create_obj(PO_LONG_SWORD, 1, 0, y, x);
	}
	break;
    case PM_WIZARD:
	if (!zero_die(4))
	{
	    create_obj_class(POCLASS_SCROLL, 1, 0, y, x);
	}
	else if (!zero_die(3))
	{
	    create_obj_class(POCLASS_POTION, 1, 0, y, x);
	}
        break;
    case PM_WARLORD:
	if (!zero_die(3))
	{
	    create_obj(PO_RUNESWORD, 1, 0, y, x);
	}
        break;
    default:
	break;
    }
    map_updated = 1;
}

int mon_can_pass(int mon, int y, int x)
{
    if ((y < 0) || (x < 0) || (y >= DUN_HEIGHT) || (x >= DUN_WIDTH))
    {
	return 0;
    }
    if (mapmonster[y][x] != -1)
    {
	return 0;
    }
    if ((y == u.y) && (x == u.x))
    {
	/* Sanity check! */
	return 0;
    }
    if (monsters[mon].mon_id == PM_WRAITH)
    {
	/* Wraiths can walk through walls. */
	return 1;
    }
    if (terrain[y][x] == WALL)
    {
	return 0;
    }
    return 1;
}

void print_mon_name(int mon, int article)
{
    if (permons[monsters[mon].mon_id].name[0] == '\0')
    {
	print_msg("GROB THE VOID (%d)", monsters[mon].mon_id);
    }
    switch (article)
    {
    case 0:	/* a */
	print_msg("a%s %s", is_vowel(permons[monsters[mon].mon_id].name[0]) ? "n" : "", permons[monsters[mon].mon_id].name);
	break;
    case 1: /* the */
	print_msg("the %s", permons[monsters[mon].mon_id].name);
	break;
    case 2: /* A */
	print_msg("A%s %s", is_vowel(permons[monsters[mon].mon_id].name[0]) ? "n" : "", permons[monsters[mon].mon_id].name);
	break;
    case 3: /* The */
	print_msg("The %s", permons[monsters[mon].mon_id].name);
	break;
    }
}

void heal_mon(int mon, int amount, int cansee)
{
    if (amount > (monsters[mon].hpmax - monsters[mon].hpcur))
    {
	amount = monsters[mon].hpmax - monsters[mon].hpcur;
    }
    if (amount > 0)
    {
	if (cansee)
	{
	    print_mon_name(mon, 3);
	    print_msg(" looks healthier.\n");
	}
	monsters[mon].hpcur += amount;
    }
}

void damage_mon(int mon, int amount, int by_you)
{
    struct mon *mptr;
    mptr = monsters + mon;
    if (amount >= mptr->hpcur)
    {
	if (by_you)
	{
	    print_msg("You kill ");
	    if (mon_visible(mon))
	    {
		print_mon_name(mon, 1);
	    }
	    else
	    {
		print_msg("something");
	    }
	    print_msg("!\n");
	    gain_experience(permons[mptr->mon_id].exp);
	}
	else if (mon_visible(mon))
	{
	    print_mon_name(mon, 2);
	    print_msg(" dies.\n");
	}
	death_drop(mon);
	mapmonster[mptr->y][mptr->x] = -1;
	newsym(mptr->y, mptr->x);
	mptr->used = 0;
	map_updated = 1;
	display_update();
    }
    else
    {
	mptr->hpcur -= amount;
    }
}

int reject_mon(int pm)
{
    if ((permons[pm].power > depth) || (zero_die(100) < permons[pm].rarity))
    {
	return 1;
    }
    return 0;
}

int teleport_mon_to_you(int mon)
{
    int tryct;
    int dy, dx;
    int y, x;
    int success = 0;
    int roomnum;
    if (roomnums[u.y][u.x] == -1)
    {
	/* Player in corridor.  Try to teleport next to him. */
	for (tryct = 0; tryct < 40; tryct++)
	{
	    dy = zero_die(3) - 1;
	    dx = zero_die(3) - 1;
	    y = u.y + dy;
	    x = u.x + dx;
	    if (mon_can_pass(mon, y, x))
	    {
		success = 1;
		break;
	    }
	}
    }
    else
    {
	/* Player is in a room.  Try to teleport into same room. */
	roomnum = roomnums[u.y][u.x];
	for (tryct = 0; tryct < 200; tryct++)
	{
	    y = exclusive_flat(roombounds[roomnum][0], roombounds[roomnum][1]);
	    x = exclusive_flat(roombounds[roomnum][2], roombounds[roomnum][3]);
	    if (mon_can_pass(mon, y, x))
	    {
		success = 1;
		break;
	    }
	}
    }
    if (success)
    {
	move_mon(mon, y, x);
	print_mon_name(mon, 2);
	print_msg(" appears in a puff of smoke.\n");
	return 1;
    }
    return 0;
}

int teleport_mon(int mon)
{
    int rval = -1;
    int room_try;
    int cell_try;
    int room;
    int y, x;
    for (room_try = 0; room_try < MAX_ROOMS * 4; room_try++)
    {
	room = zero_die(MAX_ROOMS);
	for (cell_try = 0; cell_try < 200; cell_try++)
	{
	    y = exclusive_flat(roombounds[room][0], roombounds[room][1]);
	    x = exclusive_flat(roombounds[room][2], roombounds[room][3]);
	    if ((mapmonster[y][x] == -1) && (terrain[y][x] == FLOOR) && ((y != u.y) || (x != u.x)))
	    {
		move_mon(mon, y, x);
		rval = 0;
		break;
	    }
	}
    }
    return rval;
}

void move_mon(int mon, int y, int x)
{
    struct mon *mptr;
    if (!mon_can_pass(mon, y, x))
    {
	print_msg("Warning: monster attempted an invalid move.\n");
	return;
    }
    mptr = monsters + mon;
    if (mapmonster[mptr->y][mptr->x] != mon)
    {
	print_msg("Monster map array in disorder.\n");
	press_enter();
	return;
    }
    mapmonster[mptr->y][mptr->x] = -1;
    newsym(mptr->y, mptr->x);
    mptr->y = y;
    mptr->x = x;
    mapmonster[mptr->y][mptr->x] = mon;
    newsym(mptr->y, mptr->x);
    display_update();
}

void summon_demon_near(int y, int x)
{
    int y2, x2;
    int i;
    y2 = y - 1 + zero_die(3);
    x2 = x - 1 + zero_die(3);
    if ((terrain[y2][x2] == FLOOR) && (mapmonster[y2][x2] == -1) &&
	((y2 != u.y) || (x2 != u.x)))
    {
	i = create_mon(PM_DEMON, y2, x2);
	if (i != -1)
	{
	    print_msg("Another demon appears!\n");
	}
	else
	{
	    print_msg("You smell sulphurous fumes.\n");
	}
    }
}

int mon_visible(int mon)
{
    int dy, dx;
    if (monsters[mon].used == 0)
    {
	return 0;
    }
    dy = u.y - monsters[mon].y;
    dx = u.x - monsters[mon].x;
    if (((dy > -2) && (dy < 2) && (dx > -2) && (dx < 2)) ||
	((roomnums[u.y][u.x] != -1) && (roomnums[u.y][u.x] == roomnums[monsters[mon].y][monsters[mon].x])))
    {
	return 1;
    }
    else
    {
	return 0;
    }
}

void update_mon(int mon)
{
    int cansee;
    if (monsters[mon].hpcur < monsters[mon].hpmax)
    {
	cansee = mon_visible(mon);
	switch (monsters[mon].mon_id)
	{
	case PM_TROLL:
	    if (!(game_tick % 10))
	    {
		if (cansee)
		{
		    print_msg("The troll regenerates.\n");
		}
		heal_mon(mon, one_die(3) + 3, 0);
	    }
	    break;

	case PM_ZOMBIE:
	    /* Zombies don't recover from their injuries. */
	    break;

	default:
	    if (!(game_tick % 20))
	    {
		heal_mon(mon, 1, cansee);
	    }
	    break;
	}
    }
}

/* monsters.c */
