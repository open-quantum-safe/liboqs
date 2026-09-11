
// SPDX-License-Identifier: Apache-2.0

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <stdint.h>
#include <mayo.h>
#include <stddef.h>

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#ifndef TARGET_BIG_ENDIAN
#define TARGET_BIG_ENDIAN
#endif
#endif

/* byte j of the little-endian view of a limb (uint64_t) buffer: big endian
   mirrors the index within its 8-byte limb */
#ifdef TARGET_BIG_ENDIAN
#define MAYO_LIMB_BYTE(j) ((j) ^ 7)
#else
#define MAYO_LIMB_BYTE(j) (j)
#endif

#define uint32_t_blocker MAYO_NAMESPACE(uint32_t_blocker)
extern volatile uint32_t uint32_t_blocker;
#define uint64_t_blocker MAYO_NAMESPACE(uint64_t_blocker)
extern volatile uint64_t uint64_t_blocker;
#define unsigned_char_blocker MAYO_NAMESPACE(unsigned_char_blocker)
extern volatile unsigned char unsigned_char_blocker;

#if !(((!defined(__clang__) && defined(__GNUC__) && __GNUC__ <= 12)) && (defined(__x86_64__) || defined(_M_X64)))
// a > b -> b - a is negative
// returns 0xFFFFFFFF if true, 0x00000000 if false
static inline uint64_t ct_64_is_greater_than(int a, int b) {
    int64_t diff = ((int64_t) b) - ((int64_t) a);
    return ((uint64_t) (diff >> (8*sizeof(uint64_t)-1)) ^ uint64_t_blocker);
}

// if a == b -> 0x00000000, else 0xFFFFFFFF
static inline uint32_t ct_compare_32(int a, int b) {
    return ((uint32_t)((-(int32_t)(a ^ b)) >> (8*sizeof(uint32_t)-1)) ^ uint32_t_blocker);
}

// if a == b -> 0x0000000000000000, else 0xFFFFFFFFFFFFFFFF
static inline uint64_t ct_compare_64(int a, int b) {
    return ((uint64_t)((-(int64_t)(a ^ b)) >> (8*sizeof(uint64_t)-1)) ^ uint64_t_blocker);
}

// if a == b -> 0x00, else 0xFF
static inline unsigned char ct_compare_8(unsigned char a, unsigned char b) {
    return ((int8_t)((-(int32_t)(a ^ b)) >> (8*sizeof(uint32_t)-1)) ^ unsigned_char_blocker);
}
#else
// a > b -> b - a is negative
// returns 0xFFFFFFFF if true, 0x00000000 if false
static inline uint64_t ct_64_is_greater_than(int a, int b) {
    int64_t diff = ((int64_t) b) - ((int64_t) a);
    return ((uint64_t) (diff >> (8*sizeof(uint64_t)-1)));
}

// if a == b -> 0x00000000, else 0xFFFFFFFF
static inline uint32_t ct_compare_32(int a, int b) {
    return ((uint32_t)((-(int32_t)(a ^ b)) >> (8*sizeof(uint32_t)-1)));
}

// if a == b -> 0x0000000000000000, else 0xFFFFFFFFFFFFFFFF
static inline uint64_t ct_compare_64(int a, int b) {
    return ((uint64_t)((-(int64_t)(a ^ b)) >> (8*sizeof(uint64_t)-1)));
}

// if a == b -> 0x00, else 0xFF
static inline unsigned char ct_compare_8(unsigned char a, unsigned char b) {
    return ((int8_t)((-(int32_t)(a ^ b)) >> (8*sizeof(uint32_t)-1)));
}
#endif

// GFNI backend, selected by MAYO_BUILD_TYPE=gfni. Needs AVX-512. The products pair
// columns into 2x2 GF(16) blocks, two in and two out per affine (gfni_arithmetic.h).
#if defined(MAYO_GFNI) && !(defined(__GFNI__) && defined(__AVX512F__) && defined(__AVX512BW__) && defined(__AVX512DQ__))
    #error "MAYO_BUILD_TYPE=gfni requires GFNI and AVX-512F/BW (e.g. -march=icelake-server or -mgfni -mavx512f -mavx512bw)"
#endif

/* dst[i] ^= each nibble of src[i] times the constant t16 tabulates, i < n. Four integer
   multiplies per word; the vector backends do it in one instruction. */
static inline void m_gf16_mulc_xor_scalar(uint64_t *dst, const uint64_t *src, int n, const unsigned char *t16) {
    const uint64_t low = 0x1111111111111111ULL;
    const uint64_t f0 = t16[1], f1 = t16[2], f2 = t16[4], f3 = t16[8];
    for (int i = 0; i < n; i++) {
        const uint64_t v = src[i];
        dst[i] ^= ( v       & low) * f0
                ^ ((v >> 1) & low) * f1
                ^ ((v >> 2) & low) * f2
                ^ ((v >> 3) & low) * f3;
    }
}

#if defined(MAYO_GFNI) && !defined(ENABLE_PARAMS_DYNAMIC) && (M_MAX >= 64)
    #include <gfni_arithmetic.h>
#elif defined(MAYO_AVX) || defined(MAYO_NEON)
    #include <shuffle_arithmetic.h>
#elif defined(MAYO_M4)
    #include <m4_arithmetic.h>
#else
    #include <generic_arithmetic.h>
#endif

// only the gfni backend interleaves VKtmp into pairs
#ifndef MAYO_VKTMP_N
#define MAYO_VKTMP_N (V_MAX * K_MAX * M_VEC_LIMBS_MAX + MAYO_MVEC_SLACK)
#endif

// zero an accumulator, unless the backend's first kernel pass stores instead
#ifdef MAYO_ACC_STORES_FIRST
#define MAYO_ACC_INIT(acc) ((void) 0)
#else
#define MAYO_ACC_INIT(acc) memset((acc), 0, sizeof(acc))
#endif

/* lambda into column 0 of P1 * V^t, whose rows are M_VEC_LIMBS_MAX-strided columns
   here; the gfni backend interleaves them into pairs and overrides this. */
#ifndef MAYO_HAVE_VKT_ADD_LAMBDA
static inline void m_vkt_add_lambda(uint64_t *VKtmp, const uint64_t *lambda, int v) {
    for (int r = 0; r < v; r++) {
        uint64_t *dst = VKtmp + (size_t) r * K_MAX * M_VEC_LIMBS_MAX;
        for (int l = 0; l < M_VEC_LIMBS_MAX; l++) {
            dst[l] ^= lambda[(size_t) r * M_VEC_LIMBS_MAX + l];
        }
    }
}
#endif

// only the gfni backend asks for more than natural alignment on the multabs
#ifndef MAYO_MULTAB_ALIGN
#define MAYO_MULTAB_ALIGN _Alignof(mayo_multab_t)
#endif

#ifndef MAYO_HAVE_GF16_MULC_XOR
#define MAYO_GF16_MULC_NEEDS_TABLE 1
static inline void m_gf16_mulc_xor(uint64_t *dst, const uint64_t *src, int n, unsigned char coeff, const unsigned char *t16) {
    (void) coeff;
    m_gf16_mulc_xor_scalar(dst, src, n, t16);
}
#endif

/* A[(row0+l)*A_width + col + c] ^= M[c*mvl + l] << bits, plus the carry into the next
   row. Reading M per column and writing A per limb is a transpose, which the backends do
   in registers; kept scalar here because clang otherwise synthesises a gather. */
#ifndef MAYO_HAVE_COMPUTE_A_ACC
static inline void m_compute_A_acc(uint64_t *A, size_t A_width, int row0, int col,
                                   const uint64_t *M, int o, int mvl, int bits) {
#if defined(__clang__)
    #pragma clang loop vectorize(disable)
#endif
    for (int c = 0; c < o; c++) {
        for (int l = 0; l < mvl; l++) {
            const uint64_t v = M[(size_t) c * mvl + l];
            A[(size_t) (row0 + l) * A_width + col + c] ^= v << bits;
            if (bits > 0) {
                A[(size_t) (row0 + l + 1) * A_width + col + c] ^= v >> (64 - bits);
            }
        }
    }
}
#endif

static
inline void vec_mul_add_u64(const int legs, const uint64_t *in, unsigned char a, uint64_t *acc) {
    uint32_t tab = mul_table(a);

    uint64_t lsb_ask = 0x1111111111111111ULL;

    for(int i=0; i < legs; i++){
        acc[i] ^= ( in[i]       & lsb_ask) * (tab & 0xff)
                ^ ((in[i] >> 1) & lsb_ask) * ((tab >> 8)  & 0xf)
                ^ ((in[i] >> 2) & lsb_ask) * ((tab >> 16) & 0xf)
                ^ ((in[i] >> 3) & lsb_ask) * ((tab >> 24) & 0xf);
    }
}

// Calculate Upper in KeyGen
#define m_upper MAYO_NAMESPACE(m_upper)
void m_upper(const mayo_params_t* p, const uint64_t *in, uint64_t *out, int size);

// Sample solution in Sign
#define sample_solution MAYO_NAMESPACE(sample_solution)
int sample_solution(const mayo_params_t *p, unsigned char *A, const unsigned char *y, const unsigned char *r, unsigned char *x, int k, int o, int m, int A_cols);

#if defined(__GNUC__) || defined(__clang__)
#define BSWAP32(i) __builtin_bswap32((i))
#define BSWAP64(i) __builtin_bswap64((i))
#else
#define BSWAP32(i) ((((i) >> 24) & 0xff) | (((i) >> 8) & 0xff00) | (((i) & 0xff00) << 8) | ((i) << 24))
#define BSWAP64(i) ((BSWAP32((i) >> 32) & 0xffffffff) | (BSWAP32(i) << 32))
#endif

/* ------------------------------------------------------------------ *
 * sign orchestration                                                  *
 * ------------------------------------------------------------------ *
 * Sign mirrors verify's m_calculate_PS_SPS: one hook per backend family
 * computes everything compute_rhs and compute_A consume, so that
 * sign_signature_internal stays backend-free. The multabs and P1 * V^t
 * stay inside the hook. Backends with multab kernels declare
 * MAYO_HAVE_MULTAB_SIGN to select the multab variant. */

#include <string.h>
#include <stdalign.h>
#include <mem.h>
#include <simple_arithmetic.h>

/* dst += c * src, one m-vector. gfni folds the affine matrix straight from the
   coefficient, so its constant-multiply is one instruction; the table-driven
   backends would first have to build a 16-byte table per constant, which costs
   more than the scalar multiply-accumulate they fall back on here. c is secret,
   so both paths stay branch-free. */
static inline void mvec_mul_add(int mvl, const uint64_t *src, unsigned char c,
                                uint64_t *dst) {
#ifdef MAYO_GF16_MULC_NEEDS_TABLE
    vec_mul_add_u64(mvl, src, c, dst);
#else
    m_gf16_mulc_xor(dst, src, mvl, c, NULL);
#endif
}

/* The oil half of lambda goes straight into block 0 of Mtmp: o m-vector XORs.
   With the O^t lambda_vin insert this completes M's block 0 correction, which
   compute_A weights by E_i0. */
static inline void m_lambda_oil(const mayo_params_t *p, const uint64_t *lambda,
                                uint64_t *Mtmp) {
    (void) p;
    const int mvl = PARAM_m_vec_limbs(p);
    for (int c = 0; c < PARAM_o(p); c++) {
        uint64_t *dst = Mtmp + (size_t) c * mvl;
        const uint64_t *src = lambda + (size_t) (PARAM_v(p) + c) * mvl;
        for (int l = 0; l < mvl; l++) {
            dst[l] ^= src[l];
        }
    }
}

/* O^t lambda_vin into block 0 of Mtmp: o * v constant multiplies of m-vectors,
   through the same primitive the f-tail fold uses. Used where the backend has
   no m_lambda_times_O. */
static inline void m_lambda_o_correction(const mayo_params_t *p, const uint64_t *lambda,
                                         const unsigned char *O, uint64_t *Mtmp) {
    (void) p;
    const int mvl = PARAM_m_vec_limbs(p);
    for (int c = 0; c < PARAM_o(p); c++) {
        uint64_t *dst = Mtmp + (size_t) c * mvl;
        for (int r = 0; r < PARAM_v(p); r++) {
            mvec_mul_add(mvl, lambda + (size_t) r * mvl,
                         O[(size_t) r * PARAM_o(p) + c], dst);
        }
    }
}

#ifdef MAYO_HAVE_MULTAB_SIGN

/* One attempt's M into Mtmp and all v_i P v_j into VPV, with lambda folded in:
   Lambda(v_i) reaches column 0 of VPV through VKtmp, and block 0 of Mtmp picks
   up O^t lambda_vin + lambda_oil. */
static inline void m_calculate_M_and_VPV(const mayo_params_t *p,
        const uint64_t *P1, const uint64_t *P2_or_L, const unsigned char *O,
        const unsigned char *Vdec, const uint64_t *lambda, int use_l,
        uint64_t *Mtmp, uint64_t *VPV) {
    (void) p;
    // natural alignment costs ~1% of MAYO_5 sign on gfni
    alignas(64) uint64_t VKtmp[MAYO_VKTMP_N];
    MAYO_ZERO_SLACK(VKtmp, (size_t) PARAM_v(p) * PARAM_k(p) * PARAM_m_vec_limbs(p));
    mayo_multab_t O_multabs[MAYO_O_MULTABS_N];
    alignas(MAYO_MULTAB_ALIGN) mayo_multab_t V_multabs[MAYO_V_MULTABS_N];

    mayo_V_multabs(Vdec, V_multabs);

    MAYO_ACC_INIT(VKtmp); // P1_times_Vt accumulates into it
    P1_times_Vt(P1, V_multabs, VKtmp);

    /* lambda into column 0 of P1 * V^t. Vt_times_Pv runs on both paths, so
       Lambda(v_i) reaches VPV[i][0] for free either way. With L implicit,
       P1t_times_Vt accumulates on top and Ut_times_O additionally turns
       column 0 into (O^t lambda)[c] in block 0 of Mtmp; with L materialized
       there is no such pass, so that half is inserted explicitly.
       v m-vector XORs, no multiplications. */
    m_vkt_add_lambda(VKtmp, lambda, PARAM_v(p));

    Vt_times_Pv(VKtmp, V_multabs, VPV);

    if (use_l) {
        // L is materialized in the expanded key: Mtmp = V * L directly.
        Vt_times_L(P2_or_L, V_multabs, Mtmp);
#if defined(MAYO_HAVE_LAMBDA_TIMES_O)
        // O multabs feed the O^t lambda_vin insert
        mayo_O_multabs(O, O_multabs);
        m_lambda_times_O(lambda, O_multabs, Mtmp);
#else
        m_lambda_o_correction(p, lambda, O, Mtmp);
#endif
    } else {
        // implicit L: Mtmp = V * ((P1 + P1^t) * O + P2), reusing P1 * V^t.
        // VKtmp = (P1 + P1^t) * V^t; the diagonal cancels in characteristic two.
        mayo_O_multabs(O, O_multabs);
        P1t_times_Vt(P1, V_multabs, VKtmp);
        Ut_times_O(VKtmp, O_multabs, Mtmp);
        Vt_times_L(P2_or_L, V_multabs, Mtmp);
    }

    m_lambda_oil(p, lambda, Mtmp);

    mayo_secure_clear(VKtmp, sizeof(VKtmp));
    mayo_secure_clear(O_multabs, sizeof(O_multabs));
    mayo_secure_clear(V_multabs, sizeof(V_multabs));
}

// assemble the signature vector: s_i = [ v_i + O x_i | x_i ]
static inline void m_compute_s(const mayo_params_t *p, const unsigned char *O,
        const unsigned char *Vdec, const unsigned char *x, unsigned char *s) {
    (void) p;
    const int param_n = PARAM_n(p), param_o = PARAM_o(p);
    unsigned char OT[O_MAX * MAYO_OT_STRIDE]; // O transposed, secret data
    unsigned char Ox[MAYO_OT_STRIDE];         // secret data; padded for vector stores
    for (int c = 0; c < param_o; c++) {
        int rr = 0;
        for (; rr < param_n - param_o; rr++) {
            OT[c * MAYO_OT_STRIDE + rr] = O[rr * param_o + c];
        }
        for (; rr < MAYO_OT_STRIDE; rr++) {
            OT[c * MAYO_OT_STRIDE + rr] = 0;
        }
    }
    for (int i = 0; i < PARAM_k(p); ++i) {
        const unsigned char *vi = Vdec + i * (param_n - param_o);
        mayo_Ot_rows_times_x(OT, x + i * param_o, Ox);
        mat_add(vi, Ox, s + i * param_n, param_n - param_o, 1);
        memcpy(s + i * param_n + (param_n - param_o), x + i * param_o, param_o);
    }
    mayo_secure_clear(OT, sizeof(OT));
    mayo_secure_clear(Ox, sizeof(Ox));
}

#else /* the generic and m4 backends */

static inline void m_calculate_M_and_VPV(const mayo_params_t *p,
        const uint64_t *P1, const uint64_t *P2_or_L, const unsigned char *O,
        const unsigned char *Vdec, const uint64_t *lambda, int use_l,
        uint64_t *Mtmp, uint64_t *VPV) {
    (void) p;
    if (use_l) {
        compute_M_and_VPV(p, Vdec, P2_or_L, P1, Mtmp, VPV);
        /* compute_M_and_VPV keeps P1 * V^t internal, so lambda cannot be folded
           in there: insert Lambda_vin(v_i) into column 0 of VPV explicitly, and
           O^t lambda_vin into block 0 of Mtmp. */
        const int mvl = PARAM_m_vec_limbs(p);
        for (int i = 0; i < PARAM_k(p); i++) {
            uint64_t *dst = VPV + (size_t) i * PARAM_k(p) * mvl;
            for (int r = 0; r < PARAM_v(p); r++) {
                mvec_mul_add(mvl, lambda + (size_t) r * mvl,
                             Vdec[(size_t) i * PARAM_v(p) + r], dst);
            }
        }
        m_lambda_o_correction(p, lambda, O, Mtmp);
    } else {
        // implicit L: M = V * ((P1 + P1^t) * O + P2), reusing P1 * V^t
        compute_M_and_VPV_implicit(p, Vdec, P2_or_L, P1, O, lambda, Mtmp, VPV);
    }
    m_lambda_oil(p, lambda, Mtmp);
}

// assemble the signature vector: s_i = [ v_i + O x_i | x_i ]
static inline void m_compute_s(const mayo_params_t *p, const unsigned char *O,
        const unsigned char *Vdec, const unsigned char *x, unsigned char *s) {
    (void) p;
    const int param_n = PARAM_n(p), param_o = PARAM_o(p);
    unsigned char Ox[V_MAX]; // secret data
    for (int i = 0; i < PARAM_k(p); ++i) {
        const unsigned char *vi = Vdec + i * (param_n - param_o);
        mat_mul(O, x + i * param_o, Ox, param_o, param_n - param_o, 1);
        mat_add(vi, Ox, s + i * param_n, param_n - param_o, 1);
        memcpy(s + i * param_n + (param_n - param_o), x + i * param_o, param_o);
    }
    mayo_secure_clear(Ox, sizeof(Ox));
}

#endif

#endif
