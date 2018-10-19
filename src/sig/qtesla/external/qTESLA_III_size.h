OQS_API OQS_STATUS OQS_SIG_qTESLA_III_size_keypair(
    unsigned char *,
    unsigned char *);

OQS_API OQS_STATUS OQS_SIG_qTESLA_III_size_sign(
    unsigned char *, unsigned long long *,
    const unsigned char *, unsigned long long,
    const unsigned char *);

OQS_API OQS_STATUS OQS_SIG_qTESLA_III_size_verify(
    unsigned char *, unsigned long long,
    const unsigned char *, unsigned long long,
    const unsigned char *);
