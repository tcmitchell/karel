#include	<stdio.h>
#include	<ctype.h>
#include	"karel.h"
#include	"words.h"

int	nkeys, nbltins;			/* number of keywords and bltins */
int	gotsemcolon;
int	gotturnoff;
int	tokenid;			/* token number */
int	linecount;			/* no. of lines in input */
int	yyval;
char	yytext[BUFSIZ];
static	char	c;			/* character being handled */
extern	FILE	*fp;			/* source file; found in main.c */

initlex()				/* prepare the lexical analyzer */
{
	/* count the number of keywords */
	for (nkeys = 0; keywords[nkeys].name; nkeys++)
		;
	for (nbltins = 0; bltins[nbltins].name; nbltins++)
		;
	c = ' ';
}

char	egetc(fp)			/* get a character, checking for EOF */
FILE	*fp;
{
	char	c;

	if ((c = getc(fp)) == EOF)
		severe("unexpected end of program");
	else
		return(c);
}

yylex()				/* lexical analyzer */
{
	int	len;				/* length of word	*/
	int	n;				/* temporary		*/

	skipwhite();
	while (c == '{') {				/* skip over comment */
		while (c != '}')
			c = egetc(fp);
		c = getc(fp);
		skipwhite();
	}
	len = 0;
	while (isalnum(c) || c == '-') {		/* read one word */
		yytext[len++] = c;
		c = getc(fp);
	}
	yytext[len] = '\0';			/* mark end of word */
	if (len > 0 && c != EOF) {
		tokenid = getkeyid(yytext);
		if (tokenid >= 0)
			yyval = keywords[tokenid].keyid;
		else {
			tokenid = getbltinid(yytext);
			if (tokenid >= 0)
				yyval = bltins[tokenid].type;
			else
				yyval = sscanf(yytext, "%d", &n) ? NUMBER:NAME;
		}
	}
	else {
		yyval = c;
		c = getc(fp);
	}
	return(yyval);
}

getkeyid(s)	/* find s in keyword array; return -1 if not found */
char	*s;
{
	int	cmp, lower, upper, guess, found;

	/* use a binary search */
	found = lower = 0;
	upper = nkeys;
	while (lower <= upper && !found) {
		if (!(cmp = strcmp(s, keywords[guess=(lower+upper)/2].name)))
			found = 1;
		else
			if (cmp > 0)
				lower = guess + 1;
			else
				upper = guess - 1;
	}
	return(found ? guess : -1);
}


/* this is sort of redundant, but I didn't want to pass two kinds of	*/
/* arrays to the same search routine					*/


getbltinid(s)		/* find s in built-in array; return -1 if not found */
char	*s;
{
	int	cmp, lower, upper, guess, found;

	/* use a binary search */
	found = lower = 0;
	upper = nbltins;
	while (lower <= upper && !found) {
		if (!(cmp = strcmp(s, bltins[guess=(lower+upper)/2].name)))
			found = 1;
		else
			if (cmp > 0)
				lower = guess + 1;
			else
				upper = guess - 1;
	}
	return(found ? guess : -1);
}

skipwhite()			/* skip over white space (tabs, etc.) */
{
 	while (isspace(c)) {
		if (c == '\n')
			linecount++;
		c = getc(fp);
	}
}
