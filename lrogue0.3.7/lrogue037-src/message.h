/*
 * message.h
 *
 * Created by Ashwin N.
 */

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

void message(char *msg, boolean intrpt);

void remessage(void);

void check_message(void);

short get_input_line(char *prompt, char *buf, char *insert,
                     char *if_cancelled, boolean add_blank,
                     boolean do_echo);

int rgetchar(void);

void print_stats(int stat_mask);

void sound_bell(void);

boolean is_digit(short ch);

int r_index(char *str, int ch, boolean last);

#endif	/* _MESSAGE_H_ */
