#include "sb_variables.h"

int
sb_determineCommand (int line, char *lineNumber, char *command)
{
  int linenum;
  if (sscanf (lineNumber, "%d", &linenum) == 0)
    {
      printf ("Error at %d: Unexpected symbols in lineId\n", line);
      return -1;
    }

  for (int i = 0; i < BASIC_COMMAND_TYPES_COUNT; i++)
    {
      if (strcmp (command, basicCommandTypes[i].type) != 0)
        continue;
      basicProgram[line - 1].line = linenum;
      basicProgram[line - 1].command = &basicCommandTypes[i];
      return 0;
    }
  printf ("Error at %d: Unknown command '%s'\n", line, command);
  return -1;
}