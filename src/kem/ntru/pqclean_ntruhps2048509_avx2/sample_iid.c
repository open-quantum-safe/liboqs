#include <immintrin.h>

#include "sample.h"

extern void PQCLEAN_NTRUHPS2048509_AVX2_vec32_sample_iid(poly *r, const unsigned char uniformbytes[PAD32(NTRU_SAMPLE_IID_BYTES)]);

void PQCLEAN_NTRUHPS2048509_AVX2_sample_iid(poly *r, const unsigned char uniformbytes[NTRU_SAMPLE_IID_BYTES]) {
    int i;
    union { /* align to 32 byte boundary for vmovdqa */
        unsigned char b[PAD32(NTRU_SAMPLE_IID_BYTES)];
        __m256i b_x32[PAD32(NTRU_SAMPLE_IID_BYTES) / 32];
    } buffer;

    for (i = 0; i < NTRU_SAMPLE_IID_BYTES; i++) {
        buffer.b[i] = uniformbytes[i];
    }
    for (i = NTRU_SAMPLE_IID_BYTES; i < PAD32(NTRU_SAMPLE_IID_BYTES); i++) {
        buffer.b[i] = 0;
    }
    PQCLEAN_NTRUHPS2048509_AVX2_vec32_sample_iid(r, buffer.b);
}
