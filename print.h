#ifndef PRINT_H
#define PRINT_H

void printlink(FTSENT *p);
void printtime(time_t ftime);
void printfcn(FTSENT *chp, PRINT_PARAMS *params);

#endif
