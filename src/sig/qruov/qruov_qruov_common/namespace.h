#ifndef QRUOV_LIBOQS_NAMESPACE_H
#define QRUOV_LIBOQS_NAMESPACE_H

#ifndef QRUOV_NAMESPACE_PREFIX
#error "QRUOV_NAMESPACE_PREFIX must be defined for the liboqs build"
#endif

#define QRUOV_CONCAT_INNER(a, b) a##b
#define QRUOV_CONCAT(a, b) QRUOV_CONCAT_INNER(a, b)
#define QRUOV_NAMESPACE(name) QRUOV_CONCAT(QRUOV_NAMESPACE_PREFIX, name)

#define aes128ctr_free                    QRUOV_NAMESPACE(aes128ctr_free)
#define aes128ctr_init                    QRUOV_NAMESPACE(aes128ctr_init)
#define aes128ctr_stream                  QRUOV_NAMESPACE(aes128ctr_stream)

#define compute_hash                      QRUOV_NAMESPACE(compute_hash)
#define compute_mu                        QRUOV_NAMESPACE(compute_mu)
#define compute_mu_and_hash               QRUOV_NAMESPACE(compute_mu_and_hash)

#define crypto_sign                       QRUOV_NAMESPACE(crypto_sign)
#define crypto_sign_keypair               QRUOV_NAMESPACE(crypto_sign_keypair)
#define crypto_sign_open                  QRUOV_NAMESPACE(crypto_sign_open)

#define linsys_check_consistency          QRUOV_NAMESPACE(linsys_check_consistency)
#define linsys_check_prepared_consistency QRUOV_NAMESPACE(linsys_check_prepared_consistency)
#define linsys_lu_decompose               QRUOV_NAMESPACE(linsys_lu_decompose)
#define linsys_prepare_consistency         QRUOV_NAMESPACE(linsys_prepare_consistency)
#define linsys_sample_solution            QRUOV_NAMESPACE(linsys_sample_solution)

#define load_fq_checked                   QRUOV_NAMESPACE(load_fq_checked)
#define p3_store_upper_MxM                 QRUOV_NAMESPACE(p3_store_upper_MxM)

#define prepare_pi1_coeffs_from_prg       QRUOV_NAMESPACE(prepare_pi1_coeffs_from_prg)
#define prepare_pi1_from_prg              QRUOV_NAMESPACE(prepare_pi1_from_prg)
#define prepare_pi1_raw_from_prg          QRUOV_NAMESPACE(prepare_pi1_raw_from_prg)

#define prepare_pi2_coeffs_from_prg       QRUOV_NAMESPACE(prepare_pi2_coeffs_from_prg)
#define prepare_pi2_raw_from_prg          QRUOV_NAMESPACE(prepare_pi2_raw_from_prg)
#define prepare_pi2t_from_prg             QRUOV_NAMESPACE(prepare_pi2t_from_prg)

#define prepare_sd_from_seed              QRUOV_NAMESPACE(prepare_sd_from_seed)
#define prepare_sdt_from_sd               QRUOV_NAMESPACE(prepare_sdt_from_sd)
#define prepare_sdt_from_seed             QRUOV_NAMESPACE(prepare_sdt_from_seed)
#define prepare_seed_pk_from_seed         QRUOV_NAMESPACE(prepare_seed_pk_from_seed)
#define prepare_sol_from_seed             QRUOV_NAMESPACE(prepare_sol_from_seed)
#define prepare_y_from_seed               QRUOV_NAMESPACE(prepare_y_from_seed)

#define prg_free                          QRUOV_NAMESPACE(prg_free)
#define prg_init                          QRUOV_NAMESPACE(prg_init)
#define prg_yield                         QRUOV_NAMESPACE(prg_yield)

#define qruov_common_canonicalize_s       QRUOV_NAMESPACE(qruov_common_canonicalize_s)
#define qruov_common_is_canonical_s       QRUOV_NAMESPACE(qruov_common_is_canonical_s)
#define qruov_common_sig_s                QRUOV_NAMESPACE(qruov_common_sig_s)
#define qruov_common_sign_scalar          QRUOV_NAMESPACE(qruov_common_sign_scalar)
#define qruov_common_sign_solve_oil       QRUOV_NAMESPACE(qruov_common_sign_solve_oil)

#define qruov_init                        QRUOV_NAMESPACE(qruov_init)
#define qruov_keygen                      QRUOV_NAMESPACE(qruov_keygen)
#define qruov_sign                        QRUOV_NAMESPACE(qruov_sign)
#define qruov_verify                      QRUOV_NAMESPACE(qruov_verify)

#define rejection_sample                  QRUOV_NAMESPACE(rejection_sample)

#define shake128_digestfinal              QRUOV_NAMESPACE(shake128_digestfinal)
#define shake128_free                     QRUOV_NAMESPACE(shake128_free)
#define shake128_init                     QRUOV_NAMESPACE(shake128_init)
#define shake128_update                   QRUOV_NAMESPACE(shake128_update)

#define shake256_digestfinal              QRUOV_NAMESPACE(shake256_digestfinal)
#define shake256_free                     QRUOV_NAMESPACE(shake256_free)
#define shake256_init                     QRUOV_NAMESPACE(shake256_init)
#define shake256_reset                    QRUOV_NAMESPACE(shake256_reset)
#define shake256_squeeze                  QRUOV_NAMESPACE(shake256_squeeze)
#define shake256_update                   QRUOV_NAMESPACE(shake256_update)

#define unpack_upper_to_symm_MxM          QRUOV_NAMESPACE(unpack_upper_to_symm_MxM)
#define unpack_upper_to_symm_VxV          QRUOV_NAMESPACE(unpack_upper_to_symm_VxV)

#define randombytes                       QRUOV_NAMESPACE(randombytes)


#define crypto_sign_signature QRUOV_NAMESPACE(crypto_sign_signature)
#define crypto_sign_verify QRUOV_NAMESPACE(crypto_sign_verify)

#define AES128_CTR_Stream                    QRUOV_NAMESPACE(AES128_CTR_Stream)
#define AES128_Key_Expansion                 QRUOV_NAMESPACE(AES128_Key_Expansion)
#define evaluate                             QRUOV_NAMESPACE(evaluate)
#define fq_eval_transpose_planes             QRUOV_NAMESPACE(fq_eval_transpose_planes)
#define fql_aos_to_eval_transposed_soa       QRUOV_NAMESPACE(fql_aos_to_eval_transposed_soa)
#define fql_aos_to_eval_vnni                 QRUOV_NAMESPACE(fql_aos_to_eval_vnni)
#define fql_aos_to_soa                       QRUOV_NAMESPACE(fql_aos_to_soa)
#define fql_aos_to_vnni                      QRUOV_NAMESPACE(fql_aos_to_vnni)
#define fql_compact_sym_aos_to_eval_vnni     QRUOV_NAMESPACE(fql_compact_sym_aos_to_eval_vnni)
#define fql_compact_sym_aos_to_vnni          QRUOV_NAMESPACE(fql_compact_sym_aos_to_vnni)
#define fql_matmul_eval_vnni_fast            QRUOV_NAMESPACE(fql_matmul_eval_vnni_fast)
#define fql_matmul_vnni_fast                 QRUOV_NAMESPACE(fql_matmul_vnni_fast)
#define fql_soa_to_aos_permute               QRUOV_NAMESPACE(fql_soa_to_aos_permute)
#define fql_soa_to_aos_permute_double        QRUOV_NAMESPACE(fql_soa_to_aos_permute_double)
#define interpolate                          QRUOV_NAMESPACE(interpolate)
#define interpolate_q31_q31L3                QRUOV_NAMESPACE(interpolate_q31_q31L3)
#define MAT_EVAL                             QRUOV_NAMESPACE(MAT_EVAL)
#define MAT_INTERP                           QRUOV_NAMESPACE(MAT_INTERP)
#define MAT_REEVAL                           QRUOV_NAMESPACE(MAT_REEVAL)
#define matmulfq_vnni_4x16                   QRUOV_NAMESPACE(matmulfq_vnni_4x16)
#define matmulfq_vnni_4x4                    QRUOV_NAMESPACE(matmulfq_vnni_4x4)
#define matmulfq_vnni_4x6                    QRUOV_NAMESPACE(matmulfq_vnni_4x6)
#define matmulfq_vnni_4x7                    QRUOV_NAMESPACE(matmulfq_vnni_4x7)
#define matmulfq_vnni_4x8                    QRUOV_NAMESPACE(matmulfq_vnni_4x8)
#define matmulfql_symmetric_upper            QRUOV_NAMESPACE(matmulfql_symmetric_upper)
#define matmulfql_vnni_1x4                   QRUOV_NAMESPACE(matmulfql_vnni_1x4)
#define matmulfql_vnni_1x6                   QRUOV_NAMESPACE(matmulfql_vnni_1x6)
#define matmulfql_vnni_1x7                   QRUOV_NAMESPACE(matmulfql_vnni_1x7)
#define matmulfql_vnni_1x8                   QRUOV_NAMESPACE(matmulfql_vnni_1x8)
#define qruov_avx2_bdepmask                  QRUOV_NAMESPACE(qruov_avx2_bdepmask)
#define qruov_avx2_bextmask                  QRUOV_NAMESPACE(qruov_avx2_bextmask)
#define reevaluate                           QRUOV_NAMESPACE(reevaluate)
#define reevaluate_q31_q31L3                 QRUOV_NAMESPACE(reevaluate_q31_q31L3)

#define evaluate_q7_q7L10                    QRUOV_NAMESPACE(evaluate_q7_q7L10)
#define interpolate_avx2_q127L10             QRUOV_NAMESPACE(interpolate_avx2_q127L10)
#define interpolate_avx2_q127L3              QRUOV_NAMESPACE(interpolate_avx2_q127L3)
#define interpolate_q31_q31L10               QRUOV_NAMESPACE(interpolate_q31_q31L10)
#define interpolate_q7_q7L10                 QRUOV_NAMESPACE(interpolate_q7_q7L10)
#define reevaluate_avx2_q127L10              QRUOV_NAMESPACE(reevaluate_avx2_q127L10)
#define reevaluate_avx2_q127L3               QRUOV_NAMESPACE(reevaluate_avx2_q127L3)
#define reevaluate_q31_q31L10                QRUOV_NAMESPACE(reevaluate_q31_q31L10)
#define reevaluate_q7_q7L10                  QRUOV_NAMESPACE(reevaluate_q7_q7L10)

#endif
