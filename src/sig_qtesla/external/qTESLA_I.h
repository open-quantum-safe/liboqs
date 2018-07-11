#include <oqs/rand.h>

int oqs_qTESLA_I_crypto_sign_keypair(
    OQS_RAND *rand, 
    unsigned char *,
    unsigned char *
    );

int oqs_qTESLA_I_crypto_sign(
    OQS_RAND *rand,
    unsigned char *,unsigned long long *,
    const unsigned char *,unsigned long long,
    const unsigned char *
    );

int oqs_qTESLA_I_crypto_verify(
    unsigned char *,unsigned long long,
    const unsigned char *,unsigned long long,
    const unsigned char *
    );
