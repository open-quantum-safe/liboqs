#include <ec.h>
#include <endomorphism_action.h>
#include <hd.h>
#include <id2iso.h>
#include <inttypes.h>
#include <locale.h>
#include <quaternion.h>
#include <tools.h>
#include <torsion_constants.h>

static int
_fixed_degree_isogeny_impl(quat_left_ideal_t *lideal,
                           const ibz_t *u,
                           bool small,
                           theta_couple_curve_t *E34,
                           theta_couple_point_t *P12,
                           size_t numP,
                           const int index_alternate_order)
{

    // var declaration
    int ret;
    ibz_t two_pow, tmp;
    quat_alg_elem_t theta;

    ec_curve_t E0;
    copy_curve(&E0, &CURVES_WITH_ENDOMORPHISMS[index_alternate_order].curve);
    ec_curve_normalize_A24(&E0);

    unsigned length;

    int u_bitsize = ibz_bitsize(u);

    // deciding the power of 2 of the dim2 isogeny we use for this
    // the smaller the faster, but if it set too low there is a risk that
    // RepresentInteger will fail
    if (!small) {
        // in that case, we just set it to be the biggest value possible
        length = TORSION_EVEN_POWER - HD_extra_torsion;
    } else {
        length = ibz_bitsize(&QUATALG_PINFTY.p) + QUAT_repres_bound_input - u_bitsize;
        assert(u_bitsize < (int)length);
        assert(length < TORSION_EVEN_POWER - HD_extra_torsion);
    }
    assert(length);

    // var init
    ibz_init(&two_pow);
    ibz_init(&tmp);
    quat_alg_elem_init(&theta);

    ibz_pow(&two_pow, &ibz_const_two, length);
    ibz_copy(&tmp, u);
    assert(ibz_cmp(&two_pow, &tmp) > 0);
    assert(!ibz_is_even(&tmp));

    // computing the endomorphism theta of norm u * (2^(length) - u)
    ibz_sub(&tmp, &two_pow, &tmp);
    ibz_mul(&tmp, &tmp, u);
    assert(!ibz_is_even(&tmp));

    // setting-up the quat_represent_integer_params
    quat_represent_integer_params_t ri_params;
    ri_params.primality_test_iterations = QUAT_represent_integer_params.primality_test_iterations;

    quat_p_extremal_maximal_order_t order_hnf;
    quat_alg_elem_init(&order_hnf.z);
    quat_alg_elem_copy(&order_hnf.z, &EXTREMAL_ORDERS[index_alternate_order].z);
    quat_alg_elem_init(&order_hnf.t);
    quat_alg_elem_copy(&order_hnf.t, &EXTREMAL_ORDERS[index_alternate_order].t);
    quat_lattice_init(&order_hnf.order);
    ibz_copy(&order_hnf.order.denom, &EXTREMAL_ORDERS[index_alternate_order].order.denom);
    ibz_mat_4x4_copy(&order_hnf.order.basis, &EXTREMAL_ORDERS[index_alternate_order].order.basis);
    order_hnf.q = EXTREMAL_ORDERS[index_alternate_order].q;
    ri_params.order = &order_hnf;
    ri_params.algebra = &QUATALG_PINFTY;

#ifndef NDEBUG
    assert(quat_lattice_contains(NULL, &ri_params.order->order, &ri_params.order->z));
    assert(quat_lattice_contains(NULL, &ri_params.order->order, &ri_params.order->t));
#endif

    ret = quat_represent_integer(&theta, &tmp, 1, &ri_params);

    assert(!ibz_is_even(&tmp));

    if (!ret) {
        printf("represent integer failed for the alternate order number %d and for "
               "a target of "
               "size %d for a u of size %d with length = "
               "%u \n",
               index_alternate_order,
               ibz_bitsize(&tmp),
               ibz_bitsize(u),
               length);
        goto cleanup;
    }
    quat_lideal_create(lideal, &theta, u, &order_hnf.order, &QUATALG_PINFTY);

    quat_alg_elem_finalize(&order_hnf.z);
    quat_alg_elem_finalize(&order_hnf.t);
    quat_lattice_finalize(&order_hnf.order);

#ifndef NDEBUG
    ibz_t test_norm, test_denom;
    ibz_init(&test_denom);
    ibz_init(&test_norm);
    quat_alg_norm(&test_norm, &test_denom, &theta, &QUATALG_PINFTY);
    assert(ibz_is_one(&test_denom));
    assert(ibz_cmp(&test_norm, &tmp) == 0);
    assert(!ibz_is_even(&tmp));
    assert(quat_lattice_contains(NULL, &EXTREMAL_ORDERS[index_alternate_order].order, &theta));
    ibz_finalize(&test_norm);
    ibz_finalize(&test_denom);
#endif

    ec_basis_t B0_two;
    // copying the basis
    copy_basis(&B0_two, &CURVES_WITH_ENDOMORPHISMS[index_alternate_order].basis_even);
    assert(test_basis_order_twof(&B0_two, &E0, TORSION_EVEN_POWER));
    ec_dbl_iter_basis(&B0_two, TORSION_EVEN_POWER - length - HD_extra_torsion, &B0_two, &E0);

    assert(test_basis_order_twof(&B0_two, &E0, length + HD_extra_torsion));

    // now we set-up the kernel
    theta_couple_point_t T1;
    theta_couple_point_t T2, T1m2;

    copy_point(&T1.P1, &B0_two.P);
    copy_point(&T2.P1, &B0_two.Q);
    copy_point(&T1m2.P1, &B0_two.PmQ);

    // multiplication of theta by (u)^-1 mod 2^(length+2)
    ibz_mul(&two_pow, &two_pow, &ibz_const_two);
    ibz_mul(&two_pow, &two_pow, &ibz_const_two);
    ibz_copy(&tmp, u);
    ibz_invmod(&tmp, &tmp, &two_pow);
    assert(!ibz_is_even(&tmp));

    ibz_mul(&theta.coord[0], &theta.coord[0], &tmp);
    ibz_mul(&theta.coord[1], &theta.coord[1], &tmp);
    ibz_mul(&theta.coord[2], &theta.coord[2], &tmp);
    ibz_mul(&theta.coord[3], &theta.coord[3], &tmp);

    // applying theta to the basis
    ec_basis_t B0_two_theta;
    copy_basis(&B0_two_theta, &B0_two);
    endomorphism_application_even_basis(&B0_two_theta, index_alternate_order, &E0, &theta, length + HD_extra_torsion);

    // Ensure the basis we're using has the expected order
    assert(test_basis_order_twof(&B0_two_theta, &E0, length + HD_extra_torsion));

    // Set-up the domain E0 x E0
    theta_couple_curve_t E00;
    E00.E1 = E0;
    E00.E2 = E0;

    // Set-up the kernel from the bases
    theta_kernel_couple_points_t dim_two_ker;
    copy_bases_to_kernel(&dim_two_ker, &B0_two, &B0_two_theta);

    ret = theta_chain_compute_and_eval(length, &E00, &dim_two_ker, true, E34, P12, numP);
    if (!ret)
        goto cleanup;

    assert(length);
    ret = (int)length;

cleanup:
    // var finalize
    ibz_finalize(&two_pow);
    ibz_finalize(&tmp);
    quat_alg_elem_finalize(&theta);

    return ret;
}

int
fixed_degree_isogeny_and_eval(quat_left_ideal_t *lideal,
                              const ibz_t *u,
                              bool small,
                              theta_couple_curve_t *E34,
                              theta_couple_point_t *P12,
                              size_t numP,
                              const int index_alternate_order)
{
    return _fixed_degree_isogeny_impl(lideal, u, small, E34, P12, numP, index_alternate_order);
}

// takes the output of LLL and apply some small treatment on the basis
// reordering vectors and switching some signs if needed to make it in a nicer
// shape
static void
post_LLL_basis_treatment(ibz_mat_4x4_t *gram, ibz_mat_4x4_t *reduced, const ibz_t *norm, bool is_special_order)
{
    // if the left order is the special one, then we apply some additional post
    // treatment
    if (is_special_order) {
        // reordering the basis if needed
        if (ibz_cmp(&(*gram)[0][0], &(*gram)[2][2]) == 0) {
            for (int i = 0; i < 4; i++) {
                ibz_swap(&(*reduced)[i][1], &(*reduced)[i][2]);
            }
            ibz_swap(&(*gram)[0][2], &(*gram)[0][1]);
            ibz_swap(&(*gram)[2][0], &(*gram)[1][0]);
            ibz_swap(&(*gram)[3][2], &(*gram)[3][1]);
            ibz_swap(&(*gram)[2][3], &(*gram)[1][3]);
            ibz_swap(&(*gram)[2][2], &(*gram)[1][1]);
        } else if (ibz_cmp(&(*gram)[0][0], &(*gram)[3][3]) == 0) {
            for (int i = 0; i < 4; i++) {
                ibz_swap(&(*reduced)[i][1], &(*reduced)[i][3]);
            }
            ibz_swap(&(*gram)[0][3], &(*gram)[0][1]);
            ibz_swap(&(*gram)[3][0], &(*gram)[1][0]);
            ibz_swap(&(*gram)[2][3], &(*gram)[2][1]);
            ibz_swap(&(*gram)[3][2], &(*gram)[1][2]);
            ibz_swap(&(*gram)[3][3], &(*gram)[1][1]);
        } else if (ibz_cmp(&(*gram)[1][1], &(*gram)[3][3]) == 0) {
            // in this case it seems that we need to swap the second and third
            // element, and then recompute entirely the second element from the first
            // first we swap the second and third element
            for (int i = 0; i < 4; i++) {
                ibz_swap(&(*reduced)[i][1], &(*reduced)[i][2]);
            }
            ibz_swap(&(*gram)[0][2], &(*gram)[0][1]);
            ibz_swap(&(*gram)[2][0], &(*gram)[1][0]);
            ibz_swap(&(*gram)[3][2], &(*gram)[3][1]);
            ibz_swap(&(*gram)[2][3], &(*gram)[1][3]);
            ibz_swap(&(*gram)[2][2], &(*gram)[1][1]);
        }

        // adjusting the sign if needed
        if (ibz_cmp(&(*reduced)[0][0], &(*reduced)[1][1]) != 0) {
            for (int i = 0; i < 4; i++) {
                ibz_neg(&(*reduced)[i][1], &(*reduced)[i][1]);
                ibz_neg(&(*gram)[i][1], &(*gram)[i][1]);
                ibz_neg(&(*gram)[1][i], &(*gram)[1][i]);
            }
        }
        if (ibz_cmp(&(*reduced)[0][2], &(*reduced)[1][3]) != 0) {
            for (int i = 0; i < 4; i++) {
                ibz_neg(&(*reduced)[i][3], &(*reduced)[i][3]);
                ibz_neg(&(*gram)[i][3], &(*gram)[i][3]);
                ibz_neg(&(*gram)[3][i], &(*gram)[3][i]);
            }
            // assert(ibz_cmp(&(*reduced)[0][2],&(*reduced)[1][3])==0);
        }
    }
}

// enumerate all vectors in an hypercube of norm m for the infinity norm
// with respect to a basis whose gram matrix is given by gram
// Returns an int `count`, the number of vectors found with the desired
// properties
static int
enumerate_hypercube(ibz_vec_4_t *vecs, ibz_t *norms, int m, const ibz_mat_4x4_t *gram, const ibz_t *adjusted_norm)
{

    ibz_t remain, norm;
    ibz_vec_4_t point;

    ibz_init(&remain);
    ibz_init(&norm);
    ibz_vec_4_init(&point);

    assert(m > 0);

    int count = 0;
    int dim = 2 * m + 1;
    int dim2 = dim * dim;
    int dim3 = dim2 * dim;

    // if the basis is of the form alpha, i*alpha, beta, i*beta
    // we can remove some values due to symmetry of the basis that
    bool need_remove_symmetry =
        (ibz_cmp(&(*gram)[0][0], &(*gram)[1][1]) == 0 && ibz_cmp(&(*gram)[3][3], &(*gram)[2][2]) == 0);

    int check1, check2, check3;

    // Enumerate over points in a hypercube with coordinates (x, y, z, w)
    for (int x = -m; x <= 0; x++) { // We only check non-positive x-values
        for (int y = -m; y < m + 1; y++) {
            // Once x = 0 we only consider non-positive y values
            if (x == 0 && y > 0) {
                break;
            }
            for (int z = -m; z < m + 1; z++) {
                // If x and y are both zero, we only consider non-positive z values
                if (x == 0 && y == 0 && z > 0) {
                    break;
                }
                for (int w = -m; w < m + 1; w++) {
                    // If x, y, z are all zero, we only consider negative w values
                    if (x == 0 && y == 0 && z == 0 && w >= 0) {
                        break;
                    }

                    // Now for each candidate (x, y, z, w) we need to check a number of
                    // conditions We have already filtered for symmetry with several break
                    // statements, but there are more checks.

                    // 1. We do not allow all (x, y, z, w) to be multiples of 2
                    // 2. We do not allow all (x, y, z, w) to be multiples of 3
                    // 3. We do not want elements of the same norm, so we quotient out the
                    // action
                    //    of a group of order four generated by i for a basis expected to
                    //    be of the form: [gamma, i gamma, beta, i beta ].

                    // Ensure that not all values are even
                    if (!((x | y | z | w) & 1)) {
                        continue;
                    }
                    // Ensure that not all values are multiples of three
                    if (x % 3 == 0 && y % 3 == 0 && z % 3 == 0 && w % 3 == 0) {
                        continue;
                    }

                    check1 = (m + w) + dim * (m + z) + dim2 * (m + y) + dim3 * (m + x);
                    check2 = (m - z) + dim * (m + w) + dim2 * (m - x) + dim3 * (m + y);
                    check3 = (m + z) + dim * (m - w) + dim2 * (m + x) + dim3 * (m - y);

                    // either the basis does not have symmetry and we are good,
                    // or there is a special symmetry that we can exploit
                    // and we ensure that we don't record the same norm in the list
                    if (!need_remove_symmetry || (check1 <= check2 && check1 <= check3)) {
                        // Set the point as a vector (x, y, z, w)
                        ibz_set(&point[0], x);
                        ibz_set(&point[1], y);
                        ibz_set(&point[2], z);
                        ibz_set(&point[3], w);

                        // Evaluate this through the gram matrix and divide out by the
                        // adjusted_norm
                        quat_qf_eval(&norm, gram, &point);
                        ibz_div(&norm, &remain, &norm, adjusted_norm);
                        assert(ibz_is_zero(&remain));

                        if (ibz_mod_ui(&norm, 2) == 1) {
                            ibz_set(&vecs[count][0], x);
                            ibz_set(&vecs[count][1], y);
                            ibz_set(&vecs[count][2], z);
                            ibz_set(&vecs[count][3], w);
                            ibz_copy(&norms[count], &norm);
                            count++;
                        }
                    }
                }
            }
        }
    }

    ibz_finalize(&remain);
    ibz_finalize(&norm);
    ibz_vec_4_finalize(&point);

    return count - 1;
}

// enumerate through the two list given in input to find to integer d1,d2 such
// that there exists u,v with u d1 + v d2 = target the bool is diagonal
// indicates if the two lists are the same
static int
find_uv_from_lists(ibz_t *au,
                   ibz_t *bu,
                   ibz_t *av,
                   ibz_t *bv,
                   ibz_t *u,
                   ibz_t *v,
                   int *index_sol1,
                   int *index_sol2,
                   const ibz_t *target,
                   const ibz_t *small_norms1,
                   const ibz_t *small_norms2,
                   const ibz_t *quotients,
                   const int index1,
                   const int index2,
                   const int is_diagonal,
                   const int number_sum_square)
{

    ibz_t n, remain, adjusted_norm;
    ibz_init(&n);
    ibz_init(&remain);
    ibz_init(&adjusted_norm);

    int found = 0;
    int cmp;
    ibz_copy(&n, target);

    // enumerating through the list
    for (int i1 = 0; i1 < index1; i1++) {
        ibz_mod(&adjusted_norm, &n, &small_norms1[i1]);
        int starting_index2;
        if (is_diagonal) {
            starting_index2 = i1;
        } else {
            starting_index2 = 0;
        }
        for (int i2 = starting_index2; i2 < index2; i2++) {
            // u = target / d1 mod d2
            if (!ibz_invmod(&remain, &small_norms2[i2], &small_norms1[i1])) {
                continue;
            }
            ibz_mul(v, &remain, &adjusted_norm);
            ibz_mod(v, v, &small_norms1[i1]);
            cmp = ibz_cmp(v, &quotients[i2]);
            while (!found && cmp < 0) {
                if (number_sum_square > 0) {
                    found = ibz_cornacchia_prime(av, bv, &ibz_const_one, v);
                } else if (number_sum_square == 0) {
                    found = 1;
                }
                if (found) {
                    ibz_mul(&remain, v, &small_norms2[i2]);
                    ibz_copy(au, &n);
                    ibz_sub(u, au, &remain);
                    assert(ibz_cmp(u, &ibz_const_zero) > 0);
                    ibz_div(u, &remain, u, &small_norms1[i1]);
                    assert(ibz_is_zero(&remain));
                    // we want to remove weird cases where u,v have big power of two
                    found = found && (ibz_get(u) != 0 && ibz_get(v) != 0);
                    if (number_sum_square == 2) {
                        found = ibz_cornacchia_prime(au, bu, &ibz_const_one, u);
                    }
                }
                if (!found) {
                    ibz_add(v, v, &small_norms1[i1]);
                    cmp = ibz_cmp(v, &quotients[i2]);
                }
            }

            if (found) {
                // copying the indices
                *index_sol1 = i1;
                *index_sol2 = i2;
                break;
            }
        }
        if (found) {
            break;
        }
    }

    ibz_finalize(&n);
    ibz_finalize(&remain);
    ibz_finalize(&adjusted_norm);

    return found;
}

struct vec_and_norm
{
    ibz_vec_4_t vec;
    ibz_t norm;
    int idx;
};

static int
compare_vec_by_norm(const void *_first, const void *_second)
{
    const struct vec_and_norm *first = _first, *second = _second;
    int res = ibz_cmp(&first->norm, &second->norm);
    if (res != 0)
        return res;
    else
        return first->idx - second->idx;
}

// use several special curves
// we assume that the first one is always j=1728
int
find_uv(ibz_t *u,
        ibz_t *v,
        quat_alg_elem_t *beta1,
        quat_alg_elem_t *beta2,
        ibz_t *d1,
        ibz_t *d2,
        int *index_alternate_order_1,
        int *index_alternate_order_2,
        const ibz_t *target,
        const quat_left_ideal_t *lideal,
        const quat_alg_t *Bpoo,
        int num_alternate_order)

{

    // variable declaration & init
    ibz_vec_4_t vec;
    ibz_t n;
    ibz_t au, bu, av, bv;
    ibz_t norm_d;
    ibz_t remain;
    ibz_init(&au);
    ibz_init(&bu);
    ibz_init(&av);
    ibz_init(&bv);
    ibz_init(&norm_d);
    ibz_init(&n);
    ibz_vec_4_init(&vec);
    ibz_init(&remain);

    ibz_copy(&n, target);

    ibz_t adjusted_norm[num_alternate_order + 1];
    ibz_mat_4x4_t gram[num_alternate_order + 1], reduced[num_alternate_order + 1];
    quat_left_ideal_t ideal[num_alternate_order + 1];

    for (int i = 0; i < num_alternate_order + 1; i++) {
        ibz_init(&adjusted_norm[i]);
        ibz_mat_4x4_init(&gram[i]);
        ibz_mat_4x4_init(&reduced[i]);
        quat_left_ideal_init(&ideal[i]);
    }

    // first we reduce the ideal given in input
    quat_lideal_copy(&ideal[0], lideal);
    quat_lideal_reduce_basis(&reduced[0], &gram[0], &ideal[0], Bpoo);

    ibz_mat_4x4_copy(&ideal[0].lattice.basis, &reduced[0]);
    ibz_set(&adjusted_norm[0], 1);
    ibz_mul(&adjusted_norm[0], &adjusted_norm[0], &ideal[0].lattice.denom);
    ibz_mul(&adjusted_norm[0], &adjusted_norm[0], &ideal[0].lattice.denom);
    post_LLL_basis_treatment(&gram[0], &reduced[0], &ideal[0].norm, true);

    // for efficient lattice reduction, we replace ideal[0] by the equivalent
    // ideal of smallest norm
    quat_left_ideal_t reduced_id;
    quat_left_ideal_init(&reduced_id);
    quat_lideal_copy(&reduced_id, &ideal[0]);
    quat_alg_elem_t delta;
    // delta will be the element of smallest norm
    quat_alg_elem_init(&delta);
    ibz_set(&delta.coord[0], 1);
    ibz_set(&delta.coord[1], 0);
    ibz_set(&delta.coord[2], 0);
    ibz_set(&delta.coord[3], 0);
    ibz_copy(&delta.denom, &reduced_id.lattice.denom);
    ibz_mat_4x4_eval(&delta.coord, &reduced[0], &delta.coord);
    assert(quat_lattice_contains(NULL, &reduced_id.lattice, &delta));

    // reduced_id = ideal[0] * \overline{delta}/n(ideal[0])
    quat_alg_conj(&delta, &delta);
    ibz_mul(&delta.denom, &delta.denom, &ideal[0].norm);
    quat_lattice_alg_elem_mul(&reduced_id.lattice, &reduced_id.lattice, &delta, Bpoo);
    ibz_copy(&reduced_id.norm, &gram[0][0][0]);
    ibz_div(&reduced_id.norm, &remain, &reduced_id.norm, &adjusted_norm[0]);
    assert(ibz_cmp(&remain, &ibz_const_zero) == 0);

    // and conj_ideal is the conjugate of reduced_id
    // init the right order;
    quat_lattice_t right_order;
    quat_lattice_init(&right_order);
    // computing the conjugate
    quat_left_ideal_t conj_ideal;
    quat_left_ideal_init(&conj_ideal);
    quat_lideal_conjugate_without_hnf(&conj_ideal, &right_order, &reduced_id, Bpoo);

    // computing all the other connecting ideals and reducing them
    for (int i = 1; i < num_alternate_order + 1; i++) {
        quat_lideal_lideal_mul_reduced(&ideal[i], &gram[i], &conj_ideal, &ALTERNATE_CONNECTING_IDEALS[i - 1], Bpoo);
        ibz_mat_4x4_copy(&reduced[i], &ideal[i].lattice.basis);
        ibz_set(&adjusted_norm[i], 1);
        ibz_mul(&adjusted_norm[i], &adjusted_norm[i], &ideal[i].lattice.denom);
        ibz_mul(&adjusted_norm[i], &adjusted_norm[i], &ideal[i].lattice.denom);
        post_LLL_basis_treatment(&gram[i], &reduced[i], &ideal[i].norm, false);
    }

    // enumerating small vectors

    // global parameters for the enumeration
    int m = FINDUV_box_size;
    int m4 = FINDUV_cube_size;

    ibz_vec_4_t small_vecs[num_alternate_order + 1][m4];
    ibz_t small_norms[num_alternate_order + 1][m4];
    ibz_vec_4_t alternate_small_vecs[num_alternate_order + 1][m4];
    ibz_t alternate_small_norms[num_alternate_order + 1][m4];
    ibz_t quotients[num_alternate_order + 1][m4];
    int indices[num_alternate_order + 1];

    for (int j = 0; j < num_alternate_order + 1; j++) {
        for (int i = 0; i < m4; i++) {
            ibz_init(&small_norms[j][i]);
            ibz_vec_4_init(&small_vecs[j][i]);
            ibz_init(&alternate_small_norms[j][i]);
            ibz_init(&quotients[j][i]);
            ibz_vec_4_init(&alternate_small_vecs[j][i]);
        }
        // enumeration in the hypercube of norm m
        indices[j] = enumerate_hypercube(small_vecs[j], small_norms[j], m, &gram[j], &adjusted_norm[j]);

        // sorting the list
        {
            struct vec_and_norm small_vecs_and_norms[indices[j]];
            for (int i = 0; i < indices[j]; ++i) {
                memcpy(&small_vecs_and_norms[i].vec, &small_vecs[j][i], sizeof(ibz_vec_4_t));
                memcpy(&small_vecs_and_norms[i].norm, &small_norms[j][i], sizeof(ibz_t));
                small_vecs_and_norms[i].idx = i;
            }
            qsort(small_vecs_and_norms, indices[j], sizeof(*small_vecs_and_norms), compare_vec_by_norm);
            for (int i = 0; i < indices[j]; ++i) {
                memcpy(&small_vecs[j][i], &small_vecs_and_norms[i].vec, sizeof(ibz_vec_4_t));
                memcpy(&small_norms[j][i], &small_vecs_and_norms[i].norm, sizeof(ibz_t));
            }
#ifndef NDEBUG
            for (int i = 1; i < indices[j]; ++i)
                assert(ibz_cmp(&small_norms[j][i - 1], &small_norms[j][i]) <= 0);
#endif
        }

        for (int i = 0; i < indices[j]; i++) {
            ibz_div(&quotients[j][i], &remain, &n, &small_norms[j][i]);
        }
    }

    int found = 0;
    int i1;
    int i2;
    for (int j1 = 0; j1 < num_alternate_order + 1; j1++) {
        for (int j2 = j1; j2 < num_alternate_order + 1; j2++) {
            // in this case, there are some small adjustements to make
            int is_diago = (j1 == j2);
            found = find_uv_from_lists(&au,
                                       &bu,
                                       &av,
                                       &bv,
                                       u,
                                       v,
                                       &i1,
                                       &i2,
                                       target,
                                       small_norms[j1],
                                       small_norms[j2],
                                       quotients[j2],
                                       indices[j1],
                                       indices[j2],
                                       is_diago,
                                       0);
            // }

            if (found) {
                // recording the solutions that we found
                ibz_copy(&beta1->denom, &ideal[j1].lattice.denom);
                ibz_copy(&beta2->denom, &ideal[j2].lattice.denom);
                ibz_copy(d1, &small_norms[j1][i1]);
                ibz_copy(d2, &small_norms[j2][i2]);
                ibz_mat_4x4_eval(&beta1->coord, &reduced[j1], &small_vecs[j1][i1]);
                ibz_mat_4x4_eval(&beta2->coord, &reduced[j2], &small_vecs[j2][i2]);
                assert(quat_lattice_contains(NULL, &ideal[j1].lattice, beta1));
                assert(quat_lattice_contains(NULL, &ideal[j2].lattice, beta2));
                if (j1 != 0 || j2 != 0) {
                    ibz_div(&delta.denom, &remain, &delta.denom, &lideal->norm);
                    assert(ibz_cmp(&remain, &ibz_const_zero) == 0);
                    ibz_mul(&delta.denom, &delta.denom, &conj_ideal.norm);
                }
                if (j1 != 0) {
                    // we send back beta1 to the original ideal
                    quat_alg_mul(beta1, &delta, beta1, Bpoo);
                    quat_alg_normalize(beta1);
                }
                if (j2 != 0) {
                    // we send back beta2 to the original ideal
                    quat_alg_mul(beta2, &delta, beta2, Bpoo);
                    quat_alg_normalize(beta2);
                }

                // if the selected element belong to an alternate order, we conjugate it
                if (j1 != 0) {
                    quat_alg_conj(beta1, beta1);
                }
                if (j2 != 0) {
                    quat_alg_conj(beta2, beta2);
                }

#ifndef NDEBUG
                quat_alg_norm(&remain, &norm_d, beta1, &QUATALG_PINFTY);
                assert(ibz_is_one(&norm_d));
                ibz_mul(&n, d1, &ideal->norm);
                if (j1 > 0) {
                    ibz_mul(&n, &n, &ALTERNATE_CONNECTING_IDEALS[j1 - 1].norm);
                }
                assert(ibz_cmp(&n, &remain) == 0);
                quat_alg_norm(&remain, &norm_d, beta2, &QUATALG_PINFTY);
                assert(ibz_is_one(&norm_d));
                ibz_mul(&n, d2, &ideal->norm);
                if (j2 > 0) {
                    ibz_mul(&n, &n, &ALTERNATE_CONNECTING_IDEALS[j2 - 1].norm);
                }
                assert(ibz_cmp(&n, &remain) == 0);
                assert(quat_lattice_contains(NULL, &ideal->lattice, beta1));
                assert(quat_lattice_contains(NULL, &ideal->lattice, beta2));

                quat_left_ideal_t ideal_test;
                quat_lattice_t ro;
                quat_left_ideal_init(&ideal_test);
                quat_lattice_init(&ro);
                if (j1 > 0) {
                    quat_lideal_copy(&ideal_test, &ALTERNATE_CONNECTING_IDEALS[j1 - 1]);
                    quat_lideal_conjugate_without_hnf(&ideal_test, &ro, &ideal_test, Bpoo);
                    quat_lideal_lideal_mul_reduced(&ideal_test, &gram[0], &ideal_test, ideal, Bpoo);
                    assert(quat_lattice_contains(NULL, &ideal_test.lattice, beta1));
                }
                if (j2 > 0) {
                    quat_lideal_copy(&ideal_test, &ALTERNATE_CONNECTING_IDEALS[j2 - 1]);
                    quat_lideal_conjugate_without_hnf(&ideal_test, &ro, &ideal_test, Bpoo);
                    quat_lideal_lideal_mul_reduced(&ideal_test, &gram[0], &ideal_test, ideal, Bpoo);
                    assert(quat_lattice_contains(NULL, &ideal_test.lattice, beta2));
                }

                quat_lattice_finalize(&ro);
                quat_left_ideal_finalize(&ideal_test);
#endif

                *index_alternate_order_1 = j1;
                *index_alternate_order_2 = j2;
                break;
            }
        }
        if (found) {
            break;
        }
    }

    for (int j = 0; j < num_alternate_order + 1; j++) {
        for (int i = 0; i < m4; i++) {
            ibz_finalize(&small_norms[j][i]);
            ibz_vec_4_finalize(&small_vecs[j][i]);
            ibz_finalize(&alternate_small_norms[j][i]);
            ibz_finalize(&quotients[j][i]);
            ibz_vec_4_finalize(&alternate_small_vecs[j][i]);
        }
    }

    // var finalize
    for (int i = 0; i < num_alternate_order + 1; i++) {
        ibz_mat_4x4_finalize(&gram[i]);
        ibz_mat_4x4_finalize(&reduced[i]);
        quat_left_ideal_finalize(&ideal[i]);
        ibz_finalize(&adjusted_norm[i]);
    }

    ibz_finalize(&n);
    ibz_vec_4_finalize(&vec);
    ibz_finalize(&au);
    ibz_finalize(&bu);
    ibz_finalize(&av);
    ibz_finalize(&bv);
    ibz_finalize(&remain);
    ibz_finalize(&norm_d);
    quat_lattice_finalize(&right_order);
    quat_left_ideal_finalize(&conj_ideal);
    quat_left_ideal_finalize(&reduced_id);
    quat_alg_elem_finalize(&delta);

    return found;
}

int
dim2id2iso_ideal_to_isogeny_clapotis(quat_alg_elem_t *beta1,
                                     quat_alg_elem_t *beta2,
                                     ibz_t *u,
                                     ibz_t *v,
                                     ibz_t *d1,
                                     ibz_t *d2,
                                     ec_curve_t *codomain,
                                     ec_basis_t *basis,
                                     const quat_left_ideal_t *lideal,
                                     const quat_alg_t *Bpoo)
{
    ibz_t target, tmp, two_pow;
    ;
    quat_alg_elem_t theta;

    ibz_t norm_d;
    ibz_init(&norm_d);
    ibz_t test1, test2;
    ibz_init(&test1);
    ibz_init(&test2);

    ibz_init(&target);
    ibz_init(&tmp);
    ibz_init(&two_pow);
    int exp = TORSION_EVEN_POWER;
    quat_alg_elem_init(&theta);

    // first, we find u,v,d1,d2,beta1,beta2
    // such that u*d1 + v*d2 = 2^TORSION_EVEN_POWER and there are ideals of
    // norm d1,d2 equivalent to ideal beta1 and beta2 are elements of norm nd1,
    // nd2 where n=n(lideal)
    int ret;
    int index_order1 = 0, index_order2 = 0;
#ifndef NDEBUG
    unsigned int Fu_length, Fv_length;
#endif
    ret = find_uv(u,
                  v,
                  beta1,
                  beta2,
                  d1,
                  d2,
                  &index_order1,
                  &index_order2,
                  &TORSION_PLUS_2POWER,
                  lideal,
                  Bpoo,
                  NUM_ALTERNATE_EXTREMAL_ORDERS);
    if (!ret) {
        goto cleanup;
    }

    assert(ibz_is_odd(d1) && ibz_is_odd(d2));
    // compute the valuation of the GCD of u,v
    ibz_gcd(&tmp, u, v);
    assert(ibz_cmp(&tmp, &ibz_const_zero) != 0);
    int exp_gcd = ibz_two_adic(&tmp);
    exp = TORSION_EVEN_POWER - exp_gcd;
    // removing the power of 2 from u and v
    ibz_div(u, &test1, u, &tmp);
    assert(ibz_cmp(&test1, &ibz_const_zero) == 0);
    ibz_div(v, &test1, v, &tmp);
    assert(ibz_cmp(&test1, &ibz_const_zero) == 0);

#ifndef NDEBUG
    // checking that ud1+vd2 = 2^exp
    ibz_t pow_check, tmp_check;
    ibz_init(&pow_check);
    ibz_init(&tmp_check);
    ibz_pow(&pow_check, &ibz_const_two, exp);
    ibz_mul(&tmp_check, d1, u);
    ibz_sub(&pow_check, &pow_check, &tmp_check);
    ibz_mul(&tmp_check, v, d2);
    ibz_sub(&pow_check, &pow_check, &tmp_check);
    assert(ibz_cmp(&pow_check, &ibz_const_zero) == 0);
    ibz_finalize(&tmp_check);
    ibz_finalize(&pow_check);
#endif

    // now we compute the dimension 2 isogeny
    // F : Eu x Ev -> E x E'
    // where we have phi_u : Eu -> E_index_order1 and phi_v : Ev -> E_index_order2
    // if we have phi1 : E_index_order_1 -> E of degree d1
    // and phi2 : E_index_order_2 -> E of degree d2
    // we can define theta = phi2 o hat{phi1}
    // and the kernel of F is given by
    // ( [ud1](P), phiv o theta o hat{phiu} (P)),( [ud1](Q), phiv o theta o
    // hat{phiu} (Q)) where P,Q is a basis of E0[2e]

    // now we set-up the kernel
    // ec_curve_t E0 = CURVE_E0;
    ec_curve_t E1;
    copy_curve(&E1, &CURVES_WITH_ENDOMORPHISMS[index_order1].curve);
    ec_curve_t E2;
    copy_curve(&E2, &CURVES_WITH_ENDOMORPHISMS[index_order2].curve);
    ec_basis_t bas1, bas2;
    theta_couple_curve_t E01;
    theta_kernel_couple_points_t ker;

    ec_basis_t bas_u;
    copy_basis(&bas1, &CURVES_WITH_ENDOMORPHISMS[index_order1].basis_even);
    copy_basis(&bas2, &CURVES_WITH_ENDOMORPHISMS[index_order2].basis_even);

    // we start by computing theta = beta2 \hat{beta1}/n
    ibz_set(&theta.denom, 1);
    quat_alg_conj(&theta, beta1);
    quat_alg_mul(&theta, beta2, &theta, &QUATALG_PINFTY);
    ibz_mul(&theta.denom, &theta.denom, &lideal->norm);

    // now we perform the actual computation
    quat_left_ideal_t idealu, idealv;
    quat_left_ideal_init(&idealu);
    quat_left_ideal_init(&idealv);
    theta_couple_curve_t Fu_codomain, Fv_codomain;
    theta_couple_point_t pushed_points[3];
    theta_couple_point_t *const V1 = pushed_points + 0, *const V2 = pushed_points + 1, *const V1m2 = pushed_points + 2;
    theta_couple_point_t P, Q, PmQ;

    copy_point(&P.P1, &bas1.P);
    copy_point(&PmQ.P1, &bas1.PmQ);
    copy_point(&Q.P1, &bas1.Q);
    // Set points to zero
    ec_point_init(&P.P2);
    ec_point_init(&Q.P2);
    ec_point_init(&PmQ.P2);

    pushed_points[0] = P;
    pushed_points[1] = Q;
    pushed_points[2] = PmQ;
    // we perform the computation of phiu with a fixed degree isogeny
    ret = fixed_degree_isogeny_and_eval(
        &idealu, u, true, &Fu_codomain, pushed_points, sizeof(pushed_points) / sizeof(*pushed_points), index_order1);

    if (!ret) {
        goto cleanup;
    }
    assert(test_point_order_twof(&V1->P1, &Fu_codomain.E1, TORSION_EVEN_POWER));
    assert(test_point_order_twof(&V1->P2, &Fu_codomain.E2, TORSION_EVEN_POWER));

#ifndef NDEBUG
    Fu_length = (unsigned int)ret;
    // presumably the correct curve is the first one, we check this
    fp2_t w0a, w1a, w2a;
    ec_curve_t E1_tmp, Fu_codomain_E1_tmp, Fu_codomain_E2_tmp;
    copy_curve(&E1_tmp, &E1);
    copy_curve(&Fu_codomain_E1_tmp, &Fu_codomain.E1);
    copy_curve(&Fu_codomain_E2_tmp, &Fu_codomain.E2);
    weil(&w0a, TORSION_EVEN_POWER, &bas1.P, &bas1.Q, &bas1.PmQ, &E1_tmp);
    weil(&w1a, TORSION_EVEN_POWER, &V1->P1, &V2->P1, &V1m2->P1, &Fu_codomain_E1_tmp);
    weil(&w2a, TORSION_EVEN_POWER, &V1->P2, &V2->P2, &V1m2->P2, &Fu_codomain_E2_tmp);
    ibz_pow(&two_pow, &ibz_const_two, Fu_length);
    ibz_sub(&two_pow, &two_pow, u);

    // now we are checking that the weil pairings are equal to the correct value
    digit_t digit_u[NWORDS_ORDER] = { 0 };
    ibz_to_digit_array(digit_u, u);
    fp2_t test_powa;
    fp2_pow_vartime(&test_powa, &w0a, digit_u, NWORDS_ORDER);

    assert(fp2_is_equal(&test_powa, &w1a));
    ibz_to_digit_array(digit_u, &two_pow);
    fp2_pow_vartime(&test_powa, &w0a, digit_u, NWORDS_ORDER);
    assert(fp2_is_equal(&test_powa, &w2a));
#endif

    // copying the basis images
    copy_point(&bas_u.P, &V1->P1);
    copy_point(&bas_u.Q, &V2->P1);
    copy_point(&bas_u.PmQ, &V1m2->P1);

    // copying the points to the first part of the kernel
    copy_point(&ker.T1.P1, &bas_u.P);
    copy_point(&ker.T2.P1, &bas_u.Q);
    copy_point(&ker.T1m2.P1, &bas_u.PmQ);
    copy_curve(&E01.E1, &Fu_codomain.E1);

    copy_point(&P.P1, &bas2.P);
    copy_point(&PmQ.P1, &bas2.PmQ);
    copy_point(&Q.P1, &bas2.Q);
    pushed_points[0] = P;
    pushed_points[1] = Q;
    pushed_points[2] = PmQ;

    // computation of phiv
    ret = fixed_degree_isogeny_and_eval(
        &idealv, v, true, &Fv_codomain, pushed_points, sizeof(pushed_points) / sizeof(*pushed_points), index_order2);
    if (!ret) {
        goto cleanup;
    }

    assert(test_point_order_twof(&V1->P1, &Fv_codomain.E1, TORSION_EVEN_POWER));
    assert(test_point_order_twof(&V1->P2, &Fv_codomain.E2, TORSION_EVEN_POWER));

#ifndef NDEBUG
    Fv_length = (unsigned int)ret;
    ec_curve_t E2_tmp, Fv_codomain_E1_tmp, Fv_codomain_E2_tmp;
    copy_curve(&E2_tmp, &E2);
    copy_curve(&Fv_codomain_E1_tmp, &Fv_codomain.E1);
    copy_curve(&Fv_codomain_E2_tmp, &Fv_codomain.E2);
    // presumably the correct curve is the first one, we check this
    weil(&w0a, TORSION_EVEN_POWER, &bas2.P, &bas2.Q, &bas2.PmQ, &E2_tmp);
    weil(&w1a, TORSION_EVEN_POWER, &V1->P1, &V2->P1, &V1m2->P1, &Fv_codomain_E1_tmp);
    weil(&w2a, TORSION_EVEN_POWER, &V1->P2, &V2->P2, &V1m2->P2, &Fv_codomain_E2_tmp);
    if (Fv_length == 0) {
        ibz_set(&tmp, 1);
        ibz_set(&two_pow, 1);
    } else {
        ibz_pow(&two_pow, &ibz_const_two, Fv_length);
        ibz_sub(&two_pow, &two_pow, v);
    }

    // now we are checking that one of the two is equal to the correct value
    ibz_to_digit_array(digit_u, v);
    fp2_pow_vartime(&test_powa, &w0a, digit_u, NWORDS_ORDER);
    assert(fp2_is_equal(&test_powa, &w1a));
    ibz_to_digit_array(digit_u, &two_pow);
    fp2_pow_vartime(&test_powa, &w0a, digit_u, NWORDS_ORDER);
    assert(fp2_is_equal(&test_powa, &w2a));

#endif

    copy_point(&bas2.P, &V1->P1);
    copy_point(&bas2.Q, &V2->P1);
    copy_point(&bas2.PmQ, &V1m2->P1);

    // multiplying theta by 1 / (d1 * n(connecting_ideal2))
    ibz_pow(&two_pow, &ibz_const_two, TORSION_EVEN_POWER);
    ibz_copy(&tmp, d1);
    if (index_order2 > 0) {
        ibz_mul(&tmp, &tmp, &ALTERNATE_CONNECTING_IDEALS[index_order2 - 1].norm);
    }
    ibz_invmod(&tmp, &tmp, &two_pow);

    ibz_mul(&theta.coord[0], &theta.coord[0], &tmp);
    ibz_mul(&theta.coord[1], &theta.coord[1], &tmp);
    ibz_mul(&theta.coord[2], &theta.coord[2], &tmp);
    ibz_mul(&theta.coord[3], &theta.coord[3], &tmp);

    // applying theta
    endomorphism_application_even_basis(&bas2, 0, &Fv_codomain.E1, &theta, TORSION_EVEN_POWER);

    assert(test_basis_order_twof(&bas2, &Fv_codomain.E1, TORSION_EVEN_POWER));

    // copying points to the second part of the kernel
    copy_point(&ker.T1.P2, &bas2.P);
    copy_point(&ker.T2.P2, &bas2.Q);
    copy_point(&ker.T1m2.P2, &bas2.PmQ);
    copy_curve(&E01.E2, &Fv_codomain.E1);

    // copying the points to the first part of the kernel
    quat_left_ideal_finalize(&idealu);
    quat_left_ideal_finalize(&idealv);

    double_couple_point_iter(&ker.T1, TORSION_EVEN_POWER - exp, &ker.T1, &E01);
    double_couple_point_iter(&ker.T2, TORSION_EVEN_POWER - exp, &ker.T2, &E01);
    double_couple_point_iter(&ker.T1m2, TORSION_EVEN_POWER - exp, &ker.T1m2, &E01);

    assert(test_point_order_twof(&ker.T1.P1, &E01.E1, exp));
    assert(test_point_order_twof(&ker.T1m2.P2, &E01.E2, exp));

    assert(ibz_is_odd(u));

    // now we evaluate the basis points through the isogeny
    assert(test_basis_order_twof(&bas_u, &E01.E1, TORSION_EVEN_POWER));

    // evaluating the basis through the isogeny of degree u*d1
    copy_point(&pushed_points[0].P1, &bas_u.P);
    copy_point(&pushed_points[2].P1, &bas_u.PmQ);
    copy_point(&pushed_points[1].P1, &bas_u.Q);
    // Set points to zero
    ec_point_init(&pushed_points[0].P2);
    ec_point_init(&pushed_points[1].P2);
    ec_point_init(&pushed_points[2].P2);

    theta_couple_curve_t theta_codomain;

    ret = theta_chain_compute_and_eval_randomized(
        exp, &E01, &ker, false, &theta_codomain, pushed_points, sizeof(pushed_points) / sizeof(*pushed_points));
    if (!ret) {
        goto cleanup;
    }

    theta_couple_point_t T1, T2, T1m2;
    T1 = pushed_points[0];
    T2 = pushed_points[1];
    T1m2 = pushed_points[2];

    assert(test_point_order_twof(&T1.P2, &theta_codomain.E2, TORSION_EVEN_POWER));
    assert(test_point_order_twof(&T1.P1, &theta_codomain.E1, TORSION_EVEN_POWER));
    assert(test_point_order_twof(&T1m2.P2, &theta_codomain.E2, TORSION_EVEN_POWER));

    copy_point(&basis->P, &T1.P1);
    copy_point(&basis->Q, &T2.P1);
    copy_point(&basis->PmQ, &T1m2.P1);
    copy_curve(codomain, &theta_codomain.E1);

    // using weil pairing to verify that we selected the correct curve
    fp2_t w0, w1;
    // ec_curve_t E0 = CURVE_E0;
    // ec_basis_t bas0 = BASIS_EVEN;
    weil(&w0, TORSION_EVEN_POWER, &bas1.P, &bas1.Q, &bas1.PmQ, &E1);
    weil(&w1, TORSION_EVEN_POWER, &basis->P, &basis->Q, &basis->PmQ, codomain);

    digit_t digit_d[NWORDS_ORDER] = { 0 };
    ibz_mul(&tmp, d1, u);
    ibz_mul(&tmp, &tmp, u);
    ibz_mod(&tmp, &tmp, &TORSION_PLUS_2POWER);
    ibz_to_digit_array(digit_d, &tmp);
    fp2_t test_pow;
    fp2_pow_vartime(&test_pow, &w0, digit_d, NWORDS_ORDER);

    // then we have selected the wrong one
    if (!fp2_is_equal(&w1, &test_pow)) {
        copy_point(&basis->P, &T1.P2);
        copy_point(&basis->Q, &T2.P2);
        copy_point(&basis->PmQ, &T1m2.P2);
        copy_curve(codomain, &theta_codomain.E2);

// verifying that the other one is the good one
#ifndef NDEBUG
        ec_curve_t codomain_tmp;
        copy_curve(&codomain_tmp, codomain);
        weil(&w1, TORSION_EVEN_POWER, &basis->P, &basis->Q, &basis->PmQ, &codomain_tmp);
        fp2_pow_vartime(&test_pow, &w0, digit_d, NWORDS_ORDER);
        assert(fp2_is_equal(&test_pow, &w1));
#endif
    }

    // now we apply M / (u * d1) where M is the matrix corresponding to the
    // endomorphism beta1 = phi o dual(phi1) we multiply beta1 by the inverse of
    // (u*d1) mod 2^TORSION_EVEN_POWER
    ibz_mul(&tmp, u, d1);
    if (index_order1 != 0) {
        ibz_mul(&tmp, &tmp, &CONNECTING_IDEALS[index_order1].norm);
    }
    ibz_invmod(&tmp, &tmp, &TORSION_PLUS_2POWER);
    ibz_mul(&beta1->coord[0], &beta1->coord[0], &tmp);
    ibz_mul(&beta1->coord[1], &beta1->coord[1], &tmp);
    ibz_mul(&beta1->coord[2], &beta1->coord[2], &tmp);
    ibz_mul(&beta1->coord[3], &beta1->coord[3], &tmp);

    endomorphism_application_even_basis(basis, 0, codomain, beta1, TORSION_EVEN_POWER);

#ifndef NDEBUG
    {
        ec_curve_t E0 = CURVE_E0;
        ec_curve_t codomain_tmp;
        ec_basis_t bas0 = CURVES_WITH_ENDOMORPHISMS[0].basis_even;
        copy_curve(&codomain_tmp, codomain);
        copy_curve(&E1_tmp, &E1);
        copy_curve(&E2_tmp, &E2);
        weil(&w0a, TORSION_EVEN_POWER, &bas0.P, &bas0.Q, &bas0.PmQ, &E0);
        weil(&w1a, TORSION_EVEN_POWER, &basis->P, &basis->Q, &basis->PmQ, &codomain_tmp);
        digit_t tmp_d[2 * NWORDS_ORDER] = { 0 };
        if (index_order1 != 0) {
            copy_basis(&bas1, &CURVES_WITH_ENDOMORPHISMS[index_order1].basis_even);
            weil(&w0, TORSION_EVEN_POWER, &bas1.P, &bas1.Q, &bas1.PmQ, &E1_tmp);
            ibz_to_digit_array(tmp_d, &CONNECTING_IDEALS[index_order1].norm);
            fp2_pow_vartime(&test_pow, &w0a, tmp_d, 2 * NWORDS_ORDER);
            assert(fp2_is_equal(&test_pow, &w0));
        }
        if (index_order2 != 0) {
            copy_basis(&bas2, &CURVES_WITH_ENDOMORPHISMS[index_order2].basis_even);
            weil(&w0, TORSION_EVEN_POWER, &bas2.P, &bas2.Q, &bas2.PmQ, &E2_tmp);
            ibz_to_digit_array(tmp_d, &CONNECTING_IDEALS[index_order2].norm);
            fp2_pow_vartime(&test_pow, &w0a, tmp_d, 2 * NWORDS_ORDER);
            assert(fp2_is_equal(&test_pow, &w0));
        }
        ibz_to_digit_array(tmp_d, &lideal->norm);
        fp2_pow_vartime(&test_pow, &w0a, tmp_d, 2 * NWORDS_ORDER);
        assert(fp2_is_equal(&test_pow, &w1a));
    }
#endif

cleanup:
    ibz_finalize(&norm_d);
    ibz_finalize(&test1);
    ibz_finalize(&test2);
    ibz_finalize(&target);
    ibz_finalize(&tmp);
    ibz_finalize(&two_pow);
    quat_alg_elem_finalize(&theta);
    return ret;
}

int
dim2id2iso_arbitrary_isogeny_evaluation(ec_basis_t *basis, ec_curve_t *codomain, const quat_left_ideal_t *lideal)
{
    int ret;

    quat_alg_elem_t beta1, beta2;
    ibz_t u, v, d1, d2;

    quat_alg_elem_init(&beta1);
    quat_alg_elem_init(&beta2);

    ibz_init(&u);
    ibz_init(&v);
    ibz_init(&d1);
    ibz_init(&d2);

    ret = dim2id2iso_ideal_to_isogeny_clapotis(
        &beta1, &beta2, &u, &v, &d1, &d2, codomain, basis, lideal, &QUATALG_PINFTY);

    quat_alg_elem_finalize(&beta1);
    quat_alg_elem_finalize(&beta2);

    ibz_finalize(&u);
    ibz_finalize(&v);
    ibz_finalize(&d1);
    ibz_finalize(&d2);

    return ret;
}
