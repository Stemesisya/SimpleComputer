#include "include/mySimpleComputer.h"
#include "sb_variables.h"
#include <ctype.h>
#include <stdlib.h>

typedef struct STACK_NODE
{
  char isDependent;
  char *chara;
} StackNode;

#define stack_push(charaa, isDependentt)                                      \
  stack[stackSize].chara = charaa;                                            \
  stack[stackSize++].isDependent = isDependentt;

#define stack_pop(into, ...) into = stack[--stackSize];

#define stack_peek() stack[stackSize - 1]

int
sb_optimizeExpression (char postfixExpression[MEMORY_SIZE][4], int line)
{

  int stackSize = 0;

  StackNode stack[MEMORY_SIZE];

  int pi = 0;
  for (; postfixExpression[pi][0] != '\0' && pi < 128; pi++)
    {
      if (isdigit (postfixExpression[pi][0]))
        {
          stack_push (postfixExpression[pi], 0);
          continue;
        }
      if (isalpha (postfixExpression[pi][0]))
        {
          stack_push (postfixExpression[pi], 1);
          continue;
        }

      if (stack_peek ().isDependent == 1)
        {
          stack_push (postfixExpression[pi], 1);
          continue;
        }

      if (stackSize <= 0)
        {
          printf ("%d Invalid expression\n", line);
          return -1;
        }

      stack_pop (StackNode snb);

      if (stack_peek ().isDependent == 1)
        {
          stack_push (snb.chara, snb.isDependent);
          stack_push (postfixExpression[pi], 1);
          continue;
        }

      if (stackSize <= 0)
        {
          printf ("%d Invalid expression\n", line);
          return -1;
        }

      stack_pop (StackNode sna);

      int b = atoi (snb.chara);
      int a = atoi (sna.chara);

      int result;
      switch (postfixExpression[pi][0])
        {
        case '-':
          result = a - b;
          break;
        case '+':
          result = a + b;
          break;
        case '*':
          result = a * b;
          break;
        case '/':
          result = a / b;
          break;
        }
      printf ("%d %c %d = %d\n", a, postfixExpression[pi][0], b, result);

      if (result > 128)
        {
          printf ("%d: Control point reached variable overflow during "
                  "evaluation.\n",
                  line);
          return -1;
        }

      sprintf (sna.chara, "%d", result);
      stack_push (sna.chara, 0);
    }

  for (int i = 0; i < stackSize; i++)
    for (int j = 0; j < 4; j++)
      postfixExpression[i][j] = stack[i].chara[j];

  if (stackSize != MEMORY_SIZE - 1)
    postfixExpression[stackSize][0] = '\0';

  return 0;
}