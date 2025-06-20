
#ifndef SQISIGN_NAMESPACE_H
#define SQISIGN_NAMESPACE_H

//#define DISABLE_NAMESPACING

#if defined(_WIN32)
#define SQISIGN_API __declspec(dllexport)
#else
#define SQISIGN_API __attribute__((visibility("default")))
#endif

#define PARAM_JOIN3_(a, b, c) sqisign_##a##_##b##_##c
#define PARAM_JOIN3(a, b, c) PARAM_JOIN3_(a, b, c)
#define PARAM_NAME3(end, s) PARAM_JOIN3(SQISIGN_VARIANT, end, s)

#define PARAM_JOIN2_(a, b) sqisign_##a##_##b
#define PARAM_JOIN2(a, b) PARAM_JOIN2_(a, b)
#define PARAM_NAME2(end, s) PARAM_JOIN2(end, s)

#ifndef DISABLE_NAMESPACING
#define SQISIGN_NAMESPACE_GENERIC(s) PARAM_NAME2(gen, s)
#else
#define SQISIGN_NAMESPACE_GENERIC(s) s
#endif

#if defined(SQISIGN_VARIANT) && !defined(DISABLE_NAMESPACING)
#if defined(SQISIGN_BUILD_TYPE_REF)
#define SQISIGN_NAMESPACE(s) PARAM_NAME3(ref, s)
#elif defined(SQISIGN_BUILD_TYPE_OPT)
#define SQISIGN_NAMESPACE(s) PARAM_NAME3(opt, s)
#elif defined(SQISIGN_BUILD_TYPE_BROADWELL)
#define SQISIGN_NAMESPACE(s) PARAM_NAME3(broadwell, s)
#elif defined(SQISIGN_BUILD_TYPE_ARM64CRYPTO)
#define SQISIGN_NAMESPACE(s) PARAM_NAME3(arm64crypto, s)
#else
#error "Build type not known"
#endif

#else
#define SQISIGN_NAMESPACE(s) s
#endif

// Namespacing symbols exported from algebra.c:
#undef quat_alg_add
#undef quat_alg_conj
#undef quat_alg_coord_mul
#undef quat_alg_elem_copy
#undef quat_alg_elem_copy_ibz
#undef quat_alg_elem_equal
#undef quat_alg_elem_is_zero
#undef quat_alg_elem_mul_by_scalar
#undef quat_alg_elem_set
#undef quat_alg_equal_denom
#undef quat_alg_init_set_ui
#undef quat_alg_make_primitive
#undef quat_alg_mul
#undef quat_alg_norm
#undef quat_alg_normalize
#undef quat_alg_scalar
#undef quat_alg_sub

#define quat_alg_add                                    SQISIGN_NAMESPACE_GENERIC(quat_alg_add)
#define quat_alg_conj                                   SQISIGN_NAMESPACE_GENERIC(quat_alg_conj)
#define quat_alg_coord_mul                              SQISIGN_NAMESPACE_GENERIC(quat_alg_coord_mul)
#define quat_alg_elem_copy                              SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_copy)
#define quat_alg_elem_copy_ibz                          SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_copy_ibz)
#define quat_alg_elem_equal                             SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_equal)
#define quat_alg_elem_is_zero                           SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_is_zero)
#define quat_alg_elem_mul_by_scalar                     SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_mul_by_scalar)
#define quat_alg_elem_set                               SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_set)
#define quat_alg_equal_denom                            SQISIGN_NAMESPACE_GENERIC(quat_alg_equal_denom)
#define quat_alg_init_set_ui                            SQISIGN_NAMESPACE_GENERIC(quat_alg_init_set_ui)
#define quat_alg_make_primitive                         SQISIGN_NAMESPACE_GENERIC(quat_alg_make_primitive)
#define quat_alg_mul                                    SQISIGN_NAMESPACE_GENERIC(quat_alg_mul)
#define quat_alg_norm                                   SQISIGN_NAMESPACE_GENERIC(quat_alg_norm)
#define quat_alg_normalize                              SQISIGN_NAMESPACE_GENERIC(quat_alg_normalize)
#define quat_alg_scalar                                 SQISIGN_NAMESPACE_GENERIC(quat_alg_scalar)
#define quat_alg_sub                                    SQISIGN_NAMESPACE_GENERIC(quat_alg_sub)

// Namespacing symbols exported from api.c:
#undef crypto_sign
#undef crypto_sign_keypair
#undef crypto_sign_open

#define crypto_sign                                     SQISIGN_NAMESPACE(crypto_sign)
#define crypto_sign_keypair                             SQISIGN_NAMESPACE(crypto_sign_keypair)
#define crypto_sign_open                                SQISIGN_NAMESPACE(crypto_sign_open)

// Namespacing symbols exported from basis.c:
#undef ec_curve_to_basis_2f_from_hint
#undef ec_curve_to_basis_2f_to_hint
#undef ec_recover_y
#undef lift_basis
#undef lift_basis_normalized

#define ec_curve_to_basis_2f_from_hint                  SQISIGN_NAMESPACE(ec_curve_to_basis_2f_from_hint)
#define ec_curve_to_basis_2f_to_hint                    SQISIGN_NAMESPACE(ec_curve_to_basis_2f_to_hint)
#define ec_recover_y                                    SQISIGN_NAMESPACE(ec_recover_y)
#define lift_basis                                      SQISIGN_NAMESPACE(lift_basis)
#define lift_basis_normalized                           SQISIGN_NAMESPACE(lift_basis_normalized)

// Namespacing symbols exported from biextension.c:
#undef clear_cofac
#undef ec_dlog_2_tate
#undef ec_dlog_2_weil
#undef fp2_frob
#undef reduced_tate
#undef weil

#define clear_cofac                                     SQISIGN_NAMESPACE(clear_cofac)
#define ec_dlog_2_tate                                  SQISIGN_NAMESPACE(ec_dlog_2_tate)
#define ec_dlog_2_weil                                  SQISIGN_NAMESPACE(ec_dlog_2_weil)
#define fp2_frob                                        SQISIGN_NAMESPACE(fp2_frob)
#define reduced_tate                                    SQISIGN_NAMESPACE(reduced_tate)
#define weil                                            SQISIGN_NAMESPACE(weil)

// Namespacing symbols exported from common.c:
#undef hash_to_challenge
#undef public_key_finalize
#undef public_key_init

#define hash_to_challenge                               SQISIGN_NAMESPACE(hash_to_challenge)
#define public_key_finalize                             SQISIGN_NAMESPACE(public_key_finalize)
#define public_key_init                                 SQISIGN_NAMESPACE(public_key_init)

// Namespacing symbols exported from dim2.c:
#undef ibz_2x2_mul_mod
#undef ibz_mat_2x2_add
#undef ibz_mat_2x2_copy
#undef ibz_mat_2x2_det_from_ibz
#undef ibz_mat_2x2_eval
#undef ibz_mat_2x2_inv_mod
#undef ibz_mat_2x2_set
#undef ibz_vec_2_set

#define ibz_2x2_mul_mod                                 SQISIGN_NAMESPACE_GENERIC(ibz_2x2_mul_mod)
#define ibz_mat_2x2_add                                 SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_add)
#define ibz_mat_2x2_copy                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_copy)
#define ibz_mat_2x2_det_from_ibz                        SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_det_from_ibz)
#define ibz_mat_2x2_eval                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_eval)
#define ibz_mat_2x2_inv_mod                             SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_inv_mod)
#define ibz_mat_2x2_set                                 SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_set)
#define ibz_vec_2_set                                   SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_set)

// Namespacing symbols exported from dim2id2iso.c:
#undef dim2id2iso_arbitrary_isogeny_evaluation
#undef dim2id2iso_ideal_to_isogeny_clapotis
#undef find_uv
#undef fixed_degree_isogeny_and_eval

#define dim2id2iso_arbitrary_isogeny_evaluation         SQISIGN_NAMESPACE(dim2id2iso_arbitrary_isogeny_evaluation)
#define dim2id2iso_ideal_to_isogeny_clapotis            SQISIGN_NAMESPACE(dim2id2iso_ideal_to_isogeny_clapotis)
#define find_uv                                         SQISIGN_NAMESPACE(find_uv)
#define fixed_degree_isogeny_and_eval                   SQISIGN_NAMESPACE(fixed_degree_isogeny_and_eval)

// Namespacing symbols exported from dim4.c:
#undef ibz_inv_dim4_make_coeff_mpm
#undef ibz_inv_dim4_make_coeff_pmp
#undef ibz_mat_4x4_copy
#undef ibz_mat_4x4_equal
#undef ibz_mat_4x4_eval
#undef ibz_mat_4x4_eval_t
#undef ibz_mat_4x4_gcd
#undef ibz_mat_4x4_identity
#undef ibz_mat_4x4_inv_with_det_as_denom
#undef ibz_mat_4x4_is_identity
#undef ibz_mat_4x4_mul
#undef ibz_mat_4x4_negate
#undef ibz_mat_4x4_scalar_div
#undef ibz_mat_4x4_scalar_mul
#undef ibz_mat_4x4_transpose
#undef ibz_mat_4x4_zero
#undef ibz_vec_4_add
#undef ibz_vec_4_content
#undef ibz_vec_4_copy
#undef ibz_vec_4_copy_ibz
#undef ibz_vec_4_is_zero
#undef ibz_vec_4_linear_combination
#undef ibz_vec_4_negate
#undef ibz_vec_4_scalar_div
#undef ibz_vec_4_scalar_mul
#undef ibz_vec_4_set
#undef ibz_vec_4_sub
#undef quat_qf_eval

#define ibz_inv_dim4_make_coeff_mpm                     SQISIGN_NAMESPACE_GENERIC(ibz_inv_dim4_make_coeff_mpm)
#define ibz_inv_dim4_make_coeff_pmp                     SQISIGN_NAMESPACE_GENERIC(ibz_inv_dim4_make_coeff_pmp)
#define ibz_mat_4x4_copy                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_copy)
#define ibz_mat_4x4_equal                               SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_equal)
#define ibz_mat_4x4_eval                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_eval)
#define ibz_mat_4x4_eval_t                              SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_eval_t)
#define ibz_mat_4x4_gcd                                 SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_gcd)
#define ibz_mat_4x4_identity                            SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_identity)
#define ibz_mat_4x4_inv_with_det_as_denom               SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_inv_with_det_as_denom)
#define ibz_mat_4x4_is_identity                         SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_is_identity)
#define ibz_mat_4x4_mul                                 SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_mul)
#define ibz_mat_4x4_negate                              SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_negate)
#define ibz_mat_4x4_scalar_div                          SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_scalar_div)
#define ibz_mat_4x4_scalar_mul                          SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_scalar_mul)
#define ibz_mat_4x4_transpose                           SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_transpose)
#define ibz_mat_4x4_zero                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_zero)
#define ibz_vec_4_add                                   SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_add)
#define ibz_vec_4_content                               SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_content)
#define ibz_vec_4_copy                                  SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_copy)
#define ibz_vec_4_copy_ibz                              SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_copy_ibz)
#define ibz_vec_4_is_zero                               SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_is_zero)
#define ibz_vec_4_linear_combination                    SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_linear_combination)
#define ibz_vec_4_negate                                SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_negate)
#define ibz_vec_4_scalar_div                            SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_scalar_div)
#define ibz_vec_4_scalar_mul                            SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_scalar_mul)
#define ibz_vec_4_set                                   SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_set)
#define ibz_vec_4_sub                                   SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_sub)
#define quat_qf_eval                                    SQISIGN_NAMESPACE_GENERIC(quat_qf_eval)

// Namespacing symbols exported from ec.c:
#undef cswap_points
#undef ec_biscalar_mul
#undef ec_curve_init
#undef ec_curve_init_from_A
#undef ec_curve_normalize_A24
#undef ec_curve_verify_A
#undef ec_dbl
#undef ec_dbl_iter
#undef ec_dbl_iter_basis
#undef ec_has_zero_coordinate
#undef ec_is_basis_four_torsion
#undef ec_is_equal
#undef ec_is_four_torsion
#undef ec_is_two_torsion
#undef ec_is_zero
#undef ec_j_inv
#undef ec_ladder3pt
#undef ec_mul
#undef ec_normalize_curve
#undef ec_normalize_curve_and_A24
#undef ec_normalize_point
#undef ec_point_init
#undef select_point
#undef xADD
#undef xDBL
#undef xDBLADD
#undef xDBLMUL
#undef xDBL_A24
#undef xDBL_E0
#undef xMUL

#define cswap_points                                    SQISIGN_NAMESPACE(cswap_points)
#define ec_biscalar_mul                                 SQISIGN_NAMESPACE(ec_biscalar_mul)
#define ec_curve_init                                   SQISIGN_NAMESPACE(ec_curve_init)
#define ec_curve_init_from_A                            SQISIGN_NAMESPACE(ec_curve_init_from_A)
#define ec_curve_normalize_A24                          SQISIGN_NAMESPACE(ec_curve_normalize_A24)
#define ec_curve_verify_A                               SQISIGN_NAMESPACE(ec_curve_verify_A)
#define ec_dbl                                          SQISIGN_NAMESPACE(ec_dbl)
#define ec_dbl_iter                                     SQISIGN_NAMESPACE(ec_dbl_iter)
#define ec_dbl_iter_basis                               SQISIGN_NAMESPACE(ec_dbl_iter_basis)
#define ec_has_zero_coordinate                          SQISIGN_NAMESPACE(ec_has_zero_coordinate)
#define ec_is_basis_four_torsion                        SQISIGN_NAMESPACE(ec_is_basis_four_torsion)
#define ec_is_equal                                     SQISIGN_NAMESPACE(ec_is_equal)
#define ec_is_four_torsion                              SQISIGN_NAMESPACE(ec_is_four_torsion)
#define ec_is_two_torsion                               SQISIGN_NAMESPACE(ec_is_two_torsion)
#define ec_is_zero                                      SQISIGN_NAMESPACE(ec_is_zero)
#define ec_j_inv                                        SQISIGN_NAMESPACE(ec_j_inv)
#define ec_ladder3pt                                    SQISIGN_NAMESPACE(ec_ladder3pt)
#define ec_mul                                          SQISIGN_NAMESPACE(ec_mul)
#define ec_normalize_curve                              SQISIGN_NAMESPACE(ec_normalize_curve)
#define ec_normalize_curve_and_A24                      SQISIGN_NAMESPACE(ec_normalize_curve_and_A24)
#define ec_normalize_point                              SQISIGN_NAMESPACE(ec_normalize_point)
#define ec_point_init                                   SQISIGN_NAMESPACE(ec_point_init)
#define select_point                                    SQISIGN_NAMESPACE(select_point)
#define xADD                                            SQISIGN_NAMESPACE(xADD)
#define xDBL                                            SQISIGN_NAMESPACE(xDBL)
#define xDBLADD                                         SQISIGN_NAMESPACE(xDBLADD)
#define xDBLMUL                                         SQISIGN_NAMESPACE(xDBLMUL)
#define xDBL_A24                                        SQISIGN_NAMESPACE(xDBL_A24)
#define xDBL_E0                                         SQISIGN_NAMESPACE(xDBL_E0)
#define xMUL                                            SQISIGN_NAMESPACE(xMUL)

// Namespacing symbols exported from ec_jac.c:
#undef ADD
#undef DBL
#undef DBLW
#undef copy_jac_point
#undef jac_from_ws
#undef jac_init
#undef jac_is_equal
#undef jac_neg
#undef jac_to_ws
#undef jac_to_xz
#undef jac_to_xz_add_components
#undef select_jac_point

#define ADD                                             SQISIGN_NAMESPACE(ADD)
#define DBL                                             SQISIGN_NAMESPACE(DBL)
#define DBLW                                            SQISIGN_NAMESPACE(DBLW)
#define copy_jac_point                                  SQISIGN_NAMESPACE(copy_jac_point)
#define jac_from_ws                                     SQISIGN_NAMESPACE(jac_from_ws)
#define jac_init                                        SQISIGN_NAMESPACE(jac_init)
#define jac_is_equal                                    SQISIGN_NAMESPACE(jac_is_equal)
#define jac_neg                                         SQISIGN_NAMESPACE(jac_neg)
#define jac_to_ws                                       SQISIGN_NAMESPACE(jac_to_ws)
#define jac_to_xz                                       SQISIGN_NAMESPACE(jac_to_xz)
#define jac_to_xz_add_components                        SQISIGN_NAMESPACE(jac_to_xz_add_components)
#define select_jac_point                                SQISIGN_NAMESPACE(select_jac_point)

// Namespacing symbols exported from encode_signature.c:
#undef secret_key_from_bytes
#undef secret_key_to_bytes

#define secret_key_from_bytes                           SQISIGN_NAMESPACE(secret_key_from_bytes)
#define secret_key_to_bytes                             SQISIGN_NAMESPACE(secret_key_to_bytes)

// Namespacing symbols exported from encode_verification.c:
#undef public_key_from_bytes
#undef public_key_to_bytes
#undef signature_from_bytes
#undef signature_to_bytes

#define public_key_from_bytes                           SQISIGN_NAMESPACE(public_key_from_bytes)
#define public_key_to_bytes                             SQISIGN_NAMESPACE(public_key_to_bytes)
#define signature_from_bytes                            SQISIGN_NAMESPACE(signature_from_bytes)
#define signature_to_bytes                              SQISIGN_NAMESPACE(signature_to_bytes)

// Namespacing symbols exported from finit.c:
#undef ibz_mat_2x2_finalize
#undef ibz_mat_2x2_init
#undef ibz_mat_4x4_finalize
#undef ibz_mat_4x4_init
#undef ibz_vec_2_finalize
#undef ibz_vec_2_init
#undef ibz_vec_4_finalize
#undef ibz_vec_4_init
#undef quat_alg_elem_finalize
#undef quat_alg_elem_init
#undef quat_alg_finalize
#undef quat_alg_init_set
#undef quat_lattice_finalize
#undef quat_lattice_init
#undef quat_left_ideal_finalize
#undef quat_left_ideal_init

#define ibz_mat_2x2_finalize                            SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_finalize)
#define ibz_mat_2x2_init                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_init)
#define ibz_mat_4x4_finalize                            SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_finalize)
#define ibz_mat_4x4_init                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_init)
#define ibz_vec_2_finalize                              SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_finalize)
#define ibz_vec_2_init                                  SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_init)
#define ibz_vec_4_finalize                              SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_finalize)
#define ibz_vec_4_init                                  SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_init)
#define quat_alg_elem_finalize                          SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_finalize)
#define quat_alg_elem_init                              SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_init)
#define quat_alg_finalize                               SQISIGN_NAMESPACE_GENERIC(quat_alg_finalize)
#define quat_alg_init_set                               SQISIGN_NAMESPACE_GENERIC(quat_alg_init_set)
#define quat_lattice_finalize                           SQISIGN_NAMESPACE_GENERIC(quat_lattice_finalize)
#define quat_lattice_init                               SQISIGN_NAMESPACE_GENERIC(quat_lattice_init)
#define quat_left_ideal_finalize                        SQISIGN_NAMESPACE_GENERIC(quat_left_ideal_finalize)
#define quat_left_ideal_init                            SQISIGN_NAMESPACE_GENERIC(quat_left_ideal_init)

// Namespacing symbols exported from fp.c:
#undef fp_select
#undef p
#undef p2

#define fp_select                                       SQISIGN_NAMESPACE(fp_select)
#define p                                               SQISIGN_NAMESPACE(p)
#define p2                                              SQISIGN_NAMESPACE(p2)

// Namespacing symbols exported from fp.c, fp_p27500_64.c, fp_p5248_64.c, fp_p65376_64.c:
#undef fp_exp3div4
#undef fp_inv
#undef fp_is_square
#undef fp_sqrt

#define fp_exp3div4                                     SQISIGN_NAMESPACE(fp_exp3div4)
#define fp_inv                                          SQISIGN_NAMESPACE(fp_inv)
#define fp_is_square                                    SQISIGN_NAMESPACE(fp_is_square)
#define fp_sqrt                                         SQISIGN_NAMESPACE(fp_sqrt)

// Namespacing symbols exported from fp2.c:
#undef fp2_add
#undef fp2_add_one
#undef fp2_batched_inv
#undef fp2_copy
#undef fp2_cswap
#undef fp2_decode
#undef fp2_encode
#undef fp2_half
#undef fp2_inv
#undef fp2_is_equal
#undef fp2_is_one
#undef fp2_is_square
#undef fp2_is_zero
#undef fp2_mul
#undef fp2_mul_small
#undef fp2_neg
#undef fp2_pow_vartime
#undef fp2_print
#undef fp2_select
#undef fp2_set_one
#undef fp2_set_small
#undef fp2_set_zero
#undef fp2_sqr
#undef fp2_sqrt
#undef fp2_sqrt_verify
#undef fp2_sub

#define fp2_add                                         SQISIGN_NAMESPACE(fp2_add)
#define fp2_add_one                                     SQISIGN_NAMESPACE(fp2_add_one)
#define fp2_batched_inv                                 SQISIGN_NAMESPACE(fp2_batched_inv)
#define fp2_copy                                        SQISIGN_NAMESPACE(fp2_copy)
#define fp2_cswap                                       SQISIGN_NAMESPACE(fp2_cswap)
#define fp2_decode                                      SQISIGN_NAMESPACE(fp2_decode)
#define fp2_encode                                      SQISIGN_NAMESPACE(fp2_encode)
#define fp2_half                                        SQISIGN_NAMESPACE(fp2_half)
#define fp2_inv                                         SQISIGN_NAMESPACE(fp2_inv)
#define fp2_is_equal                                    SQISIGN_NAMESPACE(fp2_is_equal)
#define fp2_is_one                                      SQISIGN_NAMESPACE(fp2_is_one)
#define fp2_is_square                                   SQISIGN_NAMESPACE(fp2_is_square)
#define fp2_is_zero                                     SQISIGN_NAMESPACE(fp2_is_zero)
#define fp2_mul                                         SQISIGN_NAMESPACE(fp2_mul)
#define fp2_mul_small                                   SQISIGN_NAMESPACE(fp2_mul_small)
#define fp2_neg                                         SQISIGN_NAMESPACE(fp2_neg)
#define fp2_pow_vartime                                 SQISIGN_NAMESPACE(fp2_pow_vartime)
#define fp2_print                                       SQISIGN_NAMESPACE(fp2_print)
#define fp2_select                                      SQISIGN_NAMESPACE(fp2_select)
#define fp2_set_one                                     SQISIGN_NAMESPACE(fp2_set_one)
#define fp2_set_small                                   SQISIGN_NAMESPACE(fp2_set_small)
#define fp2_set_zero                                    SQISIGN_NAMESPACE(fp2_set_zero)
#define fp2_sqr                                         SQISIGN_NAMESPACE(fp2_sqr)
#define fp2_sqrt                                        SQISIGN_NAMESPACE(fp2_sqrt)
#define fp2_sqrt_verify                                 SQISIGN_NAMESPACE(fp2_sqrt_verify)
#define fp2_sub                                         SQISIGN_NAMESPACE(fp2_sub)

// Namespacing symbols exported from fp_p27500_64.c, fp_p5248_64.c, fp_p65376_64.c:
#undef fp_copy
#undef fp_cswap
#undef fp_decode
#undef fp_decode_reduce
#undef fp_div3
#undef fp_encode
#undef fp_half
#undef fp_is_equal
#undef fp_is_zero
#undef fp_mul_small
#undef fp_neg
#undef fp_set_one
#undef fp_set_small
#undef fp_set_zero

#define fp_copy                                         SQISIGN_NAMESPACE(fp_copy)
#define fp_cswap                                        SQISIGN_NAMESPACE(fp_cswap)
#define fp_decode                                       SQISIGN_NAMESPACE(fp_decode)
#define fp_decode_reduce                                SQISIGN_NAMESPACE(fp_decode_reduce)
#define fp_div3                                         SQISIGN_NAMESPACE(fp_div3)
#define fp_encode                                       SQISIGN_NAMESPACE(fp_encode)
#define fp_half                                         SQISIGN_NAMESPACE(fp_half)
#define fp_is_equal                                     SQISIGN_NAMESPACE(fp_is_equal)
#define fp_is_zero                                      SQISIGN_NAMESPACE(fp_is_zero)
#define fp_mul_small                                    SQISIGN_NAMESPACE(fp_mul_small)
#define fp_neg                                          SQISIGN_NAMESPACE(fp_neg)
#define fp_set_one                                      SQISIGN_NAMESPACE(fp_set_one)
#define fp_set_small                                    SQISIGN_NAMESPACE(fp_set_small)
#define fp_set_zero                                     SQISIGN_NAMESPACE(fp_set_zero)
#define ONE                                             SQISIGN_NAMESPACE(ONE)
#define ZERO                                            SQISIGN_NAMESPACE(ZERO)

// Namespacing symbols exported from fp_p27500_64.c, fp_p5248_64.c, fp_p65376_64.c, gf27500.c, gf5248.c, gf65376.c:
#undef fp_add
#undef fp_mul
#undef fp_sqr
#undef fp_sub

#define fp_add                                          SQISIGN_NAMESPACE(fp_add)
#define fp_mul                                          SQISIGN_NAMESPACE(fp_mul)
#define fp_sqr                                          SQISIGN_NAMESPACE(fp_sqr)
#define fp_sub                                          SQISIGN_NAMESPACE(fp_sub)

// Namespacing symbols exported from gf27500.c:
#undef gf27500_decode
#undef gf27500_decode_reduce
#undef gf27500_div
#undef gf27500_div3
#undef gf27500_encode
#undef gf27500_invert
#undef gf27500_legendre
#undef gf27500_sqrt

#define gf27500_decode                                  SQISIGN_NAMESPACE(gf27500_decode)
#define gf27500_decode_reduce                           SQISIGN_NAMESPACE(gf27500_decode_reduce)
#define gf27500_div                                     SQISIGN_NAMESPACE(gf27500_div)
#define gf27500_div3                                    SQISIGN_NAMESPACE(gf27500_div3)
#define gf27500_encode                                  SQISIGN_NAMESPACE(gf27500_encode)
#define gf27500_invert                                  SQISIGN_NAMESPACE(gf27500_invert)
#define gf27500_legendre                                SQISIGN_NAMESPACE(gf27500_legendre)
#define gf27500_sqrt                                    SQISIGN_NAMESPACE(gf27500_sqrt)

// Namespacing symbols exported from gf27500.c, gf5248.c, gf65376.c:
#undef fp2_mul_c0
#undef fp2_mul_c1
#undef fp2_sq_c0
#undef fp2_sq_c1

#define fp2_mul_c0                                      SQISIGN_NAMESPACE(fp2_mul_c0)
#define fp2_mul_c1                                      SQISIGN_NAMESPACE(fp2_mul_c1)
#define fp2_sq_c0                                       SQISIGN_NAMESPACE(fp2_sq_c0)
#define fp2_sq_c1                                       SQISIGN_NAMESPACE(fp2_sq_c1)

// Namespacing symbols exported from gf5248.c:
#undef gf5248_decode
#undef gf5248_decode_reduce
#undef gf5248_div
#undef gf5248_div3
#undef gf5248_encode
#undef gf5248_invert
#undef gf5248_legendre
#undef gf5248_sqrt

#define gf5248_decode                                   SQISIGN_NAMESPACE(gf5248_decode)
#define gf5248_decode_reduce                            SQISIGN_NAMESPACE(gf5248_decode_reduce)
#define gf5248_div                                      SQISIGN_NAMESPACE(gf5248_div)
#define gf5248_div3                                     SQISIGN_NAMESPACE(gf5248_div3)
#define gf5248_encode                                   SQISIGN_NAMESPACE(gf5248_encode)
#define gf5248_invert                                   SQISIGN_NAMESPACE(gf5248_invert)
#define gf5248_legendre                                 SQISIGN_NAMESPACE(gf5248_legendre)
#define gf5248_sqrt                                     SQISIGN_NAMESPACE(gf5248_sqrt)

// Namespacing symbols exported from gf65376.c:
#undef gf65376_decode
#undef gf65376_decode_reduce
#undef gf65376_div
#undef gf65376_div3
#undef gf65376_encode
#undef gf65376_invert
#undef gf65376_legendre
#undef gf65376_sqrt

#define gf65376_decode                                  SQISIGN_NAMESPACE(gf65376_decode)
#define gf65376_decode_reduce                           SQISIGN_NAMESPACE(gf65376_decode_reduce)
#define gf65376_div                                     SQISIGN_NAMESPACE(gf65376_div)
#define gf65376_div3                                    SQISIGN_NAMESPACE(gf65376_div3)
#define gf65376_encode                                  SQISIGN_NAMESPACE(gf65376_encode)
#define gf65376_invert                                  SQISIGN_NAMESPACE(gf65376_invert)
#define gf65376_legendre                                SQISIGN_NAMESPACE(gf65376_legendre)
#define gf65376_sqrt                                    SQISIGN_NAMESPACE(gf65376_sqrt)

// Namespacing symbols exported from hd.c:
#undef add_couple_jac_points
#undef copy_bases_to_kernel
#undef couple_jac_to_xz
#undef double_couple_jac_point
#undef double_couple_jac_point_iter
#undef double_couple_point
#undef double_couple_point_iter

#define add_couple_jac_points                           SQISIGN_NAMESPACE(add_couple_jac_points)
#define copy_bases_to_kernel                            SQISIGN_NAMESPACE(copy_bases_to_kernel)
#define couple_jac_to_xz                                SQISIGN_NAMESPACE(couple_jac_to_xz)
#define double_couple_jac_point                         SQISIGN_NAMESPACE(double_couple_jac_point)
#define double_couple_jac_point_iter                    SQISIGN_NAMESPACE(double_couple_jac_point_iter)
#define double_couple_point                             SQISIGN_NAMESPACE(double_couple_point)
#define double_couple_point_iter                        SQISIGN_NAMESPACE(double_couple_point_iter)

// Namespacing symbols exported from hnf.c:
#undef ibz_mat_4x4_is_hnf
#undef ibz_mat_4xn_hnf_mod_core
#undef ibz_vec_4_copy_mod
#undef ibz_vec_4_linear_combination_mod
#undef ibz_vec_4_scalar_mul_mod

#define ibz_mat_4x4_is_hnf                              SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_is_hnf)
#define ibz_mat_4xn_hnf_mod_core                        SQISIGN_NAMESPACE_GENERIC(ibz_mat_4xn_hnf_mod_core)
#define ibz_vec_4_copy_mod                              SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_copy_mod)
#define ibz_vec_4_linear_combination_mod                SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_linear_combination_mod)
#define ibz_vec_4_scalar_mul_mod                        SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_scalar_mul_mod)

// Namespacing symbols exported from hnf_internal.c:
#undef ibz_centered_mod
#undef ibz_conditional_assign
#undef ibz_mod_not_zero
#undef ibz_xgcd_with_u_not_0

#define ibz_centered_mod                                SQISIGN_NAMESPACE_GENERIC(ibz_centered_mod)
#define ibz_conditional_assign                          SQISIGN_NAMESPACE_GENERIC(ibz_conditional_assign)
#define ibz_mod_not_zero                                SQISIGN_NAMESPACE_GENERIC(ibz_mod_not_zero)
#define ibz_xgcd_with_u_not_0                           SQISIGN_NAMESPACE_GENERIC(ibz_xgcd_with_u_not_0)

// Namespacing symbols exported from ibz_division.c:
#undef ibz_xgcd

#define ibz_xgcd                                        SQISIGN_NAMESPACE_GENERIC(ibz_xgcd)

// Namespacing symbols exported from id2iso.c:
#undef change_of_basis_matrix_tate
#undef change_of_basis_matrix_tate_invert
#undef ec_biscalar_mul_ibz_vec
#undef endomorphism_application_even_basis
#undef id2iso_ideal_to_kernel_dlogs_even
#undef id2iso_kernel_dlogs_to_ideal_even
#undef matrix_application_even_basis

#define change_of_basis_matrix_tate                     SQISIGN_NAMESPACE(change_of_basis_matrix_tate)
#define change_of_basis_matrix_tate_invert              SQISIGN_NAMESPACE(change_of_basis_matrix_tate_invert)
#define ec_biscalar_mul_ibz_vec                         SQISIGN_NAMESPACE(ec_biscalar_mul_ibz_vec)
#define endomorphism_application_even_basis             SQISIGN_NAMESPACE(endomorphism_application_even_basis)
#define id2iso_ideal_to_kernel_dlogs_even               SQISIGN_NAMESPACE(id2iso_ideal_to_kernel_dlogs_even)
#define id2iso_kernel_dlogs_to_ideal_even               SQISIGN_NAMESPACE(id2iso_kernel_dlogs_to_ideal_even)
#define matrix_application_even_basis                   SQISIGN_NAMESPACE(matrix_application_even_basis)

// Namespacing symbols exported from ideal.c:
#undef quat_lideal_add
#undef quat_lideal_class_gram
#undef quat_lideal_conjugate_without_hnf
#undef quat_lideal_copy
#undef quat_lideal_create
#undef quat_lideal_create_principal
#undef quat_lideal_equals
#undef quat_lideal_generator
#undef quat_lideal_inter
#undef quat_lideal_inverse_lattice_without_hnf
#undef quat_lideal_mul
#undef quat_lideal_norm
#undef quat_lideal_right_order
#undef quat_lideal_right_transporter
#undef quat_order_discriminant
#undef quat_order_is_maximal

#define quat_lideal_add                                 SQISIGN_NAMESPACE_GENERIC(quat_lideal_add)
#define quat_lideal_class_gram                          SQISIGN_NAMESPACE_GENERIC(quat_lideal_class_gram)
#define quat_lideal_conjugate_without_hnf               SQISIGN_NAMESPACE_GENERIC(quat_lideal_conjugate_without_hnf)
#define quat_lideal_copy                                SQISIGN_NAMESPACE_GENERIC(quat_lideal_copy)
#define quat_lideal_create                              SQISIGN_NAMESPACE_GENERIC(quat_lideal_create)
#define quat_lideal_create_principal                    SQISIGN_NAMESPACE_GENERIC(quat_lideal_create_principal)
#define quat_lideal_equals                              SQISIGN_NAMESPACE_GENERIC(quat_lideal_equals)
#define quat_lideal_generator                           SQISIGN_NAMESPACE_GENERIC(quat_lideal_generator)
#define quat_lideal_inter                               SQISIGN_NAMESPACE_GENERIC(quat_lideal_inter)
#define quat_lideal_inverse_lattice_without_hnf         SQISIGN_NAMESPACE_GENERIC(quat_lideal_inverse_lattice_without_hnf)
#define quat_lideal_mul                                 SQISIGN_NAMESPACE_GENERIC(quat_lideal_mul)
#define quat_lideal_norm                                SQISIGN_NAMESPACE_GENERIC(quat_lideal_norm)
#define quat_lideal_right_order                         SQISIGN_NAMESPACE_GENERIC(quat_lideal_right_order)
#define quat_lideal_right_transporter                   SQISIGN_NAMESPACE_GENERIC(quat_lideal_right_transporter)
#define quat_order_discriminant                         SQISIGN_NAMESPACE_GENERIC(quat_order_discriminant)
#define quat_order_is_maximal                           SQISIGN_NAMESPACE_GENERIC(quat_order_is_maximal)

// Namespacing symbols exported from intbig.c:
#undef ibz_abs
#undef ibz_add
#undef ibz_bitsize
#undef ibz_cmp
#undef ibz_cmp_int32
#undef ibz_convert_to_str
#undef ibz_copy
#undef ibz_copy_digits
#undef ibz_div
#undef ibz_div_2exp
#undef ibz_div_floor
#undef ibz_divides
#undef ibz_finalize
#undef ibz_gcd
#undef ibz_get
#undef ibz_init
#undef ibz_invmod
#undef ibz_is_even
#undef ibz_is_odd
#undef ibz_is_one
#undef ibz_is_zero
#undef ibz_legendre
#undef ibz_mod
#undef ibz_mod_ui
#undef ibz_mul
#undef ibz_neg
#undef ibz_pow
#undef ibz_pow_mod
#undef ibz_print
#undef ibz_probab_prime
#undef ibz_rand_interval
#undef ibz_rand_interval_bits
#undef ibz_rand_interval_i
#undef ibz_rand_interval_minm_m
#undef ibz_set
#undef ibz_set_from_str
#undef ibz_size_in_base
#undef ibz_sqrt
#undef ibz_sqrt_floor
#undef ibz_sqrt_mod_p
#undef ibz_sub
#undef ibz_swap
#undef ibz_to_digits
#undef ibz_two_adic

#define ibz_abs                                         SQISIGN_NAMESPACE_GENERIC(ibz_abs)
#define ibz_add                                         SQISIGN_NAMESPACE_GENERIC(ibz_add)
#define ibz_bitsize                                     SQISIGN_NAMESPACE_GENERIC(ibz_bitsize)
#define ibz_cmp                                         SQISIGN_NAMESPACE_GENERIC(ibz_cmp)
#define ibz_cmp_int32                                   SQISIGN_NAMESPACE_GENERIC(ibz_cmp_int32)
#define ibz_convert_to_str                              SQISIGN_NAMESPACE_GENERIC(ibz_convert_to_str)
#define ibz_copy                                        SQISIGN_NAMESPACE_GENERIC(ibz_copy)
#define ibz_copy_digits                                 SQISIGN_NAMESPACE_GENERIC(ibz_copy_digits)
#define ibz_div                                         SQISIGN_NAMESPACE_GENERIC(ibz_div)
#define ibz_div_2exp                                    SQISIGN_NAMESPACE_GENERIC(ibz_div_2exp)
#define ibz_div_floor                                   SQISIGN_NAMESPACE_GENERIC(ibz_div_floor)
#define ibz_divides                                     SQISIGN_NAMESPACE_GENERIC(ibz_divides)
#define ibz_finalize                                    SQISIGN_NAMESPACE_GENERIC(ibz_finalize)
#define ibz_gcd                                         SQISIGN_NAMESPACE_GENERIC(ibz_gcd)
#define ibz_get                                         SQISIGN_NAMESPACE_GENERIC(ibz_get)
#define ibz_init                                        SQISIGN_NAMESPACE_GENERIC(ibz_init)
#define ibz_invmod                                      SQISIGN_NAMESPACE_GENERIC(ibz_invmod)
#define ibz_is_even                                     SQISIGN_NAMESPACE_GENERIC(ibz_is_even)
#define ibz_is_odd                                      SQISIGN_NAMESPACE_GENERIC(ibz_is_odd)
#define ibz_is_one                                      SQISIGN_NAMESPACE_GENERIC(ibz_is_one)
#define ibz_is_zero                                     SQISIGN_NAMESPACE_GENERIC(ibz_is_zero)
#define ibz_legendre                                    SQISIGN_NAMESPACE_GENERIC(ibz_legendre)
#define ibz_mod                                         SQISIGN_NAMESPACE_GENERIC(ibz_mod)
#define ibz_mod_ui                                      SQISIGN_NAMESPACE_GENERIC(ibz_mod_ui)
#define ibz_mul                                         SQISIGN_NAMESPACE_GENERIC(ibz_mul)
#define ibz_neg                                         SQISIGN_NAMESPACE_GENERIC(ibz_neg)
#define ibz_pow                                         SQISIGN_NAMESPACE_GENERIC(ibz_pow)
#define ibz_pow_mod                                     SQISIGN_NAMESPACE_GENERIC(ibz_pow_mod)
#define ibz_print                                       SQISIGN_NAMESPACE_GENERIC(ibz_print)
#define ibz_probab_prime                                SQISIGN_NAMESPACE_GENERIC(ibz_probab_prime)
#define ibz_rand_interval                               SQISIGN_NAMESPACE_GENERIC(ibz_rand_interval)
#define ibz_rand_interval_bits                          SQISIGN_NAMESPACE_GENERIC(ibz_rand_interval_bits)
#define ibz_rand_interval_i                             SQISIGN_NAMESPACE_GENERIC(ibz_rand_interval_i)
#define ibz_rand_interval_minm_m                        SQISIGN_NAMESPACE_GENERIC(ibz_rand_interval_minm_m)
#define ibz_set                                         SQISIGN_NAMESPACE_GENERIC(ibz_set)
#define ibz_set_from_str                                SQISIGN_NAMESPACE_GENERIC(ibz_set_from_str)
#define ibz_size_in_base                                SQISIGN_NAMESPACE_GENERIC(ibz_size_in_base)
#define ibz_sqrt                                        SQISIGN_NAMESPACE_GENERIC(ibz_sqrt)
#define ibz_sqrt_floor                                  SQISIGN_NAMESPACE_GENERIC(ibz_sqrt_floor)
#define ibz_sqrt_mod_p                                  SQISIGN_NAMESPACE_GENERIC(ibz_sqrt_mod_p)
#define ibz_sub                                         SQISIGN_NAMESPACE_GENERIC(ibz_sub)
#define ibz_swap                                        SQISIGN_NAMESPACE_GENERIC(ibz_swap)
#define ibz_to_digits                                   SQISIGN_NAMESPACE_GENERIC(ibz_to_digits)
#define ibz_two_adic                                    SQISIGN_NAMESPACE_GENERIC(ibz_two_adic)

// Namespacing symbols exported from integers.c:
#undef ibz_cornacchia_prime
#undef ibz_generate_random_prime

#define ibz_cornacchia_prime                            SQISIGN_NAMESPACE_GENERIC(ibz_cornacchia_prime)
#define ibz_generate_random_prime                       SQISIGN_NAMESPACE_GENERIC(ibz_generate_random_prime)

// Namespacing symbols exported from isog_chains.c:
#undef ec_eval_even
#undef ec_eval_small_chain
#undef ec_iso_eval
#undef ec_isomorphism

#define ec_eval_even                                    SQISIGN_NAMESPACE(ec_eval_even)
#define ec_eval_small_chain                             SQISIGN_NAMESPACE(ec_eval_small_chain)
#define ec_iso_eval                                     SQISIGN_NAMESPACE(ec_iso_eval)
#define ec_isomorphism                                  SQISIGN_NAMESPACE(ec_isomorphism)

// Namespacing symbols exported from keygen.c:
#undef protocols_keygen
#undef secret_key_finalize
#undef secret_key_init

#define protocols_keygen                                SQISIGN_NAMESPACE(protocols_keygen)
#define secret_key_finalize                             SQISIGN_NAMESPACE(secret_key_finalize)
#define secret_key_init                                 SQISIGN_NAMESPACE(secret_key_init)

// Namespacing symbols exported from l2.c:
#undef quat_lattice_lll
#undef quat_lll_core

#define quat_lattice_lll                                SQISIGN_NAMESPACE_GENERIC(quat_lattice_lll)
#define quat_lll_core                                   SQISIGN_NAMESPACE_GENERIC(quat_lll_core)

// Namespacing symbols exported from lat_ball.c:
#undef quat_lattice_bound_parallelogram
#undef quat_lattice_sample_from_ball

#define quat_lattice_bound_parallelogram                SQISIGN_NAMESPACE_GENERIC(quat_lattice_bound_parallelogram)
#define quat_lattice_sample_from_ball                   SQISIGN_NAMESPACE_GENERIC(quat_lattice_sample_from_ball)

// Namespacing symbols exported from lattice.c:
#undef quat_lattice_add
#undef quat_lattice_alg_elem_mul
#undef quat_lattice_conjugate_without_hnf
#undef quat_lattice_contains
#undef quat_lattice_dual_without_hnf
#undef quat_lattice_equal
#undef quat_lattice_gram
#undef quat_lattice_hnf
#undef quat_lattice_inclusion
#undef quat_lattice_index
#undef quat_lattice_intersect
#undef quat_lattice_mat_alg_coord_mul_without_hnf
#undef quat_lattice_mul
#undef quat_lattice_reduce_denom

#define quat_lattice_add                                SQISIGN_NAMESPACE_GENERIC(quat_lattice_add)
#define quat_lattice_alg_elem_mul                       SQISIGN_NAMESPACE_GENERIC(quat_lattice_alg_elem_mul)
#define quat_lattice_conjugate_without_hnf              SQISIGN_NAMESPACE_GENERIC(quat_lattice_conjugate_without_hnf)
#define quat_lattice_contains                           SQISIGN_NAMESPACE_GENERIC(quat_lattice_contains)
#define quat_lattice_dual_without_hnf                   SQISIGN_NAMESPACE_GENERIC(quat_lattice_dual_without_hnf)
#define quat_lattice_equal                              SQISIGN_NAMESPACE_GENERIC(quat_lattice_equal)
#define quat_lattice_gram                               SQISIGN_NAMESPACE_GENERIC(quat_lattice_gram)
#define quat_lattice_hnf                                SQISIGN_NAMESPACE_GENERIC(quat_lattice_hnf)
#define quat_lattice_inclusion                          SQISIGN_NAMESPACE_GENERIC(quat_lattice_inclusion)
#define quat_lattice_index                              SQISIGN_NAMESPACE_GENERIC(quat_lattice_index)
#define quat_lattice_intersect                          SQISIGN_NAMESPACE_GENERIC(quat_lattice_intersect)
#define quat_lattice_mat_alg_coord_mul_without_hnf      SQISIGN_NAMESPACE_GENERIC(quat_lattice_mat_alg_coord_mul_without_hnf)
#define quat_lattice_mul                                SQISIGN_NAMESPACE_GENERIC(quat_lattice_mul)
#define quat_lattice_reduce_denom                       SQISIGN_NAMESPACE_GENERIC(quat_lattice_reduce_denom)

// Namespacing symbols exported from lll_applications.c:
#undef quat_lideal_lideal_mul_reduced
#undef quat_lideal_prime_norm_reduced_equivalent
#undef quat_lideal_reduce_basis

#define quat_lideal_lideal_mul_reduced                  SQISIGN_NAMESPACE_GENERIC(quat_lideal_lideal_mul_reduced)
#define quat_lideal_prime_norm_reduced_equivalent       SQISIGN_NAMESPACE_GENERIC(quat_lideal_prime_norm_reduced_equivalent)
#define quat_lideal_reduce_basis                        SQISIGN_NAMESPACE_GENERIC(quat_lideal_reduce_basis)

// Namespacing symbols exported from lll_verification.c:
#undef ibq_vec_4_copy_ibz
#undef quat_lll_bilinear
#undef quat_lll_gram_schmidt_transposed_with_ibq
#undef quat_lll_set_ibq_parameters
#undef quat_lll_verify

#define ibq_vec_4_copy_ibz                              SQISIGN_NAMESPACE_GENERIC(ibq_vec_4_copy_ibz)
#define quat_lll_bilinear                               SQISIGN_NAMESPACE_GENERIC(quat_lll_bilinear)
#define quat_lll_gram_schmidt_transposed_with_ibq       SQISIGN_NAMESPACE_GENERIC(quat_lll_gram_schmidt_transposed_with_ibq)
#define quat_lll_set_ibq_parameters                     SQISIGN_NAMESPACE_GENERIC(quat_lll_set_ibq_parameters)
#define quat_lll_verify                                 SQISIGN_NAMESPACE_GENERIC(quat_lll_verify)

// Namespacing symbols exported from mem.c:
#undef sqisign_secure_clear
#undef sqisign_secure_free

#define sqisign_secure_clear                            SQISIGN_NAMESPACE_GENERIC(sqisign_secure_clear)
#define sqisign_secure_free                             SQISIGN_NAMESPACE_GENERIC(sqisign_secure_free)

// Namespacing symbols exported from mp.c:
#undef MUL
#undef mp_add
#undef mp_compare
#undef mp_copy
#undef mp_inv_2e
#undef mp_invert_matrix
#undef mp_is_one
#undef mp_is_zero
#undef mp_mod_2exp
#undef mp_mul
#undef mp_mul2
#undef mp_neg
#undef mp_print
#undef mp_shiftl
#undef mp_shiftr
#undef mp_sub
#undef multiple_mp_shiftl
#undef select_ct
#undef swap_ct

#define MUL                                             SQISIGN_NAMESPACE_GENERIC(MUL)
#define mp_add                                          SQISIGN_NAMESPACE_GENERIC(mp_add)
#define mp_compare                                      SQISIGN_NAMESPACE_GENERIC(mp_compare)
#define mp_copy                                         SQISIGN_NAMESPACE_GENERIC(mp_copy)
#define mp_inv_2e                                       SQISIGN_NAMESPACE_GENERIC(mp_inv_2e)
#define mp_invert_matrix                                SQISIGN_NAMESPACE_GENERIC(mp_invert_matrix)
#define mp_is_one                                       SQISIGN_NAMESPACE_GENERIC(mp_is_one)
#define mp_is_zero                                      SQISIGN_NAMESPACE_GENERIC(mp_is_zero)
#define mp_mod_2exp                                     SQISIGN_NAMESPACE_GENERIC(mp_mod_2exp)
#define mp_mul                                          SQISIGN_NAMESPACE_GENERIC(mp_mul)
#define mp_mul2                                         SQISIGN_NAMESPACE_GENERIC(mp_mul2)
#define mp_neg                                          SQISIGN_NAMESPACE_GENERIC(mp_neg)
#define mp_print                                        SQISIGN_NAMESPACE_GENERIC(mp_print)
#define mp_shiftl                                       SQISIGN_NAMESPACE_GENERIC(mp_shiftl)
#define mp_shiftr                                       SQISIGN_NAMESPACE_GENERIC(mp_shiftr)
#define mp_sub                                          SQISIGN_NAMESPACE_GENERIC(mp_sub)
#define multiple_mp_shiftl                              SQISIGN_NAMESPACE_GENERIC(multiple_mp_shiftl)
#define select_ct                                       SQISIGN_NAMESPACE_GENERIC(select_ct)
#define swap_ct                                         SQISIGN_NAMESPACE_GENERIC(swap_ct)

// Namespacing symbols exported from normeq.c:
#undef quat_change_to_O0_basis
#undef quat_lattice_O0_set
#undef quat_lattice_O0_set_extremal
#undef quat_order_elem_create
#undef quat_represent_integer
#undef quat_sampling_random_ideal_O0_given_norm

#define quat_change_to_O0_basis                         SQISIGN_NAMESPACE_GENERIC(quat_change_to_O0_basis)
#define quat_lattice_O0_set                             SQISIGN_NAMESPACE_GENERIC(quat_lattice_O0_set)
#define quat_lattice_O0_set_extremal                    SQISIGN_NAMESPACE_GENERIC(quat_lattice_O0_set_extremal)
#define quat_order_elem_create                          SQISIGN_NAMESPACE_GENERIC(quat_order_elem_create)
#define quat_represent_integer                          SQISIGN_NAMESPACE_GENERIC(quat_represent_integer)
#define quat_sampling_random_ideal_O0_given_norm        SQISIGN_NAMESPACE_GENERIC(quat_sampling_random_ideal_O0_given_norm)

// Namespacing symbols exported from printer.c:
#undef ibz_mat_2x2_print
#undef ibz_mat_4x4_print
#undef ibz_vec_2_print
#undef ibz_vec_4_print
#undef quat_alg_elem_print
#undef quat_alg_print
#undef quat_lattice_print
#undef quat_left_ideal_print

#define ibz_mat_2x2_print                               SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_print)
#define ibz_mat_4x4_print                               SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_print)
#define ibz_vec_2_print                                 SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_print)
#define ibz_vec_4_print                                 SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_print)
#define quat_alg_elem_print                             SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_print)
#define quat_alg_print                                  SQISIGN_NAMESPACE_GENERIC(quat_alg_print)
#define quat_lattice_print                              SQISIGN_NAMESPACE_GENERIC(quat_lattice_print)
#define quat_left_ideal_print                           SQISIGN_NAMESPACE_GENERIC(quat_left_ideal_print)

// Namespacing symbols exported from random_input_generation.c:
#undef quat_test_input_random_ideal_generation
#undef quat_test_input_random_ideal_lattice_generation
#undef quat_test_input_random_lattice_generation

#define quat_test_input_random_ideal_generation         SQISIGN_NAMESPACE_GENERIC(quat_test_input_random_ideal_generation)
#define quat_test_input_random_ideal_lattice_generation SQISIGN_NAMESPACE_GENERIC(quat_test_input_random_ideal_lattice_generation)
#define quat_test_input_random_lattice_generation       SQISIGN_NAMESPACE_GENERIC(quat_test_input_random_lattice_generation)

// Namespacing symbols exported from rationals.c:
#undef ibq_abs
#undef ibq_add
#undef ibq_cmp
#undef ibq_copy
#undef ibq_finalize
#undef ibq_init
#undef ibq_inv
#undef ibq_is_ibz
#undef ibq_is_one
#undef ibq_is_zero
#undef ibq_mat_4x4_finalize
#undef ibq_mat_4x4_init
#undef ibq_mat_4x4_print
#undef ibq_mul
#undef ibq_neg
#undef ibq_reduce
#undef ibq_set
#undef ibq_sub
#undef ibq_to_ibz
#undef ibq_vec_4_finalize
#undef ibq_vec_4_init
#undef ibq_vec_4_print

#define ibq_abs                                         SQISIGN_NAMESPACE_GENERIC(ibq_abs)
#define ibq_add                                         SQISIGN_NAMESPACE_GENERIC(ibq_add)
#define ibq_cmp                                         SQISIGN_NAMESPACE_GENERIC(ibq_cmp)
#define ibq_copy                                        SQISIGN_NAMESPACE_GENERIC(ibq_copy)
#define ibq_finalize                                    SQISIGN_NAMESPACE_GENERIC(ibq_finalize)
#define ibq_init                                        SQISIGN_NAMESPACE_GENERIC(ibq_init)
#define ibq_inv                                         SQISIGN_NAMESPACE_GENERIC(ibq_inv)
#define ibq_is_ibz                                      SQISIGN_NAMESPACE_GENERIC(ibq_is_ibz)
#define ibq_is_one                                      SQISIGN_NAMESPACE_GENERIC(ibq_is_one)
#define ibq_is_zero                                     SQISIGN_NAMESPACE_GENERIC(ibq_is_zero)
#define ibq_mat_4x4_finalize                            SQISIGN_NAMESPACE_GENERIC(ibq_mat_4x4_finalize)
#define ibq_mat_4x4_init                                SQISIGN_NAMESPACE_GENERIC(ibq_mat_4x4_init)
#define ibq_mat_4x4_print                               SQISIGN_NAMESPACE_GENERIC(ibq_mat_4x4_print)
#define ibq_mul                                         SQISIGN_NAMESPACE_GENERIC(ibq_mul)
#define ibq_neg                                         SQISIGN_NAMESPACE_GENERIC(ibq_neg)
#define ibq_reduce                                      SQISIGN_NAMESPACE_GENERIC(ibq_reduce)
#define ibq_set                                         SQISIGN_NAMESPACE_GENERIC(ibq_set)
#define ibq_sub                                         SQISIGN_NAMESPACE_GENERIC(ibq_sub)
#define ibq_to_ibz                                      SQISIGN_NAMESPACE_GENERIC(ibq_to_ibz)
#define ibq_vec_4_finalize                              SQISIGN_NAMESPACE_GENERIC(ibq_vec_4_finalize)
#define ibq_vec_4_init                                  SQISIGN_NAMESPACE_GENERIC(ibq_vec_4_init)
#define ibq_vec_4_print                                 SQISIGN_NAMESPACE_GENERIC(ibq_vec_4_print)

// Namespacing symbols exported from sign.c:
#undef protocols_sign

#define protocols_sign                                  SQISIGN_NAMESPACE(protocols_sign)

// Namespacing symbols exported from sqisign.c:
#undef sqisign_keypair
#undef sqisign_open
#undef sqisign_sign
#undef sqisign_sign_signature
#undef sqisign_verify
#undef sqisign_verify_signature

#define sqisign_keypair                                 SQISIGN_NAMESPACE(sqisign_keypair)
#define sqisign_open                                    SQISIGN_NAMESPACE(sqisign_open)
#define sqisign_sign                                    SQISIGN_NAMESPACE(sqisign_sign)
#define sqisign_sign_signature                          SQISIGN_NAMESPACE(sqisign_sign_signature)
#define sqisign_verify                                  SQISIGN_NAMESPACE(sqisign_verify)
#define sqisign_verify_signature                        SQISIGN_NAMESPACE(sqisign_verify_signature)

// Namespacing symbols exported from theta_isogenies.c:
#undef theta_chain_compute_and_eval
#undef theta_chain_compute_and_eval_randomized
#undef theta_chain_compute_and_eval_verify

#define theta_chain_compute_and_eval                    SQISIGN_NAMESPACE(theta_chain_compute_and_eval)
#define theta_chain_compute_and_eval_randomized         SQISIGN_NAMESPACE(theta_chain_compute_and_eval_randomized)
#define theta_chain_compute_and_eval_verify             SQISIGN_NAMESPACE(theta_chain_compute_and_eval_verify)

// Namespacing symbols exported from theta_structure.c:
#undef double_iter
#undef double_point
#undef is_product_theta_point
#undef theta_precomputation

#define double_iter                                     SQISIGN_NAMESPACE(double_iter)
#define double_point                                    SQISIGN_NAMESPACE(double_point)
#define is_product_theta_point                          SQISIGN_NAMESPACE(is_product_theta_point)
#define theta_precomputation                            SQISIGN_NAMESPACE(theta_precomputation)

// Namespacing symbols exported from verify.c:
#undef protocols_verify

#define protocols_verify                                SQISIGN_NAMESPACE(protocols_verify)

// Namespacing symbols exported from xeval.c:
#undef xeval_2
#undef xeval_2_singular
#undef xeval_4

#define xeval_2                                         SQISIGN_NAMESPACE(xeval_2)
#define xeval_2_singular                                SQISIGN_NAMESPACE(xeval_2_singular)
#define xeval_4                                         SQISIGN_NAMESPACE(xeval_4)

// Namespacing symbols exported from xisog.c:
#undef xisog_2
#undef xisog_2_singular
#undef xisog_4

#define xisog_2                                         SQISIGN_NAMESPACE(xisog_2)
#define xisog_2_singular                                SQISIGN_NAMESPACE(xisog_2_singular)
#define xisog_4                                         SQISIGN_NAMESPACE(xisog_4)

// Namespacing symbols from precomp:
#undef BASIS_E0_PX
#undef BASIS_E0_QX
#undef p_cofactor_for_2f
#undef CURVES_WITH_ENDOMORPHISMS
#undef EVEN_INDEX
#undef CHI_EVAL
#undef FP2_CONSTANTS
#undef SPLITTING_TRANSFORMS
#undef NORMALIZATION_TRANSFORMS
#undef QUAT_prime_cofactor
#undef QUATALG_PINFTY
#undef EXTREMAL_ORDERS
#undef CONNECTING_IDEALS
#undef CONJUGATING_ELEMENTS
#undef TWO_TO_SECURITY_BITS
#undef TORSION_PLUS_2POWER
#undef SEC_DEGREE
#undef COM_DEGREE

#define BASIS_E0_PX                                     SQISIGN_NAMESPACE(BASIS_E0_PX)
#define BASIS_E0_QX                                     SQISIGN_NAMESPACE(BASIS_E0_QX)
#define p_cofactor_for_2f                               SQISIGN_NAMESPACE(p_cofactor_for_2f)
#define CURVES_WITH_ENDOMORPHISMS                       SQISIGN_NAMESPACE(CURVES_WITH_ENDOMORPHISMS)
#define EVEN_INDEX                                      SQISIGN_NAMESPACE(EVEN_INDEX)
#define CHI_EVAL                                        SQISIGN_NAMESPACE(CHI_EVAL)
#define FP2_CONSTANTS                                   SQISIGN_NAMESPACE(FP2_CONSTANTS)
#define SPLITTING_TRANSFORMS                            SQISIGN_NAMESPACE(SPLITTING_TRANSFORMS)
#define NORMALIZATION_TRANSFORMS                        SQISIGN_NAMESPACE(NORMALIZATION_TRANSFORMS)
#define QUAT_prime_cofactor                             SQISIGN_NAMESPACE(QUAT_prime_cofactor)
#define QUATALG_PINFTY                                  SQISIGN_NAMESPACE(QUATALG_PINFTY)
#define EXTREMAL_ORDERS                                 SQISIGN_NAMESPACE(EXTREMAL_ORDERS)
#define CONNECTING_IDEALS                               SQISIGN_NAMESPACE(CONNECTING_IDEALS)
#define CONJUGATING_ELEMENTS                            SQISIGN_NAMESPACE(CONJUGATING_ELEMENTS)
#define TWO_TO_SECURITY_BITS                            SQISIGN_NAMESPACE(TWO_TO_SECURITY_BITS)
#define TORSION_PLUS_2POWER                             SQISIGN_NAMESPACE(TORSION_PLUS_2POWER)
#define SEC_DEGREE                                      SQISIGN_NAMESPACE(SEC_DEGREE)
#define COM_DEGREE                                      SQISIGN_NAMESPACE(COM_DEGREE)


#endif

