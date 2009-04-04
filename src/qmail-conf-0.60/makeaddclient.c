#include "generic-conf.h"
#include "makeaddclient.h"

void makeaddclient (void)
{
    start("add-client");
    outs("#!/bin/sh\n");
    outs("test $# = 0 && exit\n");
    outs("cp tcp tcp.new\n");
    outs("chmod 644 tcp.new\n");
    outs("for address in \"$@\"\n");
    outs("  do\n");
    outs("    echo \"$address\"':allow'\n");
    outs("  done >> tcp.new\n");
    outs("mv tcp.new tcp\n");
    finish();
    perm(0755);
}
