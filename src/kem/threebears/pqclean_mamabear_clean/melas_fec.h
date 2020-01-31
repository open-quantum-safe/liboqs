#ifndef __THREEBEARS_MELAS_FEC_H__
#define __THREEBEARS_MELAS_FEC_H__

#include "api.h"

#define MELAS_FEC_BYTES 3
#define MELAS_FEC_BITS 18

typedef uint32_t fec_gf_t;
static const fec_gf_t Q = 0x211;

/* Append 3 bytes of FEC(data) to data, so that the FEC becomes 0 */
void PQCLEAN_MAMABEAR_CLEAN_melas_fec_set(
    uint8_t out[MELAS_FEC_BYTES],
    const uint8_t *data,
    size_t len
);

/* Append 3 bytes of FEC(data) to data, so that the FEC becomes 0 */
void PQCLEAN_MAMABEAR_CLEAN_melas_fec_correct(
    uint8_t *data,
    size_t len,
    const uint8_t fec[MELAS_FEC_BYTES]
);

#endif
