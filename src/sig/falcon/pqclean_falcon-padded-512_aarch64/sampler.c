/*
 * Falcon signature generation.
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
#include <arm_neon.h>

/*
 * Sample an integer value along a half-gaussian distribution centered
 * on zero and standard deviation 1.8205, with a precision of 72 bits.
 */
int
PQCLEAN_FALCONPADDED512_AARCH64_gaussian0_sampler(prng *p) {

    static const uint32_t dist[] = {
        10745844u,  3068844u,  3741698u,
        5559083u,  1580863u,  8248194u,
        2260429u, 13669192u,  2736639u,
        708981u,  4421575u, 10046180u,
        169348u,  7122675u,  4136815u,
        30538u, 13063405u,  7650655u,
        4132u, 14505003u,  7826148u,
        417u, 16768101u, 11363290u,
        31u,  8444042u,  8086568u,
        1u, 12844466u,   265321u,
        0u,  1232676u, 13644283u,
        0u,    38047u,  9111839u,
        0u,      870u,  6138264u,
        0u,       14u, 12545723u,
        0u,        0u,  3104126u,
        0u,        0u,    28824u,
        0u,        0u,      198u,
        0u,        0u,        1u
    };

    uint32_t v0, v1, v2, hi;
    uint64_t lo;
    int z;

    /*
     * Get a random 72-bit value, into three 24-bit limbs v0..v2.
     */
    lo = prng_get_u64(p);
    hi = prng_get_u8(p);
    v0 = (uint32_t)lo & 0xFFFFFF;
    v1 = (uint32_t)(lo >> 24) & 0xFFFFFF;
    v2 = (uint32_t)(lo >> 48) | (hi << 16);

    /*
     * Sampled value is z, such that v0..v2 is lower than the first
     * z elements of the table.
     */

    uint32x4x3_t w;
    uint32x4_t x0, x1, x2, cc0, cc1, cc2, zz;
    uint32x2x3_t wh;
    uint32x2_t cc0h, cc1h, cc2h, zzh;
    x0 = vdupq_n_u32(v0);
    x1 = vdupq_n_u32(v1);
    x2 = vdupq_n_u32(v2);

    // 0: 0, 3, 6, 9
    // 1: 1, 4, 7, 10
    // 2: 2, 5, 8, 11
    // v0 - w0
    // v1 - w1
    // v2 - w2
    // cc1 - cc0 >> 31
    // cc2 - cc1 >> 31
    // z + cc2 >> 31
    w = vld3q_u32(&dist[0]);
    cc0 = vsubq_u32(x0, w.val[2]);
    cc1 = vsubq_u32(x1, w.val[1]);
    cc2 = vsubq_u32(x2, w.val[0]);
    cc1 = (uint32x4_t)vsraq_n_s32((int32x4_t)cc1, (int32x4_t)cc0, 31);
    cc2 = (uint32x4_t)vsraq_n_s32((int32x4_t)cc2, (int32x4_t)cc1, 31);
    zz = vshrq_n_u32(cc2, 31);

    w = vld3q_u32(&dist[12]);
    cc0 = vsubq_u32(x0, w.val[2]);
    cc1 = vsubq_u32(x1, w.val[1]);
    cc2 = vsubq_u32(x2, w.val[0]);
    cc1 = (uint32x4_t)vsraq_n_s32((int32x4_t)cc1, (int32x4_t)cc0, 31);
    cc2 = (uint32x4_t)vsraq_n_s32((int32x4_t)cc2, (int32x4_t)cc1, 31);
    zz = vsraq_n_u32(zz, cc2, 31);

    w = vld3q_u32(&dist[24]);
    cc0 = vsubq_u32(x0, w.val[2]);
    cc1 = vsubq_u32(x1, w.val[1]);
    cc2 = vsubq_u32(x2, w.val[0]);
    cc1 = (uint32x4_t)vsraq_n_s32((int32x4_t)cc1, (int32x4_t)cc0, 31);
    cc2 = (uint32x4_t)vsraq_n_s32((int32x4_t)cc2, (int32x4_t)cc1, 31);
    zz = vsraq_n_u32(zz, cc2, 31);

    w = vld3q_u32(&dist[36]);
    cc0 = vsubq_u32(x0, w.val[2]);
    cc1 = vsubq_u32(x1, w.val[1]);
    cc2 = vsubq_u32(x2, w.val[0]);
    cc1 = (uint32x4_t)vsraq_n_s32((int32x4_t)cc1, (int32x4_t)cc0, 31);
    cc2 = (uint32x4_t)vsraq_n_s32((int32x4_t)cc2, (int32x4_t)cc1, 31);
    zz = vsraq_n_u32(zz, cc2, 31);

    // 0: 48, 51
    // 1: 49, 52
    // 2: 50, 53
    wh = vld3_u32(&dist[48]);
    cc0h = vsub_u32(vget_low_u32(x0), wh.val[2]);
    cc1h = vsub_u32(vget_low_u32(x1), wh.val[1]);
    cc2h = vsub_u32(vget_low_u32(x2), wh.val[0]);
    cc1h = (uint32x2_t)vsra_n_s32((int32x2_t)cc1h, (int32x2_t)cc0h, 31);
    cc2h = (uint32x2_t)vsra_n_s32((int32x2_t)cc2h, (int32x2_t)cc1h, 31);
    zzh = vshr_n_u32(cc2h, 31);

    z = (int) (vaddvq_u32(zz) + vaddv_u32(zzh));
    return z;
}

/*
 * Sample a bit with probability exp(-x) for some x >= 0.
 */
static int
BerExp(prng *p, fpr x, fpr ccs) {
    int s, i;
    fpr r;
    uint32_t sw, w;
    uint64_t z;

    /*
     * Reduce x modulo log(2): x = s*log(2) + r, with s an integer,
     * and 0 <= r < log(2). Since x >= 0, we can use fpr_trunc().
     */
    s = (int)fpr_trunc(fpr_mul(x, fpr_inv_log2));
    r = fpr_sub(x, fpr_mul(fpr_of(s), fpr_log2));

    /*
     * It may happen (quite rarely) that s >= 64; if sigma = 1.2
     * (the minimum value for sigma), r = 0 and b = 1, then we get
     * s >= 64 if the half-Gaussian produced a z >= 13, which happens
     * with probability about 0.000000000230383991, which is
     * approximatively equal to 2^(-32). In any case, if s >= 64,
     * then BerExp will be non-zero with probability less than
     * 2^(-64), so we can simply saturate s at 63.
     */
    sw = (uint32_t)s;
    sw ^= (sw ^ 63) & -((63 - sw) >> 31);
    s = (int)sw;

    /*
     * Compute exp(-r); we know that 0 <= r < log(2) at this point, so
     * we can use fpr_expm_p63(), which yields a result scaled to 2^63.
     * We scale it up to 2^64, then right-shift it by s bits because
     * we really want exp(-x) = 2^(-s)*exp(-r).
     *
     * The "-1" operation makes sure that the value fits on 64 bits
     * (i.e. if r = 0, we may get 2^64, and we prefer 2^64-1 in that
     * case). The bias is negligible since fpr_expm_p63() only computes
     * with 51 bits of precision or so.
     */
    z = ((fpr_expm_p63(r, ccs) << 1) - 1) >> s;

    /*
     * Sample a bit with probability exp(-x). Since x = s*log(2) + r,
     * exp(-x) = 2^-s * exp(-r), we compare lazily exp(-x) with the
     * PRNG output to limit its consumption, the sign of the difference
     * yields the expected result.
     */
    i = 64;
    do {
        i -= 8;
        w = prng_get_u8(p) - ((uint32_t)(z >> i) & 0xFF);
    } while (!w && i > 0);
    return (int)(w >> 31);
}

/*
 * The sampler produces a random integer that follows a discrete Gaussian
 * distribution, centered on mu, and with standard deviation sigma. The
 * provided parameter isigma is equal to 1/sigma.
 *
 * The value of sigma MUST lie between 1 and 2 (i.e. isigma lies between
 * 0.5 and 1); in Falcon, sigma should always be between 1.2 and 1.9.
 */
int
PQCLEAN_FALCONPADDED512_AARCH64_sampler(void *ctx, fpr mu, fpr isigma) {
    sampler_context *spc;
    int s;
    fpr r, dss, ccs;

    spc = ctx;

    /*
     * Center is mu. We compute mu = s + r where s is an integer
     * and 0 <= r < 1.
     */
    s = (int)fpr_floor(mu);
    r = fpr_sub(mu, fpr_of(s));

    /*
     * dss = 1/(2*sigma^2) = 0.5*(isigma^2).
     */
    dss = fpr_half(fpr_sqr(isigma));

    /*
     * ccs = sigma_min / sigma = sigma_min * isigma.
     */
    ccs = fpr_mul(isigma, spc->sigma_min);

    /*
     * We now need to sample on center r.
     */
    for (;;) {
        int z0, z, b;
        fpr x;

        /*
         * Sample z for a Gaussian distribution. Then get a
         * random bit b to turn the sampling into a bimodal
         * distribution: if b = 1, we use z+1, otherwise we
         * use -z. We thus have two situations:
         *
         *  - b = 1: z >= 1 and sampled against a Gaussian
         *    centered on 1.
         *  - b = 0: z <= 0 and sampled against a Gaussian
         *    centered on 0.
         */
        z0 = PQCLEAN_FALCONPADDED512_AARCH64_gaussian0_sampler(&spc->p);
        b = (int)prng_get_u8(&spc->p) & 1;
        z = b + ((b << 1) - 1) * z0;

        /*
         * Rejection sampling. We want a Gaussian centered on r;
         * but we sampled against a Gaussian centered on b (0 or
         * 1). But we know that z is always in the range where
         * our sampling distribution is greater than the Gaussian
         * distribution, so rejection works.
         *
         * We got z with distribution:
         *    G(z) = exp(-((z-b)^2)/(2*sigma0^2))
         * We target distribution:
         *    S(z) = exp(-((z-r)^2)/(2*sigma^2))
         * Rejection sampling works by keeping the value z with
         * probability S(z)/G(z), and starting again otherwise.
         * This requires S(z) <= G(z), which is the case here.
         * Thus, we simply need to keep our z with probability:
         *    P = exp(-x)
         * where:
         *    x = ((z-r)^2)/(2*sigma^2) - ((z-b)^2)/(2*sigma0^2)
         *
         * Here, we scale up the Bernouilli distribution, which
         * makes rejection more probable, but makes rejection
         * rate sufficiently decorrelated from the Gaussian
         * center and standard deviation that the whole sampler
         * can be said to be constant-time.
         */
        x = fpr_mul(fpr_sqr(fpr_sub(fpr_of(z), r)), dss);
        x = fpr_sub(x, fpr_mul(fpr_of(z0 * z0), fpr_inv_2sqrsigma0));
        if (BerExp(&spc->p, x, ccs)) {
            /*
             * Rejection sampling was centered on r, but the
             * actual center is mu = s + r.
             */
            return s + z;
        }
    }
}
