#include <console/console.h>
#include <include/myReadKey.h>
#include <include/mySimpleComputer.h>
#include <include/myTerm.h>
#include <unistd.h>

#define STRINGIFY(x) #x

void
im_memoryLeft ()
{
  if (selectedCell / 10 == RAM_ROWS && selectedCell % RAM_LAST_ROW_LEN == 0)
    {
      moveSelectedCell (selectedCell + RAM_LAST_ROW_LEN - 1);
      return;
    }

  if (selectedCell % 10 == 0)
    moveSelectedCell (selectedCell + RAM_COLUMNS - 1);
  else
    moveSelectedCell (selectedCell - 1);
}

void
im_memoryRight ()
{
  if (selectedCell / 10 == RAM_ROWS
      && selectedCell % RAM_LAST_ROW_LEN == RAM_LAST_ROW_LEN - 1)
    {
      moveSelectedCell (selectedCell - RAM_LAST_ROW_LEN + 1);
      return;
    }

  if (selectedCell % 10 == 9)
    moveSelectedCell (selectedCell - RAM_COLUMNS + 1);
  else
    moveSelectedCell (selectedCell + 1);
}

void
im_memoryUp ()
{

  if (selectedCell / RAM_COLUMNS == 0)
    {
      if (selectedCell % RAM_COLUMNS >= RAM_LAST_ROW_LEN)
        moveSelectedCell ((RAM_ROWS - 1) * RAM_COLUMNS
                          + (selectedCell % RAM_COLUMNS));
      else
        moveSelectedCell ((RAM_ROWS)*RAM_COLUMNS
                          + (selectedCell % RAM_COLUMNS));
    }
  else
    moveSelectedCell (selectedCell - RAM_COLUMNS);
}

void
im_memoryDown ()
{
  if (selectedCell % RAM_COLUMNS >= RAM_LAST_ROW_LEN
      && selectedCell / RAM_COLUMNS == RAM_ROWS - 1)
    {
      moveSelectedCell (selectedCell % RAM_COLUMNS);
      return;
    }

  if (selectedCell / RAM_COLUMNS == RAM_ROWS)
    {
      moveSelectedCell (selectedCell % RAM_COLUMNS);
    }
  else
    moveSelectedCell (selectedCell + RAM_COLUMNS);
}

void
im_memoryWrite ()
{
  int value = 0;
  printCell (selectedCell, GREEN, GREEN);
  mt_gotoXY (2 + (selectedCell % RAM_COLUMNS) * 6,
             2 + (selectedCell / RAM_COLUMNS));

  mt_setbgcolor (BLACK);
  int result = rk_readvalue (&value, 0);
  mt_setdefaultcolor ();

  if (result == 0)
    {
      sc_memorySet (selectedCell, value);
      moveSelectedCell (selectedCell);
      printCell (selectedCell, BLACK, GREEN);
      printTerm (selectedCell, 1);
      return;
    }

  printCell (selectedCell, BLACK, RED);
  mt_gotoXY (0, COMMAND_LINE_Y);
  mt_delline ();
  mt_setfgcolor (RED);
  switch (result)
    {
    case 1:
      moveSelectedCell (selectedCell);
      break;

    case -20:
      write (1, "Illegal cell format and/or illegal symbols.", 44);
      break;
    case -10:
      write (1, "Illegal sign. Allowed only '+' or '-'.", 39);
      break;
    case -2:
      write (1, "Command field overflow.", 24);
      break;
    case -3:
      write (1, "Operand field overflow.", 24);
      break;
    default:
      write (1, "Unknown error", 14);
    }
  mt_setdefaultcolor ();
}