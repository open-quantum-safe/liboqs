#ifndef PQCLEAN_SPHINCSSHA256128FSIMPLE_AVX2_HASHX8_H
#define PQCLEAN_SPHINCSSHA256128FSIMPLE_AVX2_HASHX8_H

#include <stdint.h>

#include "params.h"

void PQCLEAN_SPHINCSSHA256128FSIMPLE_AVX2_prf_addrx8(unsigned char *out0,
        unsigned char *out1,
        unsigned char *out2,
        unsigned char *out3,
        unsigned char *out4,
        unsigned char *out5,
        unsigned char *out6,
        unsigned char *out7,
        const unsigned char *key,
        const uint32_t addrx8[8 * 8]);

#endif
