#include	<stdio.h>
#include	"karel.h"
#include	"karelP.h"
#include	"kparse.h"

/* NPROG is size of program; no one would ever write a program this large */
#define	NPROG	500

#define	advpc		++pc
#define	nextinst	prog[advpc]

static int vm_state = 1;

Inst	prog[NPROG];				/* the machine		*/
int	progp;					/* next spot for code	*/
int	pc;					/* program counter	*/
int	ktr_startaddr;				/* start of main block	*/
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
      return;
    }
  else
    setcode(progp++, n);
}

void
codeint(int n)			/* install an int as next instruction */
{
  code((Inst) n);
}

int
ktr_execute(ktr_robot_t *r, int n)		/* execute machine */
{
  int temp;

  temp = pc;
  for (pc = n; (prog[pc] != RETURN) && vm_state; pc++)
    flag = (*(prog[pc]))(r);

  pc = temp;
  return 0;
}

int
branch(ktr_robot_t *r)			/* jump to another instruction */
{
  r = r;			/* Keep gcc happy */

  dest = (int) nextinst;
  pc = dest - 1;
  return 0;
}

int
condbranch(ktr_robot_t *r)	/* jump of last logic test was false */
{
  r = r;			/* Keep gcc happy */

  if (!flag)
    branch(r);
  else
    advpc;

  return 0;
}

int
call(ktr_robot_t *r)			/* call a user-defined instruction */
{
  return ktr_execute(r, (int) nextinst);
}

int
loopexec(ktr_robot_t *r)		/* execute block of code a number of times */
{
  int k, limit, loopbody;

  limit = (int) nextinst;
  loopbody = pc + 2;
  for (k = 0; k < limit; k++)
    ktr_execute(r, loopbody);

  branch(r);
  return 0;
}

int
ktr_vm_turnoff(ktr_robot_t *r)
{
  r = r;
  vm_state = 0;
  return 0;
}
