#include <stdlib.h>
#include "karel.h"
#include "karelP.h"


/* do malloc with error checking */
void *
emalloc(size_t size)
{
  void *p;			/* pointer to free memory */

  p = malloc(size);

  if (p == NULL)
    {
      syserr("out of memory", (char *) 0);
      return NULL;		/* never reached, for compiler */
    }
  else
    return(p);
}
