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

    ret = !protocols_keygen(&pkt, &skt);

    secret_key_to_bytes(sk, &skt, &pkt);
    public_key_to_bytes(pk, &pkt);
    secret_key_finalize(&skt);
    return ret;
}

SQISIGN_API
int
sqisign_sign(unsigned char *sm,
             unsigned long long *smlen,
             const unsigned char *m,
             unsigned long long mlen,
             const unsigned char *sk)
{
    int ret = 0;
    secret_key_t skt;
    public_key_t pkt = { 0 };
    signature_t sigt;
    secret_key_init(&skt);
    secret_key_from_bytes(&skt, &pkt, sk);

    memmove(sm + SIGNATURE_BYTES, m, mlen);

    ret = !protocols_sign(&sigt, &pkt, &skt, sm + SIGNATURE_BYTES, mlen);
    if (ret != 0) {
        *smlen = 0;
        goto err;
    }

    signature_to_bytes(sm, &sigt);
    *smlen = SIGNATURE_BYTES + mlen;

err:
    secret_key_finalize(&skt);
    return ret;
}

SQISIGN_API
int
sqisign_sign_signature(unsigned char *s,
             unsigned long long *slen,
             const unsigned char *m,
             unsigned long long mlen,
             const unsigned char *sk)
{
    int ret = 0;
    secret_key_t skt;
    public_key_t pkt = { 0 };
    signature_t sigt;
    secret_key_init(&skt);
    secret_key_from_bytes(&skt, &pkt, sk);

    ret = !protocols_sign(&sigt, &pkt, &skt, m, mlen);
    if (ret != 0) {
        *slen = 0;
        goto err;
    }

    signature_to_bytes(s, &sigt);
    *slen = SIGNATURE_BYTES;

err:
    secret_key_finalize(&skt);
    return ret;
}
#endif

SQISIGN_API
int
sqisign_open(unsigned char *m,
             unsigned long long *mlen,
             const unsigned char *sm,
             unsigned long long smlen,
             const unsigned char *pk)
{
    int ret = 0;
    public_key_t pkt = { 0 };
    signature_t sigt;

    public_key_from_bytes(&pkt, pk);
    signature_from_bytes(&sigt, sm);

    ret = !protocols_verify(&sigt, &pkt, sm + SIGNATURE_BYTES, smlen - SIGNATURE_BYTES);

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
sqisign_verify(const unsigned char *m,
               unsigned long long mlen,
               const unsigned char *sig,
               unsigned long long siglen,
               const unsigned char *pk)
{
    (void) siglen;
    int ret = 0;
    public_key_t pkt = { 0 };
    signature_t sigt;

    public_key_from_bytes(&pkt, pk);
    signature_from_bytes(&sigt, sig);

    ret = !protocols_verify(&sigt, &pkt, m, mlen);

    return ret;
}

SQISIGN_API
int
sqisign_verify_signature(const unsigned char *sig,
               unsigned long long siglen,
               const unsigned char *m,
               unsigned long long mlen,
               const unsigned char *pk)
{
    return sqisign_verify(m, mlen, sig, siglen, pk);
}
