#include "sig.h"
#include "readwrite.h"
#include "exit.h"
#include "env.h"
#include "error.h"
#include "fork.h"
#include "wait.h"
#include "qmail.h"
#include "strerr.h"
#include "buffer.h"
#include "fmt.h"
#include "getln.h"
#include "mess822.h"
#include "fd.h"
#include "pathexec.h"

#define FATAL "filterto: fatal: "

struct qmail qqt;

int mywrite(fd,buf,len) int fd; char *buf; int len;
{
  qmail_put(&qqt,buf,len);
  return len;
}

char inbuf[BUFFER_INSIZE];
char outbuf[1];
buffer ssin;
buffer ssout = BUFFER_INIT(mywrite,-1,outbuf,sizeof outbuf);

char num[FMT_ULONG];

int main(int argc,char **argv,char **envp)
{
  char *sender;
  char *dtline;
  int pid;
  int wstat;
  char *qqx;
  int pf[2];

  if (!argv[1] || !argv[2])
    strerr_die1x(100,"filterto: usage: filterto newaddress program [ arg ... ]");

  sig_ignore(sig_pipe);

  if (pipe(pf) == -1) strerr_die2sys(111,FATAL,"unable to create pipe: ");
  pid = fork();
  if (pid == -1)
    strerr_die2sys(111,FATAL,"unable to fork: ");
  if (pid == 0) {
    close(pf[0]);
    if (fd_move(1,pf[1]) == -1) _exit(111);
    pathexec_run(argv[2],argv + 2,envp);
    if (error_temp(errno)) _exit(111);
    _exit(100);
  }
  close(pf[1]);

  sender = env_get("SENDER");
  if (!sender) strerr_die2x(100,FATAL,"SENDER not set");
  dtline = env_get("DTLINE");
  if (!dtline) strerr_die2x(100,FATAL,"DTLINE not set");
 
  if (qmail_open(&qqt) == -1)
    strerr_die2sys(111,FATAL,"unable to fork: ");
  qmail_puts(&qqt,dtline);

  buffer_init(&ssin,read,pf[0],inbuf,sizeof inbuf);
  if (buffer_copy(&ssout,&ssin) != 0)
    strerr_die2sys(111,FATAL,"unable to read message: ");
  if (buffer_flush(&ssout) == -1)
    strerr_die2sys(111,FATAL,"unable to write message: ");

  if (wait_pid(&wstat,pid) == -1)
    strerr_die2x(111,FATAL,"wait failed");
  if (wait_crashed(wstat))
    strerr_die2x(111,FATAL,"child crashed");
  switch(wait_exitcode(wstat)) {
    case 0: break;
    case 111: strerr_die2x(111,FATAL,"temporary child error");
    default: _exit(0);
  }
  close(pf[0]);
 
  num[fmt_ulong(num,qmail_qp(&qqt))] = 0;

  qmail_from(&qqt,sender);
  qmail_to(&qqt,argv[1]);
  qqx = qmail_close(&qqt);
  if (*qqx) strerr_die2x(*qqx == 'D' ? 100 : 111,FATAL,qqx + 1);
  strerr_die2x(99,"filterto: qp ",num);
}
