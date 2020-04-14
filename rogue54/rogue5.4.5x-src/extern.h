/*
 * Defines for things used in mach_dep.c
 *
 * @(#)extern.h	4.35 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */


#ifdef HAVE_CONFIG_H
#ifdef PDCURSES
#undef HAVE_UNISTD_H
#undef HAVE_LIMITS_H
#undef HAVE_MEMORY_H
#undef HAVE_STRING_H
#endif
#include "config.h"
#elif defined(__DJGPP__)
#define HAVE_SYS_TYPES_H 1
#define HAVE_PROCESS_H 1
#define HAVE_PWD_H 1
#define HAVE_TERMIOS_H 1
#define HAVE_SETGID 1
#define HAVE_GETGID 1
#define HAVE_SETUID 1
#define HAVE_GETUID 1
#define HAVE_GETPASS 1
#define HAVE_SPAWNL 1
#define HAVE_ALARM 1
#define HAVE_ERASECHAR 1
#define HAVE_KILLCHAR 1
#elif defined(_WIN32)
#define HAVE_CURSES_H
#define HAVE_TERM_H
#define HAVE__SPAWNL
#define HAVE_SYS_TYPES_H
#define HAVE_PROCESS_H
#define HAVE_ERASECHAR 1
#define HAVE_KILLCHAR 1
#ifndef uid_t
typedef unsigned int uid_t;
#endif
#ifndef pid_t
typedef unsigned int pid_t;
#endif
#elif defined(__CYGWIN__)
#define HAVE_SYS_TYPES_H 1
#define HAVE_PWD_H 1
#define HAVE_PWD_H 1
#define HAVE_SYS_UTSNAME_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_UNISTD_H 1
#define HAVE_TERMIOS_H 1
#define HAVE_NCURSES_TERM_H 1
#define HAVE_ESCDELAY
#define HAVE_SETGID 1
#define HAVE_GETGID 1
#define HAVE_SETUID 1
#define HAVE_GETUID 1
#define HAVE_GETPASS 1
#define HAVE_GETPWUID 1
#define HAVE_WORKING_FORK 1
#define HAVE_ALARM 1
#define HAVE_SPAWNL 1
#define HAVE__SPAWNL 1
#define HAVE_ERASECHAR 1
#define HAVE_KILLCHAR 1
#else /* POSIX */
#define HAVE_SYS_TYPES_H 1
#define HAVE_PWD_H 1
#define HAVE_PWD_H 1
#define HAVE_SYS_UTSNAME_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_UNISTD_H 1
#define HAVE_TERMIOS_H 1
#define HAVE_TERM_H 1
#define HAVE_SETGID 1
#define HAVE_GETGID 1
#define HAVE_SETUID 1
#define HAVE_GETUID 1
#define HAVE_SETREUID 1
#define HAVE_SETREGID 1
#define HAVE_GETPASS 1
#define HAVE_GETPWUID 1
#define HAVE_WORKING_FORK 1
#define HAVE_ERASECHAR 1
#define HAVE_KILLCHAR 1
#ifndef _AIX
#define HAVE_GETLOADAVG 1
#endif
#define HAVE_ALARM 1
#endif

#ifdef __DJGPP__
#undef HAVE_GETPWUID /* DJGPP's limited version doesn't even work as documented */
#endif

/*
 * Don't change the constants, since they are used for sizes in many
 * places in the program.
 */

#include <stdlib.h>

#undef SIGTSTP

#define MAXSTR		1024	/* maximum length of strings */
#define MAXLINES	32	/* maximum number of screen lines used */
#define MAXCOLS		80	/* maximum number of screen columns used */

#define RN		(((seed = seed*11109+13849) >> 16) & 0xffff)
#ifdef CTRL
#undef CTRL
#endif
#define CTRL(c)		(c & 037)

/*
 * Now all the global variables
 */

extern int got_ltc, in_shell;
extern int	wizard;
extern char	fruit[], prbuf[], whoami[];
extern int orig_dsusp;
extern FILE	*scoreboard;
extern FILE     *logfi;
extern int numscores;
extern char *Numname;
extern int allscore;
extern int use_savedir;
extern char *Numname;

extern int	max_pot_prob;
extern int	max_scr_prob;
extern int	max_ws_prob;
extern int	rname;
extern int	nopt;
extern char	*class_name[];
extern int	slime_md;
extern int	slime_md2;
extern int	idscr_md;
extern int	pc_md;
extern int	s_time;
extern char	*snake_name;
extern char	*bunicorn_name;
extern char	*slime_name;
extern char	*urvile_name;
extern int	color_mode;
extern int	color_mode2;
extern int	bright_mode;
extern int	bright_mode2;
extern int	reverse_attr;
extern int	ccolor[2][0x60];
extern int	fcolor[];
extern int	bcolor[];
extern int	hcolor[];
extern int	_COLS, _LINES;

void	refresh2(void);
void	start_custom_color(void);
#define	addch2(c)	waddch2(stdscr,c)
int	waddch2(WINDOW *, const chtype);
#define	mvaddch2(y,x,c)	mvwaddch2(stdscr,y,x,c)
int	mvwaddch2(WINDOW *, int, int, const chtype);
int	standend2(void);

/*
 * Function types
 */

int	md_chmod(const char *filename, int mode);
char	*md_crypt(const char *key, const char *salt);
int	md_dsuspchar(void);
int	md_erasechar(void);
char	*md_gethomedir(void);
char	*md_getusername(void);
uid_t	md_getuid(void);
char	*md_getpass(char *prompt);
pid_t	md_getpid(void);
char	*md_getrealname(uid_t uid);
void	md_init(void);
int	md_killchar(void);
void	md_normaluser(void);
void	md_raw_standout(void);
void	md_raw_standend(void);
int	md_readchar(WINDOW *win);
int	md_setdsuspchar(int c);
int	md_shellescape(void);
void	md_sleep(int s);
int	md_suspchar(void);
int	md_hasclreol(void);
int	md_unlink(char *file);
int	md_unlink_open_file(const char *file, FILE *inf);
void md_tstpsignal(void);
void md_tstphold(void);
void md_tstpresume(void);
void md_ignoreallsignals(void);
void md_onsignal_autosave(void);
void md_onsignal_exit(void);
void md_onsignal_default(void);
int md_issymlink(char *sp);
extern char *xcrypt(const char *key, const char *setting);
void	resetcolor(void);

