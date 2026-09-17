#include "vole_private.h"

void vole_parameters_init_avx(vole_parameters* vole_params, uint64_t lambda, uint64_t tau, uint64_t kappa) {
  vole_parameters_init_with_variant_avx(vole_params, lambda, tau, kappa, PROOFOW_VARIANT_SHAKE);
}

EXPORT void vole_parameters_init_with_variant_avx(vole_parameters* vole_params, uint64_t lambda, uint64_t tau, uint64_t kappa, proofow_variant_t proofow_variant) {
  CASSERT(lambda > 0 && lambda >= tau * kappa, "bug? lambda is not larger than tau*kappa (check parameter order)");
  vole_params->proofow_variant = proofow_variant;
  vole_params->KAPPA = kappa;
  vole_params->TAU = tau;
  vole_params->LAMBDA = lambda;
  vole_params->lambda_bytes = (lambda + 7) >> 3;
  vole_params->bitvec_xor = bitvec_xor_avx2;
  vole_params->bitvec_xor_to = bitvec_xor_to_avx2;
  vole_params->bitvec_cascade_xor_to = bitvec_cascade_xor_to_avx2;
  vole_params->matrix_f2_times_vector_flambda = matrix_f2_times_vector_flambda_avx2;
  vole_params->matrix_vector_product_f2 = matrix_vector_product_f2_avx2;
  vole_params->ct = ct_utils_avx;
  switch (lambda) {
    case 128:
      // crypto
      switch (proofow_variant) {
      case PROOFOW_VARIANT_SHAKE:
          vole_params->bytes_of_proofow_ctx = bytes_of_proofow_ctx_shake_cat1;
          vole_params->proofow_init = proofow_init_shake_cat1;
          vole_params->proofow_grind_w = proofow_grind_w_shake_cat1_4way;  // AVX2 4-way KeccakP-1600-times4
          vole_params->proofow_verify_w = proofow_verify_w_shake_cat1;
          break;
      case PROOFOW_VARIANT_CIPHER:
          vole_params->bytes_of_proofow_ctx = bytes_of_proofow_ctx_cipher_cat1;
          vole_params->proofow_init = proofow_init_cipher_cat1_avx;
          vole_params->proofow_grind_w = proofow_grind_w_cipher_cat1_avx;
          vole_params->proofow_verify_w = proofow_verify_w_cipher_cat1_avx;
          break;
      default:
          abort();  // unknown variant
      }
      vole_params->extend_leaf_seed = extend_leaf_seed_cat1_aes128_avx2;
      vole_params->extend_leaf_seed_4x = extend_leaf_seed_cat1_aes128_4x_avx2;
      vole_params->extended_node_seed_bytes = extended_node_seed_bytes_cat1_aes128;
      vole_params->ggm_extseed_rng_lr = ggm_seed_rng_lr_ext_cat1_aes128_avx2;
      vole_params->ggm_extseed_rng_lr_x4 = ggm_seed_rng_lr_ext_cat1_aes128_x4_avx2;  // I8
      vole_params->ggm_extseed_rng_commit = ggm_commit_rng_ext_cat1_aes128_avx2;
      vole_params->vole_extseed_rng = vole_rng_ext_cat1_aes128_ctrle_nocarry_avx2;
      vole_params->xof = xof_shake128;
      vole_params->xof_vector = xof_vector_shake128;
      vole_params->matrix_prg = matrix_rng_cat1_aes128_avx;
      vole_params->keygen_rng = keygen_rng_cat1_aes128_avx;
      // arithmetic
      vole_params->flambda_set = gf128_flambda_set_ref;
      vole_params->flambda_inverse = gf128_flambda_inverse_pclmul;
      vole_params->flambda_sum_pow2 = gf128_flambda_sum_pow2_ref;
      vole_params->flambda_sum = gf128_flambda_sum_avx2;
      vole_params->flambda_product = gf128_flambda_product_pclmul;
      vole_params->flambda_product_f2 = gf128_flambda_product_pclmul_f2;
      vole_params->flambda_dot_product = gf128_flambda_dot_product_pclmul;
      vole_params->flambda_dot_product_acc = gf128_flambda_dot_product_acc_pclmul;
      vole_params->flambda_dot_product_f2_acc = gf128_flambda_dot_product_f2_acc_avx2;
      vole_params->flambda_echelon_pow2 = gf128_flambda_echelon_pow2_avx;
      vole_params->matrix_lambda_transpose = transpose_128_L_ref;
      break;
    case 192:
      // crypto
      switch (proofow_variant) {
      case PROOFOW_VARIANT_SHAKE:
          vole_params->bytes_of_proofow_ctx = bytes_of_proofow_ctx_shake_cat3;
          vole_params->proofow_init = proofow_init_shake_cat3;
          vole_params->proofow_grind_w = proofow_grind_w_shake_cat3_4way;  // AVX2 4-way KeccakP-1600-times4
          vole_params->proofow_verify_w = proofow_verify_w_shake_cat3;
          break;
      case PROOFOW_VARIANT_CIPHER:
          vole_params->bytes_of_proofow_ctx = bytes_of_proofow_ctx_cipher_cat3;
          vole_params->proofow_init = proofow_init_cipher_cat3_avx;
          vole_params->proofow_grind_w = proofow_grind_w_cipher_cat3_avx;
          vole_params->proofow_verify_w = proofow_verify_w_cipher_cat3_avx;
          break;
      default:
          CREQUIRE(0, "Unknown proof-of-work variant");
      }
      vole_params->extend_leaf_seed = extend_leaf_seed_cat3_rijndael256_avx2;
      vole_params->extend_leaf_seed_4x = extend_leaf_seed_cat3_rijndael256_4x_avx2;
      vole_params->extended_node_seed_bytes = extended_node_seed_bytes_cat3_rijndael256;
      vole_params->ggm_extseed_rng_lr = ggm_seed_rng_lr_ext_cat3_rijndael256_avx2;
      vole_params->ggm_extseed_rng_lr_x4 = ggm_seed_rng_lr_ext_cat3_rijndael256_x4_avx2;  // I8: naive four calls
      vole_params->ggm_extseed_rng_commit = ggm_commit_rng_ext_cat3_rijndael256_avx2;
      vole_params->vole_extseed_rng = vole_rng_ext_cat3_rijndael256_ctrle_avx2;
      vole_params->xof = xof_shake256;
      vole_params->xof_vector = xof_vector_shake256;
      vole_params->matrix_prg = matrix_rng_cat3_rijndael256_avx;
      vole_params->keygen_rng = keygen_rng_cat3_rijndael256_avx;
      // arithmetic
      vole_params->flambda_set = gf192_flambda_set_ref;
      vole_params->flambda_inverse = gf192_flambda_inverse_pclmul;
      vole_params->flambda_sum_pow2 = gf192_flambda_sum_pow2_ref;
      vole_params->flambda_sum = gf192_flambda_sum_ref;
      vole_params->flambda_product = gf192_flambda_product_pclmul;
      vole_params->flambda_product_f2 = gf192_flambda_product_f2_ref; // no avx version
      vole_params->flambda_dot_product = gf192_flambda_dot_product_pclmul;
      vole_params->flambda_dot_product_acc = gf192_flambda_dot_product_acc_pclmul;
      vole_params->flambda_dot_product_f2_acc = gf192_flambda_dot_product_f2_acc_avx2;
      vole_params->flambda_echelon_pow2 = gf192_flambda_echelon_pow2_avx;
      vole_params->matrix_lambda_transpose = transpose_192_L_ref;
      break;
    case 256:
      // crypto
      switch (proofow_variant) {
      case PROOFOW_VARIANT_SHAKE:
          vole_params->bytes_of_proofow_ctx = bytes_of_proofow_ctx_shake_cat5;
          vole_params->proofow_init = proofow_init_shake_cat5;
          vole_params->proofow_grind_w = proofow_grind_w_shake_cat5_4way;  // AVX2 4-way KeccakP-1600-times4
          vole_params->proofow_verify_w = proofow_verify_w_shake_cat5;
          break;
      case PROOFOW_VARIANT_CIPHER:
          vole_params->bytes_of_proofow_ctx = bytes_of_proofow_ctx_cipher_cat5;
          vole_params->proofow_init = proofow_init_cipher_cat5_avx;
          vole_params->proofow_grind_w = proofow_grind_w_cipher_cat5_avx;
          vole_params->proofow_verify_w = proofow_verify_w_cipher_cat5_avx;
          break;
      default:
          CREQUIRE(0, "Unknown proof-of-work variant");
      }
      vole_params->extend_leaf_seed = extend_leaf_seed_cat5_rijndael256_avx2;
      vole_params->extend_leaf_seed_4x = extend_leaf_seed_cat5_rijndael256_4x_avx2;
      vole_params->extended_node_seed_bytes = extended_node_seed_bytes_cat5_rijndael256;
      vole_params->ggm_extseed_rng_lr = ggm_seed_rng_lr_ext_cat5_rijndael256_avx2;
      vole_params->ggm_extseed_rng_lr_x4 = ggm_seed_rng_lr_ext_cat5_rijndael256_x4_avx2;  // I8: naive four calls
      vole_params->ggm_extseed_rng_commit = ggm_commit_rng_ext_cat5_rijndael256_avx2;
      vole_params->vole_extseed_rng = vole_rng_ext_cat5_rijndael256_ctrle_avx2;
      vole_params->xof = xof_shake256;
      vole_params->xof_vector = xof_vector_shake256;
      vole_params->matrix_prg = matrix_rng_cat5_rijndael256_avx;
      vole_params->keygen_rng = keygen_rng_cat5_rijndael256_avx;
      // arithmetic
      vole_params->flambda_set = gf256_flambda_set_ref;
      vole_params->flambda_inverse = gf256_flambda_inverse_pclmul;
      vole_params->flambda_sum_pow2 = gf256_flambda_sum_pow2_ref;
      vole_params->flambda_sum = gf256_flambda_sum_avx2;
      vole_params->flambda_product = gf256_flambda_product_pclmul;
      vole_params->flambda_product_f2 = gf256_flambda_product_pclmul_f2;
      vole_params->flambda_dot_product = gf256_flambda_dot_product_pclmul;
      vole_params->flambda_dot_product_acc = gf256_flambda_dot_product_acc_pclmul;
      vole_params->flambda_dot_product_f2_acc = gf256_flambda_dot_product_f2_acc_avx2;
      vole_params->flambda_echelon_pow2 = gf256_flambda_echelon_pow2_avx;
      vole_params->matrix_lambda_transpose = transpose_256_L_ref;
      break;
    default:
      CREQUIRE(0, "lambda not supported");
  }
}
