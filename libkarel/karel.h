/* file name suffixes */
#define	PROGSUFFIX	"k"
#define	SCRSUFFIX	"scr"

/* interpreter states */
#define	OFF	0
#define	COMPILE	1
#define	EDIT	2
#define	RUN	3

typedef	void	(*Inst)(void);		/* pseudo-compiled instruction */

/* marks end of procedure or main block */
#define	RETURN	(Inst) 0

typedef struct	Bltintype {		/* built-in procedure entry */
	char	*name;
	Inst	func;
	int	type;
} Bltintype;

typedef	struct	Symbol {		/* symbol table entry */
	char	*name;
	int	addr;
	struct	Symbol	*next;
} Symbol;

/* in main.c */
extern int nflg;
extern int state;
extern void severe(char *s, char *t);	/* print error message and die */
extern void err(char *s, char *t);	/* print error message */
extern void interrupt(int arg);		/* handle interupts, die */
extern void screrror(char *s);		/* reset terminal modes, die */
void syserr(char *s, char *t);	/* system error: print error message and die */
extern char *progname;
extern char basename[];


/* in words.h */
extern	Bltintype	bltins[];

/* in symbol.c */
extern Symbol *symtab;
extern Symbol *lookup(char *s);		/* find s in symbol table */
extern void install(char *s);		/* install s in symbol table */

/* in klex.c */
extern int nkeys, gotsemcolon, gotturnoff, linecount, tokenid, yyval;
extern int yylex(void);			/* lexical analyzer */
extern char yytext[];
void initlex(void);			/* prepare the lexical analyzer */


/* in code.c */
extern int progp, startaddr;
extern void initcode(void);
void setcode(int addr, Inst n);		/* install one program instruction */
void code(Inst n);			/* install next instruction */
void execute(int n);			/* execute machine */
extern void anybeepers(), facingeast(), facingnorth(), facingsouth();
extern void facingwest(), frontblocked(), frontclear(), leftblocked();
extern void leftclear(), nexttobeeper(), nobeepers();
extern void notfacingeast(), notfacingnorth(), notfacingsouth(), notfacingwest();
extern void notnexttobeeper(), rightblocked();
extern void rightclear(), turnleft(), turnoff();
extern void condbranch(), branch(), call(), bltin(), loopexec();
void codeint(int n);			/* install a int as next instruction */
void setcodeint(int addr, int n);	/* install one int */

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
