/*  #include "karel.h" */
#include "util.h"
#include "symbol.h"

/* FIX THIS */
extern int progp;		/* from code.c */

static	Symbol	*symtab = 0;	/* the symbol table */

/* find s in symbol table */
Symbol *
lookup(char *s)
{
  Symbol *sp;			/* loop index */

  for (sp = symtab; sp != (Symbol *) 0; sp = sp->next)
    if (strcmp(s, sp->name) == 0)
      return(sp);
  return(NULL);			/* not found */
}

/* install s in symbol table */
void
install(char *s)
{
  Symbol *sp;			/* new symbol table entry */

  sp = (Symbol *) emalloc(sizeof(Symbol));
  sp->name = emalloc(strlen(s) + 1);
  strcpy(sp->name, s);
  sp->addr = progp;
  sp->next = symtab;
  symtab = sp;
}
