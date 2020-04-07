#ifndef REJSAMPLE_H
#define REJSAMPLE_H

#include <stddef.h>
#include <stdint.h>

size_t PQCLEAN_KYBER1024_AVX2_rej_uniform(int16_t *r,
        size_t len,
        const uint8_t *buf,
        size_t buflen);

#endif
