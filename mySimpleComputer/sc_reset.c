#include <include/mySimpleComputer.h>

void
sc_reset ()
{
  sc_memoryInit ();
  sc_regInit ();
  sc_accumulatorInit ();
  sc_incounterInit ();
}