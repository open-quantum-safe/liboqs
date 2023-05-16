#ifndef SPX_F1600X2_H
#define SPX_F1600X2_H

#include <stdint.h>

extern uint64_t f1600_RC[24];
extern void _f1600x2(uint64_t *a, uint64_t *rc);

#define f1600x2(s) do {_f1600x2((s), f1600_RC);} while(0)

#endif
