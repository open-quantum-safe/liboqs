#ifndef __FIELDS_COMMON_H__
#define __FIELDS_COMMON_H__

/* Needed for primitive types definitions */
#include <stdint.h>

/* Matrices types */
typedef enum {
	REG = 0,
	TRI_SUP = 1,
	TRI_INF = 2,
} matrix_type;

/* Common macro for GF(2) matrix multiplication */
#define GF2_MAT_MULT(A, X, Y, n, mtype, fun) do { \
        uint32_t i, j, shift; \
        /* We perform our multiplication row by row with a vector to vector multiplication \
         * We deal with packed 8 bits \
         */  \
        switch(mtype){ \
                case REG:{ \
                        for(i = 0; i < n; i += 8){ \
                                Y[i / 8] = 0; \
                                for(j = 0; j < 8; j++){ \
                                        Y[i / 8] |= (fun(&A[(n / 8) * (i + j)], X, n) << j); \
                                } \
                        } \
                        break; \
                } \
                case TRI_SUP:{ \
                        for(i = 0; i < n; i += 8){ \
                                Y[i / 8] = 0; \
                                shift = i; \
                                for(j = 0; j < 8; j++){ \
                                        Y[i / 8] |= (fun(&A[((n / 8) * (i + j)) + (shift / 8)], &X[shift / 8], n - shift) << j); \
                                } \
                        } \
                        break; \
                } \
                case TRI_INF:{ \
                        for(i = 0; i < n; i += 8){ \
                                Y[i / 8] = 0; \
                                for(j = 0; j < 8; j++){ \
                                        Y[i / 8] |= (fun(&A[(n / 8) * (i + j)], X, ((i + 8))) << j); \
                                } \
                        } \
                        break; \
                } \
                default:{ \
                        break; \
                } \
        } \
} while(0)

/* Common macro for GF(256) matrix multiplication */
#define GF256_MAT_MULT(A, X, Y, n, mtype, fun) do { \
        uint32_t i; \
        switch(mtype){ \
                case REG:{ \
                        for(i = 0; i < n; i++){ \
                                Y[i] = fun(&A[n * i], X, n); \
                        } \
                        break; \
                } \
                case TRI_SUP:{ \
                        for(i = 0; i < n; i++){ \
                                Y[i] = fun(&A[(n  * i) + i], &X[i], n - i); \
                        } \
                        break; \
                } \
                case TRI_INF:{ \
                        for(i = 0; i < n; i++){ \
                                Y[i] = fun(&A[n * i], X, i + 1); \
                        } \
                        break; \
                } \
                default:{ \
                        break; \
                } \
    } \
} while(0)

/* Common macro for hybrid GF(2) matrix and GF(256) vector matrix multiplication */
/* XXX: TODO: for now, TRI_SUP is set to REG regular multiplication, because the leftover
 * bits in the byte are somehow tedious to extract. This is left for future work to optimize
 * this. */
#define GF2_GF256_MAT_MULT(A, X, Y, n, mtype, fun) do { \
        uint32_t i; \
        switch(mtype){ \
                case TRI_SUP: \
                case REG:{ \
                        for(i = 0; i < n; i++){ \
                                Y[i] = fun(&A[(n * i) / 8], X, n); \
                        } \
                        break; \
                } \
                case TRI_INF:{ \
                        for(i = 0; i < n; i++){ \
                                Y[i] = fun(&A[(n * i) / 8], X, i + 1); \
                        } \
                        break; \
                } \
                default:{ \
                        break; \
                } \
    } \
} while(0)

/* Common macro for hybrid GF(256) matrix and GF(2) vector matrix multiplication */
/* XXX: TODO: for now, TRI_SUP is set to REG regular multiplication, because the leftover
 * bits in the byte are somehow tedious to extract. This is left for future work to optimize
 * this. */
#define GF256_GF2_MAT_MULT(A, X, Y, n, mtype, fun) do { \
        uint32_t i; \
        switch(mtype){ \
        case TRI_SUP: \
                case REG:{ \
                        for(i = 0; i < n; i++){ \
                                Y[i] = fun(&A[n * i], X, n); \
                        } \
                        break; \
                } \
                case TRI_INF:{ \
                        for(i = 0; i < n; i++){ \
                                Y[i] = fun(&A[n * i], X, i + 1); \
                        } \
                        break; \
                } \
                default:{ \
                        break; \
                } \
    } \
} while(0)

/* Matrix multiplication over GF256to2: this is fundamentally the same as GF256 */
#define GF256to2_MAT_MULT GF256_MAT_MULT
#define GF2_GF256to2_MAT_MULT GF2_GF256_MAT_MULT
#define GF256to2_GF2_MAT_MULT GF256_GF2_MAT_MULT
#define GF256_GF4_MAT_MULT GF256_GF2_MAT_MULT
#define GF256to2_GF4_MAT_MULT GF256_GF2_MAT_MULT
#define GF256_GF16_MAT_MULT GF256_GF2_MAT_MULT
#define GF256to2_GF16_MAT_MULT GF256_GF2_MAT_MULT

/* GF(256) matrix transposition */
#define GF256_MAT_TRANSPOSE(A, B, n, mtype) do { \
       uint32_t i, j; \
        /* Zeroize the matrix */ \
        for(i = 0; i < (n * n); i++){ \
                B[i] = 0; \
        } \
        switch(mtype){ \
                case REG:{ \
                        for(i = 0; i < n; i++){ \
                                for(j = 0; j < n; j++){ \
                                        B[(n * j) + i] = A[(n * i) + j]; \
                                } \
                        } \
                        break; \
                } \
                case TRI_SUP:{ \
                        for(i = 0; i < n; i++){ \
                                for(j = i; j < n; j++){ \
                                        B[(n * j) + i] = A[(n * i) + j]; \
                                } \
                        } \
                        break; \
                } \
                case TRI_INF:{ \
                        for(i = 0; i < n; i++){ \
                                for(j = 0; j <= i; j++){ \
                                        B[(n * j) + i] = A[(n * i) + j]; \
                                } \
                        } \
                        break; \
                } \
                default:{ \
                        break; \
                } \
        } \
} while(0)

/* GF(256^2) matrix transposition: same procedure as  */
#define GF256to2_MAT_TRANSPOSE GF256_MAT_TRANSPOSE

#endif /* __FIELDS_COMMON_H__ */
