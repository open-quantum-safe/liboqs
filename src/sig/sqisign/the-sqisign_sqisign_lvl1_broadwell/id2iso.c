#include <quaternion.h>
#include <ec.h>
#include <mp.h>
#include <endomorphism_action.h>
#include <id2iso.h>
#include <inttypes.h>
#include <locale.h>
#include <biextension.h>
#include <torsion_constants.h>

// Scalar multiplication [x]P + [y]Q where x and y are stored
// inside an ibz_vec_2_t [x, y] and P, Q \in E[2^f]
void
ec_biscalar_mul_ibz_vec(ec_point_t *res,
                        const ibz_vec_2_t *scalar_vec,
                        const int f,
                        const ec_basis_t *PQ,
                        const ec_curve_t *curve)
{
    digit_t scalars[2][NWORDS_ORDER];
    ibz_to_digit_array(scalars[0], &(*scalar_vec)[0]);
    ibz_to_digit_array(scalars[1], &(*scalar_vec)[1]);
    ec_biscalar_mul(res, scalars[0], scalars[1], f, PQ, curve);
}

// Given an ideal, computes the scalars s0, s1 which determine the kernel generator
// of the equivalent isogeny
void
id2iso_ideal_to_kernel_dlogs_even(ibz_vec_2_t *vec, const quat_left_ideal_t *lideal)
{
    ibz_t tmp;
    ibz_init(&tmp);

    ibz_mat_2x2_t mat;
    ibz_mat_2x2_init(&mat);

    // construct the matrix of the dual of alpha on the 2^f-torsion
    {
        quat_alg_elem_t alpha;
        quat_alg_elem_init(&alpha);

        int lideal_generator_ok UNUSED = quat_lideal_generator(&alpha, lideal, &QUATALG_PINFTY);
        assert(lideal_generator_ok);
        quat_alg_conj(&alpha, &alpha);

        ibz_vec_4_t coeffs;
        ibz_vec_4_init(&coeffs);
        quat_change_to_O0_basis(&coeffs, &alpha);

        for (unsigned i = 0; i < 2; ++i) {
            ibz_add(&mat[i][i], &mat[i][i], &coeffs[0]);
            for (unsigned j = 0; j < 2; ++j) {
                ibz_mul(&tmp, &ACTION_GEN2[i][j], &coeffs[1]);
                ibz_add(&mat[i][j], &mat[i][j], &tmp);
                ibz_mul(&tmp, &ACTION_GEN3[i][j], &coeffs[2]);
                ibz_add(&mat[i][j], &mat[i][j], &tmp);
                ibz_mul(&tmp, &ACTION_GEN4[i][j], &coeffs[3]);
                ibz_add(&mat[i][j], &mat[i][j], &tmp);
            }
        }

        ibz_vec_4_finalize(&coeffs);
        quat_alg_elem_finalize(&alpha);
    }

    // find the kernel of alpha modulo the norm of the ideal
    {
        const ibz_t *const norm = &lideal->norm;

        ibz_mod(&(*vec)[0], &mat[0][0], norm);
        ibz_mod(&(*vec)[1], &mat[1][0], norm);
        ibz_gcd(&tmp, &(*vec)[0], &(*vec)[1]);
        if (ibz_is_even(&tmp)) {
            ibz_mod(&(*vec)[0], &mat[0][1], norm);
            ibz_mod(&(*vec)[1], &mat[1][1], norm);
        }
#ifndef NDEBUG
        ibz_gcd(&tmp, &(*vec)[0], norm);
        ibz_gcd(&tmp, &(*vec)[1], &tmp);
        assert(!ibz_cmp(&tmp, &ibz_const_one));
#endif
    }

    ibz_mat_2x2_finalize(&mat);
    ibz_finalize(&tmp);
}

// helper function to apply a matrix to a basis of E[2^f]
// works in place
int
matrix_application_even_basis(ec_basis_t *bas, const ec_curve_t *E, ibz_mat_2x2_t *mat, int f)
{
    digit_t scalars[2][NWORDS_ORDER] = { 0 };
    int ret;

    ibz_t tmp, pow_two;
    ibz_init(&tmp);
    ibz_init(&pow_two);
    ibz_pow(&pow_two, &ibz_const_two, f);

    ec_basis_t tmp_bas;
    copy_basis(&tmp_bas, bas);

    // reduction mod 2f
    ibz_mod(&(*mat)[0][0], &(*mat)[0][0], &pow_two);
    ibz_mod(&(*mat)[0][1], &(*mat)[0][1], &pow_two);
    ibz_mod(&(*mat)[1][0], &(*mat)[1][0], &pow_two);
    ibz_mod(&(*mat)[1][1], &(*mat)[1][1], &pow_two);

    // For a matrix [[a, c], [b, d]] we compute:
    //
    // first basis element R = [a]P + [b]Q
    ibz_to_digit_array(scalars[0], &(*mat)[0][0]);
    ibz_to_digit_array(scalars[1], &(*mat)[1][0]);
    ec_biscalar_mul(&bas->P, scalars[0], scalars[1], f, &tmp_bas, E);

    // second basis element S = [c]P + [d]Q
    ibz_to_digit_array(scalars[0], &(*mat)[0][1]);
    ibz_to_digit_array(scalars[1], &(*mat)[1][1]);
    ec_biscalar_mul(&bas->Q, scalars[0], scalars[1], f, &tmp_bas, E);

    // Their difference R - S = [a - c]P + [b - d]Q
    ibz_sub(&tmp, &(*mat)[0][0], &(*mat)[0][1]);
    ibz_mod(&tmp, &tmp, &pow_two);
    ibz_to_digit_array(scalars[0], &tmp);
    ibz_sub(&tmp, &(*mat)[1][0], &(*mat)[1][1]);
    ibz_mod(&tmp, &tmp, &pow_two);
    ibz_to_digit_array(scalars[1], &tmp);
    ret = ec_biscalar_mul(&bas->PmQ, scalars[0], scalars[1], f, &tmp_bas, E);

    ibz_finalize(&tmp);
    ibz_finalize(&pow_two);

    return ret;
}

// helper function to apply some endomorphism of E0 on the precomputed basis of E[2^f]
// works in place
void
endomorphism_application_even_basis(ec_basis_t *bas,
                                    const int index_alternate_curve,
                                    const ec_curve_t *E,
                                    const quat_alg_elem_t *theta,
                                    int f)
{
    ibz_t tmp;
    ibz_init(&tmp);
    ibz_vec_4_t coeffs;
    ibz_vec_4_init(&coeffs);
    ibz_mat_2x2_t mat;
    ibz_mat_2x2_init(&mat);

    ibz_t content;
    ibz_init(&content);

    // decomposing theta on the basis
    quat_alg_make_primitive(&coeffs, &content, theta, &EXTREMAL_ORDERS[index_alternate_curve].order);
    assert(ibz_is_odd(&content));

    ibz_set(&mat[0][0], 0);
    ibz_set(&mat[0][1], 0);
    ibz_set(&mat[1][0], 0);
    ibz_set(&mat[1][1], 0);

    // computing the matrix

    for (unsigned i = 0; i < 2; ++i) {
        ibz_add(&mat[i][i], &mat[i][i], &coeffs[0]);
        for (unsigned j = 0; j < 2; ++j) {
            ibz_mul(&tmp, &CURVES_WITH_ENDOMORPHISMS[index_alternate_curve].action_gen2[i][j], &coeffs[1]);
            ibz_add(&mat[i][j], &mat[i][j], &tmp);
            ibz_mul(&tmp, &CURVES_WITH_ENDOMORPHISMS[index_alternate_curve].action_gen3[i][j], &coeffs[2]);
            ibz_add(&mat[i][j], &mat[i][j], &tmp);
            ibz_mul(&tmp, &CURVES_WITH_ENDOMORPHISMS[index_alternate_curve].action_gen4[i][j], &coeffs[3]);
            ibz_add(&mat[i][j], &mat[i][j], &tmp);
            ibz_mul(&mat[i][j], &mat[i][j], &content);
        }
    }

    // and now we apply it
    matrix_application_even_basis(bas, E, &mat, f);

    ibz_vec_4_finalize(&coeffs);
    ibz_mat_2x2_finalize(&mat);
    ibz_finalize(&content);

    ibz_finalize(&tmp);
}

// compute the ideal whose kernel is generated by vec2[0]*BO[0] + vec2[1]*B0[1] where B0 is the
// canonical basis of E0
void
id2iso_kernel_dlogs_to_ideal_even(quat_left_ideal_t *lideal, const ibz_vec_2_t *vec2, int f)
{

    // algorithm: apply endomorphisms 1 and j+(1+k)/2 to the kernel point,
    // the result should form a basis of the respective torsion subgroup.
    // then apply i to the kernel point and decompose over said basis.
    // hence we have an equation a*P + b*[j+(1+k)/2]P == [i]P, which will
    // easily reveal an endomorphism that kills P.

    ibz_t two_pow;
    ibz_init(&two_pow);

    ibz_vec_2_t vec;
    ibz_vec_2_init(&vec);

    if (f == TORSION_EVEN_POWER) {
        ibz_copy(&two_pow, &TORSION_PLUS_2POWER);
    } else {
        ibz_pow(&two_pow, &ibz_const_two, f);
    }

    {
        ibz_mat_2x2_t mat;
        ibz_mat_2x2_init(&mat);

        ibz_copy(&mat[0][0], &(*vec2)[0]);
        ibz_copy(&mat[1][0], &(*vec2)[1]);

        ibz_mat_2x2_eval(&vec, &ACTION_J, vec2);
        ibz_copy(&mat[0][1], &vec[0]);
        ibz_copy(&mat[1][1], &vec[1]);

        ibz_mat_2x2_eval(&vec, &ACTION_GEN4, vec2);
        ibz_add(&mat[0][1], &mat[0][1], &vec[0]);
        ibz_add(&mat[1][1], &mat[1][1], &vec[1]);

        ibz_mod(&mat[0][1], &mat[0][1], &two_pow);
        ibz_mod(&mat[1][1], &mat[1][1], &two_pow);

        ibz_mat_2x2_t inv;
        ibz_mat_2x2_init(&inv);
        {
            int inv_ok UNUSED = ibz_mat_2x2_inv_mod(&inv, &mat, &two_pow);
            assert(inv_ok);
        }
        ibz_mat_2x2_finalize(&mat);

        ibz_mat_2x2_eval(&vec, &ACTION_I, vec2);
        ibz_mat_2x2_eval(&vec, &inv, &vec);

        ibz_mat_2x2_finalize(&inv);
    }

    // final result: a - i + b*(j+(1+k)/2)
    quat_alg_elem_t gen;
    quat_alg_elem_init(&gen);
    ibz_set(&gen.denom, 2);
    ibz_add(&gen.coord[0], &vec[0], &vec[0]);
    ibz_set(&gen.coord[1], -2);
    ibz_add(&gen.coord[2], &vec[1], &vec[1]);
    ibz_copy(&gen.coord[3], &vec[1]);
    ibz_add(&gen.coord[0], &gen.coord[0], &vec[1]);
    ibz_vec_2_finalize(&vec);

    quat_lideal_create(lideal, &gen, &two_pow, &MAXORD_O0, &QUATALG_PINFTY);

    assert(0 == ibz_cmp(&lideal->norm, &two_pow));

    quat_alg_elem_finalize(&gen);
    ibz_finalize(&two_pow);
}

// finds mat such that:
// (mat*v).B2 = v.B1
// where "." is the dot product, defined as (v1,v2).(P,Q) = v1*P + v2*Q
// mat encodes the coordinates of the points of B1 in the basis B2
// specifically requires B1 or B2 to be "full" w.r.t to the 2^n torsion, so that we use tate
// full = 0 assumes B2 is "full" so the easier case.
// if we want to switch the role of B2 and B1, we invert the matrix, e.g. set full = 1
static void
_change_of_basis_matrix_tate(ibz_mat_2x2_t *mat,
                             const ec_basis_t *B1,
                             const ec_basis_t *B2,
                             ec_curve_t *E,
                             int f,
                             bool invert)
{
    digit_t x1[NWORDS_ORDER] = { 0 }, x2[NWORDS_ORDER] = { 0 }, x3[NWORDS_ORDER] = { 0 }, x4[NWORDS_ORDER] = { 0 };

#ifndef NDEBUG
    int e_full = TORSION_EVEN_POWER;
    int e_diff = e_full - f;
#endif

    // Ensure the input basis has points of order 2^f
    if (invert) {
        assert(test_basis_order_twof(B1, E, e_full));
        ec_dlog_2_tate(x1, x2, x3, x4, B1, B2, E, f);
        mp_invert_matrix(x1, x2, x3, x4, f, NWORDS_ORDER);
    } else {
        assert(test_basis_order_twof(B2, E, e_full));
        ec_dlog_2_tate(x1, x2, x3, x4, B2, B1, E, f);
    }

#ifndef NDEBUG
    {
        if (invert) {
            ec_point_t test, test2;
            ec_biscalar_mul(&test, x1, x2, f, B2, E);
            ec_dbl_iter(&test2, e_diff, &B1->P, E);
            assert(ec_is_equal(&test, &test2));

            ec_biscalar_mul(&test, x3, x4, f, B2, E);
            ec_dbl_iter(&test2, e_diff, &B1->Q, E);
            assert(ec_is_equal(&test, &test2));
        } else {
            ec_point_t test;
            ec_biscalar_mul(&test, x1, x2, f, B2, E);
            ec_dbl_iter(&test, e_diff, &test, E);
            assert(ec_is_equal(&test, &(B1->P)));

            ec_biscalar_mul(&test, x3, x4, f, B2, E);
            ec_dbl_iter(&test, e_diff, &test, E);
            assert(ec_is_equal(&test, &(B1->Q)));
        }
    }
#endif

    // Copy the results into the matrix
    ibz_copy_digit_array(&((*mat)[0][0]), x1);
    ibz_copy_digit_array(&((*mat)[1][0]), x2);
    ibz_copy_digit_array(&((*mat)[0][1]), x3);
    ibz_copy_digit_array(&((*mat)[1][1]), x4);
}

void
change_of_basis_matrix_tate(ibz_mat_2x2_t *mat, const ec_basis_t *B1, const ec_basis_t *B2, ec_curve_t *E, int f)
{
    _change_of_basis_matrix_tate(mat, B1, B2, E, f, false);
}

void
change_of_basis_matrix_tate_invert(ibz_mat_2x2_t *mat, const ec_basis_t *B1, const ec_basis_t *B2, ec_curve_t *E, int f)
{
    _change_of_basis_matrix_tate(mat, B1, B2, E, f, true);
}
