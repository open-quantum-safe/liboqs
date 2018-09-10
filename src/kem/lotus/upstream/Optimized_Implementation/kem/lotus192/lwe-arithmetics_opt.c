/**
 * @file lwe-arithmetics.c
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 * @brief Arithmetics used in LOTUS-CPA-PKE (optimized version)
 * @note Sometiems reference implementation is faster than this optimized implementation
 * with -march=native option. Perhaps this is because of SIMD instructions and memory alignemnt. 
 */

#include "type.h"
#include "param.h"

void submat_add_nn(U16 *C, const U16 *A, const U16 *B);
void submat_add_nl(U16 *C, const U16 *A, const U16 *B);
void submat_sub_nn(U16 *C, const U16 *A, const U16 *B);
void submat_sub_nl(U16 *C, const U16 *A, const U16 *B);
void submat_negmul(U16 *C, const U16 *A, const U16 *B);
void submat_submul(U16 *C, const U16 *A, const U16 *B);
void submat_negsubmul(U16 *C, const U16 *A, const U16 *B);
void distribute_2x2_nn(U16 *C, const U16 *A);
void distribute_2x2_nl(U16 *C, const U16 *A);
void merge_2x2_nl(U16 *C, const U16 *A);

/**
 * @brief P -= A * S using
 * level-1 Strassen-Winograd matrix multiplication with 3 temporaries, 
 *  see Table 2 of [B. Boyer, C. Pernet, and W. Zhou, 
 * "Memory efficient scheduing of Strassen-Winograd's matrix multiplication algorithm", 
 *  ISSAC 2009.] for details (this function is the case of alpha = -1, beta = 1). */
void submul(U16 *P, const U16 *A, const U16 *S){
  const U32 hnn = _LOTUS_LWE_DIM * _LOTUS_LWE_DIM / 4, hnl = _LOTUS_LWE_DIM * _LOTUS_LWE_PT / 4;
  U16 X[hnn],
    Y[hnl],
    Z[hnl],
    A_[_LOTUS_LWE_DIM * _LOTUS_LWE_DIM],
    S_[_LOTUS_LWE_DIM * _LOTUS_LWE_PT],
    P_[_LOTUS_LWE_DIM * _LOTUS_LWE_PT];
  
  distribute_2x2_nn(A_, A);
  distribute_2x2_nl(S_, S);
  distribute_2x2_nl(P_, P);
  
  submat_add_nn(X, A_ + hnn * 2, A_ + hnn * 3);
  submat_sub_nl(Y, S_ + hnl * 1, S_ + hnl * 0);
  submat_negmul(Z, X, Y);
  submat_add_nl(P_ + hnl * 3, P_ + hnl * 3, Z);
  submat_add_nl(P_ + hnl * 1, P_ + hnl * 1, Z);
  submat_sub_nn(X, X, A_ + hnn * 0);
  submat_sub_nl(Y, S_ + hnl * 3, Y);
  submat_negmul(Z, A_ + hnn * 0, S_ + hnl * 0);
  submat_add_nl(P_ + hnl * 0, P_ + hnl * 0, Z);
  submat_submul(Z, X, Y);
  submat_submul(P_ + hnl * 0, A_ + hnn * 1, S_ + hnl * 2);
  submat_sub_nn(X, A_ + hnn * 1, X);
  submat_sub_nl(Y, Y, S_ + hnl * 2);
  submat_submul(P_ + hnl * 1, X, S_ + hnl * 3);
  submat_add_nl(P_ + hnl * 1, P_ + hnl * 1, Z);
  submat_negsubmul(P_ + hnl * 2, A_ + hnn * 3, Y);
  submat_sub_nn(X, A_ + hnn * 0, A_ + hnn * 2);
  submat_sub_nl(Y, S_ + hnl * 3, S_ + hnl * 1);
  submat_submul(Z, X, Y);
  submat_add_nl(P_ + hnl * 3, P_ + hnl * 3, Z);
  submat_sub_nl(P_ + hnl * 2, Z, P_ + hnl * 2);

  merge_2x2_nl(P, P_);
}

/**
 * @brief v = v mod q, where q = 2^n
 */
void redc(U16 *v, const int len){
  int i;
  for(i = 0; i < len; ++i){
    v[i] &= (_LOTUS_LWE_MOD - 1);
  }
}


/**
 * @brief c2 += sigma * \ceil q / 2 \rceil, where sigma \in \{0, 1}^l and q = 2^n 
 */
void add_sigma(U16 *c2, const U8 *sigma){
  int i, j, idx = 0;
  for(i = 0; i < _LOTUS_LWE_PT_BYTES; ++i){
    for(j = 0; j < 8; ++j){
      /* move j-th bit of sigma[i] to (_LOTUS_LWE_LOG2_MOD - 1)-th bit */
      c2[idx] += (((U16)sigma[i]) << (_LOTUS_LWE_LOG2_MOD - 1 - j)) & (_LOTUS_LWE_MOD >> 1);
      ++idx;
    }
  }
}

/**
 * @brief sigma \in {0, 1}^l, where sigma[i] = 0 when t[i] \in [-q/4, q/4), 1 otherwise.
 */
void reconstruct(U8 *sigma, const U16 *t){
  int i, j, idx = 0;
  /* sigma[i] = 0 if 0 <= t[i] < q / 4 or 3q / 4 <= t[i] < q,
  otherwise sigma[i] = 1, i.e., q / 4 <= t[i] < 3q / 4.
  This means that, for q = 2^k, sigma[i] = b_{k-1} xor b_{k-2} where b_j is a j-th bit of t[idx],
  since {0 <= t[idx] < q / 4} or {3q / 4 <= t[idx] < q} then {b_{k-1} = 0 and b_{k-2} = 0} or
  {b_{k-1} = 1 and b_{k-2} = 1}, and {q / 4 <= t[idx] < 3q / 4},
  then {b_{k-1} = 1 and b_{k-2} = 0} or {b_{k-1} = 0 and b_{k-2} = 1}. */
  for(i = 0; i < _LOTUS_LWE_PT_BYTES; ++i){
    sigma[i] = 0;
    for(j = 0; j < 8; ++j){
      sigma[i] |= (U8)((((t[idx] >> (_LOTUS_LWE_LOG2_MOD - 1))
			 ^ (t[idx] >> (_LOTUS_LWE_LOG2_MOD - 2))) & 1) << j);
      ++idx;
    }
  }
}


/* arithmetics for submatrix, used in submul() */

/**
 * @brief C = A + B
 */
void submat_add_nn(U16 *C, const U16 *A, const U16 *B){
  U32 i;
  for(i = 0; i < _LOTUS_LWE_DIM * _LOTUS_LWE_DIM / 4; ++i) C[i] = A[i] + B[i];
}

/**
 * @brief C = A + B
 */
void submat_add_nl(U16 *C, const U16 *A, const U16 *B){
  U32 i;
  for(i = 0; i < _LOTUS_LWE_DIM * _LOTUS_LWE_PT / 4; ++i) C[i] = A[i] + B[i];
}

/**
 * @brief C = A - B
 */
void submat_sub_nn(U16 *C, const U16 *A, const U16 *B){
  U32 i;
  for(i = 0; i < _LOTUS_LWE_DIM * _LOTUS_LWE_DIM / 4; ++i) C[i] = A[i] - B[i];
}

/**
 * @brief C = A - B
 */
void submat_sub_nl(U16 *C, const U16 *A, const U16 *B){
  U32 i;
  for(i = 0; i < _LOTUS_LWE_DIM * _LOTUS_LWE_PT / 4; ++i) C[i] = A[i] - B[i];
}

/**
 * @brief C = -(A * B)
 */
void submat_negmul(U16 *C, const U16 *A, const U16 *B){
  const U32 hn = _LOTUS_LWE_DIM / 2, hl = _LOTUS_LWE_PT / 2;
  U32 i, j, k;
  const U16 *p;
  for(i = 0; i < hn; ++i){
    for(j = 0; j < hl; ++j) C[j] = 0;
    p = B;
    for(j = 0; j < hn; ++j){
      for(k = 0; k < hl; ++k){
        C[k] -= A[j] * p[k];
      }
      p += hl;
    }
    A += hn; C += hl;
  }
}

/**
 * @brief C -= A * B
 */
void submat_submul(U16 *C, const U16 *A, const U16 *B){
  const U32 hn = _LOTUS_LWE_DIM / 2, hl = _LOTUS_LWE_PT / 2;
  U32 i, j, k;
  const U16 *p;
  for(i = 0; i < hn; ++i){
    p = B;
    for(j = 0; j < hn; ++j){
      for(k = 0; k < hl; ++k){
        C[k] -= A[j] * p[k];
      }
      p += hl;
    }
    A += hn; C += hl;
  }
}

/**
 * @brief C = -C - (A * B)
 */
void submat_negsubmul(U16 *C, const U16 *A, const U16 *B){
  const U32 hn = _LOTUS_LWE_DIM / 2, hl = _LOTUS_LWE_PT / 2;
  U32 i, j, k;
  const U16 *p;
  for(i = 0; i < hn; ++i){
    for(j = 0; j < hl; ++j) C[j] = _LOTUS_LWE_MOD - C[j];
    p = B;
    for(j = 0; j < hn; ++j){
      for(k = 0; k < hl; ++k){
        C[k] -= A[j] * p[k];
      }
      p += hl;
    }
    A += hn; C += hl;
  }
}


/**
 * @brief change memory align from a row-major matrix to 2x2 row-major sub-matrices
 */
void distribute_2x2_nn(U16 *C, const U16 *A){
  const U32 hn = _LOTUS_LWE_DIM / 2;
  U32 i, j;
  U16 *c11 = C,
    *c12 = c11 + hn * hn,
    *c21 = c12 + hn * hn,
    *c22 = c21 + hn * hn;
  for(i = 0; i < hn; ++i){
    for(j = 0; j < hn; ++j) c11[j] = A[j];
    A += hn; c11 += hn;
    for(j = 0; j < hn; ++j) c12[j] = A[j];
    A += hn; c12 += hn;
  }
  for(i = 0; i < hn; ++i){
    for(j = 0; j < hn; ++j) c21[j] = A[j];
    A += hn; c21 += hn;
    for(j = 0; j < hn; ++j) c22[j] = A[j];
    A += hn; c22 += hn;
  }
}

/**
 * @brief change memory align from a row-major matrix to 2x2 row-major sub-matrices
 */
void distribute_2x2_nl(U16 *C, const U16 *A){
  const U32 hn = _LOTUS_LWE_DIM / 2, hl = _LOTUS_LWE_PT / 2;
  U32 i, j;
  U16 *c11 = C,
    *c12 = c11 + hn * hl,
    *c21 = c12 + hn * hl,
    *c22 = c21 + hn * hl;
  for(i = 0; i < hn; ++i){
    for(j = 0; j < hl; ++j) c11[j] = A[j];
    A += hl; c11 += hl;
    for(j = 0; j < hl; ++j) c12[j] = A[j];
    A += hl; c12 += hl;
  }
  for(i = 0; i < hn; ++i){
    for(j = 0; j < hl; ++j) c21[j] = A[j];
    A += hl; c21 += hl;
    for(j = 0; j < hl; ++j) c22[j] = A[j];
    A += hl; c22 += hl;
  }
}

/**
 * @brief change memory align from 2x2 row-major sub-matrices to a row-major matrix
 */
void merge_2x2_nl(U16 *C, const U16 *A){
  const U32 hn = _LOTUS_LWE_DIM / 2, hl = _LOTUS_LWE_PT / 2;
  U32 i, j;
  const U16 *a11 = A,
    *a12 = a11 + hn * hl,
    *a21 = a12 + hn * hl,
    *a22 = a21 + hn * hl;
  for(i = 0; i < hn; ++i){
    for(j = 0; j < hl; ++j) C[j] = a11[j];
    C += hl; a11 += hl;
    for(j = 0; j < hl; ++j) C[j] = a12[j];
    C += hl; a12 += hl;
  }
  for(i = 0; i < hn; ++i){
    for(j = 0; j < hl; ++j) C[j] = a21[j];
    C += hl; a21 += hl;
    for(j = 0; j < hl; ++j) C[j] = a22[j];
    C += hl; a22 += hl;
  }
}
