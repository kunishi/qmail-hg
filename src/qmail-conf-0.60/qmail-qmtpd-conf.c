#include <pwd.h>
#include "auto_qmail.h"
#include "exit.h"
#include "generic-conf.h"
#include "makeaddrelayclient.h"
#include "makelogdir.h"
#include "makemakefile.h"
#include "maketcp.h"
#include "readme.h"
#include "stralloc.h"
#include "strerr.h"
#include "tcpserver.h"

#define FATAL "qmail-qmtpd-conf: fatal: "

#define TEMP 111
#define PERM 100

#define PORT "209"

static char *user;
static char *loguser;
static char *dir;
static char *ip;
static struct passwd *pw;
static stralloc me = {0};

static void usage (void)
{
    strerr_die1x(PERM, "qmail-qmtpd-conf: usage: qmail-qmtpd-conf acct logacct /qmtpd [ ip ]");
}

int main (int argc, char **argv)
{
    user = argv[1];
    if (!user) usage();
    loguser = argv[2];
    if (!loguser) usage();
    dir = argv[3];
    if (!dir) usage();
    if (dir[0] != '/') usage();
    ip = argv[4];
    if (!ip) ip = "0";

    pw = getpwnam(loguser);
    if (!pw) strerr_die3x(TEMP, FATAL, "unknown account ", loguser);

    if (!readme(&me)) strerr_die1(TEMP, FATAL, &readme_err);

    init(dir, FATAL);
    makelogdir(loguser, pw->pw_uid, -1);

    makedir("env");
    perm(02755);
    start("env/DATALIMIT"); outs("250000\n"); finish();
    perm(0644);
    tcpserver_env(ip, PORT, me.s);

    makemakefile();
    maketcp(0);
    makeaddrelayclient();

    start("run");
    outs("#!/bin/sh\n");
    tcpserver_run(user, PORT);
    outs("    "); outs(auto_qmail); outs("/bin/qmail-qmtpd\n");
    outs("'\n");
    finish();
    perm(0755);

    _exit(0);
    return 0;
}
