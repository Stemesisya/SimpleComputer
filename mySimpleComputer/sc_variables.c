#include "sc_variables.h"

static int memory[MEMORY_SIZE];

static int flagRegister = REG_TICK_IGNORE;
static int accumulator = 0;
static int incounter = 0;
static char idleIncounter = 0;
static int tickCounter = 0;
static int isIdleJustCompleted = 0;

int *
getMemory ()
{
  return memory;
}

int
getFlagRegister ()
{
  return flagRegister;
}

void
setFlagRegister (int value)
{
  flagRegister = value;
}

int
getAccumulator ()
{
  return accumulator;
}

void
setAccumulator (int value)
{
  accumulator = value;
}

int
getIncounter ()
{
  return incounter;
}

void
setIncounter (int value)
{
  incounter = value;
}

int
getTickCounter ()
{
  return tickCounter;
}

void
incrementTickCounter ()
{
  tickCounter++;
}

void
resetTickCounter ()
{
  tickCounter = 0;
}

char
getIdleIncounter ()
{
  return idleIncounter;
}

void
decrementIdleIncounter ()
{
  idleIncounter--;
}

void
incrementIdleIncounter (char value)
{
  idleIncounter += value;
}

int
getIsJustIdleCompleted ()
{
  return isIdleJustCompleted;
}
void
setIsJustIdleCompleted (int value)
{
  isIdleJustCompleted = value;
}