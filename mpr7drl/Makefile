# Makefile for martin's seven-day roguelike
#
# Linux/gcc only; I'm writing this in riceboy-hacker mode.  Deal.

OBJS=u.o permobj.o permons.o display.o main.o map.o objects.o monsters.o combat.o
GAME=mpr7drl
CFLAGS=-c -g -Wall -Wstrict-prototypes -Wwrite-strings -Wmissing-prototypes -Werrori -Wno-parentheses-equality
LINKFLAGS=-lpanel -lncurses -g

all: $(GAME)

$(GAME): $(OBJS)
	$(CC) $(OBJS) $(LINKFLAGS) -o $(GAME)

archive:
	tar cvzf mpr7drl.tar.gz *.c *.h Makefile *.txt

clean:
	-rm -f *.o $(GAME)

display.o: display.c 7drl.h

main.o: main.c 7drl.h

combat.o: combat.c 7drl.h

u.o: u.c 7drl.h

permobj.o: permobj.c 7drl.h

map.o: map.c 7drl.h

permons.o: permons.c 7drl.h

objects.o: objects.c 7drl.h

monsters.o: monsters.c 7drl.h
