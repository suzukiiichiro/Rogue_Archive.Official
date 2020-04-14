#########################################################
#							#
#	Makefile - Makefile for Rogue-clone II		#
#							#
#########################################################

OBJS =	curses.obj hit.obj init.obj invent.obj level.obj\
	machdep.obj main.obj message.obj monster.obj move.obj\
	object.obj pack.obj play.obj random.obj ring.obj\
	room.obj save.obj score.obj spechit.obj throw.obj\
	trap.obj use.obj zap.obj

CC	= tcc -mm
CFLAGS	= -O -G- -J -d -f- -DMSDOS -DCURSES -DJAPAN -DCOLOR -DTOPSCO

.SUFFIXES:
.SUFFIXES: .obj .c

.c.obj:
	$(CC) -c $(CFLAGS) $*.c

all: rogue.exe rogue.com

rogue.exe: $(OBJS)
	-del roguelib.*
	tlib roguelib +curses+hit+init+invent+level\
	+machdep+message+monster+move+object,nul
	tlib roguelib +pack+play+random+ring+room+save\
	+score+spechit+throw+trap+use+zap,nul,
	tcc -mm -erogue main.obj roguelib.lib

rogue.com: playmenu.c
	tcc -mt $(CFLAGS) playmenu.c
	-del rogue.com
	ren playmenu.exe rogue.com

clean:
	-del rogue.exe
	-del rogue.com
	-del roguelib.*
	-del *.obj

$(OBJS): rogue.h
