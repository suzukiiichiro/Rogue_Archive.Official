/* u.c
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
#include <limits.h>
#include <string.h>
#include <stdio.h>

struct player u;

void recalc_defence(void)
{
    int i;
    for (i = 0; i < DT_COUNT; i++)
    {
	u.resistances[i] &= RESIST_MASK_TEMPORARY;
    }
    u.speed = (u.leadfoot ? 0 : 1);
    if (u.armour != -1)
    {
	u.defence = u.armourmelt ? 0 : permobjs[objects[u.armour].obj_id].power;
	u.defence += u.withering ? (u.agility / 10) : (u.agility / 5);
	switch (objects[u.armour].obj_id)
	{
	case PO_DRAGON_ARMOUR:
	case PO_METEOR_ARMOUR:
	    u.resistances[DT_FIRE] |= RESIST_ARMOUR;
	    break;
	case PO_ROBE_SWIFTNESS:
	    u.speed++;
	    break;
	default:
	    break;
	}
    }
    else
    {
	u.defence = u.withering ? (u.agility / 10) : (u.agility / 5);
    }
    if (u.ring != -1)
    {
	switch (objects[u.ring].obj_id)
	{
	case PO_RING_FIRE:
	    u.resistances[DT_FIRE] |= RESIST_RING;
	    break;
	case PO_RING_FROST:
	    u.resistances[DT_COLD] |= RESIST_RING;
	    break;
	case PO_RING_VAMPIRE:
	    u.resistances[DT_NECRO] |= RESIST_RING;
	    break;
	}
    }
    status_updated = 1;
    display_update();
}

int move_player(int dy, int dx)
{
    if ((u.y + dy < 0) || (u.y + dy >= DUN_HEIGHT) ||
	(u.x + dx < 0) || (u.x + dx >= DUN_WIDTH))
    {
	print_msg("Attempted move out of bounds.\n");
	return 0;	/* No movement. */
    }
    if (mapmonster[u.y + dy][u.x + dx] != -1)
    {
	if (u.weapon != -1)
	{
	    if ((objects[u.weapon].obj_id == PO_BOW) ||
		(objects[u.weapon].obj_id == PO_CROSSBOW))
	    {
		print_msg("You can't use that weapon in melee!\n");
		return 0;
	    }
	}
	return player_attack(dy, dx);
    }
    switch (terrain[u.y + dy][u.x + dx])
    {
    case WALL:
	print_msg("You cannot go there.\n");
	return 0;
    case FLOOR:
    case DOOR:
    case STAIRS:
	reloc_player(u.y + dy, u.x + dx);
	return 1;
    }
    return 0;
}

int reloc_player(int y, int x)
{
    int y2, x2;
    int oy, ox;

    oy = u.y;
    ox = u.x;
    u.y = y;
    u.x = x;
    newsym(oy, ox);
    newsym(u.y, u.x);
    if ((roomnums[oy][ox] != roomnums[y][x]) &&
	(roomnums[oy][ox] != -1))
    {
	for (y2 = roombounds[roomnums[oy][ox]][0]; y2 <= roombounds[roomnums[oy][ox]][1]; y2++)
	{
	    for (x2 = roombounds[roomnums[oy][ox]][2]; x2 <= roombounds[roomnums[oy][ox]][3]; x2++)
	    {
		newsym(y2, x2);
	    }
	}
    }
    if (roomnums[y][x] != -1)
    {
	for (y2 = roombounds[roomnums[y][x]][0]; y2 <= roombounds[roomnums[y][x]][1]; y2++)
	{
	    for (x2 = roombounds[roomnums[y][x]][2]; x2 <= roombounds[roomnums[y][x]][3]; x2++)
	    {
		if (!(mapflags[y2][x2] & MAPFLAG_EXPLORED))
		{
		    mapflags[y2][x2] |= MAPFLAG_EXPLORED;
		}
		newsym(y2, x2);
	    }
	}
    }
    for (y2 = y - 1; y2 <= y + 1; y2++)
    {
	if ((y2 < 0) || (y2 >= DUN_HEIGHT))
	{
	    continue;
	}
	for (x2 = x - 1; x2 <= x + 1; x2++)
	{
	    if ((x2 < 0) || (x2 >= DUN_WIDTH))
	    {
		continue;
	    }
	    if (!(mapflags[y2][x2] & MAPFLAG_EXPLORED))
	    {
		mapflags[y2][x2] |= MAPFLAG_EXPLORED;
	    }
	    newsym(y2, x2);
	}
    }
    map_updated = 1;
    status_updated = 1;
    if (mapobject[y][x] != -1)
    {
	print_msg("You see here ");
	print_obj_name(mapobject[y][x]);
	print_msg(".\n");
    }
    display_update();
    return 0;
}

int gain_body(int amount, int loud)
{
    if (amount < 1)
    {
	print_msg("Absurd body gain %d\n", amount);
    }
    if (u.body < 99)
    {
	if (u.body + amount > 99)
	{
	    amount = 99 - u.body;
	}
	u.body += amount;
	status_updated = 1;
	if (loud)
	{
	    print_msg("You feel stronger!\n");
	}
	else
	{
	    display_update();
	}
	return amount;
    }
    else
    {
	print_msg("You feel disappointed.\n");
	return 0;
    }
}

int drain_body(int amount, const char *what, int permanent)
{
    print_msg("You feel weaker!\n");
    if (permanent)
    {
	u.body -= amount;
    }
    else
    {
	u.bdam += amount;
    }
    status_updated = 1;
    if ((u.body - u.bdam) < 0)
    {
	print_msg("Your heart is too weak to beat.\n");
	return do_death(DEATH_BODY, what);
    }
    display_update();
    return 0;
}

int gain_agility(int amount, int loud)
{
    if (amount < 1)
    {
	print_msg("Absurd agility gain %d\n", amount);
    }
    if (u.agility < 99)
    {
	if (u.agility + amount > 99)
	{
	    amount = 99 - u.agility;
	}
	u.agility += amount;
	status_updated = 1;
	recalc_defence();
	if (loud)
	{
	    print_msg("You feel more agile!\n");
	}
	else
	{
	    display_update();
	}
	return amount;
    }
    else
    {
	print_msg("You feel disappointed.\n");
	return 0;
    }
}

int drain_agility(int amount, const char *what, int permanent)
{
    print_msg("You feel clumsy!\n");
    if (permanent)
    {
	u.agility -= amount;
    }
    else
    {
	u.adam += amount;
    }
    status_updated = 1;
    if ((u.agility - u.adam) < 0)
    {
	print_msg("You forget how to breathe.\n");
	return do_death(DEATH_AGILITY, what);
    }
    recalc_defence();
    return 0;
}

int damage_u(int amount, enum death d, const char *what)
{
    u.hpcur -= amount;
    status_updated = 1;
    if (u.hpcur < 0)
    {
	u.hpcur = 0;
	return do_death(d, what);
    }
    return 0;
}

void heal_u(int amount, int boost, int loud)
{
    if (u.hpcur + amount > u.hpmax)
    {
	if (boost)
	{
	    u.hpmax++;
	}
	amount = u.hpmax - u.hpcur;
    }
    u.hpcur += amount;
    /* Touch the status line */
    status_updated = 1;
    if (loud)
    {
	/* Tell the player how much better he feels. */
	if (u.hpcur == u.hpmax)
	{
	    print_msg("You feel great.\n");
	}
	else
	{
	    print_msg("You feel %sbetter.\n", amount > 10 ? "much " : "");
	}
    }
    else
    {
	/* Update the display. */
	display_update();
    }
    return;
}

int do_death(enum death d, const char *what)
{
    FILE *fp;
	  fp = fopen("dunbash.log", "a");
    int really = 0;

    if (wizard_mode)
    {
	really = getyn("Really die? ");
	if (really != 1)
	{
	    u.hpcur = u.hpmax;
	    u.adam = 0;
	    u.bdam = 0;
	    status_updated = 1;
	    print_msg("You survived that attempt on your life.");
	    return 0;
	}
    }
    if (!wizard_mode)
    {
	fp = fopen("dunbash.log", "a");
    }
    fprintf(fp, "%s, ", u.name);
    print_msg("THOU ART SLAIN!\n");
    game_finished = 1;
    switch (d)
    {
    case DEATH_KILLED:
	print_msg("You were killed by %s.\n", what);
	if (!wizard_mode)
	{
	    fprintf(fp, "you were killed by %s.\n", what);
	}
	break;
    case DEATH_KILLED_MON:
	print_msg("You were killed by a nasty %s.\n", what);
	if (!wizard_mode)
	{
	    fprintf(fp, "you were killed by a nasty %s.\n", what);
	}
	break;
    case DEATH_BODY:
	print_msg("Your heart was stopped by %s.\n", what);
	if (!wizard_mode)
	{
	    fprintf(fp, "your heart was stopped by %s.\n", what);
	}
	break;
    case DEATH_AGILITY:
	print_msg("Your nerves were destroyed by %s.\n", what); 
	if (!wizard_mode)
	{
	    fprintf(fp, "your nerves were destroyed by %s.\n", what); 
	}
	break;
    }
    if (!wizard_mode)
    {
	fprintf(fp, "You died after %d ticks, with %d XP and %d gold, on dungeon level %d.\n\n", game_tick, u.experience, u.gold, depth);
	fflush(fp);
	fclose(fp);
    }
    print_msg("Your game lasted %d ticks.\n", game_tick);
    print_msg("You killed monsters worth %d experience.\n", u.experience);
    print_msg("You found %d pieces of gold.\n", u.gold);

    return 1;
}

void write_char_dump(void)
{
    FILE *fp;
    char filename[32];
    int i;
    snprintf(filename, 31, "%s.dump", u.name);
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
	print_msg("Couldn't create dump file. Dump failed.\n");
	return;
    }
    fprintf(fp, "%s, level %d adventurer (%d XP)\n", u.name, u.level, u.experience);
    fprintf(fp, "%d gold pieces collected.\n", u.gold);
    fprintf(fp, "%d of %d hit points.\n", u.hpcur, u.hpmax);
    fprintf(fp, "Body %d (%d damage).\n", u.body, u.bdam);
    fprintf(fp, "Agility %d (%d damage).\n", u.agility, u.adam);
    fprintf(fp, "Defence %d.\n", u.defence);
    fprintf(fp, "Inventory:\n");
    for (i = 0; i < 19; i++)
    {
	if (u.inventory[i] != -1)
	{
	    fprint_obj_name(fp, u.inventory[i]);
	    fputc('\n', fp);
	}
    }
    fflush(fp);
    fclose(fp);
}

void u_init(void)
{
    char * hasslash = NULL;
    u.name[16] = '\0';
    do {
	print_msg("What is your name, stranger?\n");
	read_input(u.name, 16);
	hasslash = strchr(u.name, '/');
	/* Now that we create a named dump file, we must not
	 * permit the player's name to contain a slash, colon,
	 * or backslash. */
	if (hasslash)
	{
	    print_msg("No slashes permitted.\n");
	    continue;
	}
	hasslash = strchr(u.name, '\\');
	if (hasslash)
	{
	    print_msg("No backslashes permitted.\n");
	    continue;
	}
	hasslash = strchr(u.name, ':');
	if (hasslash)
	{
	    print_msg("No colons permitted.\n");
	    continue;
	}
    } while (hasslash != NULL);
    u.body = 10;
    u.bdam = 0;
    u.agility = 10;
    u.adam = 0;
    u.hpmax = 20;
    u.hpcur = 20;
    u.experience = 0;
    u.level = 1;
    u.food = 2000;
    memset(u.inventory, -1, sizeof u.inventory);
    u.inventory[0] = create_obj(PO_DAGGER, 1, 1, -1, -1);
    if (u.inventory[0] == -1)
    {
	print_msg("Couldn't create dagger!\n");
    }
    u.inventory[1] = create_obj(PO_IRON_RATION, 1, 1, -1, -1);
    u.weapon = u.inventory[0];
    u.ring = -1;
    u.armour = -1;
    recalc_defence();
}

int lev_threshold(int level)
{
    if (level < 10)
    {
	return 20 * (1 << (level - 1));
    }
    if (level < 20)
    {
	return 10000 * (level - 9);
    }
    if (level < 30)
    {
	return 100000 * (level - 18);
    }
    return INT_MAX;
}

void gain_experience(int amount)
{
    int hpgain;
    int bodygain;
    int agilgain;
    u.experience += amount;
    status_updated = 1;
    if (u.experience > lev_threshold(u.level))
    {
	u.level++;
	print_msg("You gained a level!\n");
	if (!zero_die(2))
	{
	    bodygain = gain_body(2, 0);
	    agilgain = gain_agility(1, 0);
	}
	else
	{
	    bodygain = gain_body(1, 0);
	    agilgain = gain_agility(2, 0);
	}
	print_msg("You gained %d body and %d agility.\n", bodygain, agilgain);
	hpgain = u.body / 10 + 10;
	if (u.hpmax + hpgain > 999)
	{
	    hpgain = 999 - u.hpmax;
	}
	if (hpgain > 0)
	{
	    /* v1.3: Policy change - gaining a level effectively
	     * heals you. */
	    u.hpcur += hpgain;
	    u.hpmax += hpgain;
	    status_updated = 1;
	    print_msg("You gained %d hit points.\n", hpgain);
	}
	press_enter();
    }
    else
    {
	display_update();
    }
}

int teleport_u(void)
{
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
		print_msg("You are whisked away!\n");
		reloc_player(y, x);
		return 0;
	    }
	}
    }
    print_msg("You feel briefly dislocated.\n");
    return -1;
}

void update_player(void)
{
    if (!(game_tick % 5) && (u.food >= 0) && (u.hpcur < u.hpmax))
    {
	/* Heal player for one hit point; do not allow HP gain,
	 * and don't say anything. */
	heal_u(1, 0, 0);
    }
    else if (!(game_tick % 60) && (u.hpcur < u.hpmax * 3 / 4))
    {
	/* Hungry player heals much, much slower, and cannot regain
	 * all their hit points. */
	heal_u(1, 0, 0);
    }
    /* Once you hit the nutrition endstop, your ring of regeneration stops
     * working, and like normal regen, it won't raise you above 75% HP if
     * your food counter is negative. */
    if (((game_tick % 10) == 5) &&
	(objects[u.ring].obj_id == PO_RING_REGEN) &&
	(u.hpcur < ((u.food >= 0) ? u.hpmax : ((u.hpmax * 3) / 4))) &&
	(u.food >= -1950))
    {
	/* Heal player for 1d3 hit points; do not allow HP gain,
	 * and don't say anything apart from the regen ring message. */
	print_msg("Your ring pulses soothingly.\n");
	heal_u(one_die(3), 0, 0);
	permobjs[PO_RING_REGEN].known = 1;
    }
    if (u.food >= -1950)
    {
	int food_use = 1;
	int squeal = 0;
	if ((objects[u.ring].obj_id == PO_RING_REGEN) && !(game_tick % 2) && (u.food >= -1950))
	{
	    /* If you are still less hungry than -1950 nutrition,
	     * use one more food every second tick if you are
	     * wearing a ring of regeneration. */
	    food_use++;
	}
	if ((u.food >= 100) && (u.food - food_use < 100))
	{
	    squeal = 1;
	}
	if ((u.food >= 0) && (u.food < food_use))
	{
	    squeal = 2;
	}
	u.food -= food_use;
	status_updated = 1;
	switch (squeal)
	{
	case 0:
	default:
	    break;
	case 1:
	    print_msg("You are getting quite hungry.\n");
	    break;
	case 2:
	    print_msg("You are feeling hunger pangs, and will recover\nmore slowly from your injuries.\n");
	    break;
	}
    }
    if (u.leadfoot > 0)
    {
	u.leadfoot--;
	if (!u.leadfoot)
	{
	    print_msg("You shed your feet of lead.\n");
	    recalc_defence();
	}
    }
    if (u.armourmelt > 0)
    {
	u.armourmelt--;
	if (!u.armourmelt)
	{
	    print_msg("Your armour seems solid once more.\n");
	    recalc_defence();
	}
    }
    if (u.withering > 0)
    {
	u.withering--;
	if (!u.withering)
	{
	    print_msg("Your limbs straighten.\n");
	    recalc_defence();
	}
    }
    if (u.protection > 0)
    {
	u.protection--;
	if (!u.protection)
	{
	    print_msg("You feel like you are no longer being helped.\n");
	}
    }
    display_update();
}

int player_resists_dtype(enum damtyp dtype)
{
    return u.resistances[dtype];
}

/* u.c */
