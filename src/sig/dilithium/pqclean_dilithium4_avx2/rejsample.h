#ifndef REJSAMPLE_H
#define REJSAMPLE_H

#include <stddef.h>
#include <stdint.h>

uint32_t PQCLEAN_DILITHIUM4_AVX2_rej_uniform(
    uint32_t *r,
    size_t len,
    const uint8_t *buf,
    size_t buflen);

uint32_t PQCLEAN_DILITHIUM4_AVX2_rej_eta(
    uint32_t *r,
    size_t len,
    const uint8_t *buf,
    size_t buflen);

uint32_t PQCLEAN_DILITHIUM4_AVX2_rej_gamma1m1(
    uint32_t *r,
    size_t len,
    const uint8_t *buf,
    size_t buflen);

#endif
