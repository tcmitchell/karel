#include	"y.tab.h"

/* these lists must be sorted alphabetically in order for the	*/
/* binary search routine to work				*/


/* keywords */

struct	{
	char	*name;
	int	keyid;
} keywords[] = {
	"AS", AS,
	"BEGIN", BEGIN,
	"BEGINNING-OF-EXECUTION", BEGEXEC,
	"BEGINNING-OF-PROGRAM", BEGPROG,
	"DEFINE-NEW-INSTRUCTION", DEFINST,
	"DO", DO,
	"ELSE", ELSE,
	"END", END,
	"END-OF-EXECUTION", ENDEXEC,
	"END-OF-PROGRAM", ENDPROG,
	"IF", IF,
	"ITERATE", ITERATE,
	"THEN", THEN,
	"TIMES", TIMES,
	"WHILE", WHILE,
	0, 0
};


/* built-in procedures and tests */

Bltintype	bltins[] = {
	"any-beepers-in-beeper-bag", anybeepers, TEST,
	"facing-east", facingeast, TEST,
	"facing-north", facingnorth, TEST,
	"facing-south", facingsouth, TEST,
	"facing-west", facingwest, TEST,
	"front-is-blocked", frontblocked, TEST,
	"front-is-clear", frontclear, TEST,
	"left-is-blocked", leftblocked, TEST,
	"left-is-clear", leftclear, TEST,
	"move", movekarel, BLTIN,
	"next-to-a-beeper", nexttobeeper, TEST,
	"no-beepers-in-beeper-bag", nobeepers, TEST,
	"not-facing-east", notfacingeast, TEST,
	"not-facing-north", notfacingnorth, TEST,
	"not-facing-south", notfacingsouth, TEST,
	"not-facing-west", notfacingwest, TEST,
	"not-next-to-a-beeper", notnexttobeeper, TEST,
	"pickbeeper", pickbeeper, BLTIN,
	"putbeeper", putbeeper, BLTIN,
	"right-is-blocked", rightblocked, TEST,
	"right-is-clear", rightclear, TEST,
	"turnleft", turnleft, BLTIN,
	"turnoff", turnoff, BLTIN,
	0, 0, 0
};
