/*
 * init.h
 *
 * Created by Ashwin N.
 */

#ifndef _INIT_H_
#define _INIT_H_

int init(int argc, char *argv[]);

void clean_up(char *estr);

void byebye(int sig);

void onintr(int sig);

void error_save(int sig);

#endif	/* _INIT_H_ */
