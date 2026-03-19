#include <include/mySimpleComputer.h>
#include <signal.h>
#include <sys/time.h>

static struct itimerval nval
    = { .it_interval = { 0, 500000 }, .it_value = { 0, 500000 } },
    oval;

void
IG (int signo)
{
  ICR (signo);
  setitimer (ITIMER_REAL, &nval, &oval);
}

void
IG_init ()
{
  signal (SIGALRM, IG);
  IG (SIGALRM);
}