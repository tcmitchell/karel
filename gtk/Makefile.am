bin_PROGRAMS = karel

karel_SOURCES = karel.c
karel_LDFLAGS = `gtk-config --libs`
karel_LDADD = -L../libkarel -lkarel

INCLUDES = `gtk-config --cflags` -I../libkarel
