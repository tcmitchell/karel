#ifndef __karel_h
#define __karel_h

#include <stdio.h>		/* for FILE* in ktr_initlex */

/*!
  @header Karel
  This is the public API for libkarel.
*/

/*----------------------------------------------------------------------*
			       Typedefs
 *----------------------------------------------------------------------*/

/*!
  @typedef ktr_direction_t
  @discussion The direction the robot is pointing.
 */
typedef enum {
  KTR_NORTH = 247,
  KTR_EAST,
  KTR_SOUTH,
  KTR_WEST
} ktr_direction_t;

/*!
  @typedef ktr_corner_t
 */
typedef struct {
  int n_beepers;
  int has_wall_north;
  int has_wall_east;
  int has_wall_south;
  int has_wall_west;
} ktr_corner_t;

/*!
  @typedef ktr_world_t
 */
typedef struct {
  int n_streets;
  int n_avenues;
  ktr_corner_t ***corners;
} ktr_world_t;

/*!
  @typedef ktr_robot_move_event_t
 */
typedef struct {
  int old_street;
  int old_avenue;
  int new_street;
  int new_avenue;
} ktr_robot_move_event_t;

/*!
  @typedef ktr_robot_turn_event_t
 */
typedef struct {
  ktr_direction_t old_direction;
  ktr_direction_t new_direction;
} ktr_robot_turn_event_t;

/*!
  @typedef ktr_robot_move_callback_t
 */
typedef	void (*ktr_robot_move_callback_t)(ktr_robot_move_event_t *);

/*!
  @typedef ktr_robot_turn_callback_t
*/
typedef	void (*ktr_robot_turn_callback_t)(ktr_robot_turn_event_t *);

/*!
  @typedef ktr_robot_t
 */
typedef struct ktr_robot {
  int street;
  int avenue;
  int n_beepers;
  ktr_direction_t dir;
  ktr_world_t *world;
  ktr_robot_move_callback_t move_cb;
  ktr_robot_turn_callback_t turn_cb;
} ktr_robot_t;

/*----------------------------------------------------------------------*
			   Global Variables
 *----------------------------------------------------------------------*/

/* We need to lose this from the public API! --tcm 27-Apr-2000 */

extern int ktr_startaddr;

/*----------------------------------------------------------------------*
		Functions to manipulate Karel's World
 *----------------------------------------------------------------------*/
ktr_world_t *
ktr_world_create(int n_streets, int n_avenues);

void
ktr_world_add_ew_wall(ktr_world_t *w,
		    int north_of_street,
		    int at_avenue,
		    int length_to_east);

void
ktr_world_add_ns_wall(ktr_world_t *w,
		    int at_street,
		    int east_of_avenue,
		    int length_to_north);

int
ktr_world_check_ew_wall(ktr_world_t *w, int north_of_street, int at_avenue);

int
ktr_world_check_ns_wall(ktr_world_t *w, int at_street, int east_of_avenue);

int
ktr_world_check_beeper(ktr_world_t *w, int street, int avenue);

int
ktr_world_pick_beeper(ktr_world_t *w, int street, int avenue);

int
ktr_world_put_beeper(ktr_world_t *w, int street, int avenue);

/*
 * Creates a new robot at the given position and orientation in the
 * given world.
 */
ktr_robot_t *
ktr_robot_create(ktr_world_t *world,
	       int street, int avenue,
	       ktr_direction_t dir, int n_beepers);

void
ktr_robot_set_move_callback(ktr_robot_t *r, ktr_robot_move_callback_t cb);

void
ktr_robot_set_turn_callback(ktr_robot_t *r, ktr_robot_turn_callback_t cb);

/*!
  @function ktr_robot_get_pos
 */
void ktr_robot_get_pos(ktr_robot_t *r, int *street, int *avenue);

char *ktr_robot_dir_to_string(ktr_direction_t dir);

/*!
  @function ktr_execute
  @discussion Execute the Karel machine.
  @param r A robot.
  @param n the instruction to start from (usually 'ktr_startaddr').
*/
int ktr_execute(ktr_robot_t *r, int n);

/*!
  @function ktr_initlex
  @discussion prepare the lexical analyzer
 */
void ktr_initlex(FILE *in_file);


#endif /*__karel_h */
