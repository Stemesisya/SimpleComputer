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
sb_translate (int limit)
{

  int fatal = 0;
  int bp = 0;
  int ap = 0;

  Command *commands = sc_getCommands ();

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
              BasicVariable *var = sc_getVariable (operand, 0, bp);
              if (var == NULL)
                fatal ();
              if (operand[1] != '\0')
                {
                  printf ("%d: Expected line terminator. Got '%c'\n", bp,
                          operand[1]);
                  fatal ();
                }

              assemblyProgram[ap].linkedBasicLine = bp;
              assemblyProgram[ap].command = &commands[2]; // READ
              assemblyProgram[ap].operand = var->aDefenitionPos;
              ap++;
            }
          break;

        case 2: // PRINT
          if (1)
            {

              BasicVariable *var = sc_getVariable (operand, 0, bp);
              if (var == NULL)
                fatal ();
              if (operand[1] != '\0')
                {
                  printf ("%d: Expected line terminator. Got '%c'\n", bp,
                          operand[1]);
                  fatal ();
                }

              assemblyProgram[ap].linkedBasicLine = bp;
              assemblyProgram[ap].command = &commands[3]; // WRITE
              assemblyProgram[ap].operand = var->aDefenitionPos;
              ap++;
            }
          break;
        case 3: // GOTO
          break;
        case 4: // IF
          break;
        case 5: // LET
          if (1)
            {
              BasicVariable *var = sc_getVariable (operand, 0, bp);
              if (var == NULL)
                fatal ();

              if (operand[1] != '=')
                {
                  printf ("%d: Expected assignment\n", bp);
                  fatal ();
                }

              int result = sb_evaluateExpression (&bp, &ap, var, operand + 2);
              if (result < 0)
                fatal ();

              assemblyProgram[ap].linkedBasicLine = bp;
              if (result == 1)
                assemblyProgram[ap].command = &assignmentCommand; // =
              assemblyProgram[ap].operand = var->aDefenitionPos;
            }
        case 6: // END
          break;
        }
    }

  for (int i = 0; i < ap; i++)
    {
      AssemblyCommand c = assemblyProgram[i];
      printf ("[b%d->a%d] %s %d %s ; %s [%s]\n", c.linkedBasicLine, i,
              c.command->command, c.operand,
              c.isVariable ? "[isVariable]" : "", c.comment,
              c.needsFurtherInvestigation ? "[needsFurtherInvestigation]"
                                          : "");
    }

  if (fatal)
    return -1;
  return 0;
}

/*

Command commands[] = { { "NOP", 0, OPNONE },     { "CPUINFO", 1, OPNONE },
                       { "READ", 10, OPADDR },   { "WRITE", 11, OPADDR },
                       { "LOAD", 20, OPADDR },   { "STORE", 21, OPADDR },
                       { "ADD", 30, OPADDR },    { "SUB", 31, OPADDR },
                       { "DIVIDE", 32, OPADDR }, { "MUL", 33, OPADDR },
                       { "JUMP", 40, OPADDR },   { "JNEG", 41, OPADDR },
                       { "JZ", 42, OPADDR },     { "HALT", 43, OPNONE },
                       { "RCCR", 70, OPADDR },   { "MOVA", 71, OPADDR } };
*/

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