#ifndef __error_h
#define __error_h

void
syserr(char *s, char *t);	/* system error: print error message and die */


void
severe(char *s, char *t);		/* print error message and die */


void
err(char *s, char *t);				/* print error message */


void
yyerror(char *s);				/* handle parser error */


void
interrupt(int arg);				/* handle interupts, die */

#endif /* __error_h */
