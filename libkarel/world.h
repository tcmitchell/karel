#ifndef __world_h
#define __world_h

typedef struct corner {
  int n_beepers;
  int has_wall_north;
  int has_wall_east;
  int has_wall_south;
  int has_wall_west;
} k_corner_t;

typedef k_corner_t corner_t;

typedef struct world {
  int n_streets;
  int n_avenues;
  k_corner_t ***corners;
} k_world_t;

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

void
k_world_print(k_world_t *w);

#endif
