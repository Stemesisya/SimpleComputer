#include "../sb_variables.h"

/*
Пытается найти в строке первое упоминание десятичного числа.

Если константа в строке найдена, возвращает его строчную длину.
Число должно быть в диапазоне [ -16к , +16к ]

value - куда будет записано целочисленное значение константы. [Необязательно]
*/

int
sb_checkForConstant (char *operand, /*Nullable*/ int *value)
{
  printf ("Searching constant in '%s'\n", operand);
  int isNegative = operand[0] == '-';
  int startFrom = 0;
  if (isNegative)
    startFrom = 1;
  while (isdigit (operand[startFrom]))
    startFrom++;

  if (isalpha (operand[startFrom]))
    {
      if ((isNegative && startFrom <= 1) || (!isNegative && startFrom < 1))
        {
          printf ("%d: Expected constant.\n", bp);
          return -1;
        }
      printf ("%d: Variable should not start with digit.\n", bp);
      return -1;
    }
  if (startFrom > 6)
    {
      printf ("%d: Number '%.*s' has too many digits.\n", bp, startFrom,
              operand);
      return -1;
    }

  char svalue[7];
  strncpy (svalue, operand, startFrom);
  svalue[startFrom] = '\0';

  int ivalue = atoi (svalue);
  if (ivalue > MAX_ABSOLUTE_VALUE)
    {
      printf ("%d: Number '%d' is too high.\n", bp, ivalue);
      return -1;
    }
  if (ivalue < -(MAX_ABSOLUTE_VALUE + 1))
    {
      printf ("%d: Number '%d' is too low.\n", bp, ivalue);
      return -1;
    }

  printf ("Found constant: '%.*s'\n", startFrom, operand);

  if (value != NULL)
    *value = ivalue;

  return startFrom;
}