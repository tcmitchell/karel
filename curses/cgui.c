#include <unistd.h>	/* for sleep */
#include <curses.h>
#include "karel.h"

static int robot_st = 4;
static int robot_ave = 2;
static k_direction_t robot_dir = K_NORTH;

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
  mvaddch(LINES - st, ave, ' ');
  refresh();
}

static void
draw_robot(int st, int ave, k_direction_t dir)
{
  mvaddch(LINES - st, ave, dir_to_char(dir));
  refresh();
}

static void
handle_robot_move_event(k_robot_move_event_t *ev)
{
  erase_robot(robot_st, robot_ave);
  robot_ave = ev->new_avenue;
  robot_st = ev->new_street;
  draw_robot(robot_st, robot_ave, robot_dir);
  sleep(1);
}

static void
handle_robot_turn_event(k_robot_turn_event_t *ev)
{
  robot_dir = ev->new_direction;
  draw_robot(robot_st, robot_ave, robot_dir);
  sleep(1);
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

  draw_robot(robot_st, robot_ave, robot_dir);
  sleep(1);
  execute(robot, startaddr);

  echo();
  nocrmode();
  endwin();

  return 0;
}
