// SPDX-License-Identifier: Apache-2.0

/* Constant of the scheme and their selectors:
   the reduction polynomials f, the whipping schedules, the four parameter sets,
   and the PARAM_*() accessors that resolve to either a compile-time constant or 
   a field of mayo_params_t depending on ENABLE_PARAMS_DYNAMIC. mayo.h includes
   this and declares the API. */

#ifndef MAYO_CONSTANTS_H
#define MAYO_CONSTANTS_H

#include <stdint.h>
#include <string.h>

#define F_TAIL_LEN 4
#define F_TAIL_64                                                              \
  { 8, 0, 2, 8 } // f(z) =  z^64 + x^3*z^3 + x*z^2 + x^3
#define F_TAIL_80                                                              \
  { 2, 0, 4, 2 } // f(z) =  z^80 + x*z^3 + x^2*z^2 + x
#define F_TAIL_108                                                             \
  { 8, 0, 1, 7 } // f(z) =  z^108 + (x^2 + x + 1)*z^3 + z^2 + x^3
#define F_TAIL_142                                                             \
  { 4, 0, 8, 1 } // f(z) =  z^142 + z^3 + x^3*z^2 + x^2

// Whipping schedule. The schedule for k is a symmetric k x k matrix whose
// entries are the k*(k+1)/2 labels 0 .. k*(k+1)/2 - 1, each appearing once as
// an unordered pair {i,j}: label l is the exponent of z used for the E_{i,j}
// whose position is given by the schedule table. A table holds one
// (row, column) pair per label, so it has k*(k+1) entries, and
// table[2*l] >= table[2*l+1]. The matrices themselves are reproduced in the
// comments for clarity only.
//
// The schedules are MDS: every square submatrix of Z has full rank in R, which
// denies the claw-finding attacks the low-rank submatrices they would otherwise
// use. Column 0 stays k distinct monomials, which is what the linear term's
// collision bound needs.

// new whipping schedule for k = 5: labels of the E_{i,j}, symmetric in i,j
//   11 13  3  4  6
//   13  0 14 10  2
//    3 14  8  1  5
//    4 10  1 12  9
//    6  2  5  9  7
// WHIP_POS_5[2*l], WHIP_POS_5[2*l+1] is the (row, column) of label l
#define WHIP_POS_5                                                             \
  {                                                                            \
    1,1, 3,2, 4,1, 2,0, 3,0, 4,2, 4,0, 4,4,                                    \
    2,2, 4,3, 3,1, 0,0, 3,3, 1,0, 2,1                                          \
  }

// new whipping schedule for k = 10: labels of the E_{i,j}, symmetric in i,j
//   10 25  7 44 40 24 12 32  5 27
//   25  6 45 50 19  0 11 14 37 52
//    7 45 51 16 42 30 53  8 21  4
//   44 50 16 15 39 26 48 54 23 34
//   40 19 42 39 22 33  1 35 41 18
//   24  0 30 26 33 13 46 17  2 47
//   12 11 53 48  1 46 36 20 49 43
//   32 14  8 54 35 17 20 38 29  9
//    5 37 21 23 41  2 49 29  3 31
//   27 52  4 34 18 47 43  9 31 28
// WHIP_POS_10[2*l], WHIP_POS_10[2*l+1] is the (row, column) of label l
#define WHIP_POS_10                                                            \
  {                                                                            \
    5,1, 6,4, 8,5, 8,8, 9,2, 8,0, 1,1, 2,0,                                    \
    7,2, 9,7, 0,0, 6,1, 6,0, 5,5, 7,1, 3,3,                                    \
    3,2, 7,5, 9,4, 4,1, 7,6, 8,2, 4,4, 8,3,                                    \
    5,0, 1,0, 5,3, 9,0, 9,9, 8,7, 5,2, 9,8,                                    \
    7,0, 5,4, 9,3, 7,4, 6,6, 8,1, 7,7, 4,3,                                    \
    4,0, 8,4, 4,2, 9,6, 3,0, 2,1, 6,5, 9,5,                                    \
    6,3, 8,6, 3,1, 2,2, 9,1, 6,2, 7,3                                          \
  }

// new whipping schedule for k = 11: labels of the E_{i,j}, symmetric in i,j
//   35 13 39 32  4  7 40 14 43 62 64
//   13 55 42 11 63 19 58 49 36 12 20
//   39 42  6 15 57 47 16 34 44 46 50
//   32 11 15 18  5 21 28 10 51 24 31
//    4 63 57  5 38  0 56 29 54 60 23
//    7 19 47 21  0 22 27 61  3 48  8
//   40 58 16 28 56 27 37 45 30  1 17
//   14 49 34 10 29 61 45 26 52 59 41
//   43 36 44 51 54  3 30 52 25  9  2
//   62 12 46 24 60 48  1 59  9 53 65
//   64 20 50 31 23  8 17 41  2 65 33
// WHIP_POS_11[2*l], WHIP_POS_11[2*l+1] is the (row, column) of label l
#define WHIP_POS_11                                                            \
  {                                                                            \
    5,4, 9,6, 10,8, 8,5, 4,0, 4,3, 2,2, 5,0,                                   \
    10,5, 9,8, 7,3, 3,1, 9,1, 1,0, 7,0, 3,2,                                   \
    6,2, 10,6, 3,3, 5,1, 10,1, 5,3, 5,5, 10,4,                                 \
    9,3, 8,8, 7,7, 6,5, 6,3, 7,4, 8,6, 10,3,                                   \
    3,0, 10,10, 7,2, 0,0, 8,1, 6,6, 4,4, 2,0,                                  \
    6,0, 10,7, 2,1, 8,0, 8,2, 7,6, 9,2, 5,2,                                   \
    9,5, 7,1, 10,2, 8,3, 8,7, 9,9, 8,4, 1,1,                                   \
    6,4, 4,2, 6,1, 9,7, 9,4, 7,5, 9,0, 4,1,                                    \
    10,0, 10,9                                                                 \
  }

// new whipping schedule for k = 12: labels of the E_{i,j}, symmetric in i,j
//   62 30 52 53 24  0 25 12 71 20 57 32
//   30  7 37 55 43 70 31 47 35  9 69 15
//   52 37 64 26 76 45 54 10 40 34 11 19
//   53 55 26 61 16 60  5 18 42 17  8 75
//   24 43 76 16 41 27  6  2  4 51 68 38
//    0 70 45 60 27 63 22 49 23  1 21 48
//   25 31 54  5  6 22 14 29 44 65 77 72
//   12 47 10 18  2 49 29 58 67 59 39 56
//   71 35 40 42  4 23 44 67 33 73 36 74
//   20  9 34 17 51  1 65 59 73 28 46  3
//   57 69 11  8 68 21 77 39 36 46 50 13
//   32 15 19 75 38 48 72 56 74  3 13 66
// WHIP_POS_12[2*l], WHIP_POS_12[2*l+1] is the (row, column) of label l
#define WHIP_POS_12                                                            \
  {                                                                            \
    5,0, 9,5, 7,4, 11,9, 8,4, 6,3, 6,4, 1,1,                                   \
    10,3, 9,1, 7,2, 10,2, 7,0, 11,10, 6,6, 11,1,                               \
    4,3, 9,3, 7,3, 11,2, 9,0, 10,5, 6,5, 8,5,                                  \
    4,0, 6,0, 3,2, 5,4, 9,9, 7,6, 1,0, 6,1,                                    \
    11,0, 8,8, 9,2, 8,1, 10,8, 2,1, 11,4, 10,7,                                \
    8,2, 4,4, 8,3, 4,1, 8,6, 5,2, 10,9, 7,1,                                   \
    11,5, 7,5, 10,10, 9,4, 2,0, 3,0, 6,2, 3,1,                                 \
    11,7, 10,0, 7,7, 9,7, 5,3, 3,3, 0,0, 5,5,                                  \
    2,2, 9,6, 11,11, 8,7, 10,4, 10,1, 5,1, 8,0,                                \
    11,6, 9,8, 11,8, 11,3, 4,2, 10,6                                           \
  }

#define MAYO_1_name "MAYO_1"
#define MAYO_1_n 88
#define MAYO_1_m 80
#define MAYO_1_m_vec_limbs 5
#define MAYO_1_o 8
#define MAYO_1_v 80
#define MAYO_1_A_cols (MAYO_1_k * MAYO_1_o + 1)
#define MAYO_1_k 10
#define MAYO_1_q 16
#define MAYO_1_m_bytes 40
#define MAYO_1_O_bytes 320
#define MAYO_1_v_bytes 40
#define MAYO_1_r_bytes 40
#define MAYO_1_P1_bytes 129600
#define MAYO_1_P2_bytes 25600
#define MAYO_1_P3_bytes 1440
#define MAYO_1_csk_bytes 24
#define MAYO_1_cpk_bytes 1456
#define MAYO_1_sig_bytes 464
#define MAYO_1_f_tail F_TAIL_80
#define MAYO_1_f_tail_arr f_tail_80
#define MAYO_1_whip_pos WHIP_POS_10
#define MAYO_1_whip_pos_arr whip_pos_10
#define MAYO_1_salt_bytes 24
#define MAYO_1_digest_bytes 32
#define MAYO_1_pk_seed_bytes 16
#define MAYO_1_sk_seed_bytes 24

#define MAYO_2_name "MAYO_2"
#define MAYO_2_n 86
#define MAYO_2_m 64
#define MAYO_2_m_vec_limbs 4
#define MAYO_2_o 13
#define MAYO_2_v 73
#define MAYO_2_A_cols (MAYO_2_k * MAYO_2_o + 1)
#define MAYO_2_k 5
#define MAYO_2_q 16
#define MAYO_2_m_bytes 32
#define MAYO_2_O_bytes 475
#define MAYO_2_v_bytes 37
#define MAYO_2_r_bytes 33
#define MAYO_2_P1_bytes 86432
#define MAYO_2_P2_bytes 30368
#define MAYO_2_P3_bytes 2912
#define MAYO_2_csk_bytes 24
#define MAYO_2_cpk_bytes 2928
#define MAYO_2_sig_bytes 239
#define MAYO_2_f_tail F_TAIL_64
#define MAYO_2_f_tail_arr f_tail_64
#define MAYO_2_whip_pos WHIP_POS_5
#define MAYO_2_whip_pos_arr whip_pos_5
#define MAYO_2_salt_bytes 24
#define MAYO_2_digest_bytes 32
#define MAYO_2_pk_seed_bytes 16
#define MAYO_2_sk_seed_bytes 24

#define MAYO_3_name "MAYO_3"
#define MAYO_3_n 118
#define MAYO_3_m 108
#define MAYO_3_m_vec_limbs 7
#define MAYO_3_o 10
#define MAYO_3_v (MAYO_3_n - MAYO_3_o)
#define MAYO_3_A_cols (MAYO_3_k * MAYO_3_o + 1)
#define MAYO_3_k 11
#define MAYO_3_q 16
#define MAYO_3_m_bytes 54
#define MAYO_3_O_bytes 540
#define MAYO_3_v_bytes 54
#define MAYO_3_r_bytes 55
#define MAYO_3_P1_bytes 317844
#define MAYO_3_P2_bytes 58320
#define MAYO_3_P3_bytes 2970
#define MAYO_3_csk_bytes 32
#define MAYO_3_cpk_bytes 2986
#define MAYO_3_sig_bytes 681
#define MAYO_3_f_tail F_TAIL_108
#define MAYO_3_f_tail_arr f_tail_108
#define MAYO_3_whip_pos WHIP_POS_11
#define MAYO_3_whip_pos_arr whip_pos_11
#define MAYO_3_salt_bytes 32
#define MAYO_3_digest_bytes 48
#define MAYO_3_pk_seed_bytes 16
#define MAYO_3_sk_seed_bytes 32

#define MAYO_5_name "MAYO_5"
#define MAYO_5_n 154
#define MAYO_5_m 142
#define MAYO_5_m_vec_limbs 9
#define MAYO_5_o 12
#define MAYO_5_v (MAYO_5_n - MAYO_5_o)
#define MAYO_5_A_cols (MAYO_5_k * MAYO_5_o + 1)
#define MAYO_5_k 12
#define MAYO_5_q 16
#define MAYO_5_m_bytes 71
#define MAYO_5_O_bytes 852
#define MAYO_5_v_bytes 71
#define MAYO_5_r_bytes 72
#define MAYO_5_P1_bytes 720863
#define MAYO_5_P2_bytes 120984
#define MAYO_5_P3_bytes 5538
#define MAYO_5_csk_bytes 40
#define MAYO_5_cpk_bytes 5554
#define MAYO_5_sig_bytes 964
#define MAYO_5_f_tail F_TAIL_142
#define MAYO_5_f_tail_arr f_tail_142
#define MAYO_5_whip_pos WHIP_POS_12
#define MAYO_5_whip_pos_arr whip_pos_12
#define MAYO_5_salt_bytes 40
#define MAYO_5_digest_bytes 64
#define MAYO_5_pk_seed_bytes 16
#define MAYO_5_sk_seed_bytes 40

#define PARAM_JOIN2_(a, b) a##_##b
#define PARAM_JOIN2(a, b) PARAM_JOIN2_(a, b)
#define PARAM_NAME(end) PARAM_JOIN2(MAYO_VARIANT, end)

#if defined(MAYO_VARIANT)
#define PARAM_JOIN3_(a, b, c) pqmayo_##a##_##b##_##c
#define PARAM_JOIN3(a, b, c) PARAM_JOIN3_(a, b, c)
#define PARAM_NAME3(end, s) PARAM_JOIN3(MAYO_VARIANT, end, s)

#if defined(MAYO_BUILD_TYPE_REF)
#define MAYO_NAMESPACE(s) PARAM_NAME3(ref, s)
#elif defined(MAYO_BUILD_TYPE_OPT)
#define MAYO_NAMESPACE(s) PARAM_NAME3(opt, s)
#elif defined(MAYO_BUILD_TYPE_AVX2)
#define MAYO_NAMESPACE(s) PARAM_NAME3(avx2, s)
#elif defined(MAYO_BUILD_TYPE_GFNI)
#define MAYO_NAMESPACE(s) PARAM_NAME3(gfni, s)
#elif defined(MAYO_BUILD_TYPE_NEON)
#define MAYO_NAMESPACE(s) PARAM_NAME3(neon, s)
#else
#error "Build type not known"
#endif

#else
#define MAYO_NAMESPACE(s) s
#endif

#ifdef ENABLE_PARAMS_DYNAMIC
#define N_MAX 154
#define M_MAX 142
#define O_MAX 13
#define K_MAX 12
#define Q_MAX 16
#define PK_SEED_BYTES_MAX 16
#define SK_SEED_BYTES_MAX 40
#define SALT_BYTES_MAX 40
#define DIGEST_BYTES_MAX 64
#define V_MAX 142
#define O_BYTES_MAX 852
#define V_BYTES_MAX 71
#define R_BYTES_MAX 72
#define P1_BYTES_MAX 720863
#define P2_BYTES_MAX 120984
#define P3_BYTES_MAX 5538
#define SIG_BYTES_MAX 964
#define CPK_BYTES_MAX 5554
#define CSK_BYTES_MAX 40
#define M_BYTES_MAX 71
#define M_VEC_LIMBS_MAX 9
#elif defined(MAYO_VARIANT)
#define M_MAX PARAM_NAME(m)
#define M_VEC_LIMBS_MAX PARAM_NAME(m_vec_limbs)
#define N_MAX PARAM_NAME(n)
#define O_MAX PARAM_NAME(o)
#define V_MAX PARAM_NAME(v)
#define K_MAX PARAM_NAME(k)
#define Q_MAX PARAM_NAME(q)
#define M_BYTES_MAX PARAM_NAME(m_bytes)
#define O_BYTES_MAX PARAM_NAME(O_bytes)
#define V_BYTES_MAX PARAM_NAME(v_bytes)
#define R_BYTES_MAX PARAM_NAME(r_bytes)
#define P1_BYTES_MAX PARAM_NAME(P1_bytes)
#define P2_BYTES_MAX PARAM_NAME(P2_bytes)
#define P3_BYTES_MAX PARAM_NAME(P3_bytes)
#define SIG_BYTES_MAX PARAM_NAME(sig_bytes)
#define CSK_BYTES_MAX PARAM_NAME(csk_bytes)
#define CPK_BYTES_MAX PARAM_NAME(cpk_bytes)
#define SALT_BYTES_MAX PARAM_NAME(salt_bytes)
#define DIGEST_BYTES_MAX PARAM_NAME(digest_bytes)
#define PK_SEED_BYTES_MAX PARAM_NAME(pk_seed_bytes)
#define SK_SEED_BYTES_MAX SALT_BYTES_MAX
#else
#error "Parameter not specified"
#endif

// a 512-bit load reads 8 limbs whatever M_VEC_LIMBS_MAX is, so every buffer the vector
// backends load m-vecs from carries this much slack past its last m-vec
#define MAYO_MVEC_SLACK 8
// zero a buffer's slack, so the wider load never acts on uninitialised memory
#define MAYO_ZERO_SLACK(buf, n) memset((uint64_t *)(buf) + (n), 0, MAYO_MVEC_SLACK * sizeof(uint64_t))

#define P1_LIMBS_MAX (V_MAX*(V_MAX+1)/2*M_VEC_LIMBS_MAX)
#define P2_LIMBS_MAX (V_MAX*O_MAX*M_VEC_LIMBS_MAX)
#define P3_LIMBS_MAX (O_MAX*(O_MAX+1)/2*M_VEC_LIMBS_MAX)

#ifdef ENABLE_PARAMS_DYNAMIC
#define PARAM_name(p) (p->name)
#define PARAM_m(p) (p->m)
#define PARAM_m_vec_limbs(p) (p->m_vec_limbs)
#define PARAM_n(p) (p->n)
#define PARAM_o(p) (p->o)
#define PARAM_v(p) (p->n - p->o)
#define PARAM_A_cols(p) (p->k * p->o + 1)
#define PARAM_k(p) (p->k)
#define PARAM_q(p) (p->q)
#define PARAM_m_bytes(p) (p->m_bytes)
#define PARAM_O_bytes(p) (p->O_bytes)
#define PARAM_v_bytes(p) (p->v_bytes)
#define PARAM_r_bytes(p) (p->r_bytes)
#define PARAM_P1_bytes(p) (p->P1_bytes)
#define PARAM_P2_bytes(p) (p->P2_bytes)
#define PARAM_P3_bytes(p) (p->P3_bytes)
#define PARAM_csk_bytes(p) (p->csk_bytes)
#define PARAM_cpk_bytes(p) (p->cpk_bytes)
#define PARAM_sig_bytes(p) (p->sig_bytes)
#define PARAM_f_tail(p) (p->f_tail)
#define PARAM_salt_bytes(p) (p->salt_bytes)
#define PARAM_sk_seed_bytes(p) (p->sk_seed_bytes)
#define PARAM_digest_bytes(p) (p->digest_bytes)
#define PARAM_pk_seed_bytes(p) (p->pk_seed_bytes)
#define PARAM_whip_pos(p) (p->whip_pos)
#elif defined(MAYO_VARIANT)
#define PARAM_name(p) PARAM_NAME(name)
#define PARAM_m(p) PARAM_NAME(m)
#define PARAM_m_vec_limbs(p) PARAM_NAME(m_vec_limbs)
#define PARAM_n(p) PARAM_NAME(n)
#define PARAM_o(p) PARAM_NAME(o)
#define PARAM_v(p) PARAM_NAME(v)
#define PARAM_A_cols(p) PARAM_NAME(A_cols)
#define PARAM_k(p) PARAM_NAME(k)
#define PARAM_q(p) PARAM_NAME(q)
#define PARAM_m_bytes(p) PARAM_NAME(m_bytes)
#define PARAM_O_bytes(p) PARAM_NAME(O_bytes)
#define PARAM_v_bytes(p) PARAM_NAME(v_bytes)
#define PARAM_r_bytes(p) PARAM_NAME(r_bytes)
#define PARAM_P1_bytes(p) PARAM_NAME(P1_bytes)
#define PARAM_P2_bytes(p) PARAM_NAME(P2_bytes)
#define PARAM_P3_bytes(p) PARAM_NAME(P3_bytes)
#define PARAM_csk_bytes(p) PARAM_NAME(csk_bytes)
#define PARAM_cpk_bytes(p) PARAM_NAME(cpk_bytes)
#define PARAM_sig_bytes(p) PARAM_NAME(sig_bytes)
static const unsigned char f_tail[] = PARAM_NAME(f_tail);
static const unsigned char whip_pos[] = PARAM_NAME(whip_pos);
#define PARAM_salt_bytes(p) PARAM_NAME(salt_bytes)
#define PARAM_sk_seed_bytes(p) PARAM_NAME(sk_seed_bytes)
#define PARAM_digest_bytes(p) PARAM_NAME(digest_bytes)
#define PARAM_pk_seed_bytes(p) PARAM_NAME(pk_seed_bytes)
#define PARAM_f_tail(p) f_tail
#define PARAM_whip_pos(p) whip_pos
#else
#error "Parameter not specified"
#endif

#define PARAM_P1_limbs(p) (PARAM_v(p)*(PARAM_v(p)+1)/2*PARAM_m_vec_limbs(p))
#define PARAM_P2_limbs(p) (PARAM_v(p)*PARAM_o(p)*PARAM_m_vec_limbs(p))
#define PARAM_P3_limbs(p) (PARAM_o(p)*(PARAM_o(p)+1)/2*PARAM_m_vec_limbs(p))
#define PARAM_EPK_limbs(p) (PARAM_P1_limbs(p) + PARAM_P2_limbs(p) + PARAM_P3_limbs(p))

#endif

