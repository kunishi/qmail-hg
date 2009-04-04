#include "generic-conf.h"
#include "tcpserver.h"

void tcpserver_env (char *ip, char *port, char *localname)
{
    start("env/BACKLOG"); finish();
    perm(0644);
    start("env/CONCURRENCY"); finish();
    perm(0644);
    start("env/IP"); outs(ip); outs("\n"); finish();
    perm(0644);
    start("env/LOCALNAME"); outs(localname); outs("\n"); finish();
    perm(0644);
    start("env/PORT"); outs(port); outs("\n"); finish();
    perm(0644);
    start("env/REMOTEINFO"); finish();
    perm(0644);
    start("env/REMOTENAME"); finish();
    perm(0644);
}
