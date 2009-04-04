#include "auto_ucspi_tcp.h"
#include "generic-conf.h"
#include "tcpserver.h"

void tcpserver_run (char *user, char *port)
{
    outs("exec 2>&1 \\\n");
    outs("envdir ./env \\\n");
    outs("sh -c '\n");
    outs("    case \"$REMOTENAME\" in h) H=;; p) H=p;; *) H=H;; esac\n");
    outs("    case \"$REMOTEINFO\" in r) R=;; [0-9]*) R=\"t$REMOTEINFO\";; *) R=R;; esac\n");
    outs("    exec \\\n");
    if (user) {
	outs("    envuidgid "); outs(user); outs(" \\\n");
    }
    outs("    softlimit ${DATALIMIT+\"-d$DATALIMIT\"} \\\n");
    outs("    "); outs(auto_ucspi_tcp); outs("/bin/tcpserver \\\n");
    outs("\t-vD"); if (user) outs("U"); outs("\"$H$R\" \\\n");
    outs("\t${LOCALNAME+\"-l$LOCALNAME\"} \\\n");
    outs("\t${BACKLOG+\"-b$BACKLOG\"} \\\n");
    outs("\t${CONCURRENCY+\"-c$CONCURRENCY\"} \\\n");
    outs("\t-xtcp.cdb \\\n");
    outs("\t-- \"${IP-0}\" \"${PORT-"); outs(port); outs("}\" \\\n");
}
