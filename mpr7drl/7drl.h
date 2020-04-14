/* 7drl.h */

#ifndef _7DRL_H
#define _7DRL_H

/* XXX STRUCTURES XXX */

/* XXX struct player */
struct player {
	char name[17];	/* including '\0' the fencepost. */
	int y;		/* y-coord */
	int x;		/* x-coord */
	int body;	/* determines mace to-hit, melee damage, max 99 */
	int agility;	/* determines sword, dagger, missile to-hit, max 99 */
	int hpmax;	/* Max hit points; max of 999. */
	int hpcur;	/* Current hit points; <= 0 is dead. */
	int experience;	/* Experience points earned. */
	int defence;	/* To-hit target number for monsters */
	int level;	/* Each level gets you +1 body, +1 agility, +1 random
			   point, and +(10+body/10) hit points */
	int gold;
	int inventory[19];	/* 19 inventory slots, leaving room for a prompt */
	int weapon;	/* For now, you can only wield one weapon. */
	int armour;	/* For now, you can only wear one item of armour. */
	int ring;	/* For now, you can only wear one magic ring. */
};

/* XXX enum terrain_num */
/* To start with, only four types of terrain: Wall, Floor, Door, Stairs. */
enum terrain_num {
	WALL = 0, FLOOR = 1, DOOR = 2, STAIRS = 3
};

/* XXX struct permon */
#define PM_NEWT 0
#define PM_GOBLIN 1
#define PM_RAT 2
#define PM_THUG 3
#define PM_BAD_ELF 4
#define PM_GOON 5
#define PM_ZOMBIE 6
#define PM_WOLF 7
#define PM_ICE_MONSTER 8
#define PM_SNAKE 9
#define PM_HUNTER 10
#define PM_CENTAUR 11
#define PM_TROLL 12
#define PM_DUELLIST 13
#define PM_WIZARD 14
#define PM_DRAGON 15
#define PM_LICH 16
#define PM_WARLORD 17
#define PM_DEMON 18
#define PM_REAL_COUNT ((PM_DEMON) + 1)

struct permon {
	const char name[32];
	const char plural[32];
	char sym;
	int rarity;	/* Chance in 100 of being thrown back and regen'd. */
	int power;	/* Used to determine OOD rating. */
	/* All OOD-improved stats cap out at base + (power * base) */
	int hp;		/* Improved by OOD rating at 1:1. */
	int tohit;	/* Improved by OOD rating at 1:3. */
	int dam;	/* Improved by OOD rating at 1:5. */
	int defence;	/* Improved by OOD rating at 1:3. */
	int exp;	/* Unaffected by OOD rating. */
	int speed;	/* 0 = slow; 1 = normal; 2 = quick */
};

enum death {
	DEATH_KILLED, DEATH_KILLED_MON, DEATH_BODY, DEATH_AGILITY
};

/* XXX struct permobj */
#define PO_DAGGER 0
#define PO_LONG_SWORD 1
#define PO_MACE 2
#define PO_RUNESWORD 3
#define PO_BOW 4
#define PO_CROSSBOW 5
#define PO_POT_HEAL 6
#define PO_POT_POISON 7
#define PO_POT_BODY 8
#define PO_POT_AGILITY 9
#define PO_POT_WEAKNESS 10
#define PO_SCR_TELEPORT 11
#define PO_SCR_FIRE 12
#define PO_SCR_MONSTERS 13
#define PO_LEATHER_ARMOUR 14
#define PO_CHAINMAIL 15
#define PO_PLATE_ARMOUR 16
#define PO_MAGE_ARMOUR 17
#define PO_RING_REGEN 18
#define PO_RING_FIRE 19
#define PO_RING_WEDDING 20
#define PO_RING_VAMPIRE 21
#define PO_RING_FROST 22
#define PO_RING_DOOM 23
#define PO_GOLD 24
#define PO_REAL_COUNT ((PO_GOLD) + 1)

enum poclass_num {
	POCLASS_NONE = 0, POCLASS_WEAPON = 1, POCLASS_POTION = 2,
	POCLASS_SCROLL = 3, POCLASS_ARMOUR = 4, POCLASS_RING = 5
};

struct permobj {
	const char name[32];
	const char plural[32];
	enum poclass_num poclass;
	int rarity;	/* Chance in 100 of being thrown away and regen'd. */
	int sym;
	int power;	/* AC for armour; damage for weapons. */
	int used;	/* Set to 1 for valid entries. */
	int known;	/* Set to 1 for items recognised at startup */
};

/* XXX struct mon */
struct mon {
	int mon_id;
	int y;
	int x;
	int used;
	int hpmax;	/* Improved by OOD rating at 1:1. */
	int hpcur;	/* <= 0 is dead. */
	int tohit;	/* Improved by OOD rating at 1:3. */
	int defence;	/* Improved by OOD rating at 1:3. */
	int dam;	/* Improved by OOD rating at 1:5. */
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
};

/* XXX Object/monster arrays */
extern struct permobj permobjs[100];

/* Treasure generation limit plus inventory limit means this should be
 * enough.  When I evolve to the "fully-fledged" version, I'll use a Tatham
 * tree instead. */
extern struct mon monsters[100];

/* XXX combat.c data and funcs */
enum damtyp {
	DT_PHYS = 0, DT_COLD, DT_FIRE, DT_NECRO, DT_ELEC
};
extern int player_attack(int dy, int dx);
extern int mhitu(int mon);
extern int uhitm(int mon);
extern int mshootu(int mon, enum damtyp dtype);
extern int ushootm(int sy, int sx);
/* XXX display.c data and funcs */
extern int read_input(char *buffer, int length);
extern void print_msg(const char *fmt, ...);
extern void print_help(void);
extern int display_init(void);
extern void display_update(void);
extern int display_shutdown(void);
extern void print_inv(enum poclass_num filter);
extern int inv_select(enum poclass_num filter, const char *action);
extern enum game_cmd get_command(void);
extern int select_dir(int *psy, int *psx);
extern int getyn(const char *msg);

/* "I've changed things that need to be redisplayed" flags. */
extern int status_updated;
extern int map_updated;

enum game_cmd {
	DROP_ITEM, SAVE_GAME, SHOW_INVENTORY, MOVE_WEST, MOVE_SOUTH,
	MOVE_NORTH, MOVE_EAST, MOVE_NW, MOVE_NE, MOVE_SW, MOVE_SE,
	QUAFF_POTION, READ_SCROLL, WIELD_WEAPON, WEAR_ARMOUR,
	TAKE_OFF_ARMOUR, PUT_ON_RING, REMOVE_RING, GIVE_HELP,
	ATTACK, GET_ITEM, QUIT, GO_DOWN_STAIRS, STAND_STILL
};

/* XXX main.c data and funcs */
extern int exclusive_flat(int lower, int upper); /* l+1 ... u-1 */
extern int inclusive_flat(int lower, int upper); /* l ... u */
extern int one_die(int sides);	/* 1..n */
extern int dice(int count, int sides);
extern int zero_die(int sides);	/* 0..n-1 */
extern int do_command(enum game_cmd command);
extern char running_state[256];
extern char levgen_saved_state[256];
extern int game_finished;
extern int game_tick;

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
#define DUN_SIZE 42
#define MAX_ROOMS 9

extern int mapobject[DUN_SIZE][DUN_SIZE];
extern int mapmonster[DUN_SIZE][DUN_SIZE];
extern enum terrain_num terrain[DUN_SIZE][DUN_SIZE];
#define MAPFLAG_EXPLORED 0x00000001
extern int mapflags[DUN_SIZE][DUN_SIZE];
extern int roomnums[DUN_SIZE][DUN_SIZE];
extern int roombounds[MAX_ROOMS][4];
extern int depth;

/* XXX monsters.c data and funcs */
extern void mon_acts(int mon);
extern void death_drop(int mon);
extern void print_mon_name(int mon, int article);
extern void summon_demon_near(int y, int x);
extern int create_mon(int pm_idx, int y, int x);
extern void summoning(int y, int x, int how_many);
extern int ood(int power, int ratio);
extern int get_random_pmon(void);
extern void damage_mon(int mon, int amount, int by_you);
extern int pmon_is_undead(int pm);
extern int pmon_resists_cold(int pm);
extern int pmon_resists_fire(int pm);
extern int mon_can_pass(int mon, int y, int x);
extern void move_mon(int mon, int y, int x);


/* XXX objects.c data and funcs */
extern void flavours_init(void);
extern void print_obj_name(int obj);
extern int create_obj(int po_idx, int quantity, int with_you, int y, int x);
extern int drop_obj(int inv_idx);
extern int create_obj_class(enum poclass_num pocl, int quantity, int with_you, int y, int x);
extern int create_obj_random(int y, int x);
extern struct obj objects[100];	/* SHould be enough. */
extern int read_scroll(int obj);
extern int quaff_potion(int obj);
extern void attempt_pickup(void);
extern int po_is_stackable(int po);

/* XXX permobjs.c data and funcs */
extern int pmon_resists_fire(int pm);
/* I don't intend to start with anything like this many monsters, but,
 * it gives me headroom. */
extern struct permon permons[100];

/* XXX u.c data and funcs */
extern void u_init(void);
extern void do_death(enum death d, const char *what);
extern void heal_u(int amount, int boost);
extern int damage_u(int amount, enum death d, const char *what);
extern int drain_body(int amount, const char *what);
extern int drain_agility(int amount, const char *what);
extern void gain_experience(int amount);
extern int lev_threshold(int level);
extern int move_player(int dy, int dx);
extern int reloc_player(int y, int x);
extern void recalc_defence(void);
extern void teleport_u(void);

extern struct player u;
#endif
