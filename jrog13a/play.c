/*
 * play.c
 *
 * This source herein may be modified and/or distributed by anybody who
 * so desires, with the following restrictions:
 *    1.)  No portion of this notice shall be removed.
 *    2.)  Credit shall not be taken for the creation of this source.
 *    3.)  This code is not to be traded, sold, or used for personal
 *         gain or profit.
 *
 */

#include <stdio.h>
#include "rogue.h"

#define	CTRL(c)	((c) & 037)

boolean interrupted = 0;
#ifdef JAPAN
char *unknown_command = "�R�}���h���Ⴂ�܂��B";
#else
char *unknown_command = "Unknown command";
#endif

extern short party_room, bear_trap;
extern char hit_message[];
extern boolean wizard, trap_door;

play_level()
{
	short ch, cmd, oldcmd;
	int count;

	cmd = '.';
	for (;;) {
		interrupted = 0;
		if (hit_message[0]) {
			message(hit_message, 1);
			hit_message[0] = 0;
		}
		if (trap_door) {
			trap_door = 0;
			return;
		}
		move(rogue.row, rogue.col);
		refresh();

		oldcmd = cmd;
		ch = cmd = rgetchar();
		check_message();
		count = 0;
CH:
		switch(ch) {
		case '.':
			rest((count > 0) ? count : 1);
			break;
		case 's':
			search(((count > 0) ? count : 1), 0);
			break;
		case 'i':
			inventory(&rogue.pack, ALL_OBJECTS);
			break;
		case 'f':
			fight(0);
			break;
		case 'F':
			fight(1);
			break;
		case 'h':
		case 'j':
		case 'k':
		case 'l':
		case 'y':
		case 'u':
		case 'n':
		case 'b':
			(void) one_move_rogue(ch, 1);
			break;
		case 'H':
		case 'J':
		case 'K':
		case 'L':
		case 'B':
		case 'Y':
		case 'U':
		case 'N':
		case CTRL('H'):
		case CTRL('J'):
		case CTRL('K'):
		case CTRL('L'):
		case CTRL('Y'):
		case CTRL('U'):
		case CTRL('N'):
		case CTRL('B'):
			multiple_move_rogue(ch);
			break;
		case 'e':
			eat();
			break;
		case 'q':
			quaff();
			break;
		case 'r':
			read_scroll();
			break;
		case 'm':
			move_onto();
			break;
		case 'd':
			drop();
			break;
		case 'P':
			put_on_ring();
			break;
		case 'R':
			remove_ring();
			break;
		case CTRL('P'):
			remessage();
			break;
		case CTRL('W'):
			wizardize();
			break;
		case '>':
			if (drop_check()) {
				return;
			}
			break;
		case '<':
			if (check_up()) {
				return;
			}
			break;
		case ')':
		case ']':
			inv_armor_weapon(ch == ')');
			break;
		case '=':
			inv_rings();
			break;
		case '^':
			id_trap();
			break;
		case 'I':
			single_inv(0);
			break;
		case 'T':
			take_off();
			break;
		case 'W':
			wear();
			break;
		case 'w':
			wield();
			break;
		case 'c':
			call_it();
			break;
		case 'z':
			zapp();
			break;
		case 't':
			throw();
			break;
		case 'v':
message("Rogue-clone: Version II. (Tim Stoehr was here), tektronix!zeus!tims ", 0);
#ifndef ORIGINAL
message("Japanese edition: Ver.1.3a (enhanced by ohta@src.ricoh.co.jp)", 0);
#endif
			break;
		case 'Q':
			quit(0);
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			move(rogue.row, rogue.col);
			refresh();
			do {
				if (count < 100) {
					count = (10 * count) + (ch - '0');
				}
				ch = rgetchar();
			} while (is_digit(ch));
			if (ch != CANCEL) {
				goto CH;
			}
			break;
		case ' ':
			break;
		case CTRL('I'):
			if (wizard) {
				inventory(&level_objects, ALL_OBJECTS);
			} else {
				message(unknown_command, 0);
			}
			break;
		case CTRL('S'):
			if (wizard) {
				draw_magic_map();
			} else {
				message(unknown_command, 0);
			}
			break;
		case CTRL('T'):
			if (wizard) {
				show_traps();
			} else {
				message(unknown_command, 0);
			}
			break;
		case CTRL('O'):
			if (wizard) {
				show_objects();
			} else {
				message(unknown_command, 0);
			}
			break;
		case CTRL('A'):
			show_average_hp();
			break;
#ifndef ORIGINAL
		case CTRL('G'):
#endif
		case CTRL('C'):
			if (wizard) {
				new_object_for_wizard();
			} else {
				message(unknown_command, 0);
			}
			break;
		case CTRL('M'):
			if (wizard) {
				show_monsters();
			} else {
				message(unknown_command, 0);
			}
			break;
		case 'S':
			save_game();
			break;
		case ',':
			kick_into_pack();
			break;
#ifndef ORIGINAL
		case CTRL('X'):
			if (!wizard) {
				message(unknown_command, 0);
				break;
			}
			draw_magic_map();
			show_monsters();
			show_objects();
			show_traps();
			break;
		case '?':
			help();
			break;
		case '@':
			print_stats(STAT_ALL);
			break;
		case 'D':
			discovered();
			break;
		case '/':
			identify();
			break;
		case 'o':
			options();
			break;
		case '!':
			doshell();
			break;
		case 'a':
			ch = cmd = oldcmd;
			goto CH;
#endif /* ORIGINAL */
		default:
			message(unknown_command, 0);
			break;
		}
	}
}

#ifndef ORIGINAL
char *help_message[] = {
#ifdef JAPAN
"?		�w���v��\������		>   �K�i���~���",
"/		��ʂ̕������\�����̂�\��	<   �K�i��o��",
"h, j, k, l	���A���A��A�E�ɐi��		.   ���̏�ŋx��",
"y, u, b, n	����A�E��A�����A�E���ɐi��	,   �����̂��̂��E��",
"<SHIFT><���p>	�����ɂԂ���܂Ői��		s   ��Ȃ�B���Ƃт��T��",
"<CTRL><���p>	������������܂Ői��		i   �������̂�����",
"f<���p> 	���ꂻ���ɂȂ�܂Ő키	I   �P�̎������̂�����",
"F<���p> 	�ǂ��炩���|���܂Ő키	q   ���������",
"t<���p> 	���𓊂���			r   ��������ǂ�",
"m<���p> 	�����Ă�����̂̏�ɏ��	e   �H�ו���H�ׂ�",
"z<���p> 	����U�肩����		w   �������Ɏ���",
"^<���p> 	��Ȃ̎�ނ𒲂ׂ�		W   ��낢�𒅂�",
"<CTRL>P 	�Ō�̃��b�Z�[�W���ĕ\������	T   ��낢��E��",
"<CTRL>R 	��ʂ�`������			P   �w�ւ��͂߂�",
"<CTRL>D 	��ʂ��t�@�C���ɏ����o��	R   �w�ւ��͂���",
"<ESC>		�R�}���h�𒆎~����		d   �������̂𗎂Ƃ�",
"a		���O�̃R�}���h���J��Ԃ�	c   �������̂ɌĂі���t����",
"D		���܂ŉ��𔭌���������\��	)   ���݂̕��������",
"S		�Q�[�����Z�[�u���ďI������	]   ���݂̂�낢������",
"Q		�Q�[���𒆎~����		=   ���݂̎w�ւ�����",
"v		�o�[�W��������\������	@   �v���C���[�s��`������",
"!		�V�F���R�}���h�����s����	o   �I�v�V�����̕\���E�ݒ�",
"",
"���X�y�[�X�������Ă���������",
#else /*JAPAN*/
"?		print help			>   go down a staircase",
"/		identify object			<   go up a staircase",
"h, j, k, l	left, down, up, left		.   rest for a turn",
"y, u, b, n	move diagonally			,   pick something up",
"<SHIFT><dir>	run that way			s   search trap/secret door",
"<CTRL><dir>	run till adjacent		i   inventory",
"f<dir>		fight till death or near death	I   inventory single item",
"F<dir>		fight till either of you dies	q   quaff potion",
"t<dir>		throw something			r   read scroll",
"m<dir>		move onto without picking up	e   eat food",
"z<dir>		zap a wand in a direction	w   wield a weapon",
"^<dir>		identify trap type		W   wear armor",
"<CTRL>P 	repeat last message		T   take armor off",
"<CTRL>R 	redraw screen			P   put on ring",
"<CTRL>D 	save screen to file		R   remove ring",
"<ESC>		cancel command			d   drop object",
"a		repeat last command		c   call object",
"D		recall what's been discovered	)   print current weapon",
"S		save game			]   print current armor",
"Q		quit				=   print current rings",
"v		print version information	@   print current stats",
"!		shell escape			o   examine/set options",
"",
"--Press space to continue--",
#endif /*JAPAN*/
(char *) 0
};

help()
{
	register int row, col;
	int n;

	for (row = 0; row < DROWS; row++)
		for (col = 0; col < DCOLS; col++)
			descs[row][col] = mvinch(row, col);
	clear();
	for (n = 0; help_message[n]; n++) {
		mvaddstr(n, 0, help_message[n]);
		clrtoeol();
	}
	refresh();
	wait_for_ack();

	for (row = 0; row < DROWS; row++) {
#ifdef JAPAN
		touch(row, 0, DCOLS-1);
#endif
		move(row, 0);
		for (col = 0; col < DCOLS; col++) {
			if (row == DROWS - 1 && col == DCOLS - 1)
				continue;
			if (row < MIN_ROW || row >= DROWS - 1)
				addch(descs[row][col]);
			else
				addch(colored(descs[row][col]));
		}
	}
	refresh();
}

extern char *m_names[];
static char *o_names[] = {
#ifdef JAPAN
	"�v���C���[", "�����̏�", "�����̕�", "�����̕�", "�����̃h�A",
	"�����ƕ������ނ��ԒʘH", "�ׂ̊K�ւ̊K�i", "�B���ꂽ���", "����",
	"�H��", "��낢�����т�", "����", "������", "����", "���@�̏�",
	"���@�̎w��", "�C�F���_�[�̖�����"
#else
	"You", "floor of a room", "wall of a room", "wall of a room", "door",
	"passage", "staircase", "secret trap", "a pile or pot of gold",
	"food ration", "an armor", "a weapon", "a scroll", "a potion",
	"a wand or a staff", "a ring", "the Amulet of Yendor"
#endif
};

identify()
{
	short ch, n;
	char *p, buf[80];

#ifdef JAPAN
	message("�ǂ�ȕ����ł����H", 0);
#else
	message("Which character?", 0);
#endif

again:
	ch = rgetchar();
	if (ch == '\033') {
		check_message();
		return;
	} else if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {
		check_message();
		if (ch >= 'a' && ch <= 'z')
			ch += 'A' - 'a';
		p = m_names[ch - 'A'];
	} else if ((n = r_index("@.|-+#%^*:])?!/=,", ch, 0)) != -1) {
		check_message();
		p = o_names[n];
	} else {
		sound_bell();
		goto again;
	}
	sprintf(buf, "'%c': %s", ch, p);
	message(buf, 0);
}

options()
{
	register int row, col;
	int n, i, j, changed;
	short ch;
	short pos[DROWS];
	boolean bbuf[DROWS];
	char cbuf[DROWS][MAX_TITLE_LENGTH];
	char optbuf[BUFSIZ];

	for (row = 0; row < DROWS; row++)
		for (col = 0; col < DCOLS; col++)
			descs[row][col] = mvinch(row, col);
	clear();
	for (n = 0; envopt[n].name; n++) {
		mvaddstr(n, 0, optdesc[n]);
		addstr(" (\"");
		addstr(envopt[n].name);
		addstr("\"): ");
		if (envopt[n].bp) {
			bbuf[n] = *(envopt[n].bp);
			addstr(bbuf[n]? "Yes": "No");
		} else {
			strcpy(cbuf[n], *(envopt[n].cp));
			if (envopt[n].ab) {
				i = strlen(cbuf[n]);
				cbuf[n][i-1] = 0;
			}
			addstr(cbuf[n]);
		}
		pos[n] = strlen(optdesc[n]) + strlen(envopt[n].name) + 7;
	}

	i = 0;
	while (i >= 0 && i < n) {
		if (envopt[i].bp)
			move(i, pos[i]);
		else
			move(i, pos[i]+strlen(cbuf[i]));
		refresh();
		ch = rgetchar();
		if (ch == CANCEL)
			break;
		if (r_index("-\r\n", ch, 0) > -1) {
			if (envopt[i].bp) {
				addstr(bbuf[i]? "Yes": "No");
				clrtoeol();
			}
			if (ch == '-')
				i--;
			else
				i++;
			continue;
		}
		if (envopt[i].bp) {
			if (ch >= 'A' && ch <= 'Z')
				ch += 'a'- 'Z';
			if (ch != 'y' && ch != 'n') {
				mvaddstr(i, pos[i], "(Yes or No)");
				continue;
			}
			addstr((ch == 'y')? "Yes": "No");
			clrtoeol();
			bbuf[i] = (ch == 'y');
			i++;
		} else {
			j = input_line(i, pos[i], cbuf[i], optbuf, ch);
			if (j < 0)
				break;
			strcpy(cbuf[i], optbuf);
			i++;
		}
	}
	changed = (i < 0 || i >= n);
	if (changed) {
		move(n+1, 0);
#ifdef JAPAN
		addstr("���X�y�[�X�������Ă���������");
#else
		addstr("--Press space to continue--");
#endif
		refresh();
		wait_for_ack();
	}

#ifdef JAPAN
	for (row = 0; row < n; row++)
		touch(row, 0, DCOLS-1);
#endif
	for (row = 0; row < DROWS; row++) {
		move(row, 0);
		for (col = 0; col < DCOLS; col++) {
			if (row == DROWS - 1 && col == DCOLS - 1)
				continue;
			if (row < MIN_ROW || row >= DROWS - 1)
				addch(descs[row][col]);
			else
				addch(colored(descs[row][col]));
		}
	}

	if (changed) {
		optbuf[0] = 0;
		for (i = 0; i < n; i++) {
			strcat(optbuf, ",");
			if (envopt[i].bp) {
				if (!bbuf[i])
					strcat(optbuf, "no");
				strcat(optbuf, envopt[i].name);
			} else {
				strcat(optbuf, envopt[i].name);
				strcat(optbuf, ":");
				strcat(optbuf, cbuf[i]);
			}
		}
		set_opts(optbuf);
	}
	refresh();
}

#ifdef MSDOS
#ifndef LC4
#include <process.h>
#endif
#endif

doshell()
{
	char *cmd, *md_getenv();

#ifdef MSDOS
	if ((cmd = md_getenv("COMSPEC")) == NULL) {
#ifdef JAPAN
		message("���ϐ� COMSPEC ���Z�b�g����Ă��܂���", 0);
#else
		message("Sorry, no COMSPEC...", 0);
#endif
		return;
	}
#endif /*MSDOS*/
#ifdef UNIX
	if ((cmd = md_getenv("SHELL")) == NULL)
		cmd = "/bin/sh";
#endif /*UNIX*/
	move(DROWS-1, 0);
	refresh();
	stop_window();
	if (*org_dir)
		md_chdir(org_dir);
#ifdef MSDOS
	putstr("\r\n\r\n");
#ifdef JAPAN
	putstr("[ \"exit\" �R�}���h�Ń��[�O�ɖ߂�܂� ]");
#else
	putstr("[ Type \"exit\" to return to rogue. ]");
#endif
	putstr("\r\n");
#ifdef LC4
	forkl(cmd, cmd, NULL);
#else
	spawnl(P_WAIT, cmd, cmd, NULL);
#endif
#endif /*MSDOS*/
#ifdef UNIX
	md_reset_signals();
#ifdef JAPAN
	putstr("[ �V�F�����I������ƃ��[�O�ɖ߂�܂� ]");
#else
	putstr("[ Exit shell to return to rogue. ]");
#endif
	putstr("\r\n");
	system(cmd);
	md_heed_signals();
#endif /*UNIX*/
	if (game_dir && *game_dir)
		md_chdir(game_dir);
	start_window();
	wrefresh(curscr);
}

#endif /* ORIGINAL */
