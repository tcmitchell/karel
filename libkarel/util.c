#include <stdlib.h>
#include "error.h"
#include "util.h"


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
