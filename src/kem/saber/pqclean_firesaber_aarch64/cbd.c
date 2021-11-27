/*=============================================================================
This file has been adapted from the implementation
(available at, Public Domain https://github.com/KULeuven-COSIC/SABER)
of "Saber: Module-LWR based key exchange, CPA-secure encryption and CCA-secure KEM"
by : Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, and Frederik Vercauteren
Jose Maria Bermudo Mera, Michiel Van Beirendonck, Andrea Basso.
 * Copyright (c) 2020 by Cryptographic Engineering Research Group (CERG)
 * ECE Department, George Mason University
 * Fairfax, VA, U.S.A.
 * Author: Duc Tri Nguyen
=============================================================================*/

#include "cbd.h"
#include <arm_neon.h>

#define vload4(c, ptr) c = vld4q_u8(ptr);
#define vstore4(ptr, c) vst4q_u16(ptr, c);

// c = a & b
#define vand8(c, a, b) c = vandq_u8(a, b);

// c = a >> n
#define vsr8(c, a, n) c = vshrq_n_u8(a, n);

// c = a + b
#define vadd8(c, a, b) c = vaddq_u8(a, b);

// low c = (uint16_t) (a - b)
#define vsubll8(c, a, b) c = vsubl_u8(a, b);

// high c = (uint16_t) (a - b)
#define vsublh8(c, a, b) c = vsubl_high_u8(a, b);

static uint32_t load24_littleendian(const uint8_t x[3]) {
    uint32_t r;
    r  = (uint32_t)x[0];
    r |= (uint32_t)x[1] << 8;
    r |= (uint32_t)x[2] << 16;
    return r;
}

static void cbd3(uint16_t s[SABER_N], const uint8_t buf[SABER_POLYCOINBYTES]) {
    unsigned int i, j;
    uint32_t t, d;
    int16_t a, b;

    for (i = 0; i < SABER_N / 4; i++) {
        t  = load24_littleendian(buf + 3 * i);
        d  = t & 0x00249249;
        d += (t >> 1) & 0x00249249;
        d += (t >> 2) & 0x00249249;

        for (j = 0; j < 4; j++) {
            a = (d >> (6 * j + 0)) & 0x7;
            b = (d >> (6 * j + 3)) & 0x7;
            s[4 * i + j] = a - b;
        }
    }
}




void cbd(uint16_t s[SABER_N], const uint8_t buf[SABER_POLYCOINBYTES]) {
    cbd3(s, buf);
}
