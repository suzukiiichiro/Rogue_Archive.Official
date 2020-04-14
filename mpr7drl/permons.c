/* permons.c */
#define PERMONS_C
#include "7drl.h"

struct permon permons[100] = {
	[PM_NEWT] =
	{
		"newt", "newts", 'n', 20, 1, 3, 0, 2, 1, 1, 0
	},
	[PM_GOBLIN] =
	{
		/* may drop a dagger */
		"goblin", "goblins", 'g', 20, 1, 6, 1, 3, 3, 3, 1
	},
	[PM_RAT] =
	{
		"rat", "rats", 'r', 15, 1, 4, 0, 2, 4, 2, 2
	},
	[PM_THUG] =
	{
		/* may drop a mace or leather armour */
		"thug", "thugs", 't', 30, 1, 8, 1, 5, 6, 5, 1
	},
	[PM_BAD_ELF] =
	{
		"bad elf", "bad elves", 'e', 40, 3, 15, 4, 6, 12, 15, 2
	},
	[PM_GOON] =
	{
		"goon", "goons", 'G', 20, 3, 20, 2, 10, 8, 10, 1
	},
	[PM_ZOMBIE] =
	{
		"zombie", "zombies", 'z', 25, 3, 30, -2, 20, 5, 7, 0
	},
	[PM_WOLF] =
	{
		"wolf", "wolves", 'w', 30, 6, 20, 3, 10, 10, 15, 2
	},
	[PM_ICE_MONSTER] =
	{
		/* Fires ice blasts (ring of frost resists) */
		"ice monster", "ice monsters", 'I', 50, 6, 40, 4, 15, 20, 35, 0
	},
	[PM_SNAKE] =
	{
		/* Saps one Body on a perfect hit */
		"snake", "snakes", 's', 20, 6, 15, 8, 3, 10, 40, 2
	},
	[PM_HUNTER] =
	{
		/* Has a ranged attack - arrows */
		"hunter", "hunters", 'h', 30, 9, 40, 6, 10, 15, 50, 1
	},
	[PM_CENTAUR] =
	{
		"centaur", "centaurs", 'c', 30, 9, 40, 4, 10, 15, 50, 2 
	},
	[PM_TROLL] =
	{
		"troll", "trolls", 'T', 20, 12, 80, 7, 15, 15, 150, 1
	},
	[PM_DUELLIST] =
	{
		"duellist", "duellists", 'd', 40, 12, 60, 10, 15, 20, 130, 1
	},
	[PM_WIZARD] =
	{
		/* Casts lightning bolts (unresistable). */
		"wizard", "wizards", 'w', 80, 12, 40, 10, 10, 15, 200, 1
	},
	[PM_DRAGON] =
	{
		/* Breathes fire (ring of fire resists). */
		"dragon", "dragons", 'D', 50, 15, 80, 15, 20, 20, 300, 1
	},
	[PM_LICH] =
	{
		/* Casts bolts of death magic (vampire ring resists). */
		"lich", "liches", 'L', 70, 15, 70, 15, 15, 15, 250, 1
	},
	[PM_WARLORD] =
	{
		"warlord", "warlords", 'W', 30, 15, 80, 20, 20, 20, 400, 2
	},
	[PM_DEMON] =
	{
		/* Demons summon more demons if you don't kill them
		 * quickly. */
		"demon", "demons", '&', 60, 18, 40, 20, 20, 20, 500, 1
	},
};

/* permons.c */
