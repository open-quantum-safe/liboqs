#ifndef QRUOV_H
#define QRUOV_H

#include <stddef.h>
#include <stdint.h>

#include "qruov_param.h"
#include "qruov_tau.h"

#define QRUOV_n (QRUOV_v + QRUOV_m)
#define QRUOV_N (QRUOV_n / QRUOV_L)
#define QRUOV_V (QRUOV_v / QRUOV_L)
#define QRUOV_M (QRUOV_m / QRUOV_L)

#define QRUOV_n1 (QRUOV_L * QRUOV_V * (QRUOV_V + 1) / 2)
#define QRUOV_n2 (QRUOV_L * QRUOV_V * QRUOV_M)
#define QRUOV_perm(t) ((t <= QRUOV_fe - 1) ? (QRUOV_fe - 1 - t) : (QRUOV_L + QRUOV_fe - 1 - t))

#if QRUOV_q == 127
#define QRUOV_q_LOG 7
#elif QRUOV_q == 31
#define QRUOV_q_LOG 5
#elif QRUOV_q == 7
#define QRUOV_q_LOG 3
#else
#error "Unsupported QRUOV_q"
#endif // QRUOV_q

#define BITS2BYTE(x)    (((x) + 7) >> 3)
#define SEED_PK_LEN     16
#define SEED_SK_LEN     32

#if QRUOV_security == 1
#define SALT_LEN        16
#elif QRUOV_security == 3
#define SALT_LEN        24
#elif QRUOV_security == 5
#define SALT_LEN        32
#else
#error "Unsupported QRUOV_security"
#endif

#define P3_EQ_COEFF_LEN (QRUOV_L * QRUOV_M * (QRUOV_M + 1) / 2)
#define P3_COEFF_LEN    (QRUOV_m * P3_EQ_COEFF_LEN)
#define SIG_S_COEFF_LEN (QRUOV_L * QRUOV_N)

#define P3_SERIALIZED_LEN    BITS2BYTE(QRUOV_q_LOG * P3_COEFF_LEN)
#define SIG_S_SERIALIZED_LEN BITS2BYTE(QRUOV_q_LOG * SIG_S_COEFF_LEN)

#define QRUOV_POW0(x) (1)
#define QRUOV_POW1(x) (((x) + QRUOV_q) % QRUOV_q)
#define QRUOV_POW2(x) (QRUOV_POW1(x) * QRUOV_POW1(x) % QRUOV_q)
#define QRUOV_POW3(x) (QRUOV_POW2(x) * QRUOV_POW1(x) % QRUOV_q)
#define QRUOV_POW4(x) (QRUOV_POW3(x) * QRUOV_POW1(x) % QRUOV_q)
#define QRUOV_POW5(x) (QRUOV_POW4(x) * QRUOV_POW1(x) % QRUOV_q)
#define QRUOV_POW6(x) (QRUOV_POW5(x) * QRUOV_POW1(x) % QRUOV_q)
#define QRUOV_POW7(x) (QRUOV_POW6(x) * QRUOV_POW1(x) % QRUOV_q)
#define QRUOV_POW8(x) (QRUOV_POW7(x) * QRUOV_POW1(x) % QRUOV_q)
#define QRUOV_POW9(x) (QRUOV_POW8(x) * QRUOV_POW1(x) % QRUOV_q)

#define QRUOV_SIG_LEN (SALT_LEN + SIG_S_SERIALIZED_LEN)
#define QRUOV_PK_LEN  (SEED_PK_LEN + P3_SERIALIZED_LEN)
#define QRUOV_SK_LEN  SEED_SK_LEN

#define QRUOV_STR_(x) #x
#define QRUOV_STR(x) QRUOV_STR_(x)
#if PRG_IS_AES
#define QRUOV_PRG_NAME "aes"
#else
#define QRUOV_PRG_NAME "shake"
#endif // PRG_IS_AES
#define QRUOV_ALGNAME "qruov" QRUOV_STR(QRUOV_security) "q" QRUOV_STR(QRUOV_q) "l" QRUOV_STR(QRUOV_L) "v" QRUOV_STR(QRUOV_v) "m" QRUOV_STR(QRUOV_m) QRUOV_PRG_NAME

typedef uint8_t SEED_PK[SEED_PK_LEN];
typedef uint8_t SEED_SK[SEED_SK_LEN];
typedef uint8_t SALT[SALT_LEN];
typedef uint8_t SIG_S[SIG_S_COEFF_LEN];

void qruov_init(void);
void qruov_keygen(const SEED_PK seed_pk, const SEED_SK seed_sk, uint8_t pk_p3[P3_SERIALIZED_LEN]);
int qruov_sign(const SEED_PK seed_pk, const SEED_SK seed_sk, const SEED_SK seed_y, const SEED_SK seed_r, const SEED_SK seed_sol, const uint8_t msg[], size_t msg_len, SALT sig_r, SIG_S sig_s);
int qruov_verify(const SEED_PK seed_pk, const uint8_t pk_p3[P3_SERIALIZED_LEN], const uint8_t msg[], size_t msg_len, const SALT sig_r, const SIG_S sig_s);

#endif // QRUOV_H
