#include <unistd.h>
#include <curses.h>
#include <sys/poll.h>
#include "karel.h"

static int robot_st = 3;
static int robot_ave = 4;
static k_direction_t robot_dir = K_NORTH;
static int milli_sleep_time = 500;

static void
k_sleep(int millis)
{
  poll(NULL, 0, millis);
}

static char
dir_to_char(k_direction_t dir)
{
  switch (dir)
    {
    case K_NORTH:
      return '^';

    case K_EAST:
      return '>';

    case K_SOUTH:
      return 'v';

    case K_WEST:
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
draw_robot(int st, int ave, k_direction_t dir)
{
  mvaddch(LINES - (st*2), ave*2, dir_to_char(dir));
  refresh();
}

static void
handle_robot_move_event(k_robot_move_event_t *ev)
{
  erase_robot(robot_st, robot_ave);
  robot_ave = ev->new_avenue;
  robot_st = ev->new_street;
  draw_robot(robot_st, robot_ave, robot_dir);
  k_sleep(milli_sleep_time);
}

static void
handle_robot_turn_event(k_robot_turn_event_t *ev)
{
  robot_dir = ev->new_direction;
  draw_robot(robot_st, robot_ave, robot_dir);
  k_sleep(milli_sleep_time);
}

static void
draw_world(k_world_t *w)
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
	  if (k_world_check_beeper(w, s, a))
	    mvaddch(LINES-(s*2), a*2, '*');
	  else
	    mvaddch(LINES-(s*2), a*2, '+');

	  if (k_world_check_ew_wall(w, s, a))
	    mvaddch(LINES-(s*2)-1, a*2, '-');

	  if (k_world_check_ns_wall(w, s, a))
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
  k_world_put_beeper(world, 3, 3);
  robot = k_robot_create(world, robot_st, robot_ave, robot_dir, 0);
  k_robot_set_move_callback(robot, handle_robot_move_event);
  k_robot_set_turn_callback(robot, handle_robot_turn_event);
  
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
  k_sleep(milli_sleep_time);
  execute(robot, startaddr);

  k_sleep(milli_sleep_time*4);
  echo();
  nocrmode();
  endwin();

  return 0;
}
