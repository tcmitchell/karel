#ifndef __karel_h
#define __karel_h

#include <stdio.h>		/* for FILE* in initlex */

/*!
 */
typedef enum {
  K_NORTH = 247,
  K_EAST,
  K_SOUTH,
  K_WEST
} k_direction_t;

/*----------------------------------------------------------------------*
 *			Structure Definitions                           *
 *----------------------------------------------------------------------*/

/*!
 */
typedef struct corner {
  int n_beepers;
  int has_wall_north;
  int has_wall_east;
  int has_wall_south;
  int has_wall_west;
} k_corner_t;

/*!
 */
typedef struct world {
  int n_streets;
  int n_avenues;
  k_corner_t ***corners;
} k_world_t;

/*!
 */
typedef struct k_robot_move_event {
  int old_street;
  int old_avenue;
  int new_street;
  int new_avenue;
} k_robot_move_event_t;

/*!
 */
typedef struct k_robot_turn_event {
  k_direction_t old_direction;
  k_direction_t new_direction;
} k_robot_turn_event_t;

/*!
 */
typedef	void (*k_robot_move_callback_t)(k_robot_move_event_t *);
typedef	void (*k_robot_turn_callback_t)(k_robot_turn_event_t *);

/*!
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

/*----------------------------------------------------------------------*
			   Global Variables
 *----------------------------------------------------------------------*/

/* We need to lose this from the public API! --tcm 27-Apr-2000 */

/*!
 */
extern int startaddr;

/*----------------------------------------------------------------------*
		Functions to manipulate Karel's World
 *----------------------------------------------------------------------*/
k_world_t *
k_world_create(int n_streets, int n_avenues);

void
k_world_add_ew_wall(k_world_t *w,
		    int north_of_street,
		    int at_avenue,
		    int length_to_east);

void
k_world_add_ns_wall(k_world_t *w,
		    int at_street,
		    int east_of_avenue,
		    int length_to_north);

int
k_world_check_ew_wall(k_world_t *w, int north_of_street, int at_avenue);

int
k_world_check_ns_wall(k_world_t *w, int at_street, int east_of_avenue);

int
k_world_check_beeper(k_world_t *w, int street, int avenue);

int
k_world_pick_beeper(k_world_t *w, int street, int avenue);

int
k_world_put_beeper(k_world_t *w, int street, int avenue);

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

/*!
  @function execute
  @discussion Execute the Karel machine.
  @param r A robot.
  @param n the instruction to start from (usually 'startaddr').
*/
int execute(k_robot_t *r, int n);

extern int yylex(void);			/* lexical analyzer */

void initlex(FILE *in_file);		/* prepare the lexical analyzer */


#endif /*__karel_h */
