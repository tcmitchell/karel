bin_PROGRAMS = karelg

karelg_SOURCES = karel.c
karelg_LDFLAGS = `gtk-config --libs`
karelg_LDADD = -L../libkarel -lkarel

INCLUDES = `gtk-config --cflags` -I$(srcdir)/../libkarel
