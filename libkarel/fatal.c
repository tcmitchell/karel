/*
  Fatal error handler.
  Copyright (C) 2000 Tom Mitchell

  This file is part of Karel.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <stdio.h>
#if STDC_HEADERS
# include <stdarg.h>
#endif /* STDC_HEADERS */

/* Print an error message and exit reporting failure.
   This is for a error that we cannot continue from.
   The arguments are printed a la printf.

   This function cannot be declared volatile (NORETURN) in an
   ANSI environment because exit() is not declared volatile. */

/* VARARGS */
void
ktr_fatal_err (char *string, ...)
{
  va_list args;
  va_start (args, string);
  fprintf (stderr, "\nkarel: ");
  vfprintf (stderr, string, args);
  fprintf (stderr, "\n");
  va_end (args);
  exit (1);
}
