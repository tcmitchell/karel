#include <stdio.h>
#include <signal.h>
#include "karel.h"

static char *progname = "unknown";

void
syserr(char *s, char *t)	/* system error: print error message and die */
{
/*    if (state != COMPILE) */
/*      { */
/*        finish(); */
/*      } */
  fprintf(stderr, "%s: %s", progname, s);
  if (t)
    {
      fprintf(stderr, "%s", t);
    }
  fputc('\n', stderr);
  exit(1);
}

void
severe(char *s, char *t)		/* print error message and die */
{
  fprintf(stderr, "%s", s);
  if (t)
    {
      fprintf(stderr, " %s", t);
    }
  fprintf(stderr, "\ncannot recover from previous errors -- QUIT\n");
  exit(1);
}

void
err(char *s, char *t)				/* print error message */
{
/*    errfound = 1; */
  fprintf(stderr, "\t%s", s);
  if (t)
    {
      fprintf(stderr, " %s", t);
    }
  fprintf(stderr, ", line %d\n", linecount);
}

void
yyerror(char *s)				/* handle parser error */
{
/*    errfound = 1; */
  fprintf(stderr, "\t%s, line %d", s, linecount);
  if (yytext[0])
    {
      fprintf(stderr, " near %s", yytext);
    }
  fputc('\n', stderr);
}

void
interrupt(int arg)				/* handle interupts, die */
{
  arg = arg;			/* Keep gcc happy */
  signal(SIGINT, SIG_IGN);
/*    finish(); */
  fprintf(stderr, "interupt\n");
  exit(0);
}
