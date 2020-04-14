/* mon2.c
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

/* TODO: Convert missile AI to a new-style AI function. */
#define MON2_C
#include "dunbash.h"
#include "bmagic.h"
#include "monsters.h"
#include "combat.h"

/* AI map cell descriptor. */
struct ai_cell {
    int y, x;
    int dy, dx;
    int score;
};

/* prototypes for AI preference functions. */
static void get_naive_prefs(int y, int x, int dy, int dx, int *pref_y, int *pref_x);
static void get_seeking_prefs(int y, int x, int dy, int dx, int *pref_y, int *pref_x);
static void get_drunk_prefs(int y, int x, int dy, int dx, int *pref_y, int *pref_x);
static void build_ai_cells(struct ai_cell *cells, int y, int x);
static int ai_cell_compare(struct ai_cell *cell, int dy, int dx);
static void get_dodger_prefs(int y, int x, int dy, int dx, int *pref_y, int *pref_x);
static void get_chase_prefs(int mon, int *pref_y, int *pref_x);

/* get_drunk_prefs()
 *
 * Fills the three-entry preference arrays with three randomly-selected
 * adjacent squares.
 */

static void get_drunk_prefs(int y, int x, int dy, int dx, int *pref_y, int *pref_x)
{
    int sy, sx;
    int tryct;
    int pref_idx;
    int idx2;
    int retry;
    pref_y[0] = y;
    pref_y[1] = y;
    pref_y[2] = y;
    pref_x[0] = x;
    pref_x[1] = x;
    pref_x[2] = x;
    for (pref_idx = 0; pref_idx < 3; pref_idx++)
    {
	for (tryct = 0; tryct < 40; tryct++)
	{
	    retry = 0;
	    sy = zero_die(3) - 1;
	    sx = zero_die(3) - 1;
	    if (!sy && !sx)
	    {
		continue;
	    }
	    for (idx2 = 0; idx2 < pref_idx; idx2++)
	    {
		if ((pref_y[idx2] == y + sy) &&
		    (pref_x[idx2] == x + sx))
		{
		    retry = 1;
		    break;
		}
	    }
	    if (retry) 
	    {
		continue;
	    }
	    pref_y[pref_idx] = y + sy;
	    pref_x[pref_idx] = x + sx;
	    break;
	}
    }
}

/* get_chase_prefs()
 *
 * The naive "chase" AI is used by non-stupid non-smart monsters to chase your
 * last known position. If after moving towards it once they can't see you,
 * they will give up and revert to "drunk" AI. (Contrast stupid monsters,
 * who always use "drunk" AI if they can't see you, and smart monsters, who
 * always use "seeking" AI if they can't see you.)
 *
 * This function takes different parameters to the other AI preference
 * functions because it has to have access to the monster's lasty/lastx
 * details.
 */

static void get_chase_prefs(int mon, int *pref_y, int *pref_x)
{
    int sy, sx;
    int ady, adx;
    int dy, dx;
    int y, x;
    y = monsters[mon].y;
    x = monsters[mon].x;
    dy = monsters[mon].ai_lasty - y;
    dx = monsters[mon].ai_lastx - x;
    if (dy == 0)
    {
	sy = 0;
	ady = 0;
    }
    else
    {
	sy = dy < 0 ? -1 : 1;
	ady = dy < 0 ? -dy : dy;
    }
    if (dx == 0)
    {
	sx = 0;
	adx = 0;
    }
    else
    {
	sx = dx < 0 ? -1 : 1;
	adx = dx < 0 ? -dx : dx;
    }
    if (mon_can_pass(mon, y + sy, x + sx))
    {
	*pref_y = y + sy;
	*pref_x = x + sx;
    }
    else if (!sy)
    {
	/* We're on the horizontal; check the horizontally adjacent
	 * square, then the squares one square north or south in a
	 * random order. */
	if (zero_die(2))
	{
	    pref_y[1] = y - 1;
	    pref_y[2] = y + 1;
	}
	else
	{
	    pref_y[1] = y + 1;
	    pref_y[2] = y - 1;
	}
	pref_x[1] = x + sx;
	pref_x[2] = x + sx;
	if (mon_can_pass(mon, pref_y[1], pref_x[1]))
	{
	    pref_y[0] = pref_y[1];
	    pref_x[0] = pref_x[1];
	}
	else if (mon_can_pass(mon, pref_y[2], pref_x[2]))
	{
	    pref_y[0] = pref_y[2];
	    pref_x[0] = pref_x[2];
	}
	else
	{
	    pref_y[0] = monsters[mon].y;
	    pref_x[0] = monsters[mon].x;
	}
    }
    else if (!sx)
    {
	/* We're on the horizontal; check the horizontally adjacent
	 * square, then the squares one square north or south in a
	 * random order. */
	if (zero_die(2))
	{
	    pref_x[1] = x - 1;
	    pref_x[2] = x + 1;
	}
	else
	{
	    pref_x[1] = x + 1;
	    pref_x[2] = x - 1;
	}
	pref_y[1] = y + sy;
	pref_y[2] = y + sy;
	if (mon_can_pass(mon, pref_y[1], pref_x[1]))
	{
	    pref_y[0] = pref_y[1];
	    pref_x[0] = pref_x[1];
	}
	else if (mon_can_pass(mon, pref_y[2], pref_x[2]))
	{
	    pref_y[0] = pref_y[2];
	    pref_x[0] = pref_x[2];
	}
	else
	{
	    pref_y[0] = monsters[mon].y;
	    pref_x[0] = monsters[mon].x;
	}
    }
    else
    {
	if (zero_die(2))
	{
	    pref_x[1] = x;
	    pref_y[1] = y + sy;
	    pref_x[2] = x + sx;
	    pref_y[2] = y;
	}
	else
	{
	    pref_x[2] = x;
	    pref_y[2] = y + sy;
	    pref_x[1] = x + sx;
	    pref_y[1] = y;
	}
	if (mon_can_pass(mon, pref_y[1], pref_x[1]))
	{
	    pref_y[0] = pref_y[1];
	    pref_x[0] = pref_x[1];
	}
	else if (mon_can_pass(mon, pref_y[2], pref_x[2]))
	{
	    pref_y[0] = pref_y[2];
	    pref_x[0] = pref_x[2];
	}
	else
	{
	    pref_y[0] = monsters[mon].y;
	    pref_x[0] = monsters[mon].x;
	}
    }
}

/* get_seeking_prefs()
 *
 * Does all the work of finding the best (or least-bad) square for a seeking
 * AI monster to move to.
 */

static void get_seeking_prefs(int y, int x, int dy, int dx, int *pref_y, int *pref_x)
{
    struct ai_cell ai_cells[8];
    int i;
    int ady, adx;
    int j;
    int highest_score = -10000;
    int tryct;
    int uroom;
    int mroom;
    int troom;
    *pref_y = y;
    *pref_x = x;
    ady = dy > 0 ? dy : -dy;
    adx = dx > 0 ? dx : -dx;
    build_ai_cells(ai_cells, y, x);
    uroom = roomnums[u.y][u.x];
    mroom = roomnums[y][x];
    for (i = 0; i < 8; i++)
    {
	troom = roomnums[ai_cells[i].y][ai_cells[i].x];
	ai_cells[i].dy = u.y - ai_cells[i].y;
	ai_cells[i].dx = u.x - ai_cells[i].x;
	/* Scoring factors:
	 * If player in a room:
	 *   Square in player's room: +10
	 *   Square closer to player: +1
	 *   Square further from player: -1
	 *   Square in room with linkage 1 to player's room: +5
	 * If player in corridor:
	 *   Square closer to player: +1
	 *   Square matches one of player's coords: +1
	 *   Square further from player: -1
	 * Note that now that wraiths are smart and can walk through
	 * walls, the player can only finally evade a wraith by
	 * killing it or leaving the level; fortunately, wraiths 
	 * are slow. (Danenth will feature ethereal beings, in the
	 * form of chthonic spirits, which are *not* slow.)
	 */
	if (!mon_can_pass(mapmonster[y][x], ai_cells[i].y, ai_cells[i].x))
	{
	    /* Square impassable to this monster. Set score WAY
	     * out of bounds and continue. */
	    ai_cells[i].score = -10000;
	    continue;
	}
	if (uroom != -1)
	{
	    /* Player in a room */
	    if (troom == uroom)
	    {
		ai_cells[i].score += 10;
	    }
	    else if ((troom != -1) && (roomlinkage[troom][uroom] == 1))
	    {
		ai_cells[i].score += 5;
	    }
	    j = ai_cell_compare(ai_cells + i, dy, dx);
	    if (j > 0)
	    {
		ai_cells[i].score -= 1;
	    }
	    else if (j < 0)
	    {
		ai_cells[i].score += 1;
	    }
	    if (ai_cells[i].score > highest_score)
	    {
		highest_score = ai_cells[i].score;
	    }
	}
	else
	{
	    /* Player in a corridor */
	    if ((ai_cells[i].y == u.y) || (ai_cells[i].x == u.x))
	    {
		ai_cells[i].score += 1;
	    }
	    j = ai_cell_compare(ai_cells + i, dy, dx);
	    if (j > 0)
	    {
		ai_cells[i].score -= 1;
	    }
	    else if (j < 0)
	    {
		ai_cells[i].score += 1;
	    }
	    if (ai_cells[i].score > highest_score)
	    {
		highest_score = ai_cells[i].score;
	    }
	}
    }
    if (highest_score == -10000)
    {
	/* No good targets. */
	return;
    }
    for (tryct = 0; tryct < 32; tryct++)
    {
	i = zero_die(8);
	if (ai_cells[i].score == highest_score)
	{
	    *pref_y = ai_cells[i].y;
	    *pref_x = ai_cells[i].x;
	    break;
	}
    }
    return;
}

/* get_naive_prefs()
 *
 * Fills the three-entry preference arrays with three best choices for closing
 * with the player - optimal first, then secondaries in random order as #2 and
 * #3.
 */

static void get_naive_prefs(int y, int x, int dy, int dx, int *pref_y, int *pref_x)
{
    int sy, sx;
    int ady, adx;
    if (dy == 0)
    {
	sy = 0;
	ady = 0;
    }
    else
    {
	sy = dy < 0 ? -1 : 1;
	ady = dy < 0 ? -dy : dy;
    }
    if (dx == 0)
    {
	sx = 0;
	adx = dx < 0 ? -dx : dx;
    }
    else
    {
	sx = dx < 0 ? -1 : 1;
	adx = dx < 0 ? -dx : dx;
    }
    if (!sy)
    {
	/* We're on the horizontal; check the horizontally adjacent
	 * square, then the squares one square north or south in a
	 * random order. */
	pref_y[0] = y;
	if (zero_die(2))
	{
	    pref_y[1] = y - 1;
	    pref_y[2] = y + 1;
	}
	else
	{
	    pref_y[1] = y + 1;
	    pref_y[2] = y - 1;
	}
	pref_x[0] = x + sx;
	pref_x[1] = x + sx;
	pref_x[2] = x + sx;
    }
    else if (!sx)
    {
	pref_x[0] = x;
	if (zero_die(2))
	{
	    pref_x[1] = x - 1;
	    pref_x[2] = x + 1;
	}
	else
	{
	    pref_x[1] = x + 1;
	    pref_x[2] = x - 1;
	}
	pref_y[0] = y + sy;
	pref_y[1] = y + sy;
	pref_y[2] = y + sy;
    }
    else
    {
	pref_x[0] = x + sx;
	pref_y[0] = y + sy;
	if (zero_die(2))
	{
	    pref_x[1] = x;
	    pref_y[1] = y + sy;
	    pref_x[2] = x + sx;
	    pref_y[2] = y;
	}
	else
	{
	    pref_x[2] = x;
	    pref_y[2] = y + sy;
	    pref_x[1] = x + sx;
	    pref_y[1] = y;
	}
    }
}

/* XXX build_ai_cells()
 *
 * Populate array of eight AI cell descriptors.
 */

static void build_ai_cells(struct ai_cell *cells, int y, int x)
{
    cells[0].score = 0;
    cells[1].score = 0;
    cells[2].score = 0;
    cells[3].score = 0;
    cells[4].score = 0;
    cells[5].score = 0;
    cells[6].score = 0;
    cells[7].score = 0;
    cells[0].y = y - 1;
    cells[1].y = y - 1;
    cells[2].y = y - 1;
    cells[3].y = y;
    cells[4].y = y;
    cells[5].y = y + 1;
    cells[6].y = y + 1;
    cells[7].y = y + 1;
    cells[0].x = x - 1;
    cells[1].x = x;
    cells[2].x = x + 1;
    cells[3].x = x - 1;
    cells[4].x = x + 1;
    cells[5].x = x - 1;
    cells[6].x = x;
    cells[7].x = x + 1;
}

/* XXX ai_cell_compare()
 *
 * Find relative range of cell compared to monster's current range.
 */
static int ai_cell_compare(struct ai_cell *cell, int dy, int dx)
{
    /* returns -1 for closer, 0 for same range, +1 for further. */
    int pointrange = convert_range(dy, dx);
    int cellrange = convert_range(cell->dy, cell->dx);
    if (cellrange < pointrange)
    {
	return -1;
    }
    else if (cellrange > pointrange)
    {
	return 1;
    }
    return 0;
}

/* XXX get_dodger_prefs()
 *
 * Get preferences for "smart" monsters without ranged attacks.
 */
static void get_dodger_prefs(int y, int x, int dy, int dx, int *pref_y, int *pref_x)
{
    /* "Dodgers" are smart melee-only monsters. They will try to avoid
     * the cardinals as they close, and will even flow around other
     * monsters to try to get to the player. 
     *
     * This function does *all* the work of selecting a destination square
     * for a smart melee-only monster; accordingly, only pref_y[0] and
     * pref_x[0] get set.
     */
    struct ai_cell ai_cells[8];
    int i;
    int ady, adx;
    int j;
    int highest_score = -10000;
    int tryct;
    *pref_y = y;
    *pref_x = x;
    ady = dy > 0 ? dy : -dy;
    adx = dx > 0 ? dx : -dx;
    build_ai_cells(ai_cells, y, x);
    /* Build the local dx/dy arrays. */
    for (i = 0; i < 8; i++)
    {
	ai_cells[i].dy = u.y - ai_cells[i].y;
	ai_cells[i].dx = u.x - ai_cells[i].x;
	/* Scoring factors:
	 * Square on cardinal: -2.
	 * Square closer to player: +1.
	 * Square further from player: -3.
	 * Square next to player: +10.
	 *
	 * Yes, this monster prizes not opening the range more than
	 * it prizes staying off the cardinal; this is intentional.
	 * It also prizes staying off the cardinal more than actually
	 * closing. When I add more AI state to the monster structure,
	 * this will change.
	 */
	if (!mon_can_pass(mapmonster[y][x], ai_cells[i].y, ai_cells[i].x))
	{
	    /* Square impassable. Set score WAY out of bounds
	     * and continue. */
	    ai_cells[i].score = -10000;
	    continue;
	}
	/* Cardinality */
	if ((ai_cells[i].dy == ai_cells[i].dx) || (ai_cells[i].dy == -ai_cells[i].dx) || (ai_cells[i].dy == 0) || (ai_cells[i].dx == 0))
	{
	    /* Score this square down for being on a cardinal. */
	    ai_cells[i].score -= 2;
	}
	j = ai_cell_compare(ai_cells + i, dy, dx);
	/* Range */
	if ((ai_cells[i].dy < 2) && (ai_cells[i].dy > -2) && (ai_cells[i].dx < 2) && (ai_cells[i].dx > -2))
	{
	    /* Score upward a *lot* for being adjacent to player */
	    ai_cells[i].score += 10;
	}
	else if (j > 0)
	{
	    ai_cells[i].score -= 3;
	}
	else if (j < 0)
	{
	    ai_cells[i].score += 1;
	}
	if (ai_cells[i].score > highest_score)
	{
	    highest_score = ai_cells[i].score;
	}
    }
    if (highest_score == -10000)
    {
	/* No good targets. */
	return;
    }
    for (tryct = 0; tryct < 32; tryct++)
    {
	i = zero_die(8);
	if (ai_cells[i].score == highest_score)
	{
	    *pref_y = ai_cells[i].y;
	    *pref_x = ai_cells[i].x;
	    break;
	}
    }
    return;
}

void select_space(int *py, int *px, int dy, int dx, int selection_mode)
{
    int ai_y[3];
    int ai_x[3];
    int ady, adx;
    int y, x;
    int sy, sx;
    if (dy == 0)
    {
	sy = 0;
	ady = 0;
    }
    else
    {
	sy = dy < 0 ? -1 : 1;
	ady = dy < 0 ? -dy : dy;
    }
    if (dx == 0)
    {
	sx = 0;
	adx = dx < 0 ? -dx : dx;
    }
    else
    {
	sx = dx < 0 ? -1 : 1;
	adx = dx < 0 ? -dx : dx;
    }
    switch (selection_mode)
    {
    case 0:
	/* Simple convergence */
	get_naive_prefs(*py, *px, dy, dx, ai_y, ai_x);
	if (mon_can_pass(mapmonster[*py][*px], ai_y[0], ai_x[0]))
	{
	    y = ai_y[0];
	    x = ai_x[0];
	}
	else if (mon_can_pass(mapmonster[*py][*px], ai_y[1], ai_x[1]))
	{
	    y = ai_y[1];
	    x = ai_x[1];
	}
	else if (mon_can_pass(mapmonster[*py][*px], ai_y[2], ai_x[2]))
	{
	    y = ai_y[2];
	    x = ai_x[2];
	}
	else
	{
	    y = *py;
	    x = *px;
	}
	break;
    case 1:
	/* Converge to cardinal */
	if ((dy == dx) || (dy == -dx) || (!dy) || (!dx))
	{
	    /* On cardinal. Stay there if we can. But close anyway. */
	    x = *px + sx;
	    y = *py + sy;
	    if (mon_can_pass(mapmonster[*py][*px], y, x))
	    {
		break;
	    }
	    x = *px;
	    if (mon_can_pass(mapmonster[*py][*px], y, x))
	    {
		break;
	    }
	    y = *py;
	    x = *px + sx;
	    if (mon_can_pass(mapmonster[*py][*px], y, x))
	    {
		break;
	    }
	}
	else if ((ady == 1) || ((adx > 1) && (ady > adx)))
	{
	    /* One step in ydir off a NSEW cardinal, or further
	     * off cardinal in y than in x */
	    y = *py + sy;
	    x = *px;
	    if (mon_can_pass(mapmonster[*py][*px], y, x))
	    {
		break;
	    }
	    x = *px + sx;
	    if (mon_can_pass(mapmonster[*py][*px], y, x))
	    {
		break;
	    }
	    y = *py;
	    if (mon_can_pass(mapmonster[*py][*px], y, x))
	    {
		break;
	    }
	}
	else if ((adx == 1) || ((ady > 1) && (adx > ady)))
	{
	    /* One step off a diagonal cardinal, with adx > ady */
	    x = *px + sx;
	    y = *py;
	    if (mon_can_pass(mapmonster[*py][*px], y, x))
	    {
		break;
	    }
	    y = *py + sy;
	    if (mon_can_pass(mapmonster[*py][*px], y, x))
	    {
		break;
	    }
	    x = *px;
	    if (mon_can_pass(mapmonster[*py][*px], y, x))
	    {
		break;
	    }
	}
	y = *py;
	x = *px;
	break;
    case 2:
	get_dodger_prefs(*py, *px, dy, dx, ai_y, ai_x);
	y = ai_y[0];
	x = ai_x[0];
	break;
    case 3:
	/* "Drunk" monster i.e. monster moving while it doesn't know
	 * how to find you. */
	get_drunk_prefs(*py, *px, dy, dx, ai_y, ai_x);
	if (mon_can_pass(mapmonster[*py][*px], ai_y[0], ai_x[0]))
	{
	    y = ai_y[0];
	    x = ai_x[0];
	}
	else if (mon_can_pass(mapmonster[*py][*px], ai_y[1], ai_x[1]))
	{
	    y = ai_y[1];
	    x = ai_x[1];
	}
	else if (mon_can_pass(mapmonster[*py][*px], ai_y[2], ai_x[2]))
	{
	    y = ai_y[2];
	    x = ai_x[2];
	}
	else
	{
	    y = *py;
	    x = *px;
	}
	break;
    case 4:
	/* "Seeking" monster i.e. monster moving while it can't see
	 * you, but thinks it knows where you are. This AI isn't
	 * great, but it'll do for now. */
	get_seeking_prefs(*py, *px, dy, dx, ai_y, ai_x);
	y = ai_y[0];
	x = ai_x[0];
	break;
    case 5:
	/* "chase" AI i.e. pursue your last known position. */
	get_chase_prefs(mapmonster[*py][*px], ai_y, ai_x);
	y = ai_y[0];
	x = ai_x[0];
	break;
    }
    *py = y;
    *px = x;
}

void mon_acts(int mon)
{
    struct mon *mptr;
    int dy, dx;
    int y, x;
    int sy, sx;
    int meleerange;
    int oncardinal;
    int special_used = 0;
    mptr = monsters + mon;
    /* dy,dx == direction monster must go to reach you. */
    y = mptr->y;
    x = mptr->x;
    compute_directions(u.y, u.x, y, x, &dy, &dx, &sy, &sx, &meleerange, &oncardinal);
    dy = u.y - mptr->y;
    dx = u.x - mptr->x;
    if ((dy == 0) && (dx == 0))
    {
	print_msg("Program disordered: monster in player's square.\n");
	print_msg("Discarding misplaced monster.\n");
	mptr->used = 0;
	mapmonster[y][x] = -1;
	return;
    }
    if (mapmonster[y][x] != mon)
    {
	print_msg("Program disordered: monster(s) misplaced.\n");
	mptr->used = 0;
	if (mapmonster[y][x] != -1)
	{
	    monsters[mapmonster[y][x]].used = 0;
	    mapmonster[y][x] = -1;
	}
	return;
    }
    if (meleerange)
    {
	/* Adjacent! Attack you.  Demons have a 1 in 10 chance of
	 * attempting to summon another demon instead of attacking
	 * you. */
	if (!mptr->awake)
	{
	    print_mon_name(mon, 2);
	    print_msg(" notices you.\n");
	    mptr->awake = 1;
	}
	if ((mptr->mon_id == PM_DEMON) && !zero_die(10))
	{
	    summon_demon_near(y, x);
	    special_used = 1;
	}
	else if (pmon_is_magician(mptr->mon_id))
	{
	    special_used = use_black_magic(mon);
	}
	if (!special_used)
	{
	    mhitu(mon, DT_PHYS);
	}
    }
    else if (inyourroom(y, x))
    {
	/* In same room. */
	if (!mptr->awake)
	{
	    print_mon_name(mon, 2);
	    print_msg(" notices you.\n");
	    mptr->awake = 1;
	}
	if (pmon_is_magician(mptr->mon_id))
	{
	    /* Two-thirds of the time, try to use black magic. */
	    if (zero_die(6) < 4)
	    {
		special_used = use_black_magic(mon);
	    }
	    if (special_used)
	    {
		return;
	    }
	    /* Didn't, or couldn't, use black magic; converge
	     * as if an archer. */
	    select_space(&y, &x, dy, dx, 1);
	}
	else if (pmon_is_archer(mptr->mon_id))
	{
	    if (oncardinal && (zero_die(6) < 3))
	    {
		special_used = 1;
		mshootu(mon);
	    }
	    if (special_used)
	    {
		return;
	    }
	    select_space(&y, &x, dy, dx, 1);
	}
	else if (pmon_is_smart(mptr->mon_id))
	{
	    select_space(&y, &x, dy, dx, 2);
	}
	else /* pmon_is_stupid() */
	{
	    select_space(&y, &x, dy, dx, 0);
	}
	if ((y != mptr->y) || (x != mptr->x))
	{
	    /* We decided to move; move! */
	    move_mon(mon, y, x);
	}
    }
    else if (!mptr->awake)
    {
	return;
    }
    else
    {
	/* Out of LOS, but awake. Stupid monsters move "drunkenly"; smart
	 * monsters (may) seek you out. */
	if (pmon_is_magician(mptr->mon_id))
	{
	    /* Magicians may have spells that are used when
	     * you are out of sight.  For example, some magicians
	     * may teleport themselves to your vicinity. */
	    special_used = use_black_magic(mon);
	}
	if (special_used)
	{
	    return;
	}
	if (pmon_is_smart(mptr->mon_id))
	{
	    select_space(&y, &x, dy, dx, 4);
	}
	else if (pmon_is_stupid(mptr->mon_id) || (mptr->ai_lasty == -1))
	{
	    select_space(&y, &x, dy, dx, 3);
	}
	else
	{
	    select_space(&y, &x, dy, dx, 5);
	}
	if ((y != mptr->y) || (x != mptr->x))
	{
	    /* We decided to move; move! */
	    move_mon(mon, y, x);
	}
    }
    /* Let's get the data again. */
    compute_directions(u.y, u.x, y, x, &dy, &dx, &sy, &sx, &meleerange, &oncardinal);
    if (meleerange || ((roomnums[u.y][u.x] != -1) && (roomnums[u.y][u.x] == roomnums[y][x])))
    {
	mptr->ai_lasty = u.y;
	mptr->ai_lastx = u.x;
    }
    else if (mptr->ai_lasty != -1)
    {
	mptr->ai_lasty = -1;
	mptr->ai_lastx = -1;
    }
}

/* mon2.c */
//あ
