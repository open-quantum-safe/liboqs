#include "blas.h"
#include "gf.h"

#include <stddef.h>

void PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_predicated_add(uint8_t *accu_b, uint8_t predicate, const uint8_t *a, size_t _num_byte) {
    uint8_t pr_u8 = (uint8_t) ((uint8_t) 0 - predicate);
    for (size_t i = 0; i < _num_byte; i++) {
        accu_b[i] ^= (a[i] & pr_u8);
    }
}

void PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_add(uint8_t *accu_b, const uint8_t *a, size_t _num_byte) {
    for (size_t i = 0; i < _num_byte; i++) {
        accu_b[i] ^= a[i];
    }
}


void PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_mul_scalar(uint8_t *a, uint8_t b, size_t _num_byte) {
    for (size_t i = 0; i < _num_byte; i++) {
        a[i] = PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256_mul(a[i], b);
    }
}

void PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_madd(uint8_t *accu_c, const uint8_t *a, uint8_t gf256_b, size_t _num_byte) {
    for (size_t i = 0; i < _num_byte; i++) {
        accu_c[i] ^= PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256_mul(a[i], gf256_b);
    }
}

