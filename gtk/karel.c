/* example-start scribble-simple scribble-simple.c */

/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <math.h>
#include <sys/poll.h>
#include <gtk/gtk.h>
#include <karel.h>

typedef enum {
  CELL_TYPE_OPEN = 0,
  CELL_TYPE_WALL
} cell_type_t;

static int robot_st = 3;
static int robot_ave = 4;
static ktr_direction_t robot_dir = KTR_NORTH;
static int milli_sleep_time = 500;
static ktr_engine_t *the_engine;
static ktr_world_t *the_world;
static ktr_robot_t *the_robot;

static gint win_height = 0;

/* Backing pixmap for drawing area */
static GdkPixmap *pixmap = NULL;
static GtkWidget *drawing_area = NULL;

static GdkFont *font = NULL;

static gint scale_x = 10;
static gint translate_x = 0;
static gint cell_width = 10;
static gint half_cell_width = 5;

static gint scale_y = 10;
static gint translate_y = 0;
static gint cell_height = 10;
static gint half_cell_height = 5;

static void
ktr_sleep(int millis)
{
  poll(NULL, 0, millis);
}

/* Transforms an x point from world to screen coordinates */
static gint
w2s(gint street, gint avenue, gint *x1, gint *y1)
{
  (*x1) = (scale_x * avenue) + translate_x;
  (*y1) = (scale_y * street) + translate_y;
  (*y1) = win_height - (*y1);	/* convert to y-up */
  return 0;
}

static gint
gint_min(gint a, gint b)
{
  return (a > b) ? b : a;
}

static void
ktr_draw_char(GdkDrawable *drawable, GdkGC *gc,
	    gint street, gint avenue, gchar glyph)
{
  gint txt_width, txt_height;
  gint txt_x_pos, txt_y_pos;
  gint screen_x, screen_y;
  char glyph_string[2];

  glyph_string[0] = glyph;
  glyph_string[1] = '\0';
  if (font == NULL)
    font = gdk_font_load ("-*-helvetica-bold-r-*-*-*-160-*-*-*-*-*-*");

  w2s(street, avenue, &screen_x, &screen_y);

  txt_width = gdk_string_width(font, glyph_string);
  txt_height = gdk_string_height(font, glyph_string);

  txt_x_pos = screen_x - (txt_width / 2);
  txt_y_pos = screen_y + (txt_height / 2);

  gdk_draw_string (drawable, font, gc,
		   txt_x_pos, txt_y_pos, glyph_string);
}

static void
ktr_drawrobot(GdkDrawable *drawable, GdkGC *gc,
	    gint street, gint avenue, ktr_direction_t direction)
{
  char robot = '?';

  switch (direction)
    {
    case KTR_NORTH:
      robot = '^';
      break;
    case KTR_EAST:
      robot = '>';
      break;
    case KTR_SOUTH:
      robot = 'v';
      break;
    case KTR_WEST:
      robot = '<';
      break;
    default:
      robot = '?';
      break;
    }
      
  ktr_draw_char(drawable, gc, street, avenue, robot);
}

static void
erase_robot ()
{
  gdk_draw_pixmap (drawing_area->window,
		   drawing_area->style->fg_gc[GTK_WIDGET_STATE (drawing_area)],
		   pixmap,
		   0, 0,
		   0, 0,
		   drawing_area->allocation.width,
		   drawing_area->allocation.height);
}

static void
handle_robot_move_event(ktr_robot_move_event_t *ev)
{
  erase_robot();
  robot_ave = ev->new_avenue;
  robot_st = ev->new_street;
  ktr_drawrobot(drawing_area->window, drawing_area->style->black_gc,
	      robot_st, robot_ave, robot_dir);
  while (gtk_events_pending())
    gtk_main_iteration();
  ktr_sleep(milli_sleep_time);
}

static void
handle_robot_turn_event(ktr_robot_turn_event_t *ev)
{
  erase_robot();
  robot_dir = ev->new_direction;
  ktr_drawrobot(drawing_area->window, drawing_area->style->black_gc,
	      robot_st, robot_ave, robot_dir);
  while (gtk_events_pending())
    gtk_main_iteration();
  ktr_sleep(milli_sleep_time);
}

static void
draw_world(ktr_world_t *w, GdkDrawable *drawable)
{
  GdkGC *my_gc;
  int s, a;
  gint x1, y1, x2, y2;

  my_gc = gdk_gc_new(drawable);

  gdk_gc_set_line_attributes (my_gc,
			      3,		  /* line_width */
			      GDK_LINE_SOLID,	  /* line_style */
			      GDK_CAP_PROJECTING, /* cap_style */
			      GDK_JOIN_MITER	  /* join_style */
			      );

  w2s(1, 1, &x1, &y1);
  x1 -= half_cell_width;
  y1 += half_cell_height;

  w2s(w->n_streets, 1, &x2, &y2);
  x2 -= half_cell_width;
  y2 -= half_cell_height;

  /* draw the y axis */
  gdk_draw_line(drawable, my_gc, x1, y1, x2, y2);

  w2s(1, w->n_avenues, &x2, &y2);
  x2 += half_cell_width;
  y2 += half_cell_height;

  /* draw the x axis */
  gdk_draw_line(drawable, my_gc, x1, y1, x2, y2);

  for (s=1; s<=w->n_streets; s++)
    for (a=1; a<=w->n_avenues; a++)
      {
	if (ktr_world_check_ew_wall(w, s, a))
	  {
	    w2s(s, a, &x1, &y1);
	    y1 -= half_cell_height;
	    gdk_draw_line(drawable, my_gc,
			  x1 - half_cell_width, y1,
			  x1 + half_cell_width, y1);
	  }
	if (ktr_world_check_ns_wall(w, s, a))
	  {
	    w2s(s, a, &x1, &y1);
	    x1 += half_cell_height;
	    gdk_draw_line(drawable, my_gc,
			  x1, y1 - half_cell_height,
			  x1, y1 + half_cell_height);
	  }

	/* draw any beepers at this intersection */
	if (ktr_world_check_beeper(w, s, a))
	  ktr_draw_char(drawable, my_gc, s, a, '@');
      }

  gdk_gc_set_line_attributes (my_gc,
			      1,		  /* line_width */
			      GDK_LINE_SOLID,	  /* line_style */
			      GDK_CAP_PROJECTING, /* cap_style */
			      GDK_JOIN_MITER	  /* join_style */
			      );

  for (s=1; s<=w->n_streets; s++)
    {
      w2s(s, 1, &x1, &y1);
      w2s(s, w->n_avenues, &x2, &y2);
      gdk_draw_line(drawable, my_gc,
		    x1 - half_cell_width, y1,
		    x2 + half_cell_width, y2);
    }


  for (a=1; a<=w->n_avenues; a++)
    {
      w2s(1, a, &x1, &y1);
      w2s(w->n_streets, a, &x2, &y2);
      gdk_draw_line(drawable, my_gc,
		    x1, y1 + half_cell_height,
		    x2, y2 - half_cell_height);
    }
}

static void
ktr_drawcell(GdkDrawable *drawable, GdkGC *gc,
	   gint cell_x, gint cell_y, gint cell_type)
{
  gint screen_x, screen_y;

  if (font == NULL)
    font = gdk_font_load ("-*-helvetica-bold-r-*-*-*-160-*-*-*-*-*-*");

  w2s(cell_x, cell_y, &screen_x, &screen_y);

  gdk_draw_rectangle(drawable, gc, (cell_type == CELL_TYPE_WALL),
		     screen_x, screen_y - cell_height,
		     cell_width, cell_height);

  if (cell_x == cell_y)
    {
      char beeper_string[5];
      gint txt_width, txt_height;
      gint txt_x_pos, txt_y_pos;

      gdk_draw_arc (drawable, gc, FALSE,
		    screen_x+2, screen_y - cell_height +2,
		    scale_x-4, scale_y-4,
		    0, 360*64);

      /* We actually want to find the center of the cell and
	 work it from there. */

      sprintf(beeper_string, "%d", 2*cell_x);

      txt_width = gdk_string_width(font, beeper_string);
      txt_height = gdk_string_height(font, beeper_string);

      txt_x_pos = screen_x + (cell_width / 2) - (txt_width / 2);
      txt_y_pos = screen_y - (cell_height / 2) + (txt_height / 2);

      gdk_draw_string (drawable, font, gc,
		       txt_x_pos, txt_y_pos, beeper_string);
    }
}

/* Create a new backing pixmap of the appropriate size */
static gint
ktr_configure(GtkWidget *widget, GdkEventConfigure *event)
{
  event = event;		/* Keep gcc happy */

  win_height = widget->allocation.height;

  scale_x = widget->allocation.width / (the_world->n_avenues + 1);
  scale_y = widget->allocation.height / (the_world->n_streets + 1);

  scale_x = scale_y = gint_min(scale_x, scale_y);

  cell_width = scale_x;
  cell_height = scale_y;

  half_cell_width = cell_width/2;
  half_cell_height = cell_height/2;
  
  if (pixmap)
    gdk_pixmap_unref(pixmap);

  pixmap = gdk_pixmap_new(widget->window,
			  widget->allocation.width,
			  widget->allocation.height,
			  -1);

  gdk_draw_rectangle (pixmap,
		      widget->style->white_gc,
		      TRUE,
		      0, 0,
		      widget->allocation.width,
		      widget->allocation.height);

  draw_world(the_world, pixmap);

  return TRUE;
}

/* Redraw the screen from the backing pixmap */
static gint
expose_event (GtkWidget *widget, GdkEventExpose *event)
{
  gdk_draw_pixmap(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		  pixmap,
		  event->area.x, event->area.y,
		  event->area.x, event->area.y,
		  event->area.width, event->area.height);

  return FALSE;
}

/* Draw a rectangle on the screen */
static void
draw_brush (GtkWidget *widget, gdouble x, gdouble y)
{
  GdkRectangle update_rect;

  update_rect.x = x - 5;
  update_rect.y = y - 5;
  update_rect.width = 10;
  update_rect.height = 10;
  gdk_draw_rectangle (pixmap,
		      widget->style->black_gc,
		      TRUE,
		      update_rect.x, update_rect.y,
		      update_rect.width, update_rect.height);
  gtk_widget_draw (widget, &update_rect);
}

static gint
button_press_event (GtkWidget *widget, GdkEventButton *event)
{
  if (event->button == 1 && pixmap != NULL)
    draw_brush (widget, event->x, event->y);

  return TRUE;
}

static gint
motion_notify_event (GtkWidget *widget, GdkEventMotion *event)
{
  int x, y;
  GdkModifierType state;

  if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
  else
    {
      x = event->x;
      y = event->y;
      state = event->state;
    }
    
  if (state & GDK_BUTTON1_MASK && pixmap != NULL)
    draw_brush (widget, x, y);
  
  return TRUE;
}

void
quit ()
{
  gtk_exit (0);
}

static void
gtk_karel_execute ()
{
  ktr_engine_execute(the_engine, the_robot);
}

int
main (int argc, char *argv[])
{
  FILE *fp;
  char *prog_file;
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *button;

  gtk_init (&argc, &argv);

  if (argc < 2)
    {
      printf("no karel program specified\n");
      exit(1);
    }

  prog_file = argv[1];
  if ((fp = fopen(prog_file, "r")) == NULL)
    {
      ktr_fatal_err("can't open file %s", prog_file);
      exit(1);
    }

  the_engine = ktr_load_program(fp);
  the_world = ktr_world_create(5, 5);
  ktr_world_put_beeper(the_world, 3, 3);
  the_robot = ktr_robot_create(the_world, robot_st, robot_ave, robot_dir, 0);
  ktr_robot_set_move_callback(the_robot, handle_robot_move_event);
  ktr_robot_set_turn_callback(the_robot, handle_robot_turn_event);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (window, "Test Input");

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC (quit), NULL);

  /* Create the drawing area */

  drawing_area = gtk_drawing_area_new ();
  gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 200, 200);
  gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

  gtk_widget_show (drawing_area);

  /* Signals used to handle backing pixmap */

  gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
		      (GtkSignalFunc) expose_event, NULL);

  gtk_signal_connect (GTK_OBJECT(drawing_area),"configure_event",
		      (GtkSignalFunc) ktr_configure, NULL);

  /* Event signals */

  gtk_signal_connect (GTK_OBJECT (drawing_area), "motion_notify_event",
		      (GtkSignalFunc) motion_notify_event, NULL);
  gtk_signal_connect (GTK_OBJECT (drawing_area), "button_press_event",
		      (GtkSignalFunc) button_press_event, NULL);

  gtk_widget_set_events (drawing_area, GDK_EXPOSURE_MASK
			 | GDK_LEAVE_NOTIFY_MASK
			 | GDK_BUTTON_PRESS_MASK
			 | GDK_POINTER_MOTION_MASK
			 | GDK_POINTER_MOTION_HINT_MASK);

  /* .. And a quit button */
  button = gtk_button_new_with_label ("Quit");
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (window));
  gtk_widget_show (button);

  button = gtk_button_new_with_label ("Execute");
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     GTK_SIGNAL_FUNC (gtk_karel_execute), NULL);
  gtk_widget_show (button);

  gtk_widget_show (window);

  gtk_main ();

  return 0;
}
/* example-end */
