#ifndef FOO_H_
#define FOO_H_

int byName(const FTSENT *a, const FTSENT *b);

int byNameRev(const FTSENT *a, const FTSENT *b);

int bySize(const FTSENT *a, const FTSENT *b);

int bySizeRev(const FTSENT *a, const FTSENT *b);

int byATime(const FTSENT *a, const FTSENT *b);

int byATimeRev(const FTSENT *a, const FTSENT *b);

int byMTime(const FTSENT *a, const FTSENT *b);

int byMTimeRev(const FTSENT *a, const FTSENT *b);

int byCTime(const FTSENT *a, const FTSENT *b);

int byCTimeRev(const FTSENT *a, const FTSENT *b);

#endif
