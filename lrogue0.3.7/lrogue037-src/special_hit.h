/*
 * special_hit.h
 *
 * Created by Ashwin N.
 */

#ifndef _SPECIAL_HIT_H_
#define _SPECIAL_HIT_H_

void special_hit(object *monster);

void rust(object *monster);

void cough_up(object *monster);

int seek_gold(object *monster);

void check_gold_seeker(object *monster);

int check_imitator(object *monster);

int imitating(short row, short col);

int m_confuse(object *monster);

int flame_broil(object *monster);

#endif	/* _SPECIAL_HIT_H_ */
