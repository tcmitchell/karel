#ifndef __karelP_h
#define __karelP_h

/*----------------------------------------------------------------------*
			       Defines
 *----------------------------------------------------------------------*/
/* interpreter states */
#define	OFF	0
#define	COMPILE	1
#define	EDIT	2
#define	RUN	3

/* marks end of procedure or main block */
#define	RETURN	(ktr_instruction_t) 0

#define DEF_INST(x) int x(ktr_engine_t *e, ktr_robot_t *r)

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
typedef struct ktr_keyword {
  char *name;
  int keyid;
} ktr_keyword_t;

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
typedef	struct Symbol {
  char *name;
  int addr;
  struct Symbol *next;
} Symbol;

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

/*!
 */
extern int progp;

extern int gotturnoff, linecount, tokenid;

extern char yytext[];

/*----------------------------------------------------------------------*
			 Function Prototypes
 *----------------------------------------------------------------------*/

/*!
 * find s in symbol table
 */
Symbol *lookup(char *s);

/*!
 * install s in symbol table
 */
void install(char *s);

/*!
  @function ktr_initlex
  @discussion prepare the lexical analyzer
 */
void ktr_initlex(FILE *in_file, ktr_engine_t *engine);

/*!
  @function yylex
  @discussion Lexical analyzer.
 */
int yylex(void);

/*
  system error: print error message and die
*/
void
syserr(char *s, char *t);

/*
  print error message
*/
void
err(char *s, char *t);

/*
  handle parser error
*/
void
yyerror(char *s);

/*
 */
int ktr_lex_get_progp (void);

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

DEF_INST(ktr_engine_call);
DEF_INST(ktr_engine_loopexec);
DEF_INST(ktr_engine_condbranch);
DEF_INST(ktr_engine_branch);
DEF_INST(ktr_engine_turnoff);

/*----------------------------------------------------------------------*
 			   Karel Built-in tests
 *----------------------------------------------------------------------*/
DEF_INST(ktr_robot_any_beepers_in_beeper_bag);
DEF_INST(ktr_robot_facing_east);
DEF_INST(ktr_robot_facing_north);
DEF_INST(ktr_robot_facing_south);
DEF_INST(ktr_robot_facing_west);
DEF_INST(ktr_robot_front_is_blocked);
DEF_INST(ktr_robot_front_is_clear);
DEF_INST(ktr_robot_left_is_blocked);
DEF_INST(ktr_robot_left_is_clear);
DEF_INST(ktr_robot_next_to_a_beeper);
DEF_INST(ktr_robot_no_beepers_in_beeper_bag);
DEF_INST(ktr_robot_not_facing_east);
DEF_INST(ktr_robot_not_facing_north);
DEF_INST(ktr_robot_not_facing_south);
DEF_INST(ktr_robot_not_facing_west);
DEF_INST(ktr_robot_not_next_to_a_beeper);
DEF_INST(ktr_robot_right_is_blocked);
DEF_INST(ktr_robot_right_is_clear);

/*----------------------------------------------------------------------*
 			   Karel Built-in procedures
 *----------------------------------------------------------------------*/

DEF_INST(ktr_robot_move);
DEF_INST(ktr_robot_pickbeeper);
DEF_INST(ktr_robot_putbeeper);
DEF_INST(ktr_robot_turnleft);

#endif /* __karelP_h */
