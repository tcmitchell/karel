CC = gcc
CFLAGS = -g -W -Wall
CPPFLAGS = -I../libkarel
LDFLAGS = -L../libkarel

LDLIBS = -lkarel -lcurses

cgui:	cgui.o
	$(LINK.c) -o $@ $^ $(LDLIBS)

clean:
	$(RM) cgui *.o
