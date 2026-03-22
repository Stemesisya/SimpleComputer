#include "mySimpleComputer/sc_variables.h"
#include <include/mySimpleComputer.h>
#include <signal.h>
#include <sys/time.h>

static int skippingTicks = 0;
void
ICR (int signo)
{
  if (signo == SIGUSR1)
    {
      CU (signo);
      return;
    }

  sc_regGet (REG_TICK_IGNORE, &skippingTicks);

  if (skippingTicks)
    return;

  incrementTickCounter ();

  if (getIdleIncounter () > 0)
    {
      setIsJustIdleCompleted (1);
      sc_notifyListener (STATE_INCOUNTER_UPDATE, getIdleIncounter ());
      decrementIdleIncounter ();
      return;
    }

  CU (signo);
  setIsJustIdleCompleted (0);
}