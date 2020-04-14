/* objects.c */
#define OBJECTS_C
#include "7drl.h"

struct obj objects[100];
int get_random_pobj(void);

const char ring_colours[20][16] = {
	"gold", "ruby", "sapphire", "ivory", "coral",
	"amethyst", "silver", "iron", "copper", "jade",
	"haematite", "bone", "crystal", "platinum", "lead",
	"diamond", "topaz", "emerald", "electrum", "smoky quartz"
};

const char scroll_titles[20][16] = {
	"grem pho", "terra terrax", "phong", "ateh malkuth", "xixaxa",
	"aku ryo tai san", "qoph shin tau", "ythek shri", "ia ia", "cthulhu fhtagn",
	"arifech malex", "DOOM", "leme athem", "hail smkznrf", "rorrim foo",
	"ad aerarium", "ligemrom", "asher ehiyeh", "YELLOW SIGN", "ELDER SIGN"
};

const char potion_colours[20][16] = {
	"purple", "red", "blue", "green", "yellow",
	"orange", "white", "black", "brown", "fizzy",
	"grey", "silver", "gold", "shimmering", "glowing",
	"navy blue", "bottle green", "amber", "lilac", "ivory"
};

int read_scroll(int obj)
{
	struct obj *optr = objects + obj;
	int i;
	switch (optr->obj_id)
	{
	case PO_SCR_TELEPORT:
		teleport_u();
		break;
	case PO_SCR_FIRE:
		print_msg("The scroll explodes in flames!\n");
		if (u.ring != -1)
		{
			if (objects[u.ring].obj_id == PO_RING_FIRE)
			{
				print_msg("Your ring glows, and the flames seem cool.\n");
				permobjs[objects[u.ring].obj_id].known = 1;
				break;
			}
		}
		i = damage_u(dice(4, 10), DEATH_KILLED, "searing flames");
		if (!i)
		{
			print_msg("That hurt!\n");
		}
		break;
	case PO_SCR_MONSTERS:
		print_msg("Monsters appear!\n");
		summoning(u.y, u.x, one_die(3) + 1);
		break;
	default:
		print_msg("Impossible: reading non-scroll\n");
		return 0;
	}
	permobjs[optr->obj_id].known = 1;
	optr->quan--;
	if (optr->quan < 1)
	{
		optr->used = 0;
		return 1;
	}
	return 0;
}

int quaff_potion(int obj)
{
	struct obj *optr = objects + obj;
	switch (optr->obj_id)
	{
	case PO_POT_BODY:
		u.body++;
		u.agility++;
		print_msg("You feel stronger!\n");
		break;
	case PO_POT_AGILITY:
		u.agility++;
		status_updated = 1;
		print_msg("You feel more agile!\n");
		break;
	case PO_POT_WEAKNESS:
		print_msg("You feel that was a bad idea!\n");
		drain_body(one_die(4), "a potion of weakness");
		drain_agility(one_die(4), "a potion of weakness");
		break;
	case PO_POT_POISON:
		print_msg("This is poison!\n");
		damage_u(dice(3, 12), DEATH_KILLED, "drinking poison");
		break;
	case PO_POT_HEAL:
		heal_u(dice(3, 12), 0);
		break;
	default:
		print_msg("Impossible: quaffing non-potion\n");
		return 0;
	}
	permobjs[optr->obj_id].known = 1;
	optr->quan--;
	if (optr->quan < 1)
	{
		optr->used = 0;
		return 1;
	}
	return 0;
}

void flavours_init(void)
{
	int colour_choices[10];
	int i;
	int j;
	int done;
	/* Flavoured items use "power" to track their flavour.  This is a
	 * gross and unforgiveable hack. */
	/* Rings */
	for (i = 0; i < 10;)
	{
		colour_choices[i] = zero_die(20);
		done = 1;
		for (j = 0; j < i; j++)
		{
			if (colour_choices[i] == colour_choices[j])
			{
				done = 0;
			}
		}
		if (done)
		{
			i++;
		}
	}
	permobjs[PO_RING_REGEN].power = colour_choices[0];
	permobjs[PO_RING_FIRE].power = colour_choices[1];
	permobjs[PO_RING_WEDDING].power = colour_choices[2];
	permobjs[PO_RING_VAMPIRE].power = colour_choices[3];
	permobjs[PO_RING_FROST].power = colour_choices[4];
	permobjs[PO_RING_DOOM].power = colour_choices[5];
	/* Scrolls */
	for (i = 0; i < 10;)
	{
		colour_choices[i] = zero_die(20);
		done = 1;
		for (j = 0; j < i; j++)
		{
			if (colour_choices[i] == colour_choices[j])
			{
				done = 0;
			}
		}
		if (done)
		{
			i++;
		}
	}
	permobjs[PO_SCR_FIRE].power = colour_choices[0];
	permobjs[PO_SCR_TELEPORT].power = colour_choices[1];
	permobjs[PO_SCR_MONSTERS].power = colour_choices[2];
	/* Potions */
	for (i = 0; i < 10;)
	{
		colour_choices[i] = zero_die(20);
		done = 1;
		for (j = 0; j < i; j++)
		{
			if (colour_choices[i] == colour_choices[j])
			{
				done = 0;
			}
		}
		if (done)
		{
			i++;
		}
	}
	permobjs[PO_POT_HEAL].power = colour_choices[0];
	permobjs[PO_POT_BODY].power = colour_choices[1];
	permobjs[PO_POT_POISON].power = colour_choices[2];
	permobjs[PO_POT_AGILITY].power = colour_choices[3];
	permobjs[PO_POT_WEAKNESS].power = colour_choices[4];
}

int create_obj_class(enum poclass_num po_class, int quantity, int with_you, int y, int x)
{
	int obj;
	int po_idx;
	int tryct;
	for (obj = 0; obj < 100; obj++)
	{
		if (!objects[obj].used)
		{
			break;
		}
	}
	if (obj == 100)
	{
		print_msg("ERROR: Ran out of objects[].\n");
		return -1;
	}
	for (tryct = 0; tryct < 200; tryct++)
	{
		switch (po_class)
		{
		case POCLASS_POTION:
			po_idx = inclusive_flat(PO_POT_HEAL, PO_POT_WEAKNESS);
			break;
		case POCLASS_SCROLL:
			po_idx = inclusive_flat(PO_SCR_TELEPORT, PO_SCR_MONSTERS);
			break;
		case POCLASS_RING:
			po_idx = inclusive_flat(PO_RING_REGEN, PO_RING_DOOM);
			break;
		default:
			/* No getting armour/weapons by class... yet. */
			return -1;
		}
		if (zero_die(100) < permobjs[po_idx].rarity)
		{
			continue;
		}
		break;
	}
	objects[obj].obj_id = po_idx;
	objects[obj].quan = quantity;
	return obj;
}

int get_random_pobj(void)
{
	int tryct;
	int po_idx;
	for (tryct = 0; tryct < 200; tryct++)
	{
		po_idx = zero_die(PO_REAL_COUNT);
		if (zero_die(100) < permobjs[po_idx].rarity)
		{
			po_idx = -1;
			continue;
		}
		break;
	}
	return po_idx;
}

int create_obj(int po_idx, int quantity, int with_you, int y, int x)
{
	int i;
	for (i = 0; i < 100; i++)
	{
		if (!objects[i].used)
		{
			break;
		}
	}
	if (i == 100)
	{
		print_msg("ERROR: Ran out of objects[].\n");
		return -1;
	}
	if (po_idx == -1)
	{
		po_idx = get_random_pobj();
		if (po_idx == -1)
		{
			return -1;
		}
	}
	objects[i].obj_id = po_idx;
	objects[i].with_you = with_you;
	objects[i].used = 1;
	objects[i].y = y;
	objects[i].x = x;
	if (po_idx == PO_GOLD)
	{
		objects[i].quan = dice(depth + 1, 20);
	}
	else
	{
		objects[i].quan = quantity;
	}
	if (!objects[i].with_you)
	{
		mapobject[y][x] = i;
	}
	return i;
}

void print_obj_name(int obj)
{
	struct obj *optr;
	struct permobj *poptr;
	optr = objects + obj;
	poptr = permobjs + optr->obj_id;
	if (poptr->known)
	{
		if (optr->quan > 1)
		{
			print_msg("%d %s", optr->quan, poptr->plural);
		}
		else
		{
			print_msg("a %s", poptr->name);
		}
	}
	else
	{
		switch (poptr->poclass)
		{
		case POCLASS_NONE:
		case POCLASS_WEAPON:
		case POCLASS_ARMOUR:
			print_msg("strangeness");
			break;
		case POCLASS_SCROLL:
			if (optr->quan > 1)
			{
				print_msg("%d scrolls '%s'", optr->quan, scroll_titles[poptr->power]);
			}
			else
			{
				print_msg("1 scroll '%s'", scroll_titles[poptr->power]);
			}
			break;
		case POCLASS_POTION:
			if (optr->quan > 1)
			{
				print_msg("%d %s potions", optr->quan, potion_colours[poptr->power]);
			}
			else
			{
				print_msg("1 %s potion", potion_colours[poptr->power]);
			}
			break;
		case POCLASS_RING:
			print_msg("1 %s ring", ring_colours[poptr->power]);
			break;
		}
	}
}

int drop_obj(int inv_idx)
{
	struct obj *optr;
	optr = objects + u.inventory[inv_idx];
	if (mapobject[u.y][u.x] == -1)
	{
		optr->y = u.y;
		optr->x = u.x;
		mapobject[u.y][u.x] = u.inventory[inv_idx];
		if (u.weapon == u.inventory[inv_idx])
		{
			u.weapon = -1;
		}
		u.inventory[inv_idx] = -1;
		optr->with_you = 0;
		print_msg("You drop ");
		print_obj_name(mapobject[u.y][u.x]);
		print_msg(".\n");
		return 0;
	}
	else
	{
		print_msg("There is already an item here.\n");
	}
	return -1;
}

int po_is_stackable(int po)
{
	switch (permobjs[po].poclass)
	{
	default:
		return 0;
	case POCLASS_POTION:
	case POCLASS_SCROLL:
		return 1;
	}
}

void attempt_pickup(void)
{
	int i;
	int stackable;
	if (objects[mapobject[u.y][u.x]].obj_id == PO_GOLD)
	{
		print_msg("You get %d gold\n", objects[mapobject[u.y][u.x]].quan);
		u.gold += objects[mapobject[u.y][u.x]].quan;
		objects[mapobject[u.y][u.x]].used = 0;
		mapobject[u.y][u.x] = -1;
		return;
	}
	stackable = po_is_stackable(objects[mapobject[u.y][u.x]].obj_id);
	if (stackable)
	{
		for (i = 0; i < 19; i++)
		{
			if ((objects[u.inventory[i]].obj_id == objects[mapobject[u.y][u.x]].obj_id))
			{
				print_msg("You get ");
				print_obj_name(mapobject[u.y][u.x]);
				print_msg(".\nYou now have\n");
				objects[u.inventory[i]].quan += objects[mapobject[u.y][u.x]].quan;
				objects[mapobject[u.y][u.x]].used = 0;
				mapobject[u.y][u.x] = -1;
				print_msg("%c) ", 'a' + i);
				print_obj_name(u.inventory[i]);
				print_msg("\n");
				return;
			}
		}
	}
	for (i = 0; i < 19; i++)
	{
		if (u.inventory[i] == -1)
		{
			break;
		}
	}
	if (i == 19)
	{
		print_msg("Your pack is full.\n");
		return;
	}
	u.inventory[i] = mapobject[u.y][u.x];
	mapobject[u.y][u.x] = -1;
	objects[u.inventory[i]].with_you = 1;
	objects[u.inventory[i]].x = -1;
	objects[u.inventory[i]].y = -1;
	print_msg("You now have\n");
	print_msg("%c) ", 'a' + i);
	print_obj_name(u.inventory[i]);
	print_msg("\n");
}

/* objects.c */
