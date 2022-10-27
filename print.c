#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <fts.h>
#include <stdlib.h>

#include "ls.h"

#define BLOCKSIZE 512
#define KILO 1024

void


void
print_long(FTSENT *chp, const PRINT_PARAMS *params)
{
    FTSENT *curr;
    struct stat *sp;
    
    char *tz, *blksize, buff[20], szbuff[5];
    int blocksize, size_in_kilo;

    int humanize_flags = HN_DECIMAL | HN_B | HN_NOSPACE;
    
    if ((tz = getenv("TZ")) == NULL) {
        err(EXIT_FAILURE, "getenv");        
    }

    if ((blksize = getenv("BLOCKSIZE")) == NULL) {
        blocksize = atoi(blksize);     
    } else {
        blocksize = BLOCKSIZE;
    }
    

    for (curr = chp; curr; curr = curr->fts_link) {
        sp = curr->fts_statp;

        // prints inode
        if (f_inode) {
            (void)printf("%*s  ", params->s_inode, sp->st_ino); 
        }
        
        // prints blk size in short listing
        if (f_size) {
            
            // in human readable, kilo or blk count
            if (f_kilo) {
                size_in_kilo = sp->st_blocks * BLOCK_SIZE / KILO;
                (void)printf("%*d  ", params->s_block, size_in_kilo);
            } else if (f_human) {
                if (humanize_number(szbuff, sizeof(szbuff), sp->st_blocks * blocksize, "", 0, humanize_flags) == -1) {
                    err(EXIT_FAILURE, "humanize_number");
                }
                (void)printf("%*s ", dp->s_block, szbuff);
            } else {
                (void)printf("%*d  ", params->s_block, sp->st_blocks);
            }
        }
        // prints file mode
        (void)strmode(sp->st_mode, buf);

        // prints nlinks
        (void)printf("%*lu ", dp->s_nlink, (unsigned long)sp->st_nlink);
         
        // prints owner
        // numerical
        if (f_numeric) {
            
        } else {
            (void)printf("%-*s  ", dp->s_user, );
        }
        
        // prints group    
        // numerical
        if (f_numeric) {
            
        } else {
        
        }

        // prints size
            
        // in human readable or bytes
        if (f_human) {
        
        } else {
            
        } 

        // prints time
        // show atime or ctime, or mtime by default
        if (f_atime) {
        
        } else if (f_mtime) {
        
        } else {
        
        }
    
        // prints path
        
    }
}


