#include "intbig_internal.h"
#include <limits.h>
#include <rng.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

// #define DEBUG_VERBOSE

#ifdef DEBUG_VERBOSE
#define DEBUG_STR_PRINTF(x) printf("%s\n", (x));

static void
DEBUG_STR_FUN_INT_MP(const char *op, int arg1, const ibz_t *arg2)
{
    int arg2_size = ibz_size_in_base(arg2, 16);
    char arg2_str[arg2_size + 2];
    ibz_convert_to_str(arg2, arg2_str, 16);

    printf("%s,%x,%s\n", op, arg1, arg2_str);
}

static void
DEBUG_STR_FUN_3(const char *op, const ibz_t *arg1, const ibz_t *arg2, const ibz_t *arg3)
{
    int arg1_size = ibz_size_in_base(arg1, 16);
    char arg1_str[arg1_size + 2];
    ibz_convert_to_str(arg1, arg1_str, 16);

    int arg2_size = ibz_size_in_base(arg2, 16);
    char arg2_str[arg2_size + 2];
    ibz_convert_to_str(arg2, arg2_str, 16);

    int arg3_size = ibz_size_in_base(arg3, 16);
    char arg3_str[arg3_size + 2];
    ibz_convert_to_str(arg3, arg3_str, 16);

    printf("%s,%s,%s,%s\n", op, arg1_str, arg2_str, arg3_str);
}

static void
DEBUG_STR_FUN_MP2_INT(const char *op, const ibz_t *arg1, const ibz_t *arg2, int arg3)
{
    int arg1_size = ibz_size_in_base(arg1, 16);
    char arg1_str[arg1_size + 2];
    ibz_convert_to_str(arg1, arg1_str, 16);

    int arg2_size = ibz_size_in_base(arg2, 16);
    char arg2_str[arg2_size + 2];
    ibz_convert_to_str(arg2, arg2_str, 16);

    printf("%s,%s,%s,%x\n", op, arg1_str, arg2_str, arg3);
}

static void
DEBUG_STR_FUN_INT_MP2(const char *op, int arg1, const ibz_t *arg2, const ibz_t *arg3)
{
    int arg2_size = ibz_size_in_base(arg2, 16);
    char arg2_str[arg2_size + 2];
    ibz_convert_to_str(arg2, arg2_str, 16);

    int arg3_size = ibz_size_in_base(arg3, 16);
    char arg3_str[arg3_size + 2];
    ibz_convert_to_str(arg3, arg3_str, 16);

    if (arg1 >= 0)
        printf("%s,%x,%s,%s\n", op, arg1, arg2_str, arg3_str);
    else
        printf("%s,-%x,%s,%s\n", op, -arg1, arg2_str, arg3_str);
}

static void
DEBUG_STR_FUN_INT_MP_INT(const char *op, int arg1, const ibz_t *arg2, int arg3)
{
    int arg2_size = ibz_size_in_base(arg2, 16);
    char arg2_str[arg2_size + 2];
    ibz_convert_to_str(arg2, arg2_str, 16);

    printf("%s,%x,%s,%x\n", op, arg1, arg2_str, arg3);
}

static void
DEBUG_STR_FUN_4(const char *op, const ibz_t *arg1, const ibz_t *arg2, const ibz_t *arg3, const ibz_t *arg4)
{
    int arg1_size = ibz_size_in_base(arg1, 16);
    char arg1_str[arg1_size + 2];
    ibz_convert_to_str(arg1, arg1_str, 16);

    int arg2_size = ibz_size_in_base(arg2, 16);
    char arg2_str[arg2_size + 2];
    ibz_convert_to_str(arg2, arg2_str, 16);

    int arg3_size = ibz_size_in_base(arg3, 16);
    char arg3_str[arg3_size + 2];
    ibz_convert_to_str(arg3, arg3_str, 16);

    int arg4_size = ibz_size_in_base(arg4, 16);
    char arg4_str[arg4_size + 2];
    ibz_convert_to_str(arg4, arg4_str, 16);

    printf("%s,%s,%s,%s,%s\n", op, arg1_str, arg2_str, arg3_str, arg4_str);
}
#else
#define DEBUG_STR_PRINTF(x)
#define DEBUG_STR_FUN_INT_MP(op, arg1, arg2)
#define DEBUG_STR_FUN_3(op, arg1, arg2, arg3)
#define DEBUG_STR_FUN_INT_MP2(op, arg1, arg2, arg3)
#define DEBUG_STR_FUN_INT_MP_INT(op, arg1, arg2, arg3)
#define DEBUG_STR_FUN_4(op, arg1, arg2, arg3, arg4)
#endif

/** @defgroup ibz_t Constants
 * @{
 */

const __mpz_struct ibz_const_zero[1] = {
    {
     ._mp_alloc = 0,
     ._mp_size = 0,
     ._mp_d = (mp_limb_t[]){ 0 },
     }
};

const __mpz_struct ibz_const_one[1] = {
    {
     ._mp_alloc = 0,
     ._mp_size = 1,
     ._mp_d = (mp_limb_t[]){ 1 },
     }
};

const __mpz_struct ibz_const_two[1] = {
    {
     ._mp_alloc = 0,
     ._mp_size = 1,
     ._mp_d = (mp_limb_t[]){ 2 },
     }
};

const __mpz_struct ibz_const_three[1] = {
    {
     ._mp_alloc = 0,
     ._mp_size = 1,
     ._mp_d = (mp_limb_t[]){ 3 },
     }
};

void
ibz_init(ibz_t *x)
{
    mpz_init(*x);
}

void
ibz_finalize(ibz_t *x)
{
    mpz_clear(*x);
}

void
ibz_add(ibz_t *sum, const ibz_t *a, const ibz_t *b)
{
#ifdef DEBUG_VERBOSE
    ibz_t a_cp, b_cp;
    ibz_init(&a_cp);
    ibz_init(&b_cp);
    ibz_copy(&a_cp, a);
    ibz_copy(&b_cp, b);
#endif
    mpz_add(*sum, *a, *b);
#ifdef DEBUG_VERBOSE
    DEBUG_STR_FUN_3("ibz_add", sum, &a_cp, &b_cp);
    ibz_finalize(&a_cp);
    ibz_finalize(&b_cp);
#endif
}

void
ibz_sub(ibz_t *diff, const ibz_t *a, const ibz_t *b)
{
#ifdef DEBUG_VERBOSE
    ibz_t a_cp, b_cp;
    ibz_init(&a_cp);
    ibz_init(&b_cp);
    ibz_copy(&a_cp, a);
    ibz_copy(&b_cp, b);
#endif
    mpz_sub(*diff, *a, *b);

#ifdef DEBUG_VERBOSE
    DEBUG_STR_FUN_3("ibz_sub", diff, &a_cp, &b_cp);
    ibz_finalize(&a_cp);
    ibz_finalize(&b_cp);
#endif
}

void
ibz_mul(ibz_t *prod, const ibz_t *a, const ibz_t *b)
{
#ifdef DEBUG_VERBOSE
    ibz_t a_cp, b_cp;
    ibz_init(&a_cp);
    ibz_init(&b_cp);
    ibz_copy(&a_cp, a);
    ibz_copy(&b_cp, b);
#endif
    mpz_mul(*prod, *a, *b);
#ifdef DEBUG_VERBOSE
    DEBUG_STR_FUN_3("ibz_mul", prod, &a_cp, &b_cp);
    ibz_finalize(&a_cp);
    ibz_finalize(&b_cp);
#endif
}

void
ibz_neg(ibz_t *neg, const ibz_t *a)
{
    mpz_neg(*neg, *a);
}

void
ibz_abs(ibz_t *abs, const ibz_t *a)
{
    mpz_abs(*abs, *a);
}

void
ibz_div(ibz_t *quotient, ibz_t *remainder, const ibz_t *a, const ibz_t *b)
{
#ifdef DEBUG_VERBOSE
    ibz_t a_cp, b_cp;
    ibz_init(&a_cp);
    ibz_init(&b_cp);
    ibz_copy(&a_cp, a);
    ibz_copy(&b_cp, b);
#endif
    mpz_tdiv_qr(*quotient, *remainder, *a, *b);
#ifdef DEBUG_VERBOSE
    DEBUG_STR_FUN_4("ibz_div", quotient, remainder, &a_cp, &b_cp);
    ibz_finalize(&a_cp);
    ibz_finalize(&b_cp);
#endif
}

void
ibz_div_2exp(ibz_t *quotient, const ibz_t *a, uint32_t exp)
{
#ifdef DEBUG_VERBOSE
    ibz_t a_cp;
    ibz_init(&a_cp);
    ibz_copy(&a_cp, a);
#endif
    mpz_tdiv_q_2exp(*quotient, *a, exp);
#ifdef DEBUG_VERBOSE
    DEBUG_STR_FUN_MP2_INT("ibz_div_2exp,%Zx,%Zx,%x\n", quotient, &a_cp, exp);
    ibz_finalize(&a_cp);
#endif
}

void
ibz_div_floor(ibz_t *q, ibz_t *r, const ibz_t *n, const ibz_t *d)
{
    mpz_fdiv_qr(*q, *r, *n, *d);
}

void
ibz_mod(ibz_t *r, const ibz_t *a, const ibz_t *b)
{
    mpz_mod(*r, *a, *b);
}

unsigned long int
ibz_mod_ui(const mpz_t *n, unsigned long int d)
{
    return mpz_fdiv_ui(*n, d);
}

int
ibz_divides(const ibz_t *a, const ibz_t *b)
{
    return mpz_divisible_p(*a, *b);
}

void
ibz_pow(ibz_t *pow, const ibz_t *x, uint32_t e)
{
    mpz_pow_ui(*pow, *x, e);
}

void
ibz_pow_mod(ibz_t *pow, const ibz_t *x, const ibz_t *e, const ibz_t *m)
{
    mpz_powm(*pow, *x, *e, *m);
    DEBUG_STR_FUN_4("ibz_pow_mod", pow, x, e, m);
}

int
ibz_two_adic(ibz_t *pow)
{
    return mpz_scan1(*pow, 0);
}

int
ibz_cmp(const ibz_t *a, const ibz_t *b)
{
    int ret = mpz_cmp(*a, *b);
    DEBUG_STR_FUN_INT_MP2("ibz_cmp", ret, a, b);
    return ret;
}

int
ibz_is_zero(const ibz_t *x)
{
    int ret = !mpz_cmp_ui(*x, 0);
    DEBUG_STR_FUN_INT_MP("ibz_is_zero", ret, x);
    return ret;
}

int
ibz_is_one(const ibz_t *x)
{
    int ret = !mpz_cmp_ui(*x, 1);
    DEBUG_STR_FUN_INT_MP("ibz_is_one", ret, x);
    return ret;
}

int
ibz_cmp_int32(const ibz_t *x, int32_t y)
{
    int ret = mpz_cmp_si(*x, (signed long int)y);
    DEBUG_STR_FUN_INT_MP_INT("ibz_cmp_int32", ret, x, y);
    return ret;
}

int
ibz_is_even(const ibz_t *x)
{
    int ret = !mpz_tstbit(*x, 0);
    DEBUG_STR_FUN_INT_MP("ibz_is_even", ret, x);
    return ret;
}

int
ibz_is_odd(const ibz_t *x)
{
    int ret = mpz_tstbit(*x, 0);
    DEBUG_STR_FUN_INT_MP("ibz_is_odd", ret, x);
    return ret;
}

void
ibz_set(ibz_t *i, int32_t x)
{
    mpz_set_si(*i, x);
}

int
ibz_convert_to_str(const ibz_t *i, char *str, int base)
{
    if (!str || (base != 10 && base != 16))
        return 0;

    mpz_get_str(str, base, *i);

    return 1;
}

void
ibz_print(const ibz_t *num, int base)
{
    assert(base == 10 || base == 16);

    int num_size = ibz_size_in_base(num, base);
    char num_str[num_size + 2];
    ibz_convert_to_str(num, num_str, base);
    printf("%s", num_str);
}

int
ibz_set_from_str(ibz_t *i, const char *str, int base)
{
    return (1 + mpz_set_str(*i, str, base));
}

void
ibz_copy(ibz_t *target, const ibz_t *value)
{
    mpz_set(*target, *value);
}

void
ibz_swap(ibz_t *a, ibz_t *b)
{
    mpz_swap(*a, *b);
}

int32_t
ibz_get(const ibz_t *i)
{
#if LONG_MAX == INT32_MAX
    return (int32_t)mpz_get_si(*i);
#elif LONG_MAX > INT32_MAX
    // Extracts the sign bit and the 31 least significant bits
    signed long int t = mpz_get_si(*i);
    return (int32_t)((t >> (sizeof(signed long int) * 8 - 32)) & INT32_C(0x80000000)) | (t & INT32_C(0x7FFFFFFF));
#else
#error Unsupported configuration: LONG_MAX must be >= INT32_MAX
#endif
}

int
ibz_rand_interval(ibz_t *rand, const ibz_t *a, const ibz_t *b)
{
    int randret;
    int ret = 1;
    mpz_t tmp;
    mpz_t bmina;
    mpz_init(bmina);
    mpz_sub(bmina, *b, *a);

    if (mpz_sgn(bmina) == 0) {
        mpz_set(*rand, *a);
        mpz_clear(bmina);
        return 1;
    }

    size_t len_bits = mpz_sizeinbase(bmina, 2);
    size_t len_bytes = (len_bits + 7) / 8;
    size_t sizeof_limb = sizeof(mp_limb_t);
    size_t sizeof_limb_bits = sizeof_limb * 8;
    size_t len_limbs = (len_bytes + sizeof_limb - 1) / sizeof_limb;

    mp_limb_t mask = ((mp_limb_t)-1) >> (sizeof_limb_bits - len_bits) % sizeof_limb_bits;
    mp_limb_t r[len_limbs];

#ifndef NDEBUG
    {
        for (size_t i = 0; i < len_limbs; ++i)
            r[i] = (mp_limb_t)-1;
        r[len_limbs - 1] = mask;
        mpz_t check;
        mpz_roinit_n(check, r, len_limbs);
        assert(mpz_cmp(check, bmina) >= 0); // max sampled value >= b - a
        mpz_t bmina2;
        mpz_init(bmina2);
        mpz_add(bmina2, bmina, bmina);
        assert(mpz_cmp(check, bmina2) < 0); // max sampled value < 2 * (b - a)
        mpz_clear(bmina2);
    }
#endif

    do {
        randret = randombytes((unsigned char *)r, len_bytes);
        if (randret != 0) {
            ret = 0;
            goto err;
        }
#ifdef TARGET_BIG_ENDIAN
        for (size_t i = 0; i < len_limbs; ++i)
            r[i] = BSWAP_DIGIT(r[i]);
#endif
        r[len_limbs - 1] &= mask;
        mpz_roinit_n(tmp, r, len_limbs);
        if (mpz_cmp(tmp, bmina) <= 0)
            break;
    } while (1);

    mpz_add(*rand, tmp, *a);
err:
    mpz_clear(bmina);
    return ret;
}

int
ibz_rand_interval_i(ibz_t *rand, int32_t a, int32_t b)
{
    uint32_t diff, mask;
    int32_t rand32;

    if (!(a >= 0 && b >= 0 && b > a)) {
        printf("a = %d b = %d\n", a, b);
    }
    assert(a >= 0 && b >= 0 && b > a);

    diff = b - a;

    // Create a mask with 1 + ceil(log2(diff)) least significant bits set
#if (defined(__GNUC__) || defined(__clang__)) && INT_MAX == INT32_MAX
    mask = (1 << (32 - __builtin_clz((uint32_t)diff))) - 1;
#else
    uint32_t diff2 = diff, tmp;

    mask = (diff2 > 0xFFFF) << 4;
    diff2 >>= mask;

    tmp = (diff2 > 0xFF) << 3;
    diff2 >>= tmp;
    mask |= tmp;

    tmp = (diff2 > 0xF) << 2;
    diff2 >>= tmp;
    mask |= tmp;

    tmp = (diff2 > 0x3) << 1;
    diff2 >>= tmp;
    mask |= tmp;

    mask |= diff2 >> 1;

    mask = (1 << (mask + 1)) - 1;
#endif

    assert(mask >= diff && mask < 2 * diff);

    // Rejection sampling
    do {
        randombytes((unsigned char *)&rand32, sizeof(rand32));

#ifdef TARGET_BIG_ENDIAN
        rand32 = BSWAP32(rand32);
#endif

        rand32 &= mask;
    } while (rand32 > (int32_t)diff);

    rand32 += a;
    ibz_set(rand, rand32);

    return 1;
}

int
ibz_rand_interval_minm_m(ibz_t *rand, int32_t m)
{
    int ret = 1;
    mpz_t m_big;

    // m_big = 2 * m
    mpz_init_set_si(m_big, m);
    mpz_add(m_big, m_big, m_big);

    // Sample in [0, 2*m]
    ret = ibz_rand_interval(rand, &ibz_const_zero, &m_big);

    // Adjust to range [-m, m]
    mpz_sub_ui(*rand, *rand, m);

    mpz_clear(m_big);

    return ret;
}

int
ibz_rand_interval_bits(ibz_t *rand, uint32_t m)
{
    int ret = 1;
    mpz_t tmp;
    mpz_t low;
    mpz_init_set_ui(tmp, 1);
    mpz_mul_2exp(tmp, tmp, m);
    mpz_init(low);
    mpz_neg(low, tmp);
    ret = ibz_rand_interval(rand, &low, &tmp);
    mpz_clear(tmp);
    mpz_clear(low);
    if (ret != 1)
        goto err;
    mpz_sub_ui(*rand, *rand, (unsigned long int)m);
    return ret;
err:
    mpz_clear(tmp);
    mpz_clear(low);
    return ret;
}

int
ibz_bitsize(const ibz_t *a)
{
    return (int)mpz_sizeinbase(*a, 2);
}

int
ibz_size_in_base(const ibz_t *a, int base)
{
    return (int)mpz_sizeinbase(*a, base);
}

void
ibz_copy_digits(ibz_t *target, const digit_t *dig, int dig_len)
{
    mpz_import(*target, dig_len, -1, sizeof(digit_t), 0, 0, dig);
}

void
ibz_to_digits(digit_t *target, const ibz_t *ibz)
{
    // From the GMP documentation:
    // "If op is zero then the count returned will be zero and nothing written to rop."
    // The next line ensures zero is written to the first limb of target if ibz is zero;
    // target is then overwritten by the actual value if it is not.
    target[0] = 0;
    mpz_export(target, NULL, -1, sizeof(digit_t), 0, 0, *ibz);
}

int
ibz_probab_prime(const ibz_t *n, int reps)
{
    int ret = mpz_probab_prime_p(*n, reps);
    DEBUG_STR_FUN_INT_MP_INT("ibz_probab_prime", ret, n, reps);
    return ret;
}

void
ibz_gcd(ibz_t *gcd, const ibz_t *a, const ibz_t *b)
{
    mpz_gcd(*gcd, *a, *b);
}

int
ibz_invmod(ibz_t *inv, const ibz_t *a, const ibz_t *mod)
{
    return (mpz_invert(*inv, *a, *mod) ? 1 : 0);
}

int
ibz_legendre(const ibz_t *a, const ibz_t *p)
{
    return mpz_legendre(*a, *p);
}

int
ibz_sqrt(ibz_t *sqrt, const ibz_t *a)
{
    if (mpz_perfect_square_p(*a)) {
        mpz_sqrt(*sqrt, *a);
        return 1;
    } else {
        return 0;
    }
}

void
ibz_sqrt_floor(ibz_t *sqrt, const ibz_t *a)
{
    mpz_sqrt(*sqrt, *a);
}

int
ibz_sqrt_mod_p(ibz_t *sqrt, const ibz_t *a, const ibz_t *p)
{
#ifndef NDEBUG
    assert(ibz_probab_prime(p, 100));
#endif
    // Case a = 0
    {
        ibz_t test;
        ibz_init(&test);
        ibz_mod(&test, a, p);
        if (ibz_is_zero(&test)) {
            ibz_set(sqrt, 0);
        }
        ibz_finalize(&test);
    }
#ifdef DEBUG_VERBOSE
    ibz_t a_cp, p_cp;
    ibz_init(&a_cp);
    ibz_init(&p_cp);
    ibz_copy(&a_cp, a);
    ibz_copy(&p_cp, p);
#endif

    mpz_t amod, tmp, exp, a4, a2, q, z, qnr, x, y, b, pm1;
    mpz_init(amod);
    mpz_init(tmp);
    mpz_init(exp);
    mpz_init(a4);
    mpz_init(a2);
    mpz_init(q);
    mpz_init(z);
    mpz_init(qnr);
    mpz_init(x);
    mpz_init(y);
    mpz_init(b);
    mpz_init(pm1);

    int ret = 1;

    mpz_mod(amod, *a, *p);
    if (mpz_cmp_ui(amod, 0) < 0) {
        mpz_add(amod, *p, amod);
    }

    if (mpz_legendre(amod, *p) != 1) {
        ret = 0;
        goto end;
    }

    mpz_sub_ui(pm1, *p, 1);

    if (mpz_mod_ui(tmp, *p, 4) == 3) {
        // p % 4 == 3
        mpz_add_ui(tmp, *p, 1);
        mpz_fdiv_q_2exp(tmp, tmp, 2);
        mpz_powm(*sqrt, amod, tmp, *p);
    } else if (mpz_mod_ui(tmp, *p, 8) == 5) {
        // p % 8 == 5
        mpz_sub_ui(tmp, *p, 1);
        mpz_fdiv_q_2exp(tmp, tmp, 2);
        mpz_powm(tmp, amod, tmp, *p); // a^{(p-1)/4} mod p
        if (!mpz_cmp_ui(tmp, 1)) {
            mpz_add_ui(tmp, *p, 3);
            mpz_fdiv_q_2exp(tmp, tmp, 3);
            mpz_powm(*sqrt, amod, tmp, *p); // a^{(p+3)/8} mod p
        } else {
            mpz_sub_ui(tmp, *p, 5);
            mpz_fdiv_q_2exp(tmp, tmp, 3); // (p - 5) / 8
            mpz_mul_2exp(a4, amod, 2);    // 4*a
            mpz_powm(tmp, a4, tmp, *p);

            mpz_mul_2exp(a2, amod, 1);
            mpz_mul(tmp, a2, tmp);
            mpz_mod(*sqrt, tmp, *p);
        }
    } else {
        // p % 8 == 1 -> Shanks-Tonelli
        int e = 0;
        mpz_sub_ui(q, *p, 1);
        while (mpz_tstbit(q, e) == 0)
            e++;
        mpz_fdiv_q_2exp(q, q, e);

        // 1. find generator - non-quadratic residue
        mpz_set_ui(qnr, 2);
        while (mpz_legendre(qnr, *p) != -1)
            mpz_add_ui(qnr, qnr, 1);
        mpz_powm(z, qnr, q, *p);

        // 2. Initialize
        mpz_set(y, z);
        mpz_powm(y, amod, q, *p); // y = a^q mod p

        mpz_add_ui(tmp, q, 1); // tmp = (q + 1) / 2
        mpz_fdiv_q_2exp(tmp, tmp, 1);

        mpz_powm(x, amod, tmp, *p); // x = a^(q + 1)/2 mod p

        mpz_set_ui(exp, 1);
        mpz_mul_2exp(exp, exp, e - 2);

        for (int i = 0; i < e; ++i) {
            mpz_powm(b, y, exp, *p);

            if (!mpz_cmp(b, pm1)) {
                mpz_mul(x, x, z);
                mpz_mod(x, x, *p);

                mpz_mul(y, y, z);
                mpz_mul(y, y, z);
                mpz_mod(y, y, *p);
            }

            mpz_powm_ui(z, z, 2, *p);
            mpz_fdiv_q_2exp(exp, exp, 1);
        }

        mpz_set(*sqrt, x);
    }

#ifdef DEBUG_VERBOSE
    DEBUG_STR_FUN_3("ibz_sqrt_mod_p", sqrt, &a_cp, &p_cp);
    ibz_finalize(&a_cp);
    ibz_finalize(&p_cp);
#endif

end:
    mpz_clear(amod);
    mpz_clear(tmp);
    mpz_clear(exp);
    mpz_clear(a4);
    mpz_clear(a2);
    mpz_clear(q);
    mpz_clear(z);
    mpz_clear(qnr);
    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(b);
    mpz_clear(pm1);

    return ret;
}
