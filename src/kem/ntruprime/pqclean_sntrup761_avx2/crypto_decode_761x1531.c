#include "crypto_decode_761x1531.h"
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

void PQCLEAN_SNTRUP761_AVX2_crypto_decode_761x1531(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[381], R2[191], R3[96], R4[48], R5[24], R6[12], R7[6], R8[3], R9[2], R10[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_SNTRUP761_AVX2_crypto_decode_761x1531_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 = mulhi(a1, -84) - mulhi(mullo(a1, -4828), 3475);
    a1 += *--s; /* -1738...1992 */
    a1 += (a1 >> 15) & 3475; /* 0...3474 */
    R10[0] = a1;

    /* R10 ------> R9: reconstruct mod 1*[593]+[1500] */

    i = 0;
    s -= 1;
    a2 = a0 = R10[0];
    a0 = mulhi(a0, 60) - mulhi(mullo(a0, -28292), 593); /* -297...311 */
    a0 += s[1 * i + 0]; /* -297...566 */
    a0 += (a0 >> 15) & 593; /* 0...592 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, -31055);

    /* invalid inputs might need reduction mod 1500 */
    a1 -= 1500;
    a1 += (a1 >> 15) & 1500;

    R9[0] = a0;
    R9[1] = a1;
    s -= 0;

    /* R9 ------> R8: reconstruct mod 2*[6232]+[1500] */

    R8[2] = R9[1];
    s -= 2;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, 672) - mulhi(mullo(a0, -2692), 6232); /* -3116...3284 */
        a0 += s[2 * i + 1]; /* -3116...3539 */
        a0 = mulhi(a0, 672) - mulhi(mullo(a0, -2692), 6232); /* -3148...3152 */
        a0 += s[2 * i + 0]; /* -3148...3407 */
        a0 += (a0 >> 15) & 6232; /* 0...6231 */
        a1 = (int16) ((a2 << 13) + (s[2 * i + 1] << 5) + ((s[2 * i] - a0) >> 3));
        a1 = mullo(a1, 12451);

        /* invalid inputs might need reduction mod 6232 */
        a1 -= 6232;
        a1 += (a1 >> 15) & 6232;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 5*[1263]+[304] */

    i = 0;
    s -= 1;
    a2 = a0 = R8[2];
    a0 = mulhi(a0, -476) - mulhi(mullo(a0, -13284), 1263); /* -751...631 */
    a0 += s[1 * i + 0]; /* -751...886 */
    a0 += (a0 >> 15) & 1263; /* 0...1262 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, -22001);

    /* invalid inputs might need reduction mod 304 */
    a1 -= 304;
    a1 += (a1 >> 15) & 304;

    R7[4] = a0;
    R7[5] = a1;
    s -= 2;
    for (i = 1; i >= 0; --i) {
        a2 = a0 = R8[i];
        a0 = mulhi(a0, -476) - mulhi(mullo(a0, -13284), 1263); /* -751...631 */
        a0 += s[1 * i + 0]; /* -751...886 */
        a0 += (a0 >> 15) & 1263; /* 0...1262 */
        a1 = (int16) ((a2 << 8) + s[i] - a0);
        a1 = mullo(a1, -22001);

        /* invalid inputs might need reduction mod 1263 */
        a1 -= 1263;
        a1 += (a1 >> 15) & 1263;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 11*[9097]+[2188] */

    i = 0;
    s -= 2;
    a0 = R7[5];
    a0 = mulhi(a0, 2348) - mulhi(mullo(a0, -1844), 9097); /* -4549...5135 */
    a0 += s[2 * i + 1]; /* -4549...5390 */
    a0 = mulhi(a0, 2348) - mulhi(mullo(a0, -1844), 9097); /* -4712...4741 */
    a0 += s[2 * i + 0]; /* -4712...4996 */
    a0 += (a0 >> 15) & 9097; /* 0...9096 */
    a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
    a1 = mullo(a1, 17081);

    /* invalid inputs might need reduction mod 2188 */
    a1 -= 2188;
    a1 += (a1 >> 15) & 2188;

    R6[10] = a0;
    R6[11] = a1;
    s -= 10;
    for (i = 4; i >= 0; --i) {
        a0 = R7[i];
        a0 = mulhi(a0, 2348) - mulhi(mullo(a0, -1844), 9097); /* -4549...5135 */
        a0 += s[2 * i + 1]; /* -4549...5390 */
        a0 = mulhi(a0, 2348) - mulhi(mullo(a0, -1844), 9097); /* -4712...4741 */
        a0 += s[2 * i + 0]; /* -4712...4996 */
        a0 += (a0 >> 15) & 9097; /* 0...9096 */
        a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
        a1 = mullo(a1, 17081);

        /* invalid inputs might need reduction mod 9097 */
        a1 -= 9097;
        a1 += (a1 >> 15) & 9097;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 23*[1526]+[367] */

    i = 0;
    s -= 1;
    a2 = a0 = R6[11];
    a0 = mulhi(a0, 372) - mulhi(mullo(a0, -10994), 1526); /* -763...856 */
    a0 += s[1 * i + 0]; /* -763...1111 */
    a0 += (a0 >> 15) & 1526; /* 0...1525 */
    a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
    a1 = mullo(a1, -18381);

    /* invalid inputs might need reduction mod 367 */
    a1 -= 367;
    a1 += (a1 >> 15) & 367;

    R5[22] = a0;
    R5[23] = a1;
    s -= 11;
    for (i = 10; i >= 0; --i) {
        a2 = a0 = R6[i];
        a0 = mulhi(a0, 372) - mulhi(mullo(a0, -10994), 1526); /* -763...856 */
        a0 += s[1 * i + 0]; /* -763...1111 */
        a0 += (a0 >> 15) & 1526; /* 0...1525 */
        a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
        a1 = mullo(a1, -18381);

        /* invalid inputs might need reduction mod 1526 */
        a1 -= 1526;
        a1 += (a1 >> 15) & 1526;

        R5[2 * i] = a0;
        R5[2 * i + 1] = a1;
    }

    /* R5 ------> R4: reconstruct mod 47*[625]+[150] */

    i = 0;
    s -= 1;
    a2 = a0 = R5[23];
    a0 = mulhi(a0, -284) - mulhi(mullo(a0, -26844), 625); /* -384...312 */
    a0 += s[1 * i + 0]; /* -384...567 */
    a0 += (a0 >> 15) & 625; /* 0...624 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 32401);

    /* invalid inputs might need reduction mod 150 */
    a1 -= 150;
    a1 += (a1 >> 15) & 150;

    R4[46] = a0;
    R4[47] = a1;
    s -= 23;
    i = 7;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -284), mulhiconst(mulloconst(A0, -26844), 625)); /* -384...312 */
        A0 = add(A0, S0); /* -384...567 */
        A0 = ifnegaddconst(A0, 625); /* 0...624 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, 32401);

        /* invalid inputs might need reduction mod 625 */
        A1 = ifgesubconst(A1, 625);

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

    /* R4 ------> R3: reconstruct mod 95*[6400]+[1531] */

    i = 0;
    s -= 2;
    a2 = a0 = R4[47];
    a0 = mulhi(a0, 2816) - mulhi(mullo(a0, -2621), 6400); /* -3200...3904 */
    a0 += s[2 * i + 1]; /* -3200...4159 */
    a0 = mulhi(a0, 2816) - mulhi(mullo(a0, -2621), 6400); /* -3338...3378 */
    a0 += s[2 * i + 0]; /* -3338...3633 */
    a0 += (a0 >> 15) & 6400; /* 0...6399 */
    a1 = (int16) ((a2 << 8) + s[2 * i + 1] + ((s[2 * i] - a0) >> 8));
    a1 = mullo(a1, 23593);

    /* invalid inputs might need reduction mod 1531 */
    a1 -= 1531;
    a1 += (a1 >> 15) & 1531;

    R3[94] = a0;
    R3[95] = a1;
    s -= 94;
    i = 31;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 2816), mulhiconst(mulloconst(A0, -2621), 6400)); /* -3200...3904 */
        A0 = add(A0, S1); /* -3200...4159 */
        A0 = sub(mulhiconst(A0, 2816), mulhiconst(mulloconst(A0, -2621), 6400)); /* -3338...3378 */
        A0 = add(A0, S0); /* -3338...3633 */
        A0 = ifnegaddconst(A0, 6400); /* 0...6399 */
        A1 = add(add(shiftleftconst(A2, 8), S1), signedshiftrightconst(sub(S0, A0), 8));
        A1 = mulloconst(A1, 23593);

        /* invalid inputs might need reduction mod 6400 */
        A1 = ifgesubconst(A1, 6400);

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

    /* R3 ------> R2: reconstruct mod 190*[1280]+[1531] */

    R2[190] = R3[95];
    s -= 95;
    i = 79;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 256), mulhiconst(mulloconst(A0, -13107), 1280)); /* -640...704 */
        A0 = add(A0, S0); /* -640...959 */
        A0 = ifnegaddconst(A0, 1280); /* 0...1279 */
        A1 = add(A2, signedshiftrightconst(sub(S0, A0), 8));
        A1 = mulloconst(A1, -13107);

        /* invalid inputs might need reduction mod 1280 */
        A1 = ifgesubconst(A1, 1280);

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

    /* R2 ------> R1: reconstruct mod 380*[9157]+[1531] */

    R1[380] = R2[190];
    s -= 380;
    i = 174;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 1592), mulhiconst(mulloconst(A0, -1832), 9157)); /* -4579...4976 */
        A0 = add(A0, S1); /* -4579...5231 */
        A0 = sub(mulhiconst(A0, 1592), mulhiconst(mulloconst(A0, -1832), 9157)); /* -4690...4705 */
        A0 = add(A0, S0); /* -4690...4960 */
        A0 = ifnegaddconst(A0, 9157); /* 0...9156 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, 25357);

        /* invalid inputs might need reduction mod 9157 */
        A1 = ifgesubconst(A1, 9157);

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

    /* R1 ------> R0: reconstruct mod 761*[1531] */

    R0[760] = (int16) (3 * R1[380] - 2295);
    s -= 380;
    i = 364;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 518), mulhiconst(mulloconst(A0, -10958), 1531)); /* -766...895 */
        A0 = add(A0, S0); /* -766...1150 */
        A0 = ifnegaddconst(A0, 1531); /* 0...1530 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, 15667);

        /* invalid inputs might need reduction mod 1531 */
        A1 = ifgesubconst(A1, 1531);

        A0 = mulloconst(A0, 3);
        A1 = mulloconst(A1, 3);
        A0 = subconst(A0, 2295);
        A1 = subconst(A1, 2295);
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
