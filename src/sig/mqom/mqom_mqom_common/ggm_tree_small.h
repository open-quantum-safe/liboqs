#ifndef __GGM_TREE_DEFAULT_H__
#define __GGM_TREE_DEFAULT_H__

/* MQOM3 parameters */
#include "mqom3_parameters.h"
/* Encryption primitive */
#include "enc.h"
/* Common helpers */
#include "common.h"

/* Deal with namespacing */
#define SmallGGMTree_Expand MQOM_NAMESPACE(SmallGGMTree_Expand)
#define SmallGGMTree_Open MQOM_NAMESPACE(SmallGGMTree_Open)
#define SmallGGMTree_PartiallyExpand MQOM_NAMESPACE(SmallGGMTree_PartiallyExpand)
#define SmallGGMTree_ExpandPath MQOM_NAMESPACE(SmallGGMTree_ExpandPath)

int SmallGGMTree_Expand(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t delta[MQOM3_PARAM_SEED_SIZE], uint32_t e, uint8_t node[MQOM3_PARAM_FULL_TREE_SIZE + 1][MQOM3_PARAM_SEED_SIZE], uint8_t lseed[MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]);

int SmallGGMTree_Open(const uint8_t node[MQOM3_PARAM_FULL_TREE_SIZE + 1][MQOM3_PARAM_SEED_SIZE], uint32_t i_star, uint8_t path[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE]);

int SmallGGMTree_PartiallyExpand(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star, uint8_t lseed[MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]);

int SmallGGMTree_ExpandPath(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t delta[MQOM3_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star, uint8_t path[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE], uint8_t lseed[MQOM3_PARAM_SEED_SIZE]);

#endif /* __GGM_TREE_DEFAULT_H__ */
