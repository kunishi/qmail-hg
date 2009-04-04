/*$Id$*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "env.h"
#include "strerr.h"
#include "sgetopt.h"
#include "substdio.h"
#include "subfd.h"
#include "errtxt.h"
#include "error.h"
#include "byte.h"
#include "fmt.h"
#include "str.h"
#include "stralloc.h"
#include "qmail.h"
#include "seek.h"
#include "wrap.h"
#include "slurp.h"
#include "die.h"
#include "auto_version.h"

const char FATAL[] = "ezmlm-dispatch: fatal: ";
const char USAGE[] =
"ezmlm-dispatch: usage: ezmlm-dispatch dir [list]";

static const char *sender;
static stralloc basedir;

static struct qmail qq;
static char strnum[FMT_ULONG];
static int did_program;
static int did_forward;

static int is_file(const char *fn)
{
  struct stat st;
  return wrap_stat(fn,&st) == 0
    && S_ISREG(st.st_mode);
}

static void forward(const char *rcpt)
{
  char buf[4096];
  const char *dtline;
  const char *err;
  int r;
  if (qmail_open(&qq,0) == -1)
    strerr_die2sys(111,FATAL,ERR_QMAIL_QUEUE);
  if ((dtline = env_get("DTLINE")) != 0)
    qmail_puts(&qq,dtline);
  while ((r = read(0,buf,sizeof buf)) > 0)
    qmail_put(&qq,buf,r);
  if (r == -1)
    strerr_die3sys(111,FATAL,ERR_READ,": ");
  qmail_from(&qq,sender);
  qmail_to(&qq,rcpt);
  if (*(err = qmail_close(&qq)) != '\0')
    strerr_die3x(111,FATAL,ERR_TMP_QMAIL_QUEUE,err + 1);
  strnum[fmt_ulong(strnum,qmail_qp(&qq))] = 0;
  substdio_puts(subfderr,"qp ");
  substdio_puts(subfderr,strnum);
  substdio_putsflush(subfderr,"\n");
  ++did_forward;
}

static int execute_line(const char *fn,const char *line)
{
  int child;
  int code;
  if (seek_begin(0) == -1)
    strerr_die2sys(111,FATAL,ERR_SEEK_INPUT);
  while (*line
	 && (*line == '\t'
	     || *line == ' '))
      ++line;
  switch (*line) {
  case '#':
    /* ignore comments */
    return 0;
  case '|':
    if ((child = wrap_fork()) == 0)
      wrap_execsh(line+1);
    code = wrap_waitpid(child);
    ++did_program;
    return code;
  case '/':
  case '.':
    strerr_die5x(100,FATAL,basedir.s,"/",fn,": Delivery to files is not supported.");
    return 100;
  default:
    if (*line == '&')
      ++line;
    forward(line);
    return 0;
  }
}

static int execute_file(const char *fn,stralloc *file)
{
  unsigned int start;
  unsigned int end;
  unsigned int len;
  int code;
  for (start = 0; start < file->len; start = end + 1) {
    len = byte_chr(file->s+start,file->len-start,'\n');
    end = start + len;
    file->s[end] = 0;
    if ((code = execute_line(fn,file->s+start)) != 0)
      return code;
  }
  return 0;
}

static void execute(const char *fn,const char *def)
     /* Load and execute a qmail command file. */
{
  stralloc file = {0,0,0};
  int code;
  if (def != 0)
    env_put2("DEFAULT",def);
  else
    env_unset("DEFAULT");
  if (slurp(fn,&file,256) != 1)
    strerr_die6sys(111,FATAL,ERR_READ_INPUT,basedir.s,"/",fn,": ");
  code = execute_file(fn,&file);
  substdio_puts(subfderr,"did 0+");
  substdio_put(subfderr,strnum,fmt_ulong(strnum,did_forward));
  substdio_puts(subfderr,"+");
  substdio_put(subfderr,strnum,fmt_ulong(strnum,did_program));
  substdio_putsflush(subfderr,"\n");
  _exit(code);
}

static void try_dispatch(const char *def,const char *prefix,unsigned int len,
			 const char *fn)
     /* Test if def has a certain prefix, and if so execute the
      * associated file. */
{
  if (str_diffn(def,prefix,len) == 0
      && is_file(fn))
    execute(fn,def+len);
}

static void dispatch(const char *dir,const char *def)
     /* Hand off control to one of the command files in the list directory. */
{
  if (!stralloc_append(&basedir,"/")) die_nomem();
  if (!stralloc_cats(&basedir,dir)) die_nomem();
  /* FIXME: set up $EXT $EXT2 $EXT3 $EXT4 ?  Is it feasable? */
  if (def == 0)
    execute("editor",0);
  else if (str_diff(def,"owner") == 0)
    execute("owner",0);
  else if (str_diff(def,"digest-owner") == 0)
    execute("owner",0);
  else {
    try_dispatch(def,"digest-return-",14,"bouncer");
    try_dispatch(def,"return-",7,"bouncer");
    try_dispatch(def,"confirm-",8,"confirmer");
    try_dispatch(def,"discard-",8,"confirmer");
    try_dispatch(def,"accept-",7,"moderator");
    try_dispatch(def,"reject-",7,"moderator");
    /* If nothing else matches, call the manager. */
    execute("manager",def);
  }
}

void main(int argc,char **argv)
{
  char *def;
  int opt;
  int dash;
  
  while ((opt = getopt(argc,argv,"vV")) != opteof)
    switch (opt) {
    case 'v':
    case 'V':
      strerr_die2x(0, "ezmlm-dispatch version: ",auto_version);
    default:
      die_usage();
    }

  if (argv[optind] == 0)
    die_usage();
  wrap_chdir(argv[optind]);
  if (!stralloc_copys(&basedir,argv[optind++])) die_nomem();

  sender = env_get("SENDER");
  if (!sender)
    strerr_die2x(100,FATAL,ERR_NOSENDER);
  def = env_get("DEFAULT");

  if (argv[optind] != 0) {
    wrap_chdir(argv[optind]);
    dispatch(argv[optind],def);
  }
  else if (!def || !*def)
    strerr_die2x(100,FATAL,ERR_NODEFAULT);
  else {
    if (def[str_chr(def,'/')] != 0)
      strerr_die2x(100,FATAL,"Recipient address may not contain '/'");

    if (chdir(def) == 0)
      dispatch(def,0);

    dash = str_len(def);
    for (;;) {
      while (--dash > 0)
	if (def[dash] == '-')
	  break;
      if (dash <= 0)
	break;
      def[dash] = 0;
      if (chdir(def) == 0)
	dispatch(def,def+dash+1);
      def[dash] = '-';
    }
    strerr_die3x(100,FATAL,"Could not match recipient name to any list: ",def);
  }
}
