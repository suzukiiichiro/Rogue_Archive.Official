/* machdep.c */

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

#include "rogue.h"
#include "init.h"
#include "machdep.h"
#include "random.h"

/* md_slurp:
 *
 * This routine throws away all keyboard input that has not
 * yet been read.  It is used to get rid of input that the user may have
 * typed-ahead.
 *
 * This function is not necessary, so it may be stubbed.  The might cause
 * message-line output to flash by because the game has continued to read
 * input without waiting for the user to read the message.  Not such a
 * big deal.
 */

void md_slurp(void)
{
// TODO: Check if this is really needed or else remove
#if 0
#ifdef UNIX_BSD4_2
	long ln;
	int i, n;
	ioctl(0, FIONREAD, &ln);
	n = (int) (stdin->_cnt + ln);

	for (i = 0; i < n; i++)
	{
		(void) getchar();
	}
#endif	/* UNIX_BSD4_2 */
#endif
}

/* md_control_keyboard():
 *
 * This routine is much like md_cbreak_no_echo_nonl() above.  It sets up the
 * keyboard for appropriate input.  Specifically, it prevents the tty driver
 * from stealing characters.  For example, ^Y is needed as a command
 * character, but the tty driver intercepts it for another purpose.  Any
 * such behavior should be stopped.  This routine could be avoided if
 * we used RAW mode instead of CBREAK.  But RAW mode does not allow the
 * generation of keyboard signals, which the program uses.
 *
 * The parameter 'mode' when true, indicates that the keyboard should
 * be set up to play rogue.  When false, it should be restored if
 * necessary.
 *
 * This routine is not strictly necessary and may be stubbed.  This may
 * cause certain command characters to be unavailable.
 */

void md_control_keyboard(int mode)
{
	static boolean called_before = 0;
	static struct termios tc_orig;
	static struct termios tc_temp;

	signal(SIGTSTP,SIG_IGN);

	if (!called_before)
	{
		called_before = 1;
        tcgetattr(0, &tc_orig);	
	}
	tc_temp = tc_orig;

	if (!mode)
	{
		tc_temp.c_cc[VINTR] = -1;
		tc_temp.c_cc[VQUIT] = -1;
		tc_temp.c_cc[VERASE] = -1;
		tc_temp.c_cc[VKILL] = -1;
		tc_temp.c_cc[VEOF] = -1;
/* 		tc_temp.c_cc[VSWTCH] = -1; */
		tc_temp.c_cc[VSTART] = -1;
		tc_temp.c_cc[VSTOP] = -1;
		tc_temp.c_cc[VSUSP] = -1;
		tc_temp.c_cc[VLNEXT] = -1;
		tc_temp.c_cc[VWERASE] = -1;
		tc_temp.c_cc[VREPRINT] = -1;
		tc_temp.c_cc[VDISCARD] = -1;
	}
	tcsetattr(0, TCSANOW, &tc_temp);
}

/* md_heed_signals():
 *
 * This routine tells the program to call particular routines when
 * certain interrupts/events occur:
 *
 *      SIGINT: call onintr() to interrupt fight with monster or long rest.
 *      SIGQUIT: call byebye() to check for game termination.
 *      SIGHUP: call error_save() to save game when terminal hangs up.
 *
 *		On VMS, SIGINT and SIGQUIT correspond to ^C and ^Y.
 *
 * This routine is not strictly necessary and can be stubbed.  This will
 * mean that the game cannot be interrupted properly with keyboard
 * input, this is not usually critical.
 */

void md_heed_signals(void)
{
	signal(SIGINT, onintr);
	signal(SIGQUIT, byebye);
	signal(SIGHUP, error_save);
}

/* md_ignore_signals():
 *
 * This routine tells the program to completely ignore the events mentioned
 * in md_heed_signals() above.  The event handlers will later be turned on
 * by a future call to md_heed_signals(), so md_heed_signals() and
 * md_ignore_signals() need to work together.
 *
 * This function should be implemented or the user risks interrupting
 * critical sections of code, which could cause score file, or saved-game
 * file, corruption.
 */

void md_ignore_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGTSTP,SIG_IGN);	/* No ^Z ! */
}

/* md_get_file_id():
 *
 * This function returns an integer that uniquely identifies the specified
 * file.  It need not check for the file's existence.  In UNIX, the inode
 * number is used.
 *
 * This function need not be implemented.  To stub the routine, just make
 * it return 0.  This will make the game less able to prevent users from
 * modifying saved-game files.  This is probably no big deal.
 */

int md_get_file_id(char *fname)
{
	struct stat sbuf;

	if (stat(fname, &sbuf))
	{
		return(-1);
	}
	return((int) sbuf.st_ino);
}

/* md_link_count():
 *
 * This routine returns the number of hard links to the specified file.
 *
 * This function is not strictly necessary.  On systems without hard links
 * this routine can be stubbed by just returning 1.
 */

int md_link_count(char *fname)
{
	struct stat sbuf;

	stat(fname, &sbuf);
	return((int) sbuf.st_nlink);
}

/* md_gct(): (Get Current Time)
 *
 * This function returns the current year, month(1-12), day(1-31), hour(0-23),
 * minute(0-59), and second(0-59).  This is used for identifying the time
 * at which a game is saved.
 *
 * This function is not strictly necessary.  It can be stubbed by returing
 * zeros instead of the correct year, month, etc.  If your operating
 * system doesn't provide all of the time units requested here, then you
 * can provide only those that it does, and return zeros for the others.
 * If you cannot provide good time values, then users may be able to copy
 * saved-game files and play them.  
 */

void md_gct(struct rogue_time *rt_buf)
{
	time_t rawtime;
	struct tm * now;

	time(&rawtime);
	now=localtime(&rawtime);

	rt_buf->year = now->tm_year;
	rt_buf->month = now->tm_mon+1;
	rt_buf->day = now->tm_mday;
	rt_buf->hour = now->tm_hour;
	rt_buf->minute = now->tm_min;
	rt_buf->second = now->tm_sec;
}

/* md_gfmt: (Get File Modification Time)
 *
 * This routine returns a file's date of last modification in the same format
 * as md_gct() above.
 *
 * This function is not strictly necessary.  It is used to see if saved-game
 * files have been modified since they were saved.  If you have stubbed the
 * routine md_gct() above by returning constant values, then you may do
 * exactly the same here.
 * Or if md_gct() is implemented correctly, but your system does not provide
 * file modification dates, you may return some date far in the past so
 * that the program will never know that a saved-game file being modified.  
 * You may also do this if you wish to be able to restore games from
 * saved-games that have been modified.
 */

void md_gfmt(char *fname, struct rogue_time *rt_buf)
{
	//time_t rawtime;
	struct tm * now;
	struct stat attrib;

	stat(fname,&attrib);

	now=localtime(&(attrib.st_mtime));

	rt_buf->year = now->tm_year;
	rt_buf->month = now->tm_mon+1;
	rt_buf->day = now->tm_mday;
	rt_buf->hour = now->tm_hour;
	rt_buf->minute = now->tm_min;
	rt_buf->second = now->tm_sec;
}

/* md_df: (Delete File)
 *
 * This function deletes the specified file, and returns true (1) if the
 * operation was successful.  This is used to delete saved-game files
 * after restoring games from them.
 *
 * Again, this function is not strictly necessary, and can be stubbed
 * by simply returning 1.  In this case, saved-game files will not be
 * deleted and can be replayed.
 */

boolean md_df(char *fname)
{
	#ifdef SAVE_CHEAT
		return 1;
	#else
		if (unlink(fname))
		{
			perror("unlink error");
			return(0);
		}

		return(1);
	#endif
}

/* md_gln: (Get login name)
 *
 * This routine returns the login name of the user.  This string is
 * used mainly for identifying users in score files.
 *
 * A dummy string may be returned if you are unable to implement this
 * function, but then the score file would only have one name in it.
 */

char * md_gln(void)
{
	struct passwd *t;
	t = getpwuid(getuid());
	return(t->pw_name);
}

/* md_sleep:
 *
 * This routine causes the game to pause for the specified number of
 * seconds.
 *
 * This routine is not necessary at all, and can be stubbed with no ill
 * effects.
 */

void md_sleep(int nsecs)
{
	(void) sleep(nsecs);
}

/* md_malloc()
 *
 * This routine allocates, and returns a pointer to, the specified number
 * of bytes.  This routines absolutely MUST be implemented for your
 * particular system or the program will not run at all.  Return zero
 * when no more memory can be allocated.
 */

char * md_malloc(int n)
{
        void * malloc();
        char * t;

        t = (char *) malloc(n);

        return(t);
}

/* md_gseed() (Get Seed)
 *
 * This function returns a seed for the random number generator (RNG).  This
 * seed causes the RNG to begin generating numbers at some point in it's
 * sequence.  Without a random seed, the RNG will generate the same set
 * of numbers, and every game will start out exactly the same way.  A good
 * number to use is the process id, given by getpid() on most UNIX systems.
 *
 * You need to find some single random integer, such as:
 *   process id.
 *   current time (minutes + seconds) returned from md_gct(), if implemented.
 *   
 * It will not help to return "get_rand()" or "rand()" or the return value of
 * any pseudo-RNG.  If you cannot a random number, you can just return 1,
 * but this means you games will ALWAYS start the same way, and will play
 * exactly the same way given the same input.
 */

int md_gseed(void)
{
	return(getpid());
}

/* md_exit():
 *
 * This function causes the program to discontinue execution and exit.
 * This function must be implemented or the program will continue to
 * hang when it should quit.
 */

void md_exit(int status)
{
	exit(status);
}
