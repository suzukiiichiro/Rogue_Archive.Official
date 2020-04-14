/* rng.c
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
#include <time.h>
#include <unistd.h>

/* FIX mpr v1.1 2005.02.06: Changed type of RNG stuff to "unsigned int" to
 * prevent malformed behaviour on x86-64 (where long int is 64 bits). */
unsigned int rng_state[5];
unsigned int saved_state[5];

unsigned int rng(void)
{
	unsigned int tmp;
	tmp = rng_state[0] ^ (rng_state[0] >> 7);
	rng_state[0] = rng_state[1];
	rng_state[1] = rng_state[2];
	rng_state[2] = rng_state[3];
	rng_state[3] = rng_state[4];
	rng_state[4] = (rng_state[4] ^ (rng_state[4] << 6)) ^ (tmp ^ (tmp << 13));
	return (rng_state[2] + rng_state[2] + 1) * rng_state[4];
}

void rng_init(void)
{
	int i;
	/* To make manipulating the RNG by monitoring the system time
	 * harder, we use PID and UID to perturb the return value of time()
	 * used to initialise the libc RNG.
	 *
	 * Yes, I am aware that the libc RNG on many platforms is a steaming
	 * pile of shite. However, I need *something* with which to populate
	 * my RNG's state array.
	 */
	srand(time(NULL) ^ getpid() ^ (getuid() << 16));
	rng_state[0] = rand();
	rng_state[1] = rand();
	rng_state[2] = rand();
	rng_state[3] = rand();
	rng_state[4] = rand();
	/* Flush through the first 100 numbers just in case some bastard
	 * tries to run us with a 16-bit rand(). */
	for (i = 0; i < 100; i++)
	{
		rng();
	}
}

/* rng.c */
