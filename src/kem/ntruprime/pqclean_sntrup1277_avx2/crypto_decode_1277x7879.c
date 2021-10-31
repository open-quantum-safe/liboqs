#include "crypto_decode_1277x7879.h"
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

void PQCLEAN_SNTRUP1277_AVX2_crypto_decode_1277x7879(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[639], R2[320], R3[160], R4[80], R5[40], R6[20], R7[10], R8[5], R9[3], R10[2], R11[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_SNTRUP1277_AVX2_crypto_decode_1277x7879_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 = mulhi(a1, 1072) - mulhi(mullo(a1, -4539), 3696);
    a1 += *--s; /* -1848...2107 */
    a1 += (a1 >> 15) & 3696; /* 0...3695 */
    R11[0] = a1;

    /* R11 ------> R10: reconstruct mod 1*[376]+[2516] */

    i = 0;
    s -= 1;
    a2 = a0 = R11[0];
    a0 = mulhi(a0, 96) - mulhi(mullo(a0, 20916), 376); /* -188...212 */
    a0 += s[1 * i + 0]; /* -188...467 */
    a0 -= 376; /* -564..>91 */
    a0 += (a0 >> 15) & 376; /* -188...375 */
    a0 += (a0 >> 15) & 376; /* 0...375 */
    a1 = (int16) ((a2 << 5) + ((s[i] - a0) >> 3));
    a1 = mullo(a1, 18127);

    /* invalid inputs might need reduction mod 2516 */
    a1 -= 2516;
    a1 += (a1 >> 15) & 2516;

    R10[0] = a0;
    R10[1] = a1;
    s -= 0;

    /* R10 ------> R9: reconstruct mod 2*[4962]+[2516] */

    R9[2] = R10[1];
    s -= 2;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R10[i];
        a0 = mulhi(a0, 694) - mulhi(mullo(a0, -3381), 4962); /* -2481...2654 */
        a0 += s[2 * i + 1]; /* -2481...2909 */
        a0 = mulhi(a0, 694) - mulhi(mullo(a0, -3381), 4962); /* -2508...2511 */
        a0 += s[2 * i + 0]; /* -2508...2766 */
        a0 += (a0 >> 15) & 4962; /* 0...4961 */
        a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
        a1 = mullo(a1, -24751);

        /* invalid inputs might need reduction mod 4962 */
        a1 -= 4962;
        a1 += (a1 >> 15) & 4962;

        R9[2 * i] = a0;
        R9[2 * i + 1] = a1;
    }

    /* R9 ------> R8: reconstruct mod 4*[1127]+[2516] */

    R8[4] = R9[2];
    s -= 2;
    for (i = 1; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, -433) - mulhi(mullo(a0, -14887), 1127); /* -672...563 */
        a0 += s[1 * i + 0]; /* -672...818 */
        a0 += (a0 >> 15) & 1127; /* 0...1126 */
        a1 = (int16) ((a2 << 8) + s[i] - a0);
        a1 = mullo(a1, -10409);

        /* invalid inputs might need reduction mod 1127 */
        a1 -= 1127;
        a1 += (a1 >> 15) & 1127;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 9*[537]+[1199] */

    i = 0;
    s -= 1;
    a2 = a0 = R8[4];
    a0 = mulhi(a0, 262) - mulhi(mullo(a0, -31242), 537); /* -269...334 */
    a0 += s[1 * i + 0]; /* -269...589 */
    a0 -= 537; /* -806..>52 */
    a0 += (a0 >> 15) & 537; /* -269...536 */
    a0 += (a0 >> 15) & 537; /* 0...536 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 14889);

    /* invalid inputs might need reduction mod 1199 */
    a1 -= 1199;
    a1 += (a1 >> 15) & 1199;

    R7[8] = a0;
    R7[9] = a1;
    s -= 4;
    for (i = 3; i >= 0; --i) {
        a2 = a0 = R8[i];
        a0 = mulhi(a0, 262) - mulhi(mullo(a0, -31242), 537); /* -269...334 */
        a0 += s[1 * i + 0]; /* -269...589 */
        a0 -= 537; /* -806..>52 */
        a0 += (a0 >> 15) & 537; /* -269...536 */
        a0 += (a0 >> 15) & 537; /* 0...536 */
        a1 = (int16) ((a2 << 8) + s[i] - a0);
        a1 = mullo(a1, 14889);

        /* invalid inputs might need reduction mod 537 */
        a1 -= 537;
        a1 += (a1 >> 15) & 537;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 19*[5929]+[13244] */

    i = 0;
    s -= 2;
    a0 = R7[9];
    a0 = mulhi(a0, -1854) - mulhi(mullo(a0, -2830), 5929); /* -3428...2964 */
    a0 += s[2 * i + 1]; /* -3428...3219 */
    a0 = mulhi(a0, -1854) - mulhi(mullo(a0, -2830), 5929); /* -3056...3061 */
    a0 += s[2 * i + 0]; /* -3056...3316 */
    a0 += (a0 >> 15) & 5929; /* 0...5928 */
    a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
    a1 = mullo(a1, 29977);

    /* invalid inputs might need reduction mod 13244 */
    a1 -= 13244;
    a1 += (a1 >> 15) & 13244;

    R6[18] = a0;
    R6[19] = a1;
    s -= 18;
    for (i = 8; i >= 0; --i) {
        a0 = R7[i];
        a0 = mulhi(a0, -1854) - mulhi(mullo(a0, -2830), 5929); /* -3428...2964 */
        a0 += s[2 * i + 1]; /* -3428...3219 */
        a0 = mulhi(a0, -1854) - mulhi(mullo(a0, -2830), 5929); /* -3056...3061 */
        a0 += s[2 * i + 0]; /* -3056...3316 */
        a0 += (a0 >> 15) & 5929; /* 0...5928 */
        a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
        a1 = mullo(a1, 29977);

        /* invalid inputs might need reduction mod 5929 */
        a1 -= 5929;
        a1 += (a1 >> 15) & 5929;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 39*[77]+[172] */

    s -= 0;
    a2 = a0 = R6[19];
    a0 = mulhi(a0, 9) - mulhi(mullo(a0, -851), 77); /* -39...40 */
    a0 += (a0 >> 15) & 77; /* 0...76 */
    a1 = (int16) ((a2 - a0) >> 0);
    a1 = mullo(a1, 14469);

    /* invalid inputs might need reduction mod 172 */
    a1 -= 172;
    a1 += (a1 >> 15) & 172;

    R5[38] = a0;
    R5[39] = a1;
    s -= 0;
    i = 3;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R6[i]);
        A0 = sub(mulhiconst(A0, 9), mulhiconst(mulloconst(A0, -851), 77)); /* -39...40 */
        A0 = ifnegaddconst(A0, 77); /* 0...76 */
        A1 = signedshiftrightconst(sub(A2, A0), 0);
        A1 = mulloconst(A1, 14469);

        /* invalid inputs might need reduction mod 77 */
        A1 = ifgesubconst(A1, 77);

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
        _mm256_storeu_si256((__m256i *) (&R5[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R5[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }

    /* R5 ------> R4: reconstruct mod 79*[140]+[313] */

    i = 0;
    s -= 1;
    a2 = a0 = R5[39];
    a0 = mulhi(a0, 36) - mulhi(mullo(a0, 11235), 140); /* -70...79 */
    a0 += s[1 * i + 0]; /* -70...334 */
    a0 = mulhi(a0, 16) - mulhi(mullo(a0, -468), 140); /* -71...70 */
    a0 += (a0 >> 15) & 140; /* 0...139 */
    a1 = (int16) ((a2 << 6) + ((s[i] - a0) >> 2));
    a1 = mullo(a1, -20597);

    /* invalid inputs might need reduction mod 313 */
    a1 -= 313;
    a1 += (a1 >> 15) & 313;

    R4[78] = a0;
    R4[79] = a1;
    s -= 39;
    i = 23;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 36), mulhiconst(mulloconst(A0, 11235), 140)); /* -70...79 */
        A0 = add(A0, S0); /* -70...334 */
        A0 = sub(mulhiconst(A0, 16), mulhiconst(mulloconst(A0, -468), 140)); /* -71...70 */
        A0 = ifnegaddconst(A0, 140); /* 0...139 */
        A1 = add(shiftleftconst(A2, 6), signedshiftrightconst(sub(S0, A0), 2));
        A1 = mulloconst(A1, -20597);

        /* invalid inputs might need reduction mod 140 */
        A1 = ifgesubconst(A1, 140);

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

    /* R4 ------> R3: reconstruct mod 159*[189]+[423] */

    i = 0;
    s -= 1;
    a2 = a0 = R4[79];
    a0 = mulhi(a0, 64) - mulhi(mullo(a0, -23232), 189); /* -95...110 */
    a0 += s[1 * i + 0]; /* -95...365 */
    a0 -= 189; /* -284..>176 */
    a0 += (a0 >> 15) & 189; /* -95...188 */
    a0 += (a0 >> 15) & 189; /* 0...188 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, -1387);

    /* invalid inputs might need reduction mod 423 */
    a1 -= 423;
    a1 += (a1 >> 15) & 423;

    R3[158] = a0;
    R3[159] = a1;
    s -= 79;
    i = 63;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 64), mulhiconst(mulloconst(A0, -23232), 189)); /* -95...110 */
        A0 = add(A0, S0); /* -95...365 */
        A0 = subconst(A0, 189); /* -284...176 */
        A0 = ifnegaddconst(A0, 189); /* -95...188 */
        A0 = ifnegaddconst(A0, 189); /* 0...188 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, -1387);

        /* invalid inputs might need reduction mod 189 */
        A1 = ifgesubconst(A1, 189);

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

    /* R3 ------> R2: reconstruct mod 319*[3511]+[7879] */

    i = 0;
    s -= 2;
    a0 = R3[159];
    a0 = mulhi(a0, 1658) - mulhi(mullo(a0, -4778), 3511); /* -1756...2170 */
    a0 += s[2 * i + 1]; /* -1756...2425 */
    a0 = mulhi(a0, 1658) - mulhi(mullo(a0, -4778), 3511); /* -1800...1816 */
    a0 += s[2 * i + 0]; /* -1800...2071 */
    a0 += (a0 >> 15) & 3511; /* 0...3510 */
    a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
    a1 = mullo(a1, 24583);

    /* invalid inputs might need reduction mod 7879 */
    a1 -= 7879;
    a1 += (a1 >> 15) & 7879;

    R2[318] = a0;
    R2[319] = a1;
    s -= 318;
    i = 143;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 1658), mulhiconst(mulloconst(A0, -4778), 3511)); /* -1756...2170 */
        A0 = add(A0, S1); /* -1756...2425 */
        A0 = sub(mulhiconst(A0, 1658), mulhiconst(mulloconst(A0, -4778), 3511)); /* -1800...1816 */
        A0 = add(A0, S0); /* -1800...2071 */
        A0 = ifnegaddconst(A0, 3511); /* 0...3510 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, 24583);

        /* invalid inputs might need reduction mod 3511 */
        A1 = ifgesubconst(A1, 3511);

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

    /* R2 ------> R1: reconstruct mod 638*[948]+[7879] */

    R1[638] = R2[319];
    s -= 319;
    i = 303;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 460), mulhiconst(mulloconst(A0, -17697), 948)); /* -474...589 */
        A0 = add(A0, S0); /* -474...844 */
        A0 = ifnegaddconst(A0, 948); /* 0...947 */
        A1 = add(shiftleftconst(A2, 6), signedshiftrightconst(sub(S0, A0), 2));
        A1 = mulloconst(A1, 23781);

        /* invalid inputs might need reduction mod 948 */
        A1 = ifgesubconst(A1, 948);

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

    /* R1 ------> R0: reconstruct mod 1277*[7879] */

    R0[1276] = R1[638] - 3939;
    s -= 1276;
    i = 622;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 2825), mulhiconst(mulloconst(A0, -2129), 7879)); /* -3940...4645 */
        A0 = add(A0, S1); /* -3940...4900 */
        A0 = sub(mulhiconst(A0, 2825), mulhiconst(mulloconst(A0, -2129), 7879)); /* -4110...4150 */
        A0 = add(A0, S0); /* -4110...4405 */
        A0 = ifnegaddconst(A0, 7879); /* 0...7878 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, 17143);

        /* invalid inputs might need reduction mod 7879 */
        A1 = ifgesubconst(A1, 7879);

        A0 = subconst(A0, 3939);
        A1 = subconst(A1, 3939);
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
