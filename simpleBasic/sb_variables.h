#ifndef SB_VARIABLES
#define SB_VARIABLES

#include <ctype.h>
#include <include/mySimpleComputer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASIC_COMMAND_TYPES_COUNT 7

typedef struct _BASIC_COMMAND_TYPE
{
  char id;
  char *type;
} BasicCommandType;

typedef struct _BASIC_COMMAND
{
  int line;
  BasicCommandType *command;
  char operand[1024];
} BasicCommand;

typedef struct _BASIC_VARIABLE
{
  char bDefenitionPos;
  char aDefenitionPos;
} BasicVariable;

typedef struct _BASIC_Constant
{
  char value[7];
  char aDefenitionPos;
} BasicConstant;

typedef enum _ASSEMBLY_COMMAND_OPERAND_TYPE
{
  Raw,
  Variable,
  Constant,
  Temporary
} AssemblyCommandOperandType;

typedef struct _ASSEMBLY_COMMAND
{
  char needsFurtherInvestigation;
  char linkedBasicLine;
  char isVariable;
  int operandType;
  int operand;
  Command *command;
  char *comment;
} AssemblyCommand;

extern BasicCommandType basicCommandTypes[BASIC_COMMAND_TYPES_COUNT + 1];
extern AssemblyCommand assemblyProgram[MEMORY_SIZE];
extern BasicCommand basicProgram[MEMORY_SIZE];

extern int bp;
extern int bl;
extern int ap;

extern char variableComments[MEMORY_SIZE][7];
extern BasicVariable definedVariables[26];
extern int variablesPoolSize;

extern BasicConstant definedConstants[MEMORY_SIZE];
extern int constantsPoolSize;

extern int maxTempVariablesCount;

extern Command assignmentCommand;

int sb_isdigit (char *a);

int sb_determineCommand (int line, char *lineNumber, char *command);
int sb_toassembly (int limit);
int sb_postprocess ();
void sb_printAss ();
int sb_saveToFile (char *fileName);

// Выражения сравнения

int sb_evaluateComparison (char *expression);
int sb_jumpIfAccumulator (int expressionType, int to);

// Выражения

int sb_evaluateExpression (char *expression);
int sb_optimizeExpression (char postfixExpression[MEMORY_SIZE][7]);
int sb_expressionToAssembly (char expr[MEMORY_SIZE][7]);

// Переменные

int sb_getAddressFromConstant (char *operand);
int sb_checkForConstant (char *operand, int *value);
BasicVariable *sb_getVariable (char *operand);
BasicConstant *sb_getConstant (char *operand, int *value);
int sb_allocateTemporaryVariable ();
void sb_freeTemporaryVariable (int temporaryVariableAddress);

#endif