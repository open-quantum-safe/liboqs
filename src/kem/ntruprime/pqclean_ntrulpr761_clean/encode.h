#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>

/* PQCLEAN_NTRULPR761_CLEAN_Encode(s,R,M,len) */
/* assumes 0 <= R[i] < M[i] < 16384 */
extern void PQCLEAN_NTRULPR761_CLEAN_Encode(unsigned char *out, const uint16_t *R, const uint16_t *M, long long len);

#endif
