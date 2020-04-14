/* vector.c
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

void compute_directions(int y1, int x1, int y2, int x2, int *pdy, int *pdx, int *psy, int *psx, int *pmelee, int *pcardinal)
{
	int dy, dx, sy, sx;
	dy = y1 - y2;
	dx = x1 - x2;
	sy = dy > 0 ? 1 : (dy < 0 ? -1 : 0);
	sx = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
	if (pdy)
	{
		*pdy = dy;
	}
	if (pdx)
	{
		*pdx = dx;
	}
	if (psy)
	{
		*psy = sy;
	}
	if (psx)
	{
		*psx = sx;
	}
	if (pmelee)
	{
		*pmelee = (dy < 2) && (dy > -2) && (dx < 2) && (dx > -2);
	}
	if (pcardinal)
	{
		*pcardinal = (dy == dx) || (dy == -dx) || (dx == 0) || (dy == 0);
	}
}

/* vector.c */
// あ
