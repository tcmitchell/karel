#include <stdio.h>
#include <stdlib.h>
#include "karel.h"

/*----------------------------------------------------------------------*
 *			   Private Functions                            *
 *----------------------------------------------------------------------*/
static k_corner_t *
k_corner_create()
{
  k_corner_t *corner;

  corner = (k_corner_t *) malloc(sizeof(k_corner_t));
  corner->n_beepers = 0;
  corner->has_wall_north = 0;
  corner->has_wall_east = 0;
  corner->has_wall_south = 0;
  corner->has_wall_west = 0;

  return corner;
}

static k_corner_t *
k_world_get_corner(k_world_t *w, int s, int a)
{
  return w->corners[s][a];
}

void
k_world_init(k_world_t *w)
{
  int s, a;
  k_corner_t *c;

  k_world_add_ew_wall(w, w->n_streets, 1, w->n_avenues);
  k_world_add_ns_wall(w, 1, w->n_avenues, w->n_streets);

  /* Add the southerly boundary of the world. */
  s = 0;
  for (a=0; a<w->n_avenues; a++)
    {
      c = k_world_get_corner(w, s, a);
      c->has_wall_south = 1;
    }
  
  /* Add the westerly boundary of the world. */
  a = 0;
  for (s=0; s<w->n_streets; s++)
    {
      c = k_world_get_corner(w, s, a);
      c->has_wall_west = 1;
    }
}


/*----------------------------------------------------------------------*
 *		      Public API Implementation                         *
 *----------------------------------------------------------------------*/
k_world_t *
k_world_create(int n_streets, int n_avenues)
{
  k_world_t *world;
  int s, a;

  world = (k_world_t *) malloc(sizeof(k_world_t));
  world->n_streets = n_streets;
  world->n_avenues = n_avenues;
  world->corners = (k_corner_t ***) calloc(n_streets, sizeof(k_corner_t *));
  for (s=0; s<n_streets; s++)
    {
      world->corners[s] = (k_corner_t **) calloc(n_avenues, sizeof(k_corner_t *));
      for (a=0; a<n_avenues; a++)
	world->corners[s][a] = k_corner_create();
    }

  k_world_init(world);

  k_world_add_ew_wall(world, 2, 2, 2);
  k_world_add_ns_wall(world, 1, 1, 1);
  k_world_add_ns_wall(world, 1, 2, 2);
  k_world_add_ns_wall(world, 1, 3, 3);
  k_world_add_ns_wall(world, 1, 4, 4);

  return world;
}

void
k_world_add_ew_wall(k_world_t *w,
		    int north_of_street,
		    int at_avenue,
		    int length_to_east)
{
  int i;
  int s, a;
  k_corner_t *c;

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
      c = k_world_get_corner(w, s, a);
      c->has_wall_north = 1;
      if (s+1 < w->n_streets)
	{
	  /* Add the southerly wall to the corners above if they're still
	   * in range. */
	  c = k_world_get_corner(w, s+1, a);
	  c->has_wall_south = 1;
	}
    }
}

void
k_world_add_ns_wall(k_world_t *w,
		    int at_street,
		    int east_of_avenue,
		    int length_to_north)
{
  /* TODO: mark the wall to the west of the eastern corners */
  int i;
  int s, a;
  k_corner_t *c;

  --at_street;
  --east_of_avenue;

  a = east_of_avenue;
  for (i=0; i<length_to_north; i++)
    {
      s = at_street + i;
      c = k_world_get_corner(w, s, a);
      c->has_wall_east = 1;
      if (a+1 < w->n_avenues)
	{
	  /* Add the westerly wall to the corners east if they're still
	   * in range. */
	  c = k_world_get_corner(w, s, a+1);
	  c->has_wall_west = 1;
	}
    }
}

int
k_world_check_ew_wall(k_world_t *w, int north_of_street, int at_avenue)
{
  k_corner_t *corner;
  corner = k_world_get_corner(w, --north_of_street, --at_avenue);
  return corner->has_wall_north;
}

int
k_world_check_ns_wall(k_world_t *w, int at_street, int east_of_avenue)
{
  k_corner_t *corner;
  corner = k_world_get_corner(w, --at_street, --east_of_avenue);
  return corner->has_wall_east;
}

int
k_world_check_beeper(k_world_t *w, int street, int avenue)
{
  k_corner_t *corner;
  corner = k_world_get_corner(w, --street, --avenue);
  return (corner->n_beepers > 0);
}

int
k_world_pick_beeper(k_world_t *w, int street, int avenue)
{
  k_corner_t *corner;
  corner = k_world_get_corner(w, --street, --avenue);
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
k_world_put_beeper(k_world_t *w, int street, int avenue)
{
  k_corner_t *corner;
  corner = k_world_get_corner(w, --street, --avenue);
  corner->n_beepers += 1;
  /* send an event! */
  return 0;
}

void
k_world_print(k_world_t *w)
{
  int s, a;

  for (s=w->n_streets; s>0; s--)
    {
      for (a=1; a<=w->n_avenues; a++)
	{
	  printf(" ");
	  if (k_world_check_ew_wall(w, s, a))
	    printf("-");
	  else
	    printf(" ");
	}
      printf("\n");
      printf("|");
      for (a=1; a<=w->n_avenues; a++)
	{
	  printf("+");
	  if (k_world_check_ns_wall(w, s, a))
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
  k_world_t *world;

  world = k_world_create(5, 5);
  k_world_print(world);
  return 0;
}
