/*
 * hit.h
 *
 * Created by Ashwin N.
 */

#ifndef _HIT_H_
#define _HIT_H_

void mon_hit(object *monster, char *other, boolean flame);

void rogue_hit(object *monster, boolean force_hit);

int get_damage(char *ds, boolean r);

int get_number(char *s);

long lget_number(char *s);

int mon_damage(object *monster, short damage);

void fight(boolean to_the_death);

void get_dir_rc(short dir, short *row, short *col, short allow_off_screen);

short get_hit_chance(object *weapon);

short get_weapon_damage(object *weapon);

#endif	/* _HIT_H_ */
