/** ThreeBears reference implementation */
#include "api.h"
#include "melas_fec.h"
#include "params.h"
#include "ring.h"
#include "sp800-185.h"
#include "threebears.h"

#define FEC_BYTES ((FEC_BITS+7)/8)
#define ENC_BITS  (ENC_SEED_BYTES*8 + FEC_BITS)

enum { HASH_PURPOSE_UNIFORM = 0, HASH_PURPOSE_KEYGEN = 1, HASH_PURPOSE_ENCAPS = 2, HASH_PURPOSE_PRF = 3 };

/** Initialize the hash function with a given purpose */
static void threebears_hash_init(
    shake256incctx *ctx,
    uint8_t purpose
) {
    const unsigned char S[] = "ThreeBears";
    const uint8_t pblock[15] = {
        VERSION, PRIVATE_KEY_BYTES, MATRIX_SEED_BYTES, ENC_SEED_BYTES,
        IV_BYTES, SHARED_SECRET_BYTES, LGX, DIGITS & 0xFF, DIGITS >> 8, DIM,
                VAR_TIMES_128 - 1, LPR_BITS, FEC_BITS, CCA, 0 /* padding */
    };

    cshake256_inc_init(ctx, NULL, 0, (const uint8_t *)S, sizeof(S) - 1);
    cshake256_inc_absorb(ctx, (const uint8_t *)pblock, sizeof(pblock));
    cshake256_inc_absorb(ctx, &purpose, 1);
}

/** Sample n gf_t's uniformly from a seed */
static void uniform(gf_t matrix, const uint8_t *seed, uint8_t iv) {
    uint8_t c[GF_BYTES];
    shake256incctx ctx;

    threebears_hash_init(&ctx, HASH_PURPOSE_UNIFORM);
    cshake256_inc_absorb(&ctx, seed, MATRIX_SEED_BYTES);
    cshake256_inc_absorb(&ctx, &iv, 1);
    cshake256_inc_finalize(&ctx);
    cshake256_inc_squeeze(c, sizeof(c), &ctx);
    PQCLEAN_PAPABEAR_CLEAN_expand(matrix, c);
}

/** The ThreeBears error distribution */
static slimb_t psi(uint8_t ci) {
    int sample = 0, var = VAR_TIMES_128;

    for (; var > 64; var -= 64, ci = (uint8_t)(ci << 2)) {
        sample += ((ci + 64) >> 8) + ((ci - 64) >> 8);
    }
    return (slimb_t)(sample + ((ci + var) >> 8) + ((ci - var) >> 8));
}

/** Sample a vector of n noise elements */
static void noise(gf_t x, const shake256incctx *ctx, uint8_t iv) {
    uint8_t c[DIGITS];
    shake256incctx ctx2;

    memcpy(&ctx2, ctx, sizeof(ctx2));
    cshake256_inc_absorb(&ctx2, &iv, 1);
    cshake256_inc_finalize(&ctx2);
    cshake256_inc_squeeze(c, DIGITS, &ctx2);
    for (size_t i = 0; i < DIGITS; i++) {
        x[i] = (limb_t)(psi(c[i]) + PQCLEAN_PAPABEAR_CLEAN_modulus(i));
    }
}

/* Expand public key from private key */
void PQCLEAN_PAPABEAR_CLEAN_get_pubkey(uint8_t *pk, const uint8_t *sk) {
    shake256incctx ctx;
    shake256incctx ctx2;
    gf_t sk_expanded[DIM], b, c;

    threebears_hash_init(&ctx, HASH_PURPOSE_KEYGEN);
    cshake256_inc_absorb(&ctx, sk, PRIVATE_KEY_BYTES);

    memcpy(&ctx2, &ctx, sizeof(ctx2));
    cshake256_inc_finalize(&ctx2);
    cshake256_inc_squeeze(pk, MATRIX_SEED_BYTES, &ctx2);

    for (uint8_t i = 0; i < DIM; i++) {
        noise(sk_expanded[i], &ctx, i);
    }
    for (uint8_t i = 0; i < DIM; i++) {
        noise(c, &ctx, (uint8_t)(i + DIM));
        for (uint8_t j = 0; j < DIM; j++) {
            uniform(b, pk, (uint8_t) (i + DIM * j));
            PQCLEAN_PAPABEAR_CLEAN_mac(c, b, sk_expanded[j]);
        }
        PQCLEAN_PAPABEAR_CLEAN_contract(&pk[MATRIX_SEED_BYTES + i * GF_BYTES], c);
    }
}

/* Encapsulate a shared secret and return it */
void PQCLEAN_PAPABEAR_CLEAN_encapsulate(
    uint8_t *shared_secret,
    uint8_t *capsule,
    const uint8_t *pk,
    const uint8_t *seed
) {
    uint8_t *lpr_data = &capsule[GF_BYTES * DIM];
    shake256incctx ctx;
    gf_t sk_expanded[DIM], b, c;
    uint8_t tbi[ENC_SEED_BYTES + FEC_BYTES];
    dlimb_t rlimb0, rlimb1;
    limb_t h;
    uint8_t *iv = &lpr_data[(ENC_BITS * LPR_BITS + 7) / 8];

    memcpy(iv, &seed[ENC_SEED_BYTES], IV_BYTES);

    threebears_hash_init(&ctx, HASH_PURPOSE_ENCAPS);
    cshake256_inc_absorb(&ctx, pk, MATRIX_SEED_BYTES);
    cshake256_inc_absorb(&ctx, seed, ENC_SEED_BYTES + IV_BYTES);

    for (uint8_t i = 0; i < DIM; i++) {
        noise(sk_expanded[i], &ctx, i);
    }
    for (uint8_t i = 0; i < DIM; i++) {
        noise(c, &ctx, (uint8_t)(i + DIM));
        for (uint8_t j = 0; j < DIM; j++) {
            uniform(b, pk, (uint8_t)(j + DIM * i));
            PQCLEAN_PAPABEAR_CLEAN_mac(c, b, sk_expanded[j]);
        }
        PQCLEAN_PAPABEAR_CLEAN_contract(&capsule[i * GF_BYTES], c);
    }
    noise(c, &ctx, (uint8_t)(2 * DIM));

    /* Calculate approximate shared secret */
    for (uint8_t i = 0; i < DIM; i++) {
        PQCLEAN_PAPABEAR_CLEAN_expand(b, &pk[MATRIX_SEED_BYTES + i * GF_BYTES]);
        PQCLEAN_PAPABEAR_CLEAN_mac(c, b, sk_expanded[i]);
    }
    PQCLEAN_PAPABEAR_CLEAN_canon(c);


    memcpy(tbi, seed, ENC_SEED_BYTES);

    PQCLEAN_PAPABEAR_CLEAN_melas_fec_set(&tbi[ENC_SEED_BYTES], tbi, ENC_SEED_BYTES);

    /* Export with rounding */
    for (size_t i = 0; i < ENC_BITS; i += 2) {
        h = (limb_t)(tbi[i / 8] >> (i % 8));
        rlimb0 = (dlimb_t)((c[i / 2]          >> (LGX - LPR_BITS)) + (h << 3));
        rlimb1 = (dlimb_t)((c[DIGITS - i / 2 - 1] >> (LGX - LPR_BITS)) + ((h >> 1) << 3));
        lpr_data[i / 2] = (uint8_t)((rlimb0 & 0xF) | rlimb1 << 4);
    }

    cshake256_inc_finalize(&ctx);
    cshake256_inc_squeeze(shared_secret, SHARED_SECRET_BYTES, &ctx);
}

/* Decapsulate a shared secret and return it */
void PQCLEAN_PAPABEAR_CLEAN_decapsulate(
    uint8_t shared_secret[SHARED_SECRET_BYTES],
    const uint8_t capsule[CAPSULE_BYTES],
    const uint8_t sk[PRIVATE_KEY_BYTES]
) {
    const uint8_t *lpr_data = &capsule[GF_BYTES * DIM];
    shake256incctx ctx;
    gf_t ska, b, c = {0};
    uint8_t seed[ENC_SEED_BYTES + FEC_BYTES + IV_BYTES];
    limb_t rounding, out;
    size_t j;
    limb_t our_rlimb, their_rlimb, delta;
    uint8_t pk[PUBLIC_KEY_BYTES], capsule2[CAPSULE_BYTES];
    uint8_t ret, ok, sep, prfk[PRF_KEY_BYTES];
    uint8_t prfout[SHARED_SECRET_BYTES];

    /* Calculate approximate shared secret */
    threebears_hash_init(&ctx, HASH_PURPOSE_KEYGEN);
    cshake256_inc_absorb(&ctx, sk, PRIVATE_KEY_BYTES);

    for (uint8_t i = 0; i < DIM; i++) {
        PQCLEAN_PAPABEAR_CLEAN_expand(b, &capsule[i * GF_BYTES]);
        noise(ska, &ctx, i);
        PQCLEAN_PAPABEAR_CLEAN_mac(c, ska, b);
    }

    /* Recover seed from LPR data */
    PQCLEAN_PAPABEAR_CLEAN_canon(c);
    rounding = 1 << (LPR_BITS - 1);
    out = 0;
    for (int32_t i = ENC_BITS - 1; i >= 0; i--) {
        j = (size_t) ((i & 1) ? DIGITS - i / 2 - 1 : i / 2);
        our_rlimb = (limb_t)(c[j] >> (LGX - LPR_BITS - 1));
        their_rlimb = (limb_t)(lpr_data[i * LPR_BITS / 8] >> ((i * LPR_BITS) % 8));
        delta =  (limb_t)(their_rlimb * 2 - our_rlimb + rounding);
        out |= (limb_t)(((delta >> LPR_BITS) & 1) << (i % 8));
        if (i % 8 == 0) {
            seed[i / 8] = (uint8_t)out;
            out = 0;
        }
    }
    PQCLEAN_PAPABEAR_CLEAN_melas_fec_correct(seed, ENC_SEED_BYTES, &seed[ENC_SEED_BYTES]);

    /* Re-encapsulate and check; encapsulate will compute the shared secret */
    PQCLEAN_PAPABEAR_CLEAN_get_pubkey(pk, sk);
    memcpy(&seed[ENC_SEED_BYTES], &lpr_data[(ENC_BITS * LPR_BITS + 7) / 8], IV_BYTES);
    PQCLEAN_PAPABEAR_CLEAN_encapsulate(shared_secret, capsule2, pk, seed);

    /* Check capsule == capsule2 in constant time */
    ret = 0;
    for (size_t i = 0; i < CAPSULE_BYTES; i++) {
        ret |= capsule[i] ^ capsule2[i];
    }
    ok = (uint8_t)(((int)ret - 1) >> 8);

    /* Calculate PRF key */
    sep = 0xFF;
    cshake256_inc_absorb(&ctx, &sep, 1);
    cshake256_inc_finalize(&ctx);
    cshake256_inc_squeeze(prfk, PRF_KEY_BYTES, &ctx);

    /* Calculate PRF */
    threebears_hash_init(&ctx, HASH_PURPOSE_PRF);
    cshake256_inc_absorb(&ctx, prfk, PRF_KEY_BYTES);
    cshake256_inc_absorb(&ctx, capsule, CAPSULE_BYTES);
    cshake256_inc_finalize(&ctx);
    cshake256_inc_squeeze(prfout, SHARED_SECRET_BYTES, &ctx);

    for (size_t i = 0; i < SHARED_SECRET_BYTES; i++) {
        shared_secret[i] = (uint8_t)((shared_secret[i] & ok) | (prfout[i] & ~ok));
    }
}
