/* u.c */

#include "7drl.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct player u;

void recalc_defence(void)
{
	if (u.armour != -1)
	{
		u.defence = permobjs[objects[u.armour].obj_id].power;
		u.defence += u.agility / 5;
	}
	else
	{
		u.defence = u.agility / 5;
	}
	status_updated = 1;
	display_update();
}

int move_player(int dy, int dx)
{
	if ((u.y + dy < 0) || (u.y + dy >= DUN_SIZE) ||
	    (u.x + dx < 0) || (u.x + dx >= DUN_SIZE))
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
	u.y = y;
	u.x = x;
	if (roomnums[y][x] != -1)
	{
		for (y2 = roombounds[roomnums[y][x]][0]; y2 <= roombounds[roomnums[y][x]][1]; y2++)
		{
			for (x2 = roombounds[roomnums[y][x]][2]; x2 <= roombounds[roomnums[y][x]][3]; x2++)
			{
				mapflags[y2][x2] |= MAPFLAG_EXPLORED;
			}
		}
	}
	for (y2 = y - 1; y2 <= y + 1; y2++)
	{
		if ((y2 < 0) || (y2 >= DUN_SIZE))
		{
			continue;
		}
		for (x2 = x - 1; x2 <= x + 1; x2++)
		{
			if ((x2 < 0) || (x2 >= DUN_SIZE))
			{
				continue;
			}
			mapflags[y2][x2] |= MAPFLAG_EXPLORED;
		}
	}
	if (mapobject[y][x] != -1)
	{
		print_msg("You see here ");
		print_obj_name(mapobject[y][x]);
		print_msg(".\n");
	}
	map_updated = 1;
	status_updated = 1;
	display_update();
	return 0;
}

int drain_body(int amount, const char *what)
{
	print_msg("You feel weaker!\n");
	u.body -= amount;
	if (u.body < 0)
	{
		print_msg("Your heart is too weak to beat.\n");
		do_death(DEATH_BODY, what);
		return 1;
	}
	return 0;
}

int drain_agility(int amount, const char *what)
{
	print_msg("You feel clumsy!\n");
	u.agility -= amount;
	if (u.agility < 0)
	{
		print_msg("You forget how to breathe.\n");
		do_death(DEATH_AGILITY, what);
		return 1;
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
		do_death(d, what);
		return 1;
	}
	return 0;
}

void heal_u(int amount, int boost)
{
	u.hpcur += amount;
	if (u.hpcur > u.hpmax)
	{
		if (boost)
		{
			u.hpmax++;
		}
		u.hpcur = u.hpmax;
	}
	status_updated = 1;
	print_msg("You feel better.\n");
	return;
}

void do_death(enum death d, const char *what)
{
	print_msg("THOU ART SLAIN!\n");
	game_finished = 1;
	switch (d)
	{
	case DEATH_KILLED:
		print_msg("You were killed by %s.\n", what);
		break;
	case DEATH_KILLED_MON:
		print_msg("You were killed by a nasty %s.\n", what);
		break;
	case DEATH_BODY:
		print_msg("Your heart was stopped by %s.\n", what);
		break;
	case DEATH_AGILITY:
		print_msg("Your nerves were destroyed by %s.\n", what); 
		break;
	}
	print_msg("Your game lasted %d ticks.\n", game_tick);
	print_msg("You killed monsters worth %d experience.\n", u.experience);
	print_msg("You found %d pieces of gold.\n", u.gold);
}

void u_init(void)
{
	u.name[16] = '\0';
	print_msg("What is your name, stranger?\n");
	read_input(u.name, 16);
	u.body = 10;
	u.agility = 10;
	u.hpmax = 20;
	u.hpcur = 20;
	u.experience = 0;
	u.level = 1;
	memset(u.inventory, -1, sizeof u.inventory);
	u.inventory[0] = create_obj(PO_DAGGER, 1, 1, -1, -1);
	if (u.inventory[0] == -1)
	{
		print_msg("Couldn't create dagger!\n");
	}
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
		return 10000 * (level - 10);
	}
	if (level < 30)
	{
		return 100000 * (level - 20);
	}
	return INT_MAX;
}

void gain_experience(int amount)
{
	int hpgain;
	u.experience += amount;
	status_updated = 1;
	if (u.experience > lev_threshold(u.level))
	{
		u.level++;
		print_msg("You gained a level!\n");
		u.body++;
		u.agility++;
		if (random() & 1)
		{
			u.body++;
			status_updated = 1;
			print_msg("You gained 2 body and 1 agility.\n");
		}
		else
		{
			u.agility++;
			status_updated = 1;
			print_msg("You gained 1 body and 2 agility.\n");
		}
		hpgain = u.body / 10 + 10;
		if (u.hpmax + hpgain > 999)
		{
			hpgain = 999 - u.hpmax;
		}
		if (hpgain > 0)
		{
			u.hpmax += hpgain;
			status_updated = 1;
			print_msg("You gained %d hit points.\n", hpgain);
		}
	}
	else
	{
		display_update();
	}
}

void teleport_u(void)
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
			y = exclusive_flat(roombounds[MAX_ROOMS][0], roombounds[MAX_ROOMS][1]);
			x = exclusive_flat(roombounds[MAX_ROOMS][2], roombounds[MAX_ROOMS][3]);
			if ((mapmonster[y][x] == -1) && (terrain[y][x] == FLOOR))
			{
				print_msg("You are whisked away!\n");
				reloc_player(y, x);
				return;
			}
		}
	}
	print_msg("You feel briefly dislocated.\n");
	return;
}

/* u.c */
