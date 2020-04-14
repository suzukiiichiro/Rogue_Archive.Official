/* permobj.c
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

#define PERMOBJ_C
#include "dunbash.h"

struct permobj permobjs[NUM_OF_PERMOBJS] = {
	[PO_DAGGER] =
	{
		"dagger", "daggers", "A long knife, designed for stabbing.", POCLASS_WEAPON, 25, ')', 4, 1, 1, 1
	},
	[PO_LONG_SWORD] =
	{
		"long sword", "long swords", "A steel sword of simple but sturdy construction; the\nblade is three feet long.", POCLASS_WEAPON, 30, ')', 10, 1, 1, 4
	},
	[PO_MACE] =
	{
		"mace", "maces", "A flanged lump of iron on an iron haft.", POCLASS_WEAPON, 30, ')', 7, 1, 1, 2
	},
	[PO_RUNESWORD] =
	{
		"runesword", "runeswords", "An eerily glowing sword engraved with many strange\nrunes.", POCLASS_WEAPON, 80, ')', 20, 1, 1, 12
	},
	[PO_BOW] =
	{
		"bow", "bows", "A recurve composite bow.", POCLASS_WEAPON, 45, '(', 8, 1, 1, 1
	},
	[PO_CROSSBOW] =
	{
		"crossbow", "crossbows", "A crossbow.", POCLASS_WEAPON, 70, '(', 16, 1, 1, 6
	},
	[PO_POT_HEAL] =
	{
		"healing potion", "healing potions", "This magic elixir restores some lost hit points.", POCLASS_POTION, 10, '!', 0, 1, 0, 1
	},
	[PO_POT_POISON] =
	{
		"poison potion", "poison potions", "This liquid is poisonous.", POCLASS_POTION, 10, '!', 0, 1, 0, 1
	},
	[PO_POT_BODY] =
	{
		"body potion", "body potions", "This magic elixir will improve your physique.", POCLASS_POTION, 70, '!', 0, 1, 0, 5
	},
	[PO_POT_AGILITY] =
	{
		"agility potion", "agility potions", "This magic elixir will sharpen your reflexes.", POCLASS_POTION, 70, '!', 0, 1, 0, 5
	},
	[PO_POT_WEAKNESS] =
	{
		"weakness potion", "weakness potions", "This magic elixir causes physical degeneration in\nwhoever drinks it.", POCLASS_POTION, 40, '!', 0, 1, 0, 1
	},
	[PO_POT_RESTORATION] =
	{
		"restoration potion", "restoration potions", "This magic elixir cures temporary damage to one's\nabilities.", POCLASS_POTION, 70, '!', 0, 1, 0, 1
	},
	[PO_SCR_TELEPORT] =
	{
		"teleport scroll", "teleport scrolls", "Reading this scroll will teleport you to a random\nlocation.", POCLASS_SCROLL, 40, '?', 0, 1, 0, 1
	},
	[PO_SCR_FIRE] =
	{
		"fire scroll", "fire scrolls", "Reading this scroll will engulf you in flames.", POCLASS_SCROLL, 30, '?', 0, 1, 0, 1
	},
	[PO_SCR_MONSTERS] =
	{
		"summoning scroll", "summoning scrolls", "Reading this scroll will summon hostile monsters to\nyour side.", POCLASS_SCROLL, 30, '?', 0, 1, 0, 1
	},
	[PO_SCR_IDENTIFY] =
	{
		"identify scroll", "identify scrolls", "Reading this scroll will reveal the nature of your\npossessions.", POCLASS_SCROLL, 70, '?', 0, 1, 0, 3
	},
	[PO_SCR_AGGRAVATE] =
	{
		"aggravating scroll", "aggravating scrolls", "Reading this scroll will awaken every monster on the\nlevel.", POCLASS_SCROLL, 50, '?', 0, 1, 0, 3
	},
	[PO_SCR_PROTECTION] =
	{
		"protection scroll", "protection scrolls", "Reading this scroll will dispel any curses afflicting\nyou and protect you from curses for a time.", POCLASS_SCROLL, 80, '?', 0, 1, 0, 8
	},
	[PO_LEATHER_ARMOUR] =
	{
		"leather armour", "suits of leather armour", "A heavy leather jerkin and breeches, providing some\nprotection.", POCLASS_ARMOUR, 25, '[', 3, 1, 1, 1
	},
	[PO_CHAINMAIL] =
	{
		"chainmail", "suits of chainmail", "A suit of interlocking metal rings, providing better\nprotection than leather.", POCLASS_ARMOUR, 30, '[', 6, 1, 1, 3
	},
	[PO_PLATE_ARMOUR] =
	{
		"plate armour", "suits of plate armour", "A suit of steel plates, providing better protection than\nchainmail.", POCLASS_ARMOUR, 40, '[', 10, 1, 1, 6
	},
	[PO_MAGE_ARMOUR] =
	{
		"mage armour", "suits of mage armour", "A suit of glowing steel plates bearing enchantments of\ndefence.", POCLASS_ARMOUR, 70, '[', 15, 1, 1, 12
	},
	[PO_ROBE] =
	{
		"mundane robe", "mundane robes", "A simple woolen robe. It's better protection than your\nskin, but not by much.", POCLASS_ARMOUR, 50, '[', 2, 1, 0, 1
	},
	[PO_ROBE_SWIFTNESS] =
	{
		"robe of swiftness", "robes of swiftness", "A simple woolen robe that bears a potent enchantment,\nprotecting the wearer and making him unnaturally swift.", POCLASS_ARMOUR, 70, '[', 8, 1, 0, 8
	},
	[PO_ROBE_SHADOWS] =
	{
		"robe of shadows", "robes of shadows", "A simple woolen robe that bears an awesome enchantment,\nprotecting the wearer better than steel plate.", POCLASS_ARMOUR, 90, '[', 14, 1, 0, 18
	},
	[PO_DRAGON_ARMOUR] =
	{
		"dragonhide armour", "suits of dragonhide armour", "The skin of a dragon, formed into a jerkin and breeches;\nit turns blows like steel plate and turns away\nflames.", POCLASS_ARMOUR, 90, '[', 12, 1, 1, 21
	},
	[PO_METEOR_ARMOUR] =
	{
		"meteoric plate armour", "suits of meteoric plate armour", "This plate armour has been forged out of metal taken from\na fallen star.", POCLASS_ARMOUR, 90, '[', 18, 1, 1, 27
	},
	[PO_SACRED_MAIL] =
	{
		"sacred chainmail", "suits of sacred chainmail", "This suit of interlocking rings has been consecrated to\nthe gods of the Light.", POCLASS_ARMOUR, 90, '[', 15, 1, 1, 24
	},
	[PO_RING_REGEN] =
	{
		"regeneration ring", "regeneration rings", "This magical ring increases the wearer's healing rate,\nbut also increases the rate at which they must consume\nfood.", POCLASS_RING, 70, '=', 0, 1, 0, 1
	},
	[PO_RING_FIRE] =
	{
		"fire ring", "fire rings", "This magical ring protects the wearer from mundane and\nmagical fire, and imbues their blows in combat with the\npower of fire.", POCLASS_RING, 50, '=', 0, 1, 0, 1
	},
	[PO_RING_WEDDING] =
	{
		"wedding ring", "wedding rings", "This ring is but a simple love-token.", POCLASS_RING, 20, '=', 0, 1, 0, 1
	},
	[PO_RING_VAMPIRE] =
	{
		"vampire ring", "vampire rings", "This magical ring protects the wearer from necromantic\nenergies, and imbues their blows in combat with such\nenergies as well.", POCLASS_RING, 90, '=', 0, 1, 0, 12
	},
	[PO_RING_FROST] =
	{
		"frost ring", "frost rings", "This magical ring protects the wearer from mundane and\nmagical cold, and imbues their blows in combat with the\npower of cold.", POCLASS_RING, 40, '=', 0, 1, 0, 1
	},
	[PO_RING_DOOM] =
	{
		"doom ring", "doom rings", "This accursed ring inflicts great misery on its wearer.\nIt is said that even its removal is painful.", POCLASS_RING, 80, '=', 0, 1, 0, 1
	},
	[PO_RING_TELEPORT] =
	{
		"teleport ring", "teleport rings", "This magical ring causes the wearer to teleport at\nrandom.", POCLASS_RING, 70, '=', 0, 1, 0, 1
	},
	[PO_IRON_RATION] =
	{
		"iron ration", "iron rations", "A parcel of hardtack and beef jerky. Dull but nutritious.", POCLASS_FOOD, 75, '%', 0, 1, 1, 1
	},
	[PO_DRIED_FRUIT] =
	{
		"parcel of dried fruit", "parcels of dried fruit", "A parcel of mixed dried fruit. It sure beats hardtack\nand beef jerky.", POCLASS_FOOD, 75, '%', 0, 1, 1, 1
	},
	[PO_ELVEN_BREAD] =
	{
		"round of elven waybread", "rounds of elven waybread", "A tasty, filling, nutritious piece of elven waybread.", POCLASS_FOOD, 85, '%', 0, 1, 1, 1
	},
	[PO_GOLD] =
	{
		"gold piece", "gold pieces", "The wealth of ages. Not that it will do you much good.", POCLASS_NONE, 1, '$', 0, 1, 1, 1
	},
};

/* permobj.c */
