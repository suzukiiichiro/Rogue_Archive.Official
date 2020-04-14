/*
 * machdep.h
 *
 * Created by Ashwin N.
 */

#ifndef _MACHDEP_H_
#define _MACHDEP_H_

void md_slurp(void);

void md_control_keyboard(int mode);

void md_heed_signals(void);

void md_ignore_signals(void);

int md_get_file_id(char *fname);

int md_link_count(char *fname);

void md_gct(struct rogue_time *rt_buf);

void md_gfmt(char *fname, struct rogue_time *rt_buf);

boolean md_df(char *fname);

char * md_gln(void);

void md_sleep(int nsecs);

char * md_malloc(int n);

int md_gseed(void);

void md_exit(int status);

#endif	/* _MACHDEP_H_ */
