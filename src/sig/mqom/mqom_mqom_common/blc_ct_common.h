#ifndef __BLC_CT_COMMON_H__
#define __BLC_CT_COMMON_H__

/* MQOM3 parameters */
#include "mqom3_parameters.h"
#include "fields.h"
#include "common.h"   /* mqom_cleanse, and <string.h> for memcpy */

/* CT opening chunk layout (spec-compliant interleaved format), shared by
 * every CT implementation - default (blc_ct_default.c), memopt
 * (blc_ct_memopt.c), the memopt Verify path (sign_memopt.c) and streaming
 * verify (verify_stream_ct.c/.h): none of this depends on any one
 * implementation's memory/CPU tradeoff: it is the wire format of the CT
 * opening, plus the one derivation every CT producer shares (DeriveDelta).
 *   opening = chunk[0] || chunk[1] || ... || chunk[tau-1]
 *   chunk[e] = path[e] | ls_com[e] | partial_delta_x[e] | alpha1[e] */
static inline void DeriveDelta(const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t delta[MQOM3_PARAM_SEED_SIZE]) {
	uint8_t _x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)];
	field_base_serialize(x, MQOM3_PARAM_MQ_N, _x);
	memcpy(delta, _x, MQOM3_PARAM_SEED_SIZE);
	/* _x is the secret witness in the clear, not a seed derivative. Wiped here
	 * rather than at the caller's err: label, so the window it sits on the stack
	 * is three instructions instead of a whole Commit. */
	mqom_cleanse(_x, sizeof(_x));
}

#define CT_CHUNK_PATH_SIZE    (MQOM3_PARAM_NB_EVALS_LOG * MQOM3_PARAM_SEED_SIZE)
/* The partial delta_x is what remains of the serialized witness once its first
 * SEED_SIZE bytes have been replaced by the seed, so the witness must be the
 * larger of the two. A parameter set violating this would give the field below
 * a zero or negative size, silently corrupting both the key layout and the wire
 * format. */
#if BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) <= MQOM3_PARAM_SEED_SIZE
#error "CT chunk layout requires BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) > MQOM3_PARAM_SEED_SIZE"
#endif
#define CT_CHUNK_PDELTA_SIZE  (BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE)
#define CT_CHUNK_ALPHA1_SIZE  (BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU))
#define CT_CHUNK_LSCOM_OFF    (CT_CHUNK_PATH_SIZE)
#define CT_CHUNK_PDELTA_OFF   (CT_CHUNK_LSCOM_OFF + MQOM3_PARAM_DIGEST_SIZE)
#define CT_CHUNK_ALPHA1_OFF   (CT_CHUNK_PDELTA_OFF + CT_CHUNK_PDELTA_SIZE)
#define CT_CHUNK_SIZE         (CT_CHUNK_ALPHA1_OFF + CT_CHUNK_ALPHA1_SIZE)

#endif /* __BLC_CT_COMMON_H__ */
