#include "include/mySimpleComputer.h"
#include "sb_variables.h"

#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// --------------------------------------------------------------------------------------------------
// //
//
//                                 Спагетти код во всей красе
//
//                                             :3
//
// --------------------------------------------------------------------------------------------------
// //

#define fatal(text...)                                                        \
  if (1)                                                                      \
    {                                                                         \
      printf (text);                                                          \
      fatal++;                                                                \
      skipLine = 1;                                                           \
      continue;                                                               \
    }

#define moveNextLine()                                                        \
  line++;                                                                     \
  skipLine = 0;                                                               \
  pos = 0;                                                                    \
  lastC = c;                                                                  \
  stage = 0;                                                                  \
  i = 0;                                                                      \
  *lineNumber = '\0';                                                         \
  *command = '\0';                                                            \
  *operand = '\0';

int
parseOperand (int line, char *operand)
{
  strcpy (basicProgram[line - 1].operand, operand);
  printf ("%3d: %d, %s, %s\n", line, basicProgram[line - 1].line,
          basicProgram[line - 1].command->type, operand);
  return 0;
}

int
main (int argc, char *argv[])
{

  if (argc < 3)
    {
      printf ("Usage: basic <source> <output>\n");
      return -1;
    }

  FILE *file = fopen (argv[1], "r");

  if (file == NULL)
    {
      printf ("Source '%s' not found.\n", argv[1]);
      return -2;
    }

  for (int i = 0; i < MEMORY_SIZE; i++)
    {
      basicProgram[i].command = &basicCommandTypes[7];
      basicProgram[i].line = 0;
      basicProgram[i].operand[0] = '\0';
      assemblyProgram[i].linkedBasicLine = -1;
      assemblyProgram[i].operand = 0;
      assemblyProgram[i].command = sc_commands;
      assemblyProgram[i].comment = NULL;
      assemblyProgram[i].needsFurtherInvestigation = 0;
    }

  // Флаг, показывающий, что компиляция завершилась неудачей
  int fatal = 0;

  // Заставляет компилятор завершить обработку строки и перейти к следующей
  int skipLine = 0;

  // Номер текущей обрабатываемой строки файла
  int line = 0;
  // Номер текущего символа в обрабатываемой строке
  int pos = 0;
  char c = 0;
  char lastC = 0;

  // Номер текущей строчки команды ( _10_ INPUT C)
  char lineNumber[7] = "";
  // Текущая команда ( 10 _INPUT_ C)
  char command[6] = "";
  // Все, что идет после ( 10 INPUT _C_)
  char operand[1024] = "";
  int stage = 0;
  int i = 0;

  moveNextLine ();

  while (1)
    {
      pos++;
      lastC = c;
      c = fgetc (file);

      if (c == '\n' || c == EOF)
        {

          // Пустая строчка.
          if (*lineNumber == '\0' && *command == '\0')
            {
              moveNextLine ();
              line--;
              if (c == EOF)
                break;
              continue;
            }

          if (!skipLine && line > MEMORY_SIZE)
            {
              printf ("Error %3d: Program exeeds memory size.\n", line);
              fatal = 1;
              break;
            }

          if (!skipLine)
            {
              switch (stage)
                {
                case 0:
                  printf (
                      "Error at %d:%d: Unexpected line terminator. Expected "
                      "Command.\n",
                      line, pos);
                  fatal = 1;
                  break;
                case 1:
                  if (sb_determineCommand (line, lineNumber, command) != 0)
                    fatal = 1;
                  break;
                }

              parseOperand (line, operand);
            }

          if (c == EOF)
            break;

          moveNextLine ();

          continue;
        }

      // Прокручиваем строку до конца
      if (skipLine)
        continue;

      // Комментарий оставляем в сыром виде
      if (isblank (c)
          && strcmp (basicProgram[line - 1].command->type, "REM") != 0)
        {

          // Убираем лишние пробелы
          if (isblank (lastC))
            continue;

          if (stage <= 1)
            i = 0;

          stage++;
          if (stage == 2
              && sb_determineCommand (line, lineNumber, command) != 0)
            {
              fatal = 1;
              skipLine = 1;
            }
          continue;
        }

      switch (stage)
        {
        case 0:
          if (i >= 6)
            fatal ("Error %3d:%d: Line number overflow \n", line, pos);
          lineNumber[i++] = c;
          lineNumber[i] = '\0';
          break;
        case 1:
          if (i > 6)
            fatal ("Error %3d:%d: Unknown command\n", line, pos - 5);
          command[i++] = c;
          command[i] = '\0';
          break;
        default:
          if (i >= 1024)
            fatal ("Error %3d:%d: Operand overflow\n", line, pos);
          operand[i++] = c;
          operand[i] = '\0';
          break;
        }
    }

  fclose (file);

  if (fatal)
    return -1;

  if (sb_toassembly (line) != 0)
    return -2;

  if (sb_postprocess () != 0)
    return -3;

  sb_printAss ();

  sb_saveToFile (argv[2]);

  return 0;
}