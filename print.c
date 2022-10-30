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


static int humanize_flags = HN_DECIMAL | HN_B | HN_NOSPACE; 
static time_t now;

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
print_long(FTSENT *chp, PRINT_PARAMS *params)
{
    now = time(NULL);

    FTSENT *curr;
    struct stat *sp;
    NAMES *np;
    
    char *tz, *blksize, buff[20], szbuff[5];

    if ((tz = getenv("TZ")) == NULL) {
        // err(EXIT_FAILURE, "getenv");        
    } 
    

    for (curr = chp; curr; curr = curr->fts_link) {
        
        if (curr->fts_number & NO_PRINT) continue;

        sp = curr->fts_statp;
        np = curr->fts_pointer;

        // prints inode
        if (f_inode) {
            (void)printf("%*s  ", params->s_inode, sp->st_ino); 
        }
        
        // prints blk size in short listing
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
        // prints file mode
        (void)strmode(sp->st_mode, buff);
        (void)printf("%s  ", buff);

        // prints nlinks
        (void)printf("%*lu ", params->s_nlink, (unsigned long)sp->st_nlink);
         
        // prints owner
        // numerical
        if (f_numeric) {
            
        } else {
            (void)printf("%-*s  ", params->s_user, np->user);
        }
        
        // prints group    
        // numerical
        if (f_numeric) {

        } else {
            (void)printf("%-*s  ", params->s_group, np->group);
        }

        // prints size
        // in human readable or bytes
        if (f_human) {
            if ((humanize_number(szbuff, sizeof(szbuff), sp->st_size, "", 0, humanize_flags)) == -1) {
                err(EXIT_FAILURE, "humanize_number");
            }
            (void)printf("%*s  ", params->s_size, szbuff);
        } else {
            (void)printf("%*llu  ", params->s_size, sp->st_size);
        } 

        // prints time
        // show atime or ctime, or mtime by default
        if (f_atime) {
            printtime(sp->st_atime);
        } else if (f_ctime) {
            printtime(sp->st_ctime);
        } else {
            printtime(sp->st_mtime);
        }

        // prints path
        (void)printf("%s\n", curr->fts_name); 
    }
}

