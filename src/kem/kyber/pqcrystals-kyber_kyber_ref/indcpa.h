#ifndef INDCPA_H
#define INDCPA_H

#include <stdint.h>
#include "params.h"
#include "polyvec.h"

/* #define gen_matrix KYBER_NAMESPACE(gen_matrix) */
void gen_matrix(polyvec *a, const uint8_t *seed, int8_t security_level, int transposed);
/* #define indcpa_keypair KYBER_NAMESPACE(indcpa_keypair) */
void indcpa_keypair(uint8_t *pk,
                    uint8_t *sk,int8_t security_level);

/* #define indcpa_enc KYBER_NAMESPACE(indcpa_enc) */
void indcpa_enc(uint8_t *c,
                const uint8_t *m,
                const uint8_t *pk,
                const uint8_t *coins,
                int8_t security_level);

/* #define indcpa_dec KYBER_NAMESPACE(indcpa_dec) */
void indcpa_dec(uint8_t *m,
                const uint8_t *c,
                const uint8_t *sk,
                int8_t security_level);

#endif
