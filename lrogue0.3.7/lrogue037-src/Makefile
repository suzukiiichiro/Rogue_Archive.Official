# LinuxRogue makefile
# by Ashwin N <ashwin@despammed.com> 

VERSION ?= 0.3.7
SHELL = /bin/bash
CC = gcc

SOURCES = hit.c init.c instruct.c inventory.c level.c machdep.c main.c \
	  message.c monster.c move.c object.c pack.c play.c random.c ring.c \
	  room.c save.c score.c special_hit.c throw.c trap.c use.c zap.c
OBJECTS = $(SOURCES:.c=.o)

#CFLAGS = -O2 -fomit-frame-pointer -funroll-loops -Wall
CFLAGS = -O3 -fomit-frame-pointer -funroll-loops -w
LDFLAGS = -lncurses

BUILDDIR = ./rpm
DESTDIR ?= /usr/local/games

rogue: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o rogue

dist.linux32:
	$(MAKE) CC="gcc -m32"

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) rogue *.tar.gz .roguescores
	rm -rf $(BUILDDIR)

install:
	cp ./rogue $(DESTDIR)/rogue

uninstall:
	rm -f $(DESTDIR)/rogue

release: clean
	mkdir linuxrogue-$(VERSION)
	cp *.c linuxrogue-$(VERSION)
	cp *.h linuxrogue-$(VERSION)
	cp Makefile linuxrogue-$(VERSION)
#	cp AUTHORS linuxrogue-$(VERSION)
#	cp CHANGES linuxrogue-$(VERSION)
#	cp README linuxrogue-$(VERSION)
#	cp rogue-guide.txt linuxrogue-$(VERSION)
	tar cvzf linuxrogue-$(VERSION).tar.gz linuxrogue-$(VERSION)
	rm -rf linuxrogue-$(VERSION)

rpmprep:
	mkdir -p $(BUILDDIR)/BUILD
	mkdir -p $(BUILDDIR)/SOURCES
	mkdir -p $(BUILDDIR)/RPMS
	mkdir -p $(BUILDDIR)/SRPMS
	mkdir -p $(BUILDDIR)/SPECS

rpm: release rpmprep
	cp linuxrogue-$(VERSION).tar.gz $(BUILDDIR)/SOURCES/linuxrogue-$(VERSION).tar.gz
	cp linuxrogue.spec $(BUILDDIR)/SPECS/linuxrogue.spec
#	rpmbuild -ba --define '_topdir $(BUILDDIR)' --define '_tmpdir $(BUILDDIR)' linuxrogue.spec
	rpmbuild -ba linuxrogue.spec
