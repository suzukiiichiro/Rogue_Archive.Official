# Makefile for Dungeon Bash
#
# Linux/gcc only; I'm writing this in riceboy-hacker mode.  Deal.

OBJS=bmagic.o combat.o display.o main.o map.o misc.o monsters.o mon2.o objects.o permobj.o permons.o pmon2.o rng.o u.o vector.o

GAME=dungeonbash
# MPR: per <lj user="ewx">, users are advised to remove -Werror when
# building on Darwin due to a bug in Darwin's version of "ncurses.h".
#
# It is apparently possible to build this code for Windows XP, although I have
# no details.
MAJVERS=1
MINVERS=7
CFLAGS=-c -g -Wall -Wstrict-prototypes -Wwrite-strings -Wmissing-prototypes -Werror -Wredundant-decls -Wunreachable-code -DMAJVERS=$(MAJVERS) -DMINVERS=$(MINVERS) -Wno-parentheses-equality
LINKFLAGS=-lpanel -lncurses -g

all: $(GAME)

$(GAME): $(OBJS)
	$(CC) $(OBJS) $(LINKFLAGS) -o $(GAME)

archive: clean
	(cd .. && tar cvzf dungeonbash-$(MAJVERS).$(MINVERS).tar.gz dungeonbash-$(MAJVERS).$(MINVERS))

clean:
	-rm -f *.o $(GAME) dunbash.log dunbash.sav.gz

display.o: display.c dunbash.h

main.o: main.c combat.h dunbash.h monsters.h

combat.o: combat.c combat.h dunbash.h

u.o: u.c combat.h dunbash.h

permobj.o: permobj.c dunbash.h

map.o: map.c dunbash.h

permons.o: permons.c dunbash.h

pmon2.o: pmon2.c dunbash.h monsters.h

objects.o: objects.c dunbash.h

monsters.o: monsters.c dunbash.h monsters.h

mon2.o: mon2.c dunbash.h bmagic.h monsters.h

vector.o: vector.c dunbash.h

bmagic.o: bmagic.c dunbash.h bmagic.h
