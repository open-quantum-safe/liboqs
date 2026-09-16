// SPDX-License-Identifier: Apache-2.0

#include <arithmetic_common.h>
#include <mem.h>
#include <arm_neon.h>
#include <stdint.h>


#define MAYO_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MAYO_MIN(x, y) (((x) < (y)) ? (x) : (y))

// constant-time GF(16) inverse a^14, via a data-oblivious table lookup (no blocker)
static inline unsigned char inverse_f_ct(unsigned char a) {
    static const unsigned char tab[16] = {0, 1, 9, 14, 13, 11, 7, 6,
                                          15, 2, 12, 5, 10, 4, 3, 8};
    uint8x16_t idx = vdupq_n_u8((unsigned char)(a & 0xf));
    return (unsigned char)(vgetq_lane_u8(vqtbl1q_u8(vld1q_u8(tab), idx), 0) & 0xf);
}

// per-row masks row==pivot_row (eq) and row>pivot_row (gt), via data-oblivious
// vector compares; indexed by the public row in place of the per-row ct_ calls
static inline void ef_row_masks(int pivot_row, int nrows, int *eq, int *gt) {
    const int32x4_t pr = vdupq_n_s32(pivot_row);
    const int32x4_t base = {0, 1, 2, 3};
    for (int r = 0; r < nrows; r += 4) {
        int32x4_t rows = vaddq_s32(base, vdupq_n_s32(r));
        vst1q_s32(eq + r, vreinterpretq_s32_u32(vceqq_s32(rows, pr)));
        vst1q_s32(gt + r, vreinterpretq_s32_u32(vcgtq_s32(rows, pr)));
    }
}

/* put matrix in row echelon form with ones on first nonzero entries in constant time*/
static inline void EF(unsigned char *A, int _nrows, int _ncols) {

    (void) _nrows;
    (void) _ncols;

    #define nrows M_MAX
    #define ncols (K_MAX * O_MAX + 1)

    #define NEON_REGS_PER_ROW ((K_MAX * O_MAX + 1 + 15) / 16)
    #define MAX_COLS (NEON_REGS_PER_ROW * 16)

    uint8x16_t _pivot_row[NEON_REGS_PER_ROW];
    uint8x16_t A_neon[NEON_REGS_PER_ROW* M_MAX];

    unsigned char* A_bytes = (unsigned char*) A_neon;

    // load A in the tail of NEON registers
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++)
        {
            A_bytes[i*MAX_COLS + (MAX_COLS - ncols) + j] = A[ i*ncols + j ];
        }
    }

    // pivot row is secret, pivot col is not
    unsigned char inverse;
    uint32_t ef_sel[M_MAX];      // secret: which of those rows is selected
    int pivot_row = 0;
    int pivot_col = MAYO_MAX(MAX_COLS - ncols,0);
    for (; pivot_col < MAX_COLS; pivot_col++) {
        #include "echelon_form_loop.h"
    }

    // write the matrix A back
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            A[i * ncols + j] = A_bytes[i*NEON_REGS_PER_ROW*16 + (MAX_COLS - ncols) + j];
        }
    }
    mayo_secure_clear(_pivot_row, NEON_REGS_PER_ROW * 16);
    mayo_secure_clear(ef_sel, sizeof(ef_sel));
    mayo_secure_clear(A_neon, NEON_REGS_PER_ROW * 16 * nrows);
}


static const unsigned char ef_lt_mask[32] __attribute__((aligned(16))) = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* Ar = A * r on the byte-per-nibble matrix (the caller clears the last column
 * of A). Vector GF(16) multiply, avoiding the scalar mul_f blocker. */
static inline void EF_mat_vec_mul(const unsigned char *A, const unsigned char *r, unsigned char *Ar) {

    #define MV_COLS (K_MAX * O_MAX + 1)
    #define MV_REGS ((MV_COLS + 15) / 16)

    const uint8x16_t tab_reduce = vld1q_u8(__gf16_reduce);

    // r, zero-padded to full registers (the zero padding also nulls any
    // product with the bytes that row loads read past the row end)
    unsigned char rpad[MV_REGS * 16];
    for (int i = 0; i < MV_COLS; i++) {
        rpad[i] = r[i];
    }
    for (int i = MV_COLS; i < MV_REGS * 16; i++) {
        rpad[i] = 0;
    }
    uint8x16_t rv[MV_REGS];
    for (int j = 0; j < MV_REGS; j++) {
        rv[j] = vld1q_u8(rpad + 16 * j);
    }

    for (int i = 0; i < M_MAX; i++) {
        uint8x16_t acc = vdupq_n_u8(0);
        for (int j = 0; j < MV_REGS; j++) {
            uint8x16_t av = vld1q_u8(A + i * MV_COLS + 16 * j);
            acc ^= _gf16v_mul_unpack(av, rv[j], tab_reduce);
        }
        // horizontal xor of all bytes
        uint8x8_t h = veor_u8(vget_low_u8(acc), vget_high_u8(acc));
        h = veor_u8(h, vext_u8(h, h, 4));
        h = veor_u8(h, vext_u8(h, h, 2));
        h = veor_u8(h, vext_u8(h, h, 1));
        Ar[i] = (unsigned char) (vget_lane_u8(h, 0) & 0xf);
    }

    mayo_secure_clear(rpad, sizeof(rpad));

    #undef MV_COLS
    #undef MV_REGS
}

/* constant-time back substitution on [A | y] (one nibble per byte) into x.
 * Like the AVX2 version: read-only coefficient columns transposed once, 16
 * rows eliminated per vqtbl1q; the candidate column may be y itself, read live. */
// transpose a 16x16 byte block: dst[j*ds + i] = src[i*ss + j]
static inline void ef_transpose_16x16(const unsigned char *src, int ss,
                                      unsigned char *dst, int ds) {
    uint8x16_t r[16], t[16];
    for (int i = 0; i < 16; i++) r[i] = vld1q_u8(src + (size_t) i * ss);
    for (int i = 0; i < 8; i++) {
        t[2*i]     = vzip1q_u8(r[2*i], r[2*i+1]);
        t[2*i + 1] = vzip2q_u8(r[2*i], r[2*i+1]);
    }
    for (int g = 0; g < 4; g++) {
        r[4*g+0] = vreinterpretq_u8_u16(vzip1q_u16(vreinterpretq_u16_u8(t[4*g+0]), vreinterpretq_u16_u8(t[4*g+2])));
        r[4*g+1] = vreinterpretq_u8_u16(vzip2q_u16(vreinterpretq_u16_u8(t[4*g+0]), vreinterpretq_u16_u8(t[4*g+2])));
        r[4*g+2] = vreinterpretq_u8_u16(vzip1q_u16(vreinterpretq_u16_u8(t[4*g+1]), vreinterpretq_u16_u8(t[4*g+3])));
        r[4*g+3] = vreinterpretq_u8_u16(vzip2q_u16(vreinterpretq_u16_u8(t[4*g+1]), vreinterpretq_u16_u8(t[4*g+3])));
    }
    for (int h = 0; h < 2; h++) for (int j = 0; j < 4; j++) {
        t[8*h+2*j]   = vreinterpretq_u8_u32(vzip1q_u32(vreinterpretq_u32_u8(r[8*h+j]), vreinterpretq_u32_u8(r[8*h+j+4])));
        t[8*h+2*j+1] = vreinterpretq_u8_u32(vzip2q_u32(vreinterpretq_u32_u8(r[8*h+j]), vreinterpretq_u32_u8(r[8*h+j+4])));
    }
    for (int j = 0; j < 8; j++) {
        r[2*j]   = vreinterpretq_u8_u64(vzip1q_u64(vreinterpretq_u64_u8(t[j]), vreinterpretq_u64_u8(t[j+8])));
        r[2*j+1] = vreinterpretq_u8_u64(vzip2q_u64(vreinterpretq_u64_u8(t[j]), vreinterpretq_u64_u8(t[j+8])));
    }
    for (int i = 0; i < 16; i++) vst1q_u8(dst + (size_t) i * ds, r[i]);
}

static inline void EF_backsub(const unsigned char *A, unsigned char *x) {

    #define BS_ACOLS (K_MAX * O_MAX + 1)
    #define BS_KO (K_MAX * O_MAX)
    #define BS_CHUNKS ((M_MAX + 15) / 16)
    #define BS_STRIDE (BS_CHUNKS * 16)

    unsigned char y[BS_STRIDE];
    unsigned char T[BS_KO * BS_STRIDE];

    for (int i = 0; i < M_MAX; i++) {
        y[i] = A[i * BS_ACOLS + BS_KO];
    }
    for (int i = M_MAX; i < BS_STRIDE; i++) {
        y[i] = 0;
    }
    /* T = A^t over the first BS_KO columns, in blocks: the strided scalar copy this
       replaces dominated back substitution. Bounds are public. */
    for (int col = 0; col < BS_KO; col++) {
        for (int i = M_MAX; i < BS_STRIDE; i++) {
            T[col * BS_STRIDE + i] = 0;
        }
    }
    {
        /* the last block of each axis is anchored at the end, overlapping its
           predecessor, so no scalar tail is left; the overlap is rewritten with the
           same values. Both axes are >= 16 in every parameter set. */
        for (int bc = 0; bc < (BS_KO + 15) / 16; bc++) {
            const int col = MAYO_MIN(bc * 16, BS_KO - 16);
            for (int br = 0; br < (M_MAX + 15) / 16; br++) {
                const int i = MAYO_MIN(br * 16, M_MAX - 16);
                ef_transpose_16x16(A + (size_t) i * BS_ACOLS + col, BS_ACOLS,
                                   T + (size_t) col * BS_STRIDE + i, BS_STRIDE);
            }
        }
    }

    for (int row = M_MAX - 1; row >= 0; row--) {
        unsigned char finished = 0;
        int col_upper_bound = MAYO_MIN(row + (32 / (M_MAX - row)), BS_KO);
        // the first nonzero entry in row r is between r and col_upper_bound with probability at least ~1-q^{-32}

        for (int col = row; col <= col_upper_bound; col++) {
            // entry of the (echelonized) row at this column; live y for the last column
            unsigned char elt = (col < BS_KO) ? A[row * BS_ACOLS + col] : y[row];
            unsigned char correct_column = ct_compare_8(elt, 0) & ~finished;
            unsigned char u = correct_column & y[row];
            x[col] ^= u;

            uint8x16_t tab = gf16v_get_multab(u);
            const unsigned char *src = (col < BS_KO) ? (T + col * BS_STRIDE) : y;

            // y[i] ^= u * column[i] for i < row (public bound)
            int full = row / 16;
            int rem = row % 16;
            for (int c = 0; c < full; c++) {
                uint8x16_t yv = vld1q_u8(y + 16 * c);
                uint8x16_t cv = vld1q_u8(src + 16 * c);
                vst1q_u8(y + 16 * c, yv ^ vqtbl1q_u8(tab, cv));
            }
            if (rem) {
                uint8x16_t mask = vld1q_u8(ef_lt_mask + 16 - rem);
                uint8x16_t yv = vld1q_u8(y + 16 * full);
                uint8x16_t cv = vld1q_u8(src + 16 * full);
                vst1q_u8(y + 16 * full, yv ^ (vqtbl1q_u8(tab, cv) & mask));
            }

            finished = finished | correct_column;
        }
    }

    mayo_secure_clear(y, sizeof(y));
    mayo_secure_clear(T, sizeof(T));

    #undef BS_ACOLS
    #undef BS_KO
    #undef BS_CHUNKS
    #undef BS_STRIDE
}
