#ifndef __EXPAND_MQ_H__
#define __EXPAND_MQ_H__

#include "xof.h"
#include "prg.h"
#include "enc.h"
#include "fields.h"
#include "domain_separation.h"
#include <stdlib.h>

/* Deal with namespacing */
#define ExpandEquations_memopt_init MQOM_NAMESPACE(ExpandEquations_memopt_init)
#define ExpandEquations_memopt_update MQOM_NAMESPACE(ExpandEquations_memopt_update)
#define ExpandEquations_memopt_final MQOM_NAMESPACE(ExpandEquations_memopt_final)
#define ExpandEquations MQOM_NAMESPACE(ExpandEquations)

/* ExpandEquations context used for the "streaming" API, in order
 * to save memory.
 */
#define EXPAND_EQUATIONS_CTX_MAGIC 0xFEEDF00DF00DFEEDULL
typedef struct {
	/* Init magic */
	uint64_t magic;
	/* Global seed */
	uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE];
	uint8_t seed_eq[MQOM3_PARAM_SEED_EQ_SIZE];
	/* Current i (matrix number), i in [0, m-1]) */
	uint32_t i;
	/* Current j (row number in the matrix), j in [0, n], j < n means A_i,j and j = n means b_i*/
	uint32_t j;
	/* Number of bytes asked from the PRG so far */
	uint32_t current_nbytes;
	/* Leftover from the PRG to deal with cross blocks computations */
	uint8_t leftover[MQOM3_PARAM_SEED_EQ_SIZE];
	/* Cached key schedule for seed_eq[i]: valid for all j>0 of the current equation.
	 * Avoids repeating the key schedule across the N+1 PRG_memopt calls per equation. */
	enc_ctx_pub_ecb prg_enc_ctx;
} ExpandEquations_ctx;

int ExpandEquations_memopt_init(const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], ExpandEquations_ctx *ctx);
int ExpandEquations_memopt_update(ExpandEquations_ctx *ctx, field_ext_elt *row);
void ExpandEquations_memopt_final(ExpandEquations_ctx *ctx);

int ExpandEquations(const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], field_ext_elt A_hat[MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU][MQOM3_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt b_hat[MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)]);

#endif /* __EXPAND_MQ_H__ */
