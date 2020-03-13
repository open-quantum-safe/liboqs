/*
 * Support functions for signatures (hash-to-point, norm).
 *
 * ==========================(LICENSE BEGIN)============================
 *
 * Copyright (c) 2017-2019  Falcon Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ===========================(LICENSE END)=============================
 *
 * @author   Thomas Pornin <thomas.pornin@nccgroup.com>
 */

#include "inner.h"

/* see inner.h */
void
PQCLEAN_FALCON512_CLEAN_hash_to_point_vartime(
    inner_shake256_context *sc,
    uint16_t *x, unsigned logn) {
    /*
     * This is the straightforward per-the-spec implementation. It
     * is not constant-time, thus it might reveal information on the
     * plaintext (at least, enough to check the plaintext against a
     * list of potential plaintexts) in a scenario where the
     * attacker does not have access to the signature value or to
     * the public key, but knows the nonce (without knowledge of the
     * nonce, the hashed output cannot be matched against potential
     * plaintexts).
     */
    size_t n;

    n = (size_t)1 << logn;
    while (n > 0) {
        uint8_t buf[2];
        uint32_t w;

        inner_shake256_extract(sc, (void *)buf, sizeof buf);
        w = ((unsigned)buf[0] << 8) | (unsigned)buf[1];
        if (w < 61445) {
            while (w >= 12289) {
                w -= 12289;
            }
            *x ++ = (uint16_t)w;
            n --;
        }
    }
}

/* see inner.h */
void
PQCLEAN_FALCON512_CLEAN_hash_to_point_ct(
    inner_shake256_context *sc,
    uint16_t *x, unsigned logn, uint8_t *tmp) {
    /*
     * Each 16-bit sample is a value in 0..65535. The value is
     * kept if it falls in 0..61444 (because 61445 = 5*12289)
     * and rejected otherwise; thus, each sample has probability
     * about 0.93758 of being selected.
     *
     * We want to oversample enough to be sure that we will
     * have enough values with probability at least 1 - 2^(-256).
     * Depending on degree N, this leads to the following
     * required oversampling:
     *
     *   logn     n  oversampling
     *     1      2     65
     *     2      4     67
     *     3      8     71
     *     4     16     77
     *     5     32     86
     *     6     64    100
     *     7    128    122
     *     8    256    154
     *     9    512    205
     *    10   1024    287
     *
     * If logn >= 7, then the provided temporary buffer is large
     * enough. Otherwise, we use a stack buffer of 63 entries
     * (i.e. 126 bytes) for the values that do not fit in tmp[].
     */

    static const uint16_t overtab[] = {
        0, /* unused */
        65,
        67,
        71,
        77,
        86,
        100,
        122,
        154,
        205,
        287
    };

    unsigned n, n2, u, m, p, over;
    uint16_t *tt1, tt2[63];

    /*
     * We first generate m 16-bit value. Values 0..n-1 go to x[].
     * Values n..2*n-1 go to tt1[]. Values 2*n and later go to tt2[].
     * We also reduce modulo q the values; rejected values are set
     * to 0xFFFF.
     */
    n = 1U << logn;
    n2 = n << 1;
    over = overtab[logn];
    m = n + over;
    tt1 = (uint16_t *)tmp;
    for (u = 0; u < m; u ++) {
        uint8_t buf[2];
        uint32_t w, wr;

        inner_shake256_extract(sc, buf, sizeof buf);
        w = ((uint32_t)buf[0] << 8) | (uint32_t)buf[1];
        wr = w - ((uint32_t)24578 & (((w - 24578) >> 31) - 1));
        wr = wr - ((uint32_t)24578 & (((wr - 24578) >> 31) - 1));
        wr = wr - ((uint32_t)12289 & (((wr - 12289) >> 31) - 1));
        wr |= ((w - 61445) >> 31) - 1;
        if (u < n) {
            x[u] = (uint16_t)wr;
        } else if (u < n2) {
            tt1[u - n] = (uint16_t)wr;
        } else {
            tt2[u - n2] = (uint16_t)wr;
        }
    }

    /*
     * Now we must "squeeze out" the invalid values. We do this in
     * a logarithmic sequence of passes; each pass computes where a
     * value should go, and moves it down by 'p' slots if necessary,
     * where 'p' uses an increasing powers-of-two scale. It can be
     * shown that in all cases where the loop decides that a value
     * has to be moved down by p slots, the destination slot is
     * "free" (i.e. contains an invalid value).
     */
    for (p = 1; p <= over; p <<= 1) {
        unsigned v;

        /*
         * In the loop below:
         *
         *   - v contains the index of the final destination of
         *     the value; it is recomputed dynamically based on
         *     whether values are valid or not.
         *
         *   - u is the index of the value we consider ("source");
         *     its address is s.
         *
         *   - The loop may swap the value with the one at index
         *     u-p. The address of the swap destination is d.
         */
        v = 0;
        for (u = 0; u < m; u ++) {
            uint16_t *s, *d;
            unsigned j, sv, dv, mk;

            if (u < n) {
                s = &x[u];
            } else if (u < n2) {
                s = &tt1[u - n];
            } else {
                s = &tt2[u - n2];
            }
            sv = *s;

            /*
             * The value in sv should ultimately go to
             * address v, i.e. jump back by u-v slots.
             */
            j = u - v;

            /*
             * We increment v for the next iteration, but
             * only if the source value is valid. The mask
             * 'mk' is -1 if the value is valid, 0 otherwise,
             * so we _subtract_ mk.
             */
            mk = (sv >> 15) - 1U;
            v -= mk;

            /*
             * In this loop we consider jumps by p slots; if
             * u < p then there is nothing more to do.
             */
            if (u < p) {
                continue;
            }

            /*
             * Destination for the swap: value at address u-p.
             */
            if ((u - p) < n) {
                d = &x[u - p];
            } else if ((u - p) < n2) {
                d = &tt1[(u - p) - n];
            } else {
                d = &tt2[(u - p) - n2];
            }
            dv = *d;

            /*
             * The swap should be performed only if the source
             * is valid AND the jump j has its 'p' bit set.
             */
            mk &= -(((j & p) + 0x1FF) >> 9);

            *s = (uint16_t)(sv ^ (mk & (sv ^ dv)));
            *d = (uint16_t)(dv ^ (mk & (sv ^ dv)));
        }
    }
}

/* see inner.h */
int
PQCLEAN_FALCON512_CLEAN_is_short(
    const int16_t *s1, const int16_t *s2, unsigned logn) {
    /*
     * We use the l2-norm. Code below uses only 32-bit operations to
     * compute the square of the norm with saturation to 2^32-1 if
     * the value exceeds 2^31-1.
     */
    size_t n, u;
    uint32_t s, ng;

    n = (size_t)1 << logn;
    s = 0;
    ng = 0;
    for (u = 0; u < n; u ++) {
        int32_t z;

        z = s1[u];
        s += (uint32_t)(z * z);
        ng |= s;
        z = s2[u];
        s += (uint32_t)(z * z);
        ng |= s;
    }
    s |= -(ng >> 31);

    /*
     * Acceptance bound on the l2-norm is:
     *   1.2*1.55*sqrt(q)*sqrt(2*N)
     * Value 7085 is floor((1.2^2)*(1.55^2)*2*1024).
     */
    return s < (((uint32_t)7085 * (uint32_t)12289) >> (10 - logn));
}

/* see inner.h */
int
PQCLEAN_FALCON512_CLEAN_is_short_half(
    uint32_t sqn, const int16_t *s2, unsigned logn) {
    size_t n, u;
    uint32_t ng;

    n = (size_t)1 << logn;
    ng = -(sqn >> 31);
    for (u = 0; u < n; u ++) {
        int32_t z;

        z = s2[u];
        sqn += (uint32_t)(z * z);
        ng |= sqn;
    }
    sqn |= -(ng >> 31);

    /*
     * Acceptance bound on the l2-norm is:
     *   1.2*1.55*sqrt(q)*sqrt(2*N)
     * Value 7085 is floor((1.2^2)*(1.55^2)*2*1024).
     */
    return sqn < (((uint32_t)7085 * (uint32_t)12289) >> (10 - logn));
}
