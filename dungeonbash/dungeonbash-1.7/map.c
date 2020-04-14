/* map.c
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

#include "dunbash.h"
#include "monsters.h"

#include <string.h>
int mapobject[DUN_HEIGHT][DUN_WIDTH];
int mapmonster[DUN_HEIGHT][DUN_WIDTH];
enum terrain_num terrain[DUN_HEIGHT][DUN_WIDTH];
int mapflags[DUN_HEIGHT][DUN_WIDTH];
int roomnums[DUN_HEIGHT][DUN_WIDTH];
int depth = 1;
int roomlinkage[MAX_ROOMS][MAX_ROOMS];
int roombounds[MAX_ROOMS][4];
int stairs_room = -1;
int zoo_room = -1;
static int segsused[MAX_ROOMS];

static int get_levgen_mon_floor(int *y, int *x);
static void add_random_room(int yseg, int xseg);
static void link_rooms(int r1, int r2);
static void put_stairs(void);
static void generate_zoo(void);

void room_reset(void)
{
    int i;
    memset((void *) &(roombounds[0][0]), 0, sizeof roombounds);
    memset((void *) &(roomlinkage[0][0]), 0, sizeof roomlinkage);
    memset((void *) segsused, 0, sizeof segsused);
    zoo_room = -1;
    stairs_room = -1;
    for (i = 0; i < MAX_ROOMS; i++)
    {
	roomlinkage[i][i] = 3;
    }
    memset(mapobject, -1, sizeof mapobject);
    memset(mapmonster, -1, sizeof mapmonster);
    memset(terrain, 0, sizeof terrain);
    memset(mapflags, 0, sizeof mapflags);
    memset(roomnums, -1, sizeof roomnums);
}

static void add_random_room(int yseg, int xseg)
{
    int roomidx = (yseg * 3) + xseg;
    int ycen, xcen;
    int y1, y2, x1, x2;
    int y, x;
    ycen = (DUN_HEIGHT - 2) / 6 + yseg * ((DUN_HEIGHT - 2) / 3);
    xcen = (DUN_WIDTH - 2) / 6 + xseg * ((DUN_WIDTH - 2) / 3);
    y1 = ycen - one_die(ROOM_HT_DELTA) - 1;
    x1 = xcen - one_die(ROOM_WD_DELTA) - 1;
    y2 = ycen + one_die(ROOM_HT_DELTA) + 1;
    x2 = xcen + one_die(ROOM_WD_DELTA) + 1;
    for (y = y1 + 1; y < y2; y++)
    {
	for (x = x1 + 1; x < x2; x++)
	{
	    terrain[y][x] = FLOOR;
	    roomnums[y][x] = roomidx;
	}
    }
    for (y = y1; y <= y2; y++)
    {
	roomnums[y][x1] = roomidx;
	roomnums[y][x2] = roomidx;
    }
    for (x = x1; x <= x2; x++)
    {
	roomnums[y1][x] = roomidx;
	roomnums[y2][x] = roomidx;
    }
    roombounds[roomidx][0] = y1;
    roombounds[roomidx][1] = y2;
    roombounds[roomidx][2] = x1;
    roombounds[roomidx][3] = x2;
    segsused[yseg * 3 + xseg] = 1;
}

static void link_rooms(int r1, int r2)
{
    int i;
    int y, x;
    int y1, y2, y3, y4;
    int x1, x2, x3, x4;
    /* First rule: CORRIDORS ARE STRAIGHT! This makes the AI easier. */
    /* Second rule: Don't pass in bogus numbers to this function; it
     * has no error-checking of its own. */
    /* Update the linkage matrix. */
    roomlinkage[r1][r2] = 1;
    roomlinkage[r2][r1] = 1;
    for (i = 0; i < MAX_ROOMS; i++)
    {
	if ((i == r1) || (i == r2))
	{
	    continue;
	}
	if ((roomlinkage[r1][i] > 0) && !roomlinkage[r2][i])
	{
	    roomlinkage[r2][i] = 2;
	    roomlinkage[i][r2] = 2;
	}
	if ((roomlinkage[r2][i] > 0) && !roomlinkage[r1][i])
	{
	    roomlinkage[r1][i] = 2;
	    roomlinkage[i][r1] = 2;
	}
    }
    y1 = roombounds[r1][0];
    y2 = roombounds[r2][0];
    y3 = roombounds[r1][1];
    y4 = roombounds[r2][1];
    x1 = roombounds[r1][2];
    x2 = roombounds[r2][2];
    x3 = roombounds[r1][3];
    x4 = roombounds[r2][3];
    /* Now generate the corridor. */
    if ((r1 % 3) == (r2 % 3))
    {
	/* same xseg; north-south linkage */
	if (x4 < x3)
	{
	    x3 = x4;
	}
	if (x2 > x1)
	{
	    x1 = x2;
	}
	x = exclusive_flat(x1, x3);
	if (y3 < y2)
	{
	    /* go south from r1 */
	    terrain[y3][x] = DOOR;
	    terrain[y2][x] = DOOR;
	    for (y = y3 + 1; y < y2; y++)
	    {
		terrain[y][x] = FLOOR;
	    }
	}
	else if (y4 < y1)
	{
	    /* go south from r2 */
	    terrain[y4][x] = DOOR;
	    terrain[y1][x] = DOOR;
	    for (y = y4 + 1; y < y1; y++)
	    {
		terrain[y][x] = FLOOR;
	    }
	}
    }
    else
    {
	/* same yseg; east-west linkage */
	if (y4 < y3)
	{
	    y3 = y4;
	}
	if (y2 > y1)
	{
	    y1 = y2;
	}
	y = exclusive_flat(y1, y3);
	if (x3 < x2)
	{
	    /* go south from r1 */
	    terrain[y][x3] = DOOR;
	    terrain[y][x2] = DOOR;
	    for (x = x3 + 1; x < x2; x++)
	    {
		terrain[y][x] = FLOOR;
	    }
	}
	else if (x4 < x1)
	{
	    /* go south from r2 */
	    terrain[y][x4] = DOOR;
	    terrain[y][x1] = DOOR;
	    for (x = x4 + 1; x < x1; x++)
	    {
		terrain[y][x] = FLOOR;
	    }
	}
    }
}

void leave_level(void)
{
    int i;
    memset(mapobject, -1, sizeof mapobject);
    memset(mapmonster, -1, sizeof mapmonster);
    memset(terrain, 0, sizeof terrain);
    memset(mapflags, 0, sizeof mapflags);
    memset(roomnums, -1, sizeof roomnums);
    for (i = 0; i < 100; i++)
    {
	/* Throw away each monster */
	monsters[i].used = 0;
	/* and each object not carried by the player */
	if (!objects[i].with_you)
	{
	    objects[i].used = 0;
	}
    }
    depth++;
    status_updated = 1;
    map_updated = 1;
}

void make_new_level(void)
{
    room_reset();
    build_level();
    populate_level();
    inject_player();
    touch_back_buffer();
    display_update();
}

void put_stairs(void)
{
    int y;
    int x;
    stairs_room = zero_die(MAX_ROOMS);
    y = exclusive_flat(roombounds[stairs_room][0], roombounds[stairs_room][1]);
    x = exclusive_flat(roombounds[stairs_room][2], roombounds[stairs_room][3]);
    terrain[y][x] = STAIRS;
}

int edge_rooms[4] = { 1, 3, 5, 7 };
int corners[4][2] = { { 0, 2 }, { 0, 6 }, { 2, 8 }, { 6, 8 } };
void build_level(void)
{
    int i;
    /* Snapshot the running RNG state, so that we can rebuild the map from
     * the saved RNG state at game reload. */
    memcpy(saved_state, rng_state, sizeof saved_state);
    /* Add rooms */
    for (i = 0; i < MAX_ROOMS; i++)
    {
	add_random_room(i / 3, i % 3);
    }
    /* Add corridors */
    /* Link the centre room to an edge room. */
    link_rooms(4, edge_rooms[zero_die(4)]);
    /* And to another; if we're already linked, don't bother. */
    i = zero_die(4);
    if (roomlinkage[4][edge_rooms[i]] == 0)
    {
	link_rooms(4, edge_rooms[i]);
    }
    /* Link each edge room to one of its corner rooms. */
    for (i = 0; i < 4; i++)
    {
	link_rooms(edge_rooms[i], corners[i][zero_die(2)]);
    }
    /* At this point, 1-2 edge rooms and their attached corner rooms
     * have linkage to the centre. */
    /* Link each edge room to its unlinked corner if it is not 2-linked
     * to the centre. */
    for (i = 0; i < 4; i++)
    {
	if (!roomlinkage[4][edge_rooms[i]])
	{
	    if (roomlinkage[edge_rooms[i]][corners[i][0]])
	    {
		link_rooms(edge_rooms[i], corners[i][1]);
	    }
	    else
	    {
		link_rooms(edge_rooms[i], corners[i][0]);
	    }
	}

    }
    /* Link each corner room to its unlinked edge if that edge is not
     * 2-linked to the centre.  If we still haven't got centre
     * connectivity for the edge room, connect the edge to the centre. */
    for (i = 0; i < 4; i++)
    {
	if (!roomlinkage[4][edge_rooms[i]])
	{
	    if (!roomlinkage[edge_rooms[i]][corners[i][0]])
	    {
		link_rooms(edge_rooms[i], corners[i][0]);
	    }
	    if (!roomlinkage[edge_rooms[i]][corners[i][1]])
	    {
		link_rooms(edge_rooms[i], corners[i][1]);
	    }
	}
	if (!roomlinkage[4][edge_rooms[i]])
	{
	    link_rooms(edge_rooms[i], 4);
	}
    }
    /* Just for safety's sake: Now we know all edges are attached,
     * make sure all the corners are. (Previously, it was possible
     * for them not to be. I know, because I met such a level :) */
    for (i = 3; i > -1; i--)
    {
	if (!roomlinkage[4][corners[i][0]])
	{
	    link_rooms(edge_rooms[i], corners[i][0]);
	}
	if (!roomlinkage[4][corners[i][1]])
	{
	    link_rooms(edge_rooms[i], corners[i][1]);
	}
    }
    /* Add the stairs */
    put_stairs();
}

static void generate_zoo(void)
{
    int mons;
    int items;
    int tries;
    int index;
    int y, x;
    zoo_room = zero_die(MAX_ROOMS);
    if (zoo_room == stairs_room)
    {
	zoo_room = -1;
	return;
    }
    /* A treasure zoo should get nine monsters and nine items. */
    for (mons = 0; mons < 9; mons++)
    {
	for (tries = 0; tries < 200; tries++)
	{
	    y = exclusive_flat(roombounds[zoo_room][0], roombounds[zoo_room][1]);
	    x = exclusive_flat(roombounds[zoo_room][2], roombounds[zoo_room][3]);
	    if (mapmonster[y][x] == -1)
	    {
		index = create_mon(-1, y, x);
		if (index != -1)
		{
		    break;
		}
	    }
	}
    }
    for (items = 0; items < 9; items++)
    {
	for (tries = 0; tries < 200; tries++)
	{
	    y = exclusive_flat(roombounds[zoo_room][0], roombounds[zoo_room][1]);
	    x = exclusive_flat(roombounds[zoo_room][2], roombounds[zoo_room][3]);
	    if (mapobject[y][x] == -1)
	    {
		index = create_obj(-1, 1, 0, y, x);
		if (index != -1)
		{
		    break;
		}
	    }
	}
    }
}


int get_room_y(int room)
{
    return exclusive_flat(roombounds[room][0], roombounds[room][1]);
}

int get_room_x(int room)
{
    return exclusive_flat(roombounds[room][2], roombounds[room][3]);
}

int get_levgen_mon_floor(int *y, int *x)
{
    /* Get a vacant floor cell that isn't in the treasure zoo. */
    int room_try;
    int cell_try;
    int ty, tx;
ty=0;
    int room;
    for (room_try = 0; room_try < (MAX_ROOMS * 2); room_try++)
    {
	room = zero_die(MAX_ROOMS);
	if (room == zoo_room)
	{
	    continue;
	}
	for (cell_try = 0; cell_try < 200; cell_try++)
	{
	    ty = get_room_y(room);
	    tx = get_room_x(room);
	    if ((terrain[ty][tx] != FLOOR) ||
		(mapmonster[ty][tx] != -1))
	    {
		ty = -1;
		tx = -1;
		continue;
	    }
	    break;
	}
	break;
    }
    if (ty == -1)
    {
	return -1;
    }
    *y = ty;
    *x = tx;
    return 0;
}

void populate_level(void)
{
    int i;
    int j;
    int y, x;
    int ic;
    /* Check for a "treasure zoo" */
    if (!zero_die(10) && (depth > 2))
    {
	generate_zoo();
    }
    /* Generate some random monsters */
    for (i = 0; i < 10; i++)
    {
	j = get_levgen_mon_floor(&y, &x);
	if (j == -1)
	{
	    continue;
	}
	create_mon(-1, y, x);
    }
    ic = 3 + depth;
    if (ic > 40)
    {
	/* Never create more than 40 items. */
	ic = 40;
    }
    /* Generate some random treasure */
    for (i = 0; i < ic; i++)
    {
	j = get_levgen_mon_floor(&y, &x);
	if (j == -1)
	{
	    continue;
	}
	create_obj(-1, 1, 0, y, x);
    }
}

void inject_player(void)
{
    int i;
    int room_try;
    int cell_try;
    for (room_try = 0; room_try < (MAX_ROOMS * 2); room_try++)
    {
	i = zero_die(MAX_ROOMS);
	if (i == zoo_room)
	{
	    continue;
	}
	if (i == stairs_room)
	{
	    continue;
	}
	for (cell_try = 0; cell_try < 200; cell_try++)
	{
	    u.y = exclusive_flat(roombounds[i][0], roombounds[i][1]);
	    u.x = exclusive_flat(roombounds[i][2], roombounds[i][3]);
	    if (mapmonster[u.y][u.x] != -1)
	    {
		continue;
	    }
	    break;
	}
	break;
    }
    reloc_player(u.y, u.x);
}

/* map.c */
//あ
