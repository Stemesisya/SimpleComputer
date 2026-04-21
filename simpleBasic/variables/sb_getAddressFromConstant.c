#include "../sb_variables.h"

/*
Пытается найти в строке первое упоминание десятичного числа.

Если константа в строке найдена, возвращает его строчную длину.
Число должно быть в диапазоне [ -16к , +16к ]

value - куда будет записано целочисленное значение константы. [Необязательно]
*/

int
sb_getAddressFromConstant (char *operand)
{
  int addr = 0;
  int addrLen = sb_checkForConstant (operand, &addr);
  if (addrLen == -1)
    return -1;

  if (addr < 0)
    {
      printf ("Error at %d: Address should not be negative\n", bl);
      return -1;
    }
  if (addr > 128)
    {
      printf ("Error at %d: Address overflow.\n", bl);
      return -1;
    }
  if (operand[addrLen] != '\0')
    {
      printf ("Error at %d: Expected line break. Found '%c'.\n", bl,
              operand[addrLen]);
      return -1;
    }
  return addr;
}