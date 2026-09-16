// SPDX-License-Identifier: Apache-2.0

#include <immintrin.h>
#include <stdint.h>


#define MAYO_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MAYO_MIN(x, y) (((x) < (y)) ? (x) : (y))


//
// generate multiplication table for '4-bit' variable 'b'. From https://eprint.iacr.org/2023/059/.
//
static inline __m256i tbl32_gf16_multab( uint8_t b ) {
    __m256i bx = _mm256_set1_epi16( b & 0xf );
    __m256i b1 = _mm256_srli_epi16( bx, 1 );

    const __m256i tab0 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 0));
    const __m256i tab1 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 1));
    const __m256i tab2 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 2));
    const __m256i tab3 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 3));

    __m256i mask_1  = _mm256_set1_epi16(1);
    __m256i mask_4  = _mm256_set1_epi16(4);
    __m256i mask_0  = _mm256_setzero_si256();

    return ( tab0 & _mm256_cmpgt_epi16( bx & mask_1, mask_0) )
           ^ ( tab1 & _mm256_cmpgt_epi16( b1 & mask_1, mask_0) )
           ^ ( tab2 & _mm256_cmpgt_epi16( bx & mask_4, mask_0) )
           ^ ( tab3 & _mm256_cmpgt_epi16( b1 & mask_4, mask_0) );
}

// constant-time GF(16) inverse a^14, via a shuffle
static inline unsigned char inverse_f_ct(unsigned char a) {
    const __m128i tab = _mm_setr_epi8(0, 1, 9, 14, 13, 11, 7, 6,
                                      15, 2, 12, 5, 10, 4, 3, 8);
    __m128i idx = _mm_set1_epi8((char)(a & 0xf));
    return (unsigned char)(_mm_cvtsi128_si32(_mm_shuffle_epi8(tab, idx)) & 0xf);
}

static const unsigned char ef_lane_idx[64] __attribute__((aligned(64))) = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63
};

// per-row masks row==pivot_row (eq) and row>pivot_row (gt), via data-oblivious
// vector compares; indexed by the public row in place of the per-row ct_ calls.
// the gfni EF wants only eq; it takes row>pivot_row from a byte compare instead
static inline void ef_row_masks(int pivot_row, int nrows, int *eq, int *gt) {
    const __m256i pr = _mm256_set1_epi32(pivot_row);
    const __m256i base = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
    for (int r = 0; r < nrows; r += 8) {
        __m256i rows = _mm256_add_epi32(base, _mm256_set1_epi32(r));
        _mm256_storeu_si256((__m256i *)(eq + r), _mm256_cmpeq_epi32(rows, pr));
        _mm256_storeu_si256((__m256i *)(gt + r), _mm256_cmpgt_epi32(rows, pr));
    }
}

static inline void ef_row_mask_eq(int pivot_row, int nrows, int *eq) {
    const __m256i pr = _mm256_set1_epi32(pivot_row);
    const __m256i base = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
    for (int r = 0; r < nrows; r += 8) {
        __m256i rows = _mm256_add_epi32(base, _mm256_set1_epi32(r));
        _mm256_storeu_si256((__m256i *)(eq + r), _mm256_cmpeq_epi32(rows, pr));
    }
}

#if defined(MAYO_GFNI) && defined(__GFNI__) && defined(__AVX512F__) && defined(__AVX512BW__)

/* put matrix in row echelon form with ones on first nonzero entries in constant time.
 * GFNI variant: rows packed two nibbles per byte (one zmm per row, two for MAYO-5),
 * each scalar multiply is a single vgf2p8affineqb; output matches the byte-format EF. */
static inline void EF(unsigned char *A, int _nrows, int _ncols) {

    (void) _nrows;
    (void) _ncols;

    #define nrows M_MAX
    #define ncols (K_MAX * O_MAX + 1)
    #define EF_P_BYTES ((ncols + 1) / 2)
    #define EF_ZREGS ((EF_P_BYTES + 63) / 64)
    #define EF_TABS_N (((nrows + 31) / 32) * 32)
    #define EF_BUF_N  (((EF_TABS_N + 63) / 64) * 64)
    #define EF_STRIDE (EF_ZREGS * 64)

    __m512i Apk[EF_ZREGS * nrows];
    unsigned char *Apk_bytes = (unsigned char *) Apk;

    // pack the rows, two nibbles per byte; the zero padding stays zero
    // through all row operations
    for (int i = 0; i < nrows; i++) {
        int j = 0;
        for (; j + 1 < ncols; j += 2) {
            Apk_bytes[i * EF_STRIDE + j / 2] = (unsigned char) (A[i * ncols + j] | (A[i * ncols + j + 1] << 4));
        }
#if ncols % 2 == 1
        Apk_bytes[i * EF_STRIDE + j / 2] = A[i * ncols + j];
        j += 2;
#endif
        for (int b = j / 2; b < EF_STRIDE; b++) {
            Apk_bytes[i * EF_STRIDE + b] = 0;
        }
    }

    // pivot row is secret, pivot col is not
    // the search reads ef_col and writes ef_sel a whole register at a time
    unsigned char ef_col[EF_BUF_N] = {0};  // secret: one column of A
    // sized to the builder's batch so the multipliers miss its scalar tail, and on to a
    // whole zmm because the mask below stores one; the rows past elim_rows build junk
    unsigned char ef_elim[EF_BUF_N] = {0}; // secret: that column, masked
    uint64_t ef_tabs[EF_TABS_N];           // secret: its multiplication matrices
    uint32_t ef_sel[M_MAX + 16];  // secret: which of those rows is selected
    int pivot_row = 0;
    for (int pivot_col = 0; pivot_col < ncols; pivot_col++) {
        int pivot_row_lower_bound = MAYO_MAX(0, pivot_col + nrows - ncols);
        int pivot_row_upper_bound = MAYO_MIN(nrows - 1, pivot_col);
        /* the pivot row is guaranteed to be between these lower and upper bounds if A has full rank */
        int search_end = MAYO_MIN(nrows - 1, pivot_row_upper_bound + 32);

        const int pcb = pivot_col / 2;
        const int pcs = (pivot_col & 1) * 4;

        int eq32[M_MAX + 8];
        ef_row_mask_eq(pivot_row, nrows, eq32);

        /* the search and the elimination multipliers read the same column, one row per
           cache line, so read it once and let both run over contiguous bytes */
        const int elim_rows = nrows - pivot_row_lower_bound;
#if defined(__clang__)
        #pragma clang loop vectorize(disable)
#endif
        for (int i = 0; i < elim_rows; i++) {
            ef_col[i] = (unsigned char) ((Apk_bytes[(pivot_row_lower_bound + i) * EF_STRIDE + pcb] >> pcs) & 0xf);
        }

        /* try to get a pivot row in constant time: select every candidate up to and
           including the first nonzero one, as a lowest set bit isolate. f | (f - 1) is
           the prefix through f, and f == 0 selects every candidate. Only masks and
           public shift amounts, so nothing branches or indexes on secret data. */
        const int nsearch = search_end - pivot_row_lower_bound + 1;
        const uint64_t valid = (~(uint64_t) 0) >> (64 - nsearch);

        const __m512i colv = _mm512_loadu_si512((const void *) ef_col);
        const __m512i rowsv = _mm512_add_epi8(_mm512_load_si512((const void *) ef_lane_idx),
                                              _mm512_set1_epi8((char) pivot_row_lower_bound));
        /* candidates are the rows at or after the pivot row */
        uint64_t cand = (uint64_t) _mm512_cmpge_epu8_mask(rowsv, _mm512_set1_epi8((char) pivot_row)) & valid;
        uint64_t nz = (uint64_t) _mm512_cmpneq_epi8_mask(colv, _mm512_setzero_si512()) & cand;
        uint64_t f = nz & (~nz + 1);
        uint64_t sel = cand & (f | (f - 1));

        /* only one selected row can be nonzero, so xor-reduce to get its entry */
        __m512i selv = _mm512_maskz_mov_epi8((__mmask64) sel, colv);
        __m256i r256 = _mm512_castsi512_si256(selv) ^ _mm512_extracti64x4_epi64(selv, 1);
        __m128i r128 = _mm256_castsi256_si128(r256) ^ _mm256_extracti128_si256(r256, 1);
        r128 ^= _mm_unpackhi_epi64(r128, r128);
        uint64_t red = (uint64_t) _mm_cvtsi128_si64(r128);
        red ^= red >> 32;
        red ^= red >> 16;
        red ^= red >> 8;
        unsigned char pivot = (unsigned char) (red & 0xf);

        for (int j = 0; j < (nsearch + 15) / 16; j++) {
            _mm512_storeu_si512((void *) (ef_sel + pivot_row_lower_bound + 16 * j),
                                _mm512_movm_epi32((__mmask16) (sel >> (16 * j))));
        }
        uint32_t pivot_is_zero = ~ct_compare_32((int) pivot, 0);
        __m512i pr[EF_ZREGS];
        for (int n = 0; n < EF_ZREGS; n++) {
            pr[n] = _mm512_setzero_si512();
        }
        for (int row = pivot_row_lower_bound; row <= search_end; row++) {
            __m512i mv = _mm512_set1_epi32((int) ef_sel[row]);
            for (int n = 0; n < EF_ZREGS; n++) {
                pr[n] ^= mv & Apk[row * EF_ZREGS + n];
            }
        }

        /* multiply pivot row by inverse of pivot */
        unsigned char inverse = inverse_f_ct(pivot);
        __m512i mat_inv = _mm512_set1_epi64((long long) mayo_gfni_tab(inverse));
        for (int n = 0; n < EF_ZREGS; n++) {
            pr[n] = _mm512_gf2p8affine_epi64_epi8(pr[n], mat_inv, 0);
        }

        /* the multipliers are one column of A, so build the matrices in one batched
           pass rather than running mayo_gfni_tab's serial chain per row. The builder
           is branch-free and secret data still enters only through the mask. */
        const int elim_rows_p = (elim_rows + 31) & ~31;
        for (int i = 0; i < elim_rows_p; i += 64) {
            __m512i rowsb = _mm512_add_epi8(_mm512_load_si512((const void *) ef_lane_idx),
                                            _mm512_set1_epi8((char) (pivot_row_lower_bound + i)));
            __mmask64 gt = _mm512_cmpgt_epu8_mask(rowsb, _mm512_set1_epi8((char) pivot_row));
            _mm512_storeu_si512((void *) (ef_elim + i),
                                _mm512_maskz_mov_epi8(gt, _mm512_loadu_si512((const void *) (ef_col + i))));
        }
        mayo_gfni_tabs(ef_elim, ef_tabs, (size_t) elim_rows_p);

        /* conditionally write pivot row to the correct row, if there is a nonzero pivot */
        /* eliminate entries below pivot */
        for (int row = pivot_row_lower_bound; row < nrows; row++) {
            __m512i mat_e = _mm512_set1_epi64((long long) ef_tabs[row - pivot_row_lower_bound]);
            if (row <= pivot_row_upper_bound) {
                __mmask64 wr = (__mmask64) (int64_t) (int32_t) ((uint32_t)eq32[row] & ~pivot_is_zero);
                for (int n = 0; n < EF_ZREGS; n++) {
                    Apk[row * EF_ZREGS + n] = _mm512_mask_mov_epi8(Apk[row * EF_ZREGS + n], wr, pr[n])
                                            ^ _mm512_gf2p8affine_epi64_epi8(pr[n], mat_e, 0);
                }
            } else {
                for (int n = 0; n < EF_ZREGS; n++) {
                    Apk[row * EF_ZREGS + n] ^= _mm512_gf2p8affine_epi64_epi8(pr[n], mat_e, 0);
                }
            }
        }

        pivot_row += (-(int32_t)(~pivot_is_zero));
    }

    // write the matrix A back, one nibble per byte
    for (int i = 0; i < nrows; i++) {
        int j = 0;
        for (; j + 1 < ncols; j += 2) {
            unsigned char b = Apk_bytes[i * EF_STRIDE + j / 2];
            A[i * ncols + j] = b & 0xf;
            A[i * ncols + j + 1] = b >> 4;
        }
#if ncols % 2 == 1
        A[i * ncols + j] = Apk_bytes[i * EF_STRIDE + j / 2] & 0xf;
#endif
    }

    mayo_secure_clear(Apk, sizeof(Apk));
    mayo_secure_clear(ef_col, sizeof(ef_col));
    mayo_secure_clear(ef_elim, sizeof(ef_elim));
    mayo_secure_clear(ef_tabs, sizeof(ef_tabs));
    mayo_secure_clear(ef_sel, sizeof(ef_sel));

    #undef nrows
    #undef ncols
    #undef EF_P_BYTES
    #undef EF_ZREGS
    #undef EF_STRIDE
}

#else

/* put matrix in row echelon form with ones on first nonzero entries in constant time*/
static inline void EF(unsigned char *A, int _nrows, int _ncols) {

    (void) _nrows;
    (void) _ncols;

    #define nrows M_MAX
    #define ncols (K_MAX * O_MAX + 1)

    #define AVX_REGS_PER_ROW ((K_MAX * O_MAX + 1 + 31) / 32)
    #define MAX_COLS (AVX_REGS_PER_ROW * 32)

    __m256i _pivot_row[AVX_REGS_PER_ROW];
    __m256i A_avx[AVX_REGS_PER_ROW* M_MAX];

    unsigned char* A_bytes = (unsigned char*) A_avx;

    // load A in the tail of AVX2 registers
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++)
        {
            A_bytes[i*MAX_COLS + (MAX_COLS - ncols) + j] = A[ i*ncols + j ];
        }
    }

    // pivot row is secret, pivot col is not
    unsigned char inverse;
    unsigned char ef_val[64];    // secret: the pivot column over the search range
    uint32_t ef_sel[M_MAX];      // secret: which of those rows is selected
    int pivot_row = 0;
    int pivot_col = MAYO_MAX(MAX_COLS - ncols,0);
    for (; pivot_col < MAX_COLS-160; pivot_col++) {
        #include "echelon_form_loop.h"
    }
    for (; pivot_col < MAX_COLS-128; pivot_col++) {
        #include "echelon_form_loop.h"
    }
    for (; pivot_col < MAX_COLS-96; pivot_col++) {
        #include "echelon_form_loop.h"
    }
    for (; pivot_col < MAX_COLS-64; pivot_col++) {
        #include "echelon_form_loop.h"
    }
    for (; pivot_col < MAX_COLS-32; pivot_col++) {
        #include "echelon_form_loop.h"
    }
    for (; pivot_col < MAX_COLS; pivot_col++) {
        #include "echelon_form_loop.h"
    }

    // write the matrix A back
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            A[i * ncols + j] = A_bytes[i*AVX_REGS_PER_ROW*32 + (MAX_COLS - ncols) + j];
        }
    }
    mayo_secure_clear(_pivot_row, AVX_REGS_PER_ROW * 32);
    mayo_secure_clear(ef_val, sizeof(ef_val));
    mayo_secure_clear(ef_sel, sizeof(ef_sel));
    mayo_secure_clear(A_avx, AVX_REGS_PER_ROW * 32 * nrows);
}

#endif // MAYO_GFNI

static const unsigned char ef_lt_mask[64] __attribute__((aligned(32))) = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* Ar = A * r on the byte-per-nibble matrix (the caller clears the last column
 * of A). Vector GF(16) multiply via mask arithmetic, avoiding the scalar mul_f
 * blocker that is ~10x slower on clang >= 15 / gcc >= 13. */
static inline void EF_mat_vec_mul(const unsigned char *A, const unsigned char *r, unsigned char *Ar) {

    #define MV_COLS (K_MAX * O_MAX + 1)
    #define MV_REGS ((MV_COLS + 31) / 32)

    const __m256i low_nib = _mm256_set1_epi8(0xf);

    // r, zero-padded and pre-shifted by the four multiplier bit positions
    // (padding also nulls products with bytes read past the row end)
    unsigned char rpad[MV_REGS * 32];
    for (int i = 0; i < MV_COLS; i++) {
        rpad[i] = r[i];
    }
    for (int i = MV_COLS; i < MV_REGS * 32; i++) {
        rpad[i] = 0;
    }
    __m256i rs[4][MV_REGS];
    for (int j = 0; j < MV_REGS; j++) {
        __m256i rv = _mm256_loadu_si256((const __m256i *)(rpad + 32 * j));
        rs[0][j] = rv;
        rs[1][j] = _mm256_slli_epi16(rv, 1);
        rs[2][j] = _mm256_slli_epi16(rv, 2);
        rs[3][j] = _mm256_slli_epi16(rv, 3);
    }

    for (int i = 0; i < M_MAX; i++) {
        __m256i acc = _mm256_setzero_si256();
        for (int j = 0; j < MV_REGS; j++) {
            __m256i av = _mm256_loadu_si256((const __m256i *)(A + i * MV_COLS + 32 * j));
            // p = a * b (carryless), nibble entries per byte
            __m256i p = _mm256_setzero_si256();
            for (int bit = 0; bit < 4; bit++) {
                __m256i bv = _mm256_set1_epi8((char)(1 << bit));
                __m256i m = _mm256_cmpeq_epi8(av & bv, bv);
                p ^= m & rs[bit][j];
            }
            // reduce mod x^4 + x + 1
            __m256i top = _mm256_andnot_si256(low_nib, p);
            acc ^= (p ^ (_mm256_srli_epi16(top, 4) & low_nib)
                      ^ (_mm256_srli_epi16(top, 3) & _mm256_set1_epi8(0x1e))) & low_nib;
        }
        // horizontal xor of all bytes
        __m128i h = _mm_xor_si128(_mm256_castsi256_si128(acc), _mm256_extracti128_si256(acc, 1));
        h = _mm_xor_si128(h, _mm_srli_si128(h, 8));
        h = _mm_xor_si128(h, _mm_srli_si128(h, 4));
        h = _mm_xor_si128(h, _mm_srli_si128(h, 2));
        h = _mm_xor_si128(h, _mm_srli_si128(h, 1));
        Ar[i] = (unsigned char)(_mm_cvtsi128_si32(h) & 0xf);
    }

    mayo_secure_clear(rpad, sizeof(rpad));

    #undef MV_COLS
    #undef MV_REGS
}

/* constant-time back substitution on [A | y] (one nibble per byte) into x.
 * A's coefficient columns are read-only, so transpose them once and eliminate
 * 32 rows per vpshufb; the candidate column may be y itself (col == k*o), read
 * live. Output matches the scalar version. */
// transpose a 16x16 byte block: dst[j*ds + i] = src[i*ss + j]
static inline void ef_transpose_16x16(const unsigned char *src, int ss,
                                      unsigned char *dst, int ds) {
    __m128i r[16], t[16];
    for (int i = 0; i < 16; i++) r[i] = _mm_loadu_si128((const __m128i *)(src + (size_t) i * ss));
    for (int i = 0; i < 8; i++) {
        t[2*i]     = _mm_unpacklo_epi8(r[2*i], r[2*i+1]);
        t[2*i + 1] = _mm_unpackhi_epi8(r[2*i], r[2*i+1]);
    }
    for (int g = 0; g < 4; g++) {
        r[4*g + 0] = _mm_unpacklo_epi16(t[4*g+0], t[4*g+2]);
        r[4*g + 1] = _mm_unpackhi_epi16(t[4*g+0], t[4*g+2]);
        r[4*g + 2] = _mm_unpacklo_epi16(t[4*g+1], t[4*g+3]);
        r[4*g + 3] = _mm_unpackhi_epi16(t[4*g+1], t[4*g+3]);
    }
    for (int h = 0; h < 2; h++) for (int j = 0; j < 4; j++) {
        t[8*h + 2*j]     = _mm_unpacklo_epi32(r[8*h+j], r[8*h+j+4]);
        t[8*h + 2*j + 1] = _mm_unpackhi_epi32(r[8*h+j], r[8*h+j+4]);
    }
    for (int j = 0; j < 8; j++) {
        r[2*j]     = _mm_unpacklo_epi64(t[j], t[j+8]);
        r[2*j + 1] = _mm_unpackhi_epi64(t[j], t[j+8]);
    }
    for (int i = 0; i < 16; i++) _mm_storeu_si128((__m128i *)(dst + (size_t) i * ds), r[i]);
}

static inline void EF_backsub(const unsigned char *A, unsigned char *x) {

    #define BS_ACOLS (K_MAX * O_MAX + 1)
    #define BS_KO (K_MAX * O_MAX)
    #define BS_CHUNKS ((M_MAX + 31) / 32)
    #define BS_STRIDE (BS_CHUNKS * 32)

    unsigned char y[BS_STRIDE];
    unsigned char T[BS_KO * BS_STRIDE];

#if defined(__clang__)
    #pragma clang loop vectorize(disable)
#endif
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

            __m256i tab = tbl32_gf16_multab(u);
            const unsigned char *src = (col < BS_KO) ? (T + col * BS_STRIDE) : y;

            // y[i] ^= u * column[i] for i < row (public bound)
            int full = row / 32;
            int rem = row % 32;
            for (int c = 0; c < full; c++) {
                __m256i yv = _mm256_loadu_si256((const __m256i *)(y + 32 * c));
                __m256i cv = _mm256_loadu_si256((const __m256i *)(src + 32 * c));
                _mm256_storeu_si256((__m256i *)(y + 32 * c), yv ^ _mm256_shuffle_epi8(tab, cv));
            }
            if (rem) {
                __m256i mask = _mm256_loadu_si256((const __m256i *)(ef_lt_mask + 32 - rem));
                __m256i yv = _mm256_loadu_si256((const __m256i *)(y + 32 * full));
                __m256i cv = _mm256_loadu_si256((const __m256i *)(src + 32 * full));
                _mm256_storeu_si256((__m256i *)(y + 32 * full), yv ^ (_mm256_shuffle_epi8(tab, cv) & mask));
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

