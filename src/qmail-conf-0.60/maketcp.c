#include "generic-conf.h"
#include "maketcp.h"

void maketcp (int flagdeny)
{
    start("tcp");
    if (flagdeny) outs(":deny\n");
    finish();
    perm(0644);
}
