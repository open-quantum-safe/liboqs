#ifndef RNG_H
#define RNG_H

#include <stddef.h>
#include <stdint.h>

#define RNG_SUCCESS     ( 0)
#define RNG_BAD_MAXLEN  (-1)
#define RNG_BAD_OUTBUF  (-2)
#define RNG_BAD_REQ_LEN (-3)
#define RNG_MAXLEN      (10 * 1024 * 1024)

typedef struct {
    unsigned char   buffer[16];
    size_t          buffer_pos;
    size_t          length_remaining;
    unsigned char   key[32];
    unsigned char   ctr[16];
} AES_XOF_struct;

int PQCLEAN_LEDAKEMLT32_LEAKTIME_seedexpander(AES_XOF_struct *ctx, unsigned char *x, size_t xlen);
void PQCLEAN_LEDAKEMLT32_LEAKTIME_seedexpander_from_trng(AES_XOF_struct *ctx, const unsigned char *trng_entropy);

#endif
