#include "../sb_variables.h"

/*
Пытается найти в строке первое упоминание десятичного числа.

Если константа в строке найдена, метод либо выделяет под нее
память, либо возвращает уже выделенную.
Число должно быть в диапазоне [ -16к , +16к ]

value - куда будет записано целочисленное значение константы. [Необязательно]
*/

BasicConstant *
sb_getConstant (char *operand, /*Nullable*/ int *value)
{
  int startFrom = sb_checkForConstant (operand, value);
  if (startFrom == -1)
    return NULL;

  for (int i = 0; i < constantsPoolSize; i++)
    if (strncmp (definedConstants[i].value, operand, startFrom) == 0)
      return &definedConstants[i];

  definedConstants[constantsPoolSize].aDefenitionPos = constantsPoolSize;
  strncpy (definedConstants[constantsPoolSize].value, operand, startFrom);
  constantsPoolSize++;

  return &definedConstants[constantsPoolSize - 1];
}