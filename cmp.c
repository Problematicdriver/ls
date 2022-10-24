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
byNameRev(const FTSENT *a, const FTSENT *b) {
    return -byName(a, b);
}

int
bySize(const FTSENT *a, const FTSENT *b)
{
    struct stat *stat_a = a->fts_statp;
    struct stat *stat_b = b->fts_statp;

    return stat_a->st_size - stat_b->st_size;
}

int
bySizeRev(const FTSENT *a, const FTSENT *b)
{
    return -bySizeRev(a, b);
}

int
byATime(const FTSENT *a, const FTSENT *b)
{
    struct stat *stat_a = a->fts_statp;
    struct stat *stat_b = b->fts_statp;

    return (int) stat_a->st_atime - stat_b->st_atime;
}

int
byATimeRev(const FTSENT *a, const FTSENT *b)
{
    return -byATime(a, b);
}

int
byMTime(const FTSENT *a, const FTSENT *b)
{
    struct stat *stat_a = a->fts_statp;
    struct stat *stat_b = b->fts_statp;
    
    return (int) stat_a->st_mtime - stat_b->st_mtime;
}

int
byMTimeRev(const FTSENT *a, const FTSENT *b)
{
    return -byMTime(a, b);
}

int
byCTime(const FTSENT *a, const FTSENT *b)
{
    struct stat *stat_a = a->fts_statp;
    struct stat *stat_b = b->fts_statp;

    return (int) stat_a->st_ctime - stat_b->st_ctime;
}

int
byCTimeRev(const FTSENT *a, const FTSENT *b)
{
    return -byCTime(a, b);
}

int
noSort(const FTSENT *a, const FTSENT *b) {
    return (0);
}
