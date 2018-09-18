int oqs_qTESLA_III_size_crypto_sign_keypair(
    unsigned char *,
    unsigned char *);

int oqs_qTESLA_III_size_crypto_sign(
    unsigned char *, unsigned long long *,
    const unsigned char *, unsigned long long,
    const unsigned char *);

int oqs_qTESLA_III_size_crypto_verify(
    unsigned char *, unsigned long long,
    const unsigned char *, unsigned long long,
    const unsigned char *);
