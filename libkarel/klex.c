#include	<stdio.h>
#include	<ctype.h>
#include	"karel.h"
#include	"karelP.h"
#include	"kparse.h"

int	nkeys, nbltins;			/* number of keywords and bltins */
int	gotsemcolon;
int	gotturnoff;
int	tokenid;			/* token number */
int	linecount;			/* no. of lines in input */
int	yyval;
char	yytext[BUFSIZ];
static	char	c;			/* character being handled */

static FILE *fp = NULL;

/*----------------------------------------------------------------------*
 *			       keywords                                 *
 *
 *----------------------------------------------------------------------*/

ktr_keyword_t keywords[] = {
  { "AS", AS },
  { "BEGIN", BEGIN },
  { "BEGINNING-OF-EXECUTION", BEGEXEC },
  { "BEGINNING-OF-PROGRAM", BEGPROG },
  { "DEFINE-NEW-INSTRUCTION", DEFINST },
  { "DO", DO },
  { "ELSE", ELSE },
  { "END", END },
  { "END-OF-EXECUTION", ENDEXEC },
  { "END-OF-PROGRAM", ENDPROG },
  { "IF", IF },
  { "ITERATE", ITERATE },
  { "THEN", THEN },
  { "TIMES", TIMES },
  { "WHILE", WHILE },
  { 0, 0 }
};


/* built-in procedures and tests */

ktr_builtin_t bltins[] = {
  { "any-beepers-in-beeper-bag", k_robot_any_beepers_in_beeper_bag, TEST },
  { "facing-east", k_robot_facing_east, TEST },
  { "facing-north", k_robot_facing_north, TEST },
  { "facing-south", k_robot_facing_south, TEST },
  { "facing-west", k_robot_facing_west, TEST },
  { "front-is-blocked", k_robot_front_is_blocked, TEST },
  { "front-is-clear", k_robot_front_is_clear, TEST },
  { "left-is-blocked", k_robot_left_is_blocked, TEST },
  { "left-is-clear", k_robot_left_is_clear, TEST },
  { "move", k_robot_move, BLTIN },
  { "next-to-a-beeper", k_robot_next_to_a_beeper, TEST },
  { "no-beepers-in-beeper-bag", k_robot_no_beepers_in_beeper_bag, TEST },
  { "not-facing-east", k_robot_not_facing_west, TEST },
  { "not-facing-north", k_robot_not_facing_north, TEST },
  { "not-facing-south", k_robot_not_facing_south, TEST },
  { "not-facing-west", k_robot_not_facing_west, TEST },
  { "not-next-to-a-beeper", k_robot_not_next_to_a_beeper, TEST },
  { "pickbeeper", k_robot_pickbeeper, BLTIN },
  { "putbeeper", k_robot_putbeeper, BLTIN },
  { "right-is-blocked", k_robot_right_is_blocked, TEST },
  { "right-is-clear", k_robot_right_is_clear, TEST },
  { "turnleft", k_robot_turnleft, BLTIN },
  { "turnoff", k_vm_turnoff, BLTIN },
  { 0, 0, 0 }
};

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
static char
egetc(FILE *fp)			/* get a character, checking for EOF */
{
  char	c;

  if ((c = getc(fp)) == EOF)
    {
      severe("unexpected end of program", NULL);
      return((char) 0);		/* Never executed -- keep gcc happy */
    }
  else
    return(c);
}

static void
skipwhite(FILE *fp)		/* skip over white space (tabs, etc.) */
{
  while (isspace((int) c))
    {
      if (c == '\n')
	linecount++;

      c = (char) getc(fp);
    }
}

void
initlex(FILE *in_file)			/* prepare the lexical analyzer */
{
  /* count the number of keywords */
  for (nkeys = 0; keywords[nkeys].name; nkeys++)
    ;

  /* count the number of builtins */
  for (nbltins = 0; bltins[nbltins].name; nbltins++)
    ;

  /* set the current character */
  c = ' ';

  fp = in_file;
}

int
getkeyid(char *s)	/* find s in keyword array; return -1 if not found */
{
  int i;

  for (i=0; i<nkeys; i++)
    if (strcmp(s, keywords[i].name) == 0)
      return i;

  return -1;
}

int
getkeyid_orig(char *s)	/* find s in keyword array; return -1 if not found */
{
  int	cmp, lower, upper, guess, found;

  /* use a binary search */
  found = lower = 0;
  upper = nkeys;
  while (lower <= upper && !found)
    {
      if (!(cmp = strcmp(s, keywords[guess=(lower+upper)/2].name)))
	{
	  found = 1;
	}
      else
	{
	  if (cmp > 0)
	    {
	      lower = guess + 1;
	    }
	  else
	    {
	      upper = guess - 1;
	    }
	}
    }
  return(found ? guess : -1);
}


/* this is sort of redundant, but I didn't want to pass two kinds of	*/
/* arrays to the same search routine					*/

int
getbltinid(char *s)	/* find s in keyword array; return -1 if not found */
{
  int i = 0;
  for (i=0; i<nbltins; i++)
    {
      if (strcmp(s, bltins[i].name) == 0)
	{
	  return i;
	}
    }
  return -1;
}

int
getbltinid_orig(char *s) /* find s in built-in array; return -1 if not found */
{
  int	cmp, lower, upper, guess, found;

  /* use a binary search */
  found = lower = 0;
  upper = nbltins;
  while (lower <= upper && !found)
    {
      if (!(cmp = strcmp(s, bltins[guess=(lower+upper)/2].name)))
	{
	  found = 1;
	}
      else
	{
	  if (cmp > 0)
	    {
	      lower = guess + 1;
	    }
	  else
	    {
	      upper = guess - 1;
	    }
	}
    }
  return(found ? guess : -1);
}

int
yylex(void)				/* lexical analyzer */
{
  int len;				/* length of word	*/
  int n;				/* temporary		*/

  skipwhite(fp);
  while (c == '{')
    {				/* skip over comment */
      while (c != '}')
	c = egetc(fp);
      c = getc(fp);
      skipwhite(fp);
    }
  len = 0;
  while (isalnum((int) c) || c == '-') 		/* read one word */
    {
      yytext[len++] = c;
      c = (char) getc(fp);
    }
  yytext[len] = '\0';			/* mark end of word */
  if (len > 0 && c != EOF)
    {
      tokenid = getkeyid(yytext);
      if (tokenid >= 0)
	yyval = keywords[tokenid].keyid;
      else
	{
	  tokenid = getbltinid(yytext);
	  if (tokenid >= 0)
	    yyval = bltins[tokenid].type;
	  else
	    yyval = sscanf(yytext, "%d", &n) ? NUMBER:NAME;
	}
    }
  else
    {
      yyval = c;
      c = getc(fp);
    }
  return(yyval);
}
