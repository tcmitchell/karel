/*
  The Karel execution engine, stores and executes a Karel program.
  Copyright (C) 2000 Tom Mitchell

  This file is part of Karel.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*
  Callbacks on engine events (to indicate current source line?)
  Function to call in between instructions (for event loop processing)
  Flag to tell engine to stop executing immediately?
*/

#if STDC_HEADERS
# include "string.h"
#endif /* STDC_HEADERS */

#include "karel.h"
#include "karelP.h"

extern int yyparse();

/*
  Create a Karel engine.
*/
static ktr_engine_t *
ktr_engine_create ()
{
  ktr_engine_t *engine;

  engine = (ktr_engine_t *) ktr_malloc (sizeof (ktr_engine_t));

  memset ((void *) engine, 0, sizeof (ktr_engine_t));

  return (engine);
}

/*
  Loads a program from disk, and returns the resulting engine in a
  ready state.
 */
ktr_engine_t *
ktr_load_program (FILE *in_file)
{
  ktr_engine_t *engine;

  in_file = in_file;
  engine = ktr_engine_create ();

  ktr_initlex(in_file, engine);
  yyparse();

  return engine;
}

/*
 */
static int
ktr_engine_execute_internal (ktr_engine_t *engine, ktr_robot_t *r, int address)
{
  int temp;

  temp = engine->pc;
  for (engine->pc = address;
       ((engine->prog[engine->pc] != RETURN)
	&& engine->state == KTR_ENGINE_RUNNING);
       engine->pc++)
    engine->flag = (*(engine->prog[engine->pc]))(engine, r);

  engine->pc = temp;
  return 0;
}

/*
 */
int
ktr_engine_execute (ktr_engine_t *engine, ktr_robot_t *robot)
{
  engine->state = KTR_ENGINE_RUNNING;
  return ktr_engine_execute_internal (engine, robot, engine->startaddr);
}


ktr_instruction_t
ktr_engine_nextinst (ktr_engine_t *engine)
{
  return engine->prog[++engine->pc];
}

/*----------------------------------------------------------------------*
		      Code installation routines
 *----------------------------------------------------------------------*/
/*
  install one program instruction
*/
void
ktr_engine_setcode (ktr_engine_t *engine, int addr, ktr_instruction_t n)
{
  engine->prog[addr] = n;
}

/*
  install one int
*/
void
ktr_engine_setcodeint (ktr_engine_t *engine, int addr, int n)
{
  engine->prog[addr] = (ktr_instruction_t) n;
}

/*
  install next instruction
*/
void
ktr_engine_code (ktr_engine_t *engine, ktr_instruction_t n)
{
  if (engine->progp >= MAX_PROG)
    {
      ktr_fatal_err ("program too big");
      exit (1);
    }
  else
    ktr_engine_setcode (engine, engine->progp++, n);
}

/*
  install an int as next instruction
*/
void
ktr_engine_codeint(ktr_engine_t *engine, int n)
{
  ktr_engine_code (engine, (ktr_instruction_t) n);
}

/*----------------------------------------------------------------------*
		 Internal program flow implementation
 *----------------------------------------------------------------------*/

/*
  jump to another instruction
*/
int
ktr_engine_branch(ktr_engine_t *engine, ktr_robot_t *r)
{
  int dest;

  r = r;			/* Keep gcc happy */
  dest = (int) ktr_engine_nextinst(engine);
  engine->pc = dest - 1;
  return 0;
}

/*
  jump of last logic test was false
*/
int
ktr_engine_condbranch(ktr_engine_t *engine, ktr_robot_t *r)
{
  r = r;			/* Keep gcc happy */

  if (!engine->flag)
    ktr_engine_branch(engine, r);
  else
    ++engine->pc;

  return 0;
}

/*
  call a user-defined instruction
*/
int
ktr_engine_call(ktr_engine_t *engine, ktr_robot_t *r)
{
  return ktr_engine_execute_internal(engine, r,
				     (int) ktr_engine_nextinst(engine));
}

/*
  execute block of code a number of times
*/
int
ktr_engine_loopexec(ktr_engine_t *engine, ktr_robot_t *r)
{
  int k, limit, loopbody;

  limit = (int) ktr_engine_nextinst(engine);
  loopbody = engine->pc + 2;
  for (k = 0; k < limit; k++)
    ktr_engine_execute_internal (engine, r, loopbody);

  ktr_engine_branch(engine, r);
  return 0;
}

int
ktr_engine_turnoff(ktr_engine_t *engine, ktr_robot_t *r)
{
  r = r;
  engine->state = KTR_ENGINE_DONE;
  return 0;
}
