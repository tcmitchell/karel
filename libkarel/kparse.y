%{

#include	<stdio.h>
#include	<stdlib.h>
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
			{ ktr_parse_err ("tried to redefine primitive instruction \"%s\" at line %d.", yytext, ktr_lex_linecount ()); }
		| DEFINST TEST
			{ ktr_parse_err ("tried to redefine logical test \"%s\" at line %d.", yytext, ktr_lex_linecount ()); }
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
			  ktr_parse_err ("symbol \"%s\" undefined at line %d.",
					 yytext, ktr_lex_linecount ());
			else {
			  if (strcmp(yytext, instname) == 0)
			    ktr_parse_err ("recursive procedure call %s at line %d.",
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
			{ ktr_parse_err ("invalid logical test \"%s\" at %d",
					 yytext, ktr_lex_linecount ()); }
		| BLTIN
			{ ktr_parse_err ("invalid logical test \"%s\" at %d",
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
