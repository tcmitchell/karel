/*
  The Karel lexical analyzer.
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

static FILE *ktr_lex_curfile = NULL;
static char ktr_lex_curchar = ' ';
ktr_engine_t *ktr_lex_engine = NULL;

/*----------------------------------------------------------------------*
			       keywords
 *----------------------------------------------------------------------*/

ktr_keyword_t keywords[] = {
  { "AS",                     AS },
  { "BEGIN",                  BEGIN },
  { "BEGINNING-OF-EXECUTION", BEGEXEC },
  { "BEGINNING-OF-PROGRAM",   BEGPROG },
  { "DEFINE-NEW-INSTRUCTION", DEFINST },
  { "DO",                     DO },
  { "ELSE",                   ELSE },
  { "END",                    END },
  { "END-OF-EXECUTION",       ENDEXEC },
  { "END-OF-PROGRAM",         ENDPROG },
  { "IF",                     IF },
  { "ITERATE",                ITERATE },
  { "THEN",                   THEN },
  { "TIMES",                  TIMES },
  { "WHILE",                  WHILE }
};


/* built-in procedures and tests */

ktr_builtin_t bltins[] = {
  { "any-beepers-in-beeper-bag", ktr_robot_any_beepers_in_beeper_bag, TEST },
  { "facing-east",               ktr_robot_facing_east,               TEST },
  { "facing-north",              ktr_robot_facing_north,              TEST },
  { "facing-south",              ktr_robot_facing_south,              TEST },
  { "facing-west",               ktr_robot_facing_west,               TEST },
  { "front-is-blocked",          ktr_robot_front_is_blocked,          TEST },
  { "front-is-clear",            ktr_robot_front_is_clear,            TEST },
  { "left-is-blocked",           ktr_robot_left_is_blocked,           TEST },
  { "left-is-clear",             ktr_robot_left_is_clear,             TEST },
  { "move",                      ktr_robot_move,                      BLTIN },
  { "next-to-a-beeper",          ktr_robot_next_to_a_beeper,          TEST },
  { "no-beepers-in-beeper-bag",  ktr_robot_no_beepers_in_beeper_bag,  TEST },
  { "not-facing-east",           ktr_robot_not_facing_west,           TEST },
  { "not-facing-north",          ktr_robot_not_facing_north,          TEST },
  { "not-facing-south",          ktr_robot_not_facing_south,          TEST },
  { "not-facing-west",           ktr_robot_not_facing_west,           TEST },
  { "not-next-to-a-beeper",      ktr_robot_not_next_to_a_beeper,      TEST },
  { "pickbeeper",                ktr_robot_pickbeeper,                BLTIN },
  { "putbeeper",                 ktr_robot_putbeeper,                 BLTIN },
  { "right-is-blocked",          ktr_robot_right_is_blocked,          TEST },
  { "right-is-clear",            ktr_robot_right_is_clear,            TEST },
  { "turnleft",                  ktr_robot_turnleft,                  BLTIN },
  { "turnoff",                   ktr_engine_turnoff,                  BLTIN }
};

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
/*
  get a character, checking for EOF
*/
static char
egetc(FILE *fp)
{
  char	c;

  if ((c = getc(fp)) == EOF)
    {
      ktr_err_fatal ("unexpected end of program at line %d\nUnterminated comment?", linecount);
      exit (1);
      return((char) 0);		/* Never executed -- keep gcc happy */
    }
  else
    {
      if (c == '\n')
	linecount++;

      return(c);
    }
}

/*
  skip over white space (tabs, etc.)
*/
static void
skipwhite(FILE *fp)
{
  while (isspace((int) ktr_lex_curchar))
    {
      if (ktr_lex_curchar == '\n')
	linecount++;

      ktr_lex_curchar = (char) getc(fp);
    }
}

/*
  find s in keyword array; return -1 if not found.
*/
static int
getkeyid(char *s)
{
  int i;

  for (i=0; i<nkeys; i++)
    if (strcmp(s, keywords[i].name) == 0)
      return i;

  return -1;
}

/*
  find s in builtin array; return -1 if not found.
*/
static int
getbltinid(char *s)
{
  int i;

  for (i=0; i<nbltins; i++)
    if (strcmp(s, bltins[i].name) == 0)
      return i;

  return -1;
}

/*
  Prepare the lexical analyzer.
*/
void
ktr_lex_init(FILE *in_file, ktr_engine_t *engine)
{
  nkeys = (sizeof (keywords) / sizeof(keywords[0]));
  nbltins = (sizeof (bltins) / sizeof(bltins[0]));

  ktr_lex_curfile = in_file;
  ktr_lex_engine = engine;
  linecount = 1;
}

/*
 */
int
ktr_lex_get_progp (void)
{
  return ktr_lex_engine->progp;
}

/*
 */
int
ktr_lex_linecount (void)
{
  return (linecount);
}

/*
 */
void
ktr_lex_startaddr (int addr)
{
  ktr_lex_engine->startaddr = addr;
}

/*
  install one program instruction
*/
void
ktr_lex_setcode (int addr, ktr_instruction_t n)
{
  ktr_engine_setcode (ktr_lex_engine, addr, n);
}

/*
  install one int
*/
void
ktr_lex_setcodeint (int addr, int n)
{
  ktr_engine_setcodeint (ktr_lex_engine, addr, n);
}

/*
  install next instruction
*/
void
ktr_lex_code (ktr_instruction_t n)
{
  ktr_engine_code (ktr_lex_engine, n);
}

/*
  install an int as next instruction
*/
void
ktr_lex_codeint(int n)
{
  ktr_engine_codeint (ktr_lex_engine, n);
}



/*
  Lexical analyzer.
*/
int
yylex (void)
{
  int len;				/* length of word */
  int n;

  skipwhite(ktr_lex_curfile);
  while (ktr_lex_curchar == '{')
    {
      /* skip over comment */
      while (ktr_lex_curchar != '}')
	ktr_lex_curchar = egetc(ktr_lex_curfile);
      ktr_lex_curchar = getc(ktr_lex_curfile);
      skipwhite(ktr_lex_curfile);
    }
  len = 0;

  /* read one word */
  while (isalnum((int) ktr_lex_curchar) || ktr_lex_curchar == '-')
    {
      yytext[len++] = ktr_lex_curchar;
      ktr_lex_curchar = (char) getc(ktr_lex_curfile);
    }

  /* mark end of word */
  yytext[len] = '\0';

  if (len > 0 && ktr_lex_curchar != EOF)
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
      yyval = ktr_lex_curchar;
      ktr_lex_curchar = getc(ktr_lex_curfile);
    }
  return(yyval);
}
