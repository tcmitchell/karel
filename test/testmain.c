/*
  A Text based front end to Karel.
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
  char *world_file = NULL;
  FILE *fp;
  ktr_engine_t *engine;
  ktr_world_t *world;
  ktr_robot_t *robot;

  while ((c = getopt(argc, argv, "p:w:")) != EOF)
    {
      switch (c)
	{
	case 'p':
	  prog_file = optarg;
	  break;
	case 'w':
	  world_file = optarg;
	  break;
	}
    }

  if ((fp = fopen (prog_file, "r")) == NULL)
    ktr_err_fatal ("can't open file %s", prog_file);
  engine = ktr_load_program (fp);
  fclose (fp);

  if ((fp = fopen (world_file, "r")) == NULL)
    ktr_err_fatal("can't open file %s", world_file);
  world = ktr_world_read (fp);
  fclose (fp);

  robot = ktr_robot_create(world, 4, 2, KTR_NORTH, 5);
  ktr_robot_set_move_callback(robot, handle_robot_move_event);
  ktr_robot_set_turn_callback(robot, handle_robot_turn_event);
  
  ktr_engine_execute(engine, robot);

  return 0;
}
