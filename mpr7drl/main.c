#include "7drl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

void save_game(void);
static void load_game(void);
static void new_game(void);
static void rebuild_mapobjs(void);
static void rebuild_mapmons(void);
static void rnum_init(void);
static void main_loop(void);
char running_state[256];
char levgen_saved_state[256];
int game_finished;
int game_tick;
int do_command(enum game_cmd cmd);

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

void save_game(void)
{
	FILE *fp;
	fp = fopen("7drl.sav", "wb");
	/* Write out the snapshot we took of the RNG before generating the
	 * current level. */
	fwrite(levgen_saved_state, 256, 1, fp);
	/* Write out the map flags; if we decide to have at-generation
	 * flags as well as at-exploration flags, the at-generation flags
	 * will be stored in a separate array. */
	fwrite(mapflags, DUN_SIZE, DUN_SIZE * sizeof (int), fp);
	/* Write out the permanent monster/object data. This is hideously
	 * wasteful, but it makes handling flavours much easier... */
	fwrite(permons, 100, sizeof (struct permon), fp);
	fwrite(permobjs, 100, sizeof (struct permobj), fp);
	/* Write out the dynamic monster/object arrays. */
	fwrite(monsters, 100, sizeof (struct mon), fp);
	fwrite(objects, 100, sizeof (struct obj), fp);
	/* Write out the depth */
	fwrite(&depth, 1, sizeof depth, fp);
	/* Write out the player. */
	fwrite(&u, 1, sizeof u, fp);
	/* Clean up */
	fflush(fp);
	fclose(fp);
	/* Compress! */
	system("gzip 7drl.sav");
	game_finished = 1;
	return;
}

void load_game(void)
{
	FILE *fp;
	system("gunzip 7drl.sav");
	fp = fopen("7drl.sav", "rb");
	fread(running_state, 256, 1, fp);
	setstate(running_state);
	room_reset();
	build_level();
	fread(mapflags, DUN_SIZE, DUN_SIZE * sizeof (int), fp);
	fread(permons, 100, sizeof (struct permon), fp);
	fread(permobjs, 100, sizeof (struct permobj), fp);
	fread(monsters, 100, sizeof (struct mon), fp);
	fread(objects, 100, sizeof (struct obj), fp);
	rebuild_mapobjs();
	rebuild_mapmons();
	fread(&depth, 1, sizeof depth, fp);
	fread(&u, 1, sizeof u, fp);
	fclose(fp);
	unlink("7drl.sav");
	status_updated = 1;
	map_updated = 1;
	print_msg("Game loaded.\n");
}

static void rnum_init(void)
{
	int t;
	time((time_t *) &t);
	t ^= (getuid() << 16) | (getpid() & 0xFFFF);
	initstate(t, running_state, 256);
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
	rval = 1 + (random() / ((RAND_MAX / sides) + 1));
#if 0
	print_msg("Die: %d  divisor %d result %d\n", sides, (RAND_MAX / sides) + 1, rval);
#endif
	return rval;
}

int zero_die(int sides)
{
	int rval;
	rval = random() / ((RAND_MAX / sides) + 1);
#if 0
	print_msg("Die: %d  divisor %d result %d\n", sides, (RAND_MAX / sides) + 1, rval);
#endif
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
	u_init();
	flavours_init();
	rnum_init();
	make_new_level();
	status_updated = 1;
	map_updated = 1;
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
		if (u.weapon >= 0)
		{
			print_msg("You are already wielding a weapon.\n");
			j = getyn("Do you want to change weapons?\n");
			if (j == 1)
			{
				print_msg("Weapon unwielded. To use your bare hands, select\nno weapon.\n");
				u.weapon = 0;
			}
			else
			{
				print_msg("Cancelled.\n");
				return 0;
			}
		}
		i = inv_select(POCLASS_WEAPON, "wield");
		if (i >= 0)
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
		i = inv_select(POCLASS_ARMOUR, "wear");
		if (i >= 0)
		{
			u.armour = u.inventory[i];
			recalc_defence();
			print_msg("You change your armour.\n");
			return 1;
		}
		return 0;
	case TAKE_OFF_ARMOUR:
		u.armour = -1;
		recalc_defence();
		print_msg("You take off your armour.\n");
		return 1;

	case GIVE_HELP:
		print_help();
		return 0;

	case READ_SCROLL:
		i = inv_select(POCLASS_SCROLL, "read");
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
		leave_level();
		make_new_level();
		return 0;

	case STAND_STILL:
		return 1;

	case QUAFF_POTION:
		i = inv_select(POCLASS_POTION, "quaff");
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
		print_msg("You remove your ring.\n");
		if (objects[u.ring].obj_id == PO_RING_DOOM)
		{
			int dmg;
			print_msg("The ring of doom exacts vengeance!\n");
			drain_body(one_die(4), "a ring of doom");
			drain_agility(one_die(4), "a ring of doom");
			dmg = one_die(20);
			damage_u(dmg, DEATH_KILLED, "a ring of doom");
			u.hpmax -= dmg;
			status_updated = 1;
			display_update();
		}
		u.ring = -1;
		return 1;
	case PUT_ON_RING:
		if (u.ring != -1)
		{
			print_msg("You are already wearing a ring.\n");
			return 0;
		}
		i = inv_select(POCLASS_RING, "put on");
		if (i >= 0)
		{
			u.ring = u.inventory[i];
			print_msg("You put on ");
			print_obj_name(u.ring);
			print_msg(".\n");
			return 1;
		}
		return 0;
	case DROP_ITEM:
		i = inv_select(POCLASS_NONE, "drop");
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
	case SAVE_GAME:
		game_finished = 1;
		save_game();
		return 0;
	case QUIT:
		j = getyn("Really quit?\n");
		if (j > 0)
		{
			game_finished = 1;
		}
		return 0;
	case SHOW_INVENTORY:
		print_msg("You are carrying:\n");
		print_inv(POCLASS_NONE);
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
	while (!game_finished)
	{
		if (!(game_tick % 20) && (u.hpcur != u.hpmax))
		{
			heal_u(1, 0);
		}
		if (((game_tick % 20) == 10) && (objects[u.ring].obj_id == PO_RING_REGEN) && (u.hpcur != u.hpmax))
		{
			print_msg("Your ring pulses soothingly.\n");
			heal_u(one_die(3), 0);
			permobjs[PO_RING_REGEN].known = 1;
		}
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
		if (action_speed <= 1)
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
				permobjs[PO_RING_DOOM].known = 1;
			}
		}
		for (i = 0; i < 100; i++)
		{
			if (monsters[i].used == 0)
			{
				/* Unused monster */
				continue;
			}
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
		game_tick++;
	}
}

int main(void)
{
	struct stat s;
	int i;
	display_init();
	/* Do we have a saved game? */
	i = stat("7drl.sav.gz", &s);
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

