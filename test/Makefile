CC = gcc
CFLAGS = -g -W -Wall
CPPFLAGS = -I../libkarel
LDFLAGS = -L../libkarel

LDLIBS = -lkarel

ktest:	testmain.o
	$(LINK.c) -o $@ $^ $(LDLIBS)

clean:
	$(RM) ktest *.o
