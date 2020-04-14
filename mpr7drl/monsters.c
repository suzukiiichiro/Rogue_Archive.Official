/* monsters.c */
#define MONSTERS_C
#include "7drl.h"

struct mon monsters[100];
static int reject_mon(int pm);
static void select_space(int *py, int *px, int dy, int dx, int selection_mode);

void summoning(int y, int x, int how_many)
{
	int i;
	int dy;
	int dx;
	int tryct;
	int mon;
	for (i = 0; i < how_many; i++)
	{
		for (tryct = 0; tryct < 20; tryct++)
		{
			dy = zero_die(3) - 1;
			dx = zero_die(3) - 1;
			if (terrain[y + dy][x + dx] == FLOOR)
			{
				mon = create_mon(-1, y + dy, x + dx);
				if (mon != -1)
				{
					break;
				}
			}
		}
	}
}

int ood(int power, int ratio)
{
	return (power - depth + ratio - 1) / ratio;
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
			monsters[mon].hpmax = permons[pm_idx].hp + ood(permons[pm_idx].power, 1);
			monsters[mon].hpcur = monsters[mon].hpmax;
			monsters[mon].tohit = permons[pm_idx].tohit + ood(permons[pm_idx].power, 3);
			monsters[mon].defence = permons[pm_idx].defence + ood(permons[pm_idx].power, 3);
			monsters[mon].dam = permons[pm_idx].dam + ood(permons[pm_idx].power, 5);
			monsters[mon].awake = 0;
			mapmonster[y][x] = mon;
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
	while (((mapobject[y][x] != -1) || (terrain[y][x] != FLOOR)) && tryct < 100)
	{
		dy = zero_die(3) - 1;
		dx = zero_die(3) - 1;
		tryct++;
		y += dy;
		x += dx;
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
	case PM_WARLORD:
		if (!zero_die(3))
		{
			create_obj(PO_RUNESWORD, 1, 0, y, x);
		}
	default:
		break;
	}
}

int mon_can_pass(int mon, int y, int x)
{
	if ((y < 0) || (x < 0) || (y >= DUN_SIZE) || (x >= DUN_SIZE))
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
		print_msg("a %s", permons[monsters[mon].mon_id].name);
		break;
	case 1: /* the */
		print_msg("the %s", permons[monsters[mon].mon_id].name);
		break;
	case 2: /* A */
		print_msg("A %s", permons[monsters[mon].mon_id].name);
		break;
	case 3: /* The */
		print_msg("The %s", permons[monsters[mon].mon_id].name);
		break;
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
			print_mon_name(mon, 1);
			print_msg("!\n");
			gain_experience(permons[mptr->mon_id].exp);
		}
		else
		{
			print_mon_name(mon, 2);
			print_msg(" dies.\n");
		}
		death_drop(mon);
		mapmonster[mptr->y][mptr->x] = -1;
		mptr->used = 0;
		map_updated = 1;
		display_update();
	}
	else
	{
		mptr->hpcur -= amount;
	}
}

int pmon_is_undead(int pm)
{
	switch (pm)
	{
	case PM_LICH:
	case PM_ZOMBIE:
		return 1;
	default:
		return 0;
	}
}

int pmon_resists_cold(int pm)
{
	switch (pm)
	{
	case PM_ICE_MONSTER:
	case PM_WIZARD:
	case PM_ZOMBIE:
	case PM_LICH:
	case PM_DEMON:
		return 1;
	default:
		return 0;
	}
}

int pmon_resists_fire(int pm)
{
	switch (pm)
	{
	case PM_WIZARD:
	case PM_DRAGON:
	case PM_LICH:
	case PM_DEMON:
		return 1;
	default:
		return 0;
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

void select_space(int *py, int *px, int dy, int dx, int selection_mode)
{
	int dy2, dx2;
	int dy3, dx3;
	int ady, adx;
	int y, x;
	int sy, sx;
	if (dy == 0)
	{
		sy = 0;
		ady = 0;
	}
	else
	{
		sy = dy < 0 ? -1 : 1;
		ady = dy < 0 ? -dy : dy;
	}
	if (dx == 0)
	{
		sx = 0;
		adx = dx < 0 ? -dx : dx;
	}
	else
	{
		sx = dx < 0 ? -1 : 1;
		adx = dx < 0 ? -dx : dx;
	}
	switch (selection_mode)
	{
	case 0:
		/* Simple convergence */
		y = *py + sy;
		x = *px + sx;
		if (mon_can_pass(mapmonster[*py][*px], y, x))
		{
			break;
		}
		x = *px;
		if (mon_can_pass(mapmonster[*py][*px], y, x))
		{
			break;
		}
		y = *py;
		x = *px + sx;
		if (mon_can_pass(mapmonster[*py][*px], y, x))
		{
			break;
		}
		y = *py;
		x = *px;
		break;
	case 1:
		/* Converge to cardinal */
		if ((dy == dx) || (dy == -dx) || (!dy) || (!dx))
		{
			/* On cardinal. Stay there if we can. But close anyway. */
			x = *px + sx;
			y = *py + sy;
			if (mon_can_pass(mapmonster[*py][*px], y, x))
			{
				break;
			}
			x = *px;
			if (mon_can_pass(mapmonster[*py][*px], y, x))
			{
				break;
			}
			y = *py;
			x = *px + sx;
			if (mon_can_pass(mapmonster[*py][*px], y, x))
			{
				break;
			}
		}
		else if ((ady == 1) || ((adx > 1) && (ady > adx)))
		{
			/* One step in ydir off a NSEW cardinal, or further
			 * off cardinal in y than in x */
			y = *py + sy;
			x = *px;
			if (mon_can_pass(mapmonster[*py][*px], y, x))
			{
				break;
			}
			x = *px + sx;
			if (mon_can_pass(mapmonster[*py][*px], y, x))
			{
				break;
			}
			y = *py;
			if (mon_can_pass(mapmonster[*py][*px], y, x))
			{
				break;
			}
		}
		else if ((adx == 1) || ((ady > 1) && (adx > ady)))
		{
			/* One step off a diagonal cardinal, with adx > ady */
			x = *px + sx;
			y = *py;
			if (mon_can_pass(mapmonster[*py][*px], y, x))
			{
				break;
			}
			y = *py + sy;
			if (mon_can_pass(mapmonster[*py][*px], y, x))
			{
				break;
			}
			x = *px;
			if (mon_can_pass(mapmonster[*py][*px], y, x))
			{
				break;
			}
		}
		y = *py;
		x = *px;
		break;
	case 2:
		/* Try to close, but try harder to stay off cardinals at
		 * range */
		if ((dy <= 2) && (dy >= -2) && (dx <= 2) && (dx >= -2))
		{
			y = *py + sy;
			x = *px + sx;
			break;
		}
		else if (sx == 0)
		{
			y = *py + sy;
			dx2 = zero_die(2) ? 1 : -1;
			dx3 = -dx2;
			if (mon_can_pass(mapmonster[*py][*px], y, (*px) + dx2));
			{
				x = (*px) + dx2;
				break;
			}
			if (mon_can_pass(mapmonster[*py][*px], (*py) + dy3, x))
			{
				x = (*px) + dx2;
				break;
			}
			/* Can't get off cardinal. Diverge directly. */
			y = (*py);
			x = (*px) + sx;
			if (mon_can_pass(mapmonster[*py][*px], y, x))
			{
				break;
			}
		}
		else if (sy == 0)
		{
			x = *px + sx;
			dy2 = zero_die(2) ? 1 : -1;
			dy3 = -dy2;
			if (mon_can_pass(mapmonster[*py][*px], (*py) + dy2, x))
			{
				y = (*py) + dy2;
				break;
			}
			if (mon_can_pass(mapmonster[*py][*px], (*py) + dy3, x))
			{
				y = (*py) + dy2;
				break;
			}
			x = *px;
			if (mon_can_pass(mapmonster[*py][*px], (*py) + dy2, x))
			{
				y = (*py) + dy2;
				break;
			}
			if (mon_can_pass(mapmonster[*py][*px], (*py) + dy3, x))
			{
				y = (*py) + dy3;
				break;
			}
			/* Can't get off cardinal. Close range. */
			y = (*py);
			x = (*px) + sx;
			break;
		}
		else if ((dy == dx) || (dy == -dx))
		{
			if (zero_die(2))
			{
				dy2 = sy;
				dx2 = 0;
				dy3 = 0;
				dx3 = sx;
			}
			else
			{
				dy2 = 0;
				dx2 = sx;
				dy3 = sy;
				dx3 = 0;
			}
			if (mon_can_pass(mapmonster[*py][*px], (*py + dy2), (*px + dx2)))
			{
				y = *py + dy2;
				x = *px + dx2;
				break;
			}
			if (mon_can_pass(mapmonster[*py][*px], (*py + dy3), (*px + dx3)))
			{
				y = *py + dy2;
				x = *px + dx2;
				break;
			}
		}
		y = *py;
		x = *px;
		break;
	}
	*py = y;
	*px = x;
}

void move_mon(int mon, int y, int x)
{
	struct mon *mptr;
	mptr = monsters + mon;
	mapmonster[mptr->y][mptr->x] = -1;
	mptr->y = y;
	mptr->x = x;
	mapmonster[mptr->y][mptr->x] = mon;
	map_updated = 1;
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
		i = create_mon(PM_DEMON, y, x);
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

void mon_acts(int mon)
{
	struct mon *mptr;
	int dy, dx;
	int y, x;
	int sy, sx;
	mptr = monsters + mon;
	/* dy,dx == direction monster must go to reach you. */
	dy = u.y - mptr->y;
	dx = u.x - mptr->x;
	y = mptr->y;
	x = mptr->x;
	if ((-2 < dy) && (dy < 2) && (-2 < dx) && (dx < 2))
	{
		/* Adjacent! Attack you.  Demons have a 1 in 10 chance of
		 * attempting to summon another demon instead of attacking
		 * you. */
		if ((mptr->mon_id == PM_DEMON) && !zero_die(10))
		{
			summon_demon_near(y, x);
		}
		else
		{
			mhitu(mon);
		}
	}
	else if ((roomnums[u.y][u.x] != -1) && (roomnums[u.y][u.x] == roomnums[mptr->y][mptr->x]))
	{
		/* In same room. */
		if (!mptr->awake)
		{
			print_mon_name(mon, 2);
			print_msg(" notices you.\n");
			mptr->awake = 1;
		}
		switch (mptr->mon_id)
		{
		case PM_LICH:
			/* Maybe throw lightning if on a cardinal and not
			 * at r1. */
			if ((dy == dx) || (dy == -dx) || (!dx) || (!dy))
			{
				if (zero_die(6) < 4)
				{
					print_msg("The lich fires vile magic at you!\n");
					mshootu(mon, DT_NECRO);
				}
			}
			else
			{
				/* Move towards you, preferably converging
				 * to a roguespace cardinal. */
				select_space(&y, &x, dy, dx, 1);
			}
			break;
		case PM_WIZARD:
			/* Maybe throw lightning if on a cardinal and not
			 * at r1. */
			if ((dy == dx) || (dy == -dx) || (!dx) || (!dy))
			{
				if (zero_die(6) < 4)
				{
					print_msg("The wizard throws lightning at you!\n");
					mshootu(mon, DT_ELEC);
				}
			}
			else
			{
				/* Move towards you, preferably converging
				 * to a roguespace cardinal. */
				select_space(&y, &x, dy, dx, 1);
			}
			break;
		case PM_DRAGON:
			/* Maybe breathe fire if on a cardinal and not
			 * at r1. */
			if ((dy == dx) || (dy == -dx) || (!dx) || (!dy))
			{
				if (zero_die(6) < 3)
				{
					print_msg("The dragon breathes fire!\n");
					mshootu(mon, DT_FIRE);
				}
			}
			else
			{
				/* Move towards you, preferably converging
				 * to a roguespace cardinal. */
				select_space(&y, &x, dy, dx, 1);
			}
			break;
		case PM_HUNTER:
			/* Maybe fire an arrow if on a cardinal and not
			 * at r1. */
			if ((dy == dx) || (dy == -dx) || (!dx) || (!dy))
			{
				if (zero_die(6) < 3)
				{
					print_msg("The hunter shoots at you!\n");
					mshootu(mon, DT_PHYS);
				}
			}
			else
			{
				/* Move towards you, preferably converging
				 * to a roguespace cardinal. */
				select_space(&y, &x, dy, dx, 1);
			}
			break;
		case PM_ICE_MONSTER:
			/* Maybe fire an ice blast if on a cardinal and not
			 * at r1. */
			if ((dy == dx) || (dy == -dx) || (!dx) || (!dy))
			{
				if (zero_die(6) < 3)
				{
					print_msg("The ice monster throws frost!\n");
					mshootu(mon, DT_COLD);
				}
			}
			else
			{
				/* Move towards you, preferably converging
				 * to a roguespace cardinal. */
				select_space(&y, &x, dy, dx, 1);
			}
			break;
		case PM_WARLORD:
			/* Evade cardinals while closing */
			select_space(&y, &x, dy, dx, 2);
		default:
			/* Close blindly if not at r1. */
			select_space(&y, &x, dy, dx, 0);
			break;
		}
		move_mon(mon, y, x);
	}
	else if (roomnums[y][x] == 0)
	{
		/* In a corridor. */
		sy = dy ? ((dy > 0) ? 1 : -1) : 0;
		sx = dx ? ((dx > 0) ? 1 : -1) : 0;
		if (sy)
		{
			if (terrain[y + sy][x] == FLOOR)
			{
			}
		}
	}
	else
	{
		/* Different room. For now, do nothing. */
	}
}

/* monsters.c */
