#include "karel.h"
#include "karelP.h"

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

  sp = (Symbol *) ktr_malloc(sizeof(Symbol));
  sp->name = (char *) ktr_malloc (strlen(s) + 1);
  strcpy(sp->name, s);
  sp->addr = ktr_lex_get_progp();
  sp->next = symtab;
  symtab = sp;
}
