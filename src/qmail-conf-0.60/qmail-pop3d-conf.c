#include <pwd.h>
#include "auto_qmail.h"
#include "exit.h"
#include "generic-conf.h"
#include "makeaddclient.h"
#include "makelogdir.h"
#include "makemakefile.h"
#include "maketcp.h"
#include "strerr.h"
#include "tcpserver.h"

#define FATAL "qmail-pop3d-conf: fatal: "

#define TEMP 111
#define PERM 100

#define PORT "110"

static char *checkpassword;
static char *loguser;
static char *dir;
static char *ip;
static struct passwd *pw;

static void usage (void)
{
    strerr_die1x(PERM, "qmail-pop3d-conf: usage: qmail-pop3d-conf checkpassword logacct /pop3d [ ip ]");
}

int main (int argc, char **argv)
{
    checkpassword = argv[1];
    if (!checkpassword) usage();
    loguser = argv[2];
    if (!loguser) usage();
    dir = argv[3];
    if (!dir) usage();
    if (dir[0] != '/') usage();
    ip = argv[4];
    if (!ip) ip = "0";

    pw = getpwnam(loguser);
    if (!pw) strerr_die3x(TEMP, FATAL, "unknown account ", loguser);

    init(dir, FATAL);
    makelogdir(loguser, pw->pw_uid, -1);

    makedir("env");
    perm(02755);
    start("env/DATALIMIT"); outs("250000\n"); finish();
    perm(0644);
    tcpserver_env(ip, PORT, "0");
    start("env/POPUPHOST"); finish();
    perm(0644);
    start("env/MAILDIRNAME"); finish();
    perm(0644);

    makemakefile();
    maketcp(1);
    makeaddclient();

    start("run");
    outs("#!/bin/sh\n");
    tcpserver_run((char *) 0, PORT);
    outs("    "); outs(auto_qmail); outs("/bin/qmail-popup \"${POPUPHOST-`sed 1q "); outs(auto_qmail); outs("/control/me`}\" \\\n");
    outs("    "); outs(checkpassword); outs(" \\\n");
    outs("    "); outs(auto_qmail); outs("/bin/qmail-pop3d \"${MAILDIRNAME-Maildir}\"\n");
    outs("'\n");
    finish();
    perm(0755);

    _exit(0);
    return 0;
}
