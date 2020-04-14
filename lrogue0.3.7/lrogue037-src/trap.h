/*
 * trap.h
 *
 * Created by Ashwin N.
 */

#ifndef _TRAP_H_
#define _TRAP_H_

struct tr
{
	short trap_type;
	short trap_row, trap_col;
};

typedef struct tr trap;

void trap_player(short row, short col);

void add_traps(void);

void id_trap(void);

void show_traps(void);

void search(short n, boolean is_auto);

#endif	/* _TRAP_H_ */
