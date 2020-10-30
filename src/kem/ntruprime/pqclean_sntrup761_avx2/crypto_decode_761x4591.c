#include "crypto_decode_761x4591.h"
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

void PQCLEAN_SNTRUP761_AVX2_crypto_decode_761x4591(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[381], R2[191], R3[96], R4[48], R5[24], R6[12], R7[6], R8[3], R9[2], R10[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_SNTRUP761_AVX2_crypto_decode_761x4591_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 = mulhi(a1, -656) - mulhi(mullo(a1, -10434), 1608);
    a1 += *--s; /* -804...1056 */
    a1 += (a1 >> 15) & 1608; /* 0...1607 */
    R10[0] = a1;

    /* R10 ------> R9: reconstruct mod 1*[9470]+[11127] */

    i = 0;
    s -= 2;
    a2 = a0 = R10[0];
    a0 = mulhi(a0, -3624) - mulhi(mullo(a0, -1772), 9470); /* -5641...4735 */
    a0 += s[2 * i + 1]; /* -5641...4990 */
    a0 = mulhi(a0, -3624) - mulhi(mullo(a0, -1772), 9470); /* -5011...5046 */
    a0 += s[2 * i + 0]; /* -5011...5301 */
    a0 += (a0 >> 15) & 9470; /* 0...9469 */
    a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
    a1 = mullo(a1, -21121);

    /* invalid inputs might need reduction mod 11127 */
    a1 -= 11127;
    a1 += (a1 >> 15) & 11127;

    R9[0] = a0;
    R9[1] = a1;
    s -= 0;

    /* R9 ------> R8: reconstruct mod 2*[1557]+[11127] */

    R8[2] = R9[1];
    s -= 1;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, 541) - mulhi(mullo(a0, -10775), 1557); /* -779...913 */
        a0 += s[1 * i + 0]; /* -779...1168 */
        a0 += (a0 >> 15) & 1557; /* 0...1556 */
        a1 = (int16) ((a2 << 8) + s[i] - a0);
        a1 = mullo(a1, -26307);

        /* invalid inputs might need reduction mod 1557 */
        a1 -= 1557;
        a1 += (a1 >> 15) & 1557;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 5*[10101]+[282] */

    i = 0;
    s -= 1;
    a2 = a0 = R8[2];
    a0 = mulhi(a0, -545) - mulhi(mullo(a0, -1661), 10101); /* -5187...5050 */
    a0 += s[1 * i + 0]; /* -5187...5305 */
    a0 += (a0 >> 15) & 10101; /* 0...10100 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 12509);

    /* invalid inputs might need reduction mod 282 */
    a1 -= 282;
    a1 += (a1 >> 15) & 282;

    R7[4] = a0;
    R7[5] = a1;
    s -= 4;
    for (i = 1; i >= 0; --i) {
        a0 = R8[i];
        a0 = mulhi(a0, -545) - mulhi(mullo(a0, -1661), 10101); /* -5187...5050 */
        a0 += s[2 * i + 1]; /* -5187...5305 */
        a0 = mulhi(a0, -545) - mulhi(mullo(a0, -1661), 10101); /* -5095...5093 */
        a0 += s[2 * i + 0]; /* -5095...5348 */
        a0 += (a0 >> 15) & 10101; /* 0...10100 */
        a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
        a1 = mullo(a1, 12509);

        /* invalid inputs might need reduction mod 10101 */
        a1 -= 10101;
        a1 += (a1 >> 15) & 10101;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 11*[1608]+[11468] */

    i = 0;
    s -= 2;
    a2 = a0 = R7[5];
    a0 = mulhi(a0, -656) - mulhi(mullo(a0, -10434), 1608); /* -968...804 */
    a0 += s[2 * i + 1]; /* -968...1059 */
    a0 = mulhi(a0, -656) - mulhi(mullo(a0, -10434), 1608); /* -815...813 */
    a0 += s[2 * i + 0]; /* -815...1068 */
    a0 += (a0 >> 15) & 1608; /* 0...1607 */
    a1 = (int16) ((a2 << 13) + (s[2 * i + 1] << 5) + ((s[2 * i] - a0) >> 3));
    a1 = mullo(a1, 6521);

    /* invalid inputs might need reduction mod 11468 */
    a1 -= 11468;
    a1 += (a1 >> 15) & 11468;

    R6[10] = a0;
    R6[11] = a1;
    s -= 5;
    for (i = 4; i >= 0; --i) {
        a2 = a0 = R7[i];
        a0 = mulhi(a0, -656) - mulhi(mullo(a0, -10434), 1608); /* -968...804 */
        a0 += s[1 * i + 0]; /* -968...1059 */
        a0 += (a0 >> 15) & 1608; /* 0...1607 */
        a1 = (int16) ((a2 << 5) + ((s[i] - a0) >> 3));
        a1 = mullo(a1, 6521);

        /* invalid inputs might need reduction mod 1608 */
        a1 -= 1608;
        a1 += (a1 >> 15) & 1608;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 23*[10265]+[286] */

    i = 0;
    s -= 1;
    a2 = a0 = R6[11];
    a0 = mulhi(a0, 4206) - mulhi(mullo(a0, -1634), 10265); /* -5133...6184 */
    a0 += s[1 * i + 0]; /* -5133...6439 */
    a0 += (a0 >> 15) & 10265; /* 0...10264 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, -19415);

    /* invalid inputs might need reduction mod 286 */
    a1 -= 286;
    a1 += (a1 >> 15) & 286;

    R5[22] = a0;
    R5[23] = a1;
    s -= 22;
    for (i = 10; i >= 0; --i) {
        a0 = R6[i];
        a0 = mulhi(a0, 4206) - mulhi(mullo(a0, -1634), 10265); /* -5133...6184 */
        a0 += s[2 * i + 1]; /* -5133...6439 */
        a0 = mulhi(a0, 4206) - mulhi(mullo(a0, -1634), 10265); /* -5462...5545 */
        a0 += s[2 * i + 0]; /* -5462...5800 */
        a0 += (a0 >> 15) & 10265; /* 0...10264 */
        a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
        a1 = mullo(a1, -19415);

        /* invalid inputs might need reduction mod 10265 */
        a1 -= 10265;
        a1 += (a1 >> 15) & 10265;

        R5[2 * i] = a0;
        R5[2 * i + 1] = a1;
    }

    /* R5 ------> R4: reconstruct mod 47*[1621]+[11550] */

    i = 0;
    s -= 2;
    a0 = R5[23];
    a0 = mulhi(a0, -134) - mulhi(mullo(a0, -10350), 1621); /* -844...810 */
    a0 += s[2 * i + 1]; /* -844...1065 */
    a0 = mulhi(a0, -134) - mulhi(mullo(a0, -10350), 1621); /* -813...812 */
    a0 += s[2 * i + 0]; /* -813...1067 */
    a0 += (a0 >> 15) & 1621; /* 0...1620 */
    a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
    a1 = mullo(a1, -14595);

    /* invalid inputs might need reduction mod 11550 */
    a1 -= 11550;
    a1 += (a1 >> 15) & 11550;

    R4[46] = a0;
    R4[47] = a1;
    s -= 23;
    i = 7;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -134), mulhiconst(mulloconst(A0, -10350), 1621)); /* -844...810 */
        A0 = add(A0, S0); /* -844...1065 */
        A0 = ifnegaddconst(A0, 1621); /* 0...1620 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, -14595);

        /* invalid inputs might need reduction mod 1621 */
        A1 = ifgesubconst(A1, 1621);

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

    /* R4 ------> R3: reconstruct mod 95*[644]+[4591] */

    i = 0;
    s -= 1;
    a2 = a0 = R4[47];
    a0 = mulhi(a0, -272) - mulhi(mullo(a0, -26052), 644); /* -390...322 */
    a0 += s[1 * i + 0]; /* -390...577 */
    a0 += (a0 >> 15) & 644; /* 0...643 */
    a1 = (int16) ((a2 << 6) + ((s[i] - a0) >> 2));
    a1 = mullo(a1, -7327);

    /* invalid inputs might need reduction mod 4591 */
    a1 -= 4591;
    a1 += (a1 >> 15) & 4591;

    R3[94] = a0;
    R3[95] = a1;
    s -= 47;
    i = 31;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -272), mulhiconst(mulloconst(A0, -26052), 644)); /* -390...322 */
        A0 = add(A0, S0); /* -390...577 */
        A0 = ifnegaddconst(A0, 644); /* 0...643 */
        A1 = add(shiftleftconst(A2, 6), signedshiftrightconst(sub(S0, A0), 2));
        A1 = mulloconst(A1, -7327);

        /* invalid inputs might need reduction mod 644 */
        A1 = ifgesubconst(A1, 644);

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

    /* R3 ------> R2: reconstruct mod 190*[406]+[4591] */

    R2[190] = R3[95];
    s -= 95;
    i = 79;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 78), mulhiconst(mulloconst(A0, 24213), 406)); /* -203...222 */
        A0 = add(A0, S0); /* -203...477 */
        A0 = subconst(A0, 406); /* -609...71 */
        A0 = ifnegaddconst(A0, 406); /* -203...405 */
        A0 = ifnegaddconst(A0, 406); /* 0...405 */
        A1 = add(shiftleftconst(A2, 7), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, 25827);

        /* invalid inputs might need reduction mod 406 */
        A1 = ifgesubconst(A1, 406);

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

    /* R2 ------> R1: reconstruct mod 380*[322]+[4591] */

    R1[380] = R2[190];
    s -= 190;
    i = 174;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 50), mulhiconst(mulloconst(A0, 13433), 322)); /* -161...173 */
        A0 = add(A0, S0); /* -161...428 */
        A0 = subconst(A0, 322); /* -483...106 */
        A0 = ifnegaddconst(A0, 322); /* -161...321 */
        A0 = ifnegaddconst(A0, 322); /* 0...321 */
        A1 = add(shiftleftconst(A2, 7), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, -7327);

        /* invalid inputs might need reduction mod 322 */
        A1 = ifgesubconst(A1, 322);

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

    /* R1 ------> R0: reconstruct mod 761*[4591] */

    R0[760] = R1[380] - 2295;
    s -= 760;
    i = 364;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 1702), mulhiconst(mulloconst(A0, -3654), 4591)); /* -2296...2721 */
        A0 = add(A0, S1); /* -2296...2976 */
        A0 = sub(mulhiconst(A0, 1702), mulhiconst(mulloconst(A0, -3654), 4591)); /* -2356...2372 */
        A0 = add(A0, S0); /* -2356...2627 */
        A0 = ifnegaddconst(A0, 4591); /* 0...4590 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, 15631);

        /* invalid inputs might need reduction mod 4591 */
        A1 = ifgesubconst(A1, 4591);

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
