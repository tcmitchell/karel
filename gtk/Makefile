CC = gcc
CFLAGS = -g -W -Wall
CPPFLAGS = -I../libkarel
LDFLAGS = -L../libkarel

LDLIBS = -lkarel

karel: karel.c 
	$(CC) $(CFLAGS) `gtk-config --cflags` $(CPPFLAGS) $^ -o $@ `gtk-config --libs` $(LDFLAGS) $(LDLIBS)

clean: 
	rm -f *.o karel
