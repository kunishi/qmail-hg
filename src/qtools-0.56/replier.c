#include "stralloc.h"
#include "strerr.h"
#include "error.h"
#include "qmail.h"
#include "env.h"
#include "sig.h"
#include "open.h"
#include "getln.h"
#include "case.h"
#include "scan.h"
#include "str.h"
#include "fmt.h"
#include "readwrite.h"
#include "exit.h"
#include "buffer.h"
#include "getconf.h"
#include "constmap.h"
#include "fd.h"
#include "wait.h"
#include "pathexec.h"

#define FATAL "replier: fatal: "

void usage()
{
  strerr_die1x(100,"replier: usage: replier dir addr prog [ args ]");
}
void nomem()
{
  strerr_die2x(111,FATAL,"out of memory");
}
void badaddr()
{
  strerr_die2x(100,FATAL,"I do not accept messages at this address (#5.1.1)");
}

char strnum[FMT_ULONG];

stralloc fnadir = {0};
stralloc fnaf = {0};
stralloc fnsub = {0};
stralloc line = {0};

stralloc mailinglist = {0};
stralloc inlocal = {0};
stralloc outlocal = {0};
stralloc outhost = {0};
stralloc headerremove = {0};
struct constmap headerremovemap;
stralloc headeradd = {0};

struct qmail qq;
buffer ssin;
char inbuf[1024];
buffer ssout;
char outbuf[1];

int mywrite(fd,buf,len)
int fd;
char *buf;
unsigned int len;
{
  qmail_put(&qq,buf,len);
  return len;
}

void put(buf,len) char *buf; int len;
{
  qmail_put(&qq,buf,len);
}

void puts(buf) char *buf;
{
  qmail_puts(&qq,buf);
}

void sigalrm()
{
  strerr_die1x(111,"Timeout on maildir delivery. (#4.3.0)");
}

stralloc mydtline = {0};

int main(int argc,char **argv)
{
  int fd;
  char *dir;
  char *addr;
  int fdlock;
  char *sender;
  char *local;
  char *action;
  int flagmlwasthere;
  int match;
  int i;
  char ch;
  int flaginheader;
  int flagbadfield;
  char *qqx;
  int pf[2];
  int pid;
  int wstat;

  dir = argv[1];
  if (!dir) usage();
  addr = argv[2];
  if (!addr) usage();

  umask(022);
  sig_ignore(sig_pipe);
  sig_catch(sig_alarm,sigalrm);
  alarm(86400);

  if (chdir(dir) == -1)
    strerr_die4sys(111,FATAL,"unable to switch to ",dir,": ");

  sender = env_get("SENDER");
  if (sender) {
    if (!*sender)
      strerr_die2x(100,FATAL,"I don't reply to bounce messages (#5.7.2)");
    if (str_equal(sender,"#@[]"))
      strerr_die2x(100,FATAL,"I don't reply to bounce messages (#5.7.2)");
  }

  local = env_get("LOCAL");
  if (!local)
    strerr_die2sys(111,FATAL,"LOCAL not set");
  getconf_line(&inlocal,"inlocal",1,FATAL,dir);
  if (inlocal.len > str_len(local)) badaddr();
  if (case_diffb(inlocal.s,inlocal.len,local)) badaddr();
  action = local + inlocal.len;
  if (*action != '-') badaddr();
  ++action;

  if (pipe(pf) == -1)
    strerr_die2sys(111,FATAL,"unable to create pipe: ");
  pid = fork();
  if (pid == -1)
    strerr_die2sys(111,FATAL,"unable to fork: ");
  if (pid == 0) {
    close(pf[0]);
    if (fd_move(1,pf[1]) == -1) _exit(111);

    if (!pathexec_env("REQUEST",action)) _exit(111);
    action += str_chr(action,'-'); if (*action) ++action;
    if (!pathexec_env("REQUEST2",action)) _exit(111);
    action += str_chr(action,'-'); if (*action) ++action;
    if (!pathexec_env("REQUEST3",action)) _exit(111);
    action += str_chr(action,'-'); if (*action) ++action;
    if (!pathexec_env("REQUEST4",action)) _exit(111);

    pathexec(argv + 3);
    if (error_temp(errno)) _exit(111);
    _exit(100);
  }
  close(pf[1]);
  
  if (qmail_open(&qq) == -1)
    strerr_die2sys(111,FATAL,"unable to run qmail-queue: ");

  getconf_line(&outhost,"outhost",1,FATAL,dir);
  getconf_line(&outlocal,"outlocal",1,FATAL,dir);
  getconf_line(&mailinglist,"mailinglist",1,FATAL,dir);

  getconf(&headerremove,"headerremove",1,FATAL,dir);
  constmap_init(&headerremovemap,headerremove.s,headerremove.len,0);

  getconf(&headeradd,"headeradd",1,FATAL,dir);
  for (i = 0;i < headeradd.len;++i)
    if (!headeradd.s[i])
      headeradd.s[i] = '\n';

  if (!stralloc_copys(&mydtline,"Delivered-To: replier ")) nomem();
  if (!stralloc_cat(&mydtline,&outlocal)) nomem();
  if (!stralloc_cats(&mydtline,action)) nomem();
  if (!stralloc_cats(&mydtline,"@")) nomem();
  if (!stralloc_catb(&mydtline,outhost.s,outhost.len)) nomem();
  if (!stralloc_cats(&mydtline,"\n")) nomem();

  puts("Mailing-List: ");
  put(mailinglist.s,mailinglist.len);
  puts("\n");

  put(headeradd.s,headeradd.len);
  put(mydtline.s,mydtline.len);

  flagmlwasthere = 0;
  flaginheader = 1;
  flagbadfield = 0;

  buffer_init(&ssin,read,pf[0],inbuf,sizeof(inbuf));

  for (;;) {
    if (getln(&ssin,&line,&match,'\n') == -1)
      strerr_die2sys(111,FATAL,"unable to read input: ");

    if (flaginheader && match) {
      if (line.len == 1)
	flaginheader = 0;
      if ((line.s[0] != ' ') && (line.s[0] != '\t')) {
	flagbadfield = 0;
	if (constmap(&headerremovemap,line.s,byte_chr(line.s,line.len,':')))
	  flagbadfield = 1;
	if (case_startb(line.s,line.len,"mailing-list:"))
	  flagmlwasthere = 1;
	if (line.len == mydtline.len)
	  if (!byte_diff(line.s,line.len,mydtline.s))
            strerr_die2x(100,FATAL,"this message is looping: it already has my Delivered-To line (#5.4.6)");
      }
    }

    if (!(flaginheader && flagbadfield))
      put(line.s,line.len);

    if (!match)
      break;
  }

  if (flagmlwasthere)
    strerr_die2x(100,FATAL,"message already has Mailing-List (#5.7.2)");

  if (!stralloc_copy(&line,&outlocal)) nomem();
  if (!stralloc_cats(&line,"return-@")) nomem();
  if (!stralloc_cat(&line,&outhost)) nomem();
  if (!stralloc_0(&line)) nomem();
  qmail_from(&qq,line.s);

  buffer_init(&ssout,mywrite,-1,outbuf,sizeof(outbuf));
  if (buffer_copy(&ssout,&ssin) != 0)
    strerr_die2sys(111,FATAL,"unable to read input: ");
  if (buffer_flush(&ssout) == -1)
    strerr_die2sys(111,FATAL,"unable to write output: ");

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

  strnum[fmt_ulong(strnum,qmail_qp(&qq))] = 0;

  qmail_to(&qq,addr);
  qqx = qmail_close(&qq);
  if (*qqx) strerr_die2x(*qqx == 'D' ? 100 : 111,FATAL,qqx + 1);
  strerr_die2x(99,"replier: qp ",strnum);
}
