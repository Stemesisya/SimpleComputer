#include "include/myTerm.h"
#include "mySimpleComputer/sc_variables.h"
#include <console/console.h>
#include <include/mySimpleComputer.h>
#include <string.h>
#include <unistd.h>

int selectedCell = 0;
int incounterCell = 0;
int incounterCellIsIdling = 0;

void
forcePrintIncounterCell ()
{
  if (incounterCell < MEMORY_SIZE)
    {
      if (incounterCellIsIdling != 0)
        printCell (incounterCell, BLACK, RED);
      else
        printCell (incounterCell, BLACK, GREEN);
    }
}
void
printIncounterCell ()
{
  if (incounterCell == selectedCell)
    return;
  forcePrintIncounterCell ();
}

int lastIncounterCell = 0;
void
moveIncounterCell ()
{
  if (lastIncounterCell != selectedCell && lastIncounterCell < MEMORY_SIZE)
    printCell (lastIncounterCell, RESET, RESET);

  printIncounterCell ();

  lastIncounterCell = incounterCell;
}

void
printSelectedCell ()
{
  printCell (selectedCell, INVERSE, NOTHING);
  printBigCell ();
  printDecodedCommand ();
}

int lastSelectedCell = 0;
void
hideSelectedCell ()
{
  printCell (selectedCell, RESET, RESET);
  if (selectedCell == incounterCell)
    forcePrintIncounterCell ();
}

void
moveSelectedCell (int to)
{
  hideSelectedCell ();

  selectedCell = to;
  printSelectedCell ();

  lastSelectedCell = selectedCell;
}

void
p_determineCommand ()
{
  int s, o;
  int command;
  Command *commands = getCommands ();
  sc_incounterGet (&command);
  sc_memoryGet (command, &command);
  sc_commandDecode (command, &s, &command, &o);
  char cmdI[6] = "";
  sprintf (cmdI, "%d ", command);
  write (1, cmdI, 6);
  for (int i = 0; i < COMMANDS_COUNT_TRUNCED; i++)
    {
      if (command == commands[i].code)
        {
          write (1, commands[i].command, strlen (commands[i].command));
          break;
        }
    }
}