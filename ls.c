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

#include "ls.h"

#define NO_PRINT 1

#define	BY_NAME 0
#define	BY_SIZE 1
#define	BY_TIME	2

#define BUFF_SIZE 512
#define PERM_SIZE 12
#define ID_SIZE 12

int f_recursive;
int f_seeAll;
int f_seeHidden;
int f_long;
int f_inode;
int f_type;
int f_human;
int f_kilo;
int f_numeric;
int f_noprint;
int f_reverse;
int f_size;
int f_block;
int f_mtime;
int f_atime;
int f_raw;

int fts_options;

void
display(FTSENT *chp)
{
    /*
     * find the maximum length for
     * inode, nlinks, oname, gname, uid, gid 
     */
    
    struct stat *sp;

    FTSENT *curr;

    off_t maxsize;
    blkcnt_t maxblock;
    ino_t maxinode;
    uint32_t maxnlink;
    int maxlen, max_oname, max_gname, max_uid, max_gid;
    int olen, glen, nolen, nglen;
    char uid[ID_SIZE], gid[ID_SIZE];
    const char *owner, *group;

    for (curr = chp; curr; curr = curr->fts_link) {
        if (curr->fts_info == FTS_ERR) {
            curr->fts_number = NO_PRINT;
            continue;
        }

        if (sp->st_size > maxsize)
			maxsize = sp->st_size;
        
        if (sp->st_blocks > maxblock)
			maxblock = sp->st_blocks;
        
        if (sp->st_ino > maxinode)
			maxinode = sp->st_ino;
        
        if (sp->st_nlink > maxnlink)
			maxnlink = sp->st_nlink;

        if (curr->fts_namelen > maxlen)
			maxlen = curr->fts_namelen;
        
        owner = user_from_uid(sp->st_uid, 0);

        if ((olen = strlen(owner)) > max_oname)
			max_oname = olen;

        group = group_from_gid(sp->st_gid, 0);

        if ((glen = strlen(group)) > max_gname)
            max_gname = glen;

        (void)snprintf(uid, sizeof(uid),"%u", sp->st_uid);
        
        if ((nolen = strlen(uid)) > max_uid) {
            max_uid = nolen;
        }
        
        (void)snprintf(gid, sizeof(gid),"%u", sp->st_gid);

        if ((nglen = strlen(gid)) > max_gid) {
            max_gid = nglen;
        }
        
    }    
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
