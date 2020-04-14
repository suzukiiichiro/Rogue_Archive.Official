/*
 * monster.h
 *
 * Created by Ashwin N.
 */

#ifndef _MONSTER_H_
#define _MONSTER_H_

void put_mons(void);

object * gr_monster(object *monster, int mn);

void mv_mons(void);

void party_monsters(int rn, int n);

char gmc_row_col(int row, int col);

char gmc(object *monster);

void mv_monster(object *monster, short row, short col);

void move_mon_to(object *monster, short row, short col);

int mon_can_go(object *monster, short row, short col);

void wake_up(object *monster);

void wake_room(int rn, boolean entering, int row, int col);

char * mon_name(object *monster);

void fix_mon_damage(object *monster);

short rogue_is_around(int row, int col);

void wanderer(void);

void show_monsters(void);

void create_monster(void);

int rogue_can_see(int row, int col);

char gr_obj_char(void);

void aggravate(void);

boolean mon_sees(object *monster, int row, int col);

void mv_aquatars(void);

#endif	/* _MONSTER_H_ */
