#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>

/* PQCLEAN_NTRULPR857_CLEAN_Decode(R,s,M,len) */
/* assumes 0 < M[i] < 16384 */
/* produces 0 <= R[i] < M[i] */
extern void PQCLEAN_NTRULPR857_CLEAN_Decode(uint16_t *out, const unsigned char *S, const uint16_t *M, long long len);

#endif
