#include	<stdio.h>
#include	<signal.h>
#include	<curses.h>
#include	"karel.h"
#include	"help.h"

#define		scrnchr(t, s)	(mvinch(t,s) & A_CHARTEXT)

int	cx, cy;					/* cursor coordinates	*/
int	x, y;					/* Karel's x, y coord's	*/
int	dir;					/* Karel's heading	*/
						/* 0 = north		*/
						/* 1 = east		*/
						/* 2 = south		*/
						/* 3 = west		*/
int	beepers;				/* no. of beepers held	*/
int	placed;					/* 1 if Karel is placed	*/
int	clearln;				/* 1 if bottom line	*/
						/* should be cleared	*/
char	oldch;					/* Karel is on top of	*/
char	scrnname[80];				/* name of screen	*/
WINDOW	*helpscrn;				/* editor help screen	*/
WINDOW	*begscrn;				/* screen at start	*/

editscr()		/* main interactive loop */
{
	int	c;				/* keyboard input char	*/
	int	newx, newy;			/* coord's to move to	*/
	int	done;				/* signal to quit	*/

	done = 0;
	clearln = 0;
	c = getch();
	while (c != EOF && c != '\004' && !done) {
		if (c == ERR)
			screrror("getting character");
		/* clear bottom line if necessary */
		if (clearln)
			clearline();
		/* get current location */
		getyx(stdscr, cy, cx);
		newx = cx;
		newy = cy;
		switch (c) {
			case 'i':
				newy--;
				break;
			case 'j':
				newx--;
				break;
			case 'k':
				newx++;
				break;
			case 'm':
				newy++;
				break;
			case 'I':
				dir = 0;
				placekarel(cy, cx);
				break;
			case 'J':
				dir = 3;
				placekarel(cy, cx);
				break;
			case 'K':
				dir = 1;
				placekarel(cy, cx);
				break;
			case 'M':
				dir = 2;
				placekarel(cy, cx);
				break;
			case 'O':
			case 'o':
				placeobj('O');
				break;
			case 'Q':
			case 'q':
				done = 1;
				break;
			case '\r':
			case '\n':
				startprog();
				break;
			case 'S':
				snapshot();
				break;
			case 's':
				savescrn();
				break;
			case '8':
			case ':':
			case '*':
				putbeeper();
				break;
			case ' ':
				placeobj(' ');
				break;
			case '\014':			/* control-L */
				redraw();
				break;
			case '?':
			case '/':
				help();
				break;
		}
		/* check to see if cursor moved off screen */
		if (newx>=1 && newx<COLS && newy>=0 && newy < LINES-2) {
			cy = newy;
			cx = newx;
			move(cy, cx);
		}
		update();
		if (!done)
			c = getch();
	}
	finish();
}

initialize()				/* prepare for screen editing */
{
	int	k;

	if (initscr() == NULL || strcmp(ttytype, "unknown") == 0) {
		fprintf(stderr, "TERM variable not set\n");
		exit(0);
	}
	sprintf(scrnname, "%s.%s", basename, SCRSUFFIX);
	signal(SIGINT, interrupt);
	scrollok(stdscr, FALSE);
	crmode();
	noecho();
	clear();
	/* set up help screen */
	helpscrn = newwin(0, 0, 0, 0);
	wclear(helpscrn);
	wmove(helpscrn, 0, 0);
	for (k = 0; helpmsg[k]; k++) {
		wprintw(helpscrn, helpmsg[k]);
		waddch(helpscrn, '\n');
	}
	wprintw(helpscrn, "\npress any key to continue: ");
	/* make a second screen to save beginning screen for execution */
	begscrn = newwin(0, 0, 0, 0);
	/* draw the boundaries */
	for (k = 0; k < LINES - 2; k++)
		mvaddch(k, 0, '|');
	mvaddch(LINES - 2, 0, '+');
	for (k = 1; k < COLS; k++)
		mvaddch(LINES - 2, k, '-');
	/* place the cursor in the center of the screen */
	cx = COLS / 2;
	cy = LINES / 2;
	placed = 0;
	oldch = ' ';
	move(cy, cx);
	redraw();
}

reset()				/* reset the screen to normal mode */
{
	echo();
	nocrmode();
	endwin();
}

redraw()					/* redraw the screen */
{
	/* force curses to redraw screen by copying screen, then copying back*/
	cpscrn(stdscr, begscrn);
	clear();
	cpscrn(begscrn, stdscr);
	touchwin(stdscr);		/* make sure everything is drawn */
	move(cy, cx);
	update();
}

update()					/* refresh the screen */
{
	if (refresh() == ERR)
		screrror("refreshing screen");
}

placeobj(c)			/* place a wall, beeper, or space */
char	c;
{
	if (cy == y && cx == x)
		placed = 0;
	addch(c);
}

placekarel(newy, newx)		/* put karel on screen at new location */
int	newy, newx;
{
	char	c;			/* symbol karel will be shown as */

	if (placed)
		mvaddch(y, x, oldch);
	else
		placed = 1;
	move(newy, newx);
	oldch = inch();
	switch (dir) {
		case 0:
			c = '^';
			break;
		case 1:
			c = '>';
			break;
		case 2:
			c = 'v';
			break;
		case 3:
			c = '<';
			break;
		default:
			screrror("weird direction");
	}
	addch(c);
	y = newy;
	x = newx;
}

putbeeper()					/* put down one beeper */
{
	char	c;			/* char that cursor is on top of */

	if (state == RUN)
		c = oldch;
	else
		c = inch();
	if (c == '*')
		c = '2';
	else {
		if (c >= '0' && c <= '8')
				c++;
		else {
			if (c != '9')
				c = '*';
			else {
				shutoff("stacked beepers too high");
				return;
			}
		}
	}
	if (state == RUN) {
		if (--beepers < 0)
			shutoff("don't have enough beepers");
		else
			oldch = c;
	}
	else
		placeobj(c);
}

help()					/* print editor help screen */
{
	touchwin(helpscrn);
	if (wrefresh(helpscrn) == ERR)
		screrror("refreshing help screen");
	if (wgetch(helpscrn) == ERR)
		screrror("getting character");
	redraw();
}

savescrn()				/* save the screen to a file */
{
	int	k, j;					/* loop indexes	*/
	FILE	*fp;					/* output file	*/
	FILE	*fopen();

	if ((fp = fopen(scrnname, "w")) == NULL)
		syserr("can't open file: ", scrnname);
	for (k = 0; k < LINES - 2; k++) {
		for (j = 1; j < COLS; j++)
			fputc(scrnchr(k, j), fp);
		fputc('\n', fp);
	}
	fclose(fp);
	prbotln("screen saved");
	clearln = 1;
	move(cy, cx);
}

snapshot()				/* save screen with the borders */
{
	int	k, j;					/* loop indexes	*/
	FILE	*fp;					/* output file	*/
	FILE	*fopen();

	if ((fp = fopen("snapshot", "w")) == NULL)
		syserr("can't open file: snapshot");
	for (k = 0; k < LINES - 2; k++) {
		fputc('|', fp);
		for (j = 1; j < COLS; j++)
			fputc(scrnchr(k, j), fp);
		fputc('\n', fp);
	}
	fputc('+', fp);
	for (k = 1; k < COLS; k++)
		fputc('-', fp);
	fputc('\n', fp);
	fclose(fp);
	prbotln("snapshot saved");
	clearln = 1;
	move(cy, cx);
}

sideclear(n)		/* return 1 if side n is clear, 0 otherwise */
int	n;
{
	int	retval;			/* value to be returned */

	retval = 1;
	/* make sure side-number is between 0 and 3 */
	while (n < 0)
		n += 4;
	n = n % 4;
	switch (n) {
		case 0:
			if (y == 0)
				retval = 0;
			else
				if (scrnchr(y - 1, x) == 'O')
					retval = 0;
			break;
		case 1:
			if (x == COLS - 1)
				retval = 0;
			else
				if (scrnchr(y, x + 1) == 'O')
					retval = 0;
			break;
		case 2:
			if (y == LINES - 3)
				retval = 0;
			else
				if (scrnchr(y + 1, x) == 'O')
					retval = 0;
			break;
		case 3:
			if (x == 1)
				retval = 0;
			else
				if (scrnchr(y, x - 1) == 'O')
					retval = 0;
			break;
	}
	return(retval);
}

movekarel()			/* move karel one character forward */
{
	int	newx, newy;

	newx = x;
	newy = y;
	if (sideclear(dir)) {
		switch (dir) {
			case 0:
				newy--;
				break;
			case 1:
				newx++;
				break;
			case 2:
				newy++;
				break;
			case 3:
				newx--;
				break;
		}
		placekarel(newy, newx);
		cy = newy;
		cx = newx;
	}
	else
		shutoff("hit a wall");
}

finish()			/* clean up; reset terminal modes, etc. */
{
	scrollok(stdscr, TRUE);
	if (move(LINES - 1, 0) == ERR)
		screrror("moving to bottom of screen");
	if (clrtoeol() == ERR)
		screrror("clearing bottom line");
	if (refresh() == ERR)
		screrror("refreshing screen");
	reset();
}

pickbeeper()			/* pick up one beeper underneath karel */
{
	if (oldch == ' ')
		shutoff("tried to pick non-existent beeper");
	else {
		if (oldch == '*')
			oldch = ' ';
		else
			if (oldch == '0')
				oldch = '*';
			else
				oldch--;
		beepers++;
	}
}

readscrn()				/* read screen in from a file */
{
	int	k, j;				/* loop indexes		*/
	FILE	*fp;				/* screen file		*/
	FILE	*fopen();
	char	c;				/* char being read	*/
	char	*kar;				/* these two used to	*/
	char	*kars = "^>v<";			/* place karel		*/
	char	*index();

	if ((fp = fopen(scrnname, "r")) != NULL) {
		for (k = 0; k < LINES - 2; k++) {
			for (j = 1; j < COLS; j++) {
				if ((c = getc(fp)) == EOF)
					break;
				if (c != ' ') {
					if (kar = index(kars, c)) {
						dir = 4 - strlen(kar);
						placekarel(k, j);
					}
					else
						mvaddch(k, j, c);
				}
			}
			getc(fp);			/* ignore newline */
		}
		fclose(fp);
	}
	move(cy, cx);
	update();
}

shutoff(s)				/* print s on bottom of screen */
char	*s;
{
	char	mesg[BUFSIZ];

	state = OFF;
	sprintf(mesg, "\007error shutoff: %.45s.  press any key: ", s);
	prbotln(mesg);
	update();
	if (getch() == ERR)
		screrror("getting character");
	if (move(cy, cx) == ERR)
		screrror("moving cursor to karel");
	clearln = 1;
}

clearline()				/* clear bottom line of screen */
{
	mvbot();
	if (clrtoeol() == ERR)
		screrror("clearing bottom line");
	if (move(cy, cx) == ERR)
		screrror("moving cursor to karel");
	clearln = 0;
}

startprog()		/* prepare for execution, then call program */
{
	int	begx, begy;		/* these are all temporary	*/
	int	begdir;			/* variables to save conditions	*/
	int	begoldch;		/* at start of execution	*/
	int	begbeepers;
	int	c;			/* key user presses		*/

	/* save current status */
	begx = x;
	begy = y;
	begdir = dir;
	begoldch = oldch;
	begbeepers = beepers;
	cpscrn(stdscr, begscrn);
	if (!placed)
		shutoff("karel has not been placed");
	else {
		state = RUN;
		execute(startaddr);
		if (state != OFF)
			shutoff("turnoff instruction never reached");
		if (clearln == 0) {
			mvbot();
			printw("press any key: ");
			update();
			if ((c = getch()) == ERR)
				screrror("getting character");
		}
		else
			clearln = 0;
		clearline();
		/* restore previous status if ESC was not pressed */
		if (c != '\033') {
			x = begx;
			y = begy;
			dir = begdir;
			oldch = begoldch;
			beepers = begbeepers;
			cpscrn(begscrn, stdscr);
		}
		update();
	}
	state = EDIT;
}

cpscrn(a, b)			/* copy screen a to screen b */
WINDOW	*a, *b;
{
	int	k, j;				/* loop indexes */

	for (k = 0; k < LINES - 1; k++)
		for (j = 0; j < COLS; j++)
			mvwaddch(b, k, j, mvwinch(a, k, j));
	touchwin(b);
}

mvbot()					/* move to bottom of screen */
{
	if (move(LINES - 1, 0) == ERR)
		screrror("moving cursor to bottom of screen");
}

prbotln(s)			/* print a line on the bottom of screen */
char	*s;
{
	mvbot();
	if (clrtoeol() == ERR)
		screrror("clearing bottom line");
	printw("%s", s);
	update();
}
