/* pmon2.c
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

#define PMON2_C
#include "dunbash.h"
#include "monsters.h"

int pmon_resists_fire(int pm)
{
	return !!(permons[pm].flags & PMF_RESIST_FIRE);
}

int pmon_resists_cold(int pm)
{
	return !!(permons[pm].flags & PMF_RESIST_COLD);
}

int pmon_is_undead(int pm)
{
	return !!(permons[pm].flags & PMF_UNDEAD);
}

int pmon_is_stupid(int pm)
{
	return !!(permons[pm].flags & PMF_STUPID);
}

int pmon_is_smart(int pm)
{
	return !!(permons[pm].flags & PMF_SMART);
}

int pmon_is_magician(int pm)
{
	return !!(permons[pm].flags & PMF_MAGICIAN);
}

int pmon_is_archer(int pm)
{
	return !!(permons[pm].flags & PMF_ARCHER);
}

/* pmon2.c */
