/* permons.c
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

#define PERMONS_C
#include "dunbash.h"

/* This array is no longer necessarily defined in sequential order. Instead,
 * it has been defined in order of increasing power within "monster groups",
 * as follows:
 *   Wild Animals
 *   Evil Men
 *   Evil Humanoids
 *   Undead
 *   Demonkind
 *   Supernatural Beasts
 */
struct permon permons[NUM_OF_PERMONS] = {
	[PM_NEWT] =
	{
		"newt", "newts", 'n', DBCLR_RED, 20, 1, 3, 0, -1, 2, -1, DT_PHYS, "", 1, 1, 0, PMF_STUPID
	},
	[PM_RAT] =
	{
		"rat", "rats", 'r', DBCLR_BROWN, 15, 1, 4, 0, -1, 2, -1, DT_PHYS, "", 4, 2, 2, PMF_STUPID
	},
	[PM_WOLF] =
	{
		"wolf", "wolves", 'c', DBCLR_BROWN, 30, 6, 20, 8, -1, 10, -1, DT_PHYS, "", 6, 15, 2, 0
	},
	[PM_SNAKE] =
	{
		/* Saps one Body on a really good hit */
		"snake", "snakes", 's', DBCLR_RED, 20, 6, 15, 10, -1, 3, -1, DT_PHYS, "", 9, 40, 2, PMF_STUPID
	},
	[PM_THUG] =
	{
		/* may drop a mace or leather armour */
		"thug", "thugs", 't', DBCLR_BROWN, 30, 1, 8, 5, -1, 5, -1, DT_PHYS, "", 4, 5, 1, 0
	},
	[PM_GOON] =
	{
		"goon", "goons", 't', DBCLR_YELLOW, 20, 3, 20, 6, -1, 10, -1, DT_PHYS, "", 8, 10, 1, 0
	},
	[PM_HUNTER] =
	{
		/* Has a ranged attack - arrows */
		"hunter", "hunters", 'h', DBCLR_GREEN, 30, 9, 40, 6, 20, 6, 10, DT_PHYS, "shoots an arrow", 10, 50, 1, PMF_ARCHER
	},
	[PM_DUELLIST] =
	{
		"duellist", "duellists", 'f', DBCLR_RED, 40, 12, 60, 30, -1, 15, -1, DT_PHYS, "", 15, 130, 1, PMF_SMART
	},
	[PM_WARLORD] =
	{
		"warlord", "warlords", 'f', DBCLR_L_RED, 30, 15, 80, 25, -1, 20, -1, DT_PHYS, "", 20, 400, 2, PMF_SMART
	},
	[PM_GOBLIN] =
	{
		/* may drop a dagger */
		"goblin", "goblins", 'g', DBCLR_BROWN, 20, 1, 6, 1, -1, 3, -1, DT_PHYS, "", 3, 3, 1, 0
	},
	[PM_BAD_ELF] =
	{
		"bad elf", "bad elves", 'e', DBCLR_L_GREY, 40, 3, 15, 10, -1, 6, -1, DT_PHYS, "", 8, 15, 2, PMF_SMART
	},
	[PM_TROLL] =
	{
		"troll", "trolls", 'T', DBCLR_GREEN, 20, 12, 80, 15, -1, 15, -1, DT_PHYS, "", 13, 150, 1, PMF_STUPID
	},
	[PM_GIANT] =
	{
		"giant", "giants", 'H', DBCLR_BROWN, 20, 21, 80, 15, -1, 25, -1, DT_PHYS, "", 20, 500, 1, PMF_STUPID
	},
	[PM_GIANT_JARL] =
	{
		"giant jarl", "giant jarls", 'H', DBCLR_L_GREY, 80, 25, 160, 20, -1, 30, -1, DT_PHYS, "", 22, 1000, 1, 0
	},
	[PM_WIZARD] =
	{
		/* Uses black magic against you; see bmagic.c for details. */
		"wizard", "wizards", 'w', DBCLR_BLUE, 80, 12, 40, 10, 20, 10, 10, DT_ELEC, "casts", 15, 200, 1, PMF_SMART | PMF_MAGICIAN
	},
	[PM_ARCHMAGE] =
	{
		/* Uses black magic against you; see bmagic.c for details. */
		"archmage", "archmagi", 'w', DBCLR_L_BLUE, 80, 24, 80, 15, 30, 15, 15, DT_ELEC, "casts", 15, 1500, 1, PMF_SMART | PMF_MAGICIAN
	},
	[PM_ZOMBIE] =
	{
		"zombie", "zombies", 'z', DBCLR_L_GREY, 25, 3, 30, 2, -1, 20, -1, DT_PHYS, "", 1, 7, 0, PMF_STUPID | PMF_UNDEAD | PMF_RESIST_COLD
	},
	[PM_WRAITH] =
	{
		"wraith", "wraiths", 'W', DBCLR_WHITE, 25, 12, 40, 25, -1, 5, -1, DT_PHYS, "", 5, 100, 0, PMF_SMART | PMF_UNDEAD | PMF_RESIST_COLD
	},
	[PM_LICH] =
	{
		/* Uses black magic against you; see bmagic.c for details. */
		"lich", "liches", 'L', DBCLR_L_GREY, 70, 15, 70, 15, 25, 15, 15, DT_NECRO, "casts", 15, 250, 1, PMF_SMART | PMF_UNDEAD | PMF_RESIST_COLD | PMF_MAGICIAN
	},
	[PM_VAMPIRE] =
	{
		/* Vampires heal by hitting you. */
		"vampire", "vampires", 'V', DBCLR_RED, 55, 18, 70, 25, -1, 15, -1, DT_PHYS, "", 22, 750, 1, PMF_SMART | PMF_UNDEAD | PMF_RESIST_COLD
	},
	[PM_MASTER_LICH] =
	{
		/* Master liches use black magic against you, more powerfully
		 * than lesser practitioners. */
		"master lich", "master liches", 'L', DBCLR_PURPLE, 60, 30, 150, 30, 30, 20, 30, DT_NECRO, "", 30, 3000, 1, PMF_SMART | PMF_UNDEAD | PMF_RESIST_COLD | PMF_MAGICIAN
	},
	[PM_DEMON] =
	{
		/* Demons summon more demons if you don't kill them
		 * quickly. */
		"demon", "demons", '&', DBCLR_RED, 60, 18, 40, 25, -1, 20, -1, DT_PHYS, "", 15, 500, 1, PMF_SMART | PMF_DEMONIC | PMF_RESIST_FIRE
	},
	[PM_DEFILER] =
	{
		/* Defilers use black magic against you. */
		"defiler", "defilers", '&', DBCLR_L_GREEN, 65, 27, 120, 30, 30, 20, 30, DT_FIRE, "", 25, 2000, 1, PMF_SMART | PMF_DEMONIC | PMF_RESIST_FIRE | PMF_MAGICIAN
	},
	[PM_CENTAUR] =
	{
		"centaur", "centaurs", 'C', DBCLR_BROWN, 30, 9, 40, 15, -1, 10, -1, DT_PHYS, "", 10, 50, 2, 0
	},
	[PM_ICE_MONSTER] =
	{
		/* Fires ice blasts. */
		"ice monster", "ice monsters", 'I', DBCLR_WHITE, 50, 6, 40, 10, 20, 15, 15, DT_COLD, "launches a blast of", 10, 35, 0, PMF_RESIST_COLD | PMF_ARCHER
	},
	[PM_DRAGON] =
	{
		/* Breathes fire. */
		"dragon", "dragons", 'D', DBCLR_RED, 50, 15, 80, 20, 20, 20, 20, DT_FIRE, "breathes", 18, 300, 1, PMF_RESIST_FIRE | PMF_ARCHER
	},
};

/* permons.c */
//あ
