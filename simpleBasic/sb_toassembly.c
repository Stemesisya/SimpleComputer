#include "sb_variables.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define fatal()                                                               \
  if (1)                                                                      \
    {                                                                         \
      fatal = 1;                                                              \
      ap++;                                                                   \
      continue;                                                               \
    }

int
sb_toassembly (int limit)
{

  int haltCount = 0;
  int fatal = 0;

  for (int i = 0; i < MEMORY_SIZE; i++)
    assemblyProgram[i].linkedBasicLine = -1;

  int brokenLineNumbers = 0;
  for (; bp < limit; bp++)
    {
      if (bl == basicProgram[bp].line)
        {
          printf ("Error at %d: Line number should not be repeated.\n", bp);
          brokenLineNumbers = 1;
          fatal ();
        }
      if (bl >= basicProgram[bp].line)
        {
          printf ("Error at %d: Line number should not go downwards.\n", bp);
          brokenLineNumbers = 1;
          fatal ();
        }
      bl = brokenLineNumbers ? bp : basicProgram[bp].line;

      char *operand = basicProgram[bp].operand;

      switch (basicProgram[bp].command->id)
        {
        case 0: // REM
          assemblyProgram[ap].linkedBasicLine = bp;
          assemblyProgram[ap].comment = basicProgram[bp].operand;
          ap++;
          break;
        case 1: // INPUT
          if (1)
            {
              BasicVariable *var = sb_getVariable (operand);
              if (var == NULL)
                fatal ();
              if (operand[1] != '\0')
                {
                  printf ("Error at %d: Expected line terminator. Got '%c'\n",
                          bl, operand[1]);
                  fatal ();
                }

              assemblyProgram[ap].linkedBasicLine = bp;
              assemblyProgram[ap].command = sc_commands + 2; // READ
              assemblyProgram[ap].operand = var->aDefenitionPos;
              assemblyProgram[ap].operandType = Variable;
              ap++;
            }
          break;

        case 2: // PRINT
          if (1)
            {

              BasicVariable *var = sb_getVariable (operand);
              if (var == NULL)
                fatal ();
              if (operand[1] != '\0')
                {
                  printf ("Error at %d: Expected line terminator. Got '%c'\n",
                          bl, operand[1]);
                  fatal ();
                }

              assemblyProgram[ap].linkedBasicLine = bp;
              assemblyProgram[ap].command = sc_commands + 3; // WRITE
              assemblyProgram[ap].operand = var->aDefenitionPos;
              assemblyProgram[ap].operandType = Variable;
              ap++;
            }
          break;
        case 3: // GOTO
          if (1)
            {
              int addr = sb_getAddressFromConstant (operand);
              if (addr < -1)
                fatal ();

              assemblyProgram[ap].linkedBasicLine = bp;
              assemblyProgram[ap].command = sc_commands + 10; // JUMP
              assemblyProgram[ap].operand = addr;
              assemblyProgram[ap].operandType = Raw;
              assemblyProgram[ap].needsFurtherInvestigation = 1;
              ap++;
            }
          break;
        case 4: // IF
          if (1)
            {
              char *sgoto = "GOTO";
              int gotoPos = 0;
              int matchLength = 0;

              // Пытаемся найти GOTO в операнде
              for (int i = 0; operand[i] != '\0'; i++)
                {
                  if (operand[i] == sgoto[matchLength])
                    {
                      matchLength++;
                      if (matchLength == 4)
                        break;
                      continue;
                    }
                  matchLength = 0;
                  i = gotoPos;
                  gotoPos = i + 1;
                }

              if (matchLength == 0)
                {
                  printf ("Error at %d: Expected GOTO keyword\n", bl);
                  fatal ();
                }

              // printf ("[%c] Found GOTO: %.*s GOTO %s\n", operand[gotoPos],
              // gotoPos, operand, operand + gotoPos + 4);

              operand[gotoPos] = '\0';
              int comparisonResult = sb_evaluateComparison (operand);
              operand[gotoPos] = 'G';

              if (comparisonResult < 0)
                fatal ();

              int jumpTo = sb_getAddressFromConstant (operand + gotoPos + 4);
              if (jumpTo < -1)
                fatal ();

              sb_jumpIfAccumulator (comparisonResult, jumpTo);
            }
          break;
        case 5: // LET
          if (1)
            {
              BasicVariable *var = sb_getVariable (operand);
              if (var == NULL)
                fatal ();

              if (operand[1] != '=')
                {
                  printf ("Error at %d: Expected assignment\n", bl);
                  fatal ();
                }

              int result = sb_evaluateExpression (operand + 2);
              if (result < 0)
                fatal ();

              assemblyProgram[ap].linkedBasicLine = bp;
              assemblyProgram[ap].command = sc_commands + 5; // STORE
              assemblyProgram[ap].operand = var->aDefenitionPos;
              ap++;
            }
          break;
        case 6: // END
          if (1)
            {

              if (haltCount >= 1)
                {
                  printf ("Error at %d: Program should have 1 'END' command\n",
                          bl);
                  fatal ();
                }
              haltCount++;

              if (operand[1] != '\0')
                {
                  printf ("Error at %d: Expected line terminator. Got '%c'\n",
                          bl, operand[0]);
                  fatal ();
                }

              assemblyProgram[ap].linkedBasicLine = bp;
              assemblyProgram[ap].command = sc_commands + 13; // HALT
              ap++;
              break;
            }
        }
    }

  // for (int i = 0; i < 26; i++)
  //   {
  //     if (definedVariables[i].aDefenitionPos == 0)
  //       continue;

  //     int vap = definedVariables[i].aDefenitionPos;
  //     char *comment = variableComments[vap];
  //     assemblyProgram[vap].linkedBasicLine
  //         = definedVariables[i].bDefenitionPos;
  //     assemblyProgram[vap].command = &assignmentCommand; // =
  //     assemblyProgram[vap].operand = 0;
  //     assemblyProgram[vap].comment = comment;
  //     comment[0] = 'A' + i;
  //   }

  // for (int i = 0; i < constantsPoolSize; i++)
  //   {
  //     int cap0 = variablesPoolSize + definedConstants[i].aDefenitionPos;
  //     int vap = MEMORY_SIZE - 1 - cap0;
  //     char *comment = variableComments[cap0];
  //     assemblyProgram[vap].linkedBasicLine
  //         = definedVariables[i].bDefenitionPos;
  //     assemblyProgram[vap].command = &assignmentCommand; // =
  //     assemblyProgram[vap].operand = atoi (definedConstants[i].value);
  //     assemblyProgram[vap].comment = comment;
  //     strcpy (comment, definedConstants[i].value);
  //   }

  // for (int i = 0; i < maxTempVariablesCount; i++)
  //   {
  //     int cap0 = variablesPoolSize + constantsPoolSize
  //                + definedConstants[i].aDefenitionPos;
  //     int vap = MEMORY_SIZE - 1 - cap0;
  //     char *comment = variableComments[cap0];
  //     assemblyProgram[vap].linkedBasicLine = 0;
  //     assemblyProgram[vap].command = &assignmentCommand; // =
  //     assemblyProgram[vap].operand = 0;
  //     assemblyProgram[vap].comment = comment;
  //     strcpy (comment, "Tmp");
  //   }

  if (fatal)
    return -1;

  sb_printAss ();

  if (haltCount == 0)
    {
      printf ("Error: Program should have at least 1 'END' command\n");
      return -1;
    }

  return 0;
}
