#ifndef __karel_h
#define __karel_h

#include <stdio.h>
#include "error.h"
#include "robot.h"

/* file name suffixes */
#define	PROGSUFFIX	"k"
#define	SCRSUFFIX	"scr"

/* interpreter states */
#define	OFF	0
#define	COMPILE	1
#define	EDIT	2
#define	RUN	3

#define DEF_INST(x) int x(k_robot_t *r)

typedef	int (*Inst)(k_robot_t *);	/* pseudo-compiled instruction */

/* marks end of procedure or main block */
#define	RETURN	(Inst) 0

typedef struct	Bltintype {		/* built-in procedure entry */
	char	*name;
	Inst	func;
	int	type;
} Bltintype;

/* in robot.c */
extern k_robot_t *the_robot;

/* in main.c */
extern int nflg;
extern int state;
extern void screrror(char *s);		/* reset terminal modes, die */
extern char *progname;
extern char basename[];


/* in words.h */
extern	Bltintype	bltins[];

/* in klex.c */
extern int nkeys, gotsemcolon, gotturnoff, linecount, tokenid, yyval;
extern int yylex(void);			/* lexical analyzer */
extern char yytext[];
void initlex(FILE *in_file);		/* prepare the lexical analyzer */


/* in code.c */
extern int progp, startaddr;
extern void initcode(void);
void setcode(int addr, Inst n);		/* install one program instruction */
void code(Inst n);			/* install next instruction */
extern void anybeepers(), facingeast(), facingnorth(), facingsouth();
extern void facingwest(), frontblocked(), frontclear(), leftblocked();
extern void leftclear(), nexttobeeper(), nobeepers();
extern void notfacingeast(), notfacingnorth(), notfacingsouth(), notfacingwest();
extern void notnexttobeeper(), rightblocked();
extern void rightclear(), turnleft(), turnoff();
extern void bltin();
void codeint(int n);			/* install a int as next instruction */
void setcodeint(int addr, int n);	/* install one int */

int execute(k_robot_t *r, int n);		/* execute machine */
DEF_INST(call);
DEF_INST(loopexec);
DEF_INST(condbranch);
DEF_INST(branch);
DEF_INST(k_vm_turnoff);

/* in scr.c */
extern int beepers, dir, x, y;
extern char oldch;
void movekarel(void);		/* move karel one character forward */
void putbeeper(void);		/* put down one beeper */
void pickbeeper(void);		/* pick up one beeper underneath karel */
void initialize(void);		/* prepare for screen editing */
void reset(void);		/* reset the screen to normal mode */
void update(void);		/* refresh the screen */
void placekarel(int newy, int newx);  /* put karel on screen at new location */
void finish(void);		/* clean up; reset terminal modes, etc. */
void readscrn(void);		/* read screen in from a file */
void shutoff(char *s);		/* print s on bottom of screen */
void editscr(void);		/* main interactive loop */
int sideclear(int n);		/* return 1 if side n is clear, 0 otherwise */

#endif /*__karel_h */
