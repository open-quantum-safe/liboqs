/** ThreeBears Melas Forward Error Correction header */
#ifndef __THREEBEARS_MELAS_FEC_H__
#define __THREEBEARS_MELAS_FEC_H__
#include "common.h"
#define MELAS_FEC_BYTES 3
#define MELAS_FEC_BITS 18

/* Append 3 bytes of FEC(data) to data, so that the FEC becomes 0 */
void INTERNAL melas_fec_set(
    uint8_t out[MELAS_FEC_BYTES],
    const uint8_t *data,
    unsigned len
);

/* Append 3 bytes of FEC(data) to data, so that the FEC becomes 0 */
void INTERNAL melas_fec_correct(
    uint8_t *data,
    unsigned len,
    const uint8_t fec[MELAS_FEC_BYTES]
);

#endif /*__THREEBEARS_MELAS_FEC_H__*/
