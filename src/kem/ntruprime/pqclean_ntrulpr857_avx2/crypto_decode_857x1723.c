#include "crypto_decode_857x1723.h"
#include <immintrin.h>
/* auto-generated; do not edit */

#define int16 int16_t
#define int32 int32_t

static inline int16 mullo(int16 x, int16 y) {
    return x * y;
}

static inline int16 mulhi(int16 x, int16 y) {
    return (int16) ((x * (int32)y) >> 16);
}

static inline __m256i add(__m256i x, __m256i y) {
    return _mm256_add_epi16(x, y);
}

static inline __m256i sub(__m256i x, __m256i y) {
    return _mm256_sub_epi16(x, y);
}

static inline __m256i shiftleftconst(__m256i x, int16 y) {
    return _mm256_slli_epi16(x, y);
}

static inline __m256i signedshiftrightconst(__m256i x, int16 y) {
    return _mm256_srai_epi16(x, y);
}

static inline __m256i subconst(__m256i x, int16 y) {
    return sub(x, _mm256_set1_epi16(y));
}

static inline __m256i mulloconst(__m256i x, int16 y) {
    return _mm256_mullo_epi16(x, _mm256_set1_epi16(y));
}

static inline __m256i mulhiconst(__m256i x, int16 y) {
    return _mm256_mulhi_epi16(x, _mm256_set1_epi16(y));
}

static inline __m256i ifgesubconst(__m256i x, int16 y) {
    __m256i y16 = _mm256_set1_epi16(y);
    __m256i top16 = _mm256_set1_epi16((int16)(y - 1));
    return sub(x, _mm256_cmpgt_epi16(x, top16) & y16);
}

static inline __m256i ifnegaddconst(__m256i x, int16 y) {
    return add(x, signedshiftrightconst(x, 15) & _mm256_set1_epi16(y));
}

void PQCLEAN_NTRULPR857_AVX2_crypto_decode_857x1723(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[429], R2[215], R3[108], R4[54], R5[27], R6[14], R7[7], R8[4], R9[2], R10[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_NTRULPR857_AVX2_crypto_decode_857x1723_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 -= 160; /* -160...95 */
    a1 += (a1 >> 15) & 160; /* 0...159 */
    R10[0] = a1;

    /* R10 ------> R9: reconstruct mod 1*[743]+[14044] */

    i = 0;
    s -= 2;
    a0 = R10[0];
    a0 = mulhi(a0, 276) - mulhi(mullo(a0, -22580), 743); /* -372...440 */
    a0 += s[2 * i + 1]; /* -372...695 */
    a0 = mulhi(a0, 276) - mulhi(mullo(a0, -22580), 743); /* -374...374 */
    a0 += s[2 * i + 0]; /* -374...629 */
    a0 += (a0 >> 15) & 743; /* 0...742 */
    a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
    a1 = mullo(a1, -3881);

    /* invalid inputs might need reduction mod 14044 */
    a1 -= 14044;
    a1 += (a1 >> 15) & 14044;

    R9[0] = a0;
    R9[1] = a1;
    s -= 0;

    /* R9 ------> R8: reconstruct mod 3*[436]+[8246] */

    i = 0;
    s -= 1;
    a2 = a0 = R9[1];
    a0 = mulhi(a0, -64) - mulhi(mullo(a0, 27056), 436); /* -234...218 */
    a0 += s[1 * i + 0]; /* -234...473 */
    a0 -= 436; /* -670..>37 */
    a0 += (a0 >> 15) & 436; /* -234...435 */
    a0 += (a0 >> 15) & 436; /* 0...435 */
    a1 = (int16) ((a2 << 6) + ((s[i] - a0) >> 2));
    a1 = mullo(a1, 2405);

    /* invalid inputs might need reduction mod 8246 */
    a1 -= 8246;
    a1 += (a1 >> 15) & 8246;

    R8[2] = a0;
    R8[3] = a1;
    s -= 1;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, -64) - mulhi(mullo(a0, 27056), 436); /* -234...218 */
        a0 += s[1 * i + 0]; /* -234...473 */
        a0 -= 436; /* -670..>37 */
        a0 += (a0 >> 15) & 436; /* -234...435 */
        a0 += (a0 >> 15) & 436; /* 0...435 */
        a1 = (int16) ((a2 << 6) + ((s[i] - a0) >> 2));
        a1 = mullo(a1, 2405);

        /* invalid inputs might need reduction mod 436 */
        a1 -= 436;
        a1 += (a1 >> 15) & 436;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 6*[334]+[8246] */

    R7[6] = R8[3];
    s -= 3;
    for (i = 2; i >= 0; --i) {
        a2 = a0 = R8[i];
        a0 = mulhi(a0, 62) - mulhi(mullo(a0, 15305), 334); /* -167...182 */
        a0 += s[1 * i + 0]; /* -167...437 */
        a0 -= 334; /* -501..>103 */
        a0 += (a0 >> 15) & 334; /* -167...333 */
        a0 += (a0 >> 15) & 334; /* 0...333 */
        a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
        a1 = mullo(a1, -22761);

        /* invalid inputs might need reduction mod 334 */
        a1 -= 334;
        a1 += (a1 >> 15) & 334;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 13*[292]+[7229] */

    i = 0;
    s -= 1;
    a2 = a0 = R7[6];
    a0 = mulhi(a0, 64) - mulhi(mullo(a0, 8080), 292); /* -146...162 */
    a0 += s[1 * i + 0]; /* -146...417 */
    a0 -= 292; /* -438..>125 */
    a0 += (a0 >> 15) & 292; /* -146...291 */
    a0 += (a0 >> 15) & 292; /* 0...291 */
    a1 = (int16) ((a2 << 6) + ((s[i] - a0) >> 2));
    a1 = mullo(a1, -3591);

    /* invalid inputs might need reduction mod 7229 */
    a1 -= 7229;
    a1 += (a1 >> 15) & 7229;

    R6[12] = a0;
    R6[13] = a1;
    s -= 6;
    for (i = 5; i >= 0; --i) {
        a2 = a0 = R7[i];
        a0 = mulhi(a0, 64) - mulhi(mullo(a0, 8080), 292); /* -146...162 */
        a0 += s[1 * i + 0]; /* -146...417 */
        a0 -= 292; /* -438..>125 */
        a0 += (a0 >> 15) & 292; /* -146...291 */
        a0 += (a0 >> 15) & 292; /* 0...291 */
        a1 = (int16) ((a2 << 6) + ((s[i] - a0) >> 2));
        a1 = mullo(a1, -3591);

        /* invalid inputs might need reduction mod 292 */
        a1 -= 292;
        a1 += (a1 >> 15) & 292;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 26*[273]+[7229] */

    R5[26] = R6[13];
    s -= 13;
    for (i = 12; i >= 0; --i) {
        a2 = a0 = R6[i];
        a0 = mulhi(a0, 1) - mulhi(mullo(a0, 4081), 273); /* -137...136 */
        a0 += s[1 * i + 0]; /* -137...391 */
        a0 -= 273; /* -410..>118 */
        a0 += (a0 >> 15) & 273; /* -137...272 */
        a0 += (a0 >> 15) & 273; /* 0...272 */
        a1 = (int16) ((a2 << 8) + s[i] - a0);
        a1 = mullo(a1, 4081);

        /* invalid inputs might need reduction mod 273 */
        a1 -= 273;
        a1 += (a1 >> 15) & 273;

        R5[2 * i] = a0;
        R5[2 * i + 1] = a1;
    }

    /* R5 ------> R4: reconstruct mod 53*[4225]+[438] */

    i = 0;
    s -= 1;
    a2 = a0 = R5[26];
    a0 = mulhi(a0, -259) - mulhi(mullo(a0, -3971), 4225); /* -2178...2112 */
    a0 += s[1 * i + 0]; /* -2178...2367 */
    a0 += (a0 >> 15) & 4225; /* 0...4224 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 12161);

    /* invalid inputs might need reduction mod 438 */
    a1 -= 438;
    a1 += (a1 >> 15) & 438;

    R4[52] = a0;
    R4[53] = a1;
    s -= 52;
    i = 10;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -259), mulhiconst(mulloconst(A0, -3971), 4225)); /* -2178...2112 */
        A0 = add(A0, S1); /* -2178...2367 */
        A0 = sub(mulhiconst(A0, -259), mulhiconst(mulloconst(A0, -3971), 4225)); /* -2122...2121 */
        A0 = add(A0, S0); /* -2122...2376 */
        A0 = ifnegaddconst(A0, 4225); /* 0...4224 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, 12161);

        /* invalid inputs might need reduction mod 4225 */
        A1 = ifgesubconst(A1, 4225);

        /* A0: r0r2r4r6r8r10r12r14 r16r18r20r22r24r26r28r30 */
        /* A1: r1r3r5r7r9r11r13r15 r17r19r21r23r25r27r29r31 */
        B0 = _mm256_unpacklo_epi16(A0, A1);
        B1 = _mm256_unpackhi_epi16(A0, A1);
        /* B0: r0r1r2r3r4r5r6r7 r16r17r18r19r20r21r22r23 */
        /* B1: r8r9r10r11r12r13r14r15 r24r25r26r27r28r29r30r31 */
        C0 = _mm256_permute2x128_si256(B0, B1, 0x20);
        C1 = _mm256_permute2x128_si256(B0, B1, 0x31);
        /* C0: r0r1r2r3r4r5r6r7 r8r9r10r11r12r13r14r15 */
        /* C1: r16r17r18r19r20r21r22r23 r24r25r26r27r28r29r30r31 */
        _mm256_storeu_si256((__m256i *) (&R4[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R4[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }

    /* R4 ------> R3: reconstruct mod 107*[65]+[1723] */

    i = 0;
    s -= 1;
    a2 = a0 = R4[53];
    a0 = mulhi(a0, 1) - mulhi(mullo(a0, 4033), 65); /* -33...32 */
    a0 += s[1 * i + 0]; /* -33...287 */
    a0 = mulhi(a0, 16) - mulhi(mullo(a0, -1008), 65); /* -33...32 */
    a0 += (a0 >> 15) & 65; /* 0...64 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 4033);

    /* invalid inputs might need reduction mod 1723 */
    a1 -= 1723;
    a1 += (a1 >> 15) & 1723;

    R3[106] = a0;
    R3[107] = a1;
    s -= 0;
    i = 37;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        A0 = sub(mulhiconst(A0, 16), mulhiconst(mulloconst(A0, -1008), 65)); /* -33...36 */
        A0 = ifnegaddconst(A0, 65); /* 0...64 */
        A1 = signedshiftrightconst(sub(A2, A0), 0);
        A1 = mulloconst(A1, 4033);

        /* invalid inputs might need reduction mod 65 */
        A1 = ifgesubconst(A1, 65);

        /* A0: r0r2r4r6r8r10r12r14 r16r18r20r22r24r26r28r30 */
        /* A1: r1r3r5r7r9r11r13r15 r17r19r21r23r25r27r29r31 */
        B0 = _mm256_unpacklo_epi16(A0, A1);
        B1 = _mm256_unpackhi_epi16(A0, A1);
        /* B0: r0r1r2r3r4r5r6r7 r16r17r18r19r20r21r22r23 */
        /* B1: r8r9r10r11r12r13r14r15 r24r25r26r27r28r29r30r31 */
        C0 = _mm256_permute2x128_si256(B0, B1, 0x20);
        C1 = _mm256_permute2x128_si256(B0, B1, 0x31);
        /* C0: r0r1r2r3r4r5r6r7 r8r9r10r11r12r13r14r15 */
        /* C1: r16r17r18r19r20r21r22r23 r24r25r26r27r28r29r30r31 */
        _mm256_storeu_si256((__m256i *) (&R3[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R3[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }

    /* R3 ------> R2: reconstruct mod 214*[2053]+[1723] */

    R2[214] = R3[107];
    s -= 214;
    i = 91;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 100), mulhiconst(mulloconst(A0, -8172), 2053)); /* -1027...1051 */
        A0 = add(A0, S1); /* -1027...1306 */
        A0 = sub(mulhiconst(A0, 100), mulhiconst(mulloconst(A0, -8172), 2053)); /* -1029...1028 */
        A0 = add(A0, S0); /* -1029...1283 */
        A0 = ifnegaddconst(A0, 2053); /* 0...2052 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, -31539);

        /* invalid inputs might need reduction mod 2053 */
        A1 = ifgesubconst(A1, 2053);

        /* A0: r0r2r4r6r8r10r12r14 r16r18r20r22r24r26r28r30 */
        /* A1: r1r3r5r7r9r11r13r15 r17r19r21r23r25r27r29r31 */
        B0 = _mm256_unpacklo_epi16(A0, A1);
        B1 = _mm256_unpackhi_epi16(A0, A1);
        /* B0: r0r1r2r3r4r5r6r7 r16r17r18r19r20r21r22r23 */
        /* B1: r8r9r10r11r12r13r14r15 r24r25r26r27r28r29r30r31 */
        C0 = _mm256_permute2x128_si256(B0, B1, 0x20);
        C1 = _mm256_permute2x128_si256(B0, B1, 0x31);
        /* C0: r0r1r2r3r4r5r6r7 r8r9r10r11r12r13r14r15 */
        /* C1: r16r17r18r19r20r21r22r23 r24r25r26r27r28r29r30r31 */
        _mm256_storeu_si256((__m256i *) (&R2[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R2[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }

    /* R2 ------> R1: reconstruct mod 428*[11597]+[1723] */

    R1[428] = R2[214];
    s -= 428;
    i = 198;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -3643), mulhiconst(mulloconst(A0, -1447), 11597)); /* -6710...5798 */
        A0 = add(A0, S1); /* -6710...6053 */
        A0 = sub(mulhiconst(A0, -3643), mulhiconst(mulloconst(A0, -1447), 11597)); /* -6135...6171 */
        A0 = add(A0, S0); /* -6135...6426 */
        A0 = ifnegaddconst(A0, 11597); /* 0...11596 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, -11387);

        /* invalid inputs might need reduction mod 11597 */
        A1 = ifgesubconst(A1, 11597);

        /* A0: r0r2r4r6r8r10r12r14 r16r18r20r22r24r26r28r30 */
        /* A1: r1r3r5r7r9r11r13r15 r17r19r21r23r25r27r29r31 */
        B0 = _mm256_unpacklo_epi16(A0, A1);
        B1 = _mm256_unpackhi_epi16(A0, A1);
        /* B0: r0r1r2r3r4r5r6r7 r16r17r18r19r20r21r22r23 */
        /* B1: r8r9r10r11r12r13r14r15 r24r25r26r27r28r29r30r31 */
        C0 = _mm256_permute2x128_si256(B0, B1, 0x20);
        C1 = _mm256_permute2x128_si256(B0, B1, 0x31);
        /* C0: r0r1r2r3r4r5r6r7 r8r9r10r11r12r13r14r15 */
        /* C1: r16r17r18r19r20r21r22r23 r24r25r26r27r28r29r30r31 */
        _mm256_storeu_si256((__m256i *) (&R1[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R1[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }

    /* R1 ------> R0: reconstruct mod 857*[1723] */

    R0[856] = (int16) (3 * R1[428] - 2583);
    s -= 428;
    i = 412;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 365), mulhiconst(mulloconst(A0, -9737), 1723)); /* -862...952 */
        A0 = add(A0, S0); /* -862...1207 */
        A0 = ifnegaddconst(A0, 1723); /* 0...1722 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, 20083);

        /* invalid inputs might need reduction mod 1723 */
        A1 = ifgesubconst(A1, 1723);

        A0 = mulloconst(A0, 3);
        A1 = mulloconst(A1, 3);
        A0 = subconst(A0, 2583);
        A1 = subconst(A1, 2583);
        /* A0: r0r2r4r6r8r10r12r14 r16r18r20r22r24r26r28r30 */
        /* A1: r1r3r5r7r9r11r13r15 r17r19r21r23r25r27r29r31 */
        B0 = _mm256_unpacklo_epi16(A0, A1);
        B1 = _mm256_unpackhi_epi16(A0, A1);
        /* B0: r0r1r2r3r4r5r6r7 r16r17r18r19r20r21r22r23 */
        /* B1: r8r9r10r11r12r13r14r15 r24r25r26r27r28r29r30r31 */
        C0 = _mm256_permute2x128_si256(B0, B1, 0x20);
        C1 = _mm256_permute2x128_si256(B0, B1, 0x31);
        /* C0: r0r1r2r3r4r5r6r7 r8r9r10r11r12r13r14r15 */
        /* C1: r16r17r18r19r20r21r22r23 r24r25r26r27r28r29r30r31 */
        _mm256_storeu_si256((__m256i *) (&R0[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R0[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }
}
