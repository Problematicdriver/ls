# ls(1)

## Description
An implementation of ls(1) command with a subset of the options available on most systems.<br /><br />

Run <strong>ls(1)</strong> with following options: <br />

<strong>−A</strong>&ensp;List all entries except for ‘.’ and ‘..’ .<br />
<strong>−a</strong>&ensp;Include directory entries whose names begin with a dot ( ‘.’ ) .<br />
<strong>−c</strong>&ensp;Use time when file status was last changed for sorting ( −t ) or printing ( −l ) .<br />
<strong>−d</strong>&ensp;Directories are listed as plain files and symbolic links in the argument list are not indirected through.<br />
<strong>−F</strong>&ensp;Display a slash ( ‘/’ ) immediately after each pathname that is a directory, an asterisk ( ‘∗’) after
each that is executable, an at sign ( ‘@’ ) after each symbolic link, a percent sign ( ‘%’ ) after each
whiteout, an equal sign ( ‘=’ ) after each socket, and a vertical bar ( ‘|’ ) after each that is a FIFO.
<strong>−f</strong>&ensp;Output is not sorted.<br />
<strong>−h</strong>&ensp;Modifies the −s and −l options, causing the sizes to be reported in bytes displayed in a human
readable format.<br />
<strong>−i</strong>&ensp;Print the file’s file serial number (inode number).<br />
<strong>−k</strong>&ensp;Modifies the −s option, causing the sizes to be reported in kilobytes. <br />
<strong>−l</strong>&ensp;(The lowercase letter “ell”). List in long format. <br />
<strong>−n</strong>&ensp;The same as −l, except that the owner and group IDs are displayed numerically rather than converting to a owner or group name. <br />
<strong>−R</strong>&ensp;Recursively list subdirectories encountered. <br />
<strong>−r</strong>&ensp;Reverse the order of the sort to get reverse lexicographical order or the smallest or oldest entries
first. <br />
<strong>−S</strong>&ensp;Sort by size, largest file first. <br />
<strong>−s</strong>&ensp;Display the number of file system blocks actually used by each file, in units of 512 bytes or BLOCKSIZE (see ENVIRONMENT). <br />
<strong>−u</strong>&ensp;Use time of last access for sorting ( −t ) or printing ( −l ) . <br />

## A Note on implementation
### Project file structure
 
- <strong>ls.c</strong> : Main file that reads input, sets flags and invoke display funciton to print listings <br />
- <strong>ls.h</strong> : Header file for ls, defining external variables and auxiliary data structures for printing <br />
- <strong>print.c</strong> : Handles printing of the listings while traversing UNIX file structure <br />
- <strong>cmp.c</strong> : Offers different options for sorting the listings <br />
    
   
    
