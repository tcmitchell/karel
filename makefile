# makefile for karel 1.0	11/6/85

SRCS = code.c klex.c main.c scr.c symbol.c
HDRS = help.h karel.h words.h
OBJS = $(SRCS:.c=.o) y.tab.o

CC = gcc
CFLAGS = -W -Wall -g
CFLAGS = -ansi -g
LDFLAGS = -lcurses -ltermcap

karel:		$(OBJS)
		$(LINK.c) -o karel $(OBJS)

$(OBJS):	karel.h

y.tab.c y.tab.h:	karel.y
		yacc -d karel.y

scr.o:		help.h

clean:
	$(RM) *.o karel y.tab.h y.tab.c

distclean:	clean
	$(RM) *~

depend:	y.tab.h
	makedepend $(SRCS) $(HDRS)


# DO NOT DELETE

code.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
code.o: /usr/include/gnu/stubs.h
code.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
code.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
code.o: /usr/include/bits/types.h /usr/include/libio.h
code.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h karel.h
code.o: y.tab.h
klex.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
klex.o: /usr/include/gnu/stubs.h
klex.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
klex.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
klex.o: /usr/include/bits/types.h /usr/include/libio.h
klex.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
klex.o: /usr/include/ctype.h /usr/include/endian.h /usr/include/bits/endian.h
klex.o: karel.h words.h y.tab.h
main.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
main.o: /usr/include/gnu/stubs.h
main.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
main.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
main.o: /usr/include/bits/types.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
main.o: /usr/include/signal.h /usr/include/bits/sigset.h
main.o: /usr/include/bits/signum.h /usr/include/time.h
main.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
main.o: /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h
main.o: /usr/include/bits/sigstack.h karel.h
scr.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
scr.o: /usr/include/gnu/stubs.h
scr.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
scr.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
scr.o: /usr/include/bits/types.h /usr/include/libio.h
scr.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
scr.o: /usr/include/signal.h /usr/include/bits/sigset.h
scr.o: /usr/include/bits/signum.h /usr/include/time.h
scr.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
scr.o: /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h
scr.o: /usr/include/bits/sigstack.h /usr/include/curses.h
scr.o: /usr/include/ncurses/unctrl.h /usr/include/ncurses/curses.h karel.h
scr.o: help.h
symbol.o: karel.h
words.o: y.tab.h
