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

// ibz_t

static byte_t *
ibz_to_bytes(byte_t *enc, const ibz_t *x, size_t nbytes, bool sgn)
{
#ifndef NDEBUG
    {
        // make sure there is enough space
        ibz_t abs, bnd;
        ibz_init(&bnd);
        ibz_init(&abs);
        ibz_pow(&bnd, &ibz_const_two, 8 * nbytes - sgn);
        ibz_abs(&abs, x);
        assert(ibz_cmp(&abs, &bnd) < 0);
        ibz_finalize(&bnd);
        ibz_finalize(&abs);
    }
#endif
    const size_t digits = (nbytes + sizeof(digit_t) - 1) / sizeof(digit_t);
    digit_t d[digits];
    memset(d, 0, sizeof(d));
    if (ibz_cmp(x, &ibz_const_zero) >= 0) {
        // non-negative, straightforward.
        ibz_to_digits(d, x);
    } else {
        assert(sgn);
        // negative; use two's complement.
        ibz_t tmp;
        ibz_init(&tmp);
        ibz_neg(&tmp, x);
        ibz_sub(&tmp, &tmp, &ibz_const_one);
        ibz_to_digits(d, &tmp);
        for (size_t i = 0; i < digits; ++i)
            d[i] = ~d[i];
#ifndef NDEBUG
        {
            // make sure the result is correct
            ibz_t chk;
            ibz_init(&chk);
            ibz_copy_digit_array(&tmp, d);
            ibz_sub(&tmp, &tmp, x);
            ibz_pow(&chk, &ibz_const_two, 8 * sizeof(d));
            assert(!ibz_cmp(&tmp, &chk));
            ibz_finalize(&chk);
        }
#endif
        ibz_finalize(&tmp);
    }
    encode_digits(enc, d, nbytes);
    return enc + nbytes;
}

static const byte_t *
ibz_from_bytes(ibz_t *x, const byte_t *enc, size_t nbytes, bool sgn)
{
    assert(nbytes > 0);
    const size_t ndigits = (nbytes + sizeof(digit_t) - 1) / sizeof(digit_t);
    assert(ndigits > 0);
    digit_t d[ndigits];
    memset(d, 0, sizeof(d));
    decode_digits(d, enc, nbytes, ndigits);
    if (sgn && enc[nbytes - 1] >> 7) {
        // negative, decode two's complement
        const size_t s = sizeof(digit_t) - 1 - (sizeof(d) - nbytes);
        assert(s < sizeof(digit_t));
        d[ndigits - 1] |= ((digit_t)-1) >> 8 * s << 8 * s;
        for (size_t i = 0; i < ndigits; ++i)
            d[i] = ~d[i];
        ibz_copy_digits(x, d, ndigits);
        ibz_add(x, x, &ibz_const_one);
        ibz_neg(x, x);
    } else {
        // non-negative
        ibz_copy_digits(x, d, ndigits);
    }
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

    enc = ibz_to_bytes(enc, &sk->secret_ideal.norm, FP_ENCODED_BYTES, false);
    {
        quat_alg_elem_t gen;
        quat_alg_elem_init(&gen);
        int ret UNUSED = quat_lideal_generator(&gen, &sk->secret_ideal, &QUATALG_PINFTY);
        assert(ret);
        // we skip encoding the denominator since it won't change the generated ideal
#ifndef NDEBUG
        {
            // let's make sure that the denominator is indeed coprime to the norm of the ideal
            ibz_t gcd;
            ibz_init(&gcd);
            ibz_gcd(&gcd, &gen.denom, &sk->secret_ideal.norm);
            assert(!ibz_cmp(&gcd, &ibz_const_one));
            ibz_finalize(&gcd);
        }
#endif
        enc = ibz_to_bytes(enc, &gen.coord[0], FP_ENCODED_BYTES, true);
        enc = ibz_to_bytes(enc, &gen.coord[1], FP_ENCODED_BYTES, true);
        enc = ibz_to_bytes(enc, &gen.coord[2], FP_ENCODED_BYTES, true);
        enc = ibz_to_bytes(enc, &gen.coord[3], FP_ENCODED_BYTES, true);
        quat_alg_elem_finalize(&gen);
    }

    enc = ibz_to_bytes(enc, &sk->mat_BAcan_to_BA0_two[0][0], TORSION_2POWER_BYTES, false);
    enc = ibz_to_bytes(enc, &sk->mat_BAcan_to_BA0_two[0][1], TORSION_2POWER_BYTES, false);
    enc = ibz_to_bytes(enc, &sk->mat_BAcan_to_BA0_two[1][0], TORSION_2POWER_BYTES, false);
    enc = ibz_to_bytes(enc, &sk->mat_BAcan_to_BA0_two[1][1], TORSION_2POWER_BYTES, false);

    assert(enc - start == SECRETKEY_BYTES);
}

void
secret_key_from_bytes(secret_key_t *sk, public_key_t *pk, const byte_t *enc)
{
#ifndef NDEBUG
    const byte_t *const start = enc;
#endif

    enc = public_key_from_bytes(pk, enc);

    {
        ibz_t norm;
        ibz_init(&norm);
        quat_alg_elem_t gen;
        quat_alg_elem_init(&gen);
        enc = ibz_from_bytes(&norm, enc, FP_ENCODED_BYTES, false);
        enc = ibz_from_bytes(&gen.coord[0], enc, FP_ENCODED_BYTES, true);
        enc = ibz_from_bytes(&gen.coord[1], enc, FP_ENCODED_BYTES, true);
        enc = ibz_from_bytes(&gen.coord[2], enc, FP_ENCODED_BYTES, true);
        enc = ibz_from_bytes(&gen.coord[3], enc, FP_ENCODED_BYTES, true);
        quat_lideal_create(&sk->secret_ideal, &gen, &norm, &MAXORD_O0, &QUATALG_PINFTY);
        ibz_finalize(&norm);
        quat_alg_elem_finalize(&gen);
    }

    enc = ibz_from_bytes(&sk->mat_BAcan_to_BA0_two[0][0], enc, TORSION_2POWER_BYTES, false);
    enc = ibz_from_bytes(&sk->mat_BAcan_to_BA0_two[0][1], enc, TORSION_2POWER_BYTES, false);
    enc = ibz_from_bytes(&sk->mat_BAcan_to_BA0_two[1][0], enc, TORSION_2POWER_BYTES, false);
    enc = ibz_from_bytes(&sk->mat_BAcan_to_BA0_two[1][1], enc, TORSION_2POWER_BYTES, false);

    assert(enc - start == SECRETKEY_BYTES);

    sk->curve = pk->curve;
    ec_curve_to_basis_2f_from_hint(&sk->canonical_basis, &sk->curve, TORSION_EVEN_POWER, pk->hint_pk);
}
