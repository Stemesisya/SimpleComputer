#include <console/console.h>
#include <stdio.h>

#define printPink(what) printf ("\n\033[1;38;2;255;20;140m" what "\033[0m\n");

#define setCell(addr, sign, command, operand)                                 \
  sc_commandEncode (sign, command, operand, &value);                          \
  sc_memorySet (addr, value);                                                 \
  cli_printCell (addr);                                                       \
  putchar ('\n')

#define setFlags(select, enable)                                              \
  sc_regSet (select, enable);                                                 \
  putchar ('[');                                                              \
  cli_printFlags ();                                                          \
  putchar (']');                                                              \
  putchar ('\n')

int
main ()
{
  int value = 0;

  printPink ("Initializing devices...\n");
  sc_accumulatorInit ();
  sc_incounterInit ();
  sc_memoryInit ();
  sc_regInit ();

  cli_printFlags ();
  sc_accumulatorGet (&value);
  printf (" [accumulator: ");
  cli_printDecodedCommand (value);
  sc_incounterGet (&value);
  printf (", incounter: ");
  cli_printDecodedCommand (value);
  printf ("]\n");
  cli_printMemory ();

  printPink ("Setting memory cell's values...\n");

  printf ("Cell 0. Expected: -4C7F (Max value) Got: ");
  setCell (0, 1, 76, 127);

  printf ("Cell 10. Expected: +426D Got: ");
  setCell (10, 0, 66, 109);

  printf ("Cell 127 (Max address). Expected: +0F10 Got: ");
  setCell (127, 0, 15, 16);

  printf ("Cell 52. Expected: -0000 Got: ");
  setCell (52, 1, 0, 0);

  cli_printMemory ();

  printPink ("Trying to pass invalid values to cell...\n");

  printf ("Invalid value: 0x8000 (Max value + 1). Expected: -2 (Variable "
          "overflow). Got: ");
  printf ("%d\n", sc_memorySet (52, MAX_CELL_VALUE + 1));

  printf ("Invalid value: -1. Expected: -2 (Variable underflow). Got: ");
  printf ("%d\n", sc_memorySet (52, -1));

  printf ("Invalid value: 0x7FFF (Absolute max value). Expected: -3 (Command "
          "overflow). Got: ");
  printf ("%d\n", sc_memorySet (52, MAX_CELL_VALUE));

  printPink ("Setting register flags...\n");

  printf ("enabling reg'0'. Expected: [_ 0 _ T _] Got: ");
  setFlags (REG_ZERO_DIV, REG_ZERO_DIV);

  printf ("disabling reg'T'. Expected: [_ 0 _ _ _] Got: ");
  setFlags (REG_TICK_IGNORE, 0);

  printf ("enabling reg'M' and reg'E'. Expected: [_ 0 M _ E] Got: ");
  setFlags (REG_OUT_OF_BOUNDS | REG_INVALID_COMMAND,
            REG_OUT_OF_BOUNDS | REG_INVALID_COMMAND);

  printf ("do nothing. Expected: [_ 0 M _ E] Got: ");
  setFlags (0, 0);

  printf ("enabling reg'P' and disabling reg'M'. Expected: [P 0 _ _ E] Got: ");
  setFlags (REG_OVERFLOW | REG_OUT_OF_BOUNDS, REG_OVERFLOW);

  printf ("disabling all. Expected: [_ _ _ _ _] Got: ");
  setFlags (REG_ALL, 0);

  printPink ("Passing invalid parameters to flags...\n");

  printf ("enabling reg 0xFF. Expected: -1 (Invalid address) Got: ");
  printf ("%d\n", sc_regSet (0xFF, 0xFF));

  printf ("selecting reg'0'. Enabling reg 0xFF Expected: -3 (Invalid value) "
          "Got: ");
  printf ("%d\n", sc_regSet (REG_ZERO_DIV, 0xFF));

  printf ("selecting reg'0'. Enabling reg'T' Expected: -2 (Modifying "
          "non-selected flag) Got: ");
  printf ("%d\n", sc_regSet (REG_ZERO_DIV, REG_TICK_IGNORE));
  cli_printFlags ();
  putchar ('\n');

  printPink ("Set accumulator\n");
  printf ("With value -4C7F. Expected: -4C7f. Got: ");
  sc_commandEncode (1, 76, 127, &value);
  sc_accumulatorSet (value);
  sc_accumulatorGet (&value);
  cli_printDecodedCommand (value);
  putchar ('\n');

  printPink ("Trying to pass to accumulator invalid values\n");
  printf ("Invalid value: 0x8000 (Max value + 1). Expected: -1 (Variable "
          "overflow). Got: ");
  printf ("%d\n", sc_accumulatorSet (MAX_CELL_VALUE + 1));

  printf ("Invalid value: -1. Expected: -1 (Variable underflow). Got: ");
  printf ("%d\n", sc_accumulatorSet (-1));

  printf ("Invalid value: 0x7FFF (Absolute max value). Expected: -3 (Command "
          "overflow). Got: ");
  printf ("%d\n", sc_accumulatorSet (MAX_CELL_VALUE));

  printPink ("Set incounter\n");
  printf ("With value -4C7F. Expected: -4C7f. Got: ");
  sc_commandEncode (1, 76, 127, &value);
  sc_incounterSet (value);
  sc_incounterGet (&value);
  cli_printDecodedCommand (value);
  putchar ('\n');

  printPink ("Trying to pass to incounter invalid values\n");
  printf ("Invalid value: 0x8000 (Max value + 1). Expected: -2 (Variable "
          "overflow). Got: ");
  printf ("%d\n", sc_incounterSet (MAX_CELL_VALUE + 1));

  printf ("Invalid value: -1. Expected: -2 (Variable underflow). Got: ");
  printf ("%d\n", sc_incounterSet (-1));

  printf ("Invalid value: 0x7FFF (Absolute max value). Expected: -3 (Command "
          "overflow). Got: ");
  printf ("%d\n", sc_incounterSet (MAX_CELL_VALUE));

  printPink ("Finished.\n");

  return 0;
}