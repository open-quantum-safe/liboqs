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




static uint64_t load40_littleendian(const uint8_t x[5]) {
    uint64_t r;
    r  = (uint64_t) x[0];
    r |= (uint64_t) x[1] << 8;
    r |= (uint64_t) x[2] << 16;
    r |= (uint64_t) x[3] << 24;
    r |= (uint64_t) x[4] << 32;
    return r;
}

static
void cbd5(uint16_t s[SABER_N], const uint8_t buf[SABER_POLYCOINBYTES]) {
    uint64_t t, d, a[4], b[4];
    int i, j;

    for (i = 0; i < SABER_N / 4; i++) {
        t = load40_littleendian(buf);
        d = 0;
        for (j = 0; j < 5; j++) {
            d += (t >> j) & 0x0842108421UL;
        }

        a[0] = d & 0x1f;
        b[0] = (d >> 5) & 0x1f;
        a[1] = (d >> 10) & 0x1f;
        b[1] = (d >> 15) & 0x1f;
        a[2] = (d >> 20) & 0x1f;
        b[2] = (d >> 25) & 0x1f;
        a[3] = (d >> 30) & 0x1f;
        b[3] = (d >> 35);

        s[4 * i + 0] = (uint16_t)(a[0] - b[0]);
        s[4 * i + 1] = (uint16_t)(a[1] - b[1]);
        s[4 * i + 2] = (uint16_t)(a[2] - b[2]);
        s[4 * i + 3] = (uint16_t)(a[3] - b[3]);

        buf += 5;
    }
}

void cbd(uint16_t s[SABER_N], const uint8_t buf[SABER_POLYCOINBYTES]) {
    cbd5(s, buf);
}
