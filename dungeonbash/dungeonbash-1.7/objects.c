/* objects.c
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

#define OBJECTS_C
#include "dunbash.h"
#include "monsters.h"

struct obj objects[100];
int get_random_pobj(void);
static int consume_obj(int obj);

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
    case PO_SCR_IDENTIFY:
	print_msg("This is an identify scroll!\n");
	for (i = 0; i < 19; i++)
	{
	    if (u.inventory[i] != -1)
	    {
		permobjs[objects[u.inventory[i]].obj_id].known = 1;
	    }
	}
	break;
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
	i = summoning(u.y, u.x, one_die(3) + 1);
	if (i > 0)
	{
	    print_msg("Monsters appear!\n");
	}
	else
	{
	    print_msg("You hear a snarl of frustration.\n");
	}
	break;
    case PO_SCR_AGGRAVATE:
	print_msg("You hear a high-pitched humming noise.\n");
	for (i = 0; i < 100; i++)
	{
	    if (monsters[i].used)
	    {
		monsters[i].awake = 1;
	    }
	}
	break;
    case PO_SCR_PROTECTION:
	print_msg("You feel like something is helping you.\n");
	if (!u.protection)
	{
	    /* Do not prolong existing protection, only grant
	     * protection to the unprotected. */
	    u.protection = 100;
	}
	if (u.withering)
	{
	    print_msg("Your limbs straighten.\n");
	    u.withering = 0;
	}
	if (u.armourmelt)
	{
	    print_msg("Your armour regains its strength.\n");
	    u.armourmelt = 0;
	}
	if (u.leadfoot)
	{
	    print_msg("You shed your feet of lead.\n");
	    u.leadfoot = 0;
	}
	break;
    default:
	print_msg("Impossible: reading non-scroll\n");
	return 0;
    }
    permobjs[optr->obj_id].known = 1;
    return consume_obj(obj);
}

static int consume_obj(int obj)
{
    int i;
    objects[obj].quan--;
    if (objects[obj].quan == 0)
    {
	objects[obj].used = 0;
	if (objects[obj].with_you)
	{
	    if (obj == u.armour)
	    {
		u.armour = -1;
		recalc_defence();
	    }
	    else if (obj == u.weapon)
	    {
		u.weapon = -1;
		recalc_defence();
	    }
	    else if (obj == u.ring)
	    {
		u.ring = -1;
		recalc_defence();
	    }
	    for (i = 0; i < 19; i++)
	    {
		if (u.inventory[i] == obj)
		{
		    u.inventory[i] = -1;
		    break;
		}
	    }
	}
	return 1;
    }
    return 0;
}

int eat_food(int obj)
{
    struct obj *optr = objects + obj;
    if (permobjs[optr->obj_id].poclass != POCLASS_FOOD)
    {
	print_msg("Error: attempt to eat non-food (%d)!\n", optr->obj_id);
	return -1;
    }
    if (u.food < 0)
    {
	print_msg("You ravenously devour your food!\n");
    }
    else
    {
	print_msg("You eat some food.\n");
    }
    u.food += 1500;
    status_updated = 1;
    display_update();
    return consume_obj(obj);
}

int quaff_potion(int obj)
{
    struct obj *optr = objects + obj;
    switch (optr->obj_id)
    {
    case PO_POT_BODY:
	gain_body(1, 1);
	break;
    case PO_POT_AGILITY:
	gain_agility(1, 1);
	break;
    case PO_POT_WEAKNESS:
	print_msg("You feel that was a bad idea!\n");
	drain_body(one_die(4), "a potion of weakness", 1);
	drain_agility(one_die(4), "a potion of weakness", 1);
	break;
    case PO_POT_POISON:
	print_msg("This is poison!\n");
	damage_u(dice(3, 12), DEATH_KILLED, "drinking poison");
	display_update();
	break;
    case PO_POT_HEAL:
	/* Heal player; if hit points brought to max, gain one
	 * hit point. */
	heal_u(dice(3, 12), 1, 1);
	break;
    case PO_POT_RESTORATION:
	print_msg("This potion makes you feel warm all over.\n");
	status_updated = 1;
	if (!zero_die(2))
	{
	    if (u.adam)
	    {
		u.adam = 0;
		print_msg("You feel less clumsy.\n");
	    }
	    else if (u.bdam)
	    {
		u.bdam = 0;
		print_msg("You feel less feeble.\n");
	    }
	}
	else
	{
	    if (u.bdam)
	    {
		u.bdam = 0;
		print_msg("You feel less feeble.\n");
	    }
	    else if (u.adam)
	    {
		u.adam = 0;
		print_msg("You feel less clumsy.\n");
	    }
	}
	break;
    default:
	print_msg("Impossible: quaffing non-potion\n");
	return 0;
    }
    permobjs[optr->obj_id].known = 1;
    return consume_obj(obj);
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
    permobjs[PO_RING_TELEPORT].power = colour_choices[6];
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
    permobjs[PO_SCR_IDENTIFY].power = colour_choices[3];
    permobjs[PO_SCR_PROTECTION].power = colour_choices[4];
    permobjs[PO_SCR_AGGRAVATE].power = colour_choices[5];
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
    permobjs[PO_POT_RESTORATION].power = colour_choices[5];
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
	    po_idx = inclusive_flat(PO_FIRST_POTION, PO_LAST_POTION);
	    break;
	case POCLASS_SCROLL:
	    po_idx = inclusive_flat(PO_FIRST_SCROLL, PO_LAST_SCROLL);
	    break;
	case POCLASS_RING:
	    po_idx = inclusive_flat(PO_FIRST_RING, PO_LAST_RING);
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
	/* v1.3: Do not permit generation of particularly powerful
	 * items (runeswords, mage armour, etc.) at shallow depths.
	 * (game balance fix) */
	if (depth < permobjs[po_idx].depth)
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
    switch (permobjs[po_idx].poclass)
    {
    case POCLASS_WEAPON:
    case POCLASS_ARMOUR:
	/* Set durability of weapons and armour to a suitable value.
	 * 100 has been chosen for the moment, but this may need
	 * tuning. */
	objects[i].durability = OBJ_MAX_DUR;
	break;
    default:
	break;
    }
    if (!objects[i].with_you)
    {
	mapobject[y][x] = i;
    }
    return i;
}

void fprint_obj_name(FILE *fp, int obj)
{
    struct obj *optr;
    struct permobj *poptr;
    optr = objects + obj;
    poptr = permobjs + optr->obj_id;
    if (poptr->known)
    {
	if (optr->quan > 1)
	{
	    fprintf(fp, "%d %s", optr->quan, poptr->plural);
	}
	else if (po_is_stackable(optr->obj_id))
	{
	    fprintf(fp, "1 %s", poptr->name);
	}
	else if ((poptr->poclass == POCLASS_WEAPON) ||
		 (poptr->poclass == POCLASS_ARMOUR))
	{
	    fprintf(fp, "a%s %s (%d/%d)", is_vowel(poptr->name[0]) ? "n" : "", poptr->name, optr->durability, OBJ_MAX_DUR);
	}
	else
	{
	    fprintf(fp, "a%s %s", is_vowel(poptr->name[0]) ? "n" : "", poptr->name);
	}
    }
    else
    {
	switch (poptr->poclass)
	{
	case POCLASS_NONE:
	    fprintf(fp, "a non-thing (%d)", optr->obj_id);
	    break;
	case POCLASS_FOOD:
	    fprintf(fp, "a mysterious food (%d)", optr->obj_id);
	    break;
	case POCLASS_WEAPON:
	    fprintf(fp, "a mysterious weapon (%d)", optr->obj_id);
	    break;
	case POCLASS_ARMOUR:
	    if ((optr->obj_id == PO_ROBE) ||
		(optr->obj_id == PO_ROBE_SHADOWS) ||
		(optr->obj_id == PO_ROBE_SWIFTNESS))
	    {
		fprintf(fp, "a robe");
	    }
	    else
	    {
		fprintf(fp, "some mysterious armour (%d)", optr->obj_id);
	    }
	    break;
	case POCLASS_SCROLL:
	    if (optr->quan > 1)
	    {
		fprintf(fp, "%d scrolls '%s'", optr->quan, scroll_titles[poptr->power]);
	    }
	    else
	    {
		fprintf(fp, "1 scroll '%s'", scroll_titles[poptr->power]);
	    }
	    break;
	case POCLASS_POTION:
	    if (optr->quan > 1)
	    {
		fprintf(fp, "%d %s potions", optr->quan, potion_colours[poptr->power]);
	    }
	    else
	    {
		fprintf(fp, "1 %s potion", potion_colours[poptr->power]);
	    }
	    break;
	case POCLASS_RING:
	    fprintf(fp, "a%s %s ring", is_vowel(ring_colours[poptr->power][0]) ? "n" : "", ring_colours[poptr->power]);
	    break;
	}
    }
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
	else if (po_is_stackable(optr->obj_id))
	{
	    print_msg("1 %s", poptr->name);
	}
	else if ((poptr->poclass == POCLASS_WEAPON) ||
		 (poptr->poclass == POCLASS_ARMOUR))
	{
	    print_msg("a%s %s (%d/%d)", is_vowel(poptr->name[0]) ? "n" : "", poptr->name, optr->durability, OBJ_MAX_DUR);
	}
	else
	{
	    print_msg("a%s %s", is_vowel(poptr->name[0]) ? "n" : "", poptr->name);
	}
    }
    else
    {
	switch (poptr->poclass)
	{
	case POCLASS_NONE:
	    print_msg("a non-thing (%d)", optr->obj_id);
	    break;
	case POCLASS_FOOD:
	    print_msg("a mysterious food (%d)", optr->obj_id);
	    break;
	case POCLASS_WEAPON:
	    print_msg("a mysterious weapon (%d)", optr->obj_id);
	    break;
	case POCLASS_ARMOUR:
	    if ((optr->obj_id == PO_ROBE) ||
		(optr->obj_id == PO_ROBE_SHADOWS) ||
		(optr->obj_id == PO_ROBE_SWIFTNESS))
	    {
		print_msg("a robe");
	    }
	    else
	    {
		print_msg("some mysterious armour (%d)", optr->obj_id);
	    }
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
	    print_msg("a%s %s ring", is_vowel(ring_colours[poptr->power][0]) ? "n" : "", ring_colours[poptr->power]);
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
    case POCLASS_FOOD:
	return 1;
    }
}

void attempt_pickup(void)
{
    int i;
    int stackable;
    if (objects[mapobject[u.y][u.x]].obj_id == PO_GOLD)
    {
	print_msg("You get %d gold.\n", objects[mapobject[u.y][u.x]].quan);
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

void damage_obj(int obj)
{
    /* Only weapons and armour have non-zero durability. */
    if (objects[obj].durability == 0)
    {
	/* Break the object. Weapons and armour don't stack. */
	if (obj == u.weapon)
	{
	    print_msg("Your weapon breaks!\n");
	}
	else if (obj == u.armour)
	{
	    print_msg("Your armour is ruined!\n");
	}
	consume_obj(obj);
	recalc_defence();
    }
    else
    {
	objects[obj].durability--;
    }
} 

void describe_object(int obj)
{
    struct obj *optr;
    struct permobj *poptr;
    print_obj_name(obj);
    optr = objects + obj;
    poptr = permobjs + optr->obj_id;
    if (poptr->known)
    {
	print_msg("\n%s\n", poptr->description);
    }
    else
    {
	switch (poptr->poclass)
	{
	case POCLASS_NONE:
	default:
	    print_msg("This unidentified permobj (%d) is indescribable.\n", optr->obj_id);
	    break;
	case POCLASS_ARMOUR:
	    if ((optr->obj_id == PO_ROBE) ||
		(optr->obj_id == PO_ROBE_SHADOWS) ||
		(optr->obj_id == PO_ROBE_SWIFTNESS))
	    {
		print_msg("\nA simple woolen robe.\n");
	    }
	    else
	    {
		print_msg("\nAn unidentified and indescribable piece of armour (%d)\n", optr->obj_id);
	    }
	    break;
	case POCLASS_SCROLL:
	    print_msg("\nA mysterious scroll.\nReading it will unleash its enchantment.\n");
	    break;
	case POCLASS_POTION:
	    print_msg("\nA rather dubious-looking liquid.\nQuaffing it may be baleful or beneficial.\n");
	    break;
	case POCLASS_RING:
	    print_msg("\nSome rings are baneful, some are beneficial, and\nsome are junk.\n");
	    break;
	}
    }
}

int evasion_penalty(int obj)
{
    switch (objects[obj].obj_id)
    {
    case PO_ROBE:
	return 5;

    case PO_LEATHER_ARMOUR:
    case PO_DRAGON_ARMOUR:
	return 10;

    case PO_CHAINMAIL:
    case PO_SACRED_MAIL:
	return 25;

    case PO_PLATE_ARMOUR:
    case PO_MAGE_ARMOUR:
    case PO_METEOR_ARMOUR:
	return 40;

    case PO_ROBE_SWIFTNESS:
	return 0;

    case PO_ROBE_SHADOWS:
	return -15;	/* This is a bonus. */

    default:
	/* If you've somehow managed to wear a non-armour, you're abusing
	 * a bug; get a 100% penalty to evasion. */
	print_msg("Trying to find evasion penalty of non-armour!\n");
	return 100;
    }
}

/* objects.c */
//あ
