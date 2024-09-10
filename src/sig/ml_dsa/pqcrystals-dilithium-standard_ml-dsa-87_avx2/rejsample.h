#ifndef REJSAMPLE_H
#define REJSAMPLE_H

#include <stdint.h>
#include "params.h"
#include "symmetric.h"

#define REJ_UNIFORM_NBLOCKS ((768+STREAM128_BLOCKBYTES-1)/STREAM128_BLOCKBYTES)
#define REJ_UNIFORM_BUFLEN (REJ_UNIFORM_NBLOCKS*STREAM128_BLOCKBYTES)

#if ETA == 2
#define REJ_UNIFORM_ETA_NBLOCKS ((136+STREAM256_BLOCKBYTES-1)/STREAM256_BLOCKBYTES)
#elif ETA == 4
#define REJ_UNIFORM_ETA_NBLOCKS ((227+STREAM256_BLOCKBYTES-1)/STREAM256_BLOCKBYTES)
#endif
#define REJ_UNIFORM_ETA_BUFLEN (REJ_UNIFORM_ETA_NBLOCKS*STREAM256_BLOCKBYTES)

#define idxlut DILITHIUM_NAMESPACE(idxlut)
extern const uint8_t idxlut[256][8];

#define rej_uniform_avx DILITHIUM_NAMESPACE(rej_uniform_avx)
unsigned int rej_uniform_avx(int32_t *r, const uint8_t buf[REJ_UNIFORM_BUFLEN+8]);

#define rej_eta_avx DILITHIUM_NAMESPACE(rej_eta_avx)
unsigned int rej_eta_avx(int32_t *r, const uint8_t buf[REJ_UNIFORM_ETA_BUFLEN]);

#endif

