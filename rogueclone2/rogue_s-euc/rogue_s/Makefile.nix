#########################################################
#							#
#	Rogue-clone Makefile for UNIX			#
#							#
#########################################################

OBJS =	curses.o hit.o init.o invent.o level.o \
	machdep.o main.o message.o monster.o move.o \
	object.o pack.o play.o random.o ring.o \
	room.o save.o score.o spechit.o throw.o \
	trap.o use.o zap.o

# for BSD (Japanese, using my own CURSES)	by Yasha
CC	= gcc -traditional-cpp
CFLAGS	= -O -g -DUNIX -DUNIX_BSD4_2 -DCURSES -DJAPAN -DEUC -DTOPSCO
LIBS	=

# for 386BSD (Japanese, using my own CURSES)	by Yasha
#CC	= gcc
#CFLAGS	= -O -g -DUNIX -DUNIX_BSD4_2 -DUNIX_386BSD -DCURSES -DJAPAN -DEUC -DTOPSCO
#LIBS	=

# for NeXT (Japanese, using jcurses of NeXT)	by Yasha
#CFLAGS	= -O -DUNIX -DUNIX_BSD4_2 -DJAPAN -DEUC -DTOPSCO
#LIBS	= -L/usr/nje/lib -lwcurses -ltermcap
#ADDHEADER = jcurses.h

# for BSD
#CFLAGS	= -O -DUNIX -DUNIX_BSD4_2
#LIBS	= -lcurses -ltermcap

# for BSD (with COLOR, using my own CURSES)
#CFLAGS	= -O -DUNIX -DUNIX_BSD4_2 -DCURSES -DCOLOR
#LIBS	=

# for System V
#CFLAGS	= -O -DUNIX -DUNIX_SYSV
#LIBS	= -lcurses

# for System V (with COLOR, using my own CURSES)
#CFLAGS	= -O -DUNIX -DUNIX_SYSV -DCURSES -DCOLOR
#LIBS	=


rogue: $(OBJS)
	$(CC) -o rogue $(OBJS) $(LIBS)

clean:
	rm -f rogue *.o core

$(OBJS): rogue.h $(ADDHEADER)

# Only for jcurses of NeXT step 2.1J
jcurses.h:
	sed -e 's/.*widec\.h.*//g' /usr/nje/include/jcurses.h > jcurses.h
