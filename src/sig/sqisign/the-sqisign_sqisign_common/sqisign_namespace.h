
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

#ifdef DISABLE_NAMESPACING
#define SQISIGN_NAMESPACE_GENERIC(s) s
#elif defined(SQISIGN_NAMESPACE_GENERIC_AS_VARIANT)
// One binary can carry several parameter sets; generic symbols are
// level-dependent (bounded ibz sizes), so they need the variant prefix too.
#define SQISIGN_NAMESPACE_GENERIC(s) SQISIGN_NAMESPACE(s)
#else
#define SQISIGN_NAMESPACE_GENERIC(s) PARAM_NAME2(gen, s)
#endif

#if defined(SQISIGN_VARIANT) && !defined(DISABLE_NAMESPACING)
#if defined(SQISIGN_BUILD_TYPE_REF)
#define SQISIGN_NAMESPACE(s) PARAM_NAME3(ref, s)
#elif defined(SQISIGN_BUILD_TYPE_OPT)
#define SQISIGN_NAMESPACE(s) PARAM_NAME3(opt, s)
#elif defined(SQISIGN_BUILD_TYPE_BROADWELL)
#define SQISIGN_NAMESPACE(s) PARAM_NAME3(broadwell, s)
#elif defined(SQISIGN_BUILD_TYPE_ARM64)
#define SQISIGN_NAMESPACE(s) PARAM_NAME3(arm64, s)
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
#undef quat_alg_elem_scalar_mul
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
#define quat_alg_elem_scalar_mul                        SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_scalar_mul)
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
#undef crypto_sign_signature
#undef crypto_sign_verify

#define crypto_sign                                     SQISIGN_NAMESPACE(crypto_sign)
#define crypto_sign_keypair                             SQISIGN_NAMESPACE(crypto_sign_keypair)
#define crypto_sign_open                                SQISIGN_NAMESPACE(crypto_sign_open)
#define crypto_sign_signature                           SQISIGN_NAMESPACE(crypto_sign_signature)
#define crypto_sign_verify                              SQISIGN_NAMESPACE(crypto_sign_verify)

// Namespacing symbols exported from basis.c:
#undef ec_curve_to_basis_2f_from_hint
#undef ec_curve_to_basis_2f_to_hint

#define ec_curve_to_basis_2f_from_hint                  SQISIGN_NAMESPACE(ec_curve_to_basis_2f_from_hint)
#define ec_curve_to_basis_2f_to_hint                    SQISIGN_NAMESPACE(ec_curve_to_basis_2f_to_hint)

// Namespacing symbols exported from biextension.c:
#undef pairing_dlog_2_tate
#undef pairing_reduced_tate

#define pairing_dlog_2_tate                             SQISIGN_NAMESPACE(pairing_dlog_2_tate)
#define pairing_reduced_tate                            SQISIGN_NAMESPACE(pairing_reduced_tate)

// Namespacing symbols exported from common.c:
#undef hash_to_challenge
#undef public_key_init

#define hash_to_challenge                               SQISIGN_NAMESPACE(hash_to_challenge)
#define public_key_init                                 SQISIGN_NAMESPACE(public_key_init)

// Namespacing symbols exported from dim2.c:
#undef ibz_mat_2x2_copy
#undef ibz_mat_2x2_det_from_ibz
#undef ibz_mat_2x2_eval
#undef ibz_mat_2x2_inv_mod
#undef ibz_mat_2x2_inv_with_det_as_denom
#undef ibz_mat_2x2_mul
#undef ibz_mat_2x2_mul_mod
#undef ibz_mat_2x2_normalize
#undef ibz_mat_2x2_scalar_mul
#undef ibz_mat_2x2_set
#undef ibz_vec_2_add
#undef ibz_vec_2_copy
#undef ibz_vec_2_gaussian_euclidean_division
#undef ibz_vec_2_gaussian_gcd
#undef ibz_vec_2_gaussian_is_unit
#undef ibz_vec_2_gaussian_mul
#undef ibz_vec_2_is_zero
#undef ibz_vec_2_set
#undef ibz_vec_2_sub

#define ibz_mat_2x2_copy                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_copy)
#define ibz_mat_2x2_det_from_ibz                        SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_det_from_ibz)
#define ibz_mat_2x2_eval                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_eval)
#define ibz_mat_2x2_inv_mod                             SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_inv_mod)
#define ibz_mat_2x2_inv_with_det_as_denom               SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_inv_with_det_as_denom)
#define ibz_mat_2x2_mul                                 SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_mul)
#define ibz_mat_2x2_mul_mod                             SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_mul_mod)
#define ibz_mat_2x2_normalize                           SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_normalize)
#define ibz_mat_2x2_scalar_mul                          SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_scalar_mul)
#define ibz_mat_2x2_set                                 SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_set)
#define ibz_vec_2_add                                   SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_add)
#define ibz_vec_2_copy                                  SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_copy)
#define ibz_vec_2_gaussian_euclidean_division           SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_gaussian_euclidean_division)
#define ibz_vec_2_gaussian_gcd                          SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_gaussian_gcd)
#define ibz_vec_2_gaussian_is_unit                      SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_gaussian_is_unit)
#define ibz_vec_2_gaussian_mul                          SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_gaussian_mul)
#define ibz_vec_2_is_zero                               SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_is_zero)
#define ibz_vec_2_set                                   SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_set)
#define ibz_vec_2_sub                                   SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_sub)

// Namespacing symbols exported from dim2id2iso.c:
#undef dim2id2iso_arbitrary_isogeny_evaluation
#undef dim2id2iso_ideal_to_isogeny_qlapoty

#define dim2id2iso_arbitrary_isogeny_evaluation         SQISIGN_NAMESPACE(dim2id2iso_arbitrary_isogeny_evaluation)
#define dim2id2iso_ideal_to_isogeny_qlapoty             SQISIGN_NAMESPACE(dim2id2iso_ideal_to_isogeny_qlapoty)

// Namespacing symbols exported from dim4.c:
#undef ibz_mat_4x4_copy
#undef ibz_mat_4x4_equal
#undef ibz_mat_4x4_eval
#undef ibz_mat_4x4_eval_t
#undef ibz_mat_4x4_gcd
#undef ibz_mat_4x4_identity
#undef ibz_mat_4x4_inv_make_coeff_mpm
#undef ibz_mat_4x4_inv_make_coeff_pmp
#undef ibz_mat_4x4_inv_with_det_as_denom
#undef ibz_mat_4x4_mul
#undef ibz_mat_4x4_scalar_div
#undef ibz_mat_4x4_scalar_mul
#undef ibz_mat_4x4_transpose
#undef ibz_mat_4x4_zero
#undef ibz_vec_4_add
#undef ibz_vec_4_content
#undef ibz_vec_4_copy
#undef ibz_vec_4_copy_ibz
#undef ibz_vec_4_is_zero
#undef ibz_vec_4_scalar_div
#undef ibz_vec_4_scalar_mul
#undef ibz_vec_4_set
#undef ibz_vec_4_sub
#undef quat_qf_eval

#define ibz_mat_4x4_copy                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_copy)
#define ibz_mat_4x4_equal                               SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_equal)
#define ibz_mat_4x4_eval                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_eval)
#define ibz_mat_4x4_eval_t                              SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_eval_t)
#define ibz_mat_4x4_gcd                                 SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_gcd)
#define ibz_mat_4x4_identity                            SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_identity)
#define ibz_mat_4x4_inv_make_coeff_mpm                  SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_inv_make_coeff_mpm)
#define ibz_mat_4x4_inv_make_coeff_pmp                  SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_inv_make_coeff_pmp)
#define ibz_mat_4x4_inv_with_det_as_denom               SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_inv_with_det_as_denom)
#define ibz_mat_4x4_mul                                 SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_mul)
#define ibz_mat_4x4_scalar_div                          SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_scalar_div)
#define ibz_mat_4x4_scalar_mul                          SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_scalar_mul)
#define ibz_mat_4x4_transpose                           SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_transpose)
#define ibz_mat_4x4_zero                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_zero)
#define ibz_vec_4_add                                   SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_add)
#define ibz_vec_4_content                               SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_content)
#define ibz_vec_4_copy                                  SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_copy)
#define ibz_vec_4_copy_ibz                              SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_copy_ibz)
#define ibz_vec_4_is_zero                               SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_is_zero)
#define ibz_vec_4_scalar_div                            SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_scalar_div)
#define ibz_vec_4_scalar_mul                            SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_scalar_mul)
#define ibz_vec_4_set                                   SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_set)
#define ibz_vec_4_sub                                   SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_sub)
#define quat_qf_eval                                    SQISIGN_NAMESPACE_GENERIC(quat_qf_eval)

// Namespacing symbols exported from e0_basis.c:
#undef BASIS_E0_PX
#undef BASIS_E0_QX

#define BASIS_E0_PX                                     SQISIGN_NAMESPACE(BASIS_E0_PX)
#define BASIS_E0_QX                                     SQISIGN_NAMESPACE(BASIS_E0_QX)

// Namespacing symbols exported from ec.c:
#undef ec_biscalar_mul
#undef ec_biscalar_mul_verif
#undef ec_curve_init
#undef ec_curve_init_from_A
#undef ec_curve_normalize_A24
#undef ec_curve_verify_A
#undef ec_dbl
#undef ec_dbl_iter
#undef ec_dbl_iter_basis
#undef ec_has_zero_coordinate
#undef ec_is_equal
#undef ec_is_four_torsion
#undef ec_is_two_torsion
#undef ec_is_zero
#undef ec_ladder3pt
#undef ec_mul
#undef ec_normalize_curve
#undef ec_normalize_curve_and_A24
#undef ec_normalize_point
#undef ec_point_init
#undef ec_points_to_bary_coordinates
#undef ec_xADD
#undef ec_xDBL
#undef ec_xDBLADD
#undef ec_xDBL_A24
#undef ec_xDBL_E0

#define ec_biscalar_mul                                 SQISIGN_NAMESPACE(ec_biscalar_mul)
#define ec_biscalar_mul_verif                           SQISIGN_NAMESPACE(ec_biscalar_mul_verif)
#define ec_curve_init                                   SQISIGN_NAMESPACE(ec_curve_init)
#define ec_curve_init_from_A                            SQISIGN_NAMESPACE(ec_curve_init_from_A)
#define ec_curve_normalize_A24                          SQISIGN_NAMESPACE(ec_curve_normalize_A24)
#define ec_curve_verify_A                               SQISIGN_NAMESPACE(ec_curve_verify_A)
#define ec_dbl                                          SQISIGN_NAMESPACE(ec_dbl)
#define ec_dbl_iter                                     SQISIGN_NAMESPACE(ec_dbl_iter)
#define ec_dbl_iter_basis                               SQISIGN_NAMESPACE(ec_dbl_iter_basis)
#define ec_has_zero_coordinate                          SQISIGN_NAMESPACE(ec_has_zero_coordinate)
#define ec_is_equal                                     SQISIGN_NAMESPACE(ec_is_equal)
#define ec_is_four_torsion                              SQISIGN_NAMESPACE(ec_is_four_torsion)
#define ec_is_two_torsion                               SQISIGN_NAMESPACE(ec_is_two_torsion)
#define ec_is_zero                                      SQISIGN_NAMESPACE(ec_is_zero)
#define ec_ladder3pt                                    SQISIGN_NAMESPACE(ec_ladder3pt)
#define ec_mul                                          SQISIGN_NAMESPACE(ec_mul)
#define ec_normalize_curve                              SQISIGN_NAMESPACE(ec_normalize_curve)
#define ec_normalize_curve_and_A24                      SQISIGN_NAMESPACE(ec_normalize_curve_and_A24)
#define ec_normalize_point                              SQISIGN_NAMESPACE(ec_normalize_point)
#define ec_point_init                                   SQISIGN_NAMESPACE(ec_point_init)
#define ec_points_to_bary_coordinates                   SQISIGN_NAMESPACE(ec_points_to_bary_coordinates)
#define ec_xADD                                         SQISIGN_NAMESPACE(ec_xADD)
#define ec_xDBL                                         SQISIGN_NAMESPACE(ec_xDBL)
#define ec_xDBLADD                                      SQISIGN_NAMESPACE(ec_xDBLADD)
#define ec_xDBL_A24                                     SQISIGN_NAMESPACE(ec_xDBL_A24)
#define ec_xDBL_E0                                      SQISIGN_NAMESPACE(ec_xDBL_E0)

// Namespacing symbols exported from ec_params.c:
#undef p_cofactor_for_2f

#define p_cofactor_for_2f                               SQISIGN_NAMESPACE(p_cofactor_for_2f)

// Namespacing symbols exported from encode_public.c:
#undef public_key_from_bytes
#undef public_key_to_bytes
#undef signature_from_bytes
#undef signature_to_bytes

#define public_key_from_bytes                           SQISIGN_NAMESPACE(public_key_from_bytes)
#define public_key_to_bytes                             SQISIGN_NAMESPACE(public_key_to_bytes)
#define signature_from_bytes                            SQISIGN_NAMESPACE(signature_from_bytes)
#define signature_to_bytes                              SQISIGN_NAMESPACE(signature_to_bytes)

// Namespacing symbols exported from encode_secret.c:
#undef secret_key_from_bytes
#undef secret_key_to_bytes

#define secret_key_from_bytes                           SQISIGN_NAMESPACE(secret_key_from_bytes)
#define secret_key_to_bytes                             SQISIGN_NAMESPACE(secret_key_to_bytes)

// Namespacing symbols exported from endomorphism_action.c:
#undef CURVE_WITH_ENDOMORPHISMS

#define CURVE_WITH_ENDOMORPHISMS                        SQISIGN_NAMESPACE(CURVE_WITH_ENDOMORPHISMS)

// Namespacing symbols exported from finit.c:
#undef ibz_mat_2x2_init
#undef ibz_mat_4x4_init
#undef ibz_vec_2_init
#undef ibz_vec_4_init
#undef quat_alg_elem_init
#undef quat_alg_init_set
#undef quat_ideal_init
#undef quat_lattice_init

#define ibz_mat_2x2_init                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_init)
#define ibz_mat_4x4_init                                SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_init)
#define ibz_vec_2_init                                  SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_init)
#define ibz_vec_4_init                                  SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_init)
#define quat_alg_elem_init                              SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_init)
#define quat_alg_init_set                               SQISIGN_NAMESPACE_GENERIC(quat_alg_init_set)
#define quat_ideal_init                                 SQISIGN_NAMESPACE_GENERIC(quat_ideal_init)
#define quat_lattice_init                               SQISIGN_NAMESPACE_GENERIC(quat_lattice_init)

// Namespacing symbols exported from fp.c:
#undef p
#undef p2

#define p                                               SQISIGN_NAMESPACE(p)
#define p2                                              SQISIGN_NAMESPACE(p2)

// Namespacing symbols exported from fp.c, fp_generic.c:
#undef fp_select

#define fp_select                                       SQISIGN_NAMESPACE(fp_select)

// Namespacing symbols exported from fp2.c:
#undef fp2_add
#undef fp2_add_one
#undef fp2_batched_inv
#undef fp2_copy
#undef fp2_cswap
#undef fp2_decode
#undef fp2_encode
#undef fp2_frob
#undef fp2_half
#undef fp2_inv
#undef fp2_is_equal
#undef fp2_is_one
#undef fp2_is_square
#undef fp2_is_zero
#undef fp2_less_than
#undef fp2_mul
#undef fp2_mul_by_i
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
#define fp2_frob                                        SQISIGN_NAMESPACE(fp2_frob)
#define fp2_half                                        SQISIGN_NAMESPACE(fp2_half)
#define fp2_inv                                         SQISIGN_NAMESPACE(fp2_inv)
#define fp2_is_equal                                    SQISIGN_NAMESPACE(fp2_is_equal)
#define fp2_is_one                                      SQISIGN_NAMESPACE(fp2_is_one)
#define fp2_is_square                                   SQISIGN_NAMESPACE(fp2_is_square)
#define fp2_is_zero                                     SQISIGN_NAMESPACE(fp2_is_zero)
#define fp2_less_than                                   SQISIGN_NAMESPACE(fp2_less_than)
#define fp2_mul                                         SQISIGN_NAMESPACE(fp2_mul)
#define fp2_mul_by_i                                    SQISIGN_NAMESPACE(fp2_mul_by_i)
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

// Namespacing symbols exported from fp_generic.c:
#undef fp2_mul_c0
#undef fp2_mul_c1
#undef fp2_sq_c0
#undef fp2_sq_c1
#undef fpg_ONE
#undef fpg_ZERO

#define fp2_mul_c0                                      SQISIGN_NAMESPACE(fp2_mul_c0)
#define fp2_mul_c1                                      SQISIGN_NAMESPACE(fp2_mul_c1)
#define fp2_sq_c0                                       SQISIGN_NAMESPACE(fp2_sq_c0)
#define fp2_sq_c1                                       SQISIGN_NAMESPACE(fp2_sq_c1)
#define fpg_ONE                                         SQISIGN_NAMESPACE(fpg_ONE)
#define fpg_ZERO                                        SQISIGN_NAMESPACE(fpg_ZERO)

// Namespacing symbols exported from fp_generic.c, fp_p324_3.c, fp_p500_27.c, fp_p664_17.c:
#undef fp_add
#undef fp_copy
#undef fp_cswap
#undef fp_decode
#undef fp_decode_reduce
#undef fp_div3
#undef fp_encode
#undef fp_exp3div4
#undef fp_half
#undef fp_inv
#undef fp_is_equal
#undef fp_is_square
#undef fp_is_zero
#undef fp_mul
#undef fp_mul_small
#undef fp_neg
#undef fp_set_one
#undef fp_set_small
#undef fp_set_zero
#undef fp_sqr
#undef fp_sqrt
#undef fp_sub

#define fp_add                                          SQISIGN_NAMESPACE(fp_add)
#define fp_copy                                         SQISIGN_NAMESPACE(fp_copy)
#define fp_cswap                                        SQISIGN_NAMESPACE(fp_cswap)
#define fp_decode                                       SQISIGN_NAMESPACE(fp_decode)
#define fp_decode_reduce                                SQISIGN_NAMESPACE(fp_decode_reduce)
#define fp_div3                                         SQISIGN_NAMESPACE(fp_div3)
#define fp_encode                                       SQISIGN_NAMESPACE(fp_encode)
#define fp_exp3div4                                     SQISIGN_NAMESPACE(fp_exp3div4)
#define fp_half                                         SQISIGN_NAMESPACE(fp_half)
#define fp_inv                                          SQISIGN_NAMESPACE(fp_inv)
#define fp_is_equal                                     SQISIGN_NAMESPACE(fp_is_equal)
#define fp_is_square                                    SQISIGN_NAMESPACE(fp_is_square)
#define fp_is_zero                                      SQISIGN_NAMESPACE(fp_is_zero)
#define fp_mul                                          SQISIGN_NAMESPACE(fp_mul)
#define fp_mul_small                                    SQISIGN_NAMESPACE(fp_mul_small)
#define fp_neg                                          SQISIGN_NAMESPACE(fp_neg)
#define fp_set_one                                      SQISIGN_NAMESPACE(fp_set_one)
#define fp_set_small                                    SQISIGN_NAMESPACE(fp_set_small)
#define fp_set_zero                                     SQISIGN_NAMESPACE(fp_set_zero)
#define fp_sqr                                          SQISIGN_NAMESPACE(fp_sqr)
#define fp_sqrt                                         SQISIGN_NAMESPACE(fp_sqrt)
#define fp_sub                                          SQISIGN_NAMESPACE(fp_sub)

// Namespacing symbols exported from fp_p324_3.c, fp_p500_27.c, fp_p664_17.c:
#undef ONE
#undef ZERO

#define ONE                                             SQISIGN_NAMESPACE(ONE)
#define ZERO                                            SQISIGN_NAMESPACE(ZERO)

// Namespacing symbols exported from gaussian_xgcd.c:
#undef quat_lll_reduce_O0_ideal

#define quat_lll_reduce_O0_ideal                        SQISIGN_NAMESPACE(quat_lll_reduce_O0_ideal)

// Namespacing symbols exported from gluing.c:
#undef gluing_basis_compute
#undef gluing_compute
#undef gluing_couple_point_to_theta
#undef gluing_eval_point_bary
#undef gluing_eval_point_special_case
#undef gluing_ladder
#undef gluing_start_chain

#define gluing_basis_compute                            SQISIGN_NAMESPACE(gluing_basis_compute)
#define gluing_compute                                  SQISIGN_NAMESPACE(gluing_compute)
#define gluing_couple_point_to_theta                    SQISIGN_NAMESPACE(gluing_couple_point_to_theta)
#define gluing_eval_point_bary                          SQISIGN_NAMESPACE(gluing_eval_point_bary)
#define gluing_eval_point_special_case                  SQISIGN_NAMESPACE(gluing_eval_point_special_case)
#define gluing_ladder                                   SQISIGN_NAMESPACE(gluing_ladder)
#define gluing_start_chain                              SQISIGN_NAMESPACE(gluing_start_chain)

// Namespacing symbols exported from hd.c:
#undef theta_DBL_couple_point
#undef theta_DBL_couple_point_iter
#undef theta_copy_bases_to_kernel

#define theta_DBL_couple_point                          SQISIGN_NAMESPACE(theta_DBL_couple_point)
#define theta_DBL_couple_point_iter                     SQISIGN_NAMESPACE(theta_DBL_couple_point_iter)
#define theta_copy_bases_to_kernel                      SQISIGN_NAMESPACE(theta_copy_bases_to_kernel)

// Namespacing symbols exported from id2iso.c:
#undef id2iso_change_of_basis_matrix_tate
#undef id2iso_change_of_basis_matrix_tate_invert
#undef id2iso_endomorphism_application_even_basis
#undef id2iso_kernel_dlogs_to_ideal_even
#undef id2iso_matrix_application_even_basis

#define id2iso_change_of_basis_matrix_tate              SQISIGN_NAMESPACE(id2iso_change_of_basis_matrix_tate)
#define id2iso_change_of_basis_matrix_tate_invert       SQISIGN_NAMESPACE(id2iso_change_of_basis_matrix_tate_invert)
#define id2iso_endomorphism_application_even_basis      SQISIGN_NAMESPACE(id2iso_endomorphism_application_even_basis)
#define id2iso_kernel_dlogs_to_ideal_even               SQISIGN_NAMESPACE(id2iso_kernel_dlogs_to_ideal_even)
#define id2iso_matrix_application_even_basis            SQISIGN_NAMESPACE(id2iso_matrix_application_even_basis)

// Namespacing symbols exported from ideal.c:
#undef quat_ideal_copy
#undef quat_ideal_create_O0_inert
#undef quat_ideal_create_O0_inert_odd
#undef quat_ideal_create_O0_odd
#undef quat_ideal_create_O0_pow_two
#undef quat_ideal_intersect_O0
#undef quat_ideal_mul_O0
#undef quat_ideal_odd_inert_gen
#undef quat_ideal_product_gram_matrix
#undef quat_ideal_shortest_equivalent
#undef quat_ideal_small_equivalent_coprime
#undef quat_ideal_small_equivalent_coprime_enumeration
#undef quat_random_ideal_O0_given_arbitrary_odd_norm
#undef quat_random_ideal_O0_given_prime_norm
#undef quat_to_lattice
#undef quat_xyn_to_inert_ideal

#define quat_ideal_copy                                 SQISIGN_NAMESPACE_GENERIC(quat_ideal_copy)
#define quat_ideal_create_O0_inert                      SQISIGN_NAMESPACE_GENERIC(quat_ideal_create_O0_inert)
#define quat_ideal_create_O0_inert_odd                  SQISIGN_NAMESPACE_GENERIC(quat_ideal_create_O0_inert_odd)
#define quat_ideal_create_O0_odd                        SQISIGN_NAMESPACE_GENERIC(quat_ideal_create_O0_odd)
#define quat_ideal_create_O0_pow_two                    SQISIGN_NAMESPACE_GENERIC(quat_ideal_create_O0_pow_two)
#define quat_ideal_intersect_O0                         SQISIGN_NAMESPACE_GENERIC(quat_ideal_intersect_O0)
#define quat_ideal_mul_O0                               SQISIGN_NAMESPACE_GENERIC(quat_ideal_mul_O0)
#define quat_ideal_odd_inert_gen                        SQISIGN_NAMESPACE_GENERIC(quat_ideal_odd_inert_gen)
#define quat_ideal_product_gram_matrix                  SQISIGN_NAMESPACE_GENERIC(quat_ideal_product_gram_matrix)
#define quat_ideal_shortest_equivalent                  SQISIGN_NAMESPACE_GENERIC(quat_ideal_shortest_equivalent)
#define quat_ideal_small_equivalent_coprime             SQISIGN_NAMESPACE_GENERIC(quat_ideal_small_equivalent_coprime)
#define quat_ideal_small_equivalent_coprime_enumeration SQISIGN_NAMESPACE_GENERIC(quat_ideal_small_equivalent_coprime_enumeration)
#define quat_random_ideal_O0_given_arbitrary_odd_norm   SQISIGN_NAMESPACE_GENERIC(quat_random_ideal_O0_given_arbitrary_odd_norm)
#define quat_random_ideal_O0_given_prime_norm           SQISIGN_NAMESPACE_GENERIC(quat_random_ideal_O0_given_prime_norm)
#define quat_to_lattice                                 SQISIGN_NAMESPACE_GENERIC(quat_to_lattice)
#define quat_xyn_to_inert_ideal                         SQISIGN_NAMESPACE_GENERIC(quat_xyn_to_inert_ideal)

// Namespacing symbols exported from integers.c:
#undef ibz_cornacchia_prime
#undef ibz_generate_random_prime
#undef ibz_rounded_div
#undef ibz_sum_two_squares

#define ibz_cornacchia_prime                            SQISIGN_NAMESPACE_GENERIC(ibz_cornacchia_prime)
#define ibz_generate_random_prime                       SQISIGN_NAMESPACE_GENERIC(ibz_generate_random_prime)
#define ibz_rounded_div                                 SQISIGN_NAMESPACE_GENERIC(ibz_rounded_div)
#define ibz_sum_two_squares                             SQISIGN_NAMESPACE_GENERIC(ibz_sum_two_squares)

// Namespacing symbols exported from isog.c:
#undef iso_isogeny_2chain
#undef iso_xeval_4
#undef iso_xisog_4

#define iso_isogeny_2chain                              SQISIGN_NAMESPACE(iso_isogeny_2chain)
#define iso_xeval_4                                     SQISIGN_NAMESPACE(iso_xeval_4)
#define iso_xisog_4                                     SQISIGN_NAMESPACE(iso_xisog_4)

// Namespacing symbols exported from keygen.c:
#undef protocols_keygen
#undef secret_key_init

#define protocols_keygen                                SQISIGN_NAMESPACE(protocols_keygen)
#define secret_key_init                                 SQISIGN_NAMESPACE(secret_key_init)

// Namespacing symbols exported from lattice.c:
#undef ibz_mat_4x4_is_hnf
#undef ibz_mat_4x4_is_triangular
#undef quat_lattice_contains
#undef quat_lattice_equal
#undef quat_lattice_gram
#undef quat_lattice_inclusion
#undef quat_lattice_reduce_denom
#undef quat_lattice_sample_from_ball

#define ibz_mat_4x4_is_hnf                              SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_is_hnf)
#define ibz_mat_4x4_is_triangular                       SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_is_triangular)
#define quat_lattice_contains                           SQISIGN_NAMESPACE_GENERIC(quat_lattice_contains)
#define quat_lattice_equal                              SQISIGN_NAMESPACE_GENERIC(quat_lattice_equal)
#define quat_lattice_gram                               SQISIGN_NAMESPACE_GENERIC(quat_lattice_gram)
#define quat_lattice_inclusion                          SQISIGN_NAMESPACE_GENERIC(quat_lattice_inclusion)
#define quat_lattice_reduce_denom                       SQISIGN_NAMESPACE_GENERIC(quat_lattice_reduce_denom)
#define quat_lattice_sample_from_ball                   SQISIGN_NAMESPACE_GENERIC(quat_lattice_sample_from_ball)

// Namespacing symbols exported from lehmer_xgcd.c:
#undef quat_lll_dim2_short_basis
#undef quat_lll_dim2_sumofsquares

#define quat_lll_dim2_short_basis                       SQISIGN_NAMESPACE(quat_lll_dim2_short_basis)
#define quat_lll_dim2_sumofsquares                      SQISIGN_NAMESPACE(quat_lll_dim2_sumofsquares)

// Namespacing symbols exported from lll_applications.c:
#undef quat_ideal_reduce_basis
#undef quat_lattice_bound_parallelogram

#define quat_ideal_reduce_basis                         SQISIGN_NAMESPACE_GENERIC(quat_ideal_reduce_basis)
#define quat_lattice_bound_parallelogram                SQISIGN_NAMESPACE_GENERIC(quat_lattice_bound_parallelogram)

// Namespacing symbols exported from lll_dim4.c:
#undef quat_lll_dual_reduce_ideal

#define quat_lll_dual_reduce_ideal                      SQISIGN_NAMESPACE(quat_lll_dual_reduce_ideal)

// Namespacing symbols exported from lll_lg2.c:
#undef quat_lll_gram_lehmer
#undef quat_lll_materialise_block

#define quat_lll_gram_lehmer                            SQISIGN_NAMESPACE(quat_lll_gram_lehmer)
#define quat_lll_materialise_block                      SQISIGN_NAMESPACE(quat_lll_materialise_block)

// Namespacing symbols exported from mem.c:
#undef sqisign_secure_clear
#undef sqisign_secure_free

#define sqisign_secure_clear                            SQISIGN_NAMESPACE_GENERIC(sqisign_secure_clear)
#define sqisign_secure_free                             SQISIGN_NAMESPACE_GENERIC(sqisign_secure_free)

// Namespacing symbols exported from modqx.c:
#undef modqx_getndash
#undef modqx_modadd
#undef modqx_modcipolla
#undef modqx_modcmp
#undef modqx_modcpy
#undef modqx_modis0
#undef modqx_modmul
#undef modqx_modone
#undef modqx_modshl
#undef modqx_modshr
#undef modqx_modsqr
#undef modqx_modsub
#undef modqx_modxpowe
#undef modqx_modxpowe_windowed
#undef modqx_modzer
#undef modqx_nresx
#undef modqx_redc

#define modqx_getndash                                  SQISIGN_NAMESPACE(modqx_getndash)
#define modqx_modadd                                    SQISIGN_NAMESPACE(modqx_modadd)
#define modqx_modcipolla                                SQISIGN_NAMESPACE(modqx_modcipolla)
#define modqx_modcmp                                    SQISIGN_NAMESPACE(modqx_modcmp)
#define modqx_modcpy                                    SQISIGN_NAMESPACE(modqx_modcpy)
#define modqx_modis0                                    SQISIGN_NAMESPACE(modqx_modis0)
#define modqx_modmul                                    SQISIGN_NAMESPACE(modqx_modmul)
#define modqx_modone                                    SQISIGN_NAMESPACE(modqx_modone)
#define modqx_modshl                                    SQISIGN_NAMESPACE(modqx_modshl)
#define modqx_modshr                                    SQISIGN_NAMESPACE(modqx_modshr)
#define modqx_modsqr                                    SQISIGN_NAMESPACE(modqx_modsqr)
#define modqx_modsub                                    SQISIGN_NAMESPACE(modqx_modsub)
#define modqx_modxpowe                                  SQISIGN_NAMESPACE(modqx_modxpowe)
#define modqx_modxpowe_windowed                         SQISIGN_NAMESPACE(modqx_modxpowe_windowed)
#define modqx_modzer                                    SQISIGN_NAMESPACE(modqx_modzer)
#define modqx_nresx                                     SQISIGN_NAMESPACE(modqx_nresx)
#define modqx_redc                                      SQISIGN_NAMESPACE(modqx_redc)

// Namespacing symbols exported from mp.c:
#undef ibz_abs
#undef ibz_add
#undef ibz_add_and_set_bound
#undef ibz_add_int_and_set_bound
#undef ibz_bitsize
#undef ibz_bitsize_ct
#undef ibz_cmp
#undef ibz_cmp_int32
#undef ibz_cneg
#undef ibz_const_one
#undef ibz_const_three
#undef ibz_const_two
#undef ibz_const_zero
#undef ibz_convert_to_str
#undef ibz_copy
#undef ibz_copy_bits
#undef ibz_copy_digits
#undef ibz_crt
#undef ibz_cswap
#undef ibz_ct_fp_recip
#undef ibz_ct_highmul_p
#undef ibz_ct_highmul_s
#undef ibz_ct_lt_mask
#undef ibz_ct_nonzero_mask
#undef ibz_ct_round_shift_limb
#undef ibz_ct_shift
#undef ibz_ct_shl
#undef ibz_div
#undef ibz_div_2exp
#undef ibz_divides
#undef ibz_extract_i64
#undef ibz_extract_u64
#undef ibz_gcd
#undef ibz_get
#undef ibz_get_bound
#undef ibz_init
#undef ibz_invmat
#undef ibz_invmod
#undef ibz_is_even
#undef ibz_is_odd
#undef ibz_is_one
#undef ibz_is_positive
#undef ibz_is_zero
#undef ibz_legendre
#undef ibz_mod
#undef ibz_mod2exp
#undef ibz_mod_ui
#undef ibz_mul
#undef ibz_mul_2exp
#undef ibz_mul_by_int_and_set_bound
#undef ibz_neg
#undef ibz_pow
#undef ibz_pow_mod
#undef ibz_print
#undef ibz_probab_prime
#undef ibz_rand_interval
#undef ibz_rand_interval_minm_m
#undef ibz_rand_interval_minm_m_with_domain
#undef ibz_rand_interval_with_domain
#undef ibz_set
#undef ibz_set_bound
#undef ibz_set_bound_ct
#undef ibz_set_from_str
#undef ibz_size_in_base
#undef ibz_sqrt_floor
#undef ibz_sqrt_m1_mod
#undef ibz_sqrt_m1_mod_verified
#undef ibz_sqrt_mod_p
#undef ibz_sub
#undef ibz_swap
#undef ibz_to_digits
#undef ibz_two_adic
#undef ibz_xgcd

#define ibz_abs                                         SQISIGN_NAMESPACE_GENERIC(ibz_abs)
#define ibz_add                                         SQISIGN_NAMESPACE_GENERIC(ibz_add)
#define ibz_add_and_set_bound                           SQISIGN_NAMESPACE_GENERIC(ibz_add_and_set_bound)
#define ibz_add_int_and_set_bound                       SQISIGN_NAMESPACE_GENERIC(ibz_add_int_and_set_bound)
#define ibz_bitsize                                     SQISIGN_NAMESPACE_GENERIC(ibz_bitsize)
#define ibz_bitsize_ct                                  SQISIGN_NAMESPACE_GENERIC(ibz_bitsize_ct)
#define ibz_cmp                                         SQISIGN_NAMESPACE_GENERIC(ibz_cmp)
#define ibz_cmp_int32                                   SQISIGN_NAMESPACE_GENERIC(ibz_cmp_int32)
#define ibz_cneg                                        SQISIGN_NAMESPACE_GENERIC(ibz_cneg)
#define ibz_const_one                                   SQISIGN_NAMESPACE_GENERIC(ibz_const_one)
#define ibz_const_three                                 SQISIGN_NAMESPACE_GENERIC(ibz_const_three)
#define ibz_const_two                                   SQISIGN_NAMESPACE_GENERIC(ibz_const_two)
#define ibz_const_zero                                  SQISIGN_NAMESPACE_GENERIC(ibz_const_zero)
#define ibz_convert_to_str                              SQISIGN_NAMESPACE_GENERIC(ibz_convert_to_str)
#define ibz_copy                                        SQISIGN_NAMESPACE_GENERIC(ibz_copy)
#define ibz_copy_bits                                   SQISIGN_NAMESPACE_GENERIC(ibz_copy_bits)
#define ibz_copy_digits                                 SQISIGN_NAMESPACE_GENERIC(ibz_copy_digits)
#define ibz_crt                                         SQISIGN_NAMESPACE_GENERIC(ibz_crt)
#define ibz_cswap                                       SQISIGN_NAMESPACE_GENERIC(ibz_cswap)
#define ibz_ct_fp_recip                                 SQISIGN_NAMESPACE_GENERIC(ibz_ct_fp_recip)
#define ibz_ct_highmul_p                                SQISIGN_NAMESPACE_GENERIC(ibz_ct_highmul_p)
#define ibz_ct_highmul_s                                SQISIGN_NAMESPACE_GENERIC(ibz_ct_highmul_s)
#define ibz_ct_lt_mask                                  SQISIGN_NAMESPACE_GENERIC(ibz_ct_lt_mask)
#define ibz_ct_nonzero_mask                             SQISIGN_NAMESPACE_GENERIC(ibz_ct_nonzero_mask)
#define ibz_ct_round_shift_limb                         SQISIGN_NAMESPACE_GENERIC(ibz_ct_round_shift_limb)
#define ibz_ct_shift                                    SQISIGN_NAMESPACE_GENERIC(ibz_ct_shift)
#define ibz_ct_shl                                      SQISIGN_NAMESPACE_GENERIC(ibz_ct_shl)
#define ibz_div                                         SQISIGN_NAMESPACE_GENERIC(ibz_div)
#define ibz_div_2exp                                    SQISIGN_NAMESPACE_GENERIC(ibz_div_2exp)
#define ibz_divides                                     SQISIGN_NAMESPACE_GENERIC(ibz_divides)
#define ibz_extract_i64                                 SQISIGN_NAMESPACE_GENERIC(ibz_extract_i64)
#define ibz_extract_u64                                 SQISIGN_NAMESPACE_GENERIC(ibz_extract_u64)
#define ibz_gcd                                         SQISIGN_NAMESPACE_GENERIC(ibz_gcd)
#define ibz_get                                         SQISIGN_NAMESPACE_GENERIC(ibz_get)
#define ibz_get_bound                                   SQISIGN_NAMESPACE_GENERIC(ibz_get_bound)
#define ibz_init                                        SQISIGN_NAMESPACE_GENERIC(ibz_init)
#define ibz_invmat                                      SQISIGN_NAMESPACE_GENERIC(ibz_invmat)
#define ibz_invmod                                      SQISIGN_NAMESPACE_GENERIC(ibz_invmod)
#define ibz_is_even                                     SQISIGN_NAMESPACE_GENERIC(ibz_is_even)
#define ibz_is_odd                                      SQISIGN_NAMESPACE_GENERIC(ibz_is_odd)
#define ibz_is_one                                      SQISIGN_NAMESPACE_GENERIC(ibz_is_one)
#define ibz_is_positive                                 SQISIGN_NAMESPACE_GENERIC(ibz_is_positive)
#define ibz_is_zero                                     SQISIGN_NAMESPACE_GENERIC(ibz_is_zero)
#define ibz_legendre                                    SQISIGN_NAMESPACE_GENERIC(ibz_legendre)
#define ibz_mod                                         SQISIGN_NAMESPACE_GENERIC(ibz_mod)
#define ibz_mod2exp                                     SQISIGN_NAMESPACE_GENERIC(ibz_mod2exp)
#define ibz_mod_ui                                      SQISIGN_NAMESPACE_GENERIC(ibz_mod_ui)
#define ibz_mul                                         SQISIGN_NAMESPACE_GENERIC(ibz_mul)
#define ibz_mul_2exp                                    SQISIGN_NAMESPACE_GENERIC(ibz_mul_2exp)
#define ibz_mul_by_int_and_set_bound                    SQISIGN_NAMESPACE_GENERIC(ibz_mul_by_int_and_set_bound)
#define ibz_neg                                         SQISIGN_NAMESPACE_GENERIC(ibz_neg)
#define ibz_pow                                         SQISIGN_NAMESPACE_GENERIC(ibz_pow)
#define ibz_pow_mod                                     SQISIGN_NAMESPACE_GENERIC(ibz_pow_mod)
#define ibz_print                                       SQISIGN_NAMESPACE_GENERIC(ibz_print)
#define ibz_probab_prime                                SQISIGN_NAMESPACE_GENERIC(ibz_probab_prime)
#define ibz_rand_interval                               SQISIGN_NAMESPACE_GENERIC(ibz_rand_interval)
#define ibz_rand_interval_minm_m                        SQISIGN_NAMESPACE_GENERIC(ibz_rand_interval_minm_m)
#define ibz_rand_interval_minm_m_with_domain            SQISIGN_NAMESPACE_GENERIC(ibz_rand_interval_minm_m_with_domain)
#define ibz_rand_interval_with_domain                   SQISIGN_NAMESPACE_GENERIC(ibz_rand_interval_with_domain)
#define ibz_set                                         SQISIGN_NAMESPACE_GENERIC(ibz_set)
#define ibz_set_bound                                   SQISIGN_NAMESPACE_GENERIC(ibz_set_bound)
#define ibz_set_bound_ct                                SQISIGN_NAMESPACE_GENERIC(ibz_set_bound_ct)
#define ibz_set_from_str                                SQISIGN_NAMESPACE_GENERIC(ibz_set_from_str)
#define ibz_size_in_base                                SQISIGN_NAMESPACE_GENERIC(ibz_size_in_base)
#define ibz_sqrt_floor                                  SQISIGN_NAMESPACE_GENERIC(ibz_sqrt_floor)
#define ibz_sqrt_m1_mod                                 SQISIGN_NAMESPACE_GENERIC(ibz_sqrt_m1_mod)
#define ibz_sqrt_m1_mod_verified                        SQISIGN_NAMESPACE_GENERIC(ibz_sqrt_m1_mod_verified)
#define ibz_sqrt_mod_p                                  SQISIGN_NAMESPACE_GENERIC(ibz_sqrt_mod_p)
#define ibz_sub                                         SQISIGN_NAMESPACE_GENERIC(ibz_sub)
#define ibz_swap                                        SQISIGN_NAMESPACE_GENERIC(ibz_swap)
#define ibz_to_digits                                   SQISIGN_NAMESPACE_GENERIC(ibz_to_digits)
#define ibz_two_adic                                    SQISIGN_NAMESPACE_GENERIC(ibz_two_adic)
#define ibz_xgcd                                        SQISIGN_NAMESPACE_GENERIC(ibz_xgcd)

// Namespacing symbols exported from normalize.c:
#undef ec_apply_isomorphism
#undef ec_compute_montgomery_coefficient
#undef ec_find_max_coefficient_in_list
#undef ec_normalize_montgomery
#undef ec_theta_to_montgomery

#define ec_apply_isomorphism                            SQISIGN_NAMESPACE(ec_apply_isomorphism)
#define ec_compute_montgomery_coefficient               SQISIGN_NAMESPACE(ec_compute_montgomery_coefficient)
#define ec_find_max_coefficient_in_list                 SQISIGN_NAMESPACE(ec_find_max_coefficient_in_list)
#define ec_normalize_montgomery                         SQISIGN_NAMESPACE(ec_normalize_montgomery)
#define ec_theta_to_montgomery                          SQISIGN_NAMESPACE(ec_theta_to_montgomery)

// Namespacing symbols exported from printer.c:
#undef ibz_mat_2x2_print
#undef ibz_mat_4x4_print
#undef ibz_vec_2_print
#undef ibz_vec_4_print
#undef quat_alg_elem_print
#undef quat_alg_print
#undef quat_ideal_print
#undef quat_lattice_print

#define ibz_mat_2x2_print                               SQISIGN_NAMESPACE_GENERIC(ibz_mat_2x2_print)
#define ibz_mat_4x4_print                               SQISIGN_NAMESPACE_GENERIC(ibz_mat_4x4_print)
#define ibz_vec_2_print                                 SQISIGN_NAMESPACE_GENERIC(ibz_vec_2_print)
#define ibz_vec_4_print                                 SQISIGN_NAMESPACE_GENERIC(ibz_vec_4_print)
#define quat_alg_elem_print                             SQISIGN_NAMESPACE_GENERIC(quat_alg_elem_print)
#define quat_alg_print                                  SQISIGN_NAMESPACE_GENERIC(quat_alg_print)
#define quat_ideal_print                                SQISIGN_NAMESPACE_GENERIC(quat_ideal_print)
#define quat_lattice_print                              SQISIGN_NAMESPACE_GENERIC(quat_lattice_print)

// Namespacing symbols exported from prng.c:
#undef PRNG_default_domain
#undef prng_clear
#undef prng_domain_clear
#undef prng_domain_seed
#undef prng_random_bytes
#undef prng_seed

#define PRNG_default_domain                             SQISIGN_NAMESPACE_GENERIC(PRNG_default_domain)
#define prng_clear                                      SQISIGN_NAMESPACE_GENERIC(prng_clear)
#define prng_domain_clear                               SQISIGN_NAMESPACE_GENERIC(prng_domain_clear)
#define prng_domain_seed                                SQISIGN_NAMESPACE_GENERIC(prng_domain_seed)
#define prng_random_bytes                               SQISIGN_NAMESPACE_GENERIC(prng_random_bytes)
#define prng_seed                                       SQISIGN_NAMESPACE_GENERIC(prng_seed)

// Namespacing symbols exported from protocol.c:
#undef quat_response_element

#define quat_response_element                           SQISIGN_NAMESPACE_GENERIC(quat_response_element)

// Namespacing symbols exported from qlapoty.c:
#undef quat_qlapoty
#undef quat_qlapoty_dim2_reduce_last_step
#undef quat_qlapoty_gen_to_dim2_lattice
#undef quat_qlapoty_get_short_basis
#undef quat_qlapoty_initialize_bounds
#undef quat_qlapoty_loop_one
#undef quat_qlapoty_loop_two
#undef quat_qlapoty_normeq

#define quat_qlapoty                                    SQISIGN_NAMESPACE_GENERIC(quat_qlapoty)
#define quat_qlapoty_dim2_reduce_last_step              SQISIGN_NAMESPACE_GENERIC(quat_qlapoty_dim2_reduce_last_step)
#define quat_qlapoty_gen_to_dim2_lattice                SQISIGN_NAMESPACE_GENERIC(quat_qlapoty_gen_to_dim2_lattice)
#define quat_qlapoty_get_short_basis                    SQISIGN_NAMESPACE_GENERIC(quat_qlapoty_get_short_basis)
#define quat_qlapoty_initialize_bounds                  SQISIGN_NAMESPACE_GENERIC(quat_qlapoty_initialize_bounds)
#define quat_qlapoty_loop_one                           SQISIGN_NAMESPACE_GENERIC(quat_qlapoty_loop_one)
#define quat_qlapoty_loop_two                           SQISIGN_NAMESPACE_GENERIC(quat_qlapoty_loop_two)
#define quat_qlapoty_normeq                             SQISIGN_NAMESPACE_GENERIC(quat_qlapoty_normeq)

// Namespacing symbols exported from quaternion_data.c:
#undef MAXORD_O0
#undef QUATALG_PINFTY
#undef QUAT_prime_cofactor

#define MAXORD_O0                                       SQISIGN_NAMESPACE(MAXORD_O0)
#define QUATALG_PINFTY                                  SQISIGN_NAMESPACE(QUATALG_PINFTY)
#define QUAT_prime_cofactor                             SQISIGN_NAMESPACE(QUAT_prime_cofactor)

// Namespacing symbols exported from random_input_generation.c:
#undef ibz_rand_interval_bits
#undef quat_test_input_random_ideal_generation
#undef quat_test_input_random_ideal_lattice_generation
#undef quat_test_input_random_lattice_generation
#undef quat_test_input_resplike_lattice_generation

#define ibz_rand_interval_bits                          SQISIGN_NAMESPACE_GENERIC(ibz_rand_interval_bits)
#define quat_test_input_random_ideal_generation         SQISIGN_NAMESPACE_GENERIC(quat_test_input_random_ideal_generation)
#define quat_test_input_random_ideal_lattice_generation SQISIGN_NAMESPACE_GENERIC(quat_test_input_random_ideal_lattice_generation)
#define quat_test_input_random_lattice_generation       SQISIGN_NAMESPACE_GENERIC(quat_test_input_random_lattice_generation)
#define quat_test_input_resplike_lattice_generation     SQISIGN_NAMESPACE_GENERIC(quat_test_input_resplike_lattice_generation)

// Namespacing symbols exported from sign.c:
#undef protocols_sign

#define protocols_sign                                  SQISIGN_NAMESPACE(protocols_sign)

// Namespacing symbols exported from splitting.c:
#undef splitting_check_valid_theta_null_point
#undef splitting_tensor_product_to_couple_point
#undef splitting_to_elliptic_product

#define splitting_check_valid_theta_null_point          SQISIGN_NAMESPACE(splitting_check_valid_theta_null_point)
#define splitting_tensor_product_to_couple_point        SQISIGN_NAMESPACE(splitting_tensor_product_to_couple_point)
#define splitting_to_elliptic_product                   SQISIGN_NAMESPACE(splitting_to_elliptic_product)

// Namespacing symbols exported from sqisign.c:
#undef sqisign_keypair
#undef sqisign_open
#undef sqisign_sign
#undef sqisign_sign_signature
#undef sqisign_verify

#define sqisign_keypair                                 SQISIGN_NAMESPACE(sqisign_keypair)
#define sqisign_open                                    SQISIGN_NAMESPACE(sqisign_open)
#define sqisign_sign                                    SQISIGN_NAMESPACE(sqisign_sign)
#define sqisign_sign_signature                          SQISIGN_NAMESPACE(sqisign_sign_signature)
#define sqisign_verify                                  SQISIGN_NAMESPACE(sqisign_verify)

// Namespacing symbols exported from stdorder.c:
#undef quat_change_to_O0_basis
#undef quat_lattice_O0_set
#undef quat_mod_O0
#undef quat_represent_integer

#define quat_change_to_O0_basis                         SQISIGN_NAMESPACE_GENERIC(quat_change_to_O0_basis)
#define quat_lattice_O0_set                             SQISIGN_NAMESPACE_GENERIC(quat_lattice_O0_set)
#define quat_mod_O0                                     SQISIGN_NAMESPACE_GENERIC(quat_mod_O0)
#define quat_represent_integer                          SQISIGN_NAMESPACE_GENERIC(quat_represent_integer)

// Namespacing symbols exported from theta_isogenies.c:
#undef theta_chain_compute_and_eval
#undef theta_chain_compute_and_eval_E1
#undef theta_chain_compute_and_eval_E2
#undef theta_chain_compute_and_eval_verify

#define theta_chain_compute_and_eval                    SQISIGN_NAMESPACE(theta_chain_compute_and_eval)
#define theta_chain_compute_and_eval_E1                 SQISIGN_NAMESPACE(theta_chain_compute_and_eval_E1)
#define theta_chain_compute_and_eval_E2                 SQISIGN_NAMESPACE(theta_chain_compute_and_eval_E2)
#define theta_chain_compute_and_eval_verify             SQISIGN_NAMESPACE(theta_chain_compute_and_eval_verify)

// Namespacing symbols exported from theta_structure.c:
#undef theta_DBL
#undef theta_DBL_iter
#undef theta_extract_dual_null_point
#undef theta_is_product_theta_point
#undef theta_precomputation

#define theta_DBL                                       SQISIGN_NAMESPACE(theta_DBL)
#define theta_DBL_iter                                  SQISIGN_NAMESPACE(theta_DBL_iter)
#define theta_extract_dual_null_point                   SQISIGN_NAMESPACE(theta_extract_dual_null_point)
#define theta_is_product_theta_point                    SQISIGN_NAMESPACE(theta_is_product_theta_point)
#define theta_precomputation                            SQISIGN_NAMESPACE(theta_precomputation)

// Namespacing symbols exported from torsion_constants.c:
#undef COM_DEGREE
#undef SEC_DEGREE
#undef TORSION_PLUS_2POWER
#undef TWO_TO_CHALLENGE_BITS
#undef TWO_TO_CHALLENGE_PLUS_TWO_BITS
#undef TWO_TO_SECURITY_BITS

#define COM_DEGREE                                      SQISIGN_NAMESPACE(COM_DEGREE)
#define SEC_DEGREE                                      SQISIGN_NAMESPACE(SEC_DEGREE)
#define TORSION_PLUS_2POWER                             SQISIGN_NAMESPACE(TORSION_PLUS_2POWER)
#define TWO_TO_CHALLENGE_BITS                           SQISIGN_NAMESPACE(TWO_TO_CHALLENGE_BITS)
#define TWO_TO_CHALLENGE_PLUS_TWO_BITS                  SQISIGN_NAMESPACE(TWO_TO_CHALLENGE_PLUS_TWO_BITS)
#define TWO_TO_SECURITY_BITS                            SQISIGN_NAMESPACE(TWO_TO_SECURITY_BITS)

// Namespacing symbols exported from verify.c:
#undef protocols_verify

#define protocols_verify                                SQISIGN_NAMESPACE(protocols_verify)


#endif

