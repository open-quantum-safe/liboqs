#include "bch.h"
#include "code.h"
#include "gen_matrix.h"
#include "parameters.h"
#include "repetition.h"
#include <immintrin.h>
#include <stdint.h>
#include <string.h>
/**
 * @file code.c
 * @brief Implementation of tensor code
 */


static inline uint64_t mux(uint64_t a, uint64_t b, int64_t bit);

static inline uint64_t mux(uint64_t a, uint64_t b, int64_t bit) {
    uint64_t ret = a ^ b;
    return (ret & (-bit >> 63)) ^ a;
}



/**
 *
 * @brief Encoding the message m to a code word em using the tensor code
 *
 * We encode the message using the BCH code. For each bit obtained,
 * we duplicate the bit PARAM_N2 times to apply repetition code.
 * BCH encoding is done using the classical mG operation,
 * columns of the matrix are stored in 256-bit registers
 *
 * @param[out] em Pointer to an array that is the tensor code word
 * @param[in] m Pointer to an array that is the message
 */
void PQCLEAN_HQC256_AVX2_code_encode(uint64_t *em, const uint64_t *m) {
    const uint64_t mask[2][3] = {{0x0UL, 0x0UL, 0x0UL}, {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0x3FFFFFUL}};
    size_t i, pos_r;
    uint64_t bit;
    uint64_t idx_r;
    uint64_t idx_2;
    uint64_t select;


    __m256i *colonne, y, aux0;
    __m256i msg = _mm256_lddqu_si256((const __m256i *) m);
    colonne = ((__m256i *) PQCLEAN_HQC256_AVX2_gen_matrix);

    pos_r = 0;
    for (i = 0; i < PARAM_N1 - PARAM_K; i++) {
        // y is the and operation between m and ith column of G
        y = _mm256_and_si256(colonne[i], msg);
        // aux0 = (y2 y3 y0 y1)
        aux0 = _mm256_permute2x128_si256(y, y, 1);
        // y = (y0^y2 y1^y3 y2^y0 y3^y1)
        y = _mm256_xor_si256(y, aux0);
        // aux0 = (y1^y3 y0^y2 y1^y3 y0^y2)
        aux0 = _mm256_shuffle_epi32(y, 0x4e);
        // y = (y0^y1^y2^y3 repeated 4 times)
        y = _mm256_xor_si256(aux0, y);
        bit = _mm_popcnt_u64(_mm256_extract_epi64(y, 0)) & 1;


        idx_r = (pos_r & 0x3f);
        idx_2 = 41 - idx_r;
        idx_2 &= (uint64_t) (-((int64_t)idx_2) >> 63);
        select = mux(mask[0][0], mask[1][0], bit);
        em[(pos_r >> 6) + 0] ^= select << idx_r;
        select = mux(mask[0][1], mask[1][1], bit);
        em[(pos_r >> 6) + 1] ^= select >> idx_2;
        select = mux(mask[0][2], mask[1][2], bit);
        em[(pos_r >> 6) + 2] ^= select >> ((63 - idx_r));
        pos_r += PARAM_N2;
    }

    /* now we add the message m */
    /* systematic encoding */
    pos_r = PARAM_N2 * (PARAM_N1 - PARAM_K);
    for (int32_t i = 0; i < 4; i++) {
        for (int32_t j = 0; j < 64; j++) {
            bit = (m[i] >> j) & 0x1;


            idx_r = (pos_r & 0x3f);
            idx_2 = 41 - idx_r;
            idx_2 &= (uint64_t) (-((int64_t)idx_2) >> 63);
            select = mux(mask[0][0], mask[1][0], bit);
            em[(pos_r >> 6) + 0] ^= select << idx_r;
            select = mux(mask[0][1], mask[1][1], bit);
            em[(pos_r >> 6) + 1] ^= select >> idx_2;
            select = mux(mask[0][2], mask[1][2], bit);
            em[(pos_r >> 6) + 2] ^= select >> ((63 - idx_r));

            pos_r += PARAM_N2;
        }
    }

}


/**
 * @brief Decoding the code word em to a message m using the tensor code
 *
 * @param[out] m Pointer to an array that is the message
 * @param[in] em Pointer to an array that is the code word
 */
void PQCLEAN_HQC256_AVX2_code_decode(uint64_t *m, const uint64_t *em) {

    uint64_t tmp[VEC_N1_SIZE_64] = {0};

    PQCLEAN_HQC256_AVX2_repetition_code_decode(tmp, em);
    PQCLEAN_HQC256_AVX2_bch_code_decode(m, tmp);

}
