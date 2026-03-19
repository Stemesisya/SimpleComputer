#include "mySimpleComputer/sc_variables.h"
#include <include/mySimpleComputer.h>
#include <sys/time.h>

static int skippingTicks = 0;
void
ICR (int signo)
{
  sc_regGet (REG_TICK_IGNORE, &skippingTicks);

  if (skippingTicks)
    return;

  incrementTickCounter ();

  if (getIdleIncounter () > 0)
    {
      setIsJustIdleCompleted (1);
      sc_notifyListener (STATE_INCOUNTER_UPDATE, 1);
      decrementIdleIncounter ();
      return;
    }

  CU (signo);
  setIsJustIdleCompleted (0);
}