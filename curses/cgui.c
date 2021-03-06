/*
  A Curses based front end to Karel.
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

#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
#include <sys/poll.h>
#include "karel.h"

static int robot_st = 3;
static int robot_ave = 4;
static ktr_direction_t robot_dir = KTR_NORTH;
static int milli_sleep_time = 500;

static void
ktr_sleep(int millis)
{
  poll(NULL, 0, millis);
}

static char
dir_to_char(ktr_direction_t dir)
{
  switch (dir)
    {
    case KTR_NORTH:
      return '^';

    case KTR_EAST:
      return '>';

    case KTR_SOUTH:
      return 'v';

    case KTR_WEST:
      return '<';

    default:
      return '?';
    }
}

static void
erase_robot(int st, int ave)
{
  mvaddch(LINES - (st*2), ave*2, '+');
  refresh();
}

static void
draw_robot(int st, int ave, ktr_direction_t dir)
{
  mvaddch(LINES - (st*2), ave*2, dir_to_char(dir));
  refresh();
}

static void
handle_robot_move_event(ktr_robot_move_event_t *ev)
{
  erase_robot(robot_st, robot_ave);
  robot_ave = ev->new_avenue;
  robot_st = ev->new_street;
  draw_robot(robot_st, robot_ave, robot_dir);
  ktr_sleep(milli_sleep_time);
}

static void
handle_robot_turn_event(ktr_robot_turn_event_t *ev)
{
  robot_dir = ev->new_direction;
  draw_robot(robot_st, robot_ave, robot_dir);
  ktr_sleep(milli_sleep_time);
}

static void
draw_world(ktr_world_t *w)
{
  int s, a;

  for (a=1; a<=w->n_avenues; a++)
    {
      mvaddch(LINES-1, a*2-1, '-');
      mvaddch(LINES-1, a*2, '-');
    }

  for (s=1; s<=w->n_streets; s++)
    {
      mvaddch(LINES-(s*2), 1, '|');
      for (a=1; a<=w->n_avenues; a++)
	{
	  if (ktr_world_check_beeper(w, s, a))
	    mvaddch(LINES-(s*2), a*2, '*');
	  else
	    mvaddch(LINES-(s*2), a*2, '+');

	  if (ktr_world_check_ew_wall(w, s, a))
	    mvaddch(LINES-(s*2)-1, a*2, '-');

	  if (ktr_world_check_ns_wall(w, s, a))
	    mvaddch(LINES-(s*2), a*2+1, '|');
	}
    }
  refresh();
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

	default:
	  fprintf (stderr, "Unknown argument %c\n", c);
	  exit (1);
	  break;
	}
    }

  if (prog_file == NULL || world_file == NULL)
    {
      fprintf (stderr, "Must specify -p program -w world\n");
      exit (1);
    }

  if ((fp = fopen(prog_file, "r")) == NULL)
    {
      ktr_err_fatal("can't open file %s", prog_file);
      exit (1);
    }
  engine = ktr_load_program (fp);
  fclose (fp);

  if ((fp = fopen(world_file, "r")) == NULL)
    {
      ktr_err_fatal("can't open file %s", world_file);
    }
  world = ktr_world_read (fp);
  fclose (fp);

  robot = ktr_robot_create(world, robot_st, robot_ave, robot_dir, 0);
  ktr_robot_set_move_callback(robot, handle_robot_move_event);
  ktr_robot_set_turn_callback(robot, handle_robot_turn_event);

  if (initscr() == NULL)
    {
      perror("initscr");
      return -1;
    }

  scrollok(stdscr, FALSE);
  crmode();
  noecho();
  clear();

  draw_world(world);
  draw_robot(robot_st, robot_ave, robot_dir);
  ktr_sleep(milli_sleep_time);
  ktr_engine_execute(engine, robot);

  ktr_sleep(milli_sleep_time*4);
  echo();
  nocrmode();
  endwin();

  return 0;
}
