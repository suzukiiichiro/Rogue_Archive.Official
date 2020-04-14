/*
 * room.h
 *
 * Created by Ashwin N.
 */

#ifndef _ROOM_H_
#define _ROOM_H_

struct dr
{
	short oth_room;
	short oth_row,
	      oth_col;
	short door_row,
		  door_col;
};

typedef struct dr door;

struct rm
{
	char bottom_row, right_col, left_col, top_row;
	door doors[4];
	unsigned short is_room;
};

typedef struct rm room;

void light_up_room(int rn);

void light_passage(int row, int col);

void darken_room(int rn);

char get_dungeon_char(int row, int col);

char get_mask_char(unsigned short mask);

void gr_row_col(int *row, int *col, unsigned short mask);

short gr_room(void);

short party_objects(int rn);

int get_room_number(int row, int col);

int is_all_connected(void);

void draw_magic_map(void);

void dr_course(object *monster, boolean entering, int row, int col);

#endif	/* _ROOM_H_ */
