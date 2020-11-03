#include "crypto_decode_653x4621.h"
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

void PQCLEAN_SNTRUP653_AVX2_crypto_decode_653x4621(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[327], R2[164], R3[82], R4[41], R5[21], R6[11], R7[6], R8[3], R9[2], R10[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_SNTRUP653_AVX2_crypto_decode_653x4621_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 -= 86; /* -86...169 */
    a1 -= 86; /* -172...83 */
    a1 += (a1 >> 15) & 86; /* -86...85 */
    a1 += (a1 >> 15) & 86; /* 0...85 */
    R10[0] = a1;

    /* R10 ------> R9: reconstruct mod 1*[835]+[6708] */

    i = 0;
    s -= 2;
    a0 = R10[0];
    a0 = mulhi(a0, 396) - mulhi(mullo(a0, -20092), 835); /* -418...516 */
    a0 += s[2 * i + 1]; /* -418...771 */
    a0 = mulhi(a0, 396) - mulhi(mullo(a0, -20092), 835); /* -421...422 */
    a0 += s[2 * i + 0]; /* -421...677 */
    a0 += (a0 >> 15) & 835; /* 0...834 */
    a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
    a1 = mullo(a1, 8555);

    /* invalid inputs might need reduction mod 6708 */
    a1 -= 6708;
    a1 += (a1 >> 15) & 6708;

    R9[0] = a0;
    R9[1] = a1;
    s -= 0;

    /* R9 ------> R8: reconstruct mod 2*[7396]+[6708] */

    R8[2] = R9[1];
    s -= 2;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, 3088) - mulhi(mullo(a0, -2268), 7396); /* -3698...4470 */
        a0 += s[2 * i + 1]; /* -3698...4725 */
        a0 = mulhi(a0, 3088) - mulhi(mullo(a0, -2268), 7396); /* -3873...3920 */
        a0 += s[2 * i + 0]; /* -3873...4175 */
        a0 += (a0 >> 15) & 7396; /* 0...7395 */
        a1 = (int16) ((a2 << 14) + (s[2 * i + 1] << 6) + ((s[2 * i] - a0) >> 2));
        a1 = mullo(a1, -18679);

        /* invalid inputs might need reduction mod 7396 */
        a1 -= 7396;
        a1 += (a1 >> 15) & 7396;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 5*[86]+[78] */

    s -= 0;
    a2 = a0 = R8[2];
    a0 = mulhi(a0, 4) - mulhi(mullo(a0, -762), 86); /* -43...44 */
    a0 += (a0 >> 15) & 86; /* 0...85 */
    a1 = (int16) ((a2 - a0) >> 1);
    a1 = mullo(a1, -16765);

    /* invalid inputs might need reduction mod 78 */
    a1 -= 78;
    a1 += (a1 >> 15) & 78;

    R7[4] = a0;
    R7[5] = a1;
    s -= 0;
    for (i = 1; i >= 0; --i) {
        a2 = a0 = R8[i];
        a0 = mulhi(a0, 4) - mulhi(mullo(a0, -762), 86); /* -43...44 */
        a0 += (a0 >> 15) & 86; /* 0...85 */
        a1 = (int16) ((a2 - a0) >> 1);
        a1 = mullo(a1, -16765);

        /* invalid inputs might need reduction mod 86 */
        a1 -= 86;
        a1 += (a1 >> 15) & 86;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 10*[2370]+[78] */

    R6[10] = R7[5];
    s -= 10;
    for (i = 4; i >= 0; --i) {
        a2 = a0 = R7[i];
        a0 = mulhi(a0, -14) - mulhi(mullo(a0, -7079), 2370); /* -1189...1185 */
        a0 += s[2 * i + 1]; /* -1189...1440 */
        a0 = mulhi(a0, -14) - mulhi(mullo(a0, -7079), 2370); /* -1186...1185 */
        a0 += s[2 * i + 0]; /* -1186...1440 */
        a0 += (a0 >> 15) & 2370; /* 0...2369 */
        a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
        a1 = mullo(a1, -8351);

        /* invalid inputs might need reduction mod 2370 */
        a1 -= 2370;
        a1 += (a1 >> 15) & 2370;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 20*[12461]+[78] */

    R5[20] = R6[10];
    s -= 20;
    for (i = 9; i >= 0; --i) {
        a0 = R6[i];
        a0 = mulhi(a0, 4710) - mulhi(mullo(a0, -1346), 12461); /* -6231...7408 */
        a0 += s[2 * i + 1]; /* -6231...7663 */
        a0 = mulhi(a0, 4710) - mulhi(mullo(a0, -1346), 12461); /* -6679...6781 */
        a0 += s[2 * i + 0]; /* -6679...7036 */
        a0 += (a0 >> 15) & 12461; /* 0...12460 */
        a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
        a1 = mullo(a1, -19675);

        /* invalid inputs might need reduction mod 12461 */
        a1 -= 12461;
        a1 += (a1 >> 15) & 12461;

        R5[2 * i] = a0;
        R5[2 * i + 1] = a1;
    }

    /* R5 ------> R4: reconstruct mod 40*[1786]+[78] */

    R4[40] = R5[20];
    s -= 20;
    i = 4;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -468), mulhiconst(mulloconst(A0, -9394), 1786)); /* -1010...893 */
        A0 = add(A0, S0); /* -1010...1148 */
        A0 = ifnegaddconst(A0, 1786); /* 0...1785 */
        A1 = add(shiftleftconst(A2, 7), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, -12843);

        /* invalid inputs might need reduction mod 1786 */
        A1 = ifgesubconst(A1, 1786);

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

    /* R4 ------> R3: reconstruct mod 81*[676]+[7510] */

    i = 0;
    s -= 2;
    a2 = a0 = R4[40];
    a0 = mulhi(a0, 248) - mulhi(mullo(a0, -24818), 676); /* -338...400 */
    a0 += s[2 * i + 1]; /* -338...655 */
    a0 = mulhi(a0, 248) - mulhi(mullo(a0, -24818), 676); /* -340...340 */
    a0 += s[2 * i + 0]; /* -340...595 */
    a0 += (a0 >> 15) & 676; /* 0...675 */
    a1 = (int16) ((a2 << 14) + (s[2 * i + 1] << 6) + ((s[2 * i] - a0) >> 2));
    a1 = mullo(a1, -23655);

    /* invalid inputs might need reduction mod 7510 */
    a1 -= 7510;
    a1 += (a1 >> 15) & 7510;

    R3[80] = a0;
    R3[81] = a1;
    s -= 40;
    i = 24;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 248), mulhiconst(mulloconst(A0, -24818), 676)); /* -338...400 */
        A0 = add(A0, S0); /* -338...655 */
        A0 = ifnegaddconst(A0, 676); /* 0...675 */
        A1 = add(shiftleftconst(A2, 6), signedshiftrightconst(sub(S0, A0), 2));
        A1 = mulloconst(A1, -23655);

        /* invalid inputs might need reduction mod 676 */
        A1 = ifgesubconst(A1, 676);

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

    /* R3 ------> R2: reconstruct mod 163*[416]+[4621] */

    i = 0;
    s -= 1;
    a2 = a0 = R3[81];
    a0 = mulhi(a0, -64) - mulhi(mullo(a0, 25206), 416); /* -224...208 */
    a0 += s[1 * i + 0]; /* -224...463 */
    a0 -= 416; /* -640..>47 */
    a0 += (a0 >> 15) & 416; /* -224...415 */
    a0 += (a0 >> 15) & 416; /* 0...415 */
    a1 = (int16) ((a2 << 3) + ((s[i] - a0) >> 5));
    a1 = mullo(a1, 20165);

    /* invalid inputs might need reduction mod 4621 */
    a1 -= 4621;
    a1 += (a1 >> 15) & 4621;

    R2[162] = a0;
    R2[163] = a1;
    s -= 81;
    i = 65;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -64), mulhiconst(mulloconst(A0, 25206), 416)); /* -224...208 */
        A0 = add(A0, S0); /* -224...463 */
        A0 = subconst(A0, 416); /* -640...47 */
        A0 = ifnegaddconst(A0, 416); /* -224...415 */
        A0 = ifnegaddconst(A0, 416); /* 0...415 */
        A1 = add(shiftleftconst(A2, 3), signedshiftrightconst(sub(S0, A0), 5));
        A1 = mulloconst(A1, 20165);

        /* invalid inputs might need reduction mod 416 */
        A1 = ifgesubconst(A1, 416);

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

    /* R2 ------> R1: reconstruct mod 326*[326]+[4621] */

    R1[326] = R2[163];
    s -= 163;
    i = 147;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -48), mulhiconst(mulloconst(A0, 14072), 326)); /* -175...163 */
        A0 = add(A0, S0); /* -175...418 */
        A0 = subconst(A0, 326); /* -501...92 */
        A0 = ifnegaddconst(A0, 326); /* -175...325 */
        A0 = ifnegaddconst(A0, 326); /* 0...325 */
        A1 = add(shiftleftconst(A2, 7), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, -19701);

        /* invalid inputs might need reduction mod 326 */
        A1 = ifgesubconst(A1, 326);

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

    /* R1 ------> R0: reconstruct mod 653*[4621] */

    R0[652] = R1[326] - 2310;
    s -= 652;
    i = 310;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -1635), mulhiconst(mulloconst(A0, -3631), 4621)); /* -2720...2310 */
        A0 = add(A0, S1); /* -2720...2565 */
        A0 = sub(mulhiconst(A0, -1635), mulhiconst(mulloconst(A0, -3631), 4621)); /* -2375...2378 */
        A0 = add(A0, S0); /* -2375...2633 */
        A0 = ifnegaddconst(A0, 4621); /* 0...4620 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, -29499);

        /* invalid inputs might need reduction mod 4621 */
        A1 = ifgesubconst(A1, 4621);

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
