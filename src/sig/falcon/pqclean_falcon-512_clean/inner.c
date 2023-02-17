#include "inner.h"

/*
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
 */

unsigned set_fpu_cw(unsigned x) {
    return x;
}


uint64_t prng_get_u64(prng *p) {
    size_t u;

    /*
     * If there are less than 9 bytes in the buffer, we refill it.
     * This means that we may drop the last few bytes, but this allows
     * for faster extraction code. Also, it means that we never leave
     * an empty buffer.
     */
    u = p->ptr;
    if (u >= (sizeof p->buf.d) - 9) {
        PQCLEAN_FALCON512_CLEAN_prng_refill(p);
        u = 0;
    }
    p->ptr = u + 8;

    return (uint64_t)p->buf.d[u + 0]
           | ((uint64_t)p->buf.d[u + 1] << 8)
           | ((uint64_t)p->buf.d[u + 2] << 16)
           | ((uint64_t)p->buf.d[u + 3] << 24)
           | ((uint64_t)p->buf.d[u + 4] << 32)
           | ((uint64_t)p->buf.d[u + 5] << 40)
           | ((uint64_t)p->buf.d[u + 6] << 48)
           | ((uint64_t)p->buf.d[u + 7] << 56);
}


unsigned prng_get_u8(prng *p) {
    unsigned v;

    v = p->buf.d[p->ptr ++];
    if (p->ptr == sizeof p->buf.d) {
        PQCLEAN_FALCON512_CLEAN_prng_refill(p);
    }
    return v;
}
