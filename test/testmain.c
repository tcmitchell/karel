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
  
  execute(robot, startaddr);

  return 0;
}