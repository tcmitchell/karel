/*
  Memory management for Karel.
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


#if STDC_HEADERS
# include <stdlib.h>
#else  /* ! STDC_HEADERS */
# if HAVE_MALLOC_H
#  include <malloc.h>
# endif/* HAVE_MALLOC_H */
#endif /* ! STDC_HEADERS */

#include "karel.h"
#include "karelP.h"

/*
  Allocates nmemb elements of size bytes each.  The allocated memory
  is set to zero.  See man calloc.
*/
void *
ktr_calloc(size_t nmemb, size_t size)
{
  void *val;

  if (nmemb * size == 0)
    {
      val = NULL;
    }
  else if ((val = calloc(nmemb, size)) == NULL)
    {
      ktr_nomem_err (nmemb * size);
    }
  return (val);
}

/*
  Allocates size bytes of memory.  See man malloc.
*/
void *
ktr_malloc(size_t size)
{
  void *val;

  if (size == 0)
    {
      val = NULL;
    }
  else if ((val = malloc(size)) == NULL)
    {
      ktr_nomem_err (size);
    }
  return (val);
}

/*
  Frees memory at ptr.  See man free.
 */
void
ktr_free(void *ptr)
{
  free (ptr);
}

/*
  Reallocates memory at ptr.  See man realloc.
 */
void *
ktr_realloc(void *ptr, size_t size)
{
  register void *val;

  if (ptr != NULL)
    {
      val = realloc(ptr, size);
    }
  else
    {
      val = malloc(size);
    }
  if (val == NULL)
    {
      ktr_nomem_err (size);
    }
  return (val);
}