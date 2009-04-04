#include <pwd.h>
#include "auto_qmail.h"
#include "exit.h"
#include "generic-conf.h"
#include "makelogdir.h"
#include "strerr.h"

#define FATAL "qmail-delivery-conf: fatal: "

#define TEMP 111
#define PERM 100

static char *loguser;
static char *dir;
static struct passwd *pw;

static void usage (void)
{
    strerr_die1x(PERM, "qmail-delivery-conf: usage: qmail-delivery-conf logacct /qmail");
}

int main (int argc, char **argv)
{
    loguser = argv[1];
    if (!loguser) usage();
    dir = argv[2];
    if (!dir) usage();
    if (dir[0] != '/') usage();

    pw = getpwnam(loguser);
    if (!pw) strerr_die3x(TEMP, FATAL, "unknown account ", loguser);

    init(dir, FATAL);
    makelogdir(loguser, pw->pw_uid, -1);

    start("run");
    outs("#!/bin/sh\n");
    outs("exec "); outs(auto_qmail); outs("/rc\n");
    finish();
    perm(0755);

    _exit(0);
    return 0;
}
