#ifndef VOLE_PARAMETERS_H
#define VOLE_PARAMETERS_H

#include "sdith_ct_utils.h"
#include "sdith_prng.h"

typedef void BITVEC_XOR_TO_F(uint64_t bytelen, bitvec_t* res, const bitvec_t* b);
typedef void BITVEC_XOR_F(uint64_t bytelen, bitvec_t* res, const bitvec_t* a, const bitvec_t* b);
typedef void BITVEC_CASCADE_XOR_TO_F(uint64_t bytelen, bitvec_t* c, bitvec_t* b, const bitvec_t* a);
typedef void FLAMBDA_PRODUCT_F(flambda_t* res, const flambda_t* a, const flambda_t* b);
typedef void FLAMBDA_PRODUCT_F2_F(flambda_t* res, const flambda_t* a, const flambda_t* b_f2);
typedef void FLAMBDA_DOT_PRODUCT_F(flambda_t* res, const flambda_t* x, const flambda_t* y, uint64_t size);
// res += sum_i x[i . x_byte_slice] . y[i]: x is strided (byte stride, multiple of lambda_bytes), y is contiguous
typedef void FLAMBDA_DOT_PRODUCT_ACC_F(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice, const flambda_t* y,
                                       uint64_t size);
// same, with every y_f2[i] restricted to 0 or 1 (the products are plain bit-masks)
typedef void FLAMBDA_DOT_PRODUCT_F2_ACC_F(flambda_t* res, const flambda_t* x, uint64_t x_byte_slice,
                                          const flambda_t* y_f2, uint64_t size);
typedef void FLAMBDA_SUM_F(flambda_t* res, const flambda_t* a, const flambda_t* b);
typedef void FLAMBDA_SUM_POW2_F(flambda_t* res, const flambda_t* a);
typedef void FLAMBDA_ECHELON_POW2_F(uint64_t k, flambda_t* res, const flambda_t* a, uint64_t a_size,
                                    uint64_t a_byte_slice);
typedef void FLAMBDA_SET_F(flambda_t* res, const flambda_t* a);
typedef void FLAMBDA_INVERSE_F(flambda_t* res, const flambda_t* a);
typedef void MATRIX_LAMBDA_TRANSPOSE_F(bitmat_t* out, const bitmat_t* in, uint64_t L, uint64_t Lslice);
typedef void MATRIX_VECTOR_PRODUCT_F2(                //
    uint64_t nrows, uint64_t ncols,                   // dimensions
    bitvec_t* res,                                    // bit vector of size nrows. res=a*b
    const bitmat_t* a,                                // nrows x ncols matrix
    const bitvec_t* b);                               // bit vector of size ncols
typedef void MATRIX_F2_TIMES_VECTOR_FLAMBDA(          //
    uint64_t lambda, uint64_t nrows, uint64_t ncols,  //
    flambda_t* res,                                   // vector of size nrows. res = a*b
    const bitmat_t* a,                                // nrows x ncols matrix
    const flambda_t* b);

typedef struct vole_parameters_t vole_parameters;
struct vole_parameters_t {
  uint64_t TAU;
  uint64_t KAPPA;
  uint64_t LAMBDA;
  uint64_t lambda_bytes;
  // ptr to the prng function?
  BITVEC_XOR_TO_F* bitvec_xor_to;
  BITVEC_XOR_F* bitvec_xor;
  BITVEC_CASCADE_XOR_TO_F* bitvec_cascade_xor_to;
  FLAMBDA_SET_F* flambda_set;
  FLAMBDA_INVERSE_F* flambda_inverse;
  FLAMBDA_PRODUCT_F* flambda_product;
  FLAMBDA_PRODUCT_F* flambda_product_f2;
  FLAMBDA_DOT_PRODUCT_F* flambda_dot_product;
  FLAMBDA_DOT_PRODUCT_ACC_F* flambda_dot_product_acc;
  FLAMBDA_DOT_PRODUCT_F2_ACC_F* flambda_dot_product_f2_acc;
  FLAMBDA_SUM_F* flambda_sum;
  FLAMBDA_SUM_POW2_F* flambda_sum_pow2;
  FLAMBDA_ECHELON_POW2_F* flambda_echelon_pow2;
  MATRIX_LAMBDA_TRANSPOSE_F* matrix_lambda_transpose;
  MATRIX_VECTOR_PRODUCT_F2* matrix_vector_product_f2;
  MATRIX_F2_TIMES_VECTOR_FLAMBDA* matrix_f2_times_vector_flambda;
  // rng functions
  BYTES_OF_PROOFOW_CTX_F* bytes_of_proofow_ctx;
  PROOFOW_INIT_F* proofow_init;
  PROOFOW_GRIND_W_F* proofow_grind_w;
  PROOFOW_VERIFY_W_F* proofow_verify_w;
  EXTEND_LEAF_SEED_F* extend_leaf_seed;
  EXTEND_LEAF_SEED_4X_F* extend_leaf_seed_4x;
  EXTENDED_NODE_SEED_BYTES_F* extended_node_seed_bytes;
  GGM_EXTSEED_RNG_LR_F* ggm_extseed_rng_lr;
  GGM_EXTSEED_RNG_LR_X4_F* ggm_extseed_rng_lr_x4;  // I8: batched 4-node variant; NULL => four ggm_extseed_rng_lr calls
  GGM_EXTSEED_RNG_COMMIT_F* ggm_extseed_rng_commit;
  VOLE_EXTSEED_RNG_F* vole_extseed_rng;
  xof_functions xof;
  xof_vector_functions xof_vector;
  matrix_rng_functions matrix_prg;  // row-wise prng for public-key (H) expansion
  keygen_rng_functions keygen_rng;  // secret-key seed prng (solution sampling)
  ct_utils ct;                      // constant-time helpers (prover/keygen side only)
  // variants
  proofow_variant_t proofow_variant;
};

EXPORT void vole_parameters_init_ref(vole_parameters* vole_params, uint64_t lambda, uint64_t tau, uint64_t kappa);

EXPORT void vole_parameters_init_avx(vole_parameters* vole_params, uint64_t lambda, uint64_t tau, uint64_t kappa);

EXPORT void vole_parameters_init(vole_parameters* vole_params, uint64_t lambda, uint64_t tau, uint64_t kappa);

EXPORT void vole_parameters_init_with_variant(vole_parameters* vole_params, uint64_t lambda, uint64_t tau, uint64_t kappa, proofow_variant_t proofow_variant);

EXPORT void vole_parameters_init_with_variant_ref(vole_parameters* vole_params, uint64_t lambda, uint64_t tau, uint64_t kappa, proofow_variant_t proofow_variant);

EXPORT void vole_parameters_init_with_variant_avx(vole_parameters* vole_params, uint64_t lambda, uint64_t tau, uint64_t kappa, proofow_variant_t proofow_variant);


#endif  // VOLE_PARAMETERS_H
