#include <stdio.h>
#include <stdlib.h>
#include "robot.h"

/*----------------------------------------------------------------------*
 *			   Private Functions                            *
 *----------------------------------------------------------------------*/

static char *
dir_to_string(k_direction_t dir)
{
  switch (dir)
    {
    case K_NORTH:
      return "North";

    case K_EAST:
      return "East";

    case K_SOUTH:
      return "South";

    case K_WEST:
      return "West";

    default:
      return "Unknown";
      break;
    }
}

static void
k_robot_set_pos(k_robot_t *r, int street, int avenue)
{
  r->street = street;
  r->avenue = avenue;
  fprintf(stdout, "Karel moved to %d, %d\n", r->street, r->avenue);
}

static void
k_robot_set_dir(k_robot_t *r, k_direction_t dir)
{
  r->dir = dir;
  fprintf(stdout, "Karel is now facing %s\n", dir_to_string(r->dir));
}

static int
is_dir_blocked(k_robot_t *r, k_direction_t dir)
{
  switch (dir)
    {
    case K_NORTH:
      return k_world_check_ew_wall(r->world, r->street, r->avenue);

    case K_EAST:
      return k_world_check_ns_wall(r->world, r->street, r->avenue);

    case K_SOUTH:
      if (r->street == 1)
	return 1;
      else
	return k_world_check_ew_wall(r->world, r->street - 1, r->avenue);

    case K_WEST:
      if (r->avenue == 1)
	return 1;
      else
	return k_world_check_ns_wall(r->world, r->street, r->avenue - 1);

    default:
      /* Need an error stop function */
      return 1;
    }
}


/*----------------------------------------------------------------------*
 *			   Public API                                   *
 *----------------------------------------------------------------------*/

k_robot_t *
k_robot_create(k_world_t *world, int street, int avenue,
	       k_direction_t dir, int n_beepers)
{
  k_robot_t * robot;

  robot = (k_robot_t *) malloc(sizeof(k_robot_t));
  robot->street = street;
  robot->avenue = avenue;
  robot->n_beepers = n_beepers;
  robot->dir = dir;
  robot->world = world;

  return robot;
}

void
k_robot_get_pos(k_robot_t *r, int *street, int *avenue)
{
  (*street) = r->street;
  (*avenue) = r->avenue;
}


/*----------------------------------------------------------------------*
 *			   Karel Built-in tests                         *
 *----------------------------------------------------------------------*/

int
k_robot_any_beepers_in_beeper_bag(k_robot_t *r)
{
  return (r->n_beepers > 0);
}

int
k_robot_facing_east(k_robot_t *r)
{
  return (r->dir == K_EAST);
}

int
k_robot_facing_north(k_robot_t *r)
{
  return (r->dir == K_NORTH);
}

int
k_robot_facing_south(k_robot_t *r)
{
  return (r->dir == K_SOUTH);
}

int
k_robot_facing_west(k_robot_t *r)
{
  return (r->dir == K_WEST);
}

int
k_robot_front_is_blocked(k_robot_t *r)
{
  return is_dir_blocked(r, r->dir);
}

int
k_robot_front_is_clear(k_robot_t *r)
{
  return (! k_robot_front_is_blocked(r));
}

int
k_robot_left_is_blocked(k_robot_t *r)
{
  switch (r->dir)
    {
    case K_NORTH:
      return is_dir_blocked(r, K_WEST);

    case K_EAST:
      return is_dir_blocked(r, K_NORTH);

    case K_SOUTH:
      return is_dir_blocked(r, K_EAST);

    case K_WEST:
      return is_dir_blocked(r, K_SOUTH);

    default:
      /* Need an error stop function */
      return 1;
    }
}

int
k_robot_left_is_clear(k_robot_t *r)
{
  return (! k_robot_left_is_blocked(r));
}

int
k_robot_next_to_a_beeper(k_robot_t *r)
{
  return k_world_check_beeper(r->world, r->street, r->avenue);
}

int
k_robot_no_beepers_in_beeper_bag(k_robot_t *r)
{
  return (r->n_beepers == 0);
}

int
k_robot_not_facing_east(k_robot_t *r)
{
  return (! k_robot_facing_east(r));
}

int
k_robot_not_facing_north(k_robot_t *r)
{
  return (! k_robot_facing_north(r));
}

int
k_robot_not_facing_south(k_robot_t *r)
{
  return (! k_robot_facing_south(r));
}

int
k_robot_not_facing_west(k_robot_t *r)
{
  return (! k_robot_facing_west(r));
}

int
k_robot_not_next_to_a_beeper(k_robot_t *r)
{
  return (! k_robot_next_to_a_beeper(r));
}

int
k_robot_right_is_blocked(k_robot_t *r)
{
  switch (r->dir)
    {
    case K_NORTH:
      return is_dir_blocked(r, K_EAST);

    case K_EAST:
      return is_dir_blocked(r, K_SOUTH);

    case K_SOUTH:
      return is_dir_blocked(r, K_WEST);

    case K_WEST:
      return is_dir_blocked(r, K_NORTH);

    default:
      /* Need an error stop function */
      return 1;
    }
}

int
k_robot_right_is_clear(k_robot_t *r)
{
  return (! k_robot_right_is_blocked(r));
}



/*----------------------------------------------------------------------*
 *			   Karel Built-in procedures                    *
 *----------------------------------------------------------------------*/

/*
 * Need to check that the move forward does not
 * hit a wall, either artificial or a world boundary.
 */
int k_robot_move(k_robot_t *r)
{
  switch (r->dir)
    {
    case K_NORTH:
      if (k_world_check_ew_wall(r->world, r->street, r->avenue))
	{
	  fprintf(stderr, "Karel hit a wall!\n");
	  return K_ERROR;
	}
      else
	{
	  k_robot_set_pos(r, r->street + 1, r->avenue);
	  return K_OK;
	}
      break;

    case K_EAST:
      if (k_world_check_ns_wall(r->world, r->street, r->avenue))
	{
	  fprintf(stderr, "Karel hit a wall!\n");
	  return K_ERROR;
	}
      else
	{
	  k_robot_set_pos(r, r->street, r->avenue + 1);
	  return K_OK;
	}
      return K_ERROR;
      break;

    case K_SOUTH:
      fprintf(stderr, "NYI: South\n");
      return K_ERROR;
      break;

    case K_WEST:
      fprintf(stderr, "NYI: West\n");
      return K_ERROR;
      break;

    default:
      fprintf(stderr, "Unknown direction in k_robot_move\n");
      return K_ERROR;
      break;
    }
}

int
k_robot_pickbeeper(k_robot_t *r)
{
  if (k_world_pick_beeper(r->world, r->street, r->avenue) != -1)
    {
      r->n_beepers += 1;
      return K_OK;
    }
  else
    {
      fprintf(stderr, "No beeper to pick up!\n");
      return K_ERROR;
    }
}

int
k_robot_putbeeper(k_robot_t *r)
{
  if (r->n_beepers > 0)
    {
      k_world_put_beeper(r->world, r->street, r->avenue);
      r->n_beepers -= 1;
      return K_OK;
    }
  else
    {
      fprintf(stderr, "No beeper to put down!\n");
      return K_ERROR;
    }
}

int
k_robot_turnleft(k_robot_t *r)
{
  switch (r->dir)
    {
    case K_NORTH:
      k_robot_set_dir(r, K_WEST);
      return K_OK;

    case K_EAST:
      k_robot_set_dir(r, K_NORTH);
      return K_OK;

    case K_SOUTH:
      k_robot_set_dir(r, K_EAST);
      return K_OK;

    case K_WEST:
      k_robot_set_dir(r, K_SOUTH);
      return K_OK;

    default:
      return K_ERROR;
    }
}

int
k_robot_turnoff(k_robot_t *r)
{
  r = r;			/* Keep gcc happy */
  return K_OK;
}
