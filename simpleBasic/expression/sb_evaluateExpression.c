#include "../sb_variables.h"

#define fexpr_append(chara)                                                   \
  if (fexprI >= MEMORY_SIZE)                                                  \
    {                                                                         \
      printf ("%d: Expression limit achieved\n", bp);                         \
      return -1;                                                              \
    }                                                                         \
  fexpr[fexprI][0] = chara;                                                   \
  fexpr[fexprI++][1] = '\0';

#define stack_push(chara)                                                     \
  if (stackSize >= MEMORY_SIZE)                                               \
    {                                                                         \
      printf ("%d: Expression limit achieved\n", bp);                         \
      return -1;                                                              \
    }                                                                         \
  stack[stackSize++] = chara;

#define stack_pop(into, ...)                                                  \
  if (stackSize < 0)                                                          \
    {                                                                         \
      printf (__VA_ARGS__);                                                   \
      return -1;                                                              \
    }                                                                         \
  into = stack[--stackSize];

#define stack_peek() stack[stackSize - 1]

typedef struct _OPERATOR
{
  char chara;
  char f;
} Operator;

static Operator operators[] = { { '+', 1 }, { '-', 1 }, { '*', 2 },
                                { '/', 2 }, { '+', 1 }, { '(', 99 } };

int
calculatePriority (char a)
{
  for (int i = 0; i < 6; i++)
    if (a == operators[i].chara)
      return operators[i].f;
  return -1;
}

int
sb_evaluateExpression (char *expression)
{

  if (expression[0] == '\0')
    {
      printf ("%d: Expected expression. Got nothing.\n", bp);
      return -1;
    }

  int fexprI = 0;
  int stackSize = 0;

  char fexpr[MEMORY_SIZE][7] = { 0 };
  char stack[MEMORY_SIZE] = "";

  for (int i = 0; i < MEMORY_SIZE; i++)
    {
      fexpr[i][0] = '\0';
      stack[i] = '\0';
    }

  for (; *expression != '\0'; expression++)
    {

      printf ("> %s\n", expression);

      //   printf ("> expr : ");
      //   for (int i = 0; i < fexprI; i++)

      //     printf ("%s ", fexpr[i]);

      //   putchar ('\n');
      //   printf ("> stack: ");

      //   for (int i = 0; i < stackSize; i++)
      //     putchar (stack[i]);
      //   putchar ('\n');

      if (isalpha (*expression))
        {
          if (sb_getVariable (expression) == NULL)
            return -1;
          fexpr_append (*expression);
          printf ("append %c (var)\n", *expression);
          continue;
        }
      if (sb_isdigit (expression))
        {
          int size = sb_checkForConstant (expression, NULL);
          if (size == -1)
            return -1;

          strncpy (fexpr[fexprI], expression, size);
          fexpr[fexprI][size] = '\0';
          fexprI++;
          expression += size - 1;
          printf ("append %s (const)\n", fexpr[fexprI - 1]);
          continue;
        }

      if (*expression != ')')
        {
          int priority = calculatePriority (*expression);
          while (stackSize > 0 && calculatePriority (stack_peek ()) >= priority
                 && stack_peek () != '(')
            {
              stack_pop (char p, "how?\n");
              //   printf ("append %c (high priority)\n", p);

              fexpr_append (p);
            }

          stack_push (*expression);
          //   printf ("append %c (operator)\n", *expression);
          continue;
        }
      char popitem = '(';

      do
        {
          stack_pop (popitem, "%d: Not found matching closing bracket\n", bp);
          if (popitem == '(')
            break;
          fexpr_append (popitem);
          //   printf ("collapse %c\n", popitem);
        }
      while (1);
      continue;
    }

  char popitem = '\0';
  while (stackSize > 0)
    {
      stack_pop (popitem, "how?\n");
      fexpr_append (popitem);

      if (popitem == '(')
        {
          printf ("%d: Not found matching closing bracket\n", bp);
          return -1;
        }
    }

  printf ("\tExpression: ");
  int pi = 0;
  while (fexpr[pi][0] != '\0')
    printf (" %s", fexpr[pi++]);
  putchar ('\n');

  if (sb_optimizeExpression (fexpr, bp) != 0)
    return -1;

  printf ("\tOptimized Expression: ");
  pi = 0;
  while (fexpr[pi][0] != '\0')
    printf (" %s", fexpr[pi++]);
  putchar ('\n');

  return sb_expressionToAssembly (fexpr);
}