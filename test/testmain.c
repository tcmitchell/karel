#include <stdio.h>
#include <unistd.h>
#include "karel.h"

int
test_main(int argc, char **argv)
{
  k_world_t *world;
  k_robot_t *robot;
  
  argc = argc;			/* Keep gcc happy */
  argv = argv;			/* Keep gcc happy */

  fprintf(stdout, "Starting Karel\n");
  world = k_world_create(5, 5);
  robot = k_robot_create(world, 4, 2, K_NORTH, 5);
  k_robot_move(robot);
  k_robot_turnleft(robot);
  k_robot_turnleft(robot);
  k_robot_turnleft(robot);
  k_robot_pickbeeper(robot);
  if (k_world_check_beeper(world, robot->street, robot->avenue))
    printf("Beeper at %d, %d\n", robot->street, robot->avenue);
  else
    printf("No beeper at %d, %d\n", robot->street, robot->avenue);
  k_robot_putbeeper(robot);
  if (k_world_check_beeper(world, robot->street, robot->avenue))
    printf("Beeper at %d, %d\n", robot->street, robot->avenue);
  else
    printf("No beeper at %d, %d\n", robot->street, robot->avenue);
  k_robot_move(robot);
  return 0;
}

void
handle_robot_move_event(k_robot_move_event_t *ev)
{
  printf("robot moved from %d, %d to %d, %d\n",
	 ev->old_street, ev->old_avenue,
	 ev->new_street, ev->new_avenue);
}

void
handle_robot_turn_event(k_robot_turn_event_t *ev)
{
  printf("robot turned from %s to %s\n",
	 k_robot_dir_to_string(ev->old_direction),
	 k_robot_dir_to_string(ev->new_direction));
}

int
main(int argc, char **argv)
{
  extern int yyparse(void);
  int c;
  char *prog_file = NULL;
  FILE *fp;
  k_world_t *world;
  k_robot_t *robot;

  while ((c = getopt(argc, argv, "f:")) != EOF)
    {
    }

  if (optind <= argc)
      prog_file = argv[optind];

  if ((fp = fopen(prog_file, "r")) == NULL)
    syserr("can't open file: ", prog_file);

  initlex(fp);
  yyparse();

  world = k_world_create(5, 5);
  robot = k_robot_create(world, 4, 2, K_NORTH, 5);
  k_robot_set_move_callback(robot, handle_robot_move_event);
  k_robot_set_turn_callback(robot, handle_robot_turn_event);
  
  execute(robot, startaddr);

  return 0;
}
