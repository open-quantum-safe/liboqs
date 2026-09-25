/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include "instances.h"
#include "parameters.h"
#include "tables_128s.h"
#include "tables_128f.h"
#include "tables_192s.h"
#include "tables_192f.h"
#include "tables_em_192s.h"
#include "tables_em_192f.h"
#include "tables_256s.h"
#include "tables_256f.h"

#if defined(FAEST_TESTS)
const char* faest_get_param_name(faest_paramid_t paramid) {
  switch (paramid) {
  case PARAMETER_SET_INVALID:
    return "PARAMETER_SET_INVALID";
  case FAEST_128S:
    return "FAEST_128S";
  case FAEST_128F:
    return "FAEST_128F";
  case FAEST_192S:
    return "FAEST_192S";
  case FAEST_192F:
    return "FAEST_192F";
  case FAEST_256S:
    return "FAEST_256S";
  case FAEST_256F:
    return "FAEST_256F";
  case FAEST_EM_128S:
    return "FAEST_EM_128S";
  case FAEST_EM_128F:
    return "FAEST_EM_128F";
  case FAEST_EM_192S:
    return "FAEST_EM_192S";
  case FAEST_EM_192F:
    return "FAEST_EM_192F";
  case FAEST_EM_256S:
    return "FAEST_EM_256S";
  case FAEST_EM_256F:
    return "FAEST_EM_256F";
  default:
    return "PARAMETER_SET_MAX_INDEX";
  }
}
#endif

#define CALC_TAU1(name) ((name##_LAMBDA - name##_W_GRIND) % name##_TAU)
#define CALC_TAU0(name) (name##_TAU - CALC_TAU1(name))
#define CALC_L(name)                                                                               \
  (CALC_TAU1(name) * (1 << CALC_K(name)) + CALC_TAU0(name) * (1 << (CALC_K(name) - 1)))
#define CALC_K(name) (((name##_LAMBDA - name##_W_GRIND) / (name##_TAU)) + 1)

#define PARAMS(name, other_name)                                                                   \
  {                                                                                                \
      name##_LAMBDA,                                                                               \
      name##_TAU,                                                                                  \
      name##_W_GRIND,                                                                              \
      name##_T_OPEN,                                                                               \
      name##_ELL,                                                                                  \
      CALC_K(name),                                                                                \
      CALC_TAU0(name),                                                                             \
      CALC_TAU1(name),                                                                             \
      CALC_L(name),                                                                                \
      name##_Nst,                                                                                  \
      name##_Ske,                                                                                  \
      name##_R,                                                                                    \
      name##_Lke,                                                                                  \
      name##_Lenc,                                                                                 \
      name##_SIG_SIZE,                                                                             \
      name##_OWF_INPUT_SIZE,                                                                       \
      name##_OWF_OUTPUT_SIZE,                                                                      \
      name##_N_MULT,                                                                               \
      &other_name##_F[0][0],                                                                       \
      &other_name##_G[0][0],                                                                       \
      &other_name##_W_TREE[0][0],                                                                  \
      &other_name##_W_GATE[0][0],                                                                  \
      &other_name##_W_CRT[0][0],                                                                   \
      &other_name##_TREE_MODULI[0],                                                                \
      &other_name##_M_TREE[0],                                                                     \
      other_name##_F_WORDS,                                                                        \
      other_name##_G_WORDS,                                                                        \
      other_name##_W_TREE_WORDS,                                                                   \
      other_name##_W_GATE_WORDS,                                                                   \
      other_name##_W_CRT_WORDS,                                                                    \
  }

#define FAEST_128S_PARAMS PARAMS(FAEST_128S, FAEST_128S)
#define FAEST_128F_PARAMS PARAMS(FAEST_128F, FAEST_128F)
#define FAEST_192S_PARAMS PARAMS(FAEST_192S, FAEST_192S)
#define FAEST_192F_PARAMS PARAMS(FAEST_192F, FAEST_192F)
#define FAEST_256S_PARAMS PARAMS(FAEST_256S, FAEST_256S)
#define FAEST_256F_PARAMS PARAMS(FAEST_256F, FAEST_256F)
#define FAEST_EM_128S_PARAMS PARAMS(FAEST_EM_128S, FAEST_128S)
#define FAEST_EM_128F_PARAMS PARAMS(FAEST_EM_128F, FAEST_128F)
#define FAEST_EM_192S_PARAMS PARAMS(FAEST_EM_192S, FAEST_EM_192S)
#define FAEST_EM_192F_PARAMS PARAMS(FAEST_EM_192F, FAEST_EM_192F)
#define FAEST_EM_256S_PARAMS PARAMS(FAEST_EM_256S, FAEST_256S)
#define FAEST_EM_256F_PARAMS PARAMS(FAEST_EM_256F, FAEST_256F)

#define CASE_PARAM(P)                                                                              \
  case P: {                                                                                        \
    static const faest_paramset_t params = P##_PARAMS;                                             \
    return &params;                                                                                \
  }

const faest_paramset_t* faest_get_paramset(faest_paramid_t paramid) {
  switch (paramid) {
    CASE_PARAM(FAEST_128S)
    CASE_PARAM(FAEST_128F)
    CASE_PARAM(FAEST_EM_128S)
    CASE_PARAM(FAEST_EM_128F)
    CASE_PARAM(FAEST_192S)
    CASE_PARAM(FAEST_192F)
    CASE_PARAM(FAEST_EM_192S)
    CASE_PARAM(FAEST_EM_192F)
    CASE_PARAM(FAEST_256S)
    CASE_PARAM(FAEST_256F)
    CASE_PARAM(FAEST_EM_256S)
    CASE_PARAM(FAEST_EM_256F)
  default:
    return NULL;
  }
}
