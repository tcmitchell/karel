%{

#include	<stdio.h>
#include	<stdlib.h>
#include	"karel.h"
#include	"karelP.h"

  char instname[BUFSIZ];
  Symbol *sp;

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
			ktr_lex_code(RETURN);
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
			{ ktr_lex_code(RETURN); }
		;

definst		: DEFINST NAME {
			strcpy(instname, yytext);
			fprintf(stderr, "%s:\n", instname);
			install(instname);
		  }
		| DEFINST BLTIN
			{ err("tried to redefine primitive instruction:",
								yytext); }
		| DEFINST TEST
			{ err("tried to redefine logical test:", yytext); }
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
			ktr_lex_code(RETURN);
			ktr_lex_setcodeint($1, ktr_lex_get_progp ());
		  }
		| WHILE logictest DO stmt {
			ktr_lex_setcode($2 + 1, ktr_engine_condbranch);
			ktr_lex_setcodeint($2 + 2, ktr_lex_get_progp () + 2);
			ktr_lex_code(ktr_engine_branch);
			ktr_lex_codeint($2);
		  }
		| NAME {
			if ((sp = lookup(yytext)) == (Symbol *) 0)
				err(yytext, "undefined");
			else {
				if (strcmp(yytext, instname) == 0)
					err("recursive procedure call:",
								yytext);
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
			{ err("invalid logical test:", yytext); }
		| BLTIN
			{ err("invalid logical test:", yytext); }
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
