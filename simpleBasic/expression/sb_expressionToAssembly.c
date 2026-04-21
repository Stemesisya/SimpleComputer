#include "../sb_variables.h"

#define stack_push(charaa) stack[stackSize++] = charaa;

#define stack_pop(into, ...) into = stack[--stackSize];

#define stack_peek() stack[stackSize - 1]

#define tokenToAddress(token)                                                 \
  if (isalpha ((token)[0]))                                                   \
    {                                                                         \
      assemblyProgram[ap].operand = sb_getVariable (token)->aDefenitionPos;   \
      assemblyProgram[ap].operandType = Variable;                             \
    }                                                                         \
  else if (sb_isdigit (token))                                                \
    {                                                                         \
      assemblyProgram[ap].operand                                             \
          = sb_getConstant (token, NULL)->aDefenitionPos;                     \
      assemblyProgram[ap].operandType = Constant;                             \
      assemblyProgram[ap].needsFurtherInvestigation = 1;                      \
    }                                                                         \
  else                                                                        \
    {                                                                         \
      int tempVarAddress;                                                     \
      tempVarAddress = atoi ((token) + 1);                                    \
      assemblyProgram[ap].operand = tempVarAddress;                           \
      assemblyProgram[ap].operandType = Temporary;                            \
      assemblyProgram[ap].needsFurtherInvestigation = 1;                      \
      sb_freeTemporaryVariable (tempVarAddress);                              \
    }

int
sb_expressionToAssembly (char expr[MEMORY_SIZE][7])
{

  int stackSize = 0;
  int loadedOperand = -2;
  char *stack[MEMORY_SIZE];

  if (expr[0][0] != '\0' && expr[1][0] == '\0')
    {
      assemblyProgram[ap].linkedBasicLine = bp;
      assemblyProgram[ap].command = sc_commands + 4; // LOAD
      tokenToAddress (expr[0]);
      ap++;
    }

  for (int i = 0; i < MEMORY_SIZE; i++)
    {
      if (expr[i][0] == '\0')
        break;

      // printf ("EtoA: current token '%s'\n", expr[i]);

      if (isalpha (expr[i][0]))
        {
          stack_push (expr[i]);
          continue;
        }
      if (sb_isdigit (expr[i]))
        {
          stack_push (expr[i]);
          continue;
        }

      // printf ("| stack: ");
      // for (int i = 0; i < stackSize; i++)
      //   printf ("%s ", stack[i]);
      // putchar ('\n');

      // Необходимо выгрузить аккумулятор и записать другое значение
      // printf ("| stackSize: %d, loadedOperand: %d\n", stackSize,
      // loadedOperand);
      if (stackSize - loadedOperand > 1
          || (expr[i][0] == '/' /*несимметричная операция*/
              && stackSize - loadedOperand - 1 != 0))
        {

          if (loadedOperand != -2)
            { // Если ничего не было загружено, то и выгружать не надо.

              int storeInto = sb_allocateTemporaryVariable ();
              // Занимаем использованные, но уже ненужные временные переменные.

              assemblyProgram[ap].linkedBasicLine = bp;
              assemblyProgram[ap].command = sc_commands + 5; // STORE
              assemblyProgram[ap].operand = storeInto;
              assemblyProgram[ap].operandType = Temporary;
              assemblyProgram[ap].needsFurtherInvestigation = 1;
              ap++;

              // printf ("| Storing into &%d.\n", storeInto);

              for (int i = 0; i < stackSize; i++)
                if (stack[i][0] == '~')
                  {
                    stack[i][0] = '&';
                    sprintf (stack[i] + 1, "%d", storeInto);
                    break;
                  }
            }

          // printf ("| Loading '%s'\n", stack[stackSize - 2]);

          assemblyProgram[ap].linkedBasicLine = bp;
          assemblyProgram[ap].command = sc_commands + 4; // LOAD
          tokenToAddress (stack[stackSize - 2]);
          ap++;
        }

      assemblyProgram[ap].linkedBasicLine = bp;
      switch (expr[i][0])
        {
        case '+':
          assemblyProgram[ap].command = sc_commands + 6; // ADD
          break;
        case '-':
          assemblyProgram[ap].command = sc_commands + 7; // SUB
          break;
        case '*':
          assemblyProgram[ap].command = sc_commands + 9; // MUL
          break;
        case '/':
          assemblyProgram[ap].command = sc_commands + 8; // DIVIDE
          break;
        }

      stack_pop (char *sa);
      stack_pop (char *sb);

      if (expr[i][0] == '/' && strcmp (sa, "0") == 0)
        {
          printf ("Error at %d: Control point found division by 0 during "
                  "evaluation.\n",
                  bl);
          return -1;
        }

      tokenToAddress (sa[0] == '~' ? sb : sa);
      ap++;

      sa[0] = '~';
      sa[1] = '\0';
      loadedOperand = stackSize + 1;
      stack_push (sa);
    }

  if (stackSize > 1 && stack_peek ()[0] != '~')
    {
      printf ("Error: %d: Invalid expression. Not enough operators.\n", bl);
      return -1;
    }

  return 0;
}