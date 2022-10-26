#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fts.h>

#include "ls.h"


void
print_long(FTSENT *chp, const PRINT_PARAMS *params)
{
    FTSENT *curr;
    struct stat *sp;

    for (curr = chp; curr; curr = curr->fts_link) {
        
        // prints inode
        if (f_inode) {
        
        }
        
        // prints blk size in short listing
        if (f_size) {
            
            // in human readable, kilo or blk count
            if (f_kilo) {
            
            } 
            else if (f_human) {
            
            } else {
            
            }
        }

        // prints file name
        

        // prints owner
        if (f_long) {

            // numerical
            if (f_numeric) {
            
            } else {
            
            }
        }
        
        // prints group
        if (f_long) {
            
            // numerical
            if (f_numeric) {
                
            } else {
            
            }
        }

        // prints size
        if (f_long) {
            
            // in human readable or bytes
            if (f_human) {
            
            } else {
                
            } 
        }

        // prints time
        if (f_long) {
            
            // show atime or ctime, or mtime by default
            if (f_atime) {
            
            } else if (f_mtime) {
            
            } else {
            
            }
        }
        
        // prints path
        if (f_long) {
        
        }
    }
}
