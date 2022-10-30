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
#include "print.h"

static void	 display(FTSENT *);

#define	BY_NAME 0
#define	BY_SIZE 1
#define	BY_TIME	2

#define BUFF_SIZE 512
#define PERM_SIZE 12
#define ID_SIZE 12
#define BLKSIZE 512

long blocksize;
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
int f_ctime;
int f_atime;
int f_raw;

int fts_options;

/*
 * for debug only
 */
static void
_setFlags()
{
    f_long = 1;
    f_size = 1;
    f_kilo = 1;
    blocksize = 1024;
}

static void
setFlags(int argc, char **argv)
{
    
}

static void
_display(FTSENT *chp) 
{
    FTSENT *curr;

    for (curr = chp; curr; curr = curr->fts_link) {
        printf("%s\n", curr->fts_name);
    }
}

static void
display(FTSENT *chp)
{
    /*
     * find the maximum length for
     * inode, nlinks, user name, group name, uid, gid 
     */
    struct stat *sp;

    FTSENT *curr;

    off_t maxsize;
    blkcnt_t maxblock;
    ino_t maxinode;
    uint32_t maxnlink;
    int maxlen, maxuser, maxgroup;
    int ulen, glen, nulen, nglen;
    char nbuff[12], gbuff[12], buff[21];
    const char *user, *group;

    PRINT_PARAMS params; 
    NAMES *np;

    maxinode = maxnlink = 0;
    maxlen = maxuser = maxgroup = 0;
    maxblock = maxsize = 0;
    
    if (!f_kilo)
        (void)getbsize(NULL, &blocksize);
    blocksize /= BLKSIZE;

    for (curr = chp; curr; curr = curr->fts_link) {          
        if (curr->fts_info == FTS_ERR ||
                (curr->fts_name[0] == '.' && !f_seeHidden)) {
            curr->fts_number = NO_PRINT;
            continue;
        }

        sp = curr->fts_statp;

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

        

        // params.maxlen
        params.maxlen = maxlen;

        // params.s_inode
        (void)snprintf(buff, sizeof(buff), "%llu", (unsigned long long)maxinode);
        params.s_inode = strlen(buff);

        // params.s_size and s_block
        if (f_human) {
			params.s_size = 4; /* min buf length for humanize_number */
		} else {
			(void)snprintf(buff, sizeof(buff), "%lld",
			    (long long)maxsize);
			params.s_size = strlen(buff);
        }

        if (f_human) {
			params.s_block = 4; /* min buf length for humanize_number */
		} else {
			(void)snprintf(buff, sizeof(buff), "%lld",
			    (long long)howmany(maxblock, blocksize));
			params.s_block = strlen(buff);
        }

        // params.s_user
        if (f_numeric || 
                (user = user_from_uid(sp->st_uid, 0)) == NULL) {
            (void)snprintf(nbuff, sizeof(nbuff), "%u", sp->st_uid);
            user = nbuff;
        }
        if ((ulen = strlen(user)) > maxuser)
            maxuser = ulen;
        
        params.s_user = maxuser;

        // params.s_group
        if (f_numeric ||
            (group = group_from_gid(sp->st_gid, 0)) == NULL) {
            (void)snprintf(gbuff, sizeof(gbuff), "%u", sp->st_gid);
            group = gbuff;
        }
        if ((glen = strlen(group)) > maxgroup)
            maxgroup = glen;
        
        params.s_group = maxgroup;
        
        // params.s_size
        if (f_human) {
            params.s_size = 4;
        } else {
            (void)snprintf(buff, sizeof(buff), "%lld", (long long)maxsize);
            params.s_size = strlen(buff);
        }

        // params.s_nlink
        (void)snprintf(buff, sizeof(buff), "%u", maxnlink);
        params.s_nlink = strlen(buff);

        if ((np = malloc(sizeof(NAMES) + ulen + glen + 2)) == NULL)
            err(EXIT_FAILURE, NULL);

        np->user = &np->data[0];
        (void)strcpy(np->user, user);
        np->group = &np->data[ulen + 1];
        (void)strcpy(np->group, group);

        curr->fts_pointer = np;    
    }

    print_long(chp, &params);
        
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
    
    _setFlags();

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
                break;
            }

            (void)printf("content of %s ...\n", p->fts_name);
            if ((chp = fts_children(ftsp, ch_options)) != NULL) {
                display(chp);
            }
            
            if (!f_recursive && chp != NULL)
                (void)fts_set(ftsp, p, FTS_SKIP);
            break;
        }
    }
    (void)fts_close(ftsp);
    return EXIT_SUCCESS;
}
