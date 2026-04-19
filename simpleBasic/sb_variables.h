#include <include/mySimpleComputer.h>
#include <stdio.h>
#include <string.h>

#define BASIC_COMMAND_TYPES_COUNT 7

typedef struct _BASIC_COMMAND_TYPE
{
  char id;
  char *type;
} BasicCommandType;

typedef struct _BASIC_COMMAND
{
  char line;
  BasicCommandType *command;
  char operand[1024];
} BasicCommand;

typedef struct _BASIC_VARIABLE
{
  char bDefenitionPos;
  char aDefenitionPos;
} BasicVariable;

typedef struct _ASSEMBLY_COMMAND
{
  char needsFurtherInvestigation;
  char linkedBasicLine;
  char isVariable;
  int operand;
  Command *command;
  char *comment;
} AssemblyCommand;

extern BasicCommandType basicCommandTypes[BASIC_COMMAND_TYPES_COUNT + 1];
extern AssemblyCommand assemblyProgram[MEMORY_SIZE];
extern BasicCommand basicProgram[MEMORY_SIZE];
extern BasicVariable definedVariables[26];
extern char variablesPoolSize;
extern Command assignmentCommand;

int sb_determineCommand (int line, char *lineNumber, char *command);
int sb_translate (int limit);
int sb_evaluateExpression (int *bp, int *ap, BasicVariable *var,
                           char *expression);
BasicVariable *sc_getVariable (char *operand, int position, int bpointer);
int sb_optimizeExpression (char postfixExpression[MEMORY_SIZE][4], int line);
int sb_expressionToAssembly(char postfixExpression[MEMORY_SIZE][4], int line);