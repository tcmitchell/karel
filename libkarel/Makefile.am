lib_LIBRARIES = libkarel.a

libkarel_a_SOURCES =	kparse.y \
			engine.c \
			fatal.c \
			karel.h karelP.h \
			klex.c \
			memory.c \
			nomem.c \
			parseerr.c \
			robot.c \
			symbol.c \
			world.c \
			yyerror.c

YFLAGS = -d

EXTRA_DIST = kparse.h

AM_CFLAGS = -W -Wall

MAINTAINERCLEANFILES = kparse.h kparse.c
