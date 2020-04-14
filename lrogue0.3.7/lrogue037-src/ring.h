/*
 * ring.h
 *
 * Created by Ashwin N.
 */

#ifndef _RING_H_
#define _RING_H_

void put_on_ring(void);

void do_put_on(object *ring, boolean on_left);

void remove_ring(void);

void un_put_on(object *ring);

void gr_ring(object *ring, boolean assign_wk);

void inv_rings(void);

void wiz_ring_info(void);

void ring_stats(boolean pr);

#endif	/* _RING_H_ */
