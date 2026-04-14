
int (*stateListener) (int signal, int value) = 0;

void
sc_setStateListener (int listener (int signal, int value))
{
  stateListener = listener;
}

int
sc_notifyListener (int signal, int value)
{
  return stateListener (signal, value);
}