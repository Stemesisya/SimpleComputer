#include "../sb_variables.h"

/*
Пытается найти в строке первое упоминание переменной.

Если переменная в строке найдена, метод либо выделяет под нее
память, либо возвращает уже выделенную.
*/

BasicVariable *
sb_getVariable (char *operand)
{
  printf ("Searching variable in '%s'.\n", operand);
  if (*operand - 'A' < 0 || *operand - 'A' > 26)
    {
      int position = 0;
      if (isdigit (*operand))
        printf ("%d: Expected variable. Got number: '", bp);
      else
        printf ("%d: Illegal variable name: '", bp);

      while (isalnum (operand[position]))
        putchar (operand[position++]);
      putchar ('\'');
      putchar ('.');
      putchar ('\n');
      return NULL;
    }

  if (isalpha (operand[1]))
    {
      int position = 0;
      printf ("%d: Variable name should be 1 symbol length. Got '", bp);
      while (!isalpha (operand[position]))
        putchar (operand[position++]);
      putchar ('\'');
      putchar ('.');
      putchar ('\n');

      return NULL;
    }

  BasicVariable *var = definedVariables + (*operand - 'A');
  int definedIn = var->bDefenitionPos - 1;

  if (definedIn == -1)
    {
      var->bDefenitionPos = bp + 1;
      var->aDefenitionPos = MEMORY_SIZE - variablesPoolSize - 1;
      variablesPoolSize++;
      return var;
    }

  return var;
}