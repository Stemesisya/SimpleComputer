#include "../sb_variables.h"

static char availableTempVariables[MEMORY_SIZE];
static int availableTempVarsCount = 0;
int maxTempVariablesCount = 0;

// Возвращает адрес
int
sb_allocateTemporaryVariable ()
{
  int allocatedVariable = 0;
  if (availableTempVarsCount > 0)
    allocatedVariable = availableTempVariables[availableTempVarsCount--];
  else
    allocatedVariable = maxTempVariablesCount++;
  return allocatedVariable;
}

void
sb_freeTemporaryVariable (int temporaryVariableAddress)
{
  availableTempVariables[availableTempVarsCount++] = temporaryVariableAddress;
}