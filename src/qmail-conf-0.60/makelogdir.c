#include "generic-conf.h"
#include "makelogdir.h"

void makelogdir (char *user, int uid, int gid)
{
    makedir("log");
    perm(02755);

    makedir("log/main");
    owner(uid, gid);
    perm(02755);

    start("log/status");
    finish();
    owner(uid, gid);
    perm(0644);

    makedir("log/env");
    perm(02755);

    start("log/env/MAXFILESIZE");
    finish();
    perm(0644);

    start("log/env/MAXLOGFILES");
    finish();
    perm(0644);

    start("log/env/PROCESSOR");
    finish();
    perm(0644);

    start("log/run");
    outs("#!/bin/sh\n");
    outs("exec \\\n");
    outs("setuidgid "); outs(user); outs(" \\\n");
    outs("envdir ./env \\\n");
    outs("sh -c '\n");
    outs("    exec \\\n");
    outs("    multilog \\\n");
    outs("\tt \\\n");
    outs("\t${MAXFILESIZE+\"s$MAXFILESIZE\"} \\\n");
    outs("\t${MAXLOGFILES+\"n$MAXLOGFILES\"} \\\n");
    outs("\t${PROCESSOR+\"!$PROCESSOR\"} \\\n");
    outs("\t./main\n");
    outs("'\n");
    finish();
    perm(0755);
}
