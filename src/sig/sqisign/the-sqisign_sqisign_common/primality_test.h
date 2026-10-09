#ifndef PRIMALITY_TEST_H
#define PRIMALITY_TEST_H

#include <mp.h>
#include <modqx.h>
#include <assert.h>
#include <inttypes.h>
#define NUM_LIMBS(bits) ((int)(((bits) + (NUM_BITS_LIMB - 1)) / (NUM_BITS_LIMB)))

/*
 * Qlapoty aggressive candidate sieve.
 *
 * This uses every small odd prime, including primes q == 1 mod 4.
 * Hence, it is not a valid candidate preserving sieve for arbitrary composite
 * integers that admit sqrt(-1).
 *
 * The per-level sieving bounds are:
 *   L1: 509, L3: 769, L5: 509.
 * The table extends until 1021.
 */
static const uint32_t qlapoty_aggressive_primes[] = {
    3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,   53,   59,   61,  67,  71,  73,  79,  83,
    89,  97,  101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157,  163,  167,  173, 179, 181, 191, 193, 197,
    199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277,  281,  283,  293, 307, 311, 313, 317, 331,
    337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,  421,  431,  433, 439, 443, 449, 457, 461,
    463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563,  569,  571,  577, 587, 593, 599, 601, 607,
    613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691,  701,  709,  719, 727, 733, 739, 743, 751,
    757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853,  857,  859,  863, 877, 881, 883, 887, 907,
    911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021
};

// These primes are used to find (a/n) = -1
// With high probability only the first four are used in
// the Qlapoty code
static const uint32_t qlapoty_jacobi_bases[] = { 2,  3,   5,   7,   11,  13,  17,  19,  23,  29, 31, 37,
                                                 41, 43,  47,  53,  59,  61,  67,  71,  73,  79, 83, 89,
                                                 97, 101, 103, 107, 109, 113, 127, 131, 137, 139 };

typedef struct
{
    int numwords;
    spint n[MODQLIMBS];
    spint two_n[MODQLIMBS];
    spint one[MODQLIMBS];
    spint minus_one[MODQLIMBS];
    spint ndash;
    ibz_t d;
    int s;
} probprime_ctx_t;

/*
 * The batched preinverse sieve packs several small primes into a
 * single machine word (its product, and a Granlund-Moller reciprocal
 * normalized to that word's bit width) to turn many trial divisions into
 * one multiply.
 */
#if RADIX == 64
#define PROBPRIME_SPLIT_A UINT64_C(0xf60059af) /* 3*5*7*11*13*17*19*23*37 */
#define PROBPRIME_SPLIT_B UINT64_C(0xeb52e3f3) /* 29*31*41*43*47*53 */
static uint32_t
probprime_mod_split_a_internal(const ibz_t *n)
{
    int nlimbs = NUM_LIMBS(n->bitlen);
    digit_t r = 0;

    for (int i = nlimbs - 1; i >= 0; i--) {
        digit_t w = n->limbs[i];

        r = (digit_t)(((r << 32) | (w >> 32)) % PROBPRIME_SPLIT_A);
        r = (digit_t)(((r << 32) | (w & UINT64_C(0xffffffff))) % PROBPRIME_SPLIT_A);
    }

    return (uint32_t)r;
}
static uint32_t
probprime_mod_split_b_internal(const ibz_t *n)
{
    int nlimbs = NUM_LIMBS(n->bitlen);
    digit_t r = 0;

    for (int i = nlimbs - 1; i >= 0; i--) {
        digit_t w = n->limbs[i];

        r = (digit_t)(((r << 32) | (w >> 32)) % PROBPRIME_SPLIT_B);
        r = (digit_t)(((r << 32) | (w & UINT64_C(0xffffffff))) % PROBPRIME_SPLIT_B);
    }

    return (uint32_t)r;
}
#endif

typedef struct
{
    digit_t d;
    digit_t di;
    unsigned shift;
    unsigned first;
    unsigned last;
} probprime_sieve_batch_t;

/*
 * Fixed products of small primes and their normalized modulus preinverses.
 * For each batch d = prod(p_i), compute r = n mod d once; then
 * p_i | n iff r mod p_i = 0.
 */
#if RADIX == 64
/* Covers primes 59..487, preserving sieve bounds 509 for L1/L5 and 1021 for L3. */
static const probprime_sieve_batch_t probprime_sieve_batches_common[] = {
    { UINT64_C(0x6329899ea9f2714b), UINT64_C(0x4a72c477c0963cdb), 1u, 15u, 25u }, /* 59..101 */
    { UINT64_C(0x58edcb4c9ed39c8b), UINT64_C(0x707965cc3eac798e), 1u, 25u, 34u }, /* 103..149 */
    { UINT64_C(0x09966ff94fd516fb), UINT64_C(0xab37686a5d6e1835), 4u, 34u, 42u }, /* 151..191 */
    { UINT64_C(0x3bd7632c1f36eb51), UINT64_C(0x11ca639fffb1f62a), 2u, 42u, 50u }, /* 193..233 */
    { UINT64_C(0x00fd14b3c90d88a9), UINT64_C(0x02f3ead63d941e0e), 8u, 50u, 57u }, /* 239..271 */
    { UINT64_C(0x02ad3dbe0cca85ff), UINT64_C(0x7e8ea9abc0c5a2d6), 6u, 57u, 64u }, /* 277..313 */
    { UINT64_C(0x0787f9a02c3388a7), UINT64_C(0x0fefe739af03fd10), 5u, 64u, 71u }, /* 317..359 */
    { UINT64_C(0x1113c5cc6d101657), UINT64_C(0xdfb3f0f9b8c8b7b1), 3u, 71u, 78u }, /* 367..401 */
    { UINT64_C(0x2456c94f936bdb15), UINT64_C(0xc2dd8758870f9711), 2u, 78u, 85u }, /* 409..443 */
    { UINT64_C(0x4236a30b85ffe139), UINT64_C(0xeee270a9d031bebc), 1u, 85u, 92u }, /* 449..487 */
};
static const probprime_sieve_batch_t probprime_sieve_batches_509[] = {
    { UINT64_C(0x0000000e9aef58cb), UINT64_C(0x1872a19b47be8fb4), 28u, 92u, 96u }, /* 491..509 */
};
static const probprime_sieve_batch_t probprime_sieve_batches_1021[] = {
    { UINT64_C(0x805437b38eada69d), UINT64_C(0xfeaffe452003a47e), 0u,  92u,  99u }, /* 491..541 */
    { UINT64_C(0x00723e97bddcd2af), UINT64_C(0x1ed2cc34bc0c181d), 9u,  99u, 105u }, /* 547..577 */
    { UINT64_C(0x00a5a792ee239667), UINT64_C(0x8b9e4e9bcd5b9458), 8u, 105u, 111u }, /* 587..613 */
    { UINT64_C(0x00e451352ebca269), UINT64_C(0x1f0a0b7149843601), 8u, 111u, 117u }, /* 617..647 */
    { UINT64_C(0x013a7955f14b7805), UINT64_C(0xa0cc308fcfbd3e0d), 7u, 117u, 123u }, /* 653..683 */
    { UINT64_C(0x01d37cbd653b06ff), UINT64_C(0x1860242e63e661d6), 7u, 123u, 129u }, /* 691..733 */
    { UINT64_C(0x0288fe4eca4d7cdf), UINT64_C(0x93ec8aa98a84de0b), 6u, 129u, 135u }, /* 739..769 */
    { UINT64_C(0x039fddb60d3af63d), UINT64_C(0x1a8606753dac09b2), 6u, 135u, 141u }, /* 773..821 */
    { UINT64_C(0x04cd73f19080fb03), UINT64_C(0xaa70a958c486716c), 5u, 141u, 147u }, /* 823..857 */
    { UINT64_C(0x0639c390b9313f05), UINT64_C(0x48f64f21c7ab90db), 5u, 147u, 153u }, /* 859..887 */
    { UINT64_C(0x08a1c420d25d388f), UINT64_C(0xda84dcd209b64d3f), 4u, 153u, 159u }, /* 907..941 */
    { UINT64_C(0x0b4b5322977db499), UINT64_C(0x6aa9aee2cab2fcf9), 4u, 159u, 165u }, /* 947..983 */
    { UINT64_C(0x0e94c170a802ee29), UINT64_C(0x18e97b7f459899fe), 4u, 165u, 171u }, /* 991..1021 */
};
#elif RADIX == 32
/* Covers primes 3..487, preserving sieve bounds 509 for L1/L5 and 1021 for L3. */
static const probprime_sieve_batch_t probprime_sieve_batches_common[] = {
    { 0xc0cfd797u, 0x53e5645cu,  0u,  0u,  9u }, /* 3..29 */
    { 0x05aa381fu, 0x6985f537u,  5u,  9u, 14u }, /* 31..47 */
    { 0x361592b7u, 0x2eef45f7u,  2u, 14u, 19u }, /* 53..71 */
    { 0xf64d985du, 0x0a1422f2u,  0u, 19u, 24u }, /* 73..97 */
    { 0x073b5a0du, 0x1b3126a2u,  5u, 24u, 28u }, /* 101..109 */
    { 0x0f5a0395u, 0x0acff1afu,  4u, 28u, 32u }, /* 113..137 */
    { 0x1d43ffddu, 0x17eb3ad9u,  3u, 32u, 36u }, /* 139..157 */
    { 0x323e7003u, 0x4616e028u,  2u, 36u, 40u }, /* 163..179 */
    { 0x4e5884b7u, 0xa23f9b44u,  1u, 40u, 44u }, /* 181..197 */
    { 0x7eb0f0b1u, 0x02a50ae1u,  1u, 44u, 48u }, /* 199..227 */
    { 0xb72efc93u, 0x65c2eb04u,  0u, 48u, 52u }, /* 229..241 */
    { 0x0102deddu, 0xfa529227u,  7u, 52u, 55u }, /* 251..263 */
    { 0x01341effu, 0xa96426adu,  7u, 55u, 58u }, /* 269..277 */
    { 0x0163888fu, 0x70a9b660u,  7u, 58u, 61u }, /* 281..293 */
    { 0x01c7ff8du, 0x1f709059u,  7u, 61u, 64u }, /* 307..313 */
    { 0x021b8e8fu, 0xe5d9bd6cu,  6u, 64u, 67u }, /* 317..337 */
    { 0x028c4dafu, 0x91dfd2d4u,  6u, 67u, 70u }, /* 347..353 */
    { 0x02ede03du, 0x5d954a31u,  6u, 70u, 73u }, /* 359..373 */
    { 0x035d9a99u, 0x30404d26u,  6u, 73u, 76u }, /* 379..389 */
    { 0x03e18615u, 0x07da4f6cu,  6u, 76u, 79u }, /* 397..409 */
    { 0x04881841u, 0xc3ef7d16u,  5u, 79u, 82u }, /* 419..431 */
    { 0x0504eb9du, 0x98081137u,  5u, 82u, 85u }, /* 433..443 */
    { 0x05a363b5u, 0x6b3be150u,  5u, 85u, 88u }, /* 449..461 */
    { 0x062c59c3u, 0x4bc114edu,  5u, 88u, 91u }, /* 463..479 */
    { 0x000001e7u, 0x0d244563u, 23u, 91u, 92u }, /* 487..487 */
};
static const probprime_sieve_batch_t probprime_sieve_batches_509[] = {
    { 0x07587c67u, 0x16cdf5a8u,  5u, 92u, 95u }, /* 491..503 */
    { 0x000001fdu, 0x01824365u, 23u, 95u, 96u }, /* 509..509 */
};
static const probprime_sieve_batch_t probprime_sieve_batches_1021[] = {
    { 0x07587c67u, 0x16cdf5a8u,  5u,  92u,  95u }, /* 491..503 */
    { 0x08444cd7u, 0xef79dd1eu,  4u,  95u,  98u }, /* 509..523 */
    { 0x09d3206bu, 0xa0e867e5u,  4u,  98u, 101u }, /* 541..557 */
    { 0x0ae71bf9u, 0x77af2fe3u,  4u, 101u, 104u }, /* 563..571 */
    { 0x0bf8b47bu, 0x56255455u,  4u, 104u, 107u }, /* 577..593 */
    { 0x0d065761u, 0x3a7a4abbu,  4u, 107u, 110u }, /* 599..607 */
    { 0x0df45e8fu, 0x2586222bu,  4u, 110u, 113u }, /* 613..619 */
    { 0x0f806d65u, 0x083ac7a8u,  4u, 113u, 116u }, /* 631..643 */
    { 0x10986041u, 0xeda2db36u,  3u, 116u, 119u }, /* 647..659 */
    { 0x11f36ba9u, 0xc85b5f93u,  3u, 119u, 122u }, /* 661..677 */
    { 0x13b8340du, 0x9f6ce882u,  3u, 122u, 125u }, /* 683..701 */
    { 0x1616f5fdu, 0x72da0aa1u,  3u, 125u, 128u }, /* 709..727 */
    { 0x17fd41e1u, 0x557c5b83u,  3u, 128u, 131u }, /* 733..743 */
    { 0x19c977e3u, 0x3dadfc23u,  3u, 131u, 134u }, /* 751..761 */
    { 0x1be2655fu, 0x25c8e67du,  3u, 134u, 137u }, /* 769..787 */
    { 0x1f2afcb7u, 0x06d598bcu,  3u, 137u, 140u }, /* 797..811 */
    { 0x214e74d1u, 0xebea9b89u,  2u, 140u, 143u }, /* 821..827 */
    { 0x235cdb07u, 0xcf5014a3u,  2u, 143u, 146u }, /* 829..853 */
    { 0x25de0a7du, 0xb0ab142du,  2u, 146u, 149u }, /* 857..863 */
    { 0x28aa2207u, 0x92e7e93bu,  2u, 149u, 152u }, /* 877..883 */
    { 0x2baf48b3u, 0x770d1afdu,  2u, 152u, 155u }, /* 887..911 */
    { 0x2fae7e0fu, 0x579cd027u,  2u, 155u, 158u }, /* 919..937 */
    { 0x329e6f7fu, 0x43ac73afu,  2u, 158u, 161u }, /* 941..953 */
    { 0x36add15du, 0x2ba3cd0du,  2u, 161u, 164u }, /* 967..977 */
    { 0x39e3cd4du, 0x1b052edcu,  2u, 164u, 167u }, /* 983..997 */
    { 0x3e1498c7u, 0x07ea63deu,  2u, 167u, 170u }, /* 1009..1019 */
    { 0x000003fdu, 0x00c0906cu, 22u, 170u, 171u }, /* 1021..1021 */
};
#endif

// ===============================
/*
 * Helper functions
 */
// ===============================

static int ibz_bit_at(const ibz_t *x, int i);
static void ibz_to_modq(spint *a, const ibz_t *x);
static void modxpowe_non_ct(spint s[MODQLIMBS],
                            const spint x[MODQLIMBS],
                            const ibz_t *e,
                            const spint n[MODQLIMBS],
                            const spint two_n[MODQLIMBS],
                            const spint ndash,
                            int numwords,
                            int window);

/*Computes n mod m, where m is a small single word modulus. Used by the sieving
    and small Jacobi computations.
    For sieving m is the product of the q_i small odd primes.
*/
static digit_t
qlapoty_mod_digit(const ibz_t *n, digit_t m)
{
    assert(m != 0);
    assert(ibz_is_positive(n));

    int nlimbs = NUM_LIMBS(n->bitlen);
    digit_t r = 0;

    for (int i = nlimbs - 1; i >= 0; i--) {
        ddigit_t x = ((ddigit_t)r << NUM_BITS_LIMB) + (ddigit_t)n->limbs[i];
        r = (digit_t)(x % (ddigit_t)m);
    }

    return r;
}

/*
    Searches a short fixed list of small primes and returns one prime $a$ such that
    (n/a) = -1. If n mod 8 = 5 then returns a = 2, otherwise does a search with
    fast Jacobi symbol computations.
*/
static uint32_t
qlapoty_find_jacobi_minus_one(const ibz_t *n)
{
    assert(ibz_is_positive(n));
    assert((n->limbs[0] & 3) == 1);

    for (size_t i = 0; i < sizeof(qlapoty_jacobi_bases) / sizeof(qlapoty_jacobi_bases[0]); i++) {
        uint32_t a = qlapoty_jacobi_bases[i];

        if (a == 2) {
            if ((n->limbs[0] & 7) == 5)
                return 2;
            continue;
        }

        uint32_t r = (uint32_t)qlapoty_mod_digit(n, a);
        if (r == 0)
            continue;

        uint32_t e = (a - 1) >> 1;
        uint64_t base = r;
        uint64_t acc = 1;

        while (e != 0) {
            if (e & 1)
                acc = (acc * base) % a;
            base = (base * base) % a;
            e >>= 1;
        }

        if (acc == a - 1)
            return a;
    }

    return 0;
}

/*
 * Exact remainder of nh*2^NUM_BITS_LIMB+nl modulo normalized d, using
 * di = floor((2^(2*NUM_BITS_LIMB)-1)/d) - 2^NUM_BITS_LIMB. Preconditions:
 * the top bit of d is set, and nh < d.
 */
static digit_t
probprime_rem_2by1_preinv_internal(digit_t nh, digit_t nl, digit_t d, digit_t di)
{
    ddigit_t p = (ddigit_t)nh * (ddigit_t)di;
    digit_t qh = (digit_t)(p >> NUM_BITS_LIMB);
    digit_t ql = (digit_t)p;
    digit_t suml = ql + nl;
    digit_t carry = (suml < ql);

    ql = suml;
    qh += nh + (digit_t)1 + carry;

    digit_t r = nl - qh * d;

    if (r > ql)
        r += d;

    if (r >= d)
        r -= d;

    return r;
}

static digit_t
probprime_mod_u64_preinv_internal(const ibz_t *n, const probprime_sieve_batch_t *b)
{
    int nlimbs = NUM_LIMBS(n->bitlen);
    digit_t r = 0;
    unsigned s = b->shift;
    digit_t dn = b->d << s;

    for (int i = nlimbs - 1; i >= 0; i--) {
        digit_t w = n->limbs[i];
        digit_t nh;
        digit_t nl;

        if (s == 0) {
            nh = r;
            nl = w;
        } else {
            nh = (r << s) | (w >> (NUM_BITS_LIMB - s));
            nl = w << s;
        }

        digit_t rn = probprime_rem_2by1_preinv_internal(nh, nl, dn, b->di);

        r = (s == 0) ? rn : (rn >> s);
    }

    return r;
}

/*
 * Fast small-prime sieve:
 *
 * 1. At RADIX==64 only: reduce n modulo two <2^32 products and reject
 *    divisibility by the primes 3..53 using two native 64-by-32 divisions.
 *    At RADIX==32 these primes are already covered by the main batch table below.
 *
 * 2. Select the precomputed batches corresponding to the requested sieve
 *    bound (509 or 1021).
 *
 * 3. For each batch d = prod(p_i), compute r = n mod d using the
 *    normalized preinverse, then reject if r mod p_i = 0 for any p_i.
 *
 * Return 1 only if n survives all small-prime tests.
 */
static int
probprime_sieve_fast_internal(const ibz_t *n, uint32_t sieve_bound)
{
#if RADIX == 64
    uint32_t ra = probprime_mod_split_a_internal(n);

    if (ra % 3u == 0 || ra % 5u == 0 || ra % 7u == 0 || ra % 11u == 0 || ra % 13u == 0 || ra % 17u == 0 ||
        ra % 19u == 0 || ra % 23u == 0 || ra % 37u == 0)
        return 0;

    uint32_t rb = probprime_mod_split_b_internal(n);

    if (rb % 29u == 0 || rb % 31u == 0 || rb % 41u == 0 || rb % 43u == 0 || rb % 47u == 0 || rb % 53u == 0)
        return 0;
#endif

    const probprime_sieve_batch_t *tables[2];
    size_t sizes[2];

    tables[0] = probprime_sieve_batches_common;
    sizes[0] = sizeof(probprime_sieve_batches_common) / sizeof(probprime_sieve_batches_common[0]);

    if (sieve_bound == 509u) {
        tables[1] = probprime_sieve_batches_509;
        sizes[1] = sizeof(probprime_sieve_batches_509) / sizeof(probprime_sieve_batches_509[0]);
    } else {
        assert(sieve_bound == 1021u);
        tables[1] = probprime_sieve_batches_1021;
        sizes[1] = sizeof(probprime_sieve_batches_1021) / sizeof(probprime_sieve_batches_1021[0]);
    }

    for (size_t t = 0; t < 2; t++) {
        for (size_t j = 0; j < sizes[t]; j++) {
            const probprime_sieve_batch_t *b = &tables[t][j];
            digit_t r = probprime_mod_u64_preinv_internal(n, b);

            for (unsigned k = b->first; k < b->last; k++) {
                if (r % qlapoty_aggressive_primes[k] == 0)
                    return 0;
            }
        }
    }

    return 1;
}

/**
 * Aggressively reject candidates divisible by some small odd primes.
 * Small primes are batched into one-word products so one qlapoty_mod_digit()
 * reduction tests several primes.
 *
 * This function performs:
 * Step 0. Builds the largest product of consecutive small primes that fits
 *         in one word.
 * Step 1. Computes n modulo that product.
 * Step 2. Tests the remainder against each prime in the batch.
 * Step 3. Rejects the candidate if one of the small primes divides n,
 *         except when n is that small prime itself.
 * Step 4. Repeats the process until all primes up to sieve_bound have been
 *         tested.
 * Returns 1 if the candidate survives the sieve.
 * Otherwise returns 0.
 */
static int
probprime_sieve_internal(const ibz_t *n, uint32_t sieve_bound)
{
    if (ibz_bitsize(n) > 32 && (sieve_bound == 509u || sieve_bound == 1021u))
        return probprime_sieve_fast_internal(n, sieve_bound);

    const digit_t max_product = (digit_t)-1;
    size_t count = sizeof(qlapoty_aggressive_primes) / sizeof(qlapoty_aggressive_primes[0]);
    size_t i = 0;

    /* Step 4. Repeats the process until all primes have been tested. */
    while (i < count && qlapoty_aggressive_primes[i] <= sieve_bound) {
        digit_t product = 1;
        size_t j = i;

        /* Step 0. Builds the largest product of small primes that fits in one word. */
        while (j < count && qlapoty_aggressive_primes[j] <= sieve_bound) {
            digit_t q = qlapoty_aggressive_primes[j];

            if (product > max_product / q)
                break;

            product *= q;
            j++;
        }

        /* Step 1. Computes n modulo that product. */
        digit_t r = qlapoty_mod_digit(n, product);

        /* Step 2. Tests the remainder against each prime in the batch. */
        for (size_t k = i; k < j; k++) {
            uint32_t q = qlapoty_aggressive_primes[k];

            /* Step 3. Rejects the candidate if one of the small primes divides n,
               except when n is that small prime itself. */
            if (r % q == 0) {
                if (ibz_bitsize(n) <= 32 && n->limbs[0] == (digit_t)q)
                    continue;

                return 0;
            }
        }

        i = j;
    }

    return 1;
}

/**
 * Initialize the Montgomery constants shared by Miller-Rabin and Lucas.
 * Computes n, 2n, Montgomery +/-1, ndash, and n-1=d*2^s. Montgomery -1 is
 * obtained directly from +1, which saves a modqx_nresx(n-1) conversion.
 *
 * This function performs:
 * Step 0. Clears the context.
 * Step 1. Computes the number of MODQ words required to represent n.
 * Step 2. Converts n to MODQ representation, computes 2n, and computes ndash.
 * Step 3. Computes Montgomery +1.
 * Step 4. Computes Montgomery -1 directly from Montgomery +1.
 * Step 5. Computes n - 1.
 * Step 6. Decomposes n - 1 as d * 2^s.
 * Returns 1 if the context is initialized correctly.
 * Otherwise returns 0.
 */
static int
probprime_ctx_init_internal(probprime_ctx_t *ctx, const ibz_t *n)
{
    /* Step 0. Clears the context. */
    memset(ctx, 0, sizeof(*ctx));

    /* Step 1. Computes the number of MODQ words required to represent n and
       checks that it fits in the available MODQ storage. */
    ctx->numwords = (n->bitlen + MODQRADIX + 1) / MODQRADIX;

    if (ctx->numwords <= 0 || ctx->numwords >= MODQLIMBS)
        return 0;

    /* Step 2. Converts n to MODQ representation, computes 2n, and computes ndash. */
    ibz_to_modq(ctx->n, n);
    modqx_modcpy(ctx->n, ctx->two_n, ctx->numwords);
    modqx_modshl(1, ctx->two_n, ctx->numwords);
    ctx->ndash = modqx_getndash(ctx->n[0]);

    /* Step 3. Computes Montgomery +1. */
    modqx_modone(ctx->one, ctx->two_n, ctx->numwords);

    /* Step 4. Computes Montgomery -1 directly from Montgomery +1. */
    spint zero[MODQLIMBS] = { 0 };
    modqx_modzer(zero, ctx->numwords);
    modqx_modsub(zero, ctx->one, ctx->minus_one, ctx->two_n, ctx->numwords);

    /* Step 5. Computes n - 1. */
    ibz_t nm1 = { 0 };
    ibz_sub(&nm1, n, &ibz_const_one);

    /* Step 6. Decomposes n - 1 as d * 2^s. */
    ctx->s = ibz_two_adic(&nm1);
    ibz_div_2exp(&ctx->d, &nm1, (uint32_t)ctx->s);

    return 1;
}

/**
 * Compare two Montgomery residues modulo n without converting from
 * Montgomery form.
 *
 * MODQ operations return representatives in [0,2n), so each operand is
 * reduced by n at most once before word comparison.
 *
 * This function performs:
 * Step 0. Copies both input residues into temporary arrays.
 * Step 1. Processes each temporary residue independently.
 * Step 2. Compares the residue with n.
 * Step 3. If the residue is greater than or equal to n, subtracts n once.
 * Step 4. Compares the two normalized residues word by word.
 * Returns 1 if both residues are equal modulo n.
 * Otherwise returns 0.
 */
static int
probprime_mont_equal_internal(const spint *a, const spint *b, const probprime_ctx_t *ctx)
{
    /* Step 0. Copies both input residues into temporary arrays. */
    spint aa[MODQLIMBS] = { 0 };
    spint bb[MODQLIMBS] = { 0 };

    modqx_modcpy(a, aa, ctx->numwords);
    modqx_modcpy(b, bb, ctx->numwords);

    spint *vals[2] = { aa, bb };

    /* Step 1. Processes each temporary residue independently. */
    for (size_t which = 0; which < 2; which++) {
        spint *v = vals[which];
        int ge = 0;

        /* Step 2. Compares the residue with n. */
        for (int i = ctx->numwords - 1; i >= 0; i--) {
            spint vi = v[i] & MODQLIMBMASK;
            spint ni = ctx->n[i] & MODQLIMBMASK;

            if (vi > ni) {
                ge = 1;
                break;
            }

            if (vi < ni) {
                ge = -1;
                break;
            }
        }

        /* Step 3. If the residue is greater than or equal to n, subtracts n once. */
        if (ge >= 0) {
            const ddigit_t base = (ddigit_t)1 << MODQRADIX;
            spint borrow = 0;

            for (int i = 0; i < ctx->numwords; i++) {
                ddigit_t vi = (ddigit_t)(v[i] & MODQLIMBMASK);
                ddigit_t ni = (ddigit_t)(ctx->n[i] & MODQLIMBMASK) + borrow;

                if (vi >= ni) {
                    v[i] = (spint)(vi - ni) & MODQLIMBMASK;
                    borrow = 0;
                } else {
                    v[i] = (spint)(base + vi - ni) & MODQLIMBMASK;
                    borrow = 1;
                }
            }
        }
    }

    /* Step 4. Compares the two normalized residues word by word. */
    for (int i = ctx->numwords - 1; i >= 0; i--) {
        if ((aa[i] & MODQLIMBMASK) != (bb[i] & MODQLIMBMASK))
            return 0;
    }

    return 1;
}

/**
 * Perform one strong Miller-Rabin round using an encoded witness.
 */
static int
probprime_mr_round_internal(probprime_ctx_t *ctx, spint x[MODQLIMBS], int window)
{
    modxpowe_non_ct(x, x, &ctx->d, ctx->n, ctx->two_n, ctx->ndash, ctx->numwords, window);

    if (probprime_mont_equal_internal(x, ctx->one, ctx) || probprime_mont_equal_internal(x, ctx->minus_one, ctx))
        return 1;

    for (int r = 1; r < ctx->s; r++) {
        modqx_modsqr(x, x, ctx->n, ctx->ndash, ctx->numwords);

        if (probprime_mont_equal_internal(x, ctx->minus_one, ctx))
            return 1;

        if (probprime_mont_equal_internal(x, ctx->one, ctx))
            return 0;
    }

    return 0;
}

/**
 * Compute the Jacobi symbol (a/n) for one-word odd n.
 *
 * This function performs:
 * Step 0. Checks that n is nonzero and odd.
 * Step 1. Reduces a modulo n and initializes the sign.
 * Step 2. Removes factors of 2 from a and updates the sign using n mod 8.
 * Step 3. Swaps a and n.
 * Step 4. Applies quadratic reciprocity when both values are 3 mod 4.
 * Step 5. Reduces a modulo n and repeats until a becomes zero.
 * Step 6. Returns the accumulated sign if n == 1.
 * Otherwise returns 0.
 */
static int
probprime_jacobi_u64_internal(digit_t a, digit_t n)
{
    /* Step 0. Checks that n is nonzero and odd. */
    if (n == 0 || !(n & 1u))
        return 0;

    /* Step 1. Reduces a modulo n and initializes the sign. */
    a %= n;
    int sign = 1;

    /* Step 5. Reduces a modulo n and repeats until a becomes zero. */
    while (a != 0) {
        /* Step 2. Removes factors of 2 from a and updates the sign using n mod 8. */
        while (!(a & 1u)) {
            a >>= 1;
            digit_t r = n & 7u;

            if (r == 3 || r == 5)
                sign = -sign;
        }

        /* Step 3. Swaps a and n. */
        digit_t t = a;
        a = n;
        n = t;

        /* Step 4. Applies quadratic reciprocity when both values are 3 mod 4. */
        if ((a & 3u) == 3 && (n & 3u) == 3)
            sign = -sign;

        /* Step 5. Reduces a modulo n and repeats until a becomes zero. */
        a %= n;
    }

    /* Step 6. Returns the accumulated sign if n == 1.
       Otherwise returns 0. */
    return n == 1 ? sign : 0;
}

/**
 * Return 1 iff n is a perfect square.
 * Cheap quadratic-residue filters reject most inputs before
 * the expensive ibz_sqrt_floor() is invoked.
 *
 * This function performs:
 * Step 0. Defines a set of small moduli used for quadratic-residue filtering.
 * Step 1. Computes n modulo each small modulus.
 * Step 2. Checks whether the remainder is a quadratic residue modulo that modulus.
 * Step 3. Rejects n immediately if it is not a quadratic residue for any modulus.
 * Step 4. Computes floor(sqrt(n)) using ibz_sqrt_floor().
 * Step 5. Squares the computed root.
 * Step 6. Compares root^2 with n.
 * Returns 1 if n is a perfect square.
 * Otherwise returns 0.
 */
static int
probprime_perfect_square_internal(const ibz_t *n)
{
    /* Step 0. Defines a set of small moduli used for quadratic-residue filtering. */
    static const uint32_t mods[] = { 8, 3, 5, 7, 11, 13 };

    for (size_t i = 0; i < sizeof(mods) / sizeof(mods[0]); i++) {
        /* Step 1. Computes n modulo each small modulus. */
        uint32_t m = mods[i];
        uint32_t r = (uint32_t)qlapoty_mod_digit(n, m);
        int residue = 0;

        /* Step 2. Checks if the remainder is a quadratic residue modulo that modulus. */
        for (uint32_t x = 0; x < m; x++) {
            if ((x * x) % m == r) {
                residue = 1;
                break;
            }
        }

        /* Step 3. Rejects n if it is not a quadratic residue for any modulus. */
        if (!residue)
            return 0;
    }

    /* Step 4. Computes floor(sqrt(n)) using ibz_sqrt_floor(). */
    ibz_t root = { 0 }, sq = { 0 };
    ibz_sqrt_floor(&root, n);

    /* Step 5. Squares the computed root. */
    ibz_mul(&sq, &root, &root);

    /* Step 6. Compares root^2 with n. */
    return ibz_cmp(&sq, n) == 0;
}

/**
 * Divide a Montgomery residue by two modulo n.
 * If the input is odd, n is added before shifting.
 *
 * This function performs:
 * Step 0. Compares the input residue a with n.
 * Step 1. If a >= n, subtracts n once to obtain a representative in [0,n).
 * Step 2. Checks whether the normalized representative is odd.
 * Step 3. If a is odd, adds n to a.
 * Step 4. Divides the resulting even value by two using a right shift.
 *
 * The result represents a / 2 modulo n in Montgomery representation.
 * This constant is needed by Lucas
 */
static void
probprime_half_mod_internal(spint *a, const probprime_ctx_t *ctx)
{
    /* Step 0. Compares the input residue a with n. */
    int ge = 0;

    for (int i = ctx->numwords - 1; i >= 0; i--) {
        spint ai = a[i] & MODQLIMBMASK;
        spint ni = ctx->n[i] & MODQLIMBMASK;

        if (ai > ni) {
            ge = 1;
            break;
        }

        if (ai < ni) {
            ge = -1;
            break;
        }
    }

    /* Step 1. If a >= n, subtracts n once to obtain a representative in [0,n). */
    if (ge >= 0) {
        const ddigit_t base = (ddigit_t)1 << MODQRADIX;
        spint borrow = 0;

        for (int i = 0; i < ctx->numwords; i++) {
            ddigit_t ai = (ddigit_t)(a[i] & MODQLIMBMASK);
            ddigit_t ni = (ddigit_t)(ctx->n[i] & MODQLIMBMASK) + borrow;

            if (ai >= ni) {
                a[i] = (spint)(ai - ni) & MODQLIMBMASK;
                borrow = 0;
            } else {
                a[i] = (spint)(base + ai - ni) & MODQLIMBMASK;
                borrow = 1;
            }
        }
    }

    /* Step 2. Checks if the normalized representative is odd. */

    /* Step 3. If a is odd, adds n to a.  */
    if (a[0] & 1u) {
        spint carry = 0;

        for (int i = 0; i < ctx->numwords; i++) {
            ddigit_t z = (ddigit_t)(a[i] & MODQLIMBMASK) + (ddigit_t)(ctx->n[i] & MODQLIMBMASK) + (ddigit_t)carry;

            a[i] = (spint)z & MODQLIMBMASK;
            carry = (spint)(z >> MODQRADIX);
        }
    }

    /* Step 4. Divides the resulting even value by two using a right shift. */
    spint carry = 0;

    for (int i = ctx->numwords - 1; i >= 0; i--) {
        spint next = a[i] & 1u;

        a[i] = ((a[i] >> 1) | (carry << (MODQRADIX - 1))) & MODQLIMBMASK;

        carry = next;
    }
}

/**
 * Multiply a Montgomery residue by a small signed Lucas integer.
 * Uses binary double-and-add for the small Lucas constants.
 *
 * This function performs:
 * Step 0. Initializes the accumulator to zero and copies the input residue
 * Step 1. Computes the absolute value of the signed integer k.
 * Step 2. Scans the bits of |k| from right to left.
 * Step 3. If the current bit of |k| is 1, adds the current base to the accumulator.
 * Step 4. Doubles the current base before processing the next bit.
 * Step 5. If k is negative, negates the accumulated result modulo n.
 * Step 6. Otherwise copies the accumulated result to the output.
 */
static void
probprime_scale_small_internal(const spint *a, sdigit_t k, spint *out, const probprime_ctx_t *ctx)
{
    /* Step 0. Initializes the accumulator to zero and copies the input residue */
    spint acc[MODQLIMBS] = { 0 };
    spint base[MODQLIMBS] = { 0 };
    spint tmp[MODQLIMBS] = { 0 };

    modqx_modzer(acc, ctx->numwords);
    modqx_modcpy(a, base, ctx->numwords);

    /* Step 1. Computes the absolute value of the signed integer k. */
    digit_t mag = (k < 0) ? (digit_t)(-(k + 1)) + 1u : (digit_t)k;

    /* Step 2. Scans the bits of |k| from right to left. */
    while (mag != 0) {
        /* Step 3. If the current bit of |k| is 1, adds the current base
           to the accumulator. */
        if (mag & 1u) {
            modqx_modadd(acc, base, tmp, ctx->two_n, ctx->numwords);
            modqx_modcpy(tmp, acc, ctx->numwords);
        }

        mag >>= 1;

        /* Step 4. Doubles the current base before processing the next bit. */
        if (mag != 0) {
            modqx_modadd(base, base, tmp, ctx->two_n, ctx->numwords);
            modqx_modcpy(tmp, base, ctx->numwords);
        }
    }

    /* Step 5. If k is negative, negates the accumulated result modulo n. */
    if (k < 0) {
        spint zero[MODQLIMBS] = { 0 };
        modqx_modzer(zero, ctx->numwords);
        modqx_modsub(zero, acc, out, ctx->two_n, ctx->numwords);

        /* Step 6. Otherwise copies the accumulated result to the output. */
    } else {
        modqx_modcpy(acc, out, ctx->numwords);
    }
}

/**
 * Perform one Lucas-sequence doubling step on V and Q^k.
 */
static void
probprime_lucas_double_internal(spint V[MODQLIMBS], spint Qk[MODQLIMBS], const probprime_ctx_t *ctx)
{
    spint v2[MODQLIMBS], twoq[MODQLIMBS], q2[MODQLIMBS];

    modqx_modsqr(V, v2, ctx->n, ctx->ndash, ctx->numwords);
    modqx_modadd(Qk, Qk, twoq, ctx->two_n, ctx->numwords);
    modqx_modsub(v2, twoq, V, ctx->two_n, ctx->numwords);

    modqx_modsqr(Qk, q2, ctx->n, ctx->ndash, ctx->numwords);
    modqx_modcpy(q2, Qk, ctx->numwords);
}

/**
 * Perform the strong Lucas-Selfridge probable-prime test.
 *
 * This function performs:
 * Step 0. Rejects n if it is a perfect square.
 * Step 1. Searches the Lucas-Selfridge sequence D = 5, -7, 9, -11, ... until
 *         Jacobi(D/n) = -1, and computes Q = (1 - D) / 4.
 * Step 2. Computes n + 1 and decomposes it as d * 2^s.
 * Step 3. Initializes the Lucas sequence values U, V, Q^k and Q.
 * Step 4. Scans the bits of d from left to right.
 * Step 5. For every bit, performs the Lucas doubling step.
 * Step 6. If the current bit is 1, performs the Lucas addition step.
 * Step 7. Checks whether U_d == 0 modulo n.
 * Step 8. Checks whether one of V_d, V_2d, ..., V_(2^(s-1)d) is 0 modulo n.
 * Returns 1 if n passes the strong Lucas-Selfridge test.
 * Otherwise returns 0.
 */
static int
probprime_strong_lucas_internal(const ibz_t *n, const probprime_ctx_t *ctx)
{
    /* Step 0. Rejects n if it is a perfect square. */
    if (probprime_perfect_square_internal(n))
        return 0;

    /* Step 1. Searches the Lucas-Selfridge sequence D = 5, -7, 9, -11, ... until
       Jacobi(D/n) = -1, and computes Q = (1 - D) / 4. */
    digit_t Dabs = 5;
    sdigit_t Q = 0;

    for (;;) {
        digit_t r = qlapoty_mod_digit(n, Dabs);

        if (r == 0)
            return 0;

        int j = probprime_jacobi_u64_internal(r, Dabs);

        if (j == 0)
            return 0;

        if (j == -1) {
            Q = (Dabs & 2u) ? (sdigit_t)(Dabs >> 2) + 1 : -(sdigit_t)(Dabs >> 2);
            break;
        }

        Dabs += 2;

        if (Dabs > UINT32_C(1000001))
            return 0;
    }

    /* Step 2. Computes n + 1 and decomposes it as d * 2^s. */
    ibz_t np1 = { 0 }, d = { 0 };
    ibz_add(&np1, n, &ibz_const_one);
    int s = ibz_two_adic(&np1);
    ibz_div_2exp(&d, &np1, (uint32_t)s);

    /* Step 3. Initializes the Lucas sequence values U, V, Q^k and Q. */
    spint U[MODQLIMBS], V[MODQLIMBS];
    spint Qk[MODQLIMBS], Qmont[MODQLIMBS];
    spint zero[MODQLIMBS] = { 0 };

    modqx_modcpy(ctx->one, U, ctx->numwords);
    modqx_modcpy(ctx->one, V, ctx->numwords);

    probprime_scale_small_internal(ctx->one, Q, Qk, ctx);
    modqx_modcpy(Qk, Qmont, ctx->numwords);

    int nb = ibz_bitsize(&d);

    /* Step 4. Scans the bits of d from most significant to least significant. */
    for (int pos = nb - 2; pos >= 0; pos--) {
        spint U2[MODQLIMBS];

        /* Step 5. For every bit, performs the Lucas doubling step. */
        modqx_modmul(U, V, U2, ctx->n, ctx->ndash, ctx->numwords);
        probprime_lucas_double_internal(V, Qk, ctx);
        modqx_modcpy(U2, U, ctx->numwords);

        /* Step 6. If the current bit is 1, performs the Lucas addition step. */
        if (ibz_bit_at(&d, pos)) {
            spint oldU[MODQLIMBS], sum[MODQLIMBS], term[MODQLIMBS];
            spint vnew[MODQLIMBS], qnew[MODQLIMBS];

            modqx_modcpy(U, oldU, ctx->numwords);

            modqx_modadd(U, V, sum, ctx->two_n, ctx->numwords);
            modqx_modcpy(sum, U, ctx->numwords);
            probprime_half_mod_internal(U, ctx);

            probprime_scale_small_internal(oldU, -2 * Q, term, ctx);

            modqx_modadd(U, term, vnew, ctx->two_n, ctx->numwords);
            modqx_modcpy(vnew, V, ctx->numwords);

            modqx_modmul(Qk, Qmont, qnew, ctx->n, ctx->ndash, ctx->numwords);
            modqx_modcpy(qnew, Qk, ctx->numwords);
        }
    }

    /* Step 7. Checks whether U_d == 0 modulo n. */
    if (probprime_mont_equal_internal(U, zero, ctx))
        return 1;

    /* Step 8. Checks whether one of V_d, V_2d, ..., V_(2^(s-1)d)
       is 0 modulo n. */
    for (int r = 0; r < s; r++) {
        if (probprime_mont_equal_internal(V, zero, ctx))
            return 1;

        if (r + 1 < s)
            probprime_lucas_double_internal(V, Qk, ctx);
    }

    return 0;
}

/**
 * Execute the complete Case-1 probable prime procedure.
 * Strategy: small-prime sieve -> strong MR base 2 -> strong Lucas-Selfridge
 * -> max(reps-24,0) additional random strong-MR rounds.
 * R^2 is prepared once so each extra witness needs one Montgomery multiply.
 *
 * This function performs:
 * Step 0. Gets rid of the trivial cases n < 2, n = 2, n = 3, and even n.
 * Step 1. Selects the sieve bound and sliding-window size for the sqisign level.
 * Step 2. Applies  aggressive small prime sieving.
 * Step 3. Accepts a surviving small candidate if it is below the sieve bound.
 * Step 4. Initializes the Montgomery constants shared by Miller-Rabin and Lucas.
 * Step 5. Executes a  Miller-Rabin test with base 2.
 * Step 6. Executes the  Lucas-Selfridge probable-prime test.
 * Step 7. Computes the number of additional Miller-Rabin rounds as
 *         max(reps - 24, 0).
 * Step 8. Precomputes R^2 once for Montgomery encoding of the extra witnesses.
 * Step 9. Generates each random witness in the interval [2, n - 2].
 * Step 10. Converts the witness to MODQ representation and then to Montgomery
 *          representation using one multiplication by R^2.
 * Step 11. Executes one Miller-Rabin round for each additional witness.
 * Returns 1 if n passes the complete probable-prime procedure.
 * Otherwise returns 0.
 */
static int
probprime_bpsw_internal(const ibz_t *n, int reps)
{
    /* Step 0. Handles the trivial cases n < 2, n = 2, n = 3, and even n. */
    if (ibz_cmp(n, &ibz_const_two) < 0)
        return 0;

    if (ibz_cmp(n, &ibz_const_two) == 0 || ibz_cmp(n, &ibz_const_three) == 0)
        return 1;

    if (ibz_is_even(n))
        return 0;

    /* Step 1. Selects the sieve bound and sliding-window size for the sqisign level. */
    uint32_t sieve_bound;
    int window;

    if (MODQBITS < 1200) {
        sieve_bound = 509u;
        window = 4;
    } else if (MODQBITS < 1800) {
        sieve_bound = 1021u;
        window = 6;
    } else {
        sieve_bound = 509u;
        window = 6;
    }

    /* Step 2. Applies small-prime sieving. */
    if (!probprime_sieve_internal(n, sieve_bound))
        return 0;

    /* Step 3. Accepts a surviving small candidate if it is below
       the sieve bound. */
    if (ibz_bitsize(n) <= 32 && n->limbs[0] <= (digit_t)sieve_bound)
        return 1;

    /* Step 4. Initializes the Montgomery constants shared by Miller-Rabin and Lucas. */
    probprime_ctx_t ctx;

    if (!probprime_ctx_init_internal(&ctx, n))
        return 0;

    /* Step 5. Executes a Miller-Rabin test with base 2. */
    spint x[MODQLIMBS];
    modqx_modadd(ctx.one, ctx.one, x, ctx.two_n, ctx.numwords);

    if (!probprime_mr_round_internal(&ctx, x, window))
        return 0;

    /* Step 6. Executes the Lucas-Selfridge probable-prime test. */
    if (!probprime_strong_lucas_internal(n, &ctx))
        return 0;

    /* Step 7. Computes the number of additional Miller-Rabin rounds as
       max(reps - 24, 0). */
    int extra = reps > 24 ? reps - 24 : 0;

    if (extra == 0)
        return 1;

    /* Step 8. Precomputes R^2 once for Montgomery encoding of the extra witnesses. */
    spint r2[MODQLIMBS] = { 0 };
    modqx_nresx(ctx.one, r2, ctx.two_n, ctx.numwords);

    ibz_t n_m2 = { 0 };
    ibz_sub(&n_m2, n, &ibz_const_two);

    for (int r = 0; r < extra; r++) {
        ibz_t a = { 0 };

        /* Step 9. Generates each random witness in the interval [2, n - 2]. */
        if (!ibz_rand_interval(&a, &ibz_const_two, &n_m2))
            return 0;

        spint normal[MODQLIMBS] = { 0 };
        spint witness[MODQLIMBS] = { 0 };

        /* Step 10. Converts the witness to MODQ representation and then to Montgomery
           representation using one multiplication by R^2. */
        ibz_to_modq(normal, &a);
        modqx_modmul(normal, r2, witness, ctx.n, ctx.ndash, ctx.numwords);

        /* Step 11. Executes one  Miller-Rabin round for each additional witness. */
        if (!probprime_mr_round_internal(&ctx, witness, window))
            return 0;
    }

    return 1;
}

#endif
