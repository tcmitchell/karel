CC = gcc
CFLAGS = -g -Wall -W

SRCS =	code.c error.c klex.c kparse.c \
	robot.c symbol.c util.c world.c

OBJS = $(SRCS:.c=.o)

default:	libkarel.a($(OBJS))

clean:
	$(RM) *.o libkarel.a
	$(RM) kparse.c kparse.h

kparse.c kparse.h:
#	bison -d -o kparse.c -p karel_yy kparse.y
	bison -d -o kparse.c kparse.y

code.o:	kparse.h
