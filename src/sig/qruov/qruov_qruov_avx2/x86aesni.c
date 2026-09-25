#include "x86aesni.h"

#include <emmintrin.h>
#include <smmintrin.h>
#include <wmmintrin.h>

static inline __m128i aes128_assist(__m128i temp1, __m128i temp2)
{
    __m128i temp3;
    temp2 = _mm_shuffle_epi32(temp2, 0xff);
    temp3 = _mm_slli_si128(temp1, 0x4);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp3 = _mm_slli_si128(temp3, 0x4);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp3 = _mm_slli_si128(temp3, 0x4);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp1 = _mm_xor_si128(temp1, temp2);
    return temp1;
}

void AES128_Key_Expansion(unsigned char *key, const unsigned char *userkey)
{
    __m128i temp1, temp2;
    __m128i *key_schedule = (__m128i *)key;

    temp1 = _mm_loadu_si128((const __m128i *)userkey);
    key_schedule[0] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x1);
    temp1 = aes128_assist(temp1, temp2);
    key_schedule[1] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x2);
    temp1 = aes128_assist(temp1, temp2);
    key_schedule[2] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x4);
    temp1 = aes128_assist(temp1, temp2);
    key_schedule[3] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x8);
    temp1 = aes128_assist(temp1, temp2);
    key_schedule[4] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x10);
    temp1 = aes128_assist(temp1, temp2);
    key_schedule[5] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x20);
    temp1 = aes128_assist(temp1, temp2);
    key_schedule[6] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x40);
    temp1 = aes128_assist(temp1, temp2);
    key_schedule[7] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x80);
    temp1 = aes128_assist(temp1, temp2);
    key_schedule[8] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x1b);
    temp1 = aes128_assist(temp1, temp2);
    key_schedule[9] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x36);
    temp1 = aes128_assist(temp1, temp2);
    key_schedule[10] = temp1;
}

void AES128_CTR_Stream(unsigned char *out, unsigned long n_16B,
                       const unsigned char *key, const unsigned char nonce[16],
                       uint32_t ctr)
{
    const int rounds = 10;
    const int blocks = (int)n_16B;
    const __m128i one = _mm_set_epi32(0, 0, 0, 1);
    const __m128i bswap = _mm_setr_epi8(
        15, 14, 13, 12, 11, 10, 9, 8,
         7,  6,  5,  4,  3,  2, 1, 0);
    __m128i ctr_block, tmp0, tmp1, tmp2, tmp3;

    ctr_block = _mm_loadu_si128((const __m128i *)nonce);
    ctr_block = _mm_shuffle_epi8(ctr_block, bswap);
    ctr_block = _mm_insert_epi32(ctr_block, (int)ctr, 0);

    int i = 0;
    // Valid only for ctr % 4 == 0: +1/+2/+3 affect only the last AES-order byte.
    if ((ctr & 3) == 0) {
        const __m128i four = _mm_set_epi32(0, 0, 0, 4);
        const __m128i key0_0 = ((__m128i *)key)[0];
        const __m128i key0_1 = _mm_xor_si128(key0_0, _mm_set_epi32(0x01000000, 0, 0, 0));
        const __m128i key0_2 = _mm_xor_si128(key0_0, _mm_set_epi32(0x02000000, 0, 0, 0));
        const __m128i key0_3 = _mm_xor_si128(key0_0, _mm_set_epi32(0x03000000, 0, 0, 0));
        for (; i + 3 < blocks; i += 4) {
            __m128i ctr_block_bswap = _mm_shuffle_epi8(ctr_block, bswap);
            tmp0 = _mm_xor_si128(ctr_block_bswap, key0_0);
            tmp1 = _mm_xor_si128(ctr_block_bswap, key0_1);
            tmp2 = _mm_xor_si128(ctr_block_bswap, key0_2);
            tmp3 = _mm_xor_si128(ctr_block_bswap, key0_3);
            ctr_block = _mm_add_epi64(ctr_block, four);
            for (int j = 1; j < rounds; j++) {
                tmp0 = _mm_aesenc_si128(tmp0, ((__m128i *)key)[j]);
                tmp1 = _mm_aesenc_si128(tmp1, ((__m128i *)key)[j]);
                tmp2 = _mm_aesenc_si128(tmp2, ((__m128i *)key)[j]);
                tmp3 = _mm_aesenc_si128(tmp3, ((__m128i *)key)[j]);
            }
            tmp0 = _mm_aesenclast_si128(tmp0, ((__m128i *)key)[rounds]);
            tmp1 = _mm_aesenclast_si128(tmp1, ((__m128i *)key)[rounds]);
            tmp2 = _mm_aesenclast_si128(tmp2, ((__m128i *)key)[rounds]);
            tmp3 = _mm_aesenclast_si128(tmp3, ((__m128i *)key)[rounds]);
            _mm_storeu_si128(&((__m128i *)out)[i + 0], tmp0);
            _mm_storeu_si128(&((__m128i *)out)[i + 1], tmp1);
            _mm_storeu_si128(&((__m128i *)out)[i + 2], tmp2);
            _mm_storeu_si128(&((__m128i *)out)[i + 3], tmp3);
        }
    }
    for (; i < blocks; i++) {
        tmp0 = _mm_shuffle_epi8(ctr_block, bswap);
        ctr_block = _mm_add_epi64(ctr_block, one);
        tmp0 = _mm_xor_si128(tmp0, ((__m128i *)key)[0]);
        for (int j = 1; j < rounds; j++) {
            tmp0 = _mm_aesenc_si128(tmp0, ((__m128i *)key)[j]);
        }
        tmp0 = _mm_aesenclast_si128(tmp0, ((__m128i *)key)[rounds]);
        _mm_storeu_si128(&((__m128i *)out)[i], tmp0);
    }
}
