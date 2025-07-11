#include "internal.h"

void
quat_alg_init_set(quat_alg_t *alg, const ibz_t *p)
{
    ibz_init(&(*alg).p);
    ibz_copy(&(*alg).p, p);
}
void
quat_alg_finalize(quat_alg_t *alg)
{
    ibz_finalize(&(*alg).p);
}

void
quat_alg_elem_init(quat_alg_elem_t *elem)
{
    ibz_vec_4_init(&(*elem).coord);
    ibz_init(&(*elem).denom);
    ibz_set(&(*elem).denom, 1);
}
void
quat_alg_elem_finalize(quat_alg_elem_t *elem)
{
    ibz_vec_4_finalize(&(*elem).coord);
    ibz_finalize(&(*elem).denom);
}

void
ibz_vec_2_init(ibz_vec_2_t *vec)
{
    ibz_init(&((*vec)[0]));
    ibz_init(&((*vec)[1]));
}

void
ibz_vec_2_finalize(ibz_vec_2_t *vec)
{
    ibz_finalize(&((*vec)[0]));
    ibz_finalize(&((*vec)[1]));
}

void
ibz_vec_4_init(ibz_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibz_init(&(*vec)[i]);
    }
}
void
ibz_vec_4_finalize(ibz_vec_4_t *vec)
{
    for (int i = 0; i < 4; i++) {
        ibz_finalize(&(*vec)[i]);
    }
}

void
ibz_mat_2x2_init(ibz_mat_2x2_t *mat)
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_init(&(*mat)[i][j]);
        }
    }
}
void
ibz_mat_2x2_finalize(ibz_mat_2x2_t *mat)
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_finalize(&(*mat)[i][j]);
        }
    }
}

void
ibz_mat_4x4_init(ibz_mat_4x4_t *mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_init(&(*mat)[i][j]);
        }
    }
}
void
ibz_mat_4x4_finalize(ibz_mat_4x4_t *mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_finalize(&(*mat)[i][j]);
        }
    }
}

void
quat_lattice_init(quat_lattice_t *lat)
{
    ibz_mat_4x4_init(&(*lat).basis);
    ibz_init(&(*lat).denom);
    ibz_set(&(*lat).denom, 1);
}
void
quat_lattice_finalize(quat_lattice_t *lat)
{
    ibz_finalize(&(*lat).denom);
    ibz_mat_4x4_finalize(&(*lat).basis);
}

void
quat_left_ideal_init(quat_left_ideal_t *lideal)
{
    quat_lattice_init(&(*lideal).lattice);
    ibz_init(&(*lideal).norm);
    (*lideal).parent_order = NULL;
}
void
quat_left_ideal_finalize(quat_left_ideal_t *lideal)
{
    ibz_finalize(&(*lideal).norm);
    quat_lattice_finalize(&(*lideal).lattice);
}
