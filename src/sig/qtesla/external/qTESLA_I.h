OQS_API int OQS_SIG_qTESLA_I_keypair(
    unsigned char *,
    unsigned char *);

OQS_API int OQS_SIG_qTESLA_I_sign(
    unsigned char *, unsigned long long *,
    const unsigned char *, unsigned long long,
    const unsigned char *);

OQS_API int OQS_SIG_qTESLA_I_verify(
    unsigned char *, unsigned long long,
    const unsigned char *, unsigned long long,
    const unsigned char *);
