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
#include "cmp.h"
#include "print.h"


static void	 display(FTSENT *);

#define FLAGS "AacdFfhiklnqRrSstuw"

#define	BY_NAME 0
#define	BY_SIZE 1
#define	BY_TIME	2
#define NO_SORT 3

#define BUFF_SIZE 512
#define PERM_SIZE 12
#define ID_SIZE 12
#define BLKSIZE 512

/*
 * These global are set to be extern and 
 * used by printing functions
 */

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
int f_listdir;
int f_reverse;
int f_size;
int f_block;
int f_ctime;
int f_atime;
int f_raw;

int sortKey = BY_NAME;

int fts_options;

/*
 * sets flags given opts specified by user
 */
static void
setFlags(int argc, char **argv)
{
    fts_options = FTS_PHYSICAL;
    int ch;
    while ((ch = getopt(argc, argv, FLAGS)) != -1) {
        switch(ch) {
        case 'A':
            f_seeHidden = 1;
            break;
        case 'a':
            fts_options |= FTS_SEEDOT;
            break;
        case 'c':
            f_ctime = 1;
			f_atime = 0;
			break;
        case 'd':
            f_listdir = 1;
			f_recursive = 0;
			break;
        case 'F':
            f_type = 1;
			break;
        case 'f':
            sortKey = NO_SORT;
            break;
        case 'h':
            f_human = 1;
            f_kilo = 0;
            break;
        case 'i':
            f_inode = 1;
            break;
        case 'k':
            blocksize = 1024;
			f_kilo = 1;
			f_human = 0;
			break;
        case 'l':
            f_long = 1;
            break;
        case 'n':
            f_numeric = 1;
			f_long = 1;
            break;
        case 'q':
            f_noprint = 1;
            break;
        case 'R':
            f_recursive = 1;
			break;
        case 'r':
            f_reverse = 1;
            break;
        case 'S':
            sortKey = BY_SIZE;
            break;
        case 's':
            f_size = 1;
            break;
        case 't':
            sortKey = BY_TIME;
            break;
        case 'u':
            f_atime = 1;
			f_ctime = 0;
			break;
        case 'w':
            f_noprint = 0;
            break;
        }
    }

    /* output to terminal */
    if (isatty(STDOUT_FILENO)) {
        f_noprint = 1;
    }

    /* root sees all file */
    if (getuid() == 0) {
        f_seeHidden = 1;
    }
}

/*
 * cmp function choose the right compare function
 * from cmp.c given the flags
 */
int
cmp(const FTSENT **a, const FTSENT **b)
{
    int mult;
    int (*sortfcn)(const FTSENT *, const FTSENT *);
    mult = f_reverse ? -1 : 1;
    switch (sortKey) {
    case BY_NAME:
        sortfcn = byName;
        break;
    case BY_SIZE:
        sortfcn = bySize;
        break;
    case BY_TIME:
        if (f_atime) {
            sortfcn = byATime;
        } else if (f_ctime) {
            sortfcn = byCTime;
        } else {
            sortfcn = byMTime;
        }
        break;
    default:
        sortfcn = noSort;
    }
    return mult * (sortfcn(*a, *b));
}

/*
 * display() takes a linked list of FTSENT and collect
 * information nessary to print listings, then print them
 */
static void
display(FTSENT *chp)
{
    struct stat *sp;

    FTSENT *curr;

    off_t maxsize;
    blkcnt_t maxblock;
    u_int64_t btotal;
    ino_t maxinode;
    uint32_t maxnlink;
    uint32_t maxlen, maxuser, maxgroup;
    uint32_t ulen, glen;
    char nbuff[12], gbuff[12], buff[21];
    const char *user, *group;

    PRINT_PARAMS params;    // buffer sizes are saved here 
    NAMES *np;              // names are saved here

    maxinode = maxnlink = 0;
    maxlen = maxuser = maxgroup = 0;
    maxblock = btotal = maxsize = 0;
    
    if (!f_kilo)
        (void)getbsize(NULL, &blocksize);   // sys default is 512 byte
    blocksize /= BLKSIZE;                   // find blksize relative to default size

    for (curr = chp; curr; curr = curr->fts_link) {          
        if (curr->fts_info == FTS_ERR ||
                (curr->fts_name[0] == '.' && !f_seeHidden)) {
            curr->fts_number = NO_PRINT;
            continue;
        }

        sp = curr->fts_statp;
        btotal += sp->st_blocks;
        params.btotal = btotal;

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

        

        params.maxlen = maxlen;

        (void)snprintf(buff, sizeof(buff), "%llu", (unsigned long long)maxinode);
        params.s_inode = strlen(buff);

        if (f_human) {
			params.s_size = 4;
		} else {
			(void)snprintf(buff, sizeof(buff), "%lld",
			    (long long)maxsize);
			params.s_size = strlen(buff);
        }

        if (f_human) {
			params.s_block = 4;
		} else {
			(void)snprintf(buff, sizeof(buff), "%lld",
			    (long long)howmany(maxblock, blocksize));
			params.s_block = strlen(buff);
        }

        if (f_numeric || 
                (user = user_from_uid(sp->st_uid, 0)) == NULL) {
            (void)snprintf(nbuff, sizeof(nbuff), "%u", sp->st_uid);
            user = nbuff;
        }
        if ((ulen = strlen(user)) > maxuser)
            maxuser = ulen;
        
        params.s_user = maxuser;

        if (f_numeric ||
            (group = group_from_gid(sp->st_gid, 0)) == NULL) {
            (void)snprintf(gbuff, sizeof(gbuff), "%u", sp->st_gid);
            group = gbuff;
        }
        if ((glen = strlen(group)) > maxgroup)
            maxgroup = glen;
        
        params.s_group = maxgroup;
        
        if (f_human) {
            params.s_size = 4;
        } else {
            (void)snprintf(buff, sizeof(buff), "%lld", (long long)maxsize);
            params.s_size = strlen(buff);
        }

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

    printfcn(chp, &params);
        
}

int
main(int argc, char *argv[])
{
     
    setFlags(argc, argv);
    argc -= optind;
    argv += optind;

    FTS *ftsp;
	FTSENT *p, *chp;
	int ch_options;

    (void)printf("%s\n", argv[argc-1]);
    
    ch_options = 0;

    if (!argc) {
        argc = 1;
        char dot[] = ".", *dotav[] = { dot, NULL };
        argv = dotav;
    }

    if ((ftsp = fts_open(argv, fts_options, cmp)) == NULL) {
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
            
            if (p->fts_level != FTS_ROOTLEVEL)
                (void)printf("%s:\n", p->fts_path);

            if ((chp = fts_children(ftsp, ch_options)) != NULL) {
                display(chp);
            } else {
                (void)putchar('\n');
            }

            /* skip if not recursive */                
            if (!f_recursive && chp != NULL)
                (void)fts_set(ftsp, p, FTS_SKIP);
         }
    }
    (void)fts_close(ftsp);
    return EXIT_SUCCESS;
}
