#include <stdio.h>
#include <stdlib.h>
#include "karel.h"
#include "karelP.h"

/*----------------------------------------------------------------------*
 *			   Private Functions                            *
 *----------------------------------------------------------------------*/

static ktr_robot_move_event_t *
ktr_robot_create_move_event(int old_s, int old_a, int new_s, int new_a)
{
  ktr_robot_move_event_t *ev;

  ev = (ktr_robot_move_event_t *) ktr_malloc(sizeof(ktr_robot_move_event_t));
  ev->old_street = old_s;
  ev->old_avenue = old_a;
  ev->new_street = new_s;
  ev->new_avenue = new_a;
  return ev;
}

static ktr_robot_turn_event_t *
ktr_robot_create_turn_event(ktr_direction_t old, ktr_direction_t new)
{
  ktr_robot_turn_event_t *ev;

  ev = (ktr_robot_turn_event_t *) ktr_malloc (sizeof(ktr_robot_turn_event_t));
  ev->old_direction = old;
  ev->new_direction = new;
  return ev;
}

static void
ktr_robot_set_pos(ktr_robot_t *r, int street, int avenue)
{
  ktr_robot_move_event_t *ev;

  ev = ktr_robot_create_move_event(r->street, r->avenue, street, avenue);
  r->street = street;
  r->avenue = avenue;
  if (r->move_cb)
    r->move_cb(ev);
}

static void
ktr_robot_set_dir(ktr_robot_t *r, ktr_direction_t dir)
{
  ktr_robot_turn_event_t *ev;

  ev = ktr_robot_create_turn_event(r->dir, dir);
  r->dir = dir;
  if (r->turn_cb)
    r->turn_cb(ev);
}

static int
is_dir_blocked(ktr_robot_t *r, ktr_direction_t dir)
{
  switch (dir)
    {
    case KTR_NORTH:
      return ktr_world_check_ew_wall(r->world, r->street, r->avenue);

    case KTR_EAST:
      return ktr_world_check_ns_wall(r->world, r->street, r->avenue);

    case KTR_SOUTH:
      if (r->street == 1)
	return 1;
      else
	return ktr_world_check_ew_wall(r->world, r->street - 1, r->avenue);

    case KTR_WEST:
      if (r->avenue == 1)
	return 1;
      else
	return ktr_world_check_ns_wall(r->world, r->street, r->avenue - 1);

    default:
      /* Need an error stop function */
      return 1;
    }
}


/*----------------------------------------------------------------------*
 *			   Public API                                   *
 *----------------------------------------------------------------------*/

ktr_robot_t *
ktr_robot_create(ktr_world_t *world, int street, int avenue,
	       ktr_direction_t dir, int n_beepers)
{
  ktr_robot_t * robot;

  robot = (ktr_robot_t *) ktr_malloc (sizeof (ktr_robot_t));
  robot->street = street;
  robot->avenue = avenue;
  robot->n_beepers = n_beepers;
  robot->dir = dir;
  robot->world = world;

  return robot;
}

void
ktr_robot_set_move_callback(ktr_robot_t *r, ktr_robot_move_callback_t cb)
{
  r->move_cb = cb;
}

void
ktr_robot_set_turn_callback(ktr_robot_t *r, ktr_robot_turn_callback_t cb)
{
  r->turn_cb = cb;
}

void
ktr_robot_get_pos(ktr_robot_t *r, int *street, int *avenue)
{
  (*street) = r->street;
  (*avenue) = r->avenue;
}


/*----------------------------------------------------------------------*
 *			   Karel Built-in tests                         *
 *----------------------------------------------------------------------*/

int
ktr_robot_any_beepers_in_beeper_bag(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  return (r->n_beepers > 0);
}

int
ktr_robot_facing_east(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  return (r->dir == KTR_EAST);
}

int
ktr_robot_facing_north(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  return (r->dir == KTR_NORTH);
}

int
ktr_robot_facing_south(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  return (r->dir == KTR_SOUTH);
}

int
ktr_robot_facing_west(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  return (r->dir == KTR_WEST);
}

int
ktr_robot_front_is_blocked(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  return is_dir_blocked(r, r->dir);
}

int
ktr_robot_front_is_clear(ktr_engine_t *e, ktr_robot_t *r)
{
  return (! ktr_robot_front_is_blocked(e, r));
}

int
ktr_robot_left_is_blocked(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  switch (r->dir)
    {
    case KTR_NORTH:
      return is_dir_blocked(r, KTR_WEST);

    case KTR_EAST:
      return is_dir_blocked(r, KTR_NORTH);

    case KTR_SOUTH:
      return is_dir_blocked(r, KTR_EAST);

    case KTR_WEST:
      return is_dir_blocked(r, KTR_SOUTH);

    default:
      /* Need an error stop function */
      return 1;
    }
}

int
ktr_robot_left_is_clear(ktr_engine_t *e, ktr_robot_t *r)
{
  return (! ktr_robot_left_is_blocked(e, r));
}

int
ktr_robot_next_to_a_beeper(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  return ktr_world_check_beeper(r->world, r->street, r->avenue);
}

int
ktr_robot_no_beepers_in_beeper_bag(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  return (r->n_beepers == 0);
}

int
ktr_robot_not_facing_east(ktr_engine_t *e, ktr_robot_t *r)
{
  return (! ktr_robot_facing_east(e, r));
}

int
ktr_robot_not_facing_north(ktr_engine_t *e, ktr_robot_t *r)
{
  return (! ktr_robot_facing_north(e, r));
}

int
ktr_robot_not_facing_south(ktr_engine_t *e, ktr_robot_t *r)
{
  return (! ktr_robot_facing_south(e, r));
}

int
ktr_robot_not_facing_west(ktr_engine_t *e, ktr_robot_t *r)
{
  return (! ktr_robot_facing_west(e, r));
}

int
ktr_robot_not_next_to_a_beeper(ktr_engine_t *e, ktr_robot_t *r)
{
  return (! ktr_robot_next_to_a_beeper(e, r));
}

int
ktr_robot_right_is_blocked(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  switch (r->dir)
    {
    case KTR_NORTH:
      return is_dir_blocked(r, KTR_EAST);

    case KTR_EAST:
      return is_dir_blocked(r, KTR_SOUTH);

    case KTR_SOUTH:
      return is_dir_blocked(r, KTR_WEST);

    case KTR_WEST:
      return is_dir_blocked(r, KTR_NORTH);

    default:
      /* Need an error stop function */
      return 1;
    }
}

int
ktr_robot_right_is_clear(ktr_engine_t *e, ktr_robot_t *r)
{
  return (! ktr_robot_right_is_blocked(e, r));
}



/*----------------------------------------------------------------------*
 *			   Karel Built-in procedures                    *
 *----------------------------------------------------------------------*/

/*
 * Need to check that the move forward does not
 * hit a wall, either artificial or a world boundary.
 */
int ktr_robot_move(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  switch (r->dir)
    {
    case KTR_NORTH:
      if (ktr_world_check_ew_wall(r->world, r->street, r->avenue))
	{
	  fprintf(stderr, "Karel hit a wall to the north!\n");
	  return KTR_ERROR;
	}
      else
	{
	  ktr_robot_set_pos(r, r->street + 1, r->avenue);
	  return KTR_OK;
	}
      break;

    case KTR_EAST:
      if (ktr_world_check_ns_wall(r->world, r->street, r->avenue))
	{
	  fprintf(stderr, "Karel hit a wall to the east!\n");
	  return KTR_ERROR;
	}
      else
	{
	  ktr_robot_set_pos(r, r->street, r->avenue + 1);
	  return KTR_OK;
	}
      return KTR_ERROR;
      break;

    case KTR_SOUTH:
      if (r->street == 1)
	{
	  fprintf(stderr, "Karel hit a wall at the south edge!\n");
	  return KTR_ERROR;
	}
      else if (ktr_world_check_ew_wall(r->world, r->street - 1, r->avenue))
	{
	  fprintf(stderr, "Karel hit a wall to the south!\n");
	  return KTR_ERROR;
	}
      else
	{
	  ktr_robot_set_pos(r, r->street - 1, r->avenue);
	  return KTR_OK;
	}
      break;

    case KTR_WEST:
      if (r->avenue == 1)
	{
	  fprintf(stderr, "Karel hit a wall at the west edge!\n");
	  return KTR_ERROR;
	}
      if (ktr_world_check_ns_wall(r->world, r->street, r->avenue - 1))
	{
	  fprintf(stderr, "Karel hit a wall to the west!\n");
	  return KTR_ERROR;
	}
      else
	{
	  ktr_robot_set_pos(r, r->street, r->avenue - 1);
	  return KTR_OK;
	}
      break;

    default:
      fprintf(stderr, "Unknown direction in ktr_robot_move\n");
      return KTR_ERROR;
      break;
    }
}

int
ktr_robot_pickbeeper(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  if (ktr_world_pick_beeper(r->world, r->street, r->avenue) != -1)
    {
      r->n_beepers += 1;
      return KTR_OK;
    }
  else
    {
      fprintf(stderr, "No beeper to pick up!\n");
      return KTR_ERROR;
    }
}

int
ktr_robot_putbeeper(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  if (r->n_beepers > 0)
    {
      ktr_world_put_beeper(r->world, r->street, r->avenue);
      r->n_beepers -= 1;
      return KTR_OK;
    }
  else
    {
      fprintf(stderr, "No beeper to put down!\n");
      return KTR_ERROR;
    }
}

int
ktr_robot_turnleft(ktr_engine_t *e, ktr_robot_t *r)
{
  e = e;			/* Keep gcc happy */
  switch (r->dir)
    {
    case KTR_NORTH:
      ktr_robot_set_dir(r, KTR_WEST);
      return KTR_OK;

    case KTR_EAST:
      ktr_robot_set_dir(r, KTR_NORTH);
      return KTR_OK;

    case KTR_SOUTH:
      ktr_robot_set_dir(r, KTR_EAST);
      return KTR_OK;

    case KTR_WEST:
      ktr_robot_set_dir(r, KTR_SOUTH);
      return KTR_OK;

    default:
      return KTR_ERROR;
    }
}

/*----------------------------------------------------------------------*
 *		       Public Utility Functions                         *
 *----------------------------------------------------------------------*/
char *
ktr_robot_dir_to_string(ktr_direction_t dir)
{
  switch (dir)
    {
    case KTR_NORTH:
      return "North";

    case KTR_EAST:
      return "East";

    case KTR_SOUTH:
      return "South";

    case KTR_WEST:
      return "West";

    default:
      return "Unknown";
      break;
    }
}
