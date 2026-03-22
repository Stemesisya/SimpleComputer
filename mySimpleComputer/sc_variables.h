#ifndef SC_VARIABLES
#define SC_VARIABLES

#include "../include/mySimpleComputer.h"

int *getMemory ();

int getFlagRegister ();
void setFlagRegister (int value);

int getAccumulator ();
void setAccumulator (int value);

int getIncounter ();
void setIncounter (int value);

void incrementTickCounter ();
void resetTickCounter ();

char getIdleIncounter ();
void decrementIdleIncounter ();
void incrementIdleIncounter (char value);

int getIsJustIdleCompleted ();
void setIsJustIdleCompleted (int value);

#endif