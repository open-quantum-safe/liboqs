#include <signature.h>
#include <string.h>
#include <tutil.h>
#include <fp2.h>
#include <quaternion_data.h>
#include <torsion_constants.h>
#include <encoded_sizes.h>
#include <assert.h>

typedef unsigned char byte_t;

// digits

static void
encode_digits(byte_t *enc, const digit_t *x, size_t nbytes)
{
#ifdef TARGET_BIG_ENDIAN
    const size_t ndigits = nbytes / sizeof(digit_t);
    const size_t rem = nbytes % sizeof(digit_t);

    for (size_t i = 0; i < ndigits; i++)
        ((digit_t *)enc)[i] = BSWAP_DIGIT(x[i]);
    if (rem) {
        digit_t ld = BSWAP_DIGIT(x[ndigits]);
        memcpy(enc + ndigits * sizeof(digit_t), (byte_t *)&ld, rem);
    }
#else
    memcpy(enc, (const byte_t *)x, nbytes);
#endif
}

static void
decode_digits(digit_t *x, const byte_t *enc, size_t nbytes, size_t ndigits)
{
    assert(nbytes <= ndigits * sizeof(digit_t));
    memcpy((byte_t *)x, enc, nbytes);
    memset((byte_t *)x + nbytes, 0, ndigits * sizeof(digit_t) - nbytes);

#ifdef TARGET_BIG_ENDIAN
    for (size_t i = 0; i < ndigits; i++)
        x[i] = BSWAP_DIGIT(x[i]);
#endif
}

// ibz_t: only works for positive inputs

static byte_t *
sig_ibz_to_bytes(byte_t *enc, const ibz_t *x, size_t nbytes, bool sgn)
{
    (void)sgn;
#ifndef NDEBUG
    {
        // make sure there is enough space
        ibz_t abs, bnd;
        ibz_init(&bnd);
        ibz_init(&abs);
        ibz_mul_2exp(&bnd, &ibz_const_one, 8 * nbytes - sgn);
        ibz_abs(&abs, x);
        assert(ibz_cmp(&abs, &bnd) < 0);
    }
#endif
    const size_t digits = (nbytes + sizeof(digit_t) - 1) / sizeof(digit_t);
    digit_t d[digits];
    memset(d, 0, sizeof(d));
    assert(ibz_is_positive(x));
    assert(ibz_bitsize(x) < (int)(nbytes + sizeof(digit_t) - 1) * 8);
    // non-negative, straightforward.
    ibz_to_digits(d, x);
    encode_digits(enc, d, nbytes);
    return enc + nbytes;
}

static const byte_t *
sig_ibz_from_bytes(ibz_t *x, const byte_t *enc, size_t nbytes)
{
    assert(nbytes > 0);
    const size_t ndigits = (nbytes + sizeof(digit_t) - 1) / sizeof(digit_t);
    assert(ndigits > 0);
    digit_t d[ndigits];
    memset(d, 0, sizeof(d));
    decode_digits(d, enc, nbytes, ndigits);
    // non-negative
    ibz_copy_digits(x, d, ndigits);
    return enc + nbytes;
}

// public API

void
secret_key_to_bytes(byte_t *enc, const secret_key_t *sk, const public_key_t *pk)
{
#ifndef NDEBUG
    byte_t *const start = enc;
#endif

    enc = public_key_to_bytes(enc, pk);

#ifndef NDEBUG
    {
        fp2_t lhs, rhs;
        fp2_mul(&lhs, &sk->curve.A, &pk->curve.C);
        fp2_mul(&rhs, &sk->curve.C, &pk->curve.A);
        assert(fp2_is_equal(&lhs, &rhs));
    }
#endif

    enc = sig_ibz_to_bytes(enc, &sk->secret_ideal.norm, FP_ENCODED_BYTES, false);
    enc = sig_ibz_to_bytes(enc, &sk->secret_ideal.x, FP_ENCODED_BYTES, false);
    enc = sig_ibz_to_bytes(enc, &sk->secret_ideal.y, FP_ENCODED_BYTES, false);

    enc = sig_ibz_to_bytes(enc, &sk->mat_BAcan_to_BA0_two.m[0][0], CHALLENGE_BYTES, false);
    enc = sig_ibz_to_bytes(enc, &sk->mat_BAcan_to_BA0_two.m[0][1], CHALLENGE_BYTES, false);
    enc = sig_ibz_to_bytes(enc, &sk->mat_BAcan_to_BA0_two.m[1][0], CHALLENGE_BYTES, false);
    enc = sig_ibz_to_bytes(enc, &sk->mat_BAcan_to_BA0_two.m[1][1], CHALLENGE_BYTES, false);

    assert(enc - start == SECRETKEY_BYTES);
}

void
secret_key_from_bytes(secret_key_t *sk, public_key_t *pk, const byte_t *enc)
{
#ifndef NDEBUG
    const byte_t *const start = enc;
#endif

    enc = public_key_from_bytes(pk, enc);
    enc = sig_ibz_from_bytes(&sk->secret_ideal.norm, enc, FP_ENCODED_BYTES);
    enc = sig_ibz_from_bytes(&sk->secret_ideal.x, enc, FP_ENCODED_BYTES);
    enc = sig_ibz_from_bytes(&sk->secret_ideal.y, enc, FP_ENCODED_BYTES);

    enc = sig_ibz_from_bytes(&sk->mat_BAcan_to_BA0_two.m[0][0], enc, CHALLENGE_BYTES);
    enc = sig_ibz_from_bytes(&sk->mat_BAcan_to_BA0_two.m[0][1], enc, CHALLENGE_BYTES);
    enc = sig_ibz_from_bytes(&sk->mat_BAcan_to_BA0_two.m[1][0], enc, CHALLENGE_BYTES);
    enc = sig_ibz_from_bytes(&sk->mat_BAcan_to_BA0_two.m[1][1], enc, CHALLENGE_BYTES);

    assert(enc - start == SECRETKEY_BYTES);

    sk->curve = pk->curve;
}
