#include "crypto_decode_653x1541.h"
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

void PQCLEAN_NTRULPR653_AVX2_crypto_decode_653x1541(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[327], R2[164], R3[82], R4[41], R5[21], R6[11], R7[6], R8[3], R9[2], R10[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_NTRULPR653_AVX2_crypto_decode_653x1541_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 = mulhi(a1, -48) - mulhi(mullo(a1, -6433), 2608);
    a1 += *--s; /* -1304...1558 */
    a1 += (a1 >> 15) & 2608; /* 0...2607 */
    R10[0] = a1;

    /* R10 ------> R9: reconstruct mod 1*[71]+[9402] */

    i = 0;
    s -= 1;
    a2 = a0 = R10[0];
    a0 = mulhi(a0, -13) - mulhi(mullo(a0, 25845), 71); /* -39...35 */
    a0 += s[1 * i + 0]; /* -39...290 */
    a0 = mulhi(a0, 3) - mulhi(mullo(a0, -923), 71); /* -36...35 */
    a0 += (a0 >> 15) & 71; /* 0...70 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, -22153);

    /* invalid inputs might need reduction mod 9402 */
    a1 -= 9402;
    a1 += (a1 >> 15) & 9402;

    R9[0] = a0;
    R9[1] = a1;
    s -= 0;

    /* R9 ------> R8: reconstruct mod 2*[134]+[9402] */

    R8[2] = R9[1];
    s -= 1;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, 14) - mulhi(mullo(a0, 5869), 134); /* -67...70 */
        a0 += s[1 * i + 0]; /* -67...325 */
        a0 = mulhi(a0, 10) - mulhi(mullo(a0, -489), 134); /* -68...67 */
        a0 += (a0 >> 15) & 134; /* 0...133 */
        a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
        a1 = mullo(a1, 19563);

        /* invalid inputs might need reduction mod 134 */
        a1 -= 134;
        a1 += (a1 >> 15) & 134;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 5*[2953]+[815] */

    i = 0;
    s -= 1;
    a2 = a0 = R8[2];
    a0 = mulhi(a0, 1223) - mulhi(mullo(a0, -5681), 2953); /* -1477...1782 */
    a0 += s[1 * i + 0]; /* -1477...2037 */
    a0 += (a0 >> 15) & 2953; /* 0...2952 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, -9543);

    /* invalid inputs might need reduction mod 815 */
    a1 -= 815;
    a1 += (a1 >> 15) & 815;

    R7[4] = a0;
    R7[5] = a1;
    s -= 4;
    for (i = 1; i >= 0; --i) {
        a0 = R8[i];
        a0 = mulhi(a0, 1223) - mulhi(mullo(a0, -5681), 2953); /* -1477...1782 */
        a0 += s[2 * i + 1]; /* -1477...2037 */
        a0 = mulhi(a0, 1223) - mulhi(mullo(a0, -5681), 2953); /* -1505...1514 */
        a0 += s[2 * i + 0]; /* -1505...1769 */
        a0 += (a0 >> 15) & 2953; /* 0...2952 */
        a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
        a1 = mullo(a1, -9543);

        /* invalid inputs might need reduction mod 2953 */
        a1 -= 2953;
        a1 += (a1 >> 15) & 2953;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 10*[13910]+[815] */

    R6[10] = R7[5];
    s -= 10;
    for (i = 4; i >= 0; --i) {
        a2 = a0 = R7[i];
        a0 = mulhi(a0, 1756) - mulhi(mullo(a0, -1206), 13910); /* -6955...7394 */
        a0 += s[2 * i + 1]; /* -6955...7649 */
        a0 = mulhi(a0, 1756) - mulhi(mullo(a0, -1206), 13910); /* -7142...7159 */
        a0 += s[2 * i + 0]; /* -7142...7414 */
        a0 += (a0 >> 15) & 13910; /* 0...13909 */
        a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
        a1 = mullo(a1, -13437);

        /* invalid inputs might need reduction mod 13910 */
        a1 -= 13910;
        a1 += (a1 >> 15) & 13910;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 20*[1887]+[815] */

    R5[20] = R6[10];
    s -= 10;
    for (i = 9; i >= 0; --i) {
        a2 = a0 = R6[i];
        a0 = mulhi(a0, -101) - mulhi(mullo(a0, -8891), 1887); /* -969...943 */
        a0 += s[1 * i + 0]; /* -969...1198 */
        a0 += (a0 >> 15) & 1887; /* 0...1886 */
        a1 = (int16) ((a2 << 8) + s[i] - a0);
        a1 = mullo(a1, 5279);

        /* invalid inputs might need reduction mod 1887 */
        a1 -= 1887;
        a1 += (a1 >> 15) & 1887;

        R5[2 * i] = a0;
        R5[2 * i + 1] = a1;
    }

    /* R5 ------> R4: reconstruct mod 40*[695]+[815] */

    R4[40] = R5[20];
    s -= 20;
    i = 4;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -84), mulhiconst(mulloconst(A0, -24140), 695)); /* -369...347 */
        A0 = add(A0, S0); /* -369...602 */
        A0 = ifnegaddconst(A0, 695); /* 0...694 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, 31495);

        /* invalid inputs might need reduction mod 695 */
        A1 = ifgesubconst(A1, 695);

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

    /* R4 ------> R3: reconstruct mod 81*[6745]+[7910] */

    i = 0;
    s -= 2;
    a0 = R4[40];
    a0 = mulhi(a0, 2401) - mulhi(mullo(a0, -2487), 6745); /* -3373...3972 */
    a0 += s[2 * i + 1]; /* -3373...4227 */
    a0 = mulhi(a0, 2401) - mulhi(mullo(a0, -2487), 6745); /* -3497...3527 */
    a0 += s[2 * i + 0]; /* -3497...3782 */
    a0 += (a0 >> 15) & 6745; /* 0...6744 */
    a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
    a1 = mullo(a1, -29207);

    /* invalid inputs might need reduction mod 7910 */
    a1 -= 7910;
    a1 += (a1 >> 15) & 7910;

    R3[80] = a0;
    R3[81] = a1;
    s -= 80;
    i = 24;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 2401), mulhiconst(mulloconst(A0, -2487), 6745)); /* -3373...3972 */
        A0 = add(A0, S1); /* -3373...4227 */
        A0 = sub(mulhiconst(A0, 2401), mulhiconst(mulloconst(A0, -2487), 6745)); /* -3497...3527 */
        A0 = add(A0, S0); /* -3497...3782 */
        A0 = ifnegaddconst(A0, 6745); /* 0...6744 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, -29207);

        /* invalid inputs might need reduction mod 6745 */
        A1 = ifgesubconst(A1, 6745);

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

    /* R3 ------> R2: reconstruct mod 163*[1314]+[1541] */

    i = 0;
    s -= 1;
    a2 = a0 = R3[81];
    a0 = mulhi(a0, 64) - mulhi(mullo(a0, -12768), 1314); /* -657...673 */
    a0 += s[1 * i + 0]; /* -657...928 */
    a0 += (a0 >> 15) & 1314; /* 0...1313 */
    a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
    a1 = mullo(a1, -399);

    /* invalid inputs might need reduction mod 1541 */
    a1 -= 1541;
    a1 += (a1 >> 15) & 1541;

    R2[162] = a0;
    R2[163] = a1;
    s -= 81;
    i = 65;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 64), mulhiconst(mulloconst(A0, -12768), 1314)); /* -657...673 */
        A0 = add(A0, S0); /* -657...928 */
        A0 = ifnegaddconst(A0, 1314); /* 0...1313 */
        A1 = add(shiftleftconst(A2, 7), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, -399);

        /* invalid inputs might need reduction mod 1314 */
        A1 = ifgesubconst(A1, 1314);

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

    /* R2 ------> R1: reconstruct mod 326*[9277]+[1541] */

    R1[326] = R2[163];
    s -= 326;
    i = 147;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 4400), mulhiconst(mulloconst(A0, -1808), 9277)); /* -4639...5738 */
        A0 = add(A0, S1); /* -4639...5993 */
        A0 = sub(mulhiconst(A0, 4400), mulhiconst(mulloconst(A0, -1808), 9277)); /* -4950...5040 */
        A0 = add(A0, S0); /* -4950...5295 */
        A0 = ifnegaddconst(A0, 9277); /* 0...9276 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, -27883);

        /* invalid inputs might need reduction mod 9277 */
        A1 = ifgesubconst(A1, 9277);

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

    /* R1 ------> R0: reconstruct mod 653*[1541] */

    R0[652] = (int16) (3 * R1[326] - 2310);
    s -= 326;
    i = 310;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 349), mulhiconst(mulloconst(A0, -10887), 1541)); /* -771...857 */
        A0 = add(A0, S0); /* -771...1112 */
        A0 = ifnegaddconst(A0, 1541); /* 0...1540 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, -10547);

        /* invalid inputs might need reduction mod 1541 */
        A1 = ifgesubconst(A1, 1541);

        A0 = mulloconst(A0, 3);
        A1 = mulloconst(A1, 3);
        A0 = subconst(A0, 2310);
        A1 = subconst(A1, 2310);
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
