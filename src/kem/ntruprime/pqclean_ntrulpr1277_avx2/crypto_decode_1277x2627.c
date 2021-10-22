#include "crypto_decode_1277x2627.h"
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

void PQCLEAN_NTRULPR1277_AVX2_crypto_decode_1277x2627(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[639], R2[320], R3[160], R4[80], R5[40], R6[20], R7[10], R8[5], R9[3], R10[2], R11[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_NTRULPR1277_AVX2_crypto_decode_1277x2627_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 = mulhi(a1, 16) - mulhi(mullo(a1, 1986), 264);
    a1 += *--s; /* -132...387 */
    a1 -= 264; /* -396...123 */
    a1 += (a1 >> 15) & 264; /* -132...263 */
    a1 += (a1 >> 15) & 264; /* 0...263 */
    R11[0] = a1;

    /* R11 ------> R10: reconstruct mod 1*[7744]+[2229] */

    i = 0;
    s -= 2;
    a2 = a0 = R11[0];
    a0 = mulhi(a0, 3712) - mulhi(mullo(a0, -2166), 7744); /* -3872...4800 */
    a0 += s[2 * i + 1]; /* -3872...5055 */
    a0 = mulhi(a0, 3712) - mulhi(mullo(a0, -2166), 7744); /* -4092...4158 */
    a0 += s[2 * i + 0]; /* -4092...4413 */
    a0 += (a0 >> 15) & 7744; /* 0...7743 */
    a1 = (int16) ((a2 << 10) + (s[2 * i + 1] << 2) + ((s[2 * i] - a0) >> 6));
    a1 = mullo(a1, 27081);

    /* invalid inputs might need reduction mod 2229 */
    a1 -= 2229;
    a1 += (a1 >> 15) & 2229;

    R10[0] = a0;
    R10[1] = a1;
    s -= 0;

    /* R10 ------> R9: reconstruct mod 2*[1408]+[2229] */

    R9[2] = R10[1];
    s -= 1;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R10[i];
        a0 = mulhi(a0, -512) - mulhi(mullo(a0, -11916), 1408); /* -832...704 */
        a0 += s[1 * i + 0]; /* -832...959 */
        a0 += (a0 >> 15) & 1408; /* 0...1407 */
        a1 = (int16) ((a2 << 1) + ((s[i] - a0) >> 7));
        a1 = mullo(a1, -29789);

        /* invalid inputs might need reduction mod 1408 */
        a1 -= 1408;
        a1 += (a1 >> 15) & 1408;

        R9[2 * i] = a0;
        R9[2 * i + 1] = a1;
    }

    /* R9 ------> R8: reconstruct mod 4*[9604]+[2229] */

    R8[4] = R9[2];
    s -= 4;
    for (i = 1; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, -972) - mulhi(mullo(a0, -1747), 9604); /* -5045...4802 */
        a0 += s[2 * i + 1]; /* -5045...5057 */
        a0 = mulhi(a0, -972) - mulhi(mullo(a0, -1747), 9604); /* -4878...4876 */
        a0 += s[2 * i + 0]; /* -4878...5131 */
        a0 += (a0 >> 15) & 9604; /* 0...9603 */
        a1 = (int16) ((a2 << 14) + (s[2 * i + 1] << 6) + ((s[2 * i] - a0) >> 2));
        a1 = mullo(a1, 23201);

        /* invalid inputs might need reduction mod 9604 */
        a1 -= 9604;
        a1 += (a1 >> 15) & 9604;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 9*[98]+[5822] */

    i = 0;
    s -= 1;
    a2 = a0 = R8[4];
    a0 = mulhi(a0, 8) - mulhi(mullo(a0, 25412), 98); /* -49...51 */
    a0 += s[1 * i + 0]; /* -49...306 */
    a0 = mulhi(a0, -26) - mulhi(mullo(a0, -669), 98); /* -50...49 */
    a0 += (a0 >> 15) & 98; /* 0...97 */
    a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
    a1 = mullo(a1, 22737);

    /* invalid inputs might need reduction mod 5822 */
    a1 -= 5822;
    a1 += (a1 >> 15) & 5822;

    R7[8] = a0;
    R7[9] = a1;
    s -= 0;
    for (i = 3; i >= 0; --i) {
        a2 = a0 = R8[i];
        a0 = mulhi(a0, -26) - mulhi(mullo(a0, -669), 98); /* -56...49 */
        a0 += (a0 >> 15) & 98; /* 0...97 */
        a1 = (int16) ((a2 - a0) >> 1);
        a1 = mullo(a1, 22737);

        /* invalid inputs might need reduction mod 98 */
        a1 -= 98;
        a1 += (a1 >> 15) & 98;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 19*[158]+[9433] */

    i = 0;
    s -= 1;
    a2 = a0 = R7[9];
    a0 = mulhi(a0, -14) - mulhi(mullo(a0, 24887), 158); /* -83...79 */
    a0 += s[1 * i + 0]; /* -83...334 */
    a0 = mulhi(a0, -34) - mulhi(mullo(a0, -415), 158); /* -80...79 */
    a0 += (a0 >> 15) & 158; /* 0...157 */
    a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
    a1 = mullo(a1, 5807);

    /* invalid inputs might need reduction mod 9433 */
    a1 -= 9433;
    a1 += (a1 >> 15) & 9433;

    R6[18] = a0;
    R6[19] = a1;
    s -= 9;
    for (i = 8; i >= 0; --i) {
        a2 = a0 = R7[i];
        a0 = mulhi(a0, -14) - mulhi(mullo(a0, 24887), 158); /* -83...79 */
        a0 += s[1 * i + 0]; /* -83...334 */
        a0 = mulhi(a0, -34) - mulhi(mullo(a0, -415), 158); /* -80...79 */
        a0 += (a0 >> 15) & 158; /* 0...157 */
        a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
        a1 = mullo(a1, 5807);

        /* invalid inputs might need reduction mod 158 */
        a1 -= 158;
        a1 += (a1 >> 15) & 158;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 39*[3211]+[752] */

    i = 0;
    s -= 1;
    a2 = a0 = R6[19];
    a0 = mulhi(a0, -259) - mulhi(mullo(a0, -5225), 3211); /* -1671...1605 */
    a0 += s[1 * i + 0]; /* -1671...1860 */
    a0 += (a0 >> 15) & 3211; /* 0...3210 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, -1245);

    /* invalid inputs might need reduction mod 752 */
    a1 -= 752;
    a1 += (a1 >> 15) & 752;

    R5[38] = a0;
    R5[39] = a1;
    s -= 38;
    i = 3;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R6[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -259), mulhiconst(mulloconst(A0, -5225), 3211)); /* -1671...1605 */
        A0 = add(A0, S1); /* -1671...1860 */
        A0 = sub(mulhiconst(A0, -259), mulhiconst(mulloconst(A0, -5225), 3211)); /* -1613...1612 */
        A0 = add(A0, S0); /* -1613...1867 */
        A0 = ifnegaddconst(A0, 3211); /* 0...3210 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, -1245);

        /* invalid inputs might need reduction mod 3211 */
        A1 = ifgesubconst(A1, 3211);

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

    /* R5 ------> R4: reconstruct mod 79*[14506]+[3395] */

    i = 0;
    s -= 2;
    a2 = a0 = R5[39];
    a0 = mulhi(a0, -6226) - mulhi(mullo(a0, -1157), 14506); /* -8810...7253 */
    a0 += s[2 * i + 1]; /* -8810...7508 */
    a0 = mulhi(a0, -6226) - mulhi(mullo(a0, -1157), 14506); /* -7967...8089 */
    a0 += s[2 * i + 0]; /* -7967...8344 */
    a0 += (a0 >> 15) & 14506; /* 0...14505 */
    a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
    a1 = mullo(a1, 253);

    /* invalid inputs might need reduction mod 3395 */
    a1 -= 3395;
    a1 += (a1 >> 15) & 3395;

    R4[78] = a0;
    R4[79] = a1;
    s -= 78;
    i = 23;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -6226), mulhiconst(mulloconst(A0, -1157), 14506)); /* -8810...7253 */
        A0 = add(A0, S1); /* -8810...7508 */
        A0 = sub(mulhiconst(A0, -6226), mulhiconst(mulloconst(A0, -1157), 14506)); /* -7967...8089 */
        A0 = add(A0, S0); /* -7967...8344 */
        A0 = ifnegaddconst(A0, 14506); /* 0...14505 */
        A1 = add(add(shiftleftconst(A2, 15), shiftleftconst(S1, 7)), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, 253);

        /* invalid inputs might need reduction mod 14506 */
        A1 = ifgesubconst(A1, 14506);

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

    /* R4 ------> R3: reconstruct mod 159*[1927]+[451] */

    i = 0;
    s -= 1;
    a2 = a0 = R4[79];
    a0 = mulhi(a0, 754) - mulhi(mullo(a0, -8706), 1927); /* -964...1152 */
    a0 += s[1 * i + 0]; /* -964...1407 */
    a0 += (a0 >> 15) & 1927; /* 0...1926 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 3639);

    /* invalid inputs might need reduction mod 451 */
    a1 -= 451;
    a1 += (a1 >> 15) & 451;

    R3[158] = a0;
    R3[159] = a1;
    s -= 79;
    i = 63;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 754), mulhiconst(mulloconst(A0, -8706), 1927)); /* -964...1152 */
        A0 = add(A0, S0); /* -964...1407 */
        A0 = ifnegaddconst(A0, 1927); /* 0...1926 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, 3639);

        /* invalid inputs might need reduction mod 1927 */
        A1 = ifgesubconst(A1, 1927);

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

    /* R3 ------> R2: reconstruct mod 319*[11236]+[2627] */

    i = 0;
    s -= 2;
    a2 = a0 = R3[159];
    a0 = mulhi(a0, 1868) - mulhi(mullo(a0, -1493), 11236); /* -5618...6085 */
    a0 += s[2 * i + 1]; /* -5618...6340 */
    a0 = mulhi(a0, 1868) - mulhi(mullo(a0, -1493), 11236); /* -5779...5798 */
    a0 += s[2 * i + 0]; /* -5779...6053 */
    a0 += (a0 >> 15) & 11236; /* 0...11235 */
    a1 = (int16) ((a2 << 14) + (s[2 * i + 1] << 6) + ((s[2 * i] - a0) >> 2));
    a1 = mullo(a1, -26807);

    /* invalid inputs might need reduction mod 2627 */
    a1 -= 2627;
    a1 += (a1 >> 15) & 2627;

    R2[318] = a0;
    R2[319] = a1;
    s -= 318;
    i = 143;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 1868), mulhiconst(mulloconst(A0, -1493), 11236)); /* -5618...6085 */
        A0 = add(A0, S1); /* -5618...6340 */
        A0 = sub(mulhiconst(A0, 1868), mulhiconst(mulloconst(A0, -1493), 11236)); /* -5779...5798 */
        A0 = add(A0, S0); /* -5779...6053 */
        A0 = ifnegaddconst(A0, 11236); /* 0...11235 */
        A1 = add(add(shiftleftconst(A2, 14), shiftleftconst(S1, 6)), signedshiftrightconst(sub(S0, A0), 2));
        A1 = mulloconst(A1, -26807);

        /* invalid inputs might need reduction mod 11236 */
        A1 = ifgesubconst(A1, 11236);

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

    /* R2 ------> R1: reconstruct mod 638*[106]+[2627] */

    R1[638] = R2[319];
    s -= 0;
    i = 303;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        A0 = sub(mulhiconst(A0, 28), mulhiconst(mulloconst(A0, -618), 106)); /* -53...60 */
        A0 = ifnegaddconst(A0, 106); /* 0...105 */
        A1 = signedshiftrightconst(sub(A2, A0), 1);
        A1 = mulloconst(A1, 21021);

        /* invalid inputs might need reduction mod 106 */
        A1 = ifgesubconst(A1, 106);

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

    /* R1 ------> R0: reconstruct mod 1277*[2627] */

    R0[1276] = (int16) (3 * R1[638] - 3939);
    s -= 1276;
    i = 622;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 1194), mulhiconst(mulloconst(A0, -6386), 2627)); /* -1314...1612 */
        A0 = add(A0, S1); /* -1314...1867 */
        A0 = sub(mulhiconst(A0, 1194), mulhiconst(mulloconst(A0, -6386), 2627)); /* -1338...1347 */
        A0 = add(A0, S0); /* -1338...1602 */
        A0 = ifnegaddconst(A0, 2627); /* 0...2626 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, 4715);

        /* invalid inputs might need reduction mod 2627 */
        A1 = ifgesubconst(A1, 2627);

        A0 = mulloconst(A0, 3);
        A1 = mulloconst(A1, 3);
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
