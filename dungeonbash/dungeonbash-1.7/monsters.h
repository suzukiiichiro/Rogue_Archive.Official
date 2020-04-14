/* monsters.h
 * 
 * Copyright 2005 Martin Read
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MONSTERS_H
#define MONSTERS_H

#ifndef DUNBASH_H
#include "dunbash.h"
#endif

/* XXX monsters.c data and funcs */
extern void update_mon(int mon);
extern void mon_acts(int mon);
extern void death_drop(int mon);
extern void print_mon_name(int mon, int article);
extern void summon_demon_near(int y, int x);
extern int create_mon(int pm_idx, int y, int x);
extern int summoning(int y, int x, int how_many);
extern int ood(int power, int ratio);
extern int get_random_pmon(void);
extern void damage_mon(int mon, int amount, int by_you);
extern int mon_can_pass(int mon, int y, int x);
extern int mon_visible(int mon);
extern void move_mon(int mon, int y, int x);
extern int teleport_mon(int mon);	/* Randomly relocate monster. */
extern int teleport_mon_to_you(int mon);	/* Relocate monster to your vicinity. */
extern void heal_mon(int mon, int amount, int cansee);

/* XXX mon2.c data and funcs */
extern void select_space(int *py, int *px, int dy, int dx, int selection_mode);

/* XXX pmon2.c data and funcs */
extern int pmon_is_archer(int pm);
extern int pmon_is_magician(int pm);
extern int pmon_is_smart(int pm);
extern int pmon_is_stupid(int pm);
extern int pmon_is_undead(int pm);
extern int pmon_resists_cold(int pm);
extern int pmon_resists_fire(int pm);
#endif

/* monsters.h */
//あ
