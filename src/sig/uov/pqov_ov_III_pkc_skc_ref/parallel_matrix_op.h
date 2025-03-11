// SPDX-License-Identifier: CC0 OR Apache-2.0
///  @file  parallel_matrix_op.h
///  @brief Librarys for operations of batched matrixes.
///
///

#ifndef _P_MATRIX_OP_H_
#define _P_MATRIX_OP_H_

//IF_CRYPTO_CORE:define CRYPTO_NAMESPACE

#ifdef  __cplusplus
extern  "C" {
#endif



////////////////////////////////////////////////////////////////////////
///   Librarys for batched matrix operations.
///   A batched matrix is a matrix which each element of the matrix
///   contains size_batch GF elements.
////////////////////////////////////////////////////////////////////////




////////////////  Section:  triangle matrix <-> rectangle matrix   ///////////////////////////////////


///
/// @brief  Calculate the corresponding index in an array for an upper-triangle(UT) matrix.
///
/// @param[in]  i_row     - the i-th row in an upper-triangle matrix.
/// @param[in]  j_col     - the j-th column in an upper-triangle matrix.
/// @param[in]  dim       - the dimension of the upper-triangle matrix, i.e., an dim x dim matrix.
/// @return  the corresponding index in an array storage.
///
static inline
unsigned idx_of_trimat( unsigned i_row, unsigned j_col, unsigned dim ) {
    return (dim + dim - i_row + 1 ) * i_row / 2 + j_col - i_row;
}

///
/// @brief  Calculate the corresponding index in an array for an upper-triangle or lower-triangle matrix.
///
/// @param[in]  i_row     - the i-th row in a triangle matrix.
/// @param[in]  j_col     - the j-th column in a triangle matrix.
/// @param[in]  dim       - the dimension of the triangle matrix, i.e., an dim x dim matrix.
/// @return    the corresponding index in an array storage.
///
static inline
unsigned idx_of_2trimat( unsigned i_row, unsigned j_col, unsigned n_var ) {
    if ( i_row > j_col ) {
        return idx_of_trimat(j_col, i_row, n_var);
    } else {
        return idx_of_trimat(i_row, j_col, n_var);
    }
}


///
/// @brief  Upper trianglize a rectangle matrix to the corresponding upper-triangle matrix.
///
/// @param[out]  btriC    - the batched upper-trianglized matrix C.
/// @param[in]   bA       - a batched rectangle matrix A.
/// @param[in]   bwidth   - the width of the batched matrix A, i.e., A is a Awidth x Awidth matrix.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define UpperTrianglize PQOV_NAMESPACE(UpperTrianglize)
void UpperTrianglize( unsigned char *btriC, const unsigned char *bA, unsigned Awidth, unsigned size_batch );




////////////////////  Section:  matrix multiplications  ///////////////////////////////



///
/// @brief  bC += btriA * B  , in GF(16)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A.
/// @param[in]   B          - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_trimat_madd_gf16 PQOV_NAMESPACE(batch_trimat_madd_gf16)
void batch_trimat_madd_gf16( unsigned char *bC, const unsigned char *btriA,
                             const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );

///
/// @brief  bC += btriA * B  , in GF(256)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A.
/// @param[in]   B          - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_trimat_madd_gf256 PQOV_NAMESPACE(batch_trimat_madd_gf256)
void batch_trimat_madd_gf256( unsigned char *bC, const unsigned char *btriA,
                              const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );


///
/// @brief  bC += btriA^Tr * B  , in GF(16)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A. A will be transposed while multiplying.
/// @param[in]   B          - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_trimatTr_madd_gf16 PQOV_NAMESPACE(batch_trimatTr_madd_gf16)
void batch_trimatTr_madd_gf16( unsigned char *bC, const unsigned char *btriA,
                               const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );

///
/// @brief  bC += btriA^Tr * B  , in GF(256)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A, which will be transposed while multiplying.
/// @param[in]   B          - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_trimatTr_madd_gf256 PQOV_NAMESPACE(batch_trimatTr_madd_gf256)
void batch_trimatTr_madd_gf256( unsigned char *bC, const unsigned char *btriA,
                                const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );



///
/// @brief  bC +=  (btriA + btriA^Tr) *B  , in GF(16)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A. The operand for multiplication is (btriA + btriA^Tr).
/// @param[in]   B          - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_2trimat_madd_gf16 PQOV_NAMESPACE(batch_2trimat_madd_gf16)
void batch_2trimat_madd_gf16( unsigned char *bC, const unsigned char *btriA,
                              const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );

///
/// @brief  bC +=  (btriA + btriA^Tr) *B  , in GF(256)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A. The operand for multiplication is (btriA + btriA^Tr).
/// @param[in]   B          - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_2trimat_madd_gf256 PQOV_NAMESPACE(batch_2trimat_madd_gf256)
void batch_2trimat_madd_gf256( unsigned char *bC, const unsigned char *btriA,
                               const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );



///
/// @brief  bC += A^Tr * bB  , in GF(16)
///
/// @param[out]  bC           - the batched matrix C.
/// @param[in]   A_to_tr      - a column-major matrix A. The operand for multiplication is A^Tr.
/// @param[in]   Aheight      - the height of A.
/// @param[in]   size_Acolvec    - the size of a column vector in A.
/// @param[in]   Awidth           - the width of A.
/// @param[in]   bB          - a batched matrix B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_matTr_madd_gf16 PQOV_NAMESPACE(batch_matTr_madd_gf16)
void batch_matTr_madd_gf16( unsigned char *bC,
                            const unsigned char *A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
                            const unsigned char *bB, unsigned Bwidth, unsigned size_batch );

///
/// @brief  bC += A^Tr * bB  , in GF(256)
///
/// @param[out]  bC           - the batched matrix C.
/// @param[in]   A_to_tr      - a column-major matrix A. The operand for multiplication is A^Tr.
/// @param[in]   Aheight      - the height of A.
/// @param[in]   size_Acolvec    - the size of a column vector in A.
/// @param[in]   Awidth           - the width of A.
/// @param[in]   bB          - a batched matrix B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_matTr_madd_gf256 PQOV_NAMESPACE(batch_matTr_madd_gf256)
void batch_matTr_madd_gf256( unsigned char *bC,
                             const unsigned char *A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
                             const unsigned char *bB, unsigned Bwidth, unsigned size_batch );




///
/// @brief  bC = A^Tr * bB  and then upper(bC), in GF(16)
///
/// @param[out]  bC           - the batched matrix C.
/// @param[in]   A_to_tr      - a column-major matrix A. The operand for multiplication is A^Tr.
/// @param[in]   Aheight      - the height of A.
/// @param[in]   size_Acolvec    - the size of a column vector in A.
/// @param[in]   Awidth           - the width of A.
/// @param[in]   bB          - a batched matrix B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_upper_matTr_x_mat_gf16 PQOV_NAMESPACE(batch_upper_matTr_x_mat_gf16)
void batch_upper_matTr_x_mat_gf16( unsigned char *bC,
                                   const unsigned char *A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
                                   const unsigned char *bB, unsigned Bwidth, unsigned size_batch );

///
/// @brief  bC = A^Tr * bB  and then upper(bC), in GF(256)
///
/// @param[out]  bC           - the batched matrix C.
/// @param[in]   A_to_tr      - a column-major matrix A. The operand for multiplication is A^Tr.
/// @param[in]   Aheight      - the height of A.
/// @param[in]   size_Acolvec    - the size of a column vector in A.
/// @param[in]   Awidth           - the width of A.
/// @param[in]   bB          - a batched matrix B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_upper_matTr_x_mat_gf256 PQOV_NAMESPACE(batch_upper_matTr_x_mat_gf256)
void batch_upper_matTr_x_mat_gf256( unsigned char *bC,
                                    const unsigned char *A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
                                    const unsigned char *bB, unsigned Bwidth, unsigned size_batch );




////////////////////  Section: "quadratric" matrix evaluation  ///////////////////////////////


///
/// @brief  y =  x^Tr * trimat * x  , in GF(16)
///
/// @param[out]  y          - the returned batched element y.
/// @param[in]   trimat     - a batched matrix.
/// @param[in]   x          - an input vector x.
/// @param[in]   dim        - the dimension of matrix trimat (and x).
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_quad_trimat_eval_gf16 PQOV_NAMESPACE(batch_quad_trimat_eval_gf16)
void batch_quad_trimat_eval_gf16( unsigned char *y, const unsigned char *trimat, const unsigned char *x, unsigned dim, unsigned size_batch );

///
/// @brief  y =  x^Tr * trimat * x  , in GF(256)
///
/// @param[out]  y          - the returned batched element y.
/// @param[in]   trimat     - a batched matrix.
/// @param[in]   x          - an input vector x.
/// @param[in]   dim        - the dimension of matrix trimat (and x).
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_quad_trimat_eval_gf256 PQOV_NAMESPACE(batch_quad_trimat_eval_gf256)
void batch_quad_trimat_eval_gf256( unsigned char *y, const unsigned char *trimat, const unsigned char *x, unsigned dim, unsigned size_batch );





///////////// multiplication with tables //////////////////








///
/// @brief  bC += btriA * B  , in GF(16)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A.
/// @param[in]   multab_B   - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_trimat_madd_multab_gf16 PQOV_NAMESPACE(batch_trimat_madd_multab_gf16)
void batch_trimat_madd_multab_gf16( unsigned char *bC, const unsigned char *btriA,
                                    const unsigned char *multab_B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );

///
/// @brief  bC += btriA * B  , in GF(256)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A.
/// @param[in]   multab_B   - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_trimat_madd_multab_gf256 PQOV_NAMESPACE(batch_trimat_madd_multab_gf256)
void batch_trimat_madd_multab_gf256( unsigned char *bC, const unsigned char *btriA,
                                     const unsigned char *multab_B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );


///
/// @brief  bC += btriA^Tr * B  , in GF(16)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A. A will be transposed while multiplying.
/// @param[in]   multab_B   - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_trimatTr_madd_multab_gf16 PQOV_NAMESPACE(batch_trimatTr_madd_multab_gf16)
void batch_trimatTr_madd_multab_gf16( unsigned char *bC, const unsigned char *btriA,
                                      const unsigned char *multab_B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );

///
/// @brief  bC += btriA^Tr * B  , in GF(256)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A, which will be transposed while multiplying.
/// @param[in]   multab_B   - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_trimatTr_madd_multab_gf256 PQOV_NAMESPACE(batch_trimatTr_madd_multab_gf256)
void batch_trimatTr_madd_multab_gf256( unsigned char *bC, const unsigned char *btriA,
                                       const unsigned char *multab_B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );



///
/// @brief  bC +=  (btriA + btriA^Tr) *B  , in GF(16)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A. The operand for multiplication is (btriA + btriA^Tr).
/// @param[in]   multab_B   - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_2trimat_madd_multab_gf16 PQOV_NAMESPACE(batch_2trimat_madd_multab_gf16)
void batch_2trimat_madd_multab_gf16( unsigned char *bC, const unsigned char *btriA,
                                     const unsigned char *multab_B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );

///
/// @brief  bC +=  (btriA + btriA^Tr) *B  , in GF(256)
///
/// @param[out]  bC         - the batched matrix C.
/// @param[in]   btriA      - a batched UT matrix A. The operand for multiplication is (btriA + btriA^Tr).
/// @param[in]   multab_B   - a column-major matrix B.
/// @param[in]   Bheight          - the height of B.
/// @param[in]   size_Bcolvec     - the size of the column vector in B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_2trimat_madd_multab_gf256 PQOV_NAMESPACE(batch_2trimat_madd_multab_gf256)
void batch_2trimat_madd_multab_gf256( unsigned char *bC, const unsigned char *btriA,
                                      const unsigned char *multab_B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch );



///
/// @brief  bC += A^Tr * bB  , in GF(16)
///
/// @param[out]  bC           - the batched matrix C.
/// @param[in]   multab_A_to_tr      - a column-major matrix A. The operand for multiplication is A^Tr.
/// @param[in]   Aheight      - the height of A.
/// @param[in]   size_Acolvec    - the size of a column vector in A.
/// @param[in]   Awidth           - the width of A.
/// @param[in]   bB          - a batched matrix B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_matTr_madd_multab_gf16 PQOV_NAMESPACE(batch_matTr_madd_multab_gf16)
void batch_matTr_madd_multab_gf16( unsigned char *bC,
                                   const unsigned char *multab_A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
                                   const unsigned char *bB, unsigned Bwidth, unsigned size_batch );

///
/// @brief  bC += A^Tr * bB  , in GF(256)
///
/// @param[out]  bC           - the batched matrix C.
/// @param[in]   multab_A_to_tr   - a column-major matrix A. The operand for multiplication is A^Tr.
/// @param[in]   Aheight      - the height of A.
/// @param[in]   size_Acolvec    - the size of a column vector in A.
/// @param[in]   Awidth           - the width of A.
/// @param[in]   bB          - a batched matrix B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_matTr_madd_multab_gf256 PQOV_NAMESPACE(batch_matTr_madd_multab_gf256)
void batch_matTr_madd_multab_gf256( unsigned char *bC,
                                    const unsigned char *multab_A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
                                    const unsigned char *bB, unsigned Bwidth, unsigned size_batch );



///
/// @brief  bC = A^Tr * bB  and then upper(bC), in GF(16)
///
/// @param[out]  bC           - the batched matrix C.
/// @param[in]   multab_A_to_tr      - a column-major matrix A. The operand for multiplication is A^Tr.
/// @param[in]   Aheight      - the height of A.
/// @param[in]   size_Acolvec    - the size of a column vector in A.
/// @param[in]   Awidth           - the width of A.
/// @param[in]   bB          - a batched matrix B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_upper_matTr_x_mat_multab_gf16 PQOV_NAMESPACE(batch_upper_matTr_x_mat_multab_gf16)
void batch_upper_matTr_x_mat_multab_gf16( unsigned char *bC,
        const unsigned char *multab_A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
        const unsigned char *bB, unsigned Bwidth, unsigned size_batch );

///
/// @brief  bC = A^Tr * bB  and then upper(bC), in GF(256)
///
/// @param[out]  bC           - the batched matrix C.
/// @param[in]   multab_A_to_tr      - a column-major matrix A. The operand for multiplication is A^Tr.
/// @param[in]   Aheight      - the height of A.
/// @param[in]   size_Acolvec    - the size of a column vector in A.
/// @param[in]   Awidth           - the width of A.
/// @param[in]   bB          - a batched matrix B.
/// @param[in]   Bwidth           - the width of B.
/// @param[in]   size_batch - number of the batched elements in the corresponding position of the matrix.
///
#define batch_upper_matTr_x_mat_multab_gf256 PQOV_NAMESPACE(batch_upper_matTr_x_mat_multab_gf256)
void batch_upper_matTr_x_mat_multab_gf256( unsigned char *bC,
        const unsigned char *multab_A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
        const unsigned char *bB, unsigned Bwidth, unsigned size_batch );



#ifdef  __cplusplus
}
#endif


#endif // _P_MATRIX_OP_H_

