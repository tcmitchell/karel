#include <stdio.h>
#include <unistd.h>
#include "karel.h"

int
test_main(int argc, char **argv)
{
  ktr_world_t *world;
  ktr_robot_t *robot;
  
  argc = argc;			/* Keep gcc happy */
  argv = argv;			/* Keep gcc happy */

  fprintf(stdout, "Starting Karel\n");
  world = ktr_world_create(5, 5);
  robot = ktr_robot_create(world, 4, 2, KTR_NORTH, 5);
  ktr_robot_move(robot);
  ktr_robot_turnleft(robot);
  ktr_robot_turnleft(robot);
  ktr_robot_turnleft(robot);
  ktr_robot_pickbeeper(robot);
  if (ktr_world_check_beeper(world, robot->street, robot->avenue))
    printf("Beeper at %d, %d\n", robot->street, robot->avenue);
  else
    printf("No beeper at %d, %d\n", robot->street, robot->avenue);
  ktr_robot_putbeeper(robot);
  if (ktr_world_check_beeper(world, robot->street, robot->avenue))
    printf("Beeper at %d, %d\n", robot->street, robot->avenue);
  else
    printf("No beeper at %d, %d\n", robot->street, robot->avenue);
  ktr_robot_move(robot);
  return 0;
}

void
handle_robot_move_event(ktr_robot_move_event_t *ev)
{
  printf("robot moved from %d, %d to %d, %d\n",
	 ev->old_street, ev->old_avenue,
	 ev->new_street, ev->new_avenue);
}

void
handle_robot_turn_event(ktr_robot_turn_event_t *ev)
{
  printf("robot turned from %s to %s\n",
	 ktr_robot_dir_to_string(ev->old_direction),
	 ktr_robot_dir_to_string(ev->new_direction));
}

int
main(int argc, char **argv)
{
  extern int yyparse(void);
  int c;
  char *prog_file = NULL;
  FILE *fp;
  ktr_engine_t *engine;
  ktr_world_t *world;
  ktr_robot_t *robot;

  while ((c = getopt(argc, argv, "f:")) != EOF)
    {
    }

  if (optind <= argc)
      prog_file = argv[optind];

  if ((fp = fopen(prog_file, "r")) == NULL)
    ktr_fatal_err("can't open file %s", prog_file);

  engine = ktr_load_program (fp);
  world = ktr_world_create(5, 5);
  robot = ktr_robot_create(world, 4, 2, KTR_NORTH, 5);
  ktr_robot_set_move_callback(robot, handle_robot_move_event);
  ktr_robot_set_turn_callback(robot, handle_robot_turn_event);
  
  ktr_engine_execute(engine, robot);

  return 0;
}
