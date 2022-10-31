#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/param.h>

#include <fcntl.h>
#include <fts.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>

#include "ls.h"
#include "print.h"

#define BLOCKSIZE 512
#define KILO 1024
#define DAYSPERNYEAR 365
#define SECSPERDAY 86400
#define MAX_PATHLEN 64

static int humanize_flags = HN_DECIMAL | HN_B | HN_NOSPACE; 
static time_t now;

void
printlink(FTSENT *p)
{
    int l;
    char name[MAX_PATHLEN + 1], path[MAX_PATHLEN + 1];  // room for '\0'
    if (p->fts_level == FTS_ROOTLEVEL)
		(void)snprintf(name, sizeof(name), "%s", p->fts_name);
    else
        (void)snprintf(name, sizeof(name), "%s/%s", p->fts_parent->fts_accpath, p->fts_name);
    
    if ((l = readlink(name, path, sizeof(path))) == -1) {
        err(EXIT_FAILURE, "readlink");    
    }
    path[l] = '\0';
    (void)printf(" -> %s\n", path);
}

/*
 * printtime is referenced from
 * https://github.com/NetBSD/src/blob/trunk/bin/ls/print.c
 */

void
printtime(time_t ftime)
{
    int i;
	const char *longstring;

	if ((longstring = ctime(&ftime)) == NULL) {
			   /* 012345678901234567890123 */
		longstring = "????????????????????????";
	}
	for (i = 4; i < 11; ++i)
		(void)putchar(longstring[i]);

#define	SIXMONTHS	((DAYSPERNYEAR / 2) * SECSPERDAY)
	if (ftime + SIXMONTHS > now && ftime - SIXMONTHS < now)
		for (i = 11; i < 16; ++i)
			(void)putchar(longstring[i]);
	else {
		(void)putchar(' ');
		for (i = 20; i < 24; ++i)
			(void)putchar(longstring[i]);
	}
	(void)putchar(' ');
}


void
printfcn(FTSENT *chp, PRINT_PARAMS *params)
{
    now = time(NULL);

    FTSENT *curr;
    struct stat *sp;
    NAMES *np;
    
    char buff[20], szbuff[5];

    if (f_long || f_size) {
        (void)printf("total  %llu\n", (unsigned long long)howmany(params->btotal, blocksize));
    }

    for (curr = chp; curr; curr = curr->fts_link) {
        
        if (curr->fts_number & NO_PRINT) continue;

        sp = curr->fts_statp;
        np = curr->fts_pointer;
        
        if (f_inode) {
            (void)printf("%*ld  ", params->s_inode, sp->st_ino); 
        }
        
        if (f_size) {
            if (f_human) {
                if (humanize_number(szbuff, sizeof(szbuff), sp->st_blocks * blocksize, "", 0, humanize_flags) == -1) {
                    err(EXIT_FAILURE, "humanize_number");
                }
                (void)printf("%*s  ", params->s_block, szbuff);
            } else {
                (void)printf("%*llu  ", params->s_block, 
                        (unsigned long long)howmany(sp->st_blocks, blocksize));
            }
        }

        if (f_long || f_numeric) {
            (void)strmode(sp->st_mode, buff);
            (void)printf("%s  ", buff);
        }

        if (f_long || f_numeric)
            (void)printf("%*lu ", params->s_nlink, (unsigned long)sp->st_nlink);
         
        if (f_long || f_numeric)
            (void)printf("%-*s  ", params->s_user, np->user);
    
        if (f_long || f_numeric)    
            (void)printf("%-*s  ", params->s_group, np->group);

        if (f_long || f_numeric) {
            if (f_human) {
                if ((humanize_number(szbuff, sizeof(szbuff), sp->st_size, "", 0, humanize_flags)) == -1) {
                    err(EXIT_FAILURE, "humanize_number");
                }
                (void)printf("%*s  ", params->s_size, szbuff);
            } else {
                (void)printf("%*lu  ", params->s_size, sp->st_size);
            }
        }

        if (f_long || f_numeric) {
            if (f_atime) {
                printtime(sp->st_atime);
            } else if (f_ctime) {
                printtime(sp->st_ctime);
            } else {
                printtime(sp->st_mtime);
            }
        }

        (void)printf("%s", curr->fts_name);
        
        if (f_long && f_type) {
            switch(sp->st_mode & S_IFMT) {
            case S_IFDIR:
                (void)printf("/");
                break;
            case S_IFIFO:
                (void)printf("|");
                break;
            case S_IFLNK:
                (void)printf("@");
                break;
            case S_IFSOCK:
                (void)printf("=");
                break;
            case S_IFWHT:
                (void)printf("%%");
                break;
            default:
                if (sp->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                    (void)printf("*");
                }
            }
        }

        if (f_long && S_ISLNK(sp->st_mode)) {
            printlink(curr);
        }

        (void)putchar('\n');
        
    }
    (void)putchar('\n');
}

