/*
  The Karel private API.
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

#ifndef __karelP_h
#define __karelP_h

/*----------------------------------------------------------------------*
			    YACC functions
 *----------------------------------------------------------------------*/
/*!
  @function yylex
  @discussion Lexical analyzer.
 */
int yylex(void);

/*
  handle parser error
*/
void
yyerror(char *s);

/*----------------------------------------------------------------------*
			       Defines
 *----------------------------------------------------------------------*/
/*
  marks end of procedure or main block
*/
#define	KTR_RETURN	(ktr_instruction_t) 0

/*
  marks end of procedure or main block
*/
#define KTR_DEF_INST(x) int x(ktr_engine_t *e, ktr_robot_t *r)

/*!
  @defined KTR_ERROR
  @discussion Error return for a Karel Instruction
*/
#define KTR_ERROR 1

/*!
  @defined KTR_OK
  @discussion Successful return for a Karel Instruction
*/
#define KTR_OK 0

/*----------------------------------------------------------------------*
			       Typedefs
 *----------------------------------------------------------------------*/

/*!
  @typedef ktr_keyword_t
  @discussion An association between a name and a YACC keyword id.
 */
struct ktr_keyword {
  char *name;
  int keyid;
};

typedef struct ktr_keyword ktr_keyword_t;

/*!
  @typedef ktr_builtin_t
  @discussion A triplet of name, function, and type (TEST or BLTIN)
 */
typedef struct ktr_builtin {
  char *name;
  ktr_instruction_t func;
  int type;
} ktr_builtin_t;

/*!
  @abstract representation of a symbol
 */
typedef	struct ktr_symbol {
  char *name;
  int addr;
  struct ktr_symbol *next;
} ktr_symbol_t;

/*----------------------------------------------------------------------*
			   Global Variables
 *----------------------------------------------------------------------*/

/*!
  @discussion A vector of all Karel reserved words
 */
extern ktr_keyword_t keywords[];

/*!
  @discussion A vector of all built-in procedures and tests
 */
extern ktr_builtin_t bltins[];

extern int gotturnoff, linecount, tokenid;

extern char yytext[];


/*----------------------------------------------------------------------*

			 Function Prototypes

 *----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*
			Symbol table functions
 *----------------------------------------------------------------------*/
/*!
 * find s in symbol table
 */
ktr_symbol_t *ktr_symbol_lookup(char *s);

/*!
 * install s in symbol table
 */
void ktr_symbol_install(char *s);

/*----------------------------------------------------------------------*
		      Lexical analyzer functions
 *----------------------------------------------------------------------*/
/*!
  @function ktr_lex_init
  @discussion prepare the lexical analyzer
 */
void ktr_lex_init(FILE *in_file, ktr_engine_t *engine);

/*
 */
int ktr_lex_get_progp (void);

/*
 */
int ktr_lex_linecount (void);

/*
 */
void ktr_lex_startaddr (int addr);

/*
  install one program instruction
*/
void ktr_lex_setcode(int addr, ktr_instruction_t n);

/*
  install next instruction
*/
void ktr_lex_code(ktr_instruction_t n);

/*
  install a int as next instruction
*/
void ktr_lex_codeint(int n);

/*
  install one int
*/
void ktr_lex_setcodeint(int addr, int n);

/*----------------------------------------------------------------------*
		      Execution engine functions
 *----------------------------------------------------------------------*/
/*
  install one program instruction
*/
void ktr_engine_setcode (ktr_engine_t *engine, int addr, ktr_instruction_t n);

/*
  install one int
*/
void ktr_engine_setcodeint (ktr_engine_t *engine, int addr, int n);

/*
  install next instruction
*/
void ktr_engine_code (ktr_engine_t *engine, ktr_instruction_t n);

/*
  install an int as next instruction
*/
void ktr_engine_codeint(ktr_engine_t *engine, int n);

/*----------------------------------------------------------------------*
		     Program flow implementation
 *----------------------------------------------------------------------*/

KTR_DEF_INST(ktr_engine_call);
KTR_DEF_INST(ktr_engine_loopexec);
KTR_DEF_INST(ktr_engine_condbranch);
KTR_DEF_INST(ktr_engine_branch);
KTR_DEF_INST(ktr_engine_turnoff);

/*----------------------------------------------------------------------*
 			   Karel Built-in tests
 *----------------------------------------------------------------------*/
KTR_DEF_INST(ktr_robot_any_beepers_in_beeper_bag);
KTR_DEF_INST(ktr_robot_facing_east);
KTR_DEF_INST(ktr_robot_facing_north);
KTR_DEF_INST(ktr_robot_facing_south);
KTR_DEF_INST(ktr_robot_facing_west);
KTR_DEF_INST(ktr_robot_front_is_blocked);
KTR_DEF_INST(ktr_robot_front_is_clear);
KTR_DEF_INST(ktr_robot_left_is_blocked);
KTR_DEF_INST(ktr_robot_left_is_clear);
KTR_DEF_INST(ktr_robot_next_to_a_beeper);
KTR_DEF_INST(ktr_robot_no_beepers_in_beeper_bag);
KTR_DEF_INST(ktr_robot_not_facing_east);
KTR_DEF_INST(ktr_robot_not_facing_north);
KTR_DEF_INST(ktr_robot_not_facing_south);
KTR_DEF_INST(ktr_robot_not_facing_west);
KTR_DEF_INST(ktr_robot_not_next_to_a_beeper);
KTR_DEF_INST(ktr_robot_right_is_blocked);
KTR_DEF_INST(ktr_robot_right_is_clear);

/*----------------------------------------------------------------------*
 			   Karel Built-in procedures
 *----------------------------------------------------------------------*/

KTR_DEF_INST(ktr_robot_move);
KTR_DEF_INST(ktr_robot_pickbeeper);
KTR_DEF_INST(ktr_robot_putbeeper);
KTR_DEF_INST(ktr_robot_turnleft);

#endif /* __karelP_h */
