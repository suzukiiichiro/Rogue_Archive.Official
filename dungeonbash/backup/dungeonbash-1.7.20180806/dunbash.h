/* dunbash.h
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

#ifndef DUNBASH_H
#define DUNBASH_H

#include <stdlib.h>
#include <stdio.h>

/* change WIZARD_MODE to 1 if you want the wizard mode commands. */
#define WIZARD_MODE 0

#define is_vowel(ch) (((ch) == 'a') || ((ch) == 'e') || ((ch) == 'i') || ((ch) == 'o') || ((ch) == 'u'))

/* XXX ENUMERATED TYPES XXX */

/* XXX enum damtyp - types of damage. */
enum damtyp {
    DT_PHYS = 0, DT_COLD, DT_FIRE, DT_NECRO, DT_ELEC, DT_POISON, DT_COUNT
};

/* XXX enum game_cmd - player actions. */
enum game_cmd {
    DROP_ITEM, SAVE_GAME, SHOW_INVENTORY, MOVE_WEST, MOVE_SOUTH,
    MOVE_NORTH, MOVE_EAST, MOVE_NW, MOVE_NE, MOVE_SW, MOVE_SE,
    QUAFF_POTION, READ_SCROLL, WIELD_WEAPON, WEAR_ARMOUR,
    TAKE_OFF_ARMOUR, PUT_ON_RING, REMOVE_RING, GIVE_HELP,
    ATTACK, GET_ITEM, QUIT, GO_DOWN_STAIRS, STAND_STILL, EAT_FOOD,
    DUMP_CHARA, INSPECT_ITEM, EXAMINE_MONSTER, RNG_TEST, SHOW_TERRAIN,
    SHOW_DISCOVERIES, WIZARD_LEVELUP, WIZARD_DESCEND
};

/* XXX enum terrain_num */
/* To start with, only four types of terrain: Wall, Floor, Door, Stairs. */
enum terrain_num {
    WALL = 0, FLOOR = 1, DOOR = 2, STAIRS = 3
};

/* XXX enum death */
/* Sadly, there are not yet 52 kinds of way to die, only four: killed by
 * an arbitrary string, killed by a monster, drained of body by an arbitrary
 * string, and drained of agility by an arbitrary string. */
enum death {
    DEATH_KILLED, DEATH_KILLED_MON, DEATH_BODY, DEATH_AGILITY
};

/* XXX enum poclass_num */
/* Categories of permanent object. */
enum poclass_num {
    POCLASS_NONE = 0, POCLASS_WEAPON = 1, POCLASS_POTION = 2,
    POCLASS_SCROLL = 3, POCLASS_ARMOUR = 4, POCLASS_RING = 5,
    POCLASS_FOOD = 6
};

#define RESIST_MASK_TEMPORARY	0x0000FFFFu
#define RESIST_MASK_PERM_EQUIP	0xFFFF0000u
#define RESIST_RING	0x00010000u
#define RESIST_ARMOUR	0x00020000u
/* XXX STRUCTURES XXX */

/* XXX struct player */
struct player {
    char name[17];	/* including '\0' the fencepost. */
    int y;		/* y-coord */
    int x;		/* x-coord */
    int body;		/* determines mace to-hit, melee damage, max 99 */
    int bdam;		/* current level of temporary body drain. */
    int agility;	/* determines sword, dagger, missile to-hit, max 99 */
    int adam;		/* current level of temporary agility drain. */
    int hpmax;		/* Max hit points; max of 999. */
    int hpcur;		/* Current hit points; <= 0 is dead. */
    int food;		/* Current nutrition in body; < 0 is hungry. */
    int experience;	/* Experience points earned. */
    int defence;	/* To-hit target number for monsters */
    int protection;	/* Temporary protection from cursing */
    int leadfoot;	/* Feet-of-lead curse */
    int withering;	/* Vile withering curse */
    int armourmelt;	/* Armour-like-dust curse */
    int speed;
    unsigned int resistances[DT_COUNT];	/* Resistances to damage types. */
    int level;	/* Each level gets you +1 body, +1 agility, +1 random
		   point, and +(10+body/10) hit points */
    int gold;
    int inventory[19];	/* 19 inventory slots, leaving room for a prompt */
    int weapon;		/* For now, you can only wield one weapon. */
    int armour;		/* For now, you can only wear one item of armour. */
    int ring;		/* For now, you can only wear one magic ring. */
};

#define DBCLR_L_GREY	0
#define DBCLR_D_GREY	1
#define DBCLR_RED	2
#define DBCLR_BLUE	3
#define DBCLR_GREEN	4
#define DBCLR_PURPLE	5
#define DBCLR_BROWN	6
#define DBCLR_CYAN	7
#define DBCLR_WHITE	8
#define DBCLR_L_RED	9
#define DBCLR_L_BLUE	10
#define DBCLR_L_GREEN	11
#define DBCLR_L_PURPLE	12
#define DBCLR_YELLOW	13
#define DBCLR_L_CYAN	14

/* XXX struct permon */
#define PM_NEWT 0
#define PM_RAT (PM_NEWT + 1)
#define PM_WOLF (PM_RAT + 1)
#define PM_SNAKE (PM_WOLF + 1)
#define PM_THUG (PM_SNAKE + 1)
#define PM_GOON (PM_THUG + 1)
#define PM_HUNTER (PM_GOON + 1)
#define PM_DUELLIST (PM_HUNTER + 1)
#define PM_WARLORD (PM_DUELLIST + 1)
#define PM_WIZARD (PM_WARLORD + 1)
#define PM_ARCHMAGE (PM_WIZARD + 1)
#define PM_GOBLIN (PM_ARCHMAGE + 1)
#define PM_BAD_ELF (PM_GOBLIN + 1)
#define PM_TROLL (PM_BAD_ELF + 1)
#define PM_GIANT (PM_TROLL + 1)
#define PM_GIANT_JARL (PM_GIANT + 1)
#define PM_ZOMBIE (PM_GIANT_JARL + 1)
#define PM_WRAITH (PM_ZOMBIE + 1)
#define PM_LICH (PM_WRAITH + 1)
#define PM_VAMPIRE (PM_LICH + 1)
#define PM_MASTER_LICH (PM_VAMPIRE + 1)
#define PM_DEMON (PM_MASTER_LICH + 1)
#define PM_DEFILER (PM_DEMON + 1)
#define PM_ICE_MONSTER (PM_DEFILER + 1)
#define PM_CENTAUR (PM_ICE_MONSTER + 1)
#define PM_DRAGON (PM_CENTAUR + 1)
#define PM_REAL_COUNT ((PM_DRAGON) + 1)

#define PMF_RESIST_FIRE	0x00000001
#define PMF_RESIST_COLD	0x00000002
#define PMF_RESIST_ELEC	0x00000004
#define PMF_UNDEAD	0x00010000
#define PMF_DEMONIC	0x00020000
#define PMF_MAGICIAN	0x00040000
#define PMF_ARCHER	0x00080000
#define PMF_SMART	0x00100000
#define PMF_STUPID	0x00200000

struct permon {
    const char name[48];
    const char plural[48];
    char sym;
    int colour;
    int rarity;		/* Chance in 100 of being thrown back and regen'd. */
    int power;		/* Used to determine OOD rating. */
    /* All OOD-improved stats cap out at base + (power * base) */
    int hp;		/* Improved by OOD rating at 1:1. */
    int mtohit;		/* Improved by OOD rating at 1:3. */
    int rtohit;		/* Improved by OOD rating at 1:3. */
    int mdam;		/* Improved by OOD rating at 1:5. */
    int rdam;		/* Improved by OOD rating at 1:5. */
    enum damtyp rdtyp;	/* type of damage used by ranged attack. */
    const char shootverb[48];	/* shooting verb e.g. "fires an arrow", "breathes". */
    int defence;	/* Improved by OOD rating at 1:3. */
    int exp;		/* Unaffected by OOD rating. */
    int speed;		/* 0 = slow; 1 = normal; 2 = quick */
    int flags;		/* resistances, AI settings, etc. */
};

/* XXX struct permobj */
#define PO_DAGGER 0
#define PO_FIRST_WEAPON PO_DAGGER
#define PO_LONG_SWORD (PO_FIRST_WEAPON + 1)
#define PO_MACE (PO_FIRST_WEAPON + 2)
#define PO_RUNESWORD (PO_FIRST_WEAPON + 3)
#define PO_BOW (PO_FIRST_WEAPON + 4)
#define PO_CROSSBOW (PO_FIRST_WEAPON + 5)
#define PO_LAST_WEAPON PO_CROSSBOW
#define PO_POT_HEAL (PO_LAST_WEAPON + 1)
#define PO_FIRST_POTION PO_POT_HEAL
#define PO_POT_POISON (PO_FIRST_POTION + 1)
#define PO_POT_BODY (PO_FIRST_POTION + 2)
#define PO_POT_AGILITY (PO_FIRST_POTION + 3)
#define PO_POT_WEAKNESS (PO_FIRST_POTION + 4)
#define PO_POT_RESTORATION (PO_FIRST_POTION + 5)
#define PO_LAST_POTION PO_POT_RESTORATION
#define PO_SCR_TELEPORT (PO_LAST_POTION + 1)
#define PO_FIRST_SCROLL PO_SCR_TELEPORT
#define PO_SCR_FIRE (PO_FIRST_SCROLL + 1)
#define PO_SCR_MONSTERS (PO_FIRST_SCROLL + 2)
#define PO_SCR_IDENTIFY (PO_FIRST_SCROLL + 3)
#define PO_SCR_AGGRAVATE (PO_FIRST_SCROLL + 4)
#define PO_SCR_PROTECTION (PO_FIRST_SCROLL + 5)
#define PO_LAST_SCROLL PO_SCR_PROTECTION
#define PO_LEATHER_ARMOUR (PO_LAST_SCROLL + 1)
#define PO_FIRST_ARMOUR PO_LEATHER_ARMOUR
#define PO_CHAINMAIL (PO_FIRST_ARMOUR + 1)
#define PO_PLATE_ARMOUR (PO_FIRST_ARMOUR + 2)
#define PO_MAGE_ARMOUR (PO_FIRST_ARMOUR + 3)
#define PO_ROBE (PO_FIRST_ARMOUR + 4)
#define PO_ROBE_SWIFTNESS (PO_FIRST_ARMOUR + 5)
#define PO_ROBE_SHADOWS (PO_FIRST_ARMOUR + 6)
#define PO_DRAGON_ARMOUR (PO_FIRST_ARMOUR + 7)
#define PO_METEOR_ARMOUR (PO_FIRST_ARMOUR + 8)
#define PO_SACRED_MAIL (PO_FIRST_ARMOUR + 9)
#define PO_LAST_ARMOUR PO_SACRED_MAIL
#define PO_RING_REGEN (PO_LAST_ARMOUR + 1)
#define PO_FIRST_RING PO_RING_REGEN
#define PO_RING_FIRE (PO_FIRST_RING + 1)
#define PO_RING_WEDDING (PO_FIRST_RING + 2)
#define PO_RING_VAMPIRE (PO_FIRST_RING + 3)
#define PO_RING_FROST (PO_FIRST_RING + 4)
#define PO_RING_DOOM (PO_FIRST_RING + 5)
#define PO_RING_TELEPORT (PO_FIRST_RING + 6)
#define PO_LAST_RING PO_RING_TELEPORT
#define PO_IRON_RATION (PO_LAST_RING + 1)
#define PO_FIRST_FOOD PO_IRON_RATION
#define PO_DRIED_FRUIT (PO_FIRST_FOOD + 1)
#define PO_ELVEN_BREAD (PO_FIRST_FOOD + 2)
#define PO_LAST_FOOD PO_ELVEN_BREAD
#define PO_GOLD (PO_LAST_FOOD + 1)
#define PO_REAL_COUNT ((PO_GOLD) + 1)

struct permobj {
    const char name[48];
    const char plural[48];
    const char description[160];
    enum poclass_num poclass;
    int rarity;	/* Chance in 100 of being thrown away and regen'd. */
    int sym;
    int power;	/* AC for armour; damage for weapons; colour/title for
		 * scrolls and potions and rings and such. */
    int used;	/* Set to 1 for valid entries. */
    int known;	/* Set to 1 for items recognised at startup. Updated
		 * during play when items identified. */
    int depth;	/* If greater than 1, this item cannot be given out
		 * by get_random_pobj() before the specified depth. */
};

/* XXX struct mon */
struct mon {
    int mon_id;
    int y;
    int x;
    int ai_lasty;	/* AI's belief about your last position. -1 == lost you. */
    int ai_lastx;	/* AI's belief about your last position. -1 == lost you. */
    int used;
    int hpmax;	/* Improved by OOD rating at 1:1. */
    int hpcur;	/* <= 0 is dead. */
    int mtohit;	/* Improved by OOD rating at 1:3. */
    int rtohit;	/* Improved by OOD rating at 1:3. */
    int defence;	/* Improved by OOD rating at 1:3. */
    int mdam;	/* Improved by OOD rating at 1:5. */
    int rdam;	/* Improved by OOD rating at 1:5. */
    int awake;
};

/* XXX struct obj */
struct obj {
    int obj_id;
    int quan;
    int with_you;	/* Preserved when item DB is reaped on level change. */
    int y;
    int x;
    int used;	/* Entry is occupied. */
    int durability;	/* Weapons and armour degrade with use. */
};

#define OBJ_MAX_DUR 100
/* XXX Object/monster arrays */
#define NUM_OF_PERMOBJS 300
extern struct permobj permobjs[NUM_OF_PERMOBJS];

/* Treasure generation limit plus inventory limit means this should be
 * enough.  When I evolve to the "fully-fledged" version, I'll use a Tatham
 * tree instead. */
extern struct mon monsters[100];

/* XXX display.c data and funcs */
extern int read_input(char *buffer, int length);
extern void print_msg(const char *fmt, ...);
extern void print_help(void);
extern int display_init(void);
extern void display_update(void);
extern int display_shutdown(void);
extern void newsym(int y, int x);
extern void touch_back_buffer(void);
extern void print_inv(enum poclass_num filter);
extern int inv_select(enum poclass_num filter, const char *action, int accept_blank);
extern enum game_cmd get_command(void);
extern int select_dir(int *psy, int *psx);
extern int getYN(const char *msg);
extern int getyn(const char *msg);
extern void press_enter(void);
extern void pressanykey(void);
extern void show_discoveries(void);

/* "I've changed things that need to be redisplayed" flags. */
extern int hard_redraw;
extern int status_updated;
extern int map_updated;
/* "Show the player the terrain only" flag. */
extern int show_terrain;

/* XXX main.c data and funcs */
extern int exclusive_flat(int lower, int upper); /* l+1 ... u-1 */
extern int inclusive_flat(int lower, int upper); /* l ... u */
extern int one_die(int sides);	/* 1..n */
extern int dice(int count, int sides);
extern int zero_die(int sides);	/* 0..n-1 */
extern int do_command(enum game_cmd command);
extern unsigned int convert_range(int dy, int dx);
extern int game_finished;
extern int game_tick;
extern int wizard_mode;

/* XXX map.c data and funcs*/
extern void leave_level(void);
extern void make_new_level(void);
extern void build_level(void);
extern void populate_level(void);
extern void inject_player(void);
extern int get_room_x(int room);
extern int get_room_y(int room);
extern void room_reset(void);

/* For now, I can't be arsed with a mapcell structure */
#define DUN_WIDTH 42
#define DUN_HEIGHT 42
#define ROOM_HT_DELTA 4
#define ROOM_WD_DELTA 4
#define MAX_ROOMS 9

extern int mapobject[DUN_HEIGHT][DUN_WIDTH];
extern int mapmonster[DUN_HEIGHT][DUN_WIDTH];
extern enum terrain_num terrain[DUN_HEIGHT][DUN_WIDTH];
#define MAPFLAG_EXPLORED 0x00000001
extern int mapflags[DUN_HEIGHT][DUN_WIDTH];
extern int roomnums[DUN_HEIGHT][DUN_WIDTH];
extern int roombounds[MAX_ROOMS][4];
extern int roomlinkage[MAX_ROOMS][MAX_ROOMS];
extern int depth;

/* XXX misc.c data and funcs */
extern const char *damtype_names[DT_COUNT];

/* XXX objects.c data and funcs */
extern void flavours_init(void);
extern void fprint_obj_name(FILE *fp, int obj);
extern void print_obj_name(int obj);
extern void describe_object(int obj);
extern int create_obj(int po_idx, int quantity, int with_you, int y, int x);
extern int drop_obj(int inv_idx);
extern int create_obj_class(enum poclass_num pocl, int quantity, int with_you, int y, int x);
extern int create_obj_random(int y, int x);
extern struct obj objects[100];	/* SHould be enough. */
extern int read_scroll(int obj);
extern int quaff_potion(int obj);
extern int eat_food(int obj);
extern void attempt_pickup(void);
extern int po_is_stackable(int po);
extern void damage_obj(int obj);
extern int evasion_penalty(int obj);

/* XXX permons.c data */
#define NUM_OF_PERMONS 300
/* I don't intend to start with anything like this many monsters, but,
 * it gives me headroom. */
extern struct permon permons[NUM_OF_PERMONS];

/* XXX rng.c data and funcs */
#define RNG_MAX 0xFFFFFFFFu
extern unsigned int rng_state[5];
extern unsigned int saved_state[5];
extern unsigned int rng(void);
extern void rng_init(void);

/* XXX vector.c data and funcs */
extern void compute_directions(int y1, int x1, int y2, int x2, int *pdy, int *pdx, int *psy, int *psx, int *pmelee, int *pcardinal);

/* XXX u.c data and funcs */
extern void u_init(void);
extern void write_char_dump(void);
extern int do_death(enum death d, const char *what);
extern void heal_u(int amount, int boost, int loud);
extern int damage_u(int amount, enum death d, const char *what);
extern int gain_body(int amount, int loud);
extern int gain_agility(int amount, int loud);
extern int drain_body(int amount, const char *what, int permanent);
extern int drain_agility(int amount, const char *what, int permanent);
extern void gain_experience(int amount);
extern int lev_threshold(int level);
extern int move_player(int dy, int dx);
extern int reloc_player(int y, int x);
extern void recalc_defence(void);
extern int teleport_u(void);
extern void update_player(void);
extern int player_resists_dtype(enum damtyp dtype);

extern struct player u;

#define inyourroom(my, mx) ((roomnums[u.y][u.x] != -1) && (roomnums[u.y][u.x] == roomnums[(my)][(mx)]))
#endif

/* dunbash.h */
