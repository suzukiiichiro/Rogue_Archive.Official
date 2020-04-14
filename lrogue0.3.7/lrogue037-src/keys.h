/*
 * keys.h
 *
 * Created by
 * Miguel G. C.
 * Ashwin N.
 */

#ifndef _KEYS_H_
#define _KEYS_H_

#if	defined(KEYS_QWERTY)
	#include "keys.qwerty.h"
#elif defined(KEYS_DVORAK)
	#include "keys.dvorak.h"
#endif

#endif	/* _KEYS_H_ */
