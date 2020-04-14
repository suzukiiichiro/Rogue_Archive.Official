ROGUE_OBJS = curses.o hit.o init.o inventory.o level.o machdep.o main.o \
    message.o monster.o move.o object.o pack.o play.o random.o ring.o \
    room.o save.o score.o spec_hit.o throw.o trap.o use.o zap.o

CC = cc

# Remember to remove the "-lcurses -ltermlib" if compiling with -DCURSES.
# Put into CFLAGS those UNIX "defines" which apply to your system.  Their
# should be at least two.
# -DCURSES enables a self-contained curses emulation package.  See notes
# in curses.c for more info.

CFLAGS = -c -O3 -w -DUNIX -DCURSES -DALLSCORE
# CFLAGS = -c -Wall -DUNIX -DCURSES

PL	 = rogueclone3_linux
PW	 = rogueclone3_cygwin
PS	 = rogueclone3_src/rogue
PSU	 = rogueclone3_src

P_LNX	 = CHANGES README.txt rogue rogue.6 rogue.doc rogue_jp.doc termcap
P_WIN	 = CHANGES cmd_us.bat cygwin1.dll README.txt rogue.6 rogue.doc \
	   rogue.exe rogue_jp.doc termcap
P_SRCU	 = ../Makefile ../rogue.me
P_SRC	 = CHANGES curses.c hit.c init.c inventory.c level.c machdep.c main.c \
	   Makefile Makefile.org message.c monster.c move.c obj object.c \
	   pack.c pathnames.h play.c random.c README.txt ring.c rogue.6 \
	   rogue.doc rogue.h rogue_jp.doc room.c save.c score.c spec_hit.c \
	   tags termcap throw.c trap.c use.c zap.c

rogue: $(ROGUE_OBJS)
	$(CC) $(ROGUE_OBJS) -o rogue

dist.linux32:
	$(MAKE) CC="cc -m32"

curses.o: curses.c rogue.h
	$(CC) $(CFLAGS) curses.c

hit.o: hit.c rogue.h
	$(CC) $(CFLAGS) hit.c

init.o: init.c rogue.h
	$(CC) $(CFLAGS) init.c

inventory.o: inventory.c rogue.h
	$(CC) $(CFLAGS) inventory.c

level.o: level.c rogue.h
	$(CC) $(CFLAGS) level.c

machdep.o: machdep.c rogue.h
	$(CC) $(CFLAGS) machdep.c

main.o: main.c rogue.h
	$(CC) $(CFLAGS) main.c

message.o: message.c rogue.h
	$(CC) $(CFLAGS) message.c

monster.o: monster.c rogue.h
	$(CC) $(CFLAGS) monster.c

move.o: move.c rogue.h
	$(CC) $(CFLAGS) move.c

object.o: object.c rogue.h
	$(CC) $(CFLAGS) object.c

pack.o: pack.c rogue.h
	$(CC) $(CFLAGS) pack.c

play.o: play.c rogue.h
	$(CC) $(CFLAGS) play.c

random.o: random.c
	$(CC) $(CFLAGS) random.c

ring.o: ring.c rogue.h
	$(CC) $(CFLAGS) ring.c

room.o: room.c rogue.h
	$(CC) $(CFLAGS) room.c

save.o: save.c rogue.h
	$(CC) $(CFLAGS) save.c

score.o: score.c rogue.h
	$(CC) $(CFLAGS) score.c

spec_hit.o: spec_hit.c rogue.h
	$(CC) $(CFLAGS) spec_hit.c

throw.o: throw.c rogue.h
	$(CC) $(CFLAGS) throw.c

trap.o: trap.c rogue.h
	$(CC) $(CFLAGS) trap.c

use.o: use.c rogue.h
	$(CC) $(CFLAGS) use.c

zap.o: zap.c rogue.h
	$(CC) $(CFLAGS) zap.c

dist.src:
	rm -rf $(PL)
	rm -rf $(PW)
	rm -rf $(PS)
	rm -rf $(PSU)
	mkdir $(PL)
	mkdir $(PW)
	mkdir $(PSU)
	mkdir $(PS)
	cp -r $(P_LNX) $(PL)
	cp -r $(P_WIN) $(PW)
	cp -r $(P_SRCU) $(PSU)
	cp -r $(P_SRC) $(PS)
	tar cf $(PL).tar $(PL)
	gzip -f $(PL).tar
	tar cf $(PW).tar $(PW)
	gzip -f $(PW).tar
	tar cf $(PSU).tar $(PSU)
	gzip -f $(PSU).tar
