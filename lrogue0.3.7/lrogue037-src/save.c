/* save.c */

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
#include "machdep.h"
#include "message.h"
#include "object.h"
#include "pack.h"
#include "ring.h"
#include "room.h"
#include "save.h"
#include "score.h"
#include "trap.h"
#include "monster.h"

short write_failed = 0;
char *save_file = "";

extern boolean detect_monster;
extern int cur_level;
extern short max_level;
extern char hunger_str[];
extern char login_name[];
extern short party_room;
extern int party_counter;
extern int foods;
extern boolean is_wood[];
extern int cur_room;
extern boolean being_held;
extern int bear_trap;
extern short halluc;
extern short blind;
extern short confused;
extern short levitate;
extern short haste_self;
extern boolean see_invisible;
extern boolean wizard;
extern boolean score_only;
extern short m_moves;
extern boolean msg_cleared;
extern object level_monsters;
extern object level_objects;
extern fighter rogue;
extern struct id id_scrolls[];
extern struct id id_potions[];
extern struct id id_wands[];
extern struct id id_rings[];
extern struct id id_weapons[];
extern struct id id_armors[];
extern trap traps[];
extern unsigned short dungeon[DROWS][DCOLS];
extern room rooms[];

void r_write(FILE *, char *, int);
void write_string(char *, FILE *);
void write_pack(object *, FILE *);
void rw_dungeon(FILE *, boolean);
void rw_id_alloc(struct id [], FILE *, int, boolean);
void rw_rooms(FILE *, boolean);
void r_read(FILE *, char *, int);
void read_string(char *, FILE *);
void read_pack(object *, FILE *, boolean);
boolean has_been_touched(struct rogue_time *, struct rogue_time *);

void save_game(void)
{
	char fname[64];

	#ifdef ONE_SAVE_PER_PLAYER
		sprintf(fname,"%s%s.sav",ROGUE_SAVEPATH,login_name);
		save_into_file(fname);
	#else
	if (!get_input_line("File name?", fname, save_file, "Game not saved.",
	    0, 1))
	{
		return;
	}
	check_message();
	message(fname, 0);
	save_into_file(fname);
	#endif
}

void save_into_file(char *sfile)
{
	FILE *fp;
	int file_id;
	char name_buffer[80];
	char *hptr;
	struct rogue_time rt_buf;

	if (sfile[0] == '~')
	{
		if ((hptr = getenv("HOME")))
		{
			/* Security fix, Ulf Harnhammar 2003 */
			(void) strncpy(name_buffer, hptr, sizeof(name_buffer));
			(void) strncat(name_buffer, sfile+1, sizeof(name_buffer) - strlen(hptr));
			name_buffer[sizeof(name_buffer) - 1] = '\0';

			sfile = name_buffer;
		}
	}
	if (((fp = fopen(sfile, "w")) == NULL)
	    || ((file_id = md_get_file_id(sfile)) == -1))
	{
		message("Problem accessing the save file.", 0);
		return;
	}
	md_ignore_signals();
	write_failed = 0;
	r_write(fp, (char *) &detect_monster, sizeof(detect_monster));
	r_write(fp, (char *) &cur_level, sizeof(cur_level));
	r_write(fp, (char *) &max_level, sizeof(max_level));
	write_string(hunger_str, fp);
	write_string(login_name, fp);
	r_write(fp, (char *) &party_room, sizeof(party_room));
	r_write(fp, (char *) &party_counter, sizeof(party_counter));
	write_pack(&level_monsters, fp);
	write_pack(&level_objects, fp);
	r_write(fp, (char *) &file_id, sizeof(file_id));
	rw_dungeon(fp, 1);
	r_write(fp, (char *) &foods, sizeof(foods));
	r_write(fp, (char *) &rogue, sizeof(fighter));
	write_pack(&rogue.pack, fp);
	rw_id_alloc(id_potions, fp, POTIONS, 1);
	rw_id_alloc(id_scrolls, fp, SCROLLS, 1);
	rw_id_alloc(id_wands, fp, WANDS, 1);
	rw_id_alloc(id_rings, fp, RINGS, 1);
	r_write(fp, (char *) traps, (MAX_TRAPS * sizeof(trap)));
	r_write(fp, (char *) is_wood, (WANDS * sizeof(boolean)));
	r_write(fp, (char *) &cur_room, sizeof(cur_room));
	rw_rooms(fp, 1);
	r_write(fp, (char *) &being_held, sizeof(being_held));
	r_write(fp, (char *) &bear_trap, sizeof(bear_trap));
	r_write(fp, (char *) &halluc, sizeof(halluc));
	r_write(fp, (char *) &blind, sizeof(blind));
	r_write(fp, (char *) &confused, sizeof(confused));
	r_write(fp, (char *) &levitate, sizeof(levitate));
	r_write(fp, (char *) &haste_self, sizeof(haste_self));
	r_write(fp, (char *) &see_invisible, sizeof(see_invisible));
	r_write(fp, (char *) &detect_monster, sizeof(detect_monster));
	r_write(fp, (char *) &wizard, sizeof(wizard));
	r_write(fp, (char *) &score_only, sizeof(score_only));
	r_write(fp, (char *) &m_moves, sizeof(m_moves));
	md_gct(&rt_buf);
	rt_buf.second += 10;		/* allow for some processing time */
	r_write(fp, (char *) &rt_buf, sizeof(rt_buf));
	fclose(fp);

	if (write_failed)
	{
		(void) md_df(sfile);	/* delete file */
	}
	else
	{
		clean_up("");
	}
}

void restore(char *fname)
{
	FILE *fp = NULL;
	struct rogue_time saved_time, mod_time;
	char buf[4];
	char tbuf[40];
	int new_file_id, saved_file_id;

	if (((new_file_id = md_get_file_id(fname)) == -1)
	    || ((fp = fopen(fname, "r")) == NULL))
	{
		clean_up("cannot open file");
	}
	if (md_link_count(fname) > 1)
	{
		clean_up("file has link");
	}
	r_read(fp, (char *) &detect_monster, sizeof(detect_monster));
	r_read(fp, (char *) &cur_level, sizeof(cur_level));
	r_read(fp, (char *) &max_level, sizeof(max_level));
	read_string(hunger_str, fp);

	(void) strcpy(tbuf, login_name);
	read_string(login_name, fp);
	if (strcmp(tbuf, login_name))
	{
		clean_up("you're not the original player");
	}

	r_read(fp, (char *) &party_room, sizeof(party_room));
	r_read(fp, (char *) &party_counter, sizeof(party_counter));
	read_pack(&level_monsters, fp, 0);
	read_pack(&level_objects, fp, 0);
	r_read(fp, (char *) &saved_file_id, sizeof(saved_file_id));
	if (new_file_id != saved_file_id)
	{
		clean_up("sorry, saved game is not in the same file");
	}
	rw_dungeon(fp, 0);
	r_read(fp, (char *) &foods, sizeof(foods));
	r_read(fp, (char *) &rogue, sizeof(fighter));
	read_pack(&rogue.pack, fp, 1);
	rw_id_alloc(id_potions, fp, POTIONS, 0);
	rw_id_alloc(id_scrolls, fp, SCROLLS, 0);
	rw_id_alloc(id_wands, fp, WANDS, 0);
	rw_id_alloc(id_rings, fp, RINGS, 0);
	r_read(fp, (char *) traps, (MAX_TRAPS * sizeof(trap)));
	r_read(fp, (char *) is_wood, (WANDS * sizeof(boolean)));
	r_read(fp, (char *) &cur_room, sizeof(cur_room));
	rw_rooms(fp, 0);
	r_read(fp, (char *) &being_held, sizeof(being_held));
	r_read(fp, (char *) &bear_trap, sizeof(bear_trap));
	r_read(fp, (char *) &halluc, sizeof(halluc));
	r_read(fp, (char *) &blind, sizeof(blind));
	r_read(fp, (char *) &confused, sizeof(confused));
	r_read(fp, (char *) &levitate, sizeof(levitate));
	r_read(fp, (char *) &haste_self, sizeof(haste_self));
	r_read(fp, (char *) &see_invisible, sizeof(see_invisible));
	r_read(fp, (char *) &detect_monster, sizeof(detect_monster));
	r_read(fp, (char *) &wizard, sizeof(wizard));
	r_read(fp, (char *) &score_only, sizeof(score_only));
	r_read(fp, (char *) &m_moves, sizeof(m_moves));
	r_read(fp, (char *) &saved_time, sizeof(saved_time));

	if (fread(buf, sizeof(char), 1, fp) > 0)
	{
		clear();
		clean_up("extra characters in file");
	}

	md_gfmt(fname, &mod_time);	/* get file modification time */

	if (has_been_touched(&saved_time, &mod_time))
	{
		clear();
		clean_up("sorry, file has been touched");
	}
	if ((!wizard) && !md_df(fname))
	{
		clean_up("cannot delete file");
	}
	msg_cleared = 0;
	ring_stats(0);
	fclose(fp);
}

void write_pack(object *pack, FILE *fp)
{
	object t;

	while ((pack = pack->next_object))
	{
		r_write(fp, (char *) pack, sizeof(object));
	}
	t.ichar = t.what_is = 0;
	r_write(fp, (char *) &t, sizeof(object));
}

void read_pack(object *pack, FILE *fp, boolean is_rogue)
{
	object read_obj, *new_obj;
	int mon_flag = (pack == &level_monsters);

	for (;;)
	{
		r_read(fp, (char *) &read_obj, sizeof(object));
		if (read_obj.ichar == 0)
		{
			pack->next_object = (object *) 0;
			break;
		}
		new_obj = alloc_object();
		*new_obj = read_obj;
		if (new_obj->what_is == WEAPON)
		{
			set_weapon_damage(new_obj);
		}
		if (mon_flag)
		{
			fix_mon_damage(new_obj);
		}
		if (is_rogue)
		{
			if (new_obj->in_use_flags & BEING_WORN) {
					do_wear(new_obj);
			} else if (new_obj->in_use_flags & BEING_WIELDED) {
					do_wield(new_obj);
			} else if (new_obj->in_use_flags & (ON_EITHER_HAND)) {
				do_put_on(new_obj,
					((new_obj->in_use_flags & ON_LEFT_HAND) ? 1 : 0));
			}
		}
		pack->next_object = new_obj;
		pack = new_obj;
	}
}

void rw_dungeon(FILE *fp, boolean rw)
{
	short i, j;
	char buf[DCOLS];

	for (i = 0; i < DROWS; i++)
	{
		if (rw)
		{
			r_write(fp, (char *) dungeon[i], (DCOLS * sizeof(dungeon[0][0])));
			for (j = 0; j < DCOLS; j++)
			{
				buf[j] = mvinch(i, j);
			}
			r_write(fp, buf, DCOLS);
		}
		else
		{
			r_read(fp, (char *) dungeon[i], (DCOLS * sizeof(dungeon[0][0])));
			r_read(fp, buf, DCOLS);
			for (j = 0; j < DCOLS; j++)
			{
				mvaddch(i, j, buf[j]);
			}
		}
	}
}

void rw_id(struct id id_table[], FILE *fp, int n, boolean wr)
{
	int i;

	for (i = 0; i < n; i++)
	{
		if (wr)
		{
			r_write(fp, (char *) &(id_table[i].value), sizeof(int));
			r_write(fp, (char *) &(id_table[i].id_status),
				sizeof(unsigned int));
			write_string(id_table[i].title, fp);
		}
		else
		{
			r_read(fp, (char *) &(id_table[i].value), sizeof(int));
			r_read(fp, (char *) &(id_table[i].id_status),
				sizeof(unsigned int));
			read_string(id_table[i].title, fp);
		}
	}
}

void rw_id_alloc(struct id id_table[], FILE *fp, int n, boolean wr)
{
	int i;

	for (i = 0; i < n; i++)
	{
		if (wr)
		{
			r_write(fp, (char *) &(id_table[i].value), sizeof(int));
			r_write(fp, (char *) &(id_table[i].id_status),
				sizeof(unsigned int));
			write_string(id_table[i].title, fp);
		}
		else
		{
			r_read(fp, (char *) &(id_table[i].value), sizeof(int));
			r_read(fp, (char *) &(id_table[i].id_status),
				sizeof(unsigned int));
			read_string(id_table[i].title, fp);
		}
	}
}

void write_string(char *s, FILE *fp)
{
	short n;

	n = strlen(s) + 1;
	r_write(fp, (char *) &n, sizeof(short));
	r_write(fp, s, n);
}

void read_string(char *s, FILE *fp)
{
	short n;

	r_read(fp, (char *) &n, sizeof(short));
	r_read(fp, s, n);
}

void rw_rooms(FILE *fp, boolean rw)
{
	short i;

	for (i = 0; i < MAXROOMS; i++)
	{
		rw ? r_write(fp, (char *) (rooms + i), sizeof(room)) :
			r_read(fp, (char *) (rooms + i), sizeof(room));
	}
}

void r_read(FILE *fp, char *buf, int n)
{
	if (fread(buf, sizeof(char), n, fp) != n)
	{
		clean_up("Read() failed, don't know why.");
	}
}

void r_write(FILE *fp, char *buf, int n)
{
	if (!write_failed)
	{
		if (fwrite(buf, sizeof(char), n, fp) != n)
		{
			message("Write() failed, don't know why.", 0);
			sound_bell();
			write_failed = 1;
		}
	}
}

boolean has_been_touched(struct rogue_time *saved_time,
                         struct rogue_time *mod_time)
{
	if (saved_time->year < mod_time->year) {
		return(1);
	} else if (saved_time->year > mod_time->year) {
		return(0);
	}
	if (saved_time->month < mod_time->month) {
		return(1);
	} else if (saved_time->month > mod_time->month) {
		return(0);
	}
	if (saved_time->day < mod_time->day) {
		return(1);
	} else if (saved_time->day > mod_time->day) {
		return(0);
	}
	if (saved_time->hour < mod_time->hour) {
		return(1);
	} else if (saved_time->hour > mod_time->hour) {
		return(0);
	}
	if (saved_time->minute < mod_time->minute) {
		return(1);
	} else if (saved_time->minute > mod_time->minute) {
		return(0);
	}
	if (saved_time->second < mod_time->second) {
		return(1);
	}
	return(0);
}
