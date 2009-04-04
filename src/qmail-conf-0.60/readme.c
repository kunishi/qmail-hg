#include "auto_qmail.h"
#include "byte.h"
#include "open.h"
#include "openreadclose.h"
#include "readme.h"
#include "stralloc.h"
#include "strerr.h"

struct strerr readme_err;

int readme (stralloc *sa)
{
    int fddir;

    fddir = open_read(".");
    if (fddir == -1)
	STRERR_SYS(0, readme_err, "unable to switch to current directory: ")
    if (chdir(auto_qmail) == -1)
	STRERR_SYS3(0, readme_err, "unable to switch to ", auto_qmail, ": ")
    if (openreadclose("control/me", sa, 64) != 1)
	STRERR_SYS3(0, readme_err, "unable to read ", auto_qmail, "/control/me: ")
    sa->len = byte_chr(sa->s, sa->len, '\n');
    if (!stralloc_0(sa)) STRERR(0, readme_err, "out of memory")
    if (fchdir(fddir) == -1)
	STRERR_SYS(0, readme_err, "unable to switch to starting directory: ")
    close(fddir);
    return 1;
}
