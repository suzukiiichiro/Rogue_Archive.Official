#include "7drl.h"

struct permobj permobjs[100] = {
	[PO_DAGGER] =
	{
		"dagger", "daggers", POCLASS_WEAPON, 25, ')', 4, 1, 1
	},
	[PO_LONG_SWORD] =
	{
		"long sword", "long swords", POCLASS_WEAPON, 30, ')', 10, 1, 1
	},
	[PO_MACE] =
	{
		"mace", "maces", POCLASS_WEAPON, 30, ')', 7, 1, 1
	},
	[PO_RUNESWORD] =
	{
		"runesword", "runeswords", POCLASS_WEAPON, 80, ')', 20, 1, 1
	},
	[PO_BOW] =
	{
		"bow", "bows", POCLASS_WEAPON, 45, '(', 8, 1, 1
	},
	[PO_CROSSBOW] =
	{
		"crossbow", "crossbows", POCLASS_WEAPON, 70, '(', 16, 1, 1
	},
	[PO_POT_HEAL] =
	{
		"healing potion", "healing potions", POCLASS_POTION, 10, '!', 0, 1, 0
	},
	[PO_POT_POISON] =
	{
		"poison potion", "poison potions", POCLASS_POTION, 10, '!', 0, 1, 0
	},
	[PO_POT_BODY] =
	{
		"body potion", "body potions", POCLASS_POTION, 50, '!', 0, 1, 0
	},
	[PO_POT_AGILITY] =
	{
		"agility potion", "agility potions", POCLASS_POTION, 50, '!', 0, 1, 0
	},
	[PO_POT_WEAKNESS] =
	{
		"weakness potion", "weakness potions", POCLASS_POTION, 40, '!', 0, 1, 0
	},
	[PO_SCR_TELEPORT] =
	{
		"teleport scroll", "teleport scrolls", POCLASS_SCROLL, 40, '?', 0, 1, 0
	},
	[PO_SCR_FIRE] =
	{
		"fire scroll", "fire scrolls", POCLASS_SCROLL, 30, '?', 0, 1, 0
	},
	[PO_SCR_MONSTERS] =
	{
		"summoning scroll", "summoning scrolls", POCLASS_SCROLL, 30, '?', 0, 1, 0
	},
	[PO_LEATHER_ARMOUR] =
	{
		"leather armour", "suits of leather armour", POCLASS_ARMOUR, 15, '[', 3, 1, 1
	},
	[PO_CHAINMAIL] =
	{
		"chainmail", "suits of chainmail", POCLASS_ARMOUR, 20, '[', 6, 1, 1
	},
	[PO_PLATE_ARMOUR] =
	{
		"plate armour", "suits of plate armour", POCLASS_ARMOUR, 40, '[', 10, 1, 1
	},
	[PO_MAGE_ARMOUR] =
	{
		"mage armour", "suits of mage armour", POCLASS_ARMOUR, 70, '[', 15, 1, 1
	},
	[PO_RING_REGEN] =
	{
		"regeneration ring", "regeneration rings", POCLASS_RING, 70, '=', 0, 1, 0
	},
	[PO_RING_FIRE] =
	{
		"fire ring", "fire rings", POCLASS_RING, 50, '=', 0, 1, 0
	},
	[PO_RING_WEDDING] =
	{
		"wedding ring", "wedding rings", POCLASS_RING, 20, '=', 0, 1, 0
	},
	[PO_RING_VAMPIRE] =
	{
		"vampire ring", "vampire rings", POCLASS_RING, 90, '=', 0, 1, 0
	},
	[PO_RING_FROST] =
	{
		"frost ring", "frost rings", POCLASS_RING, 40, '=', 0, 1, 0
	},
	[PO_RING_DOOM] =
	{
		"doom ring", "doom rings", POCLASS_RING, 80, '=', 0, 1, 0
	},
	[PO_GOLD] =
	{
		"gold piece", "gold pieces", POCLASS_NONE, 1, '$', 0, 1, 1
	},
};

