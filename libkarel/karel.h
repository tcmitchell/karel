/* file name suffixes */
#define	PROGSUFFIX	"k"
#define	SCRSUFFIX	"scr"

/* interpreter states */
#define	OFF	0
#define	COMPILE	1
#define	EDIT	2
#define	RUN	3

typedef	int	(*Inst)();		/* pseudo-compiled instruction */

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
extern	nflg, state;
extern	severe(), err(), interupt(), screrror();
extern	char	*progname, basename[];
extern void interrupt(int arg);


/* in words.h */
extern	Bltintype	bltins[];

/* in symbol.c */
extern	Symbol	*symtab, *lookup();
extern	install();

/* in klex.c */
extern	nkeys, gotsemcolon, gotturnoff, linecount, tokenid, yyval;
extern	yylex(), printdump();
extern	char	yytext[];

/* in code.c */
extern	progp, startaddr, initcode(), setcode(), code(), execute();
extern	anybeepers(), facingeast(), facingnorth(), facingsouth();
extern	facingwest(), frontblocked(), frontclear(), leftblocked();
extern	leftclear(), nexttobeeper(), nobeepers();
extern	notfacingeast(), notfacingnorth(), notfacingsouth(), notfacingwest();
extern	notnexttobeeper(), pickbeeper(), putbeeper(), rightblocked();
extern	rightclear(), turnleft(), turnoff();

/* in scr.c */
extern	beepers, dir, x, y, editscr(), reset(), placekarel(), putbeeper();
extern	pickbeeper(), movekarel(), sideclear(), update(), finish();
extern	readscrn(), initialize(), shutoff();
extern	char	oldch;
