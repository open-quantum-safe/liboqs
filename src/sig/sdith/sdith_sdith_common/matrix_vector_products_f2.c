#include <string.h>

#include "vole_private.h"

// A helper to set a single bit in a bit vector (XOR it with 'value'):
static inline void bitvec_value_xor_to(bitvec_t* v, uint64_t index, uint8_t value) {
    const uint64_t byte_pos = index / 8;
    const uint64_t bit_pos  = index % 8;
    ((uint8_t*)v)[byte_pos] ^= (uint8_t)(value << bit_pos);
}

// Get a bit from a bit vector
static inline uint8_t bitvec_get_value(const bitvec_t* v, uint64_t idx) {
    const uint8_t* vec = (uint8_t*)v;
    return ( (vec[idx/8] >> (idx%8)) & 1 );
}

static __always_inline void bitvec_xor_inline_ref(uint64_t bytelen, bitvec_t* res, const bitvec_t* a,
                                                  const bitvec_t* b) {
  uint8_t* r = (uint8_t*)res;
  const uint8_t* aa = (uint8_t*)a;
  const uint8_t* bb = (uint8_t*)b;
  for (uint64_t i = 0; i < bytelen; ++i) {
    r[i] = aa[i] ^ bb[i];
  }
}

EXPORT void bitvec_xor_to_ref(uint64_t bytelen, bitvec_t* res, const bitvec_t* b) {
  bitvec_xor_inline_ref(bytelen, res, res, b);
}

EXPORT void bitvec_xor_ref(uint64_t bytelen, bitvec_t* res, const bitvec_t* a, const bitvec_t* b) {
  bitvec_xor_inline_ref(bytelen, res, a, b);
}

// b^=a then c^=b
EXPORT void bitvec_cascade_xor_to_ref(uint64_t bytelen, bitvec_t* c, bitvec_t* b, const bitvec_t* a) {
  bitvec_xor_inline_ref(bytelen, b, b, a);
  bitvec_xor_inline_ref(bytelen, c, c, b);
}


uint8_t bitmat_get_value(const bitmat_t* v, uint64_t ncols, uint64_t row, uint64_t col) {
  return bitvec_get_value(v, (row * ncols + col));
}

EXPORT void matrix_vector_product_f2_ref(  //
    uint64_t nrows, uint64_t ncols,        // dimensions
    bitvec_t* res,                         // bit vector of size nrows. res=a*b
    const bitmat_t* a,                     // nrows x ncols matrix
    const bitvec_t* b)                    // bit vector of size ncols
{
  memset(res, 0, (nrows+7)/8);
  for (uint64_t i=0; i<nrows; ++i) {
    uint8_t ri = 0;
    for (uint64_t j=0; j<ncols; ++j) {
      ri ^= bitmat_get_value(a, ncols, i, j) & bitvec_get_value(b, j);
    }
    bitvec_value_xor_to(res, i, ri);
  }
}


EXPORT void matrix_f2_times_vector_flambda_ref(       //
    uint64_t lambda, uint64_t nrows, uint64_t ncols,  //
    flambda_t* res,                                   // vector of size nrows. res = a*b
    const bitmat_t* a,                                // nrows x ncols matrix
    const flambda_t* b)                               // vector of size ncols
{
    const uint8_t* amat = (uint8_t*)a;
    const uint8_t* bvec = (uint8_t*)b;
    uint64_t lambda_bytes = lambda >> 3;
    // Clear the result
    memset(res, 0, nrows * lambda_bytes);

    // For each row i
    for (uint64_t i = 0; i < nrows; i++) {
        // Pointer to the result block for row i
        uint8_t* r_i = res + i * lambda_bytes;

        for (uint64_t j = 0; j < ncols; j++) {
            // Extract bit A[i,j]
            uint64_t bit_index = i*ncols + j;
            uint8_t a_ij = (amat[bit_index/8] >> (bit_index%8)) & 1;

            if (a_ij) {
                // If a_ij is 1, XOR the block B[j] into r_i
                const uint8_t* b_j = bvec + j * lambda_bytes;
                for (int x = 0; x < lambda_bytes; x++) {
                    r_i[x] ^= b_j[x];
                }
            }
        }
    }
}
