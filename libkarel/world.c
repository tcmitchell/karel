/*
  The Karel world object.
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
#include <stdlib.h>
#include <string.h>
#include "karel.h"

/*----------------------------------------------------------------------*
 *			   Private Functions                            *
 *----------------------------------------------------------------------*/
static ktr_corner_t *
ktr_corner_create()
{
  ktr_corner_t *corner;

  corner = (ktr_corner_t *) ktr_malloc(sizeof(ktr_corner_t));
  corner->n_beepers = 0;
  corner->has_wall_north = 0;
  corner->has_wall_east = 0;
  corner->has_wall_south = 0;
  corner->has_wall_west = 0;

  return corner;
}

static ktr_corner_t *
ktr_world_get_corner(ktr_world_t *w, int s, int a)
{
  return w->corners[s][a];
}

void
ktr_world_init(ktr_world_t *w)
{
  int s, a;
  ktr_corner_t *c;

  ktr_world_add_ew_wall(w, w->n_streets, 1, w->n_avenues);
  ktr_world_add_ns_wall(w, 1, w->n_avenues, w->n_streets);

  /* Add the southerly boundary of the world. */
  s = 0;
  for (a=0; a<w->n_avenues; a++)
    {
      c = ktr_world_get_corner(w, s, a);
      c->has_wall_south = 1;
    }
  
  /* Add the westerly boundary of the world. */
  a = 0;
  for (s=0; s<w->n_streets; s++)
    {
      c = ktr_world_get_corner(w, s, a);
      c->has_wall_west = 1;
    }
}


/*----------------------------------------------------------------------*
 *		      Public API Implementation                         *
 *----------------------------------------------------------------------*/
ktr_world_t *
ktr_world_create(int n_streets, int n_avenues)
{
  ktr_world_t *world;
  int s, a;

  world = (ktr_world_t *) ktr_malloc(sizeof(ktr_world_t));
  world->n_streets = n_streets;
  world->n_avenues = n_avenues;
  world->corners
    = (ktr_corner_t ***) ktr_calloc(n_streets, sizeof(ktr_corner_t **));

  for (s=0; s<n_streets; s++)
    {
      world->corners[s]
	= (ktr_corner_t **) ktr_calloc(n_avenues, sizeof(ktr_corner_t *));
      for (a=0; a<n_avenues; a++)
	world->corners[s][a] = ktr_corner_create();
    }

  ktr_world_init(world);

/*    ktr_world_add_ew_wall(world, 2, 2, 2); */
/*    ktr_world_add_ns_wall(world, 1, 1, 1); */
/*    ktr_world_add_ns_wall(world, 1, 2, 2); */
/*    ktr_world_add_ns_wall(world, 1, 3, 3); */
/*    ktr_world_add_ns_wall(world, 1, 4, 4); */

  return world;
}

void
ktr_world_add_ew_wall(ktr_world_t *w,
		    int north_of_street,
		    int at_avenue,
		    int length_to_east)
{
  int i;
  int s, a;
  ktr_corner_t *c;

  if (north_of_street > w->n_streets)
    {
      printf("Execution error: add_ew_wall north_of_street = %d, n_streets = %d\n", north_of_street, w->n_streets);
    }

  --north_of_street;
  --at_avenue;

  s = north_of_street;
  for (i=0; i<length_to_east; i++)
    {
      a = at_avenue + i;
      c = ktr_world_get_corner(w, s, a);
      c->has_wall_north = 1;
      if (s+1 < w->n_streets)
	{
	  /* Add the southerly wall to the corners above if they're still
	   * in range. */
	  c = ktr_world_get_corner(w, s+1, a);
	  c->has_wall_south = 1;
	}
    }
}

void
ktr_world_add_ns_wall(ktr_world_t *w,
		    int at_street,
		    int east_of_avenue,
		    int length_to_north)
{
  /* TODO: mark the wall to the west of the eastern corners */
  int i;
  int s, a;
  ktr_corner_t *c;

  --at_street;
  --east_of_avenue;

  a = east_of_avenue;
  for (i=0; i<length_to_north; i++)
    {
      s = at_street + i;
      c = ktr_world_get_corner(w, s, a);
      c->has_wall_east = 1;
      if (a+1 < w->n_avenues)
	{
	  /* Add the westerly wall to the corners east if they're still
	   * in range. */
	  c = ktr_world_get_corner(w, s, a+1);
	  c->has_wall_west = 1;
	}
    }
}

int
ktr_world_check_ew_wall(ktr_world_t *w, int north_of_street, int at_avenue)
{
  ktr_corner_t *corner;
  corner = ktr_world_get_corner(w, --north_of_street, --at_avenue);
  return corner->has_wall_north;
}

int
ktr_world_check_ns_wall(ktr_world_t *w, int at_street, int east_of_avenue)
{
  ktr_corner_t *corner;
  corner = ktr_world_get_corner(w, --at_street, --east_of_avenue);
  return corner->has_wall_east;
}

int
ktr_world_check_beeper(ktr_world_t *w, int street, int avenue)
{
  ktr_corner_t *corner;
  corner = ktr_world_get_corner(w, --street, --avenue);
  return corner->n_beepers;
}

int
ktr_world_pick_beeper(ktr_world_t *w, int street, int avenue)
{
  ktr_corner_t *corner;
  corner = ktr_world_get_corner(w, --street, --avenue);
  if (corner->n_beepers > 0)
    {
      corner->n_beepers -= 1;
      /* send an event! */
      return 0;
    }
  else
    {
      return -1;
    }
}

int
ktr_world_put_beeper(ktr_world_t *w, int street, int avenue)
{
  ktr_corner_t *corner;
  corner = ktr_world_get_corner(w, --street, --avenue);
  corner->n_beepers += 1;
  /* send an event! */
  return 0;
}

/*
  Reads a world from disk.
*/
ktr_world_t *
ktr_world_read (FILE *fp)
{
  ktr_world_t *world = NULL;
  char buf[BUFSIZ];
  char *token;
  int line_num = 1;

  while (fgets (buf, BUFSIZ, fp) != NULL)
    {
      token = strtok (buf, " ");

      if (strncasecmp(token, "world", 5) == 0)
	{
	  int n_streets, n_avenues;
	  n_avenues = atoi (strtok (NULL, " "));
	  n_streets = atoi (strtok (NULL, " "));
/*  	  printf ("Defined world as %d x %d\n", n_streets, n_avenues); */
	  world = ktr_world_create(n_streets, n_avenues);
	}
      else if (strncasecmp(token, "robot", 5) == 0)
	{
	  int st, ave, dir, beepers;
	  ave = atoi (strtok (NULL, " "));
	  st = atoi (strtok (NULL, " "));
	  dir = atoi (strtok (NULL, " "));
	  beepers = atoi (strtok (NULL, " "));
/*  	  printf ("Defined robot at %d,%d, facing %d, with %d beepers\n", */
/*  		  st, ave, dir, beepers); */
	}
      else if (strncasecmp(token, "wall", 4) == 0)
	{
	  int st, ave, dir;
	  ave = atoi (strtok (NULL, " "));
	  st = atoi (strtok (NULL, " "));
	  dir = atoi (strtok (NULL, " "));
/*  	  printf ("Defined wall %d of %d,%d\n", dir, st, ave); */
	  if (world == NULL)
	    {
	      fprintf (stderr,
		       "Commands precede world definition at line %d\n",
		       line_num);
	    }
	  else if (dir == KTR_NORTH)
	    {
	      ktr_world_add_ew_wall(world, st, ave, 1);
	    }
	  else if (dir == KTR_WEST)
	    {
	      ktr_world_add_ns_wall(world, st, ave, 1);
	    }
	  else
	    {
	      fprintf (stderr, "Unsupported wall command at line %d\n",
		       line_num);
	    }
	}
      else if (strncasecmp(token, "beepers", 7) == 0)
	{
	  int st, ave, n;
	  ave = atoi (strtok (NULL, " "));
	  st = atoi (strtok (NULL, " "));
	  n = atoi (strtok (NULL, " "));
	  printf ("Defined %d beepers at %d,%d\n", n, st, ave);
	  if (world == NULL)
	    {
	      fprintf (stderr,
		       "Commands precede world definition at line %d\n",
		       line_num);
	    }
	  else
	    {
	      int i;
	      for (i=0; i<n; i++)
		ktr_world_put_beeper(world, st, ave);
	    }
	}
      else
	{
	  fprintf (stderr, "invalid token \"%s\" at line %d\n",
		   token, line_num);
	  if (world != NULL)
	    ktr_free (world);
	  return NULL;
	}

      ++line_num;
    }
  return world;
}

void
ktr_world_print(ktr_world_t *w)
{
  int s, a;

  for (s=w->n_streets; s>0; s--)
    {
      for (a=1; a<=w->n_avenues; a++)
	{
	  printf(" ");
	  if (ktr_world_check_ew_wall(w, s, a))
	    printf("-");
	  else
	    printf(" ");
	}
      printf("\n");
      printf("|");
      for (a=1; a<=w->n_avenues; a++)
	{
	  int n_beepers = ktr_world_check_beeper(w, s, a);
	  if (n_beepers == 0)
	    printf("+");
	  else
	    printf("%d", n_beepers);
	  if (ktr_world_check_ns_wall(w, s, a))
	    printf("|");
	  else
	    printf(" ");
	}
      printf("\n");
    }
  for (a=1; a<=w->n_avenues; a++)
    {
      printf(" -");
    }
  printf("\n");
}

int
test_world()
{
  ktr_world_t *world;

  world = ktr_world_create(5, 5);
  ktr_world_print(world);
  return 0;
}
