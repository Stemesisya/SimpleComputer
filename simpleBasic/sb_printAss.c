#include "sb_variables.h"

void
sb_printAss ()
{
  printf ("\n");
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
}