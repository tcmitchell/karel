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
#define	RETURN	(Inst) 0

#define DEF_INST(x) int x(ktr_robot_t *r)

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
  @typedef Inst
  @abstract A pseudo-compiled instruction.
  @discussion The Karel execution engine is made up of these.
 */
typedef	int (*Inst)(ktr_robot_t *);

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
  Inst func;
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
 */
void *emalloc(size_t size);

/*!
 * find s in symbol table
 */
Symbol *lookup(char *s);

/*!
 * install s in symbol table
 */
void install(char *s);

int yylex(void);			/* lexical analyzer */

void
syserr(char *s, char *t);	/* system error: print error message and die */


void
severe(char *s, char *t);		/* print error message and die */


void
err(char *s, char *t);				/* print error message */


void
yyerror(char *s);				/* handle parser error */


DEF_INST(call);
DEF_INST(loopexec);
DEF_INST(condbranch);
DEF_INST(branch);
DEF_INST(ktr_vm_turnoff);

void initcode(void);
void setcode(int addr, Inst n);		/* install one program instruction */
void code(Inst n);			/* install next instruction */
void codeint(int n);			/* install a int as next instruction */
void setcodeint(int addr, int n);	/* install one int */

/*----------------------------------------------------------------------*
 *			   Karel Built-in tests                         *
 *----------------------------------------------------------------------*/
int ktr_robot_any_beepers_in_beeper_bag(ktr_robot_t *r);
int ktr_robot_facing_east(ktr_robot_t *r);
int ktr_robot_facing_north(ktr_robot_t *r);
int ktr_robot_facing_south(ktr_robot_t *r);
int ktr_robot_facing_west(ktr_robot_t *r);
int ktr_robot_front_is_blocked(ktr_robot_t *r);
int ktr_robot_front_is_clear(ktr_robot_t *r);
int ktr_robot_left_is_blocked(ktr_robot_t *r);
int ktr_robot_left_is_clear(ktr_robot_t *r);
int ktr_robot_next_to_a_beeper(ktr_robot_t *r);
int ktr_robot_no_beepers_in_beeper_bag(ktr_robot_t *r);
int ktr_robot_not_facing_east(ktr_robot_t *r);
int ktr_robot_not_facing_north(ktr_robot_t *r);
int ktr_robot_not_facing_south(ktr_robot_t *r);
int ktr_robot_not_facing_west(ktr_robot_t *r);
int ktr_robot_not_next_to_a_beeper(ktr_robot_t *r);
int ktr_robot_right_is_blocked(ktr_robot_t *r);
int ktr_robot_right_is_clear(ktr_robot_t *r);

/*----------------------------------------------------------------------*
 *			   Karel Built-in procedures                    *
 *----------------------------------------------------------------------*/
/*!
 */
int ktr_robot_move(ktr_robot_t *r);

/*!
 */
int ktr_robot_pickbeeper(ktr_robot_t *r);

/*!
 */
int ktr_robot_putbeeper(ktr_robot_t *r);

/*!
 */
int ktr_robot_turnleft(ktr_robot_t *r);

/*!
 */
int ktr_robot_turnoff(ktr_robot_t *r);

#endif /* __karelP_h */
