# ls(1) Command

## DESCRIPTION
<p>
An implementation of ls(1) command with a subset of the options available on most systems.<br /><br />

Run <strong>ls(1)</strong> with following options: <br />
</p>

<p>
<strong>−A</strong>&ensp;&ensp;List all entries except for ‘.’ and ‘..’ .<br />
<strong>−a</strong>&ensp;&ensp;Include directory entries whose names begin with a dot ( ‘.’ ) .<br />
<strong>−c</strong>&ensp;&ensp;Use time when file status was last changed for sorting ( −t ) or printing ( −l ) .<br />
<strong>−d</strong>&ensp;&ensp;Directories are listed as plain files and symbolic links in the argument list are not indirected through.<br />
<strong>−F</strong>&ensp;&ensp;Display a slash ( ‘/’ ) immediately after each pathname that is a directory, an asterisk ( ‘∗’) after
each that is executable, an at sign ( ‘@’ ) after each symbolic link, a percent sign ( ‘%’ ) after each
whiteout, an equal sign ( ‘=’ ) after each socket, and a vertical bar ( ‘|’ ) after each that is a FIFO.
<strong>−f</strong>&ensp;&ensp;Output is not sorted.<br />
<strong>−h</strong>&ensp;&ensp;Modifies the −s and −l options, causing the sizes to be reported in bytes displayed in a human
readable format.<br />
<strong>−i</strong>&ensp;&ensp;Print the file’s file serial number (inode number).<br />
<strong>−k</strong>&ensp;&ensp;Modifies the −s option, causing the sizes to be reported in kilobytes. <br />
<strong>−l</strong>&ensp;&ensp;(The lowercase letter “ell”). List in long format. <br />
<strong>−n</strong>&ensp;&ensp;The same as −l, except that the owner and group IDs are displayed numerically rather than converting to a owner or group name. <br />
<strong>−R</strong>&ensp;&ensp;Recursively list subdirectories encountered. <br />
<strong>−r</strong>&ensp;&ensp;Reverse the order of the sort to get reverse lexicographical order or the smallest or oldest entries
first. <br />
<strong>−S</strong>&ensp;&ensp;Sort by size, largest file first. <br />
<strong>−s</strong>&ensp;&ensp;Display the number of file system blocks actually used by each file, in units of 512 bytes or BLOCKSIZE (see ENVIRONMENT). <br />
<strong>−u</strong>&ensp;&ensp;Use time of last access for sorting ( −t ) or printing ( −l ) . <br />
</p>

## A Note on Implementation
### Project file structure
 
- <strong>ls.c</strong> : Main file that reads input, sets flags and invoke display funciton to print listings <br />
- <strong>ls.h</strong> : Header file for ls, defining external variables and auxiliary data structures for printing <br />
- <strong>print.c</strong> : Handles printing of the listings while traversing UNIX file structure <br />
- <strong>cmp.c</strong> : Offers different options for sorting the listings <br />

### Library Usage
- <strong>fts(3)</strong> : Used for traversing UNIX filestructure and sorting, while handling flags such as -R -A
- <strong>stat(2)</strong> : Stat struct provides necessary information that printing in longform needs
- <strong>user_from_uid(3)</strong> : Gets username from uid
- <strong>group_from_gid(3)</strong> : Gets groupname from gid
- <strong>isatty(3)</strong> : Gets name of associated terminal (tty) from file descriptor
- <strong>strmode(3)</strong> : Converts inode status information into a symbolic string
- <strong>humanize_number(3)</strong> : format a number into a human readable form and vice versa
    
## STANDARDS
<p> The ls utility is expected to be a superset of the IEEE Std 1003.2 (“POSIX.2”) specification. </p>

## How to run
    ./ls [-1AaBbCcdFfghikLlMmnOoPpqRrSsTtuWwXx] [file ...]
<p> Example </p>

    make
    ./ls -l testdir
    
