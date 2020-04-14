/*
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 *
 * @(#)main.c	4.22 (Berkeley) 02/05/99
 */

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <curses.h>
#include <ctype.h>
#include "rogue.h"
#include <unistd.h>
#include <locale.h>

/*
 * main:
 *	The main program, of course
 */
int
main(int argc, char **argv)
{
    void set_scroll_2(void);

    char *env;
    time_t lowtime;

#ifdef __CYGWIN__
    setenv("TERMINFO", "terminfo", 1);
#endif
    setlocale(LC_ALL,"");

    md_init();

#ifdef MASTER
    /*
     * Check to see if he is a wizard
     */
    if (argc >= 2 && argv[1][0] == '\0')
	if (strcmp(PASSWD, md_crypt(md_getpass("wizard's password: "), "mT")) == 0)
	{
	    wizard = TRUE;
	    player.t_flags |= SEEMONST;
	    argv++;
	    argc--;
	}

#endif

    /* Check to see if savefiles should be stored in the system location */
#ifdef SAVEDIR
    if (argc >= 3 && !strcmp(argv[1], "-n"))
    {
	use_savedir = TRUE;
	strncpy(whoami, argv[2], MAXSTR);
	whoami[MAXSTR-1] = '\0';
	snprintf(file_name, MAXSTR, "%s/%d-%.10s.r5sav", SAVEDIR,
		 md_getuid(), whoami);
    }
#endif

    open_score();
    open_log();

    /*
     * get home and options from environment
     */

    int i;
/*  strcpy(home, md_gethomedir()); */
#ifdef __CYGWIN__
    strcpy(home, "");
#else
    strcpy(home, "./");
#endif

	if (strlen(home) > MAXSTR - strlen("rogue.save") - 1)
		*home = 0;

    if (!use_savedir)
    {
	md_normaluser();
	strcpy(file_name, home);
	strcat(file_name, "rogue.save");
    }

    if ((env = getenv("ROGUEOPTS")) != NULL)
	parse_opts(env);
    if (!use_savedir && (env == NULL || whoami[0] == '\0'))
	strucpy(whoami, md_getusername(), strlen(md_getusername()));
    lowtime = time(NULL);
    if (getenv("SEED") != NULL)
    {
	dnum = atoi(getenv("SEED"));
	noscore = 1;
    }
    else
	dnum = (unsigned int) lowtime + md_getpid();
    seed = dnum;

    /*
     * check for print-score option
     */

    if (argc == 2)
    {
	if (strcmp(argv[1], "-s") == 0)
	{
	    noscore = TRUE;
	    score(0, -1, 0);
	    exit(0);
	}
	else if (strcmp(argv[1], "-d") == 0)
	{
	    dnum = rnd(100);	/* throw away some rnd()s to break patterns */
	    while (--dnum)
		rnd(100);
	    purse = rnd(100) + 1;
	    level = rnd(100) + 1;
	    initscr();
	    start_custom_color();
	    getltchars();
	    death(death_monst());
	    exit(0);
	}
    }
    if (argc >= 2) {
	if (argv[1][0] == '-'
	&&  isdigit(argv[1][1]) && isdigit(argv[1][2])) {
	    i = (argv[1][1] - '0') * 16 + argv[1][2] - '0';

	    color_mode = TRUE;
	    idscr_md = TRUE;
/*	    pc_md = TRUE; */

	    if ((i & 0x02) != 0)
		bright_mode = TRUE;
	    if ((i & 0x01) != 0) {
		color_mode = FALSE;
		bright_mode = FALSE;
	    }
/*	    if ((i & 0x04) != 0) {
		rname = TRUE;
		slime_md = TRUE;
	    } */

	    if ((i & 0x10) != 0)
		passgo = TRUE;
/*	    if ((i & 0x60) != 0) (* 0x20 | 0x40 *)
		pc_md = FALSE; */
	    if ((i & 0x40) != 0)
		idscr_md = FALSE;

	    if (argc >= 3)
		strucpy(whoami, argv[2], strlen(argv[2]));
	    else {
		whoami[0] = '?';
		argc = 3;
	    }
	}
    }
    if (argc == 3) {
	if (strcmp(argv[1], "-u") == 0
	||  strcmp(argv[1], "-n") == 0) {
	    strucpy(whoami, argv[2], strlen(argv[2])+1);
	}
    }
    if (whoami[0] == '?') {
	printf("Who are you? ");
	fflush(stdout);
	(void) fgets(whoami, MAXSTR, stdin);
	whoami[strlen(whoami) - 1] = '\0';
	if(!whoami[0])
	    strcpy(whoami, "Rodney");
    }

    init_check();			/* check for legal startup */
    if (use_savedir)
    {
	/* If there is a saved game, restore() will not return.  If it
	 * returns 1, there isn't a game, so start one.  If 0, there was
	 * an error. */
	if (!restore(file_name))
	    my_exit(1);
    }
    else if (argc == 2)
	if (!restore(argv[1]))	/* Note: restore will never return */
	    my_exit(1);
#ifdef MASTER
    if (wizard)
	printf("Hello %s, welcome to dungeon #%d\n", whoami, dnum);
    else
#endif
	printf("Hello %s, just a moment while I dig the dungeon...\n", whoami);
    if (idscr_md == TRUE)
	set_scroll_2();
    fflush(stdout);
    sleep(3);

    initscr();				/* Start up cursor package */
    start_custom_color();
    init_probs();			/* Set up prob tables for objects */
    init_player();			/* Set up initial player stats */
    init_names();			/* Set up names of scrolls */
    init_colors();			/* Set up colors of potions */
    init_stones();			/* Set up stone settings of rings */
    init_materials();			/* Set up materials of wands */
    setup();

    /*
     * The screen must be at least NUMLINES x NUMCOLS
     */
    if (LINES < NUMLINES || COLS < NUMCOLS)
    {
	endwin();
	printf("\nSorry, the screen must be at least %dx%d\n", NUMLINES, NUMCOLS);
/*	endwin(); */
	my_exit(1);
    }

    /*
     * Set up windows
     */
    hw = newwin(LINES, COLS, 0, 0);
    idlok(stdscr, TRUE);
    idlok(hw, TRUE);
#ifdef MASTER
    noscore = wizard;
#endif
    new_level();			/* Draw current level */
    /*
     * Start up daemons and fuses
     */
    start_daemon(runners, 0, AFTER);
    start_daemon(doctor, 0, AFTER);
    fuse(swander, 0, WANDERTIME, AFTER);
    start_daemon(stomach, 0, AFTER);
    playit();
    return(0);
}

/*
 * endit:
 *	Exit the program abnormally.
 */

void
endit(int sig)
{
    NOOP(sig);
    fatal("Okay, bye bye!\n");
}

/*
 * fatal:
 *	Exit the program, printing a message.
 */

void
fatal(const char *s)
{
    mvaddstr(LINES - 2, 0, s);
    refresh();
    endwin();
    my_exit(0);
}

/*
 * rnd:
 *	Pick a very random number.
 */
int
rnd(int range)
{
    return range == 0 ? 0 : abs((int) RN) % range;
}

/*
 * roll:
 *	Roll a number of dice
 */
int
roll(int number, int sides)
{
    int dtotal = 0;

    while (number--)
	dtotal += rnd(sides)+1;
    return dtotal;
}

/*
 * tstp:
 *	Handle stop and start signals
 */

void
tstp(int ignored)
{
    int y, x;
    int oy, ox;

	NOOP(ignored);

    /*
     * leave nicely
     */
    getyx(curscr, oy, ox);
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
    resetltchars();
    fflush(stdout);
	md_tstpsignal();

    /*
     * start back up again
     */
	md_tstpresume();
    raw();
    noecho();
    nonl();
    keypad(stdscr,1);
    playltchars();
    clearok(curscr, TRUE);
    wrefresh(curscr);
    getyx(curscr, y, x);
    mvcur(y, x, oy, ox);
    fflush(stdout);
#ifdef __CYGWIN__
    wmove(curscr, oy, ox);
#else
    curscr->_cury = oy;
    curscr->_curx = ox;
#endif
}

/*
 * playit:
 *	The main loop of the program.  Loop until the game is over,
 *	refreshing things and looking at the proper times.
 */

void
playit(void)
{
    char *opts;
    char whoami2[MAXSTR];

    /*
     * set up defaults for slow terminals
     */

    if (baudrate() <= 1200)
    {
	terse = TRUE;
	jump = TRUE;
	see_floor = FALSE;
    }

    if (md_hasclreol())
	inv_type = INV_CLEAR;

    /*
     * parse environment declaration of options
     */
    strcpy(whoami2, whoami);
    if ((opts = getenv("ROGUEOPTS")) != NULL)
	parse_opts(opts);
    strcpy(whoami, whoami2);


    oldpos = hero;
    oldrp = roomin(&hero);
    while (playing)
	command();			/* Command execution */
    endit(0);
}

/*
 * quit:
 *	Have player make certain, then exit.
 */

void
quit(int sig)
{
    int oy, ox;

    NOOP(sig);

    /*
     * Reset the signal in case we got here via an interrupt
     */
    if (!q_comm)
	mpos = 0;
    getyx(curscr, oy, ox);
    msg("really quit?");
    if (readchar() == 'y')
    {
	signal(SIGINT, leave);
	clear();
	mvprintw(LINES - 2, 0, "You quit with %d gold pieces", purse);
	move(LINES - 1, 0);
	refresh();
	writelog(purse, 1, 0);
	score(purse, 1, 0);
	printf("[Press return to exit]\n");
	fflush(NULL);
	getchar();
	my_exit(0);
    }
    else
    {
	move(0, 0);
	clrtoeol();
	status();
	move(oy, ox);
	refresh();
	mpos = 0;
	count = 0;
	to_death = FALSE;
    }
}

/*
 * leave:
 *	Leave quickly, but curteously
 */

void
leave(int sig)
{
    static char buf[BUFSIZ];

    NOOP(sig);

    setbuf(stdout, buf);	/* throw away pending output */

    if (!isendwin())
    {
	mvcur(0, COLS - 1, LINES - 1, 0);
	endwin();
    }

    putchar('\n');
    my_exit(0);
}

/*
 * shell:
 *	Let them escape for a while
 */

void
shell(void)
{
    /*
     * Set the terminal back to original mode
     */
    move(LINES-1, 0);
    refresh();
    endwin();
    resetltchars();
    putchar('\n');
    in_shell = TRUE;
    after = FALSE;
    fflush(stdout);
    /*
     * Fork and do a shell
     */
    md_shellescape();

    noecho();
    raw();
    nonl();
    keypad(stdscr,1);
    playltchars();
    in_shell = FALSE;
    clearok(stdscr, TRUE);
}

/*
 * my_exit:
 *	Leave the process properly
 */

void
my_exit(int st)
{
    resetltchars();
    exit(st);
}

