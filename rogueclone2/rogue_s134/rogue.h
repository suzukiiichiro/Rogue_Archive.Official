/*
 * rogue.h
 *
 * This source herein may be modified and/or distributed by anybody who
 * so desires, with the following restrictions:
 *    1.)  This notice shall not be removed.
 *    2.)  Credit shall not be taken for the creation of this source.
 *    3.)  This code is not to be traded, sold, or used for personal
 *         gain or profit.
 *
 */

/* Windows */
#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include <direct.h>

#define CURSES
#define MSDOS
#define COLOR /* カラー表示 */
#define JAPAN /* 日本語版 */
#define TOPSCO
/*
#ifdef MSDOS
#undef	getchar
#undef	putchar
#define	fflush(p)
#endif

#if 0
#ifndef ORIGINAL
typedef unsigned char	uchar;
#define char		uchar
#endif
#endif
*/
#define boolean char

#define NOTHING		((unsigned short)     0)
#define OBJECT		((unsigned short)    01)
#define MONSTER		((unsigned short)    02)
#define STAIRS		((unsigned short)    04)
#define HORWALL		((unsigned short)   010)
#define VERTWALL	((unsigned short)   020)
#define DOOR		((unsigned short)   040)
#define FLOOR		((unsigned short)  0100)
#define TUNNEL		((unsigned short)  0200)
#define TRAP		((unsigned short)  0400)
#define HIDDEN		((unsigned short) 01000)

#define GOLD		((unsigned short)   01)
#define FOOD		((unsigned short)   02)
#define ARMOR		((unsigned short)   04)
#define WEAPON		((unsigned short)  010)
#define SCROL		((unsigned short)  020)
#define POTION		((unsigned short)  040)
#define WAND		((unsigned short) 0100)
#define RING		((unsigned short) 0200)
#define AMULET		((unsigned short) 0400)
#define ALL_OBJECTS	((unsigned short) 0777)

#define LEATHER 0
#define RINGMAIL 1
#define SCALE 2
#define CHAIN 3
#define BANDED 4
#define SPLINT 5
#define PLATE 6
#define ARMORS 7

#define BOW 0
#define DART 1
#define ARROW 2
#define DAGGER 3
#define SHURIKEN 4
#define MACE 5
#define LONG_SWORD 6
#define TWO_HANDED_SWORD 7
#define WEAPONS 8

#define MAX_PACK_COUNT 24

#define PROTECT_ARMOR 0
#define HOLD_MONSTER 1
#define ENCH_WEAPON 2
#define ENCH_ARMOR 3
#define IDENTIFY 4
#define TELEPORT 5
#define SLEEP 6
#define SCARE_MONSTER 7
#define REMOVE_CURSE 8
#define CREATE_MONSTER 9
#define AGGRAVATE_MONSTER 10
#define MAGIC_MAPPING 11
#define SCROLS 12

#define INCREASE_STRENGTH 0
#define RESTORE_STRENGTH 1
#define HEALING 2
#define EXTRA_HEALING 3
#define POISON 4
#define RAISE_LEVEL 5
#define BLINDNESS 6
#define HALLUCINATION 7
#define DETECT_MONSTER 8
#define DETECT_OBJECTS 9
#define CONFUSION 10
#define LEVITATION 11
#define HASTE_SELF 12
#define SEE_INVISIBLE 13
#define POTIONS 14

#define TELE_AWAY 0
#define SLOW_MONSTER 1
#define CONFUSE_MONSTER 2
#define INVISIBILITY 3
#define POLYMORPH 4
#define HASTE_MONSTER 5
#define PUT_TO_SLEEP 6
#define MAGIC_MISSILE 7
#define CANCELLATION 8
#define DO_NOTHING 9
#define WANDS 10

#define STEALTH 0
#define R_TELEPORT 1
#define REGENERATION 2
#define SLOW_DIGEST 3
#define ADD_STRENGTH 4
#define SUSTAIN_STRENGTH 5
#define DEXTERITY 6
#define ADORNMENT 7
#define R_SEE_INVISIBLE 8
#define MAINTAIN_ARMOR 9
#define SEARCHING 10
#define RINGS 11

#define RATION 0
#define FRUIT 1

#define NOT_USED		((unsigned short)   0)
#define BEING_WIELDED	((unsigned short)  01)
#define BEING_WORN		((unsigned short)  02)
#define ON_LEFT_HAND	((unsigned short)  04)
#define ON_RIGHT_HAND	((unsigned short) 010)
#define ON_EITHER_HAND	((unsigned short) 014)
#define BEING_USED		((unsigned short) 017)

#define NO_TRAP -1
#define TRAP_DOOR 0
#define BEAR_TRAP 1
#define TELE_TRAP 2
#define DART_TRAP 3
#define SLEEPING_GAS_TRAP 4
#define RUST_TRAP 5
#define TRAPS 6

#define STEALTH_FACTOR 3
#define R_TELE_PERCENT 8

#define UNIDENTIFIED ((unsigned short) 00)	/* MUST BE ZERO! */
#define IDENTIFIED ((unsigned short) 01)
#define CALLED ((unsigned short) 02)

#define DROWS 24
#define DCOLS 80
#define MAX_TITLE_LENGTH 30
#define MAXSYLLABLES 40
#define MAX_METAL 14
#define WAND_MATERIALS 30
#define GEMS 14

#define GOLD_PERCENT 46

struct id {
	short value;
	char *title;
	char *real;
	unsigned short id_status;
};

/* The following #defines provide more meaningful names for some of the
 * struct object fields that are used for monsters.  This, since each monster
 * and object (scrolls, potions, etc) are represented by a struct object.
 * Ideally, this should be handled by some kind of union structure.
 */

#define m_damage damage
#define hp_to_kill quantity
#define m_char ichar
#define first_level is_protected
#define last_level is_cursed
#define m_hit_chance class
#define stationary_damage identified
#define drop_percent which_kind
#define trail_char d_enchant
#define slowed_toggle quiver
#define moves_confused hit_enchant
#define nap_length picked_up
#define disguise what_is
#define next_monster next_object

struct obj {				/* comment is monster meaning */
	unsigned long m_flags;	/* monster flags */
	char *damage;			/* damage it does */
	short quantity;			/* hit points to kill */
	short ichar;			/* 'A' is for aquatar */
	short kill_exp;			/* exp for killing it */
	short is_protected;		/* level starts */
	short is_cursed;		/* level ends */
	short class;			/* chance of hitting you */
	short identified;		/* 'F' damage, 1,2,3... */
	unsigned short which_kind; /* item carry/drop % */
	short o_row, o_col, o;	/* o is how many times stuck at o_row, o_col */
	short row, col;			/* current row, col */
	short d_enchant;		/* room char when detect_monster */
	short quiver;			/* monster slowed toggle */
	short trow, tcol;		/* target row, col */
	short hit_enchant;		/* how many moves is confused */
	unsigned short what_is;	/* imitator's charactor (?!%: */
	short picked_up;		/* sleep from wand of sleep */
	unsigned short in_use_flags;
	struct obj *next_object;	/* next monster */
};

typedef struct obj object;

#define INIT_HP 12

struct fight {
	object *armor;
	object *weapon;
	object *left_ring, *right_ring;
	short hp_current;
	short hp_max;
	short str_current;
	short str_max;
	object pack;
	long gold;
	short exp;
	long exp_points;
	short row, col;
	short fchar;
	short moves_left;
};

typedef struct fight fighter;

struct dr {
	short oth_room;
	short oth_row,
	      oth_col;
	short door_row,
		  door_col;
};

typedef struct dr door;

struct rm {
	char bottom_row, right_col, left_col, top_row;
	door doors[4];
	unsigned short is_room;
};

typedef struct rm room;

#define MAXROOMS 9
#define BIG_ROOM 10

#define NO_ROOM -1

#define PASSAGE -3		/* cur_room value */

#define AMULET_LEVEL 26

#define R_NOTHING	((unsigned short) 01)
#define R_ROOM		((unsigned short) 02)
#define R_MAZE		((unsigned short) 04)
#define R_DEADEND	((unsigned short) 010)
#define R_CROSS		((unsigned short) 020)

#define MAX_EXP_LEVEL 21
#define MAX_EXP 10000000L
#define MAX_GOLD 900000
#define MAX_ARMOR 99
#define MAX_HP 800
#define MAX_STRENGTH 99
#define LAST_DUNGEON 99

#define STAT_LEVEL 01
#define STAT_GOLD 02
#define STAT_HP 04
#define STAT_STRENGTH 010
#define STAT_ARMOR 020
#define STAT_EXP 040
#define STAT_HUNGER 0100
#define STAT_LABEL 0200
#define STAT_ALL 0377

#define PARTY_TIME 10	/* one party somewhere in each 10 level span */

#define MAX_TRAPS 10	/* maximum traps per level */

#define HIDE_PERCENT 12

struct tr {
	short trap_type;
	short trap_row, trap_col;
};

typedef struct tr trap;

extern fighter rogue;
extern room rooms[];
extern trap traps[];
extern unsigned short dungeon[DROWS][DCOLS];
extern object level_objects;
extern char descs[DROWS][DCOLS];

extern struct id id_scrolls[];
extern struct id id_potions[];
extern struct id id_wands[];
extern struct id id_rings[];
extern struct id id_weapons[];
extern struct id id_armors[];

extern object mon_tab[];
extern object level_monsters;

extern char mesg[][80];

#define MONSTERS 26

#define HASTED					01L
#define SLOWED					02L
#define INVISIBLE				04L
#define ASLEEP				   010L
#define WAKENS				   020L
#define WANDERS				   040L
#define FLIES				  0100L
#define FLITS				  0200L
#define CAN_FLIT			  0400L		/* can, but usually doesn't, flit */
#define CONFUSED	 		 01000L
#define RUSTS				 02000L
#define HOLDS				 04000L
#define FREEZES				010000L
#define STEALS_GOLD			020000L
#define STEALS_ITEM			040000L
#define STINGS			   0100000L
#define DRAINS_LIFE		   0200000L
#define DROPS_LEVEL		   0400000L
#define SEEKS_GOLD		  01000000L
#define FREEZING_ROGUE	  02000000L
#define RUST_VANISHED	  04000000L
#define CONFUSES		 010000000L
#define IMITATES		 020000000L
#define FLAMES			 040000000L
#define STATIONARY		0100000000L		/* damage will be 1,2,3,... */
#define NAPPING			0200000000L		/* can't wake up for a while */
#define ALREADY_MOVED	0400000000L

#define SPECIAL_HIT		(RUSTS|HOLDS|FREEZES|STEALS_GOLD|STEALS_ITEM|STINGS|DRAINS_LIFE|DROPS_LEVEL)

#define WAKE_PERCENT 45
#define FLIT_PERCENT 33
#define PARTY_WAKE_PERCENT 75

#define HYPOTHERMIA 1
#define STARVATION 2
#define POISON_DART 3
#define QUIT 4
#define WIN 5

#define UPWARD 0
#define UPRIGHT 1
#define RIGHT 2
#define RIGHTDOWN 3
#define DOWN 4
#define DOWNLEFT 5
#define LEFT 6
#define LEFTUP 7
#define DIRS 8

#define ROW1 7
#define ROW2 15

#define COL1 26
#define COL2 52

#define MOVED 0
#define MOVE_FAILED -1
#define STOPPED_ON_SOMETHING -2
#define CANCEL '\033'
#define LIST '*'

#define HUNGRY 300
#define WEAK 150
#define FAINT 20
#define STARVE 0

#define MIN_ROW 1

/* external routine declarations.
char *strcpy();
char *strncpy();
char *strcat();

char *mon_name();
char *get_ench_color();
char *name_of();
char *md_gln();
char *md_getenv();
char *md_malloc();
#ifndef ORIGINAL
char *md_getcwd();
#endif
boolean is_direction();
boolean mon_sees();
boolean mask_pack();
boolean mask_room();
boolean is_digit();
boolean check_hunger();
boolean reg_move();
boolean md_df();
boolean has_been_touched();
object *add_to_pack();
object *alloc_object();
object *get_letter_object();
object *gr_monster();
object *get_thrown_at_monster();
object *get_zapped_monster();
object *check_duplicate();
object *gr_object();
object *object_at();
object *pick_up();
struct id *get_id_table();
unsigned short gr_what_is();
long rrandom();
long lget_number();
long xxx();
 */
/*int byebye(), error_save();*//*
#ifdef MSDOS
#if defined(__TURBOC__) && __TURBOC__ < 0x0200
int onintr(), ignintr();
#else
void onintr();
#endif
#else
int onintr();
#endif
*/
struct rogue_time {
	short year;		/* >= 1987 */
	short month;	/* 1 - 12 */
	short day;		/* 1 - 31 */
	short hour;		/* 0 - 23 */
	short minute;	/* 0 - 59 */
	short second;	/* 0 - 59 */
};

#ifndef ORIGINAL
typedef struct sopt {
	char	*name;	/* option name */
	boolean	*bp;	/* boolean ptr */
	char	**cp;	/* string ptr */
	boolean	ab;	/* add blank */
	boolean	nc;	/* no colon */
} opt;
extern opt envopt[];
extern char *optdesc[];
#endif

#ifdef CURSES
struct _win_st {
	short _cury, _curx;
	short _maxy, _maxx;
};

typedef struct _win_st WINDOW;

extern int LINES, COLS;
extern WINDOW *curscr;
#ifndef MSDOS
extern char *CL;
char *md_gdtcf();
#endif
#endif /*CURSES*/

#ifdef COLOR
#define WHITE	 0
#define RED	 1
#define GREEN	 2
#define YELLOW	 3
#define BLUE	 4
#define	MAGENTA	 5
#define CYAN	 6
#define RWHITE	 8
#define RRED	 9
#define RGREEN	 10
#define RYELLOW	 11
#define RBLUE	 12
#define RMAGENTA 13
#define RCYAN	 14
extern short c_attr[];
#else
/*#define colored(ch) ch*/
#define colored(ch) (unsigned char) ch
#endif

#ifndef ORIGINAL
extern char org_dir[], *game_dir;
#endif
#ifdef MSDOS
extern char cursor_on[], cursor_off[];
extern char *mac_type;
#endif

/*************************************************************************/

/* curses */
void initscr();
void endwin();
void repaint_screen();
void move(short row,short col);
void mvaddstr(short row,short col,char *str);
void addstr(register char *str);
void addch(register int ch);
void mvaddch(short row,short col,int ch);
void touch(register int row,short bcol,short ecol);
void refresh();
void refreshw();
void wrefresh(WINDOW *scr);
int mvinch(register short row,register short col);
void clear();
void clrtoeol();
void color(short col);
void standout();
void standend();
void clear_buffers();
void put_cursor(short row,short col);
void put_st_char(register int ch);

/* hit */
void mon_hit(register object *monster,char *other,boolean flame);
void rogue_hit(register object *monster,boolean force_hit);
void rogue_damage(short d,object *monster);
int get_damage(char *ds,boolean r);
int get_w_damage(object *obj);
int get_number(register char *s);
long lget_number(register char *s);
int to_hit(object *obj);
short damage_for_strength();
boolean mon_damage(object *monster,short damage);
void fight(boolean to_the_death);
void get_dir_rc(short dir,short *row,short *col,short allow_off_screen);
short get_hit_chance(object *weapon);
short get_weapon_damage(object *weapon);

/* init */
boolean init(int argc,char *argv[]);
void player_init();
void clean_up(char *estr);
void start_window();
void stop_window();
void onintr();
void error_save();
void do_args(int argc,char *argv[]);
void do_opts();
void set_opts(char *env);
void init_color();
int get_hex_num(register char *p,int n);
void env_get_value(char **s,char *e,boolean add_blank,boolean no_colon);

/* invent */
void inventory(object *pack,unsigned short mask);
void mix_colors();
void make_scroll_titles();
void get_desc(register object *obj,register char *desc,boolean capitalized);
void get_wand_and_ring_materials();
void single_inv(short ichar);
struct id *get_id_table(object *obj);
void inv_armor_weapon(boolean is_weapon);
void discovered();
void znum(char *buf,int n,int plus);
void lznum(char *buf,long n,int plus);

/* level */
void make_level();
void make_room(short rn,short r1,short r2,short r3);
boolean connect_rooms(register short room1,register short room2);
void clear_level();
void put_door(room *rm,short dir,register short *row,register short *col);
void draw_simple_passage(short row1,short col1,short row2,short col2,short dir);
boolean same_row(short room1,short room2);
boolean same_col(short room1,short room2);
void add_mazes();
void fill_out_level();
void fill_it(register int rn,boolean do_rec_de);
void recursive_deadend(short rn,short *offsets,short srow,short scol);
boolean mask_room(short rn,short *row,short *col,unsigned short mask);
void make_maze(register short r,register short c,register short tr,register short br,register short lc,register short rc);
void hide_boxed_passage(short row1,short col1,short row2,short col2,short n);
void put_player(short nr);
boolean drop_check();
boolean check_up();
void add_exp(int e,boolean promotion);
short get_exp_level(long e);
int hp_raise();
void show_average_hp();
void mix_random_rooms();

/* machdep */
void putstr(register char *s);
char *md_getcwd(char *dir,int len);
int md_chdir(char *dir);
void md_slurp();
void md_control_keyboard(boolean mode);
void md_heed_signals();
void md_ignore_signals();
int md_get_file_id(char *fname);
void md_gct(struct rogue_time *rt_buf);
void md_gfmt(char *fname,struct rogue_time *rt_buf);
boolean md_df(char *fname);
char *md_gln();
void md_sleep(int nsecs);
void sleep(int nsecs);
char *md_getenv(char *name);
char *md_malloc(int n);
int md_gseed();
void md_exit(int status);
void md_cbreak_no_echo_nonl(boolean on);

/* message */
void message(char *msg,boolean intrpt);
void remessage();
void check_message();
int get_direction();
int get_input_line(char *prompt,char *insert,char *buf,char *if_cancelled,boolean add_blank,boolean do_echo);
short input_line(int row,int col,char *insert,char *buf,int ch);
short do_input_line(boolean is_msg,int row,int col,char *prompt,char *insert,char *buf,char *if_cancelled,boolean add_blank,boolean do_echo,int first_ch);
short rgetchar();
void print_stats(register char stat_mask);
void pad(char *s,short n);
void save_screen();
void sound_bell();
boolean is_digit(short ch);
int r_index(char *str,int ch,boolean last);

/* monster */
void put_mons();
object *gr_monster(register object *monster,register mn);
void mv_mons();
void party_monsters(int rn,int n);
short gmc_row_col(register short row,register short col);
short gmc(object *monster);
void mv_monster(register object *monster,short row,short col);
boolean mtry(register object *monster,register short row,register short col);
void move_mon_to(register object *monster,register short row,register short col);
boolean mon_can_go(register object *monster,register short row,register short col);
void wake_up(object *monster);
void wake_room(short rn,boolean entering,short row,short col);
char *mon_name(object *monster);
boolean rogue_is_around(short row,short col);
void wanderer();
void show_monsters();
void create_monster();
void put_m_at(short row,short col,object *monster);
void aim_monster(object *monster);
boolean rogue_can_see(register short row,register short col);
boolean move_confused(object *monster);
boolean flit(object *monster);
char gr_obj_char();
boolean no_room_for_monster(int rn);
void aggravate();
boolean mon_sees(object *monster,short row,short col);
void mv_aquatars();

/* move */
short one_move_rogue(short dirch,short pickup);
void multiple_move_rogue(short dirch);
boolean is_passable(register short row,register short col);
boolean next_to_something(register short drow,register short dcol);
boolean can_move(short row1,short col1,short row2,short col2);
void move_onto();
boolean is_direction(c);
boolean check_hunger(boolean messages_only);
boolean reg_move();
void rest(int count);
short gr_dir();
void heal();

/* object */
short colored(register short c);
void put_objects();
void put_gold();
void plant_gold(short row,short col,boolean is_maze);
void place_at(object *obj,short row,short col);
object *object_at(register object *pack,short row,short col);
object *get_letter_object(short ch);
void free_stuff(object *objlist);
char *name_of(object *obj);
object *gr_object();
unsigned short gr_what_is();
void gr_scroll(object *obj);
void gr_potion(object *obj);
void gr_weapon(object *obj,int assign_wk);
void gr_armor(object *obj,int assign_wk);
void gr_wand(object *obj);
void get_food(object *obj,boolean force_ration);
void put_stairs();
short get_armor_class(object *obj);
object *alloc_object();
void free_object(object *obj);
void make_party();
void show_objects();
void put_amulet();
void rand_place(object *obj);
void new_object_for_wizard();
void list_object(object *obj,short max);
int next_party();

/* pack */
object *add_to_pack(object *obj,object *pack,boolean condense);
void take_from_pack(object *obj,object *pack);
object *pick_up(short row,short col,short *status);
void drop();
object *check_duplicate(object *obj,object *pack);
char next_avail_ichar();
void wait_for_ack();
short pack_letter(char *prompt,unsigned short mask);
void take_off();
void wear();
void unwear(object *obj);
void do_wear(object *obj);
void wield();
void do_wield(object *obj);
void unwield(object *obj);
void call_it();
short pack_count(object *new_obj);
boolean mask_pack(object *pack,unsigned short mask);
boolean is_pack_letter(short *c,unsigned short *mask);
boolean has_amulet();
void kick_into_pack();

/* play */
void play_level();
void help();
void identify();
void options();
void doshell();

/* random */
void srrandom(int x);
long rrandom();
int get_rand(register int x,register int y);
boolean rand_percent(register int percentage);
boolean coin_toss();

/* ring */
void put_on_ring();
void do_put_on(object *ring,boolean on_left);
void remove_ring();
void un_put_on(object *ring);
void gr_ring(object *ring,boolean assign_wk);
void inv_rings();
void ring_stats(boolean pr);

/* room */
void light_up_room(int rn);
void light_passage(short row,short col);
void darken_room(short rn);
short get_dungeon_char(register short row,register short col);
char get_mask_char(register unsigned short mask);
void gr_row_col(short *row,short *col,unsigned short mask);
short gr_room();
short party_objects(int rn);
short get_room_number(register short row,register short col);
boolean is_all_connected();
void visit_rooms(int rn);
void draw_magic_map();
void dr_course(object *monster,boolean entering,short row,short col);
boolean get_oth_room(short rn,short *row,short *col);

/* save */
void save_game();
void save_into_file(char *sfile);
void restore(char *fname);
void write_pack(object *pack,FILE *fp);
void read_pack(object *pack,FILE *fp,boolean is_rogue);
void rw_dungeon(FILE *fp,boolean rw);
void rw_id(struct id id_table[],FILE *fp,int n,boolean wr);
void write_string(char *s,FILE *fp);
void read_string(char *s,FILE *fp);
void rw_rooms(FILE *fp,boolean rw);
void r_read(FILE *fp,char *buf,int n);
void r_write(FILE *fp,char *buf,int n);
boolean has_been_touched(struct rogue_time *saved_time,struct rogue_time *mod_time);

/* score */
void killed_by(object *monster,short other);
void win();
void mvaddbanner(short row,short col,register char *ban);
void quit(boolean from_intrpt);
void put_scores(object *monster,short other);
void insert_score(char scores[][82],char n_names[][30],char *n_name,short rank,short n,object* monster,short other);
boolean is_vowel(short ch);
void sell_pack();
int get_value(object *obj);
void id_all();
void xxxx(char *buf,short n);
long xxx(boolean st);
void nickize(char *buf,char *score,char *n_name);
void center(short row,char *buf);
void sf_error();

/* spechit */
void special_hit(object *monster);
void rust(object *monster);
void freeze(object *monster);
void steal_gold(object *monster);
void steal_item(object *monster);
void disappear(object *monster);
void cough_up(object *monster);
boolean try_to_cough(short row,short col,object *obj);
boolean seek_gold(object *monster);
boolean gold_at(short row,short col);
void check_gold_seeker(object *monster);
boolean check_imitator(object *monster);
boolean imitating(register short row,register short col);
void sting(object *monster);
void drop_level();
void drain_life();
boolean m_confuse(object *monster);
boolean flame_broil(object *monster);
void get_closer(short *row,short *col,short trow,short tcol);

/* throw */
void throw();
boolean throw_at_monster(object *monster,object *weapon);
object *get_thrown_at_monster(object *obj,short dir,short *row,short *col);
void flop_weapon(object* weapon,short row,short col);
void rand_around(short i,short *r,short *c);
void potion_monster(object *monster,unsigned short kind);

/* trap */
short trap_at(register short row,register short col);
void trap_player(short row,short col);
void add_traps();
void id_trap();
void show_traps();
void search(short n,boolean is_auto);

/* use */
void quaff();
void read_scroll();
void vanish(object *obj,short rm,object *pack);
void potion_heal(boolean extra);
void idntfy();
void eat();
void hold_monster();
void tele();
void hallucinate();
void unhallucinate();
void unblind();
void relight();
void take_a_nap();
void go_blind();
char *get_ench_color();
void confuse();
void unconfuse();
void uncurse_all();

/* zap */
void zapp();
object *get_zapped_monster(short dir,short *row,short *col);
object *get_missiled_monster(short dir,short *row,short *col);
void zap_monster(object *monster,unsigned short kind);
void tele_away(object *monster);
void wizardize();

/* appmain : windows */ 
int read_mesg(char *strmesg);

int loadINI();
int saveINI();

WPARAM MessageLoop();
short getcharWIN();
void  charreset ();

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow);
LRESULT	CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
BOOL	CALLBACK DlgProc_Version(HWND hDlg,UINT msg,WPARAM wp,LPARAM lp);
BOOL	CALLBACK DlgProc_Name(HWND hDlg,UINT msg,WPARAM wp,LPARAM lp);
BOOL	CALLBACK DlgProc_Config(HWND hDlg,UINT msg,WPARAM wp,LPARAM lp);

void SetClientWindow(HWND hw,DWORD s,int w,int h);
void SetCenterWindow(HWND hw,HWND hwD);
int OpenFileDialog(HWND hWnd,char *szFile,char *szDir,char *szTitle,char *Filter);
int SaveFileDialog(HWND hWnd,char *strFile,char *strDir,char *strTitle,char *Filter,char *strExt);

/*************************************************************************/

