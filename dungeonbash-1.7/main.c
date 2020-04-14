/* main.c
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
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

void save_game(void);
static void load_game(void);
static void new_game(void);
static void rebuild_mapobjs(void);
static void rebuild_mapmons(void);
static void main_loop(void);
int game_finished;
int game_tick;
int wizard_mode = WIZARD_MODE;

static void rebuild_mapmons(void)
{
    int i;
    for (i = 0; i < 100; i++)
    {
	if (monsters[i].used)
	{
	    mapmonster[monsters[i].y][monsters[i].x] = i;
	}
    }
}

static void rebuild_mapobjs(void)
{
    int i;
    for (i = 0; i < 100; i++)
    {
	if (objects[i].used && !objects[i].with_you)
	{
	    mapobject[objects[i].y][objects[i].x] = i;
	}
    }
}

unsigned int convert_range(int dy, int dx)
{
    int ady, adx;
    ady = dy > 0 ? dy : -dy;
    adx = dx > 0 ? dx : -dx;
    if (ady > adx)
    {
	return ady;
    }
    return adx;
}

void save_game(void)
{
    FILE *fp;
    fp = fopen("dunbash.sav", "wb");
    /* Write out the snapshot we took of the RNG before generating the
     * current level. */
    fwrite(saved_state, sizeof saved_state, 1, fp);
    /* Write out the map flags; if we decide to have at-generation
     * flags as well as at-exploration flags, the at-generation flags
     * will be stored in a separate array. */
    fwrite(mapflags, DUN_HEIGHT, DUN_WIDTH * sizeof (int), fp);
    /* Write out the permanent object data. This is hideously
     * wasteful (and guarantees savefile breakage at version-up), but
     * it makes handling flavours much easier... */
    /* FIX: We no longer write out the permons data, since none of it
     * changes at the moment. */
    fwrite(permobjs, 100, sizeof (struct permobj), fp);
    /* Write out the dynamic monster/object arrays. */
    fwrite(monsters, 100, sizeof (struct mon), fp);
    fwrite(objects, 100, sizeof (struct obj), fp);
    /* Write out the depth */
    fwrite(&depth, 1, sizeof depth, fp);
    /* Write out the player. */
    fwrite(&u, 1, sizeof u, fp);
    /* Write out the tick number */
    fwrite(&game_tick, 1, sizeof game_tick, fp);
    /* Clean up */
    fflush(fp);
    fclose(fp);
    /* Compress! */
    system("gzip dunbash.sav");
    game_finished = 1;
    return;
}

void load_game(void)
{
    FILE *fp;
    system("gunzip dunbash.sav");
    fp = fopen("dunbash.sav", "rb");
    fread(rng_state, sizeof rng_state, 1, fp);
    room_reset();
    build_level();
    fread(mapflags, DUN_HEIGHT, DUN_WIDTH * sizeof (int), fp);
    fread(permobjs, 100, sizeof (struct permobj), fp);
    fread(monsters, 100, sizeof (struct mon), fp);
    fread(objects, 100, sizeof (struct obj), fp);
    rebuild_mapobjs();
    rebuild_mapmons();
    fread(&depth, 1, sizeof depth, fp);
    fread(&u, 1, sizeof u, fp);
    fread(&game_tick, 1, sizeof game_tick, fp);
    fclose(fp);
    unlink("dunbash.sav");
    touch_back_buffer();
    status_updated = 1;
    map_updated = 1;
    hard_redraw = 1;
    print_msg("Game loaded.\n");
}

int exclusive_flat(int lower, int upper)
{
    return lower + one_die(upper - lower - 1);
}

int inclusive_flat(int lower, int upper)
{
    return lower + zero_die(upper - lower + 1);
}

int one_die(int sides)
{
    int rval;
    if (sides < 2)
    {
	return 1;
    }
    rval = 1 + (rng() / ((RNG_MAX / sides) + 1));
    return rval;
}

int zero_die(int sides)
{
    int rval;
    if (sides < 2)
    {
	return 0;
    }
    rval = rng() / ((RNG_MAX / sides) + 1);
    return rval;
}

int dice(int count, int sides)
{
    int total = 0;
    for ( ; count > 0; count--)
    {
	total += one_die(sides);
    }
    return total;
}

void new_game(void)
{
    rng_init();
    u_init();
    flavours_init();
    make_new_level();
    status_updated = 1;
    map_updated = 1;
    hard_redraw = 1;
    print_msg("Initialisation complete.\n");
}

int do_command(enum game_cmd cmd)
{
    int i;
    int j;
    int sy, sx;
    switch (cmd)
    {
    case MOVE_NORTH:
	return move_player(-1, 0);
    case MOVE_SOUTH:
	return move_player(1, 0);
    case MOVE_EAST:
	return move_player(0, 1);
    case MOVE_WEST:
	return move_player(0, -1);
    case MOVE_NW:
	return move_player(-1, -1);
    case MOVE_NE:
	return move_player(-1, 1);
    case MOVE_SE:
	return move_player(1, 1);
    case MOVE_SW:
	return move_player(1, -1);

    case ATTACK:
	i = select_dir(&sy, &sx);
	if (i != -1)
	{
	    return player_attack(sy, sx);
	}
	return 0;

    case GET_ITEM:
	if (mapobject[u.y][u.x] != -1)
	{
	    attempt_pickup();
	    return 1;
	}
	else
	{
	    print_msg("Nothing to get.\n");
	    return 0;
	}

    case WIELD_WEAPON:
	j = 0;
	i = inv_select(POCLASS_WEAPON, "wield", 1);
	if (i == -2)
	{
	    u.weapon = -1;
	    print_msg("Weapon unwielded.\n");
	}
	else if (i >= 0)
	{
	    u.weapon = u.inventory[i];
	    j = 1;
	    print_msg("Wielding ");
	    print_obj_name(u.weapon);
	    print_msg(".\n");
	}
	return j;

    case WEAR_ARMOUR:
	if (u.armour >= 0)
	{
	    print_msg("You are already wearing armour.\n");
	    return 0;
	}
	i = inv_select(POCLASS_ARMOUR, "wear", 0);
	if (i >= 0)
	{
	    u.armour = u.inventory[i];
	    permobjs[objects[u.armour].obj_id].known = 1;
	    recalc_defence();
	    print_msg("Wearing ");
	    print_obj_name(u.armour);
	    print_msg(".\n");
	    return 1;
	}
	return 0;
    case TAKE_OFF_ARMOUR:
	if (u.armour != -1)
	{
	    u.armour = -1;
	    recalc_defence();
	    print_msg("You take off your armour.\n");
	    return 1;
	}
	else
	{
	    print_msg("YOu aren't wearing any armour.\n");
	    return 0;
	}

    case GIVE_HELP:
	print_help();
	return 0;

    case READ_SCROLL:
	i = inv_select(POCLASS_SCROLL, "read", 0);
	if (i >= 0)
	{
	    j = read_scroll(u.inventory[i]);
	    if (j)
	    {
		u.inventory[i] = -1;
	    }
	    return 1;
	}
	return 0;

    case GO_DOWN_STAIRS:
	if (terrain[u.y][u.x] == STAIRS)
	{
	    leave_level();
	    make_new_level();
	}
	else
	{
	    print_msg("There are no stairs here.\n");
	}
	return 0;

    case STAND_STILL:
	return 1;

    case EAT_FOOD:
	i = inv_select(POCLASS_FOOD, "eat", 0);
	if (i >= 0)
	{
	    j = eat_food(u.inventory[i]);
	    if (j == -1)
	    {
		u.inventory[i] = -1;
	    }
            return 1;
	}
	return 0;

    case QUAFF_POTION:
	i = inv_select(POCLASS_POTION, "quaff", 0);
	if (i >= 0)
	{
	    j = quaff_potion(u.inventory[i]);
	    if (j)
	    {
		u.inventory[i] = -1;
	    }
	    return 1;
	}
	return 0;

    case REMOVE_RING:
	if (u.ring == -1)
	{
	    print_msg("You have no ring to remove!\n");
	    return 0;
	}
	if (objects[u.ring].obj_id == PO_RING_DOOM)
	{
	    int dmg;
	    print_msg("You remove your ring.\n");
	    print_msg("It exacts vengeance!\n");
	    drain_body(one_die(4), "a ring of doom", 1);
	    drain_agility(one_die(4), "a ring of doom", 1);
	    dmg = one_die(20);
	    damage_u(dmg, DEATH_KILLED, "a ring of doom");
	    u.hpmax -= dmg;
	    status_updated = 1;
	    u.ring = -1;
	    display_update();
	}
	else if (objects[u.ring].obj_id == PO_RING_TELEPORT)
	{
	    i = zero_die(u.level);
	    if (i < 4)
	    {
		print_msg("You lack the willpower to remove it.\n");
	    }
	    else
	    {
		print_msg("You manage to pull the ring off.\n");
		u.ring = -1;
	    }
	}
	else
	{
	    print_msg("You remove your ring.\n");
	    u.ring = -1;
	}
	return 1;
    case PUT_ON_RING:
	if (u.ring != -1)
	{
	    print_msg("You are already wearing a ring.\n");
	    return 0;
	}
	i = inv_select(POCLASS_RING, "put on", 0);
	if (i >= 0)
	{
	    u.ring = u.inventory[i];
	    print_msg("You put on ");
	    print_obj_name(u.ring);
	    print_msg(".\n");
	    return 1;
	}
	return 0;
    case INSPECT_ITEM:
	i = inv_select(POCLASS_NONE, "inspect", 0);
	if ((i >= 0) && (u.inventory[i] != -1))
	{
	    describe_object(u.inventory[i]);
	}
	return 0;
    case EXAMINE_MONSTER:
	print_msg("Monster examination not implemented yet.\n");
	return 0;
    case SHOW_DISCOVERIES:
	show_discoveries();
	return 0;
    case SHOW_TERRAIN:
	show_terrain = 1;
	map_updated = 1;
	display_update();
	print_msg("Display of monsters and objects suppressed.\n");
	press_enter();
	show_terrain = 0;
	map_updated = 1;
	display_update();
	return 0;
    case RNG_TEST:
	{
	    int odds = 0;
	    int evens = 0;
	    for (i = 0; i < 100000; i++)
	    {
		if (zero_die(2))
		{
		    odds++;
		}
		else
		{
		    evens++;
		}
	    }
	    print_msg("100k rolls: 0 %d, 1 %d\n", odds, evens);
	}
	print_msg("1d2-1: %d %d %d %d %d %d %d %d\n", zero_die(2), zero_die(2), zero_die(2), zero_die(2), zero_die(2), zero_die(2), zero_die(2), zero_die(2));
	print_msg("1d8-1: %d %d %d %d %d %d %d %d\n", zero_die(8), zero_die(8), zero_die(8), zero_die(8), zero_die(8), zero_die(8), zero_die(8), zero_die(8));
	print_msg("1d32-1: %d %d %d %d %d %d %d %d\n", zero_die(32), zero_die(32), zero_die(32), zero_die(32), zero_die(32), zero_die(32), zero_die(32), zero_die(32));
	return 0;
    case DROP_ITEM:
	if (mapobject[u.y][u.x] != -1)
	{
	    print_msg("There is already an item here.\n");
	    return 0;
	}
	i = inv_select(POCLASS_NONE, "drop", 0);
	if (i >= 0)
	{
	    if ((u.inventory[i] != -1) &&
		((u.inventory[i] == u.ring) ||
		 (u.inventory[i] == u.armour)))
	    {
		print_msg("You cannot drop something you are wearing.\n");
		return 0;
	    }
	    j = drop_obj(i);
	    if (j == -1)
	    {
		return 0;
	    }
	    return 1;
	}
	return 0;
    case DUMP_CHARA:
	write_char_dump();
	return 0;
    case SAVE_GAME:
	game_finished = 1;
	save_game();
	return 0;
    case QUIT:
	j = getYN("Really quit?\n");
	if (j > 0)
	{
	    game_finished = 1;
	}
	else
	{
	    print_msg("Never mind.\n");
	}
	return 0;
    case SHOW_INVENTORY:
	print_msg("You are carrying:\n");
	print_inv(POCLASS_NONE);
	return 0;

    case WIZARD_DESCEND:
	if (wizard_mode)
	{
	    leave_level();
	    make_new_level();
	}
	else
	{
	    print_msg("You aren't a wizard.\n");
	}
	return 0;

    case WIZARD_LEVELUP:
	if (wizard_mode)
	{
	    gain_experience((lev_threshold(u.level) - u.experience) + 1);
	}
	else
	{
	    print_msg("You aren't a wizard.\n");
	}
	return 0;
    }
    return 0;
}

void main_loop(void)
{
    enum game_cmd cmd;
    int i;
    int action_speed;
    print_msg("Welcome to Martin's Infinite Dungeon.\n");
    print_msg("Press '?' for help.\n");
    while (!game_finished)
    {
	switch (game_tick % 4)
	{
	case 0:
	case 2:
	    action_speed = 0;
	    break;
	case 1:
	    action_speed = 1;
	    break;
	case 3:
	    action_speed = 2;
	    break;
	}
	/* Player is always speed 1, for now. */
	if (action_speed <= u.speed)
	{
	    i = 0;
	    while (!i)
	    {
		/* Take commands until the player does
		 * something that uses an action. */
		cmd = get_command();
		i = do_command(cmd);
		if (game_finished)
		{
		    break;
		}
	    }
	    if (game_finished)
	    {
		break;
	    }
	    /* If you're wearing a ring of doom, zap you. */
	    if (objects[u.ring].obj_id == PO_RING_DOOM)
	    {
		print_msg("Your ring pulses uncleanly.\n");
		damage_u(1, DEATH_KILLED, "a ring of doom");
		display_update();
		permobjs[PO_RING_DOOM].known = 1;
	    }
	    else if (objects[u.ring].obj_id == PO_RING_TELEPORT)
	    {
		if (!zero_die(75))
		{
		    print_msg("Your ring flares white!\n");
		    permobjs[PO_RING_TELEPORT].known = 1;
		    teleport_u();
		}
	    }
	}
	for (i = 0; i < 100; i++)
	{
	    if (monsters[i].used == 0)
	    {
		/* Unused monster */
		continue;
	    }
	    /* Update the monster's status. */
	    update_mon(i);
	    if (action_speed <= permons[monsters[i].mon_id].speed)
	    {
		mon_acts(i);
	    }
	    if (game_finished)
	    {
		break;
	    }
	}
	if (game_finished)
	{
	    break;
	}
	update_player();
	game_tick++;
    }
}

int main(void)
{
    struct stat s;
    int i;
    display_init();
    /* Do we have a saved game? */
    i = stat("dunbash.sav.gz", &s);
    if (!i)
    {
	/* Yes! */
	print_msg("Loading...\n");
	load_game();
    }
    else
    {
	/* No! */
	new_game();
    }
    main_loop();
    display_shutdown();
    return 0;
}

/* main.c */
