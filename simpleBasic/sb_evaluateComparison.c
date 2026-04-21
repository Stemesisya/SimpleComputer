#include "sb_variables.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define createSeparator()                                                     \
  lastSym = expression[i];                                                    \
  lastSymAt = i;                                                              \
  expression[i] = '\0'

#define evalLeft()                                                            \
  if (sb_evaluateExpression (expression) != 0)                                \
    return -1;                                                                \
  // printf ("Successfully evaluated Left expression.\n")

#define evalRight()                                                           \
  if (sb_evaluateExpression (expression + i + 1) != 0)                        \
    return -1;                                                                \
  // printf ("Successfully evaluated Right expression.\n")

#define saveToTemporary()                                                     \
  int tmpVarAddr = sb_allocateTemporaryVariable ();                           \
  assemblyProgram[ap].linkedBasicLine = bp;                                   \
  assemblyProgram[ap].command = sc_commands + 5 /*STORE*/;                    \
  assemblyProgram[ap].operand = tmpVarAddr;                                   \
  assemblyProgram[ap].operandType = Temporary;                                \
  assemblyProgram[ap].needsFurtherInvestigation = 1;                          \
  ap++

#define substract()                                                           \
  assemblyProgram[ap].linkedBasicLine = bp;                                   \
  assemblyProgram[ap].command = sc_commands + 7 /*SUB*/;                      \
  assemblyProgram[ap].operand = tmpVarAddr;                                   \
  assemblyProgram[ap].operandType = Temporary;                                \
  assemblyProgram[ap].needsFurtherInvestigation = 1;                          \
  ap++;                                                                       \
  sb_freeTemporaryVariable (tmpVarAddr);                                      \
  expression[lastSymAt] = lastSym

int
sb_evaluateComparison (char *expression)
{
  char lastSym = 0;
  int lastSymAt = 0;

  for (int i = 0; expression[i] != '\0'; i++)
    {
      switch (expression[i])
        {
        case '>':
          if (1)
            {
              // printf ("Is '>' expression.\n");

              // Left > Right   ===   Right - Left < 0

              createSeparator ();

              evalRight ();
              saveToTemporary ();
              evalLeft ();
              substract ();

              return 1;
            }
        case '<':
          if (1)
            {
              // printf ("Is '<' expression.\n");

              // Left < Right   ===   Left - Right < 0

              createSeparator ();

              evalLeft ();
              saveToTemporary ();
              evalRight ();
              substract ();

              return 2;
            }
        case '=':
          if (1)
            {
              // printf ("Is '=' expression.\n");

              // Left == Right   ===   Left - Right == 0

              createSeparator ();

              evalLeft ();
              saveToTemporary ();
              evalRight ();
              substract ();

              return 3;
            }
        }
    }
  printf ("Error at %d: Expected expression.\n", bl);
  return -1;
}

// 1 - >
// 2 - <
// 3 - =
int
sb_jumpIfAccumulator (int expressionType, int to)
{
  switch (expressionType)
    {
    case 1:
    case 2:
      assemblyProgram[ap].linkedBasicLine = bp;
      assemblyProgram[ap].command = sc_commands + 11; // JNEG
      assemblyProgram[ap].operand = to;
      assemblyProgram[ap].operandType = Raw;
      assemblyProgram[ap].needsFurtherInvestigation = 1;
      ap++;
      break;
    case 3:
      assemblyProgram[ap].linkedBasicLine = bp;
      assemblyProgram[ap].command = sc_commands + 12; // JZ
      assemblyProgram[ap].operand = to;
      assemblyProgram[ap].operandType = Raw;
      assemblyProgram[ap].needsFurtherInvestigation = 1;
      ap++;
      break;
    default:
      return -1;
    }
  return 0;
}