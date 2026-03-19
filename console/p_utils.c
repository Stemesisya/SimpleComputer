#include "include/myTerm.h"
#include <console/console.h>
#include <include/mySimpleComputer.h>

int selectedCell = 0;
int incounterCell = 0;
int incounterCellIsIdling = 0;

void
forcePrintIncounterCell ()
{
  if (incounterCell < MEMORY_SIZE)
    printCell (incounterCell, GREEN, incounterCellIsIdling ? RED : NOTHING);
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
  printBigCell ();
  printDecodedCommand ();
  printSelectedCell ();

  lastSelectedCell = selectedCell;
}