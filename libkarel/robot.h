#ifndef __robot_h
#define __robot_h

#include "world.h"

#define K_ERROR 1
#define K_OK 0

typedef enum {
  K_NORTH = 247,
  K_EAST,
  K_SOUTH,
  K_WEST
} k_direction_t;

/*!
 *
 */
typedef struct k_robot_move_event {
  int old_street;
  int old_avenue;
  int new_street;
  int new_avenue;
} k_robot_move_event_t;

/*!
 *
 */
typedef struct k_robot_turn_event {
  k_direction_t old_direction;
  k_direction_t new_direction;
} k_robot_turn_event_t;

/*!
 *
 */
typedef	void (*k_robot_move_callback_t)(k_robot_move_event_t *);
typedef	void (*k_robot_turn_callback_t)(k_robot_turn_event_t *);

/*!
 *
 */
typedef struct k_robot {
  int street;
  int avenue;
  int n_beepers;
  k_direction_t dir;
  k_world_t *world;
  k_robot_move_callback_t move_cb;
  k_robot_turn_callback_t turn_cb;
} k_robot_t;

/*
 * Creates a new robot at the given position and orientation in the
 * given world.
 */
k_robot_t *
k_robot_create(k_world_t *world,
	       int street, int avenue,
	       k_direction_t dir, int n_beepers);

void
k_robot_set_move_callback(k_robot_t *r, k_robot_move_callback_t cb);

void
k_robot_set_turn_callback(k_robot_t *r, k_robot_turn_callback_t cb);

/*!
 */
void k_robot_get_pos(k_robot_t *r, int *street, int *avenue);

char *k_robot_dir_to_string(k_direction_t dir);

/*----------------------------------------------------------------------*
 *			   Karel Built-in tests                         *
 *----------------------------------------------------------------------*/
int k_robot_any_beepers_in_beeper_bag(k_robot_t *r);
int k_robot_facing_east(k_robot_t *r);
int k_robot_facing_north(k_robot_t *r);
int k_robot_facing_south(k_robot_t *r);
int k_robot_facing_west(k_robot_t *r);
int k_robot_front_is_blocked(k_robot_t *r);
int k_robot_front_is_clear(k_robot_t *r);
int k_robot_left_is_blocked(k_robot_t *r);
int k_robot_left_is_clear(k_robot_t *r);
int k_robot_next_to_a_beeper(k_robot_t *r);
int k_robot_no_beepers_in_beeper_bag(k_robot_t *r);
int k_robot_not_facing_east(k_robot_t *r);
int k_robot_not_facing_north(k_robot_t *r);
int k_robot_not_facing_south(k_robot_t *r);
int k_robot_not_facing_west(k_robot_t *r);
int k_robot_not_next_to_a_beeper(k_robot_t *r);
int k_robot_right_is_blocked(k_robot_t *r);
int k_robot_right_is_clear(k_robot_t *r);

/*----------------------------------------------------------------------*
 *			   Karel Built-in procedures                    *
 *----------------------------------------------------------------------*/
/*!
 */
int k_robot_move(k_robot_t *r);

/*!
 */
int k_robot_pickbeeper(k_robot_t *r);

/*!
 */
int k_robot_putbeeper(k_robot_t *r);

/*!
 */
int k_robot_turnleft(k_robot_t *r);

/*!
 */
int k_robot_turnoff(k_robot_t *r);

#endif
