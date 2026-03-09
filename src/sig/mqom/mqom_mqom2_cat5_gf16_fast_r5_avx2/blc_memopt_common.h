#ifndef __BLC_MEMOPT_COMMON_H__
#define __BLC_MEMOPT_COMMON_H__

#ifndef BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE
/* If not defined by the user, default to 1 */
#define BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE 1
#else
#if BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE > MQOM2_PARAM_NB_EVALS
#error "Error, BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE should be smaller (or equal) to MQOM2_PARAM_NB_EVALS"
#endif
#if BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE == 0
#error "Error, BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE should be > 0"
#endif
#endif

#include "blc_common.h"

/* Deal with namespacing */
#define BLC_Commit_x1_memopt MQOM_NAMESPACE(BLC_Commit_x1_memopt)
#define BLC_Eval_x1_memopt MQOM_NAMESPACE(BLC_Eval_x1_memopt)
#define BLC_Commit_x2_memopt MQOM_NAMESPACE(BLC_Commit_x2_memopt)
#define BLC_Eval_x2_memopt MQOM_NAMESPACE(BLC_Eval_x2_memopt)
#define BLC_Commit_x4_memopt MQOM_NAMESPACE(BLC_Commit_x4_memopt)
#define BLC_Eval_x4_memopt MQOM_NAMESPACE(BLC_Eval_x4_memopt)

// Times 1
int BLC_Commit_x1_memopt(uint32_t e, const uint8_t rseed[MQOM2_PARAM_SEED_SIZE], const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint8_t com[MQOM2_PARAM_DIGEST_SIZE], uint8_t partial_delta_x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE], field_ext_elt x0[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u0[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], field_ext_elt u1[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]);
int BLC_Eval_x1_memopt(uint32_t e, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t path[MQOM2_PARAM_SEED_SIZE*MQOM2_PARAM_NB_EVALS_LOG], const uint8_t out_ls_com[MQOM2_PARAM_DIGEST_SIZE], const uint8_t partial_delta_x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE], uint16_t i_star, uint8_t com[MQOM2_PARAM_DIGEST_SIZE], field_ext_elt x_eval[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u_eval[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]);

// Times 2
int BLC_Commit_x2_memopt(const uint32_t e[2], const uint8_t rseed[2][MQOM2_PARAM_SEED_SIZE], const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint8_t com[2][MQOM2_PARAM_DIGEST_SIZE], uint8_t partial_delta_x[2][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE], field_ext_elt x0[2][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u0[2][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], field_ext_elt u1[2][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]);
int BLC_Eval_x2_memopt(const uint32_t e[2], const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t path[2][MQOM2_PARAM_SEED_SIZE*MQOM2_PARAM_NB_EVALS_LOG], const uint8_t out_ls_com[2][MQOM2_PARAM_DIGEST_SIZE], const uint8_t partial_delta_x[2][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE], const uint16_t i_star[2], uint8_t com[2][MQOM2_PARAM_DIGEST_SIZE], field_ext_elt x_eval[2][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u_eval[2][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]);

// Times 4
int BLC_Commit_x4_memopt(const uint32_t e[4], const uint8_t rseed[4][MQOM2_PARAM_SEED_SIZE], const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint8_t com[4][MQOM2_PARAM_DIGEST_SIZE], uint8_t partial_delta_x[4][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE], field_ext_elt x0[4][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u0[4][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], field_ext_elt u1[4][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]);
int BLC_Eval_x4_memopt(const uint32_t e[4], const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t path[4][MQOM2_PARAM_SEED_SIZE*MQOM2_PARAM_NB_EVALS_LOG], const uint8_t out_ls_com[4][MQOM2_PARAM_DIGEST_SIZE], const uint8_t partial_delta_x[4][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE], const uint16_t i_star[4], uint8_t com[4][MQOM2_PARAM_DIGEST_SIZE], field_ext_elt x_eval[4][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u_eval[4][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]);



#endif /* __BLC_MEMOPT_COMMON_H__ */
