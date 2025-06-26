#include <stdio.h>
#include "internal.h"

void
ibz_mat_2x2_print(const ibz_mat_2x2_t *mat)
{
    printf("matrix: ");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_print(&((*mat)[i][j]), 10);
            printf(" ");
        }
        printf("\n        ");
    }
    printf("\n");
}

void
ibz_mat_4x4_print(const ibz_mat_4x4_t *mat)
{
    printf("matrix: ");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_print(&((*mat)[i][j]), 10);
            printf(" ");
        }
        printf("\n        ");
    }
    printf("\n");
}

void
ibz_vec_2_print(const ibz_vec_2_t *vec)
{
    printf("vector: ");
    for (int i = 0; i < 2; i++) {
        ibz_print(&((*vec)[i]), 10);
        printf(" ");
    }
    printf("\n\n");
}

void
ibz_vec_4_print(const ibz_vec_4_t *vec)
{
    printf("vector: ");
    for (int i = 0; i < 4; i++) {
        ibz_print(&((*vec)[i]), 10);
        printf(" ");
    }
    printf("\n\n");
}

void
quat_lattice_print(const quat_lattice_t *lat)
{
    printf("lattice\n");
    printf("denominator: ");
    ibz_print(&(lat->denom), 10);
    printf("\n");
    printf("basis: ");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_print(&((lat->basis)[i][j]), 10);
            printf(" ");
        }
        printf("\n       ");
    }
    printf("\n");
}

void
quat_alg_print(const quat_alg_t *alg)
{
    printf("quaternion algebra ramified at ");
    ibz_print(&(alg->p), 10);
    printf(" and infinity\n\n");
}

void
quat_alg_elem_print(const quat_alg_elem_t *elem)
{
    printf("denominator: ");
    ibz_print(&(elem->denom), 10);
    printf("\n");
    printf("coordinates: ");
    for (int i = 0; i < 4; i++) {
        ibz_print(&((elem->coord)[i]), 10);
        printf(" ");
    }
    printf("\n\n");
}

void
quat_left_ideal_print(const quat_left_ideal_t *lideal)
{
    printf("left ideal\n");
    printf("norm: ");
    ibz_print(&(lideal->norm), 10);
    printf("\n");
    printf("denominator: ");
    ibz_print(&(lideal->lattice.denom), 10);
    printf("\n");
    printf("basis: ");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_print(&((lideal->lattice.basis)[i][j]), 10);
            printf(" ");
        }
        if (i != 3) {
            printf("\n       ");
        } else {
            printf("\n");
        }
    }
    if ((lideal->parent_order) != NULL) {
        printf("parent order denominator: ");
        ibz_print(&(lideal->parent_order->denom), 10);
        printf("\n");
        printf("parent order basis: ");
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                ibz_print(&((lideal->parent_order->basis)[i][j]), 10);
                printf(" ");
            }
            printf("\n                    ");
        }
    } else {
        printf("Parent order not given!\n");
    }
    printf("\n");
}
