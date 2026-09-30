#include <modqx.h>
#include <assert.h>

// propagate carries
static inline spint
prop(spint n[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    spint mask = ((spint)1 << MODQRADIX) - (spint)1;
    sspint carry = (sspint)n[0];
    carry >>= MODQRADIX;
    if (numwords == 1) {
        return -((n[0] >> 1) >> (RADIX - 2));
    }
    n[0] &= mask;
    for (i = 1; i < numwords - 1; i++) {
        carry += (sspint)n[i];
        n[i] = (spint)carry & mask;
        carry >>= MODQRADIX;
    }
    n[numwords - 1] += (spint)carry;
    return -((n[numwords - 1] >> 1) >> (RADIX - 2));
}

// propagate carries and add p if negative, propagate carries again
static spint
flatten(spint n[MODQLIMBS], const spint p[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    spint carry = prop(n, numwords);
    for (i = 0; i < numwords; i++) {
        n[i] += p[i] & carry;
    }
    (void)prop(n, numwords);
    return (carry & 1);
}

// Montgomery final subtract
static spint
modfsb(spint n[MODQLIMBS], const spint p[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    for (i = 0; i < numwords; i++) {
        n[i] -= p[i];
    }
    return flatten(n, p, numwords);
}

// Modular addition - reduce less than 2p
void
modqx_modadd(const spint a[MODQLIMBS],
             const spint b[MODQLIMBS],
             spint n[MODQLIMBS],
             const spint two_p[MODQLIMBS],
             int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    spint carry;
    for (i = 0; i < numwords; i++) {
        n[i] = a[i] + b[i];
        n[i] -= two_p[i];
    }
    carry = prop(n, numwords);
    for (i = 0; i < numwords; i++) {
        n[i] += two_p[i] & carry;
    }
    (void)prop(n, numwords);
}

// Modular subtraction - reduce less than 2p
void
modqx_modsub(const spint a[MODQLIMBS],
             const spint b[MODQLIMBS],
             spint n[MODQLIMBS],
             const spint two_p[MODQLIMBS],
             int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    spint carry;
    for (i = 0; i < numwords; i++) {
        n[i] = a[i] - b[i];
    }
    carry = prop(n, numwords);
    for (i = 0; i < numwords; i++) {
        n[i] += two_p[i] & carry;
    }
    (void)prop(n, numwords);
}

// Modular multiplication, c=a*b mod 2p
void
modqx_modmul(const spint a[MODQLIMBS],
             const spint b[MODQLIMBS],
             spint c[MODQLIMBS],
             const spint p[MODQLIMBS],
             const spint ndash,
             int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i, j;
    dpint t = 0;
    spint mask = ((spint)1 << MODQRADIX) - (spint)1;
    spint v[MODQLIMBS];

    for (i = 0; i < numwords; i++) {
        for (j = 0; j < i; j++) {
            t += (dpint)a[j] * (dpint)b[i - j] + (dpint)v[j] * (dpint)p[i - j];
        }
        t += (dpint)a[i] * (dpint)b[0];
        v[i] = ((spint)t * ndash) & mask;
        t += (dpint)v[i] * (dpint)p[0];
        t >>= MODQRADIX;
    }
    for (i = numwords; i < 2 * numwords - 1; i++) {
        int jlo = i - numwords + 1;
        for (j = jlo; j < numwords; j++) {
            t += (dpint)a[j] * (dpint)b[i - j] + (dpint)v[j] * (dpint)p[i - j];
        }
        c[i - numwords] = (spint)t & mask;
        t >>= MODQRADIX;
    }
    c[numwords - 1] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
void
modqx_modsqr(const spint a[MODQLIMBS], spint c[MODQLIMBS], const spint p[MODQLIMBS], const spint ndash, int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i, j, mid;
    dpint t = 0, tot;
    spint mask = ((spint)1 << MODQRADIX) - (spint)1;
    spint v[MODQLIMBS];

    for (i = 0; i < numwords; i++) {
        tot = 0;
        for (j = 0; 2 * j < i; j++) {
            tot += (dpint)a[j] * (dpint)a[i - j];
        }
        tot *= 2;
        if ((i & 1) == 0) {
            mid = i / 2;
            assert(mid >= 0 && mid <= i);
            tot += (dpint)a[mid] * (dpint)a[mid];
        }
        t += tot;

        for (j = 0; j < i; j++) {
            t += (dpint)v[j] * (dpint)p[i - j];
        }
        v[i] = ((spint)t * ndash) & mask;
        t += (dpint)v[i] * (dpint)p[0];
        t >>= MODQRADIX;
    }

    for (i = numwords; i < 2 * numwords - 1; i++) {
        int jlo = i - numwords + 1;
        tot = 0;
        for (j = jlo; 2 * j < i; j++) {
            tot += (dpint)a[j] * (dpint)a[i - j];
        }
        tot *= 2;
        if ((i & 1) == 0) {
            mid = i / 2;
            assert(mid >= jlo && mid <= numwords - 1);
            tot += (dpint)a[mid] * (dpint)a[mid];
        }
        t += tot;

        for (j = jlo; j < numwords; j++) {
            t += (dpint)v[j] * (dpint)p[i - j];
        }
        c[i - numwords] = (spint)t & mask;
        t >>= MODQRADIX;
    }
    c[numwords - 1] = (spint)t;
}

// copy
void
modqx_modcpy(const spint a[MODQLIMBS], spint c[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    for (i = 0; i < numwords; i++) {
        c[i] = a[i];
    }
}

// Convert m to n-residue form, n=nres(m) knowing only 2p
void
modqx_nresx(const spint m[MODQLIMBS], spint n[MODQLIMBS], const spint two_p[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    modqx_modadd(m, m, n, two_p, numwords);
    for (int i = 0; i < MODQRADIX * numwords - 1; i++) {
        modqx_modadd(n, n, n, two_p, numwords);
    }
}

// Convert n back to normal form, m=modqx_redc(n)
void
modqx_redc(const spint n[MODQLIMBS], spint m[MODQLIMBS], const spint p[MODQLIMBS], const spint ndash, int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    spint c[MODQLIMBS];
    c[0] = 1;
    for (i = 1; i < MODQLIMBS; i++) {
        c[i] = 0;
    }
    modqx_modmul(n, c, m, p, ndash, numwords);
    (void)modfsb(m, p, numwords);
}

// is zero?
int
modqx_modis0(const spint a[MODQLIMBS], const spint p[MODQLIMBS], const spint ndash, int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    spint c[MODQLIMBS];
    spint d = 0;
    modqx_redc(a, c, p, ndash, numwords);
    for (i = 0; i < numwords; i++) {
        d |= c[i];
    }
    return ((spint)1 & ((d - (spint)1) >> MODQRADIX));
}

// set to zero
void
modqx_modzer(spint a[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    for (i = 0; i < numwords; i++) {
        a[i] = 0;
    }
}

/*
Internal function. Computes a = R mod two_p,
where: B = 2^MODQRADIX, R = B^numwords,
and active is the number of significant radix-B limbs of two_p.

It performs:

  Step 0. Clears the output array a.
  Step 1. Left-shift two_p so that the MSB bit of its MS word is 1.
  Step 2. Constructs the normalized dividend: R = B^numwords = [0, ..., 0, 1].
  Step 3. Performs the direct remainder computation with Knuth Algorithm D.
  Step 4. Removes the normalization shift and stores the exact remainder
          in a [0, two_p) representation.
*/
static void
modone_div(spint a[MODQLIMBS], const spint two_p[MODQLIMBS], int numwords, int active)
{
    assert(numwords > 0);
    assert(numwords <= MODQLIMBS);
    assert(active >= 2);
    assert(active <= numwords);
    assert(active == numwords || active == numwords - 1);

    const spint mask = MODQLIMBMASK;
    const ddigit_t base = (ddigit_t)1 << MODQRADIX;

    /*
     * Step 0. Clear the output.
     */
    for (int i = 0; i < numwords; i++) {
        a[i] = 0;
    }

    spint v[MODQLIMBS + 1] = { 0 };
    spint u[MODQLIMBS + 2] = { 0 };

    spint top = two_p[active - 1] & mask;
    assert(top != 0);

    /*
     * Step 1. Left-shift two_p so that the MSB bit of its MS word is 1.
     */
    unsigned shift =
        (unsigned)sqisign_clz64((unsigned long long)top) - (unsigned)(8 * sizeof(unsigned long long) - MODQRADIX);

    assert(shift < MODQRADIX);

    spint carry = 0;

    if (shift == 0) {
        for (int i = 0; i < active; i++) {
            v[i] = two_p[i] & mask;
        }
    } else {
        for (int i = 0; i < active; i++) {
            ddigit_t z = ((ddigit_t)(two_p[i] & mask) << shift) | (ddigit_t)carry;

            v[i] = (spint)z & mask;
            carry = (spint)(z >> MODQRADIX);
        }

        assert(carry == 0);
    }

    /*
     * Step 2. Constructs the normalized dividend: R = B^numwords = [0, ..., 0, 1].
     */
    u[numwords] = (spint)1 << shift;

    /*
     * Step 3. Performs the direct remainder computation with Knuth Algorithm D.
     * We consider two cases:
     * active = numwords -> 2 quotient radix words.
     * active = numwords - 1 -> 3 quotient radix words.
     */
    int jmax = (numwords + 1) - active;

    for (int j = jmax; j >= 0; j--) {
        ddigit_t numerator = ((ddigit_t)u[j + active] << MODQRADIX) | (ddigit_t)u[j + active - 1];

        ddigit_t vtop = (ddigit_t)v[active - 1];

        /*
         * One word division for the trial quotient radix word.
         */
        ddigit_t qhat = numerator / vtop;
        // Substituting the % modulus
        ddigit_t rhat = numerator - qhat * vtop;

        ddigit_t cap = (ddigit_t)(qhat >= base);

        qhat -= cap;
        rhat += cap * vtop;

        for (int correction = 0; correction < 2; correction++) {
            ddigit_t lhs = qhat * (ddigit_t)v[active - 2];
            ddigit_t rhs = (rhat << MODQRADIX) + (ddigit_t)u[j + active - 2];

            ddigit_t decrement = (ddigit_t)(rhat < base) & (ddigit_t)(lhs > rhs);

            qhat -= decrement;
            rhat += decrement * vtop;
        }

        ddigit_t borrow = 0;

        for (int i = 0; i < active; i++) {
            ddigit_t product = qhat * (ddigit_t)v[i] + borrow;

            spint lo = (spint)product & mask;
            spint hi = (spint)(product >> MODQRADIX);

            spint ui = u[j + i] & mask;
            spint under = (spint)(ui < lo);

            u[j + i] = (ui - lo) & mask;
            borrow = (ddigit_t)hi + (ddigit_t)under;
        }

        spint top_u = u[j + active] & mask;
        spint borrow_digit = (spint)borrow;
        spint negative = (spint)(top_u < borrow_digit);

        u[j + active] = (top_u - borrow_digit) & mask;

        spint add_mask = (spint)0 - negative;
        ddigit_t addcarry = 0;

        for (int i = 0; i < active; i++) {
            ddigit_t z = (ddigit_t)(u[j + i] & mask) + (ddigit_t)(v[i] & add_mask) + addcarry;

            u[j + i] = (spint)z & mask;
            addcarry = z >> MODQRADIX;
        }

        u[j + active] = (u[j + active] + (spint)addcarry) & mask;
    }

    /*
     * Step 4. Removes the normalization shift and stores the exact remainder
     * in a [0, two_p) representation.
     */
    if (shift == 0) {
        for (int i = 0; i < active; i++) {
            a[i] = u[i] & mask;
        }
    } else {
        for (int i = 0; i < active; i++) {
            spint lo = (u[i] & mask) >> shift;
            spint hi = 0;

            if (i + 1 < active) {
                hi = ((u[i + 1] & mask) << (MODQRADIX - shift)) & mask;
            }

            a[i] = (lo | hi) & mask;
        }
    }
}

/*
Sets a to the Montgomery representation of 1: a = R mod two_p,
where: R = 2^(numwords * MODQRADIX). This is a Public function. It performs:
Step 0. If two_p has numwords  radix-B words, use modone_div().
Step 1. If two_p has numwords - 1  radix-B words, use modone_div().
Step 2. Otherwise, use the original repeated-doubling implementation.
*/
void
modqx_modone(spint a[MODQLIMBS], const spint two_p[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);

    /*
     * Step 0. If two_p has numwords  radix-B words, use modone_div().
     */
    if (numwords > 1 && (two_p[numwords - 1] & MODQLIMBMASK) != 0) {
        modone_div(a, two_p, numwords, numwords);
        return;
    }

    /*
     * Step 1. If two_p has numwords - 1 significant radix-B words, use modone_div().
     */
    if (numwords > 2 && (two_p[numwords - 2] & MODQLIMBMASK) != 0) {
        modone_div(a, two_p, numwords, numwords - 1);
        return;
    }

    /*
     * Step 2. Otherwise, use the original repeated-doubling implementation.
     */
    a[0] = 1u;

    for (int i = 1; i < numwords; i++) {
        a[i] = 0;
    }

    for (int i = 0; i < numwords * MODQRADIX; i++) {
        modqx_modadd(a, a, a, two_p, numwords);
    }
}

// conditional move g to f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void SQISIGN_NOINLINE
modcmv(int b, const spint g[MODQLIMBS], volatile spint f[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    spint c0, c1, s, t, w, aux;
    static spint R = 0;
    R += 0x3cc3c33c5aa5a55au;
    w = R;
    c0 = (~b) & (w + 1);
    c1 = b + w;
    for (i = 0; i < numwords; i++) {
        s = g[i];
        t = f[i];
        f[i] = aux = c0 * t + c1 * s;
        f[i] = aux - w * (t + s);
    }
}

// conditional swap g and f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void SQISIGN_NOINLINE
modcsw(int b, volatile spint g[MODQLIMBS], volatile spint f[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    spint c0, c1, s, t, w, v, aux;
    static spint R = 0;
    R += 0x3cc3c33c5aa5a55au;
    w = R;
    c0 = (~b) & (w + 1);
    c1 = b + w;
    for (i = 0; i < numwords; i++) {
        s = g[i];
        t = f[i];
        v = w * (t + s);
        f[i] = aux = c0 * t + c1 * s;
        f[i] = aux - v;
        g[i] = aux = c0 * s + c1 * t;
        g[i] = aux - v;
    }
}

// Squares an F_p^2 element z=x+√w*y (with w a non-square over F_p)
static void
modp2sqr(spint x[MODQLIMBS],
         spint y[MODQLIMBS],
         const spint w[MODQLIMBS],
         const spint p[MODQLIMBS],
         const spint two_p[MODQLIMBS],
         const spint ndash,
         int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    spint t0[MODQLIMBS], t1[MODQLIMBS], t2[MODQLIMBS];
    modqx_modsqr(x, t0, p, ndash, numwords);
    modqx_modsqr(y, t1, p, ndash, numwords);
    modqx_modmul(t1, w, t1, p, ndash, numwords);
    modqx_modmul(x, y, t2, p, ndash, numwords);
    modqx_modadd(t0, t1, x, two_p, numwords);
    modqx_modadd(t2, t2, y, two_p, numwords);
}

// Given an F_p^2 element z=x+√w*y (with w a non-square over F_p), computes xr+√w*yr := z*(t+√w)
static void
modp2mul(const spint x[MODQLIMBS],
         const spint y[MODQLIMBS],
         const spint t[MODQLIMBS],
         const spint w[MODQLIMBS],
         spint xr[MODQLIMBS],
         spint yr[MODQLIMBS],
         const spint p[MODQLIMBS],
         const spint two_p[MODQLIMBS],
         const spint ndash,
         int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    spint t0[MODQLIMBS], t1[MODQLIMBS], t2[MODQLIMBS];
    modqx_modmul(x, t, t0, p, ndash, numwords);
    modqx_modmul(y, w, t1, p, ndash, numwords);
    modqx_modmul(y, t, t2, p, ndash, numwords);
    modqx_modadd(x, t2, yr, two_p, numwords);
    modqx_modadd(t0, t1, xr, two_p, numwords);
}

// Cipolla algorithm for computing sqrt(x); requires any auxiliary t such that t^2-x is non-sqr. Returns -1 if number is
// a square, else 0.
int
modqx_modcipolla(const spint x[MODQLIMBS],
                 const spint t[MODQLIMBS],
                 spint r[MODQLIMBS],
                 const spint p[MODQLIMBS],
                 const spint two_p[MODQLIMBS],
                 const spint one[MODQLIMBS],
                 const spint ndash,
                 int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    if (numwords <= 0 || numwords > MODQLIMBS)
        return 0;
    spint e[MODQLIMBS] = { 0 };
    spint r0[MODQLIMBS], r1[MODQLIMBS] = { 0 }, tx[MODQLIMBS], ty[MODQLIMBS], w[MODQLIMBS];
    spint carry;
    int i, b;
    modqx_modsqr(t, w, p, ndash, numwords);
    modqx_modsub(w, x, w, two_p, numwords);
    modqx_modcpy(p, e, numwords);
    modqx_modshr(1, e, numwords);
    carry = 1;
    for (i = 0; i < numwords; i++) {
        e[i] += carry;
        carry = e[i] >> MODQRADIX;
        e[i] &= ((spint)1 << MODQRADIX) - (spint)1;
    }
    modqx_modcpy(one, r0, numwords);
    modqx_modzer(r1, numwords);
    for (i = 0; i < numwords * MODQRADIX - 1; i++) {
        modp2sqr(r0, r1, w, p, two_p, ndash, numwords);
        modp2mul(r0, r1, t, w, tx, ty, p, two_p, ndash, numwords);
        b = (int)((e[numwords - 1] >> (MODQRADIX - 2)) & 1);
        modqx_modshl(1, e, numwords);
        modcmv(b, tx, r0, numwords);
        modcmv(b, ty, r1, numwords);
    }
    modqx_modcpy(r0, r, numwords);
    return -(1 & modqx_modis0(r1, p, ndash, numwords)); // r1 == 0 iff x was a quadratic residue
}

// shift left by less than a word
void
modqx_modshl(unsigned int n, spint a[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    if (numwords == 1) {
        a[0] = a[0] << n;
        return;
    }
    a[numwords - 1] = ((a[numwords - 1] << n)) + (a[numwords - 2] >> (MODQRADIX - n));
    for (i = numwords - 2; i > 0; i--) {
        a[i] = ((a[i] << n) & MODQLIMBMASK) + (a[i - 1] >> (MODQRADIX - n));
    }
    a[0] = (a[0] << n) & MODQLIMBMASK;
}

// shift right by less than a word. Return shifted out part
int
modqx_modshr(unsigned int n, spint a[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int i;
    spint r = a[0] & (((spint)1 << n) - (spint)1);
    for (i = 0; i < numwords - 1; i++) {
        a[i] = (a[i] >> n) + ((a[i + 1] << (MODQRADIX - n)) & MODQLIMBMASK);
    }
    a[numwords - 1] = a[numwords - 1] >> n;
    return r;
}

// return true if equal
int
modqx_modcmp(const spint a[MODQLIMBS],
             const spint b[MODQLIMBS],
             const spint p[MODQLIMBS],
             const spint ndash,
             int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    spint c[MODQLIMBS], d[MODQLIMBS];
    int i, eq = 1;
    modqx_redc(a, c, p, ndash, numwords);
    modqx_redc(b, d, p, ndash, numwords);
    for (i = 0; i < numwords; i++) {
        eq &= (((c[i] ^ d[i]) - 1) >> MODQRADIX) & 1;
    }
    return eq;
}

// Compute ndash := (-p)^(-1) mod 2^MODQRADIX
spint
modqx_getndash(const spint p0)
{
    spint x, a, t;
    spint q = ((spint)1 << MODQRADIX); // q is unsaturated MODQRADIX
    spint mask = (spint)(q - (spint)1);
    a = q - (p0 & mask);
    x = 1u;
    t = a * x;
    t = 2 - t;
    x = x * t;
    t = a * x;
    t = 2 - t;
    x = x * t;
    t = a * x;
    t = 2 - t;
    x = x * t;
    t = a * x;
    t = 2 - t;
    x = x * t;
    t = a * x;
    t = 2 - t;
    x = x * t;
    t = a * x;
    t = 2 - t;
    x = x * t;
    x &= mask;
    return x;
}

void
modqx_modxpowe(const spint p[MODQLIMBS],
               const spint one[MODQLIMBS],
               const spint ndash,
               const spint x[MODQLIMBS],
               const spint e[MODQLIMBS],
               spint s[MODQLIMBS],
               int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    spint r0[MODQLIMBS], r1[MODQLIMBS], ee[MODQLIMBS];
    int b;
    modqx_modcpy(one, r0, numwords);
    modqx_modcpy(x, r1, numwords);
    modqx_modcpy(e, ee, numwords);
    for (int i = 0; i < numwords * MODQRADIX; i++) {
        b = (int)((ee[numwords - 1] >> (MODQRADIX - 1)) & 1);
        modcsw(b, r0, r1, numwords);
        modqx_modmul(r0, r1, r1, p, ndash, numwords);
        modqx_modsqr(r0, r0, p, ndash, numwords);
        modcsw(b, r0, r1, numwords);
        modqx_modshl(1, ee, numwords);
    }
    modqx_modcpy(r0, s, numwords);
}

// Precomputed table for x^e. Assumes montgomery 1 is already stored to table[0]
static void
modxpowe_precomp_table(spint table[TABLE_SIZE][MODQLIMBS],
                       const spint x[MODQLIMBS],
                       const spint p[MODQLIMBS],
                       const spint ndash,
                       int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    for (int i = 1; i < TABLE_SIZE; i++) {
        modqx_modmul(table[i - 1], x, table[i], p, ndash, numwords);
    }
}

// Constant time table lookup
static void
modtablelookup(const spint table[TABLE_SIZE][MODQLIMBS], const int index, spint a[MODQLIMBS], int numwords)
{
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    int b;
    for (int i = 0; i < TABLE_SIZE; i++) {
        b = (index ^ i) & ((1 << WINDOW_SIZE) - 1);
        for (int k = 0; k < WINDOW_SIZE; k++) {
            b = b | (b >> k);
        }
        b = 1 - (b & 1);
        modcmv(b, table[i], a, numwords);
    }
}

// Computes x^e mod p using a windowed method. Assumes Montgomery 1 is already in table[0]
void
modqx_modxpowe_windowed(const spint p[MODQLIMBS],
                        const spint two_p[MODQLIMBS],
                        const spint ndash,
                        const spint x[MODQLIMBS],
                        const spint e[MODQLIMBS],
                        spint table[TABLE_SIZE][MODQLIMBS],
                        spint s[MODQLIMBS],
                        int numwords)
{
    (void)two_p;
    assert(numwords <= MODQLIMBS);
    assert(numwords > 0);
    spint ee[MODQLIMBS] = { 0 }, tmp[MODQLIMBS] = { 0 };
    int num_windows = (MODQRADIX * numwords + WINDOW_SIZE - 1) / WINDOW_SIZE;
    int shiftr = num_windows * WINDOW_SIZE - MODQRADIX * (numwords - 1) - WINDOW_SIZE;
    modxpowe_precomp_table(table, x, p, ndash, numwords);
    modqx_modcpy(table[0], s, numwords);
    modqx_modcpy(e, ee, numwords);
    for (int i = 0; i < num_windows; i++) {
        for (int j = 0; j < WINDOW_SIZE; j++) {
            modqx_modsqr(s, s, p, ndash, numwords);
        }
        int index = (ee[numwords - 1] >> shiftr) & ((1 << WINDOW_SIZE) - 1);
        modtablelookup((const spint(*)[MODQLIMBS])table, index, tmp, numwords);
        modqx_modmul(s, tmp, s, p, ndash, numwords);
        modqx_modshl(WINDOW_SIZE, ee, numwords);
    }
}
