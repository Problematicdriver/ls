#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <util.h>
#include <pwd.h>
#include <grp.h>


#define	BY_NAME 0
#define	BY_SIZE 1
#define	BY_TIME	2

#define BUFF_SIZE 512
#define PERM_SIZE 12

int f_recursive;
int f_seeAll;
int f_seeHidden;
int f_long;

int fts_options;

void
display(const FTSENT *file)
{
        
}

int
main(int argc, char **argv)
{
    char *c, cwd[BUFF_SIZE];
    if ((c = getcwd(cwd, BUFF_SIZE)) == NULL) {
        err(EXIT_FAILURE, "getpwd");
    }
    if (argc > 1) {
        (void) strlcat(cwd, argv[argc-1], BUFF_SIZE);
    }

    argv[1] = c;

    FTS *ftsp;
	FTSENT *p, *chp;
	int ch_options, error;

    if ((ftsp = fts_open(argv, fts_options, NULL)) == NULL) {
        err(EXIT_FAILURE, "fts_open");
    } 
    while ((p = fts_read(ftsp)) != NULL) {
        if (p->fts_info & FTS_D) {
            if (p->fts_level != FTS_ROOTLEVEL &&
                    p->fts_name[0] == '.' &&
                    !f_seeHidden) {
                (void)fts_set(ftsp, p, FTS_SKIP);
            }

            if ((chp = fts_children(ftsp, ch_options)) != NULL) {
                display(chp);
            }
        }
    }
}
