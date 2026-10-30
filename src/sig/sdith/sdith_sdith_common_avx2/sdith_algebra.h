#ifndef SDITH_ALGEBRA_H
#define SDITH_ALGEBRA_H

// This file contains the declaration of all algebra functions over vectors and matrices over F2

#include "commons.h"

/** vector addition over F2 */
EXPORT void bitvec_xor_to_ref(uint64_t bytelen, bitvec_t* res, const bitvec_t* b);
EXPORT void bitvec_xor_ref(uint64_t bytelen, bitvec_t* res, const bitvec_t* a, const bitvec_t* b);
EXPORT void bitvec_xor_to_avx2(uint64_t bytelen, bitvec_t* res, const bitvec_t* b);
EXPORT void bitvec_xor_avx2(uint64_t bytelen, bitvec_t* res, const bitvec_t* a, const bitvec_t* b);
/** cascade vector addition over F2 (b^=c then a^=b) */
EXPORT void bitvec_cascade_xor_to_ref(uint64_t bytelen, bitvec_t* c, bitvec_t* b, const bitvec_t* a);
EXPORT void bitvec_cascade_xor_to_avx2(uint64_t bytelen, bitvec_t* c, bitvec_t* b, const bitvec_t* a);

/** matrix vector product over F2 */
// (expect nrows between 120 and 260, ncols can be restricted to a multiple of 8)
EXPORT void matrix_vector_product_f2_ref(   //
    uint64_t nrows, uint64_t ncols,         // dimensions
    bitvec_t* res,                          // bit vector of size nrows. res=a*b
    const bitmat_t* a,                      // nrows x ncols matrix
    const bitvec_t* b);                     // bit vector of size ncols
EXPORT void matrix_vector_product_f2_avx2(  //
    uint64_t nrows, uint64_t ncols,         // dimensions
    bitvec_t* res,                          // bit vector of size nrows. res=a*b
    const bitmat_t* a,                      // nrows x ncols matrix
    const bitvec_t* b);                     // bit vector of size ncols

/** matrix vector product over F2 */
// (expect nrows between 120 and 260, ncols can be restricted to a multiple of 8. lambda will be 128, 192, or 256)
EXPORT void matrix_f2_times_vector_flambda_ref(       //
    uint64_t lambda, uint64_t nrows, uint64_t ncols,  //
    flambda_t* res,                                   // vector of size nrows. res = a*b
    const bitmat_t* a,                                // nrows x ncols matrix
    const flambda_t* b);                              // vector of size ncols
EXPORT void matrix_f2_times_vector_flambda_avx2(      //
    uint64_t lambda, uint64_t nrows, uint64_t ncols,  //
    flambda_t* res,                                   // vector of size nrows. res = a*b
    const bitmat_t* a,                                // nrows x ncols matrix
    const flambda_t* b);                              // vector of size ncols

#endif  // SDITH_ALGEBRA_H
