/*
  The Karel public API.
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

#ifndef __karel_h
#define __karel_h

#include <stdio.h>		/* for FILE* in ktr_lex_init */

/*----------------------------------------------------------------------*
			       Defines
 *----------------------------------------------------------------------*/

/**
 *  The maximum size of a Karel program.
 */
#define KTR_MAX_PROG 500

/*----------------------------------------------------------------------*
			       Typedefs
 *----------------------------------------------------------------------*/

/**
 *   All possible states of a Karel engine.
 */
enum ktr_engine_state {
  KTR_ENGINE_EMPTY,
  KTR_ENGINE_COMPILING,
  KTR_ENGINE_READY,
  KTR_ENGINE_RUNNING,
  KTR_ENGINE_DONE
};

/**
 */
typedef enum ktr_engine_state ktr_engine_state_t;

/**
   The direction the robot is pointing.

   FIXME
   Don't change the values, the world definition files depend on them.
   (boo!, hiss!)  [Let's get rid of this dependency at some point.]
 */
enum ktr_direction {
  KTR_NORTH = 1,
  KTR_EAST,
  KTR_SOUTH,
  KTR_WEST
};

/**
 */
typedef enum ktr_direction ktr_direction_t;

/**
 */
struct ktr_corner {
  int n_beepers;
  int has_wall_north;
  int has_wall_east;
  int has_wall_south;
  int has_wall_west;
};

/**
 */
typedef struct ktr_corner ktr_corner_t;

/**
 */
struct ktr_world {
  int n_streets;
  int n_avenues;
  int robot_ave;
  int robot_st;
  int robot_beepers;
  ktr_direction_t robot_direction;
  ktr_corner_t ***corners;
};

/**
 */
typedef struct ktr_world ktr_world_t;

/**
 */
struct ktr_robot_move_event {
  int old_street;
  int old_avenue;
  int new_street;
  int new_avenue;
};

/**
 */
typedef struct ktr_robot_move_event ktr_robot_move_event_t;

/**
 */
struct ktr_robot_turn_event {
  ktr_direction_t old_direction;
  ktr_direction_t new_direction;
};

/**
 */
typedef struct ktr_robot_turn_event ktr_robot_turn_event_t;

/**
 */
typedef	void (*ktr_robot_move_callback_t)(ktr_robot_move_event_t *);

/**
*/
typedef	void (*ktr_robot_turn_callback_t)(ktr_robot_turn_event_t *);

/**
 */
struct ktr_robot {
  int street;
  int avenue;
  int n_beepers;
  ktr_direction_t dir;
  ktr_world_t *world;
  ktr_robot_move_callback_t move_cb;
  ktr_robot_turn_callback_t turn_cb;
};

/**
 */
typedef struct ktr_robot ktr_robot_t;

/*
  forward declare the engine.
*/
struct ktr_engine;

/**
   A pseudo-compiled instruction.
 */
typedef	int (*ktr_instruction_t)(struct ktr_engine *, ktr_robot_t *);

/**
   The Karel execution engine object.
 */
struct ktr_engine {
  ktr_instruction_t prog[KTR_MAX_PROG];
  int progp;			/* The program pointer */
  int pc;			/* The execution pointer */
  int startaddr;		/* The location of main */
  int flag;			/* A flag to hold logic test results */
  ktr_engine_state_t state;	/* The current state of the engine */
};

/**
 */
typedef struct ktr_engine ktr_engine_t;

/*----------------------------------------------------------------------*
			    Error handlers
 *----------------------------------------------------------------------*/
/**
 */
void ktr_err_nomem (size_t size);

/**
 */
void ktr_err_fatal (char *string, ...);

/**
 */
void ktr_err_parse (char *string, ...);

/*----------------------------------------------------------------------*
		      Memory management routines
 *----------------------------------------------------------------------*/
/*
  You can override these, but if you override one, you must override
  them all.
*/

/**
   Allocates nmemb*size bytes of memory.  See man malloc.
 */
void *ktr_calloc (size_t nmemb, size_t size);

/**
   Allocates size bytes of memory.  See man malloc.
*/
void *ktr_malloc (size_t size);

/**
   Frees memory at ptr.  See man free.
 */
void ktr_free (void *ptr);

/**
   Reallocates memory at ptr.  See man realloc.
 */
void *ktr_realloc (void *ptr, size_t size);



/*----------------------------------------------------------------------*
		Functions to manipulate Karel's World
 *----------------------------------------------------------------------*/

/**
 */
ktr_world_t *
ktr_world_create(int n_streets, int n_avenues);

/**
 */
ktr_world_t *
ktr_world_read (FILE *fp);

/**
 */
void
ktr_world_add_ew_wall(ktr_world_t *w,
		    int north_of_street,
		    int at_avenue,
		    int length_to_east);

/**
 */
void
ktr_world_add_ns_wall(ktr_world_t *w,
		    int at_street,
		    int east_of_avenue,
		    int length_to_north);

/**
 */
int
ktr_world_check_ew_wall(ktr_world_t *w, int north_of_street, int at_avenue);

/**
 */
int
ktr_world_check_ns_wall(ktr_world_t *w, int at_street, int east_of_avenue);

/**
 */
int
ktr_world_check_beeper(ktr_world_t *w, int street, int avenue);

/**
 */
int
ktr_world_pick_beeper(ktr_world_t *w, int street, int avenue);

/**
 */
int
ktr_world_put_beeper(ktr_world_t *w, int street, int avenue);

/**
   Creates a new robot at the given position and orientation in the
   given world.
 */
ktr_robot_t *
ktr_robot_create(ktr_world_t *world,
	       int street, int avenue,
	       ktr_direction_t dir, int n_beepers);

/**
 */
void
ktr_robot_set_move_callback(ktr_robot_t *r, ktr_robot_move_callback_t cb);

/**
 */
void
ktr_robot_set_turn_callback(ktr_robot_t *r, ktr_robot_turn_callback_t cb);

/**
 */
void ktr_robot_get_pos(ktr_robot_t *r, int *street, int *avenue);

/**
 */
char *ktr_robot_dir_to_string(ktr_direction_t dir);

/**
 */
ktr_engine_t * ktr_load_program (FILE *in_file);

#endif /*__karel_h */
