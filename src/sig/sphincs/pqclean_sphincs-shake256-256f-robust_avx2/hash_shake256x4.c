#include <stdint.h>
#include <string.h>

#include "address.h"
#include "fips202x4.h"
#include "hashx4.h"
#include "params.h"

/*
 * 4-way parallel version of prf_addr; takes 4x as much input and output
 */
void PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_prf_addrx4(unsigned char *out0,
        unsigned char *out1,
        unsigned char *out2,
        unsigned char *out3,
        const unsigned char *key,
        const uint32_t addrx4[4 * 8],
        const hash_state *state_seeded) {
    unsigned char bufx4[4 * (PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_ADDR_BYTES)];
    unsigned int j;

    for (j = 0; j < 4; j++) {
        memcpy(bufx4 + j * (PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_ADDR_BYTES), key, PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N);
        PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_addr_to_bytes(bufx4 + PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N + j * (PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_ADDR_BYTES), addrx4 + j * 8);
    }

    PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_shake256x4(out0,
            out1,
            out2,
            out3, PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N,
            bufx4 + 0 * (PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_ADDR_BYTES),
            bufx4 + 1 * (PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_ADDR_BYTES),
            bufx4 + 2 * (PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_ADDR_BYTES),
            bufx4 + 3 * (PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_ADDR_BYTES), PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256256FROBUST_AVX2_ADDR_BYTES);

    /* Avoid unused parameter warning */
    (void)state_seeded;
}
