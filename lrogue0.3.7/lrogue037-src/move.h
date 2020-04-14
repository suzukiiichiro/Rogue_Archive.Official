/*
 * move.h
 *
 * Created by Ashwin N.
 */

#ifndef _MOVE_H_
#define _MOVE_H_

int move_rogue(short dirch, short pickup);

int one_move_rogue(short dirch, short pickup);

void multiple_move_rogue(int dirch);

int is_passable(int row, int col);

int can_move(int row1, int col1, int row2, int col2);

void move_onto(void);

boolean is_direction(char c);

boolean reg_move(void);

void rest(int count);

#endif	/* _MOVE_H_ */
