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

int f_recursive;
int f_seeAll;
int f_seeHidden;
int f_long;


int
display(const FTSENT *file)
{

    off_t maxsize;
    blkcnt_t maxblk;
    ino_t maxinode;
    nlink_t maxnlink;
    int maxuser, maxgroup;
    char *g;

    FTSENT *p = file;
    while (p != NULL) {
        struct stat *statp = p->fts_statp;
        if (statp->st_size > maxsize) maxsize = statp->st_size;
        if (statp->st_blocks > maxblk) maxblk = statp->st_blocks;
        if (statp->st_ino > maxinode) maxinode = statp->st_ino;
        if (statp->st_nlink > maxnlink) maxnlink = statp->st_nlink;
        if (p->fts_namelen > maxuser) maxuser = p->fts_namelen;
        g = group_from_gid(sp->st_gid, 0);
        if (strlen(g) > maxgroup) maxgroup = strlen(g);
    }

    p = file;
    while (p != NULL) {
        if (f_inode == 1) {
            sprintf();
        }

        if (f_long == 1) {
        
        }
    }
}

int
main(int argc, char **argv)
{
    char *c, cwd[BUFF_SIZE];
    if ((c = getpwd(cwd, BUFF_SIZE)) == NULL) {
        err(EXIT_FAILURE, "getpwd");
    }
    if (argc > 1) {
        (void) strlcat(cwd, argv[argc-1], BUFF_SIZE);
    }

    FTS *ftsp;
	FTSENT *p, *chp;
	int ch_options, error;

    if ((ftsp = fts_open(cwd, fts_options, cmp)) == NULL) {
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
