#ifndef FOO_H_
#define FOO_H_

int byName(const FTSENT *a, const FTSENT *b);

int bySize(const FTSENT *a, const FTSENT *b);

int byATime(const FTSENT *a, const FTSENT *b);

int byMTime(const FTSENT *a, const FTSENT *b);

int byCTime(const FTSENT *a, const FTSENT *b);

int noSort(const FTSENT *a, const FTSENT *b);

#endif
