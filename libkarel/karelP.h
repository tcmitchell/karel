#ifndef __karelP_h
#define __karelP_h

/* interpreter states */
#define	OFF	0
#define	COMPILE	1
#define	EDIT	2
#define	RUN	3

/*!
  @typedef Inst
  @abstract A pseudo-compiled instruction.
  @discussion The Karel execution engine is made up of these.
 */
typedef	int (*Inst)(k_robot_t *);

/*!
  @typedef ktr_keyword_t
  @discussion An association between a name and a YACC keyword id.
 */
typedef struct ktr_keyword {
  char *name;
  int keyid;
} ktr_keyword_t;

/*!
  @discussion A vector of all Karel reserved words
 */
extern ktr_keyword_t keywords[];

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
  @discussion A vector of all built-in procedures and tests
 */
extern ktr_builtin_t bltins[];

/* marks end of procedure or main block */
#define	RETURN	(Inst) 0

#define DEF_INST(x) int x(k_robot_t *r)

DEF_INST(call);
DEF_INST(loopexec);
DEF_INST(condbranch);
DEF_INST(branch);
DEF_INST(k_vm_turnoff);

extern int progp;
extern void initcode(void);
void setcode(int addr, Inst n);		/* install one program instruction */
void code(Inst n);			/* install next instruction */
void codeint(int n);			/* install a int as next instruction */
void setcodeint(int addr, int n);	/* install one int */

extern int gotturnoff, linecount, tokenid;

extern char yytext[];

/*!
  @defined K_ERROR
  @discussion Error return for a Karel Instruction
*/
#define K_ERROR 1

/*!
  @defined K_OK
  @discussion Successful return for a Karel Instruction
*/
#define K_OK 0

/*----------------------------------------------------------------------*
 *			   Karel Built-in tests                         *
 *----------------------------------------------------------------------*/
int k_robot_any_beepers_in_beeper_bag(k_robot_t *r);
int k_robot_facing_east(k_robot_t *r);
int k_robot_facing_north(k_robot_t *r);
int k_robot_facing_south(k_robot_t *r);
int k_robot_facing_west(k_robot_t *r);
int k_robot_front_is_blocked(k_robot_t *r);
int k_robot_front_is_clear(k_robot_t *r);
int k_robot_left_is_blocked(k_robot_t *r);
int k_robot_left_is_clear(k_robot_t *r);
int k_robot_next_to_a_beeper(k_robot_t *r);
int k_robot_no_beepers_in_beeper_bag(k_robot_t *r);
int k_robot_not_facing_east(k_robot_t *r);
int k_robot_not_facing_north(k_robot_t *r);
int k_robot_not_facing_south(k_robot_t *r);
int k_robot_not_facing_west(k_robot_t *r);
int k_robot_not_next_to_a_beeper(k_robot_t *r);
int k_robot_right_is_blocked(k_robot_t *r);
int k_robot_right_is_clear(k_robot_t *r);

/*----------------------------------------------------------------------*
 *			   Karel Built-in procedures                    *
 *----------------------------------------------------------------------*/
/*!
 */
int k_robot_move(k_robot_t *r);

/*!
 */
int k_robot_pickbeeper(k_robot_t *r);

/*!
 */
int k_robot_putbeeper(k_robot_t *r);

/*!
 */
int k_robot_turnleft(k_robot_t *r);

/*!
 */
int k_robot_turnoff(k_robot_t *r);

/*!
 * representation of a symbol
 */
typedef	struct Symbol {
  char *name;
  int addr;
  struct Symbol *next;
} Symbol;

/*!
 * find s in symbol table
 */
extern Symbol *lookup(char *s);

/*!
 * install s in symbol table
 */
extern void install(char *s);

#endif /* __karelP_h */
