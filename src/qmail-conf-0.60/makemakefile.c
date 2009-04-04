#include "auto_ucspi_tcp.h"
#include "generic-conf.h"
#include "makemakefile.h"

void makemakefile (void)
{
    start("Makefile");
    outs("SHELL=/bin/sh\n");
    outs("\n");
    outs("tcp.cdb: tcp\n");
    outs("\t"); outs(auto_ucspi_tcp); outs("/bin/tcprules tcp.cdb tcp.tmp < tcp\n");
    finish();
    perm(0644);
}
