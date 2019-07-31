#include "blas_u32.h"
#include "gf.h"

void PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_predicated_add_u32(uint8_t *accu_b, uint8_t predicate, const uint8_t *a, unsigned int _num_byte) {
    uint32_t pr_u32 = ((uint32_t)0) - ((uint32_t)predicate);
    uint8_t pr_u8 = pr_u32 & 0xff;

    unsigned int n_u32 = _num_byte >> 2;
    uint32_t *b_u32 = (uint32_t *)accu_b;
    const uint32_t *a_u32 = (const uint32_t *)a;
    for (unsigned int i = 0; i < n_u32; i++) {
        b_u32[i] ^= (a_u32[i] & pr_u32);
    }

    a += (n_u32 << 2);
    accu_b += (n_u32 << 2);
    unsigned int rem = _num_byte & 3;
    for (unsigned int i = 0; i < rem; i++) {
        accu_b[i] ^= (a[i] & pr_u8);
    }
}

void PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_add_u32(uint8_t *accu_b, const uint8_t *a, unsigned int _num_byte) {
    unsigned int n_u32 = _num_byte >> 2;
    uint32_t *b_u32 = (uint32_t *)accu_b;
    const uint32_t *a_u32 = (const uint32_t *)a;
    for (unsigned int i = 0; i < n_u32; i++) {
        b_u32[i] ^= a_u32[i];
    }

    a += (n_u32 << 2);
    accu_b += (n_u32 << 2);
    unsigned int rem = _num_byte & 3;
    for (unsigned int i = 0; i < rem; i++) {
        accu_b[i] ^= a[i];
    }
}


void PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_mul_scalar_u32(uint8_t *a, uint8_t b, unsigned int _num_byte) {
    unsigned int n_u32 = _num_byte >> 2;
    uint32_t *a_u32 = (uint32_t *)a;
    for (unsigned int i = 0; i < n_u32; i++) {
        a_u32[i] = PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_mul_u32(a_u32[i], b);
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    t.u32 = 0;
    a += (n_u32 << 2);
    unsigned int rem = _num_byte & 3;
    for (unsigned int i = 0; i < rem; i++) {
        t.u8[i] = a[i];
    }
    t.u32 = PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_mul_u32(t.u32, b);
    for (unsigned int i = 0; i < rem; i++) {
        a[i] = t.u8[i];
    }
}

void PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_madd_u32(uint8_t *accu_c, const uint8_t *a, uint8_t gf256_b, unsigned int _num_byte) {
    unsigned int n_u32 = _num_byte >> 2;
    uint32_t *c_u32 = (uint32_t *)accu_c;
    const uint32_t *a_u32 = (const uint32_t *)a;
    for (unsigned int i = 0; i < n_u32; i++) {
        c_u32[i] ^= PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_mul_u32(a_u32[i], gf256_b);
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    t.u32 = 0;
    accu_c += (n_u32 << 2);
    a += (n_u32 << 2);
    unsigned int rem = _num_byte & 3;
    for (unsigned int i = 0; i < rem; i++) {
        t.u8[i] = a[i];
    }
    t.u32 = PQCLEAN_RAINBOWVCCYCLIC_CLEAN_gf256v_mul_u32(t.u32, gf256_b);
    for (unsigned int i = 0; i < rem; i++) {
        accu_c[i] ^= t.u8[i];
    }
}

