/* init.c */

#ifndef __CYGWIN__
#include <ncurses.h>
#else
#include <ncurses/ncurses.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rogue.h"
#include "init.h"
#include "inventory.h"
#include "machdep.h"
#include "message.h"
#include "object.h"
#include "pack.h"
#include "random.h"
#include "ring.h"
#include "save.h"
#include "score.h"

#ifdef ONE_SAVE_PER_PLAYER
	#include <unistd.h>
#endif

char login_name[30];
/* char nick_name[30] = {'\0'}; */
/*                "1 23456789012345678901234567890" */
char *nick_name = "\0                              ";
static char *rest_file = 0;
boolean cant_int = 0;
boolean did_int = 0;
boolean score_only;
boolean init_curses = 0;
boolean save_is_interactive = 1;
boolean ask_quit = 1, show_skull = 1;
char *error_file = "rogue.esave";
char *byebye_string = "Okay, bye bye!";

extern char *fruit;
extern char *save_file;
extern int party_counter;
extern boolean jump;
extern object level_objects;
extern object level_monsters;
extern fighter rogue;

void do_args(int, char *[]);
void do_opts(void);
void start_window(void);
void player_init(void);
void stop_window(void);
void env_get_value(char **, char *, boolean);

int init(int argc, char *argv[])
{
	#ifdef ONE_SAVE_PER_PLAYER
		char testfile[128];
		int exist;
	#endif
	char *pn;

	do_args(argc, argv);
	do_opts();

	pn = md_gln();

	if ((!pn) || (strlen(pn) >= 30))
	{
		clean_up("Hey! Who are you?");
	}
	(void) strcpy(login_name, pn);

        #ifdef ASKNAME
	if ((argc > 1) && (argv[1][0] == '-') && (argv[1][1] == 'n' || argv[1][1] == 'u'))
	{
	        printf("Welcome, adventurer!  What is your name? ");
		scanf("%29s",login_name);
	}
        #endif

	#ifdef ONE_SAVE_PER_PLAYER
		sprintf(testfile,"%s%s.sav",ROGUE_SAVEPATH,login_name);
		exist=access(testfile,F_OK);
		if (exist==0) {
			printf("Savefile found, restoring...\n");
			rest_file=testfile;
		}
	#endif

	if (!score_only && !rest_file)
	{
		printf("Hello %s, just a moment while I dig the dungeon...",
		((nick_name[0]) ? nick_name : login_name));
		fflush(stdout);
		sleep(3);
	}

	initscr();
	if ((LINES < DROWS) || (COLS < DCOLS))
	{
		clean_up("Must be played on a 24 x 80 or better screen");
	}
	start_window();
	init_curses = 1;

	md_heed_signals();

	if (score_only)
	{
		put_scores((object *) 0, 0);
	}
	srand(md_gseed());
	if (rest_file)
	{
		restore(rest_file);
		return(1);
	}
	mix_colors();
	get_wand_and_ring_materials();
	make_scroll_titles();

	level_objects.next_object = 0;
	level_monsters.next_monster = 0;
	player_init();
	party_counter = get_rand(1, PARTY_TIME);
	ring_stats(0);
	return(0);
}

void player_init(void)
{
	object *obj;

	rogue.pack.next_object = 0;

	obj = alloc_object();
	get_food(obj, 1);
	(void) add_to_pack(obj, &rogue.pack, 1);

	obj = alloc_object();		/* initial armor */
	obj->what_is = ARMOR;
	obj->which_kind = RINGMAIL;
	obj->oclass = RINGMAIL+2;
	obj->is_protected = 0;
	obj->d_enchant = 1;
	(void) add_to_pack(obj, &rogue.pack, 1);
	do_wear(obj);

	obj = alloc_object();		/* initial weapons */
	obj->what_is = WEAPON;
	obj->which_kind = MACE;
	obj->damage = "2d3";
	obj->hit_enchant = obj->d_enchant = 1;
	obj->identified = 1;
	(void) add_to_pack(obj, &rogue.pack, 1);
	do_wield(obj);

	obj = alloc_object();
	obj->what_is = WEAPON;
	obj->which_kind = BOW;
	obj->damage = "1d2";
	obj->hit_enchant = 1;
	obj->d_enchant = 0;
	obj->identified = 1;
	(void) add_to_pack(obj, &rogue.pack, 1);

	obj = alloc_object();
	obj->what_is = WEAPON;
	obj->which_kind = ARROW;
	obj->quantity = get_rand(25, 35);
	obj->damage = "1d2";
	obj->hit_enchant = 0;
	obj->d_enchant = 0;
	obj->identified = 1;
	(void) add_to_pack(obj, &rogue.pack, 1);
}

void clean_up(char *estr)
{
	if (save_is_interactive)
	{
		if (init_curses)
		{
			move(DROWS - 1, 0);
			refresh();
			stop_window();
		}
		printf("\n%s\n", estr);
	}
	endwin();
	md_exit(0);
}

void start_window(void)
{
	cbreak();
//	raw();
	keypad(stdscr, TRUE);
	noecho();
	nonl();
	md_control_keyboard(0);
}

void stop_window(void)
{
	endwin();
	md_control_keyboard(1);
}

void byebye(int sig)
{
	md_ignore_signals();
	if (ask_quit)
	{
		quit(1);
	}
   	else
   	{
		clean_up(byebye_string);
	}
	md_heed_signals();
}

void onintr(int sig)
{
	md_ignore_signals();
	if (cant_int)
	{
		did_int = 1;
	}
	else
	{
		check_message();
		message("interrupt", 1);
	}
	md_heed_signals();
}

void error_save(int sig)
{
	save_is_interactive = 0;
	save_into_file(error_file);
	clean_up("");
}

void do_args(int argc, char *argv[])
{
	int i, j;
	static char buf[128];

	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			for (j = 1; argv[i][j]; j++)
			{
				switch(argv[i][j])
				{
				case 's':
					score_only = 1;
					break;
				}
			}
		}
		else
		{
			rest_file = argv[i];
			if (rest_file[0] != '.' && rest_file[0] != '/')
			{
				sprintf(buf, "%s%s", ROGUE_SAVEPATH, argv[i]);
				rest_file = buf;
			}
		}
	}
}

void do_opts(void)
{
	char *eptr;

	if ((eptr = getenv("ROGUEOPTS")))
	{
		for (;;)
		{
			while ((*eptr) == ' ')
			{
				eptr++;
			}
			if (!(*eptr))
			{
				break;
			}
			if (!strncmp(eptr, "fruit=", 6))
			{
				eptr += 6;
				env_get_value(&fruit, eptr, 1);
			}
			else
			{
				if (!strncmp(eptr, "file=", 5))
				{
					eptr += 5;
					env_get_value(&save_file, eptr, 0);
				}
				else
				{
					if (!strncmp(eptr, "nojump", 6))
					{
						jump = 0;
					}
					else
					{
						if (!strncmp(eptr, "name=", 5))
						{
							eptr += 5;
							env_get_value((void *)(&nick_name), eptr, 0);
						}
						else
						{
							if (!strncmp(eptr, "noaskquit", 9))
							{
								ask_quit = 0;
							}
							else
							{
								if (!strncmp(eptr, "noskull", 5) ||
								    !strncmp(eptr,"notomb", 6))
								{
									show_skull = 0;
								}
							}
						}
					}
				}
			}
			while ((*eptr) && (*eptr != ','))
			{
				eptr++;
			}
			if (!(*(eptr++)))
			{
				break;
			}
		}
	}
}

void env_get_value(char **s, char *e, boolean add_blank)
{
	short i = 0;
	char *t;

	t = e;

	while ((*e) && (*e != ','))
	{
		if (*e == ':')
		{
			*e = ';';		/* ':' reserved for score file purposes */
		}
		e++;
		if (++i >= 30)
		{
			break;
		}
	}
	if (!(*s = md_malloc(i + (add_blank ? 2 : 1))))
	{
		clean_up("cannot alloc() memory");
	}
	(void) strncpy(*s, t, i);
	if (add_blank)
	{
		(*s)[i++] = ' ';
	}
	(*s)[i] = '\0';
}
