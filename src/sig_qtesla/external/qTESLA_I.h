int oqs_qTESLA_I_crypto_sign_keypair(
    unsigned char *,
    unsigned char *);

int oqs_qTESLA_I_crypto_sign(
    unsigned char *, unsigned long long *,
    const unsigned char *, unsigned long long,
    const unsigned char *);

int oqs_qTESLA_I_crypto_verify(
    unsigned char *, unsigned long long,
    const unsigned char *, unsigned long long,
    const unsigned char *);
