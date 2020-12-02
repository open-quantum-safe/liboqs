#ifndef PQCLEAN_KYBER768_AVX2_REJSAMPLE_H
#define PQCLEAN_KYBER768_AVX2_REJSAMPLE_H
#include "params.h"
#include "symmetric.h"
#include <stdint.h>

#define REJ_UNIFORM_AVX_NBLOCKS ((12*KYBER_N/8*(1 << 12)/KYBER_Q + XOF_BLOCKBYTES)/XOF_BLOCKBYTES)
#define REJ_UNIFORM_AVX_BUFLEN (REJ_UNIFORM_AVX_NBLOCKS*XOF_BLOCKBYTES)

unsigned int PQCLEAN_KYBER768_AVX2_rej_uniform_avx(int16_t *r, const uint8_t *buf);

#endif
