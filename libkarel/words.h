#include "karel.h"
#include "kparse.h"

/* these lists must be sorted alphabetically in order for the	*/
/* binary search routine to work				*/


/* keywords */

struct	{
	char	*name;
	int	keyid;
} keywords[] = {
  { "AS", AS },
  { "BEGIN", BEGIN },
  { "BEGINNING-OF-EXECUTION", BEGEXEC },
  { "BEGINNING-OF-PROGRAM", BEGPROG },
  { "DEFINE-NEW-INSTRUCTION", DEFINST },
  { "DO", DO },
  { "ELSE", ELSE },
  { "END", END },
  { "END-OF-EXECUTION", ENDEXEC },
  { "END-OF-PROGRAM", ENDPROG },
  { "IF", IF },
  { "ITERATE", ITERATE },
  { "THEN", THEN },
  { "TIMES", TIMES },
  { "WHILE", WHILE },
  { 0, 0 }
};


/* built-in procedures and tests */

Bltintype bltins[] = {
  { "any-beepers-in-beeper-bag", k_robot_any_beepers_in_beeper_bag, TEST },
  { "facing-east", k_robot_facing_east, TEST },
  { "facing-north", k_robot_facing_north, TEST },
  { "facing-south", k_robot_facing_south, TEST },
  { "facing-west", k_robot_facing_west, TEST },
  { "front-is-blocked", k_robot_front_is_blocked, TEST },
  { "front-is-clear", k_robot_front_is_clear, TEST },
  { "left-is-blocked", k_robot_left_is_blocked, TEST },
  { "left-is-clear", k_robot_left_is_clear, TEST },
  { "move", k_robot_move, BLTIN },
  { "next-to-a-beeper", k_robot_next_to_a_beeper, TEST },
  { "no-beepers-in-beeper-bag", k_robot_no_beepers_in_beeper_bag, TEST },
  { "not-facing-east", k_robot_not_facing_west, TEST },
  { "not-facing-north", k_robot_not_facing_north, TEST },
  { "not-facing-south", k_robot_not_facing_south, TEST },
  { "not-facing-west", k_robot_not_facing_west, TEST },
  { "not-next-to-a-beeper", k_robot_not_next_to_a_beeper, TEST },
  { "pickbeeper", k_robot_pickbeeper, BLTIN },
  { "putbeeper", k_robot_putbeeper, BLTIN },
  { "right-is-blocked", k_robot_right_is_blocked, TEST },
  { "right-is-clear", k_robot_right_is_clear, TEST },
  { "turnleft", k_robot_turnleft, BLTIN },
  { "turnoff", k_vm_turnoff, BLTIN },
  { 0, 0, 0 }
};
