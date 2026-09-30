#include "internal.h"

void
quat_alg_init_set(quat_alg_t *alg, const ibz_t *p)
{
    ibz_init(&(*alg).p);
    ibz_copy(&(*alg).p, p);
}

void
quat_alg_elem_init(quat_alg_elem_t *elem)
{
    ibz_vec_4_init(&(*elem).coord);
    ibz_init(&(*elem).denom);
    ibz_set(&(*elem).denom, 1, 2);
}

void
ibz_vec_2_init(ibz_vec_2_t *vec)
{
    ibz_init(&(vec->v[0]));
    ibz_init(&(vec->v[1]));
}

void
ibz_vec_4_init(ibz_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibz_init(&vec->v[i]);
    }
}

void
ibz_mat_2x2_init(ibz_mat_2x2_t *mat)
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_init(&mat->m[i][j]);
        }
    }
}

void
ibz_mat_4x4_init(ibz_mat_4x4_t *mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_init(&mat->m[i][j]);
        }
    }
}

void
quat_lattice_init(quat_lattice_t *lat)
{
    ibz_mat_4x4_init(&(*lat).basis);
    ibz_init(&(*lat).denom);
    ibz_set(&(*lat).denom, 1, 2);
}

void
quat_ideal_init(quat_ideal_t *ideal)
{
    ibz_init(&ideal->x);
    ibz_init(&ideal->y);
    ibz_init(&ideal->norm);
}
