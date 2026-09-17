#include <quaternion.h>
#include <internal.h>
#include <lll.h>

// Reduced basis of an O0-ideal, via constant-time rank-2 Z[i] reduction
// Lagrange-reduced Z[i]-basis and Minkowski-reduced Z-basis v1, v2
// Output columns are v1, i*v1, v2, i*v2 achieving the minima of the rank-4 Z lattice
// The presentation is the same for any equivalent input ideal up to one global Z[i] unit
void
quat_ideal_reduce_basis(quat_lattice_t *reduced, const quat_ideal_t *ideal)
{
    quat_lattice_t ipt;
    quat_lattice_init(&ipt);
    quat_to_lattice(&ipt, ideal);

    // set bound on Z-basis entries
    int bound = ibz_get_bound(&ideal->norm) + 2;

    // reduce O0 ideal
    quat_lll_reduce_O0_ideal(reduced, &ipt, &QUATALG_PINFTY, bound);
    assert(ibz_cmp(&reduced->denom, &ibz_const_two) == 0);
    ibz_set(&reduced->denom, 2, 3);

    // set output bounds similar to L2 case
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            int b = 1 + (((i < 2) * ibz_bitsize(&QUATALG_PINFTY.p) + ibz_get_bound(&ideal->norm)) / 2);
            assert(ibz_bitsize(&reduced->basis.m[i][j]) <= b);
            ibz_set_bound_ct(&reduced->basis.m[i][j], b);
        }
    for (int j = 0; j < 2; j++) {
        int b = 1 + ((ibz_bitsize(&QUATALG_PINFTY.p) / 2 + ibz_get_bound(&ideal->norm)) / 2) + 1;
        assert(ibz_bitsize(&reduced->basis.m[j][0]) <= b);
        ibz_set_bound_ct(&reduced->basis.m[j][0], b);
    }
    for (int j = 2; j < 4; j++) {
        int b = 1 + ((-ibz_bitsize(&QUATALG_PINFTY.p) / 2 + ibz_get_bound(&ideal->norm))) + 1;
        if (b < 1) {
            // norm below sqrt(p)
            assert(ibz_is_zero(&reduced->basis.m[j][0]));
            b = 1;
        }
        assert(ibz_bitsize(&reduced->basis.m[j][0]) <= b);
        ibz_set_bound_ct(&reduced->basis.m[j][0], b);
    }
}

// Bounding parallelogram (box) for the ball of radius radius in lat
// The radius is relative to the primal gram matrix of lat scaled divided by 2N
int
quat_lattice_bound_parallelogram(ibz_vec_4_t *box, ibz_mat_4x4_t *U, const quat_lattice_t *lat, const ibz_t *radius)
{
    quat_lattice_t reduced;
    ibz_mat_4x4_t Ainv;
    ibz_t gram_diag[4];
    ibz_t num, den, rem;

    quat_lattice_init(&reduced);
    ibz_mat_4x4_init(&Ainv);
    for (int i = 0; i < 4; i++)
        ibz_init(&gram_diag[i]);
    ibz_init(&num);
    ibz_init(&den);
    ibz_init(&rem);

    quat_lll_dual_reduce_ideal(&reduced, gram_diag, &Ainv, lat, &QUATALG_PINFTY);

    // U = Ainv^T, quat_lattice_sample_from_ball maps x = U^T y,
    // and since v = y^T B' = (Ainv y)^T B it needs x = Ainv y
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ibz_copy(&U->m[i][j], &Ainv.m[j][i]);

    ibz_mul(&den, &lat->basis.m[0][0], &QUATALG_PINFTY.p);

    int trivial = 1;
    for (int i = 0; i < 4; i++) {
        ibz_mul(&num, &gram_diag[i], radius);
        ibz_div(&num, &rem, &num, &den);
        ibz_sqrt_floor(&box->v[i], &num);
        trivial &= ibz_is_zero(&box->v[i]);
    }

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ibz_set_bound(&U->m[i][j], ibz_bitsize(&U->m[i][j]) + 1);
    for (int i = 0; i < 4; i++)
        ibz_set_bound(&box->v[i], ibz_bitsize(&box->v[i]) + 1);
    return (!trivial);
}
