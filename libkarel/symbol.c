/*
  The Karel symbol table.
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

#include "karel.h"
#include "karelP.h"

static ktr_symbol_t *symtab = 0;	/* the symbol table */

/* find s in symbol table */
ktr_symbol_t *
ktr_symbol_lookup (char *s)
{
  ktr_symbol_t *sp;			/* loop index */

  for (sp = symtab; sp != (ktr_symbol_t *) 0; sp = sp->next)
    if (strcmp (s, sp->name) == 0)
      return (sp);
  return (NULL);			/* not found */
}

/* install s in symbol table */
void
ktr_symbol_install (char *s)
{
  ktr_symbol_t *sp;			/* new symbol table entry */

  sp = (ktr_symbol_t *) ktr_malloc (sizeof (ktr_symbol_t));
  sp->name = (char *) ktr_malloc (strlen (s) + 1);
  strcpy (sp->name, s);
  sp->addr = ktr_lex_get_progp ();
  sp->next = symtab;
  symtab = sp;
}
