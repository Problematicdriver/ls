#ifndef LS_H
#define LS_H

#define NO_PRINT 1

extern long blocksize;
extern int f_recursive;
extern int f_seeAll;
extern int f_seeHidden;
extern int f_long;
extern int f_inode;
extern int f_type;
extern int f_human;
extern int f_kilo;
extern int f_numeric;
extern int f_noprint;
extern int f_reverse;
extern int f_size;
extern int f_block;
extern int f_ctime;
extern int f_atime;
extern int f_raw;

typedef struct {
    unsigned int maxlen;
	int s_block;
	int s_group;
	int s_inode;
	int s_nlink;
	int s_size;
	int s_user;
} PRINT_PARAMS;

typedef struct {
    char *user;
    char *group;
    char data[1];
} NAMES;

#endif
