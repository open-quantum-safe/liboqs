#include <verification.h>
#include <string.h>
#include <tutil.h>
#include <fp2.h>
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

// fp2_t

static byte_t *
fp2_to_bytes(byte_t *enc, const fp2_t *x)
{
    fp2_encode(enc, x);
    return enc + FP2_ENCODED_BYTES;
}

static const byte_t *
fp2_from_bytes(fp2_t *x, const byte_t *enc)
{
    fp2_decode(x, enc);
    return enc + FP2_ENCODED_BYTES;
}

// curves and points

static byte_t *
proj_to_bytes(byte_t *enc, const fp2_t *x, const fp2_t *z)
{
    assert(!fp2_is_zero(z));
    fp2_t tmp = *z;
    fp2_inv(&tmp);
#ifndef NDEBUG
    {
        fp2_t chk;
        fp2_mul(&chk, z, &tmp);
        fp2_t one;
        fp2_set_one(&one);
        assert(fp2_is_equal(&chk, &one));
    }
#endif
    fp2_mul(&tmp, x, &tmp);
    enc = fp2_to_bytes(enc, &tmp);
    return enc;
}

static const byte_t *
proj_from_bytes(fp2_t *x, fp2_t *z, const byte_t *enc)
{
    enc = fp2_from_bytes(x, enc);
    fp2_set_one(z);
    return enc;
}

static byte_t *
ec_curve_to_bytes(byte_t *enc, const ec_curve_t *curve)
{
    return proj_to_bytes(enc, &curve->A, &curve->C);
}

static const byte_t *
ec_curve_from_bytes(ec_curve_t *curve, const byte_t *enc)
{
    memset(curve, 0, sizeof(*curve));
    return proj_from_bytes(&curve->A, &curve->C, enc);
}

static byte_t *
ec_point_to_bytes(byte_t *enc, const ec_point_t *point)
{
    return proj_to_bytes(enc, &point->x, &point->z);
}

static const byte_t *
ec_point_from_bytes(ec_point_t *point, const byte_t *enc)
{
    return proj_from_bytes(&point->x, &point->z, enc);
}

static byte_t *
ec_basis_to_bytes(byte_t *enc, const ec_basis_t *basis)
{
    enc = ec_point_to_bytes(enc, &basis->P);
    enc = ec_point_to_bytes(enc, &basis->Q);
    enc = ec_point_to_bytes(enc, &basis->PmQ);
    return enc;
}

static const byte_t *
ec_basis_from_bytes(ec_basis_t *basis, const byte_t *enc)
{
    enc = ec_point_from_bytes(&basis->P, enc);
    enc = ec_point_from_bytes(&basis->Q, enc);
    enc = ec_point_from_bytes(&basis->PmQ, enc);
    return enc;
}

// public API

byte_t *
public_key_to_bytes(byte_t *enc, const public_key_t *pk)
{
#ifndef NDEBUG
    const byte_t *const start = enc;
#endif
    enc = ec_curve_to_bytes(enc, &pk->curve);
    *enc++ = pk->hint_pk;
    assert(enc - start == PUBLICKEY_BYTES);
    return enc;
}

const byte_t *
public_key_from_bytes(public_key_t *pk, const byte_t *enc)
{
#ifndef NDEBUG
    const byte_t *const start = enc;
#endif
    enc = ec_curve_from_bytes(&pk->curve, enc);
    pk->hint_pk = *enc++;
    assert(enc - start == PUBLICKEY_BYTES);
    return enc;
}

void
signature_to_bytes(byte_t *enc, const signature_t *sig)
{
#ifndef NDEBUG
    byte_t *const start = enc;
#endif

    enc = fp2_to_bytes(enc, &sig->E_aux_A);

    *enc++ = sig->backtracking;
    *enc++ = sig->two_resp_length;

    size_t nbytes = (SQIsign_response_length + 9) / 8;
    encode_digits(enc, sig->mat_Bchall_can_to_B_chall[0][0], nbytes);
    enc += nbytes;
    encode_digits(enc, sig->mat_Bchall_can_to_B_chall[0][1], nbytes);
    enc += nbytes;
    encode_digits(enc, sig->mat_Bchall_can_to_B_chall[1][0], nbytes);
    enc += nbytes;
    encode_digits(enc, sig->mat_Bchall_can_to_B_chall[1][1], nbytes);
    enc += nbytes;

    nbytes = SECURITY_BITS / 8;
    encode_digits(enc, sig->chall_coeff, nbytes);
    enc += nbytes;

    *enc++ = sig->hint_aux;
    *enc++ = sig->hint_chall;

    assert(enc - start == SIGNATURE_BYTES);
}

void
signature_from_bytes(signature_t *sig, const byte_t *enc)
{
#ifndef NDEBUG
    const byte_t *const start = enc;
#endif

    enc = fp2_from_bytes(&sig->E_aux_A, enc);

    sig->backtracking = *enc++;
    sig->two_resp_length = *enc++;

    size_t nbytes = (SQIsign_response_length + 9) / 8;
    decode_digits(sig->mat_Bchall_can_to_B_chall[0][0], enc, nbytes, NWORDS_ORDER);
    enc += nbytes;
    decode_digits(sig->mat_Bchall_can_to_B_chall[0][1], enc, nbytes, NWORDS_ORDER);
    enc += nbytes;
    decode_digits(sig->mat_Bchall_can_to_B_chall[1][0], enc, nbytes, NWORDS_ORDER);
    enc += nbytes;
    decode_digits(sig->mat_Bchall_can_to_B_chall[1][1], enc, nbytes, NWORDS_ORDER);
    enc += nbytes;

    nbytes = SECURITY_BITS / 8;
    decode_digits(sig->chall_coeff, enc, nbytes, NWORDS_ORDER);
    enc += nbytes;

    sig->hint_aux = *enc++;
    sig->hint_chall = *enc++;

    assert(enc - start == SIGNATURE_BYTES);
}
