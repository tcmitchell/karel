#ifndef __karel_h
#define __karel_h

#include <stdio.h>		/* for FILE* in ktr_initlex */

/*!
  @header Karel
  This is the public API for libkarel.
*/

/*----------------------------------------------------------------------*
			       Defines
 *----------------------------------------------------------------------*/

/*!
  @defined MAX_PROG
  @discussion The maximum size of a Karel program.
 */
#define MAX_PROG 500

/*----------------------------------------------------------------------*
			       Typedefs
 *----------------------------------------------------------------------*/

/*!
  @enum ktr_engine_state_t
  @discussion All possible states of a Karel engine.
*/
typedef enum {
  KTR_ENGINE_EMPTY,
  KTR_ENGINE_COMPILING,
  KTR_ENGINE_READY,
  KTR_ENGINE_RUNNING,
  KTR_ENGINE_DONE
} ktr_engine_state_t;

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

struct ktr_engine;

/*!
  @typedef ktr_instruction_t
  @discussion A pseudo-compiled instruction.
 */
typedef	int (*ktr_instruction_t)(struct ktr_engine *, ktr_robot_t *);

/*!
  @typedef Inst
  @discussion Backwards compatibility.  Deprecated.
 */
typedef ktr_instruction_t Inst;

/*!
  @typedef ktr_engine_t
  @discussion The Karel execution engine object.
 */
typedef struct ktr_engine {
  ktr_instruction_t prog[MAX_PROG];
  int progp;			/* The program pointer */
  int pc;			/* The execution pointer */
  int startaddr;		/* The location of main */
  int flag;			/* A flag to hold logic test results */
  ktr_engine_state_t state;	/* The current state of the engine */
} ktr_engine_t;

/*----------------------------------------------------------------------*
			    Error handlers
 *----------------------------------------------------------------------*/
void ktr_nomem_err (size_t size);

void ktr_fatal_err (char *string, ...);

/*----------------------------------------------------------------------*
		      Memory management routines
 *----------------------------------------------------------------------*/
/*
  You can override these, but if you override one, you must override
  them all.
*/
void *ktr_calloc (size_t nmemb, size_t size);

/*
  Allocates size bytes of memory.  See man malloc.
*/
void *ktr_malloc (size_t size);

/*
  Frees memory at ptr.  See man free.
 */
void ktr_free (void *ptr);

/*
  Reallocates memory at ptr.  See man realloc.
 */
void *ktr_realloc (void *ptr, size_t size);



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

/*!
  @function ktr_robot_create
  @discussion Creates a new robot at the given position and orientation in the
  given world.
 */
ktr_robot_t *
ktr_robot_create(ktr_world_t *world,
	       int street, int avenue,
	       ktr_direction_t dir, int n_beepers);

/*!
  @function ktr_set_move_callback
 */
void
ktr_robot_set_move_callback(ktr_robot_t *r, ktr_robot_move_callback_t cb);

/*!
  @function ktr_set_move_callback
 */
void
ktr_robot_set_turn_callback(ktr_robot_t *r, ktr_robot_turn_callback_t cb);

/*!
  @function ktr_robot_get_pos
 */
void ktr_robot_get_pos(ktr_robot_t *r, int *street, int *avenue);

/*!
  @function ktr_robot_dir_to_string
 */
char *ktr_robot_dir_to_string(ktr_direction_t dir);

/*!
  @function ktr_load_program
 */
ktr_engine_t * ktr_load_program (FILE *in_file);

#endif /*__karel_h */
