dnl Process this file with autoconf to produce a configure script.
AC_INIT([karel], [0.8])
AM_INIT_AUTOMAKE

dnl Checks for programs.
AC_PROG_CC
AC_PROG_YACC
AC_PROG_RANLIB

AC_ARG_WITH(gtk,[  --with-gtk              build GTK based interface],
[case "${withval}" in
  yes) with_gtk=true ;;
  no)  with_gtk=false ;;
  *) AC_MSG_ERROR(bad value $withval for --with-gtk) ;;
esac],
[with_gtk=default])

dnl Check a --with-gtk flag to indicate no gtk is to be used.
dnl This will aid in testing, as well as being the right thing.
dnl
AC_PATH_PROG(GTK_CONFIG, gtk-config, no)
if test "$GTK_CONFIG" = "no" ; then
  case "${with_gtk}" in
    true) AC_MSG_ERROR(--with-gtk specified but gtk-config not found in PATH) ;; 
    false | default) build_gtk=false ;;
  esac
else
  case "${with_gtk}" in
    true | default) build_gtk=true ;;
    false) build_gtk=false ;;
  esac
fi

AM_CONDITIONAL(WITH_GTK, test x$build_gtk = xtrue)


dnl Checks for libraries.
dnl Replace `main' with a function in -lcurses:
AC_CHECK_LIB(curses, main)
dnl Replace `main' with a function in -ltermcap:
AC_CHECK_LIB(termcap, main)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(malloc.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.

AC_OUTPUT(Makefile libkarel/Makefile test/Makefile curses/Makefile gtk/Makefile doc/Makefile)
