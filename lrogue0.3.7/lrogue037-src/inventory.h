/*
 * inventory.h
 *
 * Created by Ashwin N.
 */

#ifndef _INVENTORY_H_
#define _INVENTORY_H_

char inv_sel(object *pack, unsigned short mask, char *prompt, char *term);

void inventory(object *pack, unsigned short mask);

void mix_colors(void);

void make_scroll_titles(void);

void get_desc(object *obj, char *desc);

void get_wand_and_ring_materials(void);

void single_inv(short ichar);

struct id * get_id_table(object *obj);

void inv_armor_weapon(boolean is_weapon);

#endif	/* _INVENTORY_H_ */
