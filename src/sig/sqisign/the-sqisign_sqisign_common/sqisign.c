#include <sig.h>
#include <string.h>
#include <encoded_sizes.h>
#include <verification.h>
#if defined(ENABLE_SIGN)
#include <signature.h>
#endif

#if defined(ENABLE_SIGN)
SQISIGN_API
int
sqisign_keypair(unsigned char *pk, unsigned char *sk)
{
    int ret = 0;
    secret_key_t skt;
    public_key_t pkt = { 0 };
    secret_key_init(&skt);

    ret = -!protocols_keygen(&pkt, &skt);

    secret_key_to_bytes(sk, &skt, &pkt);
    public_key_to_bytes(pk, &pkt);
    return ret;
}

SQISIGN_API
int
sqisign_sign_signature(unsigned char *sig, size_t *siglen, const unsigned char *m, size_t mlen, const unsigned char *sk)
{
    int ret = 0;
    secret_key_t skt;
    public_key_t pkt = { 0 };
    signature_t sigt;

    secret_key_init(&skt);
    secret_key_from_bytes(&skt, &pkt, sk);

    ret = -!protocols_sign(&sigt, &pkt, &skt, m, mlen);
    if (ret != 0) {
        *siglen = 0;
        return ret;
    }

    signature_to_bytes(sig, &sigt);
    *siglen = SIGNATURE_BYTES;

    return ret;
}

SQISIGN_API
int
sqisign_sign(unsigned char *sm, size_t *smlen, const unsigned char *m, size_t mlen, const unsigned char *sk)
{
    int ret = 0;

    memmove(sm + SIGNATURE_BYTES, m, mlen);

    ret = sqisign_sign_signature(sm, smlen, sm + SIGNATURE_BYTES, mlen, sk);
    if (ret != 0) {
        return ret;
    }

    *smlen += mlen;

    return ret;
}

#endif

SQISIGN_API
int
sqisign_open(unsigned char *m, size_t *mlen, const unsigned char *sm, size_t smlen, const unsigned char *pk)
{
    int ret = 0;
    public_key_t pkt = { 0 };
    signature_t sigt;

    if (smlen < SIGNATURE_BYTES) {
        *mlen = 0;
        return -1;
    }

    public_key_from_bytes(&pkt, pk);
    signature_from_bytes(&sigt, sm);

    ret = -!protocols_verify(&sigt, &pkt, sm + SIGNATURE_BYTES, smlen - SIGNATURE_BYTES);

    if (!ret) {
        *mlen = smlen - SIGNATURE_BYTES;
        memmove(m, sm + SIGNATURE_BYTES, *mlen);
    } else {
        *mlen = 0;
        memset(m, 0, smlen - SIGNATURE_BYTES);
    }

    return ret;
}

SQISIGN_API
int
sqisign_verify(const unsigned char *sig, size_t siglen, const unsigned char *m, size_t mlen, const unsigned char *pk)
{
    int ret = 0;
    public_key_t pkt = { 0 };
    signature_t sigt;

    if (siglen != SIGNATURE_BYTES) {
        return -1;
    }

    public_key_from_bytes(&pkt, pk);
    signature_from_bytes(&sigt, sig);

    ret = -!protocols_verify(&sigt, &pkt, m, mlen);

    return ret;
}
