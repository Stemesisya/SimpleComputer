
void (*stateListener) (int signal, int value) = 0;

void
sc_setStateListener (void listener (int signal, int value))
{
  stateListener = listener;
}

void
sc_notifyListener (int signal, int value)
{
  stateListener (signal, value);
}