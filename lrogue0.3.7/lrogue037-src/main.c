/* main.c */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "rogue.h"
#include "init.h"
#include "level.h"
#include "main.h"
#include "message.h"
#include "monster.h"
#include "object.h"
#include "play.h"
#include "trap.h"

extern short party_room;
extern object level_objects;
extern object level_monsters;

/*int saved_uid;
int true_uid;*/

/*void turn_into_games(void)
{
	if(setuid(saved_uid) == -1)
	{
		perror("setuid restore failed!");
		clean_up("");
	}
}

void turn_into_user(void)
{
	if(setuid(true_uid)==-1)
	{
		perror("setuid(restore)");
		clean_up("");
	}
}*/

int main(int argc, char *argv[])
{
	/* Save the setuid we have got, then turn back into the player */
//	saved_uid = geteuid();
//	setuid(true_uid = getuid());

#ifdef __CYGWIN__
	setenv("TERMINFO", "terminfo", 1);
#endif

	if (init(argc, argv))	/* restored game */
	{
		goto PL;
	}

	for (;;)
	{
		clear_level();
		make_level();
		put_objects();
		put_stairs();
		add_traps();
		put_mons();
		put_player(party_room);
		print_stats(STAT_ALL);
PL:		
		play_level();
		free_stuff(&level_objects);
		free_stuff(&level_monsters);
	}

	return 0;
}
