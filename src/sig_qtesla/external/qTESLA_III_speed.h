#include <oqs/rand.h>

int qTESLA_III_speed_crypto_sign_keypair(
    OQS_RAND *rand, 
    unsigned char *,
    unsigned char *
    );

int qTESLA_III_speed_crypto_sign(
    OQS_RAND *rand,
    unsigned char *,unsigned long long *,
    const unsigned char *,unsigned long long,
    const unsigned char *
    );

int qTESLA_III_speed_crypto_verify(
    unsigned char *,unsigned long long,
    const unsigned char *,unsigned long long,
    const unsigned char *
    );
