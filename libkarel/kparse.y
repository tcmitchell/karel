/*
  The Karel parser description for bison/yacc.
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

%{

#include	<stdio.h>
#include	<stdlib.h>
#include        <string.h>
#include	"karel.h"
#include	"karelP.h"

  char instname[BUFSIZ];
  ktr_symbol_t *sp;

%}

%start	prog
			/* Karel keywords */

%token	AS	BEGEXEC	BEGIN	BEGPROG	DEFINST	DO	ELSE	END	ENDEXEC
%token	ENDPROG	IF	ITERATE	THEN	TIMES	WHILE

			/* interpreter types */

%token	KEY	BLTIN	TEST	NUMBER	NAME


%%			/* beginning of rules */


prog		: BEGPROG deflist begexec stmtlist ENDEXEC ENDPROG {
			ktr_lex_startaddr($3);
			ktr_lex_code(KTR_RETURN);
		  }
		| prog error
			{ yyerrok; }
		;

begexec		: BEGEXEC {
			strcpy(instname, "");
			fprintf(stderr, "main block:\n");
			$$ = ktr_lex_get_progp ();
		  }
		;

deflist		: def
		| deflist ';' def
		;

def		: /* nothing */
		| definst AS stmt
			{ ktr_lex_code(KTR_RETURN); }
		;

definst		: DEFINST NAME {
			strcpy(instname, yytext);
			fprintf(stderr, "%s:\n", instname);
			ktr_symbol_install(instname);
		  }
		| DEFINST BLTIN
			{ ktr_err_parse ("tried to redefine primitive instruction \"%s\" at line %d.", yytext, ktr_lex_linecount ()); }
		| DEFINST TEST
			{ ktr_err_parse ("tried to redefine logical test \"%s\" at line %d.", yytext, ktr_lex_linecount ()); }
		;

stmtlist	: stmt
		| stmtlist ';' stmt
		;

stmt		: BEGIN stmtlist END
		| IF logictest THEN stmt {
			ktr_lex_setcode($2 + 1, ktr_engine_condbranch);
			ktr_lex_setcodeint($2 + 2, ktr_lex_get_progp ());
		  }
		| IF logictest THEN stmt else stmt {
			ktr_lex_setcode($2 + 1, ktr_engine_condbranch);
			ktr_lex_setcodeint($2 + 2, $5 + 1);
			ktr_lex_setcodeint($5, ktr_lex_get_progp ());
		  }
		| iterate TIMES stmt {
			ktr_lex_code(KTR_RETURN);
			ktr_lex_setcodeint($1, ktr_lex_get_progp ());
		  }
		| WHILE logictest DO stmt {
			ktr_lex_setcode($2 + 1, ktr_engine_condbranch);
			ktr_lex_setcodeint($2 + 2, ktr_lex_get_progp () + 2);
			ktr_lex_code(ktr_engine_branch);
			ktr_lex_codeint($2);
		  }
		| NAME {
			if ((sp = ktr_symbol_lookup(yytext)) == (ktr_symbol_t *) 0)
			  ktr_err_parse ("symbol \"%s\" undefined at line %d.",
					 yytext, ktr_lex_linecount ());
			else {
			  if (strcmp(yytext, instname) == 0)
			    ktr_err_parse ("recursive procedure call %s at line %d.",
					  yytext, ktr_lex_linecount ());
				else {
					ktr_lex_code(ktr_engine_call);
					ktr_lex_codeint(sp->addr);
				}
			}
		  }
		| BLTIN	{
			if (strcmp(yytext, "turnoff") == 0)
				gotturnoff = 1;
			ktr_lex_code(bltins[tokenid].func);
		  }
		| error
		;

logictest	: TEST {
			$$ = ktr_lex_get_progp ();
			ktr_lex_code(bltins[tokenid].func);
			ktr_lex_codeint(0);	/* leave room for branch	*/
			ktr_lex_codeint(0);	/* instruction and address	*/
		  }
		| NAME
			{ ktr_err_parse ("invalid logical test \"%s\" at %d",
					 yytext, ktr_lex_linecount ()); }
		| BLTIN
			{ ktr_err_parse ("invalid logical test \"%s\" at %d",
					 yytext, ktr_lex_linecount ()); }
		;

else		: ELSE {
			ktr_lex_code(ktr_engine_branch);
			$$ = ktr_lex_get_progp ();
			ktr_lex_codeint(0);
		};

iterate		: ITERATE NUMBER {
			ktr_lex_code(ktr_engine_loopexec);
			ktr_lex_codeint(atoi(yytext));
			$$ = ktr_lex_get_progp ();
			ktr_lex_codeint(0);
		  }
		;
