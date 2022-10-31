#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <string.h>

#include "cmp.h"

int
byName(const FTSENT *a, const FTSENT *b)
{
    return strcmp(a->fts_name, b->fts_name); 
}

int
bySize(const FTSENT *a, const FTSENT *b)
{
    struct stat *stat_a = a->fts_statp;
    struct stat *stat_b = b->fts_statp;

    return stat_b->st_size - stat_a->st_size;
}

int
byATime(const FTSENT *a, const FTSENT *b)
{
    struct stat *stat_a = a->fts_statp;
    struct stat *stat_b = b->fts_statp;

    return (int) stat_a->st_atime - stat_b->st_atime;
}

int
byMTime(const FTSENT *a, const FTSENT *b)
{
    struct stat *stat_a = a->fts_statp;
    struct stat *stat_b = b->fts_statp;
    
    return (int) stat_a->st_mtime - stat_b->st_mtime;
}

int
byCTime(const FTSENT *a, const FTSENT *b)
{
    struct stat *stat_a = a->fts_statp;
    struct stat *stat_b = b->fts_statp;

    return (int) stat_a->st_ctime - stat_b->st_ctime;
}

int
noSort(const FTSENT *b, const FTSENT *a) {
    (void)a;
    (void)b;
    return (0);
}
