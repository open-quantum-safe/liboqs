int OQS_SIG_qTESLA_III_speed_keypair(
    unsigned char *,
    unsigned char *);

int OQS_SIG_qTESLA_III_speed_sign(
    unsigned char *, unsigned long long *,
    const unsigned char *, unsigned long long,
    const unsigned char *);

int OQS_SIG_qTESLA_III_speed_verify(
    unsigned char *, unsigned long long,
    const unsigned char *, unsigned long long,
    const unsigned char *);
