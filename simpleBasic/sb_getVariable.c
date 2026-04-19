#include "sb_variables.h"
#include <ctype.h>

BasicVariable *
sc_getVariable (char *operand, int position, int bpointer)
{
  if (operand[position] - 'A' < 0 && operand[position] - 'A' > 26)
    {
      printf ("%d: Illegal variable name: '", bpointer);
      while (!isalpha (operand[position]))
        putchar (operand[position++]);
      putchar ('\'');
      putchar ('\n');
      return NULL;
    }
  if (isalpha (operand[position + 1]))
    {
      printf ("%d: Variable name should be 1 symbol length. Got '", bpointer);
      while (!isalpha (operand[position]))
        putchar (operand[position++]);
      putchar ('\'');
      putchar ('\n');

      return NULL;
    }

  BasicVariable *var = definedVariables + (operand[position] - 'A');
  int definedIn = var->bDefenitionPos - 1;

  if (definedIn == -1)
    {
      var->bDefenitionPos = bpointer + 1;
      var->aDefenitionPos = MEMORY_SIZE - 1 - variablesPoolSize;
      variablesPoolSize++;
      return var;
    }

  return var;
}