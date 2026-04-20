#include <ctype.h>

int
sb_isdigit (char *a)
{
  return isdigit (*a) || (a[0] == '-' && isdigit (a[1]));
}