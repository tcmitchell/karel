# makefile for karel 1.0	11/6/85

OBJS = main.o klex.o symbol.o code.o y.tab.o scr.o

karel:		$(OBJS)
		cc $(CFLAGS) $(OBJS) -o karel -lcurses -ltermcap

$(OBJS):	karel.h

klex.o:		words.h

words.h:	y.tab.h

y.tab.c y.tab.h:	karel.y
		yacc -d karel.y

scr.o:		help.h
