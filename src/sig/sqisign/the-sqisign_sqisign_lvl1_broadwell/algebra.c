#include <quaternion.h>
#include "internal.h"

// Internal helper functions

void
quat_alg_init_set_ui(quat_alg_t *alg, unsigned int p)
{
    ibz_t bp;
    ibz_init(&bp);
    ibz_set(&bp, p);
    quat_alg_init_set(alg, &bp);
    ibz_finalize(&bp);
}

void
quat_alg_coord_mul(ibz_vec_4_t *res, const ibz_vec_4_t *a, const ibz_vec_4_t *b, const quat_alg_t *alg)
{
    ibz_t prod;
    ibz_vec_4_t sum;
    ibz_init(&prod);
    ibz_vec_4_init(&sum);

    ibz_set(&(sum[0]), 0);
    ibz_set(&(sum[1]), 0);
    ibz_set(&(sum[2]), 0);
    ibz_set(&(sum[3]), 0);

    // compute 1 coordinate
    ibz_mul(&prod, &((*a)[2]), &((*b)[2]));
    ibz_sub(&(sum[0]), &(sum[0]), &prod);
    ibz_mul(&prod, &((*a)[3]), &((*b)[3]));
    ibz_sub(&(sum[0]), &(sum[0]), &prod);
    ibz_mul(&(sum[0]), &(sum[0]), &(alg->p));
    ibz_mul(&prod, &((*a)[0]), &((*b)[0]));
    ibz_add(&(sum[0]), &(sum[0]), &prod);
    ibz_mul(&prod, &((*a)[1]), &((*b)[1]));
    ibz_sub(&(sum[0]), &(sum[0]), &prod);
    // compute i coordiante
    ibz_mul(&prod, &((*a)[2]), &((*b)[3]));
    ibz_add(&(sum[1]), &(sum[1]), &prod);
    ibz_mul(&prod, &((*a)[3]), &((*b)[2]));
    ibz_sub(&(sum[1]), &(sum[1]), &prod);
    ibz_mul(&(sum[1]), &(sum[1]), &(alg->p));
    ibz_mul(&prod, &((*a)[0]), &((*b)[1]));
    ibz_add(&(sum[1]), &(sum[1]), &prod);
    ibz_mul(&prod, &((*a)[1]), &((*b)[0]));
    ibz_add(&(sum[1]), &(sum[1]), &prod);
    // compute j coordiante
    ibz_mul(&prod, &((*a)[0]), &((*b)[2]));
    ibz_add(&(sum[2]), &(sum[2]), &prod);
    ibz_mul(&prod, &((*a)[2]), &((*b)[0]));
    ibz_add(&(sum[2]), &(sum[2]), &prod);
    ibz_mul(&prod, &((*a)[1]), &((*b)[3]));
    ibz_sub(&(sum[2]), &(sum[2]), &prod);
    ibz_mul(&prod, &((*a)[3]), &((*b)[1]));
    ibz_add(&(sum[2]), &(sum[2]), &prod);
    // compute ij coordiante
    ibz_mul(&prod, &((*a)[0]), &((*b)[3]));
    ibz_add(&(sum[3]), &(sum[3]), &prod);
    ibz_mul(&prod, &((*a)[3]), &((*b)[0]));
    ibz_add(&(sum[3]), &(sum[3]), &prod);
    ibz_mul(&prod, &((*a)[2]), &((*b)[1]));
    ibz_sub(&(sum[3]), &(sum[3]), &prod);
    ibz_mul(&prod, &((*a)[1]), &((*b)[2]));
    ibz_add(&(sum[3]), &(sum[3]), &prod);

    ibz_copy(&((*res)[0]), &(sum[0]));
    ibz_copy(&((*res)[1]), &(sum[1]));
    ibz_copy(&((*res)[2]), &(sum[2]));
    ibz_copy(&((*res)[3]), &(sum[3]));

    ibz_finalize(&prod);
    ibz_vec_4_finalize(&sum);
}

void
quat_alg_equal_denom(quat_alg_elem_t *res_a, quat_alg_elem_t *res_b, const quat_alg_elem_t *a, const quat_alg_elem_t *b)
{
    ibz_t gcd, r;
    ibz_init(&gcd);
    ibz_init(&r);
    ibz_gcd(&gcd, &(a->denom), &(b->denom));
    // temporarily set res_a.denom to a.denom/gcd, and res_b.denom to b.denom/gcd
    ibz_div(&(res_a->denom), &r, &(a->denom), &gcd);
    ibz_div(&(res_b->denom), &r, &(b->denom), &gcd);
    for (int i = 0; i < 4; i++) {
        // multiply coordiates by reduced denominators from the other element
        ibz_mul(&(res_a->coord[i]), &(a->coord[i]), &(res_b->denom));
        ibz_mul(&(res_b->coord[i]), &(b->coord[i]), &(res_a->denom));
    }
    // multiply both reduced denominators
    ibz_mul(&(res_a->denom), &(res_a->denom), &(res_b->denom));
    // multiply them by the gcd to get the new common denominator
    ibz_mul(&(res_b->denom), &(res_a->denom), &gcd);
    ibz_mul(&(res_a->denom), &(res_a->denom), &gcd);
    ibz_finalize(&gcd);
    ibz_finalize(&r);
}

// Public Functions

void
quat_alg_add(quat_alg_elem_t *res, const quat_alg_elem_t *a, const quat_alg_elem_t *b)
{
    quat_alg_elem_t res_a, res_b;
    quat_alg_elem_init(&res_a);
    quat_alg_elem_init(&res_b);
    // put both on the same denominator
    quat_alg_equal_denom(&res_a, &res_b, a, b);
    // then add
    ibz_copy(&(res->denom), &(res_a.denom));
    ibz_vec_4_add(&(res->coord), &(res_a.coord), &(res_b.coord));
    quat_alg_elem_finalize(&res_a);
    quat_alg_elem_finalize(&res_b);
}

void
quat_alg_sub(quat_alg_elem_t *res, const quat_alg_elem_t *a, const quat_alg_elem_t *b)
{
    quat_alg_elem_t res_a, res_b;
    quat_alg_elem_init(&res_a);
    quat_alg_elem_init(&res_b);
    // put both on the same denominator
    quat_alg_equal_denom(&res_a, &res_b, a, b);
    // then substract
    ibz_copy(&res->denom, &res_a.denom);
    ibz_vec_4_sub(&res->coord, &res_a.coord, &res_b.coord);
    quat_alg_elem_finalize(&res_a);
    quat_alg_elem_finalize(&res_b);
}

void
quat_alg_mul(quat_alg_elem_t *res, const quat_alg_elem_t *a, const quat_alg_elem_t *b, const quat_alg_t *alg)
{
    // denominator: product of denominators
    ibz_mul(&(res->denom), &(a->denom), &(b->denom));
    quat_alg_coord_mul(&(res->coord), &(a->coord), &(b->coord), alg);
}

void
quat_alg_norm(ibz_t *res_num, ibz_t *res_denom, const quat_alg_elem_t *a, const quat_alg_t *alg)
{
    ibz_t r, g;
    quat_alg_elem_t norm;
    ibz_init(&r);
    ibz_init(&g);
    quat_alg_elem_init(&norm);

    quat_alg_conj(&norm, a);
    quat_alg_mul(&norm, a, &norm, alg);
    ibz_gcd(&g, &(norm.coord[0]), &(norm.denom));
    ibz_div(res_num, &r, &(norm.coord[0]), &g);
    ibz_div(res_denom, &r, &(norm.denom), &g);
    ibz_abs(res_denom, res_denom);
    ibz_abs(res_num, res_num);
    assert(ibz_cmp(res_denom, &ibz_const_zero) > 0);

    quat_alg_elem_finalize(&norm);
    ibz_finalize(&r);
    ibz_finalize(&g);
}

void
quat_alg_scalar(quat_alg_elem_t *elem, const ibz_t *numerator, const ibz_t *denominator)
{
    ibz_copy(&(elem->denom), denominator);
    ibz_copy(&(elem->coord[0]), numerator);
    ibz_set(&(elem->coord[1]), 0);
    ibz_set(&(elem->coord[2]), 0);
    ibz_set(&(elem->coord[3]), 0);
}

void
quat_alg_conj(quat_alg_elem_t *conj, const quat_alg_elem_t *x)
{
    ibz_copy(&(conj->denom), &(x->denom));
    ibz_copy(&(conj->coord[0]), &(x->coord[0]));
    ibz_neg(&(conj->coord[1]), &(x->coord[1]));
    ibz_neg(&(conj->coord[2]), &(x->coord[2]));
    ibz_neg(&(conj->coord[3]), &(x->coord[3]));
}

void
quat_alg_make_primitive(ibz_vec_4_t *primitive_x, ibz_t *content, const quat_alg_elem_t *x, const quat_lattice_t *order)
{
    int ok UNUSED = quat_lattice_contains(primitive_x, order, x);
    assert(ok);
    ibz_vec_4_content(content, primitive_x);
    ibz_t r;
    ibz_init(&r);
    for (int i = 0; i < 4; i++) {
        ibz_div(*primitive_x + i, &r, *primitive_x + i, content);
    }
    ibz_finalize(&r);
}

void
quat_alg_normalize(quat_alg_elem_t *x)
{
    ibz_t gcd, sign, r;
    ibz_init(&gcd);
    ibz_init(&sign);
    ibz_init(&r);
    ibz_vec_4_content(&gcd, &(x->coord));
    ibz_gcd(&gcd, &gcd, &(x->denom));
    ibz_div(&(x->denom), &r, &(x->denom), &gcd);
    ibz_vec_4_scalar_div(&(x->coord), &gcd, &(x->coord));
    ibz_set(&sign, 2 * (0 > ibz_cmp(&ibz_const_zero, &(x->denom))) - 1);
    ibz_vec_4_scalar_mul(&(x->coord), &sign, &(x->coord));
    ibz_mul(&(x->denom), &sign, &(x->denom));
    ibz_finalize(&gcd);
    ibz_finalize(&sign);
    ibz_finalize(&r);
}

int
quat_alg_elem_equal(const quat_alg_elem_t *a, const quat_alg_elem_t *b)
{
    quat_alg_elem_t diff;
    quat_alg_elem_init(&diff);
    quat_alg_sub(&diff, a, b);
    int res = quat_alg_elem_is_zero(&diff);
    quat_alg_elem_finalize(&diff);
    return (res);
}

int
quat_alg_elem_is_zero(const quat_alg_elem_t *x)
{
    int res = ibz_vec_4_is_zero(&(x->coord));
    return (res);
}

void
quat_alg_elem_set(quat_alg_elem_t *elem, int32_t denom, int32_t coord0, int32_t coord1, int32_t coord2, int32_t coord3)
{
    ibz_set(&(elem->coord[0]), coord0);
    ibz_set(&(elem->coord[1]), coord1);
    ibz_set(&(elem->coord[2]), coord2);
    ibz_set(&(elem->coord[3]), coord3);

    ibz_set(&(elem->denom), denom);
}

void
quat_alg_elem_copy(quat_alg_elem_t *copy, const quat_alg_elem_t *copied)
{
    ibz_copy(&copy->denom, &copied->denom);
    ibz_copy(&copy->coord[0], &copied->coord[0]);
    ibz_copy(&copy->coord[1], &copied->coord[1]);
    ibz_copy(&copy->coord[2], &copied->coord[2]);
    ibz_copy(&copy->coord[3], &copied->coord[3]);
}

// helper functions for lattices
void
quat_alg_elem_copy_ibz(quat_alg_elem_t *elem,
                       const ibz_t *denom,
                       const ibz_t *coord0,
                       const ibz_t *coord1,
                       const ibz_t *coord2,
                       const ibz_t *coord3)
{
    ibz_copy(&(elem->coord[0]), coord0);
    ibz_copy(&(elem->coord[1]), coord1);
    ibz_copy(&(elem->coord[2]), coord2);
    ibz_copy(&(elem->coord[3]), coord3);

    ibz_copy(&(elem->denom), denom);
}

void
quat_alg_elem_mul_by_scalar(quat_alg_elem_t *res, const ibz_t *scalar, const quat_alg_elem_t *elem)
{
    for (int i = 0; i < 4; i++) {
        ibz_mul(&(res->coord[i]), &(elem->coord[i]), scalar);
    }
    ibz_copy(&(res->denom), &(elem->denom));
}
