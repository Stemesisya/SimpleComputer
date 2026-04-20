#include "sb_variables.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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

  for (; bp < limit; bp++)
    {

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
                  printf ("%d: Expected line terminator. Got '%c'\n", bp,
                          operand[1]);
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
                  printf ("%d: Expected line terminator. Got '%c'\n", bp,
                          operand[1]);
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
                  printf ("%d: Expected GOTO keyword\n", bp);
                  fatal ();
                }

              printf ("[%c] Found GOTO: %.*s GOTO %s\n", operand[gotoPos],
                      gotoPos, operand, operand + gotoPos + 4);

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
                  printf ("%d: Expected assignment\n", bp);
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
                  printf ("%d: Program should have 1 'END' command\n", bp);
                  fatal ();
                }
              haltCount++;

              if (operand[1] != '\0')
                {
                  printf ("%d: Expected line terminator. Got '%c'\n", bp,
                          operand[0]);
                  fatal ();
                }

              assemblyProgram[ap].linkedBasicLine = bp;
              assemblyProgram[ap].command = sc_commands + 13; // HALT
              ap++;
              break;
            }
        }
    }

  for (int i = 0; i < 26; i++)
    {
      if (definedVariables[i].aDefenitionPos == 0)
        continue;

      int vap = definedVariables[i].aDefenitionPos;
      char *comment = variableComments[vap];
      assemblyProgram[vap].linkedBasicLine
          = definedVariables[i].bDefenitionPos;
      assemblyProgram[vap].command = &assignmentCommand; // =
      assemblyProgram[vap].operand = 0;
      assemblyProgram[vap].comment = comment;
      comment[0] = 'A' + i;
    }

  for (int i = 0; i < constantsPoolSize; i++)
    {
      int cap0 = variablesPoolSize + definedConstants[i].aDefenitionPos;
      int vap = MEMORY_SIZE - 1 - cap0;
      char *comment = variableComments[cap0];
      assemblyProgram[vap].linkedBasicLine
          = definedVariables[i].bDefenitionPos;
      assemblyProgram[vap].command = &assignmentCommand; // =
      assemblyProgram[vap].operand = atoi (definedConstants[i].value);
      assemblyProgram[vap].comment = comment;
      strcpy (comment, definedConstants[i].value);
    }

  for (int i = 0; i < maxTempVariablesCount; i++)
    {
      int cap0 = variablesPoolSize + constantsPoolSize
                 + definedConstants[i].aDefenitionPos;
      int vap = MEMORY_SIZE - 1 - cap0;
      char *comment = variableComments[cap0];
      assemblyProgram[vap].linkedBasicLine = 0;
      assemblyProgram[vap].command = &assignmentCommand; // =
      assemblyProgram[vap].operand = 0;
      assemblyProgram[vap].comment = comment;
      strcpy (comment, "Tmp");
    }

  if (fatal)
    return -1;

  for (int i = 0; i < MEMORY_SIZE; i++)
    {
      AssemblyCommand c = assemblyProgram[i];
      if (c.command == sc_commands && c.comment == NULL)
        continue;

      printf ("[b%d->a%d]\t%s\t%s:%6d ; %s %s\n", c.linkedBasicLine, i,
              c.command->command,
              c.operandType == Raw
                  ? "Raw"
                  : (c.operandType == Variable
                         ? "Var"
                         : (c.operandType == Constant ? "Int" : "Tmp")),
              c.operand, c.comment,
              c.needsFurtherInvestigation ? "[needsFurtherInvestigation]"
                                          : "");
    }

  if (haltCount == 0)
    {
      printf ("Program should have at least 1 'END' command\n");
      return -1;
    }

  return 0;
}

int
sb_saveAssembly (char *fileName)
{

  int foputput = open (fileName, O_WRONLY | O_CREAT);

  if (foputput == -1)
    {
      printf ("Can't open or create file '%s'.\n", fileName);
      return -3;
    }

  int cell = 0;
  for (int i = 0; i < MEMORY_SIZE; i++)
    {
      sc_memoryGet (i, &cell);
      write (foputput, &cell, sizeof (int));
    }

  close (foputput);
  chmod (fileName, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
  return 0;
}