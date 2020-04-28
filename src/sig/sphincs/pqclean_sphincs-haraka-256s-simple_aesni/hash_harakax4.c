#include <stdint.h>
#include <string.h>

#include "address.h"
#include "haraka.h"
#include "hash_state.h"
#include "hashx4.h"
#include "params.h"

/*
 * 4-way parallel version of prf_addr; takes 4x as much input and output
 */
void PQCLEAN_SPHINCSHARAKA256SSIMPLE_AESNI_prf_addrx4(unsigned char *out0,
        unsigned char *out1,
        unsigned char *out2,
        unsigned char *out3,
        const unsigned char *key,
        const uint32_t addrx4[4 * 8],
        const hash_state *state_seeded) {
    unsigned char bufx4[4 * PQCLEAN_SPHINCSHARAKA256SSIMPLE_AESNI_ADDR_BYTES];
    /* Since PQCLEAN_SPHINCSHARAKA256SSIMPLE_AESNI_N may be smaller than 32, we need a temporary buffer. */
    unsigned char outbuf[4 * 32];
    unsigned int i;

    (void)key; /* Suppress an 'unused parameter' warning. */

    for (i = 0; i < 4; i++) {
        PQCLEAN_SPHINCSHARAKA256SSIMPLE_AESNI_addr_to_bytes(bufx4 + i * PQCLEAN_SPHINCSHARAKA256SSIMPLE_AESNI_ADDR_BYTES, addrx4 + i * 8);
    }

    PQCLEAN_SPHINCSHARAKA256SSIMPLE_AESNI_haraka256_skx4(outbuf, bufx4, state_seeded);

    memcpy(out0, outbuf, PQCLEAN_SPHINCSHARAKA256SSIMPLE_AESNI_N);
    memcpy(out1, outbuf + 32, PQCLEAN_SPHINCSHARAKA256SSIMPLE_AESNI_N);
    memcpy(out2, outbuf + 64, PQCLEAN_SPHINCSHARAKA256SSIMPLE_AESNI_N);
    memcpy(out3, outbuf + 96, PQCLEAN_SPHINCSHARAKA256SSIMPLE_AESNI_N);
}
