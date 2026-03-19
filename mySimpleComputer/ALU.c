#include "sc_variables.h"
#include <include/mySimpleComputer.h>
int
ALU (int command, int operand)
{
  int operandValue = 0;
  int accumulatorValue = 0;

  if (getIsJustIdleCompleted () == 0)
    {
      incrementIdleIncounter (10);
      return 0;
    }

  if (sc_memoryGet (operand, &operandValue) != 0)
    return -1;

  if (sc_accumulatorGet (&accumulatorValue) != 0)
    return -2;

  switch (command)
    {
    case 30: // ADD
      accumulatorValue += operandValue;
      break;
    case 31: // SUB
      accumulatorValue -= operandValue;
      break;
    case 32: // DIVIDE
      if (operandValue == 0)
        {
          sc_regSet (REG_ZERO_DIV, REG_ZERO_DIV);
          return 1;
        }
      accumulatorValue /= operandValue;
      break;
    case 33: // MUL
      accumulatorValue *= operandValue;
      break;
    default:
      return -3;
    }

  if (accumulatorValue > MAX_CELL_VALUE || accumulatorValue < 0)
    {
      sc_regSet (REG_OVERFLOW, REG_OVERFLOW);
      return 2;
    }

  sc_accumulatorSet (accumulatorValue);
  sc_notifyListener (STATE_ACCUMULATOR_UPDATE, 0);

  return 0;
}