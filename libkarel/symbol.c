#include	"karel.h"

static	Symbol	*symtab = 0;		/* the symbol table */

Symbol *
lookup(char *s)			/* find s in symbol table */
{
	Symbol	*sp;					/* loop index */

	for (sp = symtab; sp != (Symbol *) 0; sp = sp->next)
		if (strcmp(s, sp->name) == 0)
			return(sp);
	return(0);			/* not found */
}

void
install(char *s)			/* install s in symbol table */
{
	char	*emalloc();
	Symbol	*sp;				/* new symbol table entry */

	sp = (Symbol *) emalloc(sizeof(Symbol));
	sp->name = emalloc(strlen(s) + 1);
	strcpy(sp->name, s);
	sp->addr = progp;
	sp->next = symtab;
	symtab = sp;
}

char	*emalloc(n)		/* do malloc with error checking */
int	n;
{
	char	*p;			/* pointer to free memory */
	char	*malloc();

	if ((p = malloc(n)) == (char *) 0)
		syserr("out of memory", (char *) 0);
	return(p);
}
