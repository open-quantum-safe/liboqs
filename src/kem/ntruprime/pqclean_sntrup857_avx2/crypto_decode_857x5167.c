#include "crypto_decode_857x5167.h"
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

void PQCLEAN_SNTRUP857_AVX2_crypto_decode_857x5167(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[429], R2[215], R3[108], R4[54], R5[27], R6[14], R7[7], R8[4], R9[2], R10[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_SNTRUP857_AVX2_crypto_decode_857x5167_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 = mulhi(a1, 841) - mulhi(mullo(a1, -2695), 6225);
    a1 += *--s; /* -3113...3370 */
    a1 += (a1 >> 15) & 6225; /* 0...6224 */
    R10[0] = a1;

    /* R10 ------> R9: reconstruct mod 1*[5476]+[291] */

    i = 0;
    s -= 1;
    a2 = a0 = R10[0];
    a0 = mulhi(a0, -1248) - mulhi(mullo(a0, -3064), 5476); /* -3050...2738 */
    a0 += s[1 * i + 0]; /* -3050...2993 */
    a0 += (a0 >> 15) & 5476; /* 0...5475 */
    a1 = (int16) ((a2 << 6) + ((s[i] - a0) >> 2));
    a1 = mullo(a1, -3351);

    /* invalid inputs might need reduction mod 291 */
    a1 -= 291;
    a1 += (a1 >> 15) & 291;

    R9[0] = a0;
    R9[1] = a1;
    s -= 0;

    /* R9 ------> R8: reconstruct mod 3*[74]+[1004] */

    i = 0;
    s -= 1;
    a2 = a0 = R9[1];
    a0 = mulhi(a0, 10) - mulhi(mullo(a0, -30111), 74); /* -37...39 */
    a0 += s[1 * i + 0]; /* -37...294 */
    a0 = mulhi(a0, -28) - mulhi(mullo(a0, -886), 74); /* -38...37 */
    a0 += (a0 >> 15) & 74; /* 0...73 */
    a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
    a1 = mullo(a1, 7085);

    /* invalid inputs might need reduction mod 1004 */
    a1 -= 1004;
    a1 += (a1 >> 15) & 1004;

    R8[2] = a0;
    R8[3] = a1;
    s -= 0;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, -28) - mulhi(mullo(a0, -886), 74); /* -44...37 */
        a0 += (a0 >> 15) & 74; /* 0...73 */
        a1 = (int16) ((a2 - a0) >> 1);
        a1 = mullo(a1, 7085);

        /* invalid inputs might need reduction mod 74 */
        a1 -= 74;
        a1 += (a1 >> 15) & 74;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 6*[2194]+[1004] */

    R7[6] = R8[3];
    s -= 6;
    for (i = 2; i >= 0; --i) {
        a2 = a0 = R8[i];
        a0 = mulhi(a0, -302) - mulhi(mullo(a0, -7647), 2194); /* -1173...1097 */
        a0 += s[2 * i + 1]; /* -1173...1352 */
        a0 = mulhi(a0, -302) - mulhi(mullo(a0, -7647), 2194); /* -1104...1102 */
        a0 += s[2 * i + 0]; /* -1104...1357 */
        a0 += (a0 >> 15) & 2194; /* 0...2193 */
        a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
        a1 = mullo(a1, 11769);

        /* invalid inputs might need reduction mod 2194 */
        a1 -= 2194;
        a1 += (a1 >> 15) & 2194;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 13*[11991]+[5483] */

    i = 0;
    s -= 2;
    a0 = R7[6];
    a0 = mulhi(a0, 1807) - mulhi(mullo(a0, -1399), 11991); /* -5996...6447 */
    a0 += s[2 * i + 1]; /* -5996...6702 */
    a0 = mulhi(a0, 1807) - mulhi(mullo(a0, -1399), 11991); /* -6161...6180 */
    a0 += s[2 * i + 0]; /* -6161...6435 */
    a0 += (a0 >> 15) & 11991; /* 0...11990 */
    a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
    a1 = mullo(a1, -23321);

    /* invalid inputs might need reduction mod 5483 */
    a1 -= 5483;
    a1 += (a1 >> 15) & 5483;

    R6[12] = a0;
    R6[13] = a1;
    s -= 12;
    for (i = 5; i >= 0; --i) {
        a0 = R7[i];
        a0 = mulhi(a0, 1807) - mulhi(mullo(a0, -1399), 11991); /* -5996...6447 */
        a0 += s[2 * i + 1]; /* -5996...6702 */
        a0 = mulhi(a0, 1807) - mulhi(mullo(a0, -1399), 11991); /* -6161...6180 */
        a0 += s[2 * i + 0]; /* -6161...6435 */
        a0 += (a0 >> 15) & 11991; /* 0...11990 */
        a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
        a1 = mullo(a1, -23321);

        /* invalid inputs might need reduction mod 11991 */
        a1 -= 11991;
        a1 += (a1 >> 15) & 11991;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 26*[1752]+[5483] */

    R5[26] = R6[13];
    s -= 13;
    for (i = 12; i >= 0; --i) {
        a2 = a0 = R6[i];
        a0 = mulhi(a0, 64) - mulhi(mullo(a0, -9576), 1752); /* -876...892 */
        a0 += s[1 * i + 0]; /* -876...1147 */
        a0 += (a0 >> 15) & 1752; /* 0...1751 */
        a1 = (int16) ((a2 << 5) + ((s[i] - a0) >> 3));
        a1 = mullo(a1, -1197);

        /* invalid inputs might need reduction mod 1752 */
        a1 -= 1752;
        a1 += (a1 >> 15) & 1752;

        R5[2 * i] = a0;
        R5[2 * i + 1] = a1;
    }

    /* R5 ------> R4: reconstruct mod 53*[10713]+[131] */

    i = 0;
    s -= 1;
    a2 = a0 = R5[26];
    a0 = mulhi(a0, 658) - mulhi(mullo(a0, -1566), 10713); /* -5357...5521 */
    a0 += s[1 * i + 0]; /* -5357...5776 */
    a0 += (a0 >> 15) & 10713; /* 0...10712 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, -14743);

    /* invalid inputs might need reduction mod 131 */
    a1 -= 131;
    a1 += (a1 >> 15) & 131;

    R4[52] = a0;
    R4[53] = a1;
    s -= 52;
    i = 10;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 658), mulhiconst(mulloconst(A0, -1566), 10713)); /* -5357...5521 */
        A0 = add(A0, S1); /* -5357...5776 */
        A0 = sub(mulhiconst(A0, 658), mulhiconst(mulloconst(A0, -1566), 10713)); /* -5411...5414 */
        A0 = add(A0, S0); /* -5411...5669 */
        A0 = ifnegaddconst(A0, 10713); /* 0...10712 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, -14743);

        /* invalid inputs might need reduction mod 10713 */
        A1 = ifgesubconst(A1, 10713);

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

    /* R4 ------> R3: reconstruct mod 107*[1656]+[5167] */

    i = 0;
    s -= 2;
    a2 = a0 = R4[53];
    a0 = mulhi(a0, 280) - mulhi(mullo(a0, -10131), 1656); /* -828...898 */
    a0 += s[2 * i + 1]; /* -828...1153 */
    a0 = mulhi(a0, 280) - mulhi(mullo(a0, -10131), 1656); /* -832...832 */
    a0 += s[2 * i + 0]; /* -832...1087 */
    a0 += (a0 >> 15) & 1656; /* 0...1655 */
    a1 = (int16) ((a2 << 13) + (s[2 * i + 1] << 5) + ((s[2 * i] - a0) >> 3));
    a1 = mullo(a1, 1583);

    /* invalid inputs might need reduction mod 5167 */
    a1 -= 5167;
    a1 += (a1 >> 15) & 5167;

    R3[106] = a0;
    R3[107] = a1;
    s -= 53;
    i = 37;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 280), mulhiconst(mulloconst(A0, -10131), 1656)); /* -828...898 */
        A0 = add(A0, S0); /* -828...1153 */
        A0 = ifnegaddconst(A0, 1656); /* 0...1655 */
        A1 = add(shiftleftconst(A2, 5), signedshiftrightconst(sub(S0, A0), 3));
        A1 = mulloconst(A1, 1583);

        /* invalid inputs might need reduction mod 1656 */
        A1 = ifgesubconst(A1, 1656);

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

    /* R3 ------> R2: reconstruct mod 214*[651]+[5167] */

    R2[214] = R3[107];
    s -= 107;
    i = 91;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 295), mulhiconst(mulloconst(A0, -25771), 651)); /* -326...399 */
        A0 = add(A0, S0); /* -326...654 */
        A0 = subconst(A0, 651); /* -977...3 */
        A0 = ifnegaddconst(A0, 651); /* -326...650 */
        A0 = ifnegaddconst(A0, 651); /* 0...650 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, -10973);

        /* invalid inputs might need reduction mod 651 */
        A1 = ifgesubconst(A1, 651);

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

    /* R2 ------> R1: reconstruct mod 428*[408]+[5167] */

    R1[428] = R2[214];
    s -= 214;
    i = 198;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -152), mulhiconst(mulloconst(A0, 24415), 408)); /* -242...204 */
        A0 = add(A0, S0); /* -242...459 */
        A0 = subconst(A0, 408); /* -650...51 */
        A0 = ifnegaddconst(A0, 408); /* -242...407 */
        A0 = ifnegaddconst(A0, 408); /* 0...407 */
        A1 = add(shiftleftconst(A2, 5), signedshiftrightconst(sub(S0, A0), 3));
        A1 = mulloconst(A1, -1285);

        /* invalid inputs might need reduction mod 408 */
        A1 = ifgesubconst(A1, 408);

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

    /* R1 ------> R0: reconstruct mod 857*[5167] */

    R0[856] = R1[428] - 2583;
    s -= 856;
    i = 412;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -33), mulhiconst(mulloconst(A0, -3247), 5167)); /* -2592...2583 */
        A0 = add(A0, S1); /* -2592...2838 */
        A0 = sub(mulhiconst(A0, -33), mulhiconst(mulloconst(A0, -3247), 5167)); /* -2585...2584 */
        A0 = add(A0, S0); /* -2585...2839 */
        A0 = ifnegaddconst(A0, 5167); /* 0...5166 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, -19761);

        /* invalid inputs might need reduction mod 5167 */
        A1 = ifgesubconst(A1, 5167);

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
