#include	<stdio.h>
#include	"karel.h"
#include	"kparse.h"

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

void
initcode(void)			/* set up counters, etc., for execution */
{
  progp = 0;
  flag = 0;
  pc = 0;
}

void
setcode(int addr, Inst n)		/* install one program instruction */
{
  prog[addr] = n;
}

void
setcodeint(int addr, int n)			/* install one int */
{
  prog[addr] = (Inst) n;
}

void
code(Inst n)					/* install next instruction */
{
  if (progp >= NPROG)
    {
      severe("program too big", (char *) 0);
    }
  setcode(progp++, n);
}

void
codeint(int n)				/* install a int as next instruction */
{
  code((Inst) n);
}

void
execute(int n)					/* execute machine */
{
  int temp;

  temp = pc;
  for (pc = n; (prog[pc] != RETURN) && state; pc++)
    {
      (*(prog[pc]))();
      update();
    }
  pc = temp;
}

void
turnleft(void)				/* turn karel 90 degrees left */
{
  if (--dir < 0)
    {
      dir = 3;
    }
  placekarel(y, x);
}

void
branch(void)				/* jump to another instruction */
{
  dest = (int) nextinst;
  pc = dest - 1;
}

void
condbranch(void)			/* jump of last logic test was false */
{
  if (!flag)
    {
      branch();
    }
  else
    {
      advpc;
    }
}

void
call(void)				/* call a user-defined instruction */
{
  execute((int) nextinst);
}

void
loopexec(void)			/* execute block of code a number of times */
{
  int k, limit, loopbody;

  limit = (int) nextinst;
  loopbody = pc + 2;
  for (k = 0; k < limit; k++)
    {
      execute(loopbody);
    }
  branch();
}

void
turnoff(void)					/* end program execution */
{
  state = OFF;
}

/* code for built-in logical test */

void anybeepers(void)		{	flag = beepers;			}
void facingeast(void)		{	flag = (dir == 1);		}
void facingnorth(void)		{	flag = (dir == 0);		}
void facingsouth(void)		{	flag = (dir == 2);		}
void facingwest(void)		{	flag = (dir == 3);		}
void frontblocked(void)		{	flag = !sideclear(dir);		}
void frontclear(void)		{	flag = sideclear(dir);		}
void leftblocked(void)		{	flag = !sideclear(dir-1);	}
void leftclear(void)		{	flag = sideclear(dir-1);	}
void nobeepers(void)		{	flag = !beepers;		}
void notfacingeast(void)	{	flag = (dir != 1);		}
void notfacingnorth(void)	{	flag = (dir != 0);		}
void notfacingsouth(void)	{	flag = (dir != 2);		}
void notfacingwest(void)	{	flag = (dir != 3);		}
void rightblocked(void)		{	flag = !sideclear(dir+1);	}
void rightclear(void)		{	flag = sideclear(dir+1);	}

void
nexttobeeper(void)
{
	flag = (oldch == '*' || (oldch >= '0' && oldch <= '9'));
}

void
notnexttobeeper(void)
{
	nexttobeeper();
	flag = !flag;
}
