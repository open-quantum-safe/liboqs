#include "isog.h"
#include <assert.h>

// since we use degree 4 isogeny steps, we need to handle the odd case with care
static uint32_t
ec_eval_even_strategy(ec_curve_t *curve,
                      ec_point_t *points,
                      unsigned len_points,
                      const ec_point_t *kernel,
                      const int isog_len)
{
    ec_curve_normalize_A24(curve);
    ec_point_t A24;
    copy_point(&A24, &curve->A24);

    int space = 1;
    for (int i = 1; i < isog_len; i *= 2)
        ++space;

    // Stack of remaining kernel points and their associated orders
    ec_point_t splits[space];
    uint16_t todo[space];
    splits[0] = *kernel;
    todo[0] = isog_len;

    int current = 0; // Pointer to current top of stack

    // Chain of 4-isogenies
    for (int j = 0; j < isog_len / 2; ++j) {
        assert(current >= 0);
        assert(todo[current] >= 1);
        // Get the next point of order 4
        while (todo[current] != 2) {
            assert(todo[current] >= 3);
            // A new split will be added
            ++current;
            assert(current < space);
            // We set the seed of the new split to be computed and saved
            copy_point(&splits[current], &splits[current - 1]);
            // if we copied from the very first element, then we perform one additional doubling
            unsigned num_dbls = todo[current - 1] / 4 * 2 + todo[current - 1] % 2;
            todo[current] = todo[current - 1] - num_dbls;
            while (num_dbls--)
                xDBL_A24(&splits[current], &splits[current], &A24, false);
        }

        if (j == 0) {
            assert(fp2_is_one(&A24.z));
            if (!ec_is_four_torsion(&splits[current], curve))
                return -1;

            ec_point_t T;
            xDBL_A24(&T, &splits[current], &A24, false);
            if (fp2_is_zero(&T.x))
                return -1; // special isogenies not allowed
        } else {
            assert(todo[current] == 2);
#ifndef NDEBUG
            if (fp2_is_zero(&splits[current].z))
                debug_print("splitting point z coordinate is unexpectedly zero");

            ec_point_t test;
            xDBL_A24(&test, &splits[current], &A24, false);
            if (fp2_is_zero(&test.z))
                debug_print("z coordinate is unexpectedly zero before doubling");
            xDBL_A24(&test, &test, &A24, false);
            if (!fp2_is_zero(&test.z))
                debug_print("z coordinate is unexpectedly not zero after doubling");
#endif
        }

        // Evaluate 4-isogeny
        ec_kps4_t kps4;
        xisog_4(&kps4, &A24, splits[current]);
        xeval_4(splits, splits, current, &kps4);
        for (int i = 0; i < current; ++i)
            todo[i] -= 2;
        xeval_4(points, points, len_points, &kps4);

        --current;
    }
    assert(isog_len % 2 ? !current : current == -1);

    // Final 2-isogeny
    if (isog_len % 2) {
#ifndef NDEBUG
        if (fp2_is_zero(&splits[0].z))
            debug_print("splitting point z coordinate is unexpectedly zero");
        ec_point_t test;
        copy_point(&test, &splits[0]);
        xDBL_A24(&test, &test, &A24, false);
        if (!fp2_is_zero(&test.z))
            debug_print("z coordinate is unexpectedly not zero after doubling");
#endif

        // We need to check the order of this point in case there were no 4-isogenies
        if (isog_len == 1 && !ec_is_two_torsion(&splits[0], curve))
            return -1;
        if (fp2_is_zero(&splits[0].x)) {
            // special isogenies not allowed
            // this case can only happen if isog_len == 1; otherwise the
            // previous 4-isogenies we computed ensure that $T=(0:1)$ is put
            // as the kernel of the dual isogeny
            return -1;
        }

        ec_kps2_t kps2;
        xisog_2(&kps2, &A24, splits[0]);
        xeval_2(points, points, len_points, &kps2);
    }

    // Output curve in the form (A:C)
    A24_to_AC(curve, &A24);

    curve->is_A24_computed_and_normalized = false;

    return 0;
}

uint32_t
ec_eval_even(ec_curve_t *image, ec_isog_even_t *phi, ec_point_t *points, unsigned len_points)
{
    copy_curve(image, &phi->curve);
    return ec_eval_even_strategy(image, points, len_points, &phi->kernel, phi->length);
}

// naive implementation
uint32_t
ec_eval_small_chain(ec_curve_t *curve,
                    const ec_point_t *kernel,
                    int len,
                    ec_point_t *points,
                    unsigned len_points,
                    bool special) // do we allow special isogenies?
{

    ec_point_t A24;
    AC_to_A24(&A24, curve);

    ec_kps2_t kps;
    ec_point_t small_K, big_K;
    copy_point(&big_K, kernel);

    for (int i = 0; i < len; i++) {
        copy_point(&small_K, &big_K);
        // small_K = big_K;
        for (int j = 0; j < len - i - 1; j++) {
            xDBL_A24(&small_K, &small_K, &A24, false);
        }
        // Check the order of the point before the first isogeny step
        if (i == 0 && !ec_is_two_torsion(&small_K, curve))
            return (uint32_t)-1;
        // Perform isogeny step
        if (fp2_is_zero(&small_K.x)) {
            if (special) {
                ec_point_t B24;
                xisog_2_singular(&kps, &B24, A24);
                xeval_2_singular(&big_K, &big_K, 1, &kps);
                xeval_2_singular(points, points, len_points, &kps);
                copy_point(&A24, &B24);
            } else {
                return (uint32_t)-1;
            }
        } else {
            xisog_2(&kps, &A24, small_K);
            xeval_2(&big_K, &big_K, 1, &kps);
            xeval_2(points, points, len_points, &kps);
        }
    }
    A24_to_AC(curve, &A24);

    curve->is_A24_computed_and_normalized = false;
    return 0;
}

uint32_t
ec_isomorphism(ec_isom_t *isom, const ec_curve_t *from, const ec_curve_t *to)
{
    fp2_t t0, t1, t2, t3, t4;

    fp2_mul(&t0, &from->A, &from->C);
    fp2_mul(&t1, &to->A, &to->C);

    fp2_mul(&t2, &t1, &to->C); // toA*toC^2
    fp2_add(&t3, &t2, &t2);
    fp2_add(&t3, &t3, &t3);
    fp2_add(&t3, &t3, &t3);
    fp2_add(&t2, &t2, &t3); // 9*toA*toC^2
    fp2_sqr(&t3, &to->A);
    fp2_mul(&t3, &t3, &to->A); // toA^3
    fp2_add(&t3, &t3, &t3);
    fp2_sub(&isom->Nx, &t3, &t2); // 2*toA^3-9*toA*toC^2
    fp2_mul(&t2, &t0, &from->A);  // fromA^2*fromC
    fp2_sqr(&t3, &from->C);
    fp2_mul(&t3, &t3, &from->C); // fromC^3
    fp2_add(&t4, &t3, &t3);
    fp2_add(&t3, &t4, &t3);             // 3*fromC^3
    fp2_sub(&t3, &t3, &t2);             // 3*fromC^3-fromA^2*fromC
    fp2_mul(&isom->Nx, &isom->Nx, &t3); // lambda_x = (2*toA^3-9*toA*toC^2)*(3*fromC^3-fromA^2*fromC)

    fp2_mul(&t2, &t0, &from->C); // fromA*fromC^2
    fp2_add(&t3, &t2, &t2);
    fp2_add(&t3, &t3, &t3);
    fp2_add(&t3, &t3, &t3);
    fp2_add(&t2, &t2, &t3); // 9*fromA*fromC^2
    fp2_sqr(&t3, &from->A);
    fp2_mul(&t3, &t3, &from->A); // fromA^3
    fp2_add(&t3, &t3, &t3);
    fp2_sub(&isom->D, &t3, &t2); // 2*fromA^3-9*fromA*fromC^2
    fp2_mul(&t2, &t1, &to->A);   // toA^2*toC
    fp2_sqr(&t3, &to->C);
    fp2_mul(&t3, &t3, &to->C); // toC^3
    fp2_add(&t4, &t3, &t3);
    fp2_add(&t3, &t4, &t3);           // 3*toC^3
    fp2_sub(&t3, &t3, &t2);           // 3*toC^3-toA^2*toC
    fp2_mul(&isom->D, &isom->D, &t3); // lambda_z = (2*fromA^3-9*fromA*fromC^2)*(3*toC^3-toA^2*toC)

    // Mont -> SW -> SW -> Mont
    fp2_mul(&t0, &to->C, &from->A);
    fp2_mul(&t0, &t0, &isom->Nx); // lambda_x*toC*fromA
    fp2_mul(&t1, &from->C, &to->A);
    fp2_mul(&t1, &t1, &isom->D);  // lambda_z*fromC*toA
    fp2_sub(&isom->Nz, &t0, &t1); // lambda_x*toC*fromA - lambda_z*fromC*toA
    fp2_mul(&t0, &from->C, &to->C);
    fp2_add(&t1, &t0, &t0);
    fp2_add(&t0, &t0, &t1);             // 3*fromC*toC
    fp2_mul(&isom->D, &isom->D, &t0);   // 3*lambda_z*fromC*toC
    fp2_mul(&isom->Nx, &isom->Nx, &t0); // 3*lambda_x*fromC*toC

    return (fp2_is_zero(&isom->Nx) | fp2_is_zero(&isom->D));
}

void
ec_iso_eval(ec_point_t *P, ec_isom_t *isom)
{
    fp2_t tmp;
    fp2_mul(&P->x, &P->x, &isom->Nx);
    fp2_mul(&tmp, &P->z, &isom->Nz);
    fp2_add(&P->x, &P->x, &tmp);
    fp2_mul(&P->z, &P->z, &isom->D);
}
