/* combat.c */

#include "7drl.h"

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
	if (u.ring)
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
				heal_u(healing, 0);
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
				print_msg("You hit ");
				print_mon_name(mapmonster[y][x], 1);
				print_msg(".\n");
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

int mhitu(int mon)
{
	int tohit;
	int damage;
	struct mon *mptr = monsters + mon;
	print_mon_name(mon, 3);
	tohit = zero_die(mptr->tohit + 5);
	if (tohit < u.defence)
	{
		print_msg(" misses you.\n");
		return 0;
	}
	damage = one_die(mptr->dam);
	print_msg(" hits you.\n");
	print_msg("You take %d damage.\n", damage);
	if ((tohit == mptr->tohit + 4) && (mptr->mon_id == PM_SNAKE))
	{
		drain_body(1, "snake venom");
	}
	damage_u(damage, DEATH_KILLED_MON, permons[mptr->mon_id].name);
	display_update();
	return 1;
}

int mshootu(int mon, enum damtyp dtype)
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
	mptr = monsters + mon;
	y = mptr->y;
	x = mptr->x;
	/* dy, dx == trajectory of missile */
	dy = u.y - y;
	dx = u.x - x;
	sy = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0);
	sx = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);
	tohit = zero_die(mptr->tohit);
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
			if (tohit >= u.defence)
			{
				done = 1;
				print_msg("It hits you!\n");
				unaffected = 0;
				if (u.ring != -1)
				{
					switch (dtype)
					{
					case DT_COLD:
						if (objects[u.ring].obj_id == PO_RING_FROST)
						{
							permobjs[PO_RING_FROST].known = 1;
							print_msg("Your ring flahses blue.\n");
							unaffected = 1;
						}
						break;

					case DT_FIRE:
						if (objects[u.ring].obj_id == PO_RING_FIRE)
						{
							permobjs[PO_RING_FIRE].known = 1;
							print_msg("Your ring flashes red.\n");
							unaffected = 1;
						}
						break;
					case DT_NECRO:
						if (objects[u.ring].obj_id == PO_RING_VAMPIRE)
						{
							permobjs[PO_RING_VAMPIRE].known = 1;
							unaffected = 1;
						}
						break;
					default:
						break;
					}
				}
				if (!unaffected)
				{
					damage = one_die(mptr->dam);
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
		else if (mapmonster[y][x])
		{
			done = 1;
			print_msg("Bystander\n");
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
			if (tohit >= mptr->defence)
			{
				damage = one_die(mptr->dam);
				damage_mon(mapmonster[y][x], dtype, 0);
			}
		}
	}
	return 0;
}

/* combat.c */
