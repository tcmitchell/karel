/*
  No Memory error handler.
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

void
ktr_nomem_err(size_t size)
{
  if (size > 0)
    {
      ktr_fatal_err ("virtual memory exhausted: can't allocate %ld bytes.",
		     (long) size);
    }
  else
    {
      ktr_fatal_err ("virtual memory exhausted.");
    }
}
