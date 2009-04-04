/*$Id$*/

const char FATAL[] = "ezmlm-unsub: fatal: ";
const char USAGE[] =
"ezmlm-unsub: usage: ezmlm-unsub [-h hash] [-HmMnNvV] dir [box@domain ...]";

extern void subunsub_main(int submode,
			  const char *version,
			  int argc,char **argv);

void main(int argc,char **argv)
{
  subunsub_main(0,"ezmlm-unsub version: ",argc,argv);
}
