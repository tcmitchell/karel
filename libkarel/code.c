#include	<stdio.h>
#include	"karel.h"
#include	"y.tab.h"

/* NPROG is size of program; no one would ever write a program this large */
#define	NPROG	500

#define	advpc		++pc
#define	nextinst	prog[advpc]

Inst	prog[NPROG];				/* the machine		*/
int	progp;					/* next spot for code	*/
int	pc;					/* program counter	*/
int	startaddr;				/* start of main block	*/
int	flag;					/* flag for logic tests	*/
int	dest;					/* branch destination	*/

initcode()			/* set up counters, etc., for execution */
{
	progp = 0;
	flag = 0;
	pc = 0;
}

setcode(addr, n)			/* install one program instruction */
int	addr;
Inst	n;
{
	prog[addr] = n;
}

setcodeint(addr, n)				/* install one int */
int	addr, n;
{
	prog[addr] = (Inst) n;
}

code(n)						/* install next instruction */
Inst	n;
{
	if (progp >= NPROG)
		severe("program too big", (char *) 0);
	setcode(progp++, n);
}

codeint(n)				/* install a int as next instruction */
int	n;
{
	code((Inst) n);
}

execute(n)					/* execute machine */
int	n;
{
	int	temp;

	temp = pc;
	for (pc = n; (prog[pc] != RETURN) && state; pc++) {
		(*(prog[pc]))();
		update();
	}
	pc = temp;
}

turnleft()				/* turn karel 90 degrees left */
{
	if (--dir < 0)
		dir = 3;
	placekarel(y, x);
}

branch()				/* jump to another instruction */
{
	dest = (int) nextinst;
	pc = dest - 1;
}

condbranch()			/* jump of last logic test was false */
{
	if (!flag)
		branch();
	else
		advpc;
}

call()					/* call a user-defined instruction */
{
	execute(nextinst);
}

loopexec()			/* execute block of code a number of times */
{
	int	k, limit, loopbody;

	limit = (int) nextinst;
	loopbody = pc + 2;
	for (k = 0; k < limit; k++)
		execute(loopbody);
	branch();
}

turnoff()					/* end program execution */
{
	state = OFF;
}

/* code for built-in logical test */

anybeepers()		{	flag = beepers;			}
facingeast()		{	flag = (dir == 1);		}
facingnorth()		{	flag = (dir == 0);		}
facingsouth()		{	flag = (dir == 2);		}
facingwest()		{	flag = (dir == 3);		}
frontblocked()		{	flag = !sideclear(dir);		}
frontclear()		{	flag = sideclear(dir);		}
leftblocked()		{	flag = !sideclear(dir-1);	}
leftclear()		{	flag = sideclear(dir-1);	}
nobeepers()		{	flag = !beepers;		}
notfacingeast()		{	flag = (dir != 1);		}
notfacingnorth()	{	flag = (dir != 0);		}
notfacingsouth()	{	flag = (dir != 2);		}
notfacingwest()		{	flag = (dir != 3);		}
rightblocked()		{	flag = !sideclear(dir+1);	}
rightclear()		{	flag = sideclear(dir+1);	}

nexttobeeper()
{
	flag = (oldch == '*' || (oldch >= '0' && oldch <= '9'));
}

notnexttobeeper()
{
	nexttobeeper();
	flag = !flag;
}
