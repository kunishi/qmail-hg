/*$Id$*/

#include "strerr.h"
#include "errtxt.h"
#include "die.h"

void die_sender(void)
{
  strerr_die2x(100,FATAL,ERR_NOSENDER);
}
