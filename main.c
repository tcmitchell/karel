
/*
 * karel: interpreter for Karel's programming language
 *
 * version 1.0, completed November 6th, 1985
 * by J.T.Miksovsky @ Choate Rosemary Hall
 *
 * N.B.  This is *not* the interpreter that Richard E. Pattis wrote (the one
 * described in his book, "Karel the Robot").  Approval from the Pattis was
 * obtained for this distribution.
 *
 */

#include	<stdio.h>
#include	<signal.h>
#include	<string.h>
#include	"karel.h"

extern int yyparse(void);

int	nflg;			/* -n (no screen load) option flag	*/
int	errfound;		/* 1 if a syntax or lexical error found	*/
int	state;			/* tells if compiling, running, etc.	*/
char	*progname;		/* name of this program 		*/
char	filename[BUFSIZ];	/* name of the source file		*/
char	basename[BUFSIZ];	/* name of source without suffix	*/
char	*usage = "usage: karel [-n] [-b beepers] file.k\n";
FILE	*fp;			/* source file				*/

/* there are many error handlers to handle many type of errors	*/


void
screrror(char *s)			/* reset terminal modes, die */
{
	reset();
	fprintf(stderr, "\nscreen error: %s\n", s);
	exit(1);
}

void
dofiles(char *s)			/* get filename, open files, etc. */
{
  int	suffstart;
  char	suffix[10];

  /* check suffix */
  strcpy(suffix, ".");
  strcat(suffix, PROGSUFFIX);
  suffstart = strlen(s) - strlen(suffix);
  if (strcmp(s + suffstart, suffix) != 0)
    {
      syserr("bad filename: ", s);
    }
  s[suffstart] = '\0';				/* delete suffix */
  strcpy(basename, s);
  sprintf(filename, "%s.%s", basename, PROGSUFFIX);
  if ((fp = fopen(filename, "r")) == NULL)
    {
      syserr("can't open file: ", s);
    }
}

int
main(int argc, char **argv)
{
	progname = *argv;
	state = COMPILE;
	beepers = 0;
	/* process arguments */
	nflg = 0;
	argc--;
	argv++;
	while (argc > 0 && **argv == '-') {
		(*argv)++;
		while (**argv)
			switch (*(*argv)++) {
				case 'b':
					if (**argv == '\0') {
						argc--;
						argv++;
					}
					if (sscanf(*argv, "%d",&beepers) != 1)
						syserr("bad beeper number",0);
					if (beepers < 0)
						syserr("bad beeper number",0);
					**argv = '\0';
					break;
				case 'n':
					nflg = 1;
					break;
				default:
					fprintf(stderr, usage);
					exit(1);
					break;
			}
		argc--;
		argv++;
	}
	if (argc != 1) {
		fprintf(stderr, usage);
		exit(1);
	}
	dofiles(*argv);
	/* set up for parsing */
	errfound = 0;
	gotsemcolon = 0;
	gotturnoff = 0;
	linecount = 1;
	initlex();
	initcode();
	yyparse();
	fclose(fp);
	if (!gotturnoff && !errfound)
		severe("no turnoff instruction found", NULL);
	if (!errfound) {
		fprintf(stderr, "\tno lexical or syntatic errors\n");
		/* prepare to call screen editor */
		state = EDIT;
		initialize();
		if (!nflg)
			readscrn();
		editscr();
	}
	else
		fprintf(stderr,
			"Execution suppressed due to compilation errors\n");
	return 0;
}
