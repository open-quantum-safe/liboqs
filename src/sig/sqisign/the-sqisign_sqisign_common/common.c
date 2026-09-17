#include <fips202.h>
#include <tutil.h>
#include <mp.h>
#include <encoded_sizes.h>
#include <ec_params.h>
#include <verification.h>

void
public_key_init(public_key_t *pk)
{
    ec_curve_init(&pk->curve);
}

// compute the challenge as the hash of the message and the commitment curve and public key
void
hash_to_challenge(scalar_t *scalar,
                  const public_key_t *pk,
                  const ec_curve_t *com_curve,
                  const unsigned char *message,
                  size_t length)
{
    // We hash H("SQI" || pk || j(E_com) || message)

    // First we encode the public key to bytes and append the Montgomery A-invariant of the commitment curve.
    // The message is absorbed directly into SHAKE to avoid allocating space and copying it into the buffer.
    unsigned char buf[PUBLICKEY_BYTES + FP2_ENCODED_BYTES];
    public_key_to_bytes(buf, pk);
    fp2_encode(buf + PUBLICKEY_BYTES, &com_curve->A);

    memset(*scalar, 0, sizeof(*scalar)); // we are usually not writing the full value

    shake256incctx ctx;

    shake256_inc_init(&ctx);
    shake256_inc_absorb(&ctx, (const unsigned char *)"SQI", 3);
    shake256_inc_absorb(&ctx, buf, sizeof(buf));
    shake256_inc_absorb(&ctx, message, length);
    shake256_inc_finalize(&ctx);
    shake256_inc_squeeze((void *)(*scalar), CHALLENGE_BYTES, &ctx);
    shake256_inc_ctx_release(&ctx);

#ifdef TARGET_BIG_ENDIAN
    for (unsigned i = 0; i < sizeof(*scalar) / sizeof(**scalar); ++i)
        (*scalar)[i] = BSWAP_DIGIT((*scalar)[i]);
#endif
}
