#include "sb_variables.h"
#include "include/mySimpleComputer.h"

BasicCommandType basicCommandTypes[]
    = { { 0, "REM" }, { 1, "INPUT" }, { 2, "PRINT" }, { 3, "GOTO" },
        { 4, "IF" },  { 5, "LET" },   { 6, "END" },   { 7, "-- NULL --" } };

BasicCommand basicProgram[MEMORY_SIZE] = { 0 };
AssemblyCommand assemblyProgram[MEMORY_SIZE] = { 0 };

int bp = 0;
int ap = 0;

Command assignmentCommand = { "=", -1, OPADDR };

char variableComments[MEMORY_SIZE][7] = { 0 };

BasicVariable definedVariables[26] = { 0 };
int variablesPoolSize = 0;

BasicConstant definedConstants[MEMORY_SIZE] = { 0 };
int constantsPoolSize = 0;