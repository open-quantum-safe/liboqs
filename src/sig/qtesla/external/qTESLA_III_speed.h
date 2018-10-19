OQS_API OQS_STATUS OQS_SIG_qTESLA_III_speed_keypair(
    unsigned char *,
    unsigned char *);

OQS_API OQS_STATUS OQS_SIG_qTESLA_III_speed_sign(
    unsigned char *, unsigned long long *,
    const unsigned char *, unsigned long long,
    const unsigned char *);

OQS_API OQS_STATUS OQS_SIG_qTESLA_III_speed_verify(
    unsigned char *, unsigned long long,
    const unsigned char *, unsigned long long,
    const unsigned char *);
