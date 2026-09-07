// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file ov.c
/// @brief The standard implementations for functions in ov.h
///
#include "params.h"

#include "ov_keypair.h"

#include "ov.h"

#include "blas.h"

#include "ov_blas.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utils_prng.h"
#include "utils_hash.h"
#include "utils_malloc.h"


#define MAX_ATTEMPT_VINEGAR  256

/////////////////////////////

#if defined(_VALGRIND_)
#include "valgrind/memcheck.h"
#endif


#if defined(_OV_PKC_SKC)
/// @brief  mat_l1 = v^Tr * S , with S = (P1 + P1^Tr) O + P2 left implicit.
///
/// @param[out] mat_l1   - the matrix of the linear system.
/// @param[in]  P1       - P1 of the pk.
/// @param[in]  P2       - P2 of the pk.
/// @param[in]  O        - O of the sk.
/// @param[in]  vinegar  - the vinegar vector.
///
static
void l1_mat_from_p2( uint8_t *mat_l1, const uint8_t *P1, const uint8_t *P2,
                     const uint8_t *O, const uint8_t *vinegar ) {
    #if defined(_MUL_WITH_MULTAB_)
    PQOV_ALIGN uint8_t vinegar_multabs[_V * 32];
    gfv_generate_multabs( vinegar_multabs, vinegar, _V );
    gfmat_prod_multab( mat_l1, P2, _O * _O_BYTE, _V, vinegar_multabs );
    #else
    gfmat_prod( mat_l1, P2, _O * _O_BYTE, _V, vinegar );
    #endif

    PQOV_ALIGN uint8_t p1_vinegar[_V * _O_BYTE];
    gf256v_set_zero( p1_vinegar, sizeof(p1_vinegar) );
    #if defined(_MUL_WITH_MULTAB_)
    batch_2trimat_madd_multab( p1_vinegar, P1, vinegar_multabs, _V, _V_BYTE, 1, _O_BYTE );
    #else
    batch_2trimat_madd( p1_vinegar, P1, vinegar, _V, _V_BYTE, 1, _O_BYTE );
    #endif

    for (unsigned i = 0; i < _O; i++) {
        const uint8_t *o_col = O + i * _V_BYTE;
        uint8_t *mat_col = mat_l1 + i * _O_BYTE;
        for (unsigned j = 0; j < _V; j++) {
            gfv_madd( mat_col, p1_vinegar + j * _O_BYTE, gfv_get_ele( o_col, j ), _O_BYTE );
        }
    }
}

// expand_sk_implicit_s() leaves P2, not S, in sk->S.
static inline
void ov_calc_l1_mat( uint8_t *mat_l1, const sk_t *sk, const uint8_t *vinegar ) {
    l1_mat_from_p2( mat_l1, sk->P1, sk->S, sk->O, vinegar );
}

#else
/// @brief  mat_l1 = v^Tr * S .
///
/// @param[out] mat_l1   - the matrix of the linear system.
/// @param[in]  S        - S of the sk.
/// @param[in]  vinegar  - the vinegar vector.
///
static
void l1_mat_from_s( uint8_t *mat_l1, const uint8_t *S, const uint8_t *vinegar ) {
    gfmat_prod( mat_l1, S, _O * _O_BYTE, _V, vinegar );
}

static inline
void ov_calc_l1_mat( uint8_t *mat_l1, const sk_t *sk, const uint8_t *vinegar ) {
    l1_mat_from_s( mat_l1, sk->S, vinegar );
}

#endif


static
int ov_sign_core( uint8_t *signature, const sk_t *sk, const uint8_t *message, size_t mlen ) {
    // allocate temporary storage.
    uint8_t mat_l1[_O * _O_BYTE];
    uint8_t salt[_SALT_BYTE];
    randombytes( salt, _SALT_BYTE );
    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_UNDEFINED(salt, _SALT_BYTE );  // mark secret data as undefined data
    VALGRIND_MAKE_MEM_UNDEFINED(sk, OV_SECRETKEYBYTES );  // mark secret data as undefined data
    #endif

    uint8_t vinegar[_V_BYTE];
    uint8_t r_l1_F1[_O_BYTE];
    uint8_t y[_PUB_N_BYTE];
    uint8_t x_o1[_O_BYTE];

    hash_ctx h_m_salt_secret;
    hash_ctx h_vinegar_copy;
    // The computation:  H(M||salt)  -->   y  --C-map-->   x   --T-->   w
    hash_init  (&h_m_salt_secret);
    hash_update(&h_m_salt_secret, message, mlen);
    hash_update(&h_m_salt_secret, salt, _SALT_BYTE);
    hash_ctx_copy(&h_vinegar_copy, &h_m_salt_secret);
    hash_final_digest( y, _PUB_M_BYTE, &h_m_salt_secret);    // H(M||salt)

    hash_update(&h_vinegar_copy, sk->sk_seed, LEN_SKSEED );   // H(M||salt||sk_seed ...

    unsigned n_attempt = 0;
    while ( MAX_ATTEMPT_VINEGAR > n_attempt ) {
        uint8_t ctr = n_attempt & 0xff;                    // counter for generating vinegar
        n_attempt++;
        hash_ctx h_vinegar;
        hash_ctx_copy(&h_vinegar, &h_vinegar_copy);
        hash_update(&h_vinegar, &ctr, 1 );                  // H(M||salt||sk_seed||ctr ...
        hash_final_digest( vinegar, _V_BYTE, &h_vinegar);   // H(M||salt||sk_seed||ctr)

        #if defined(_VALGRIND_)
        VALGRIND_MAKE_MEM_UNDEFINED(vinegar, _V_BYTE );  // mark secret data as undefined data
        #endif

// generate linear system:
// matrix
        ov_calc_l1_mat( mat_l1, sk, vinegar );
// constant
        // Given vinegars, evaluate P1 with the vinegars
        batch_quad_trimat_eval( r_l1_F1, sk->P1, vinegar, _V, _O_BYTE );
        gf256v_add( r_l1_F1, y, _O_BYTE );      // substract the contribution from vinegar variables

// solve linear system:
        #if _GFSIZE == 256
        unsigned l1_succ = gf256mat_gaussian_elim(mat_l1, r_l1_F1, _O);
        #if defined(_VALGRIND_)
        VALGRIND_MAKE_MEM_DEFINED(&l1_succ, sizeof(unsigned) );  // this is reject sampling
        #endif
        if ( !l1_succ ) {
            continue;
        }
        gf256mat_back_substitute(r_l1_F1, mat_l1, _O);
        memcpy( x_o1, r_l1_F1, _O_BYTE );
        #elif _GFSIZE == 16
        unsigned l1_succ = gf16mat_gaussian_elim(mat_l1, r_l1_F1, _O);
        #if defined(_VALGRIND_)
        VALGRIND_MAKE_MEM_DEFINED(&l1_succ, sizeof(unsigned) );  // this is reject sampling
        #endif
        if ( !l1_succ ) {
            continue;
        }
        gf16mat_back_substitute(r_l1_F1, mat_l1, _O);
        memcpy( x_o1, r_l1_F1, _O_BYTE );
        #else
        error -- _GFSIZE
        #endif
        break;
    }
    hash_final_digest( NULL, 0, &h_vinegar_copy);   // free
    if ( MAX_ATTEMPT_VINEGAR <= n_attempt ) {
        return -1;
    }

    //  w = T^-1 * x
    uint8_t *w = signature;    // [_PUB_N_BYTE];
    // identity part of T.
    memcpy( w, vinegar, _V_BYTE );
    memcpy( w + _V_BYTE, x_o1, _O_BYTE );

    // Computing the O part of T.
    gfmat_prod(y, sk->O, _V_BYTE, _O, x_o1 );
    gf256v_add(w, y, _V_BYTE );

    // return: signature <- w || salt.
    memcpy( signature + _PUB_N_BYTE, salt, _SALT_BYTE );

    return 0;
}


#if !defined(_OV_PKC_SKC)
int ov_sign( uint8_t *signature, const sk_t *sk, const uint8_t *message, size_t mlen ) {
    return ov_sign_core( signature, sk, message, mlen );
}
#endif


static
int _ov_verify( const uint8_t *message, size_t mlen, const uint8_t *salt, const unsigned char *digest_ck ) {
    unsigned char correct[_PUB_M_BYTE];
    hash_ctx hctx;
    hash_init(&hctx);
    hash_update(&hctx, message, mlen);
    hash_update(&hctx, salt, _SALT_BYTE);
    hash_final_digest(correct, _PUB_M_BYTE, &hctx);  // H( message || salt )

    // check consistency.
    unsigned char cc = 0;
    for (unsigned i = 0; i < _PUB_M_BYTE; i++) {
        cc |= (digest_ck[i] ^ correct[i]);
    }
    return (0 == cc) ? 0 : -1;
}




#if !(defined(_OV_PKC) || defined(_OV_PKC_SKC)) || !defined(_SAVE_MEMORY_)
int ov_verify( const uint8_t *message, size_t mlen, const uint8_t *signature, const pk_t *pk ) {
    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_DEFINED(signature, OV_SIGNATUREBYTES );  // mark signature as public data
    #endif
    unsigned char digest_ck[_PUB_M_BYTE];
    ov_publicmap( digest_ck, pk->pk, signature );

    return _ov_verify( message, mlen, signature + _PUB_N_BYTE, digest_ck );
}
#endif


#if defined(_OV_PKC) || defined(_OV_PKC_SKC)
#if !defined(PQM4)
#define _MALLOC_
#endif

#if defined(_OV_PKC_SKC)
int ov_expand_and_sign( uint8_t *signature, const csk_t *csk, const uint8_t *message, size_t mlen ) {
    #ifdef _MALLOC_
    sk_t *sk = ov_malloc(sizeof(sk_t));
    if (NULL == sk) {
        return -1;
    }
    #else
    sk_t _sk;
    sk_t *sk = &_sk;
    #endif

    expand_sk_implicit_s( sk, csk->sk_seed );    // S stays implicit; sk->S holds P2.

    int r = ov_sign_core( signature, sk, message, mlen );

    #ifdef _MALLOC_
    ov_free(sk, sizeof(sk_t));
    #endif

    return r;
}
#endif

int ov_expand_and_verify( const uint8_t *message, size_t mlen, const uint8_t *signature, const cpk_t *cpk ) {

    #ifdef _SAVE_MEMORY_
    unsigned char digest_ck[_PUB_M_BYTE];
    ov_publicmap_pkc( digest_ck, cpk, signature );
    return _ov_verify( message, mlen, signature + _PUB_N_BYTE, digest_ck );
    #else
    int rc;

    #ifdef _MALLOC_
    pk_t *pk = ov_malloc(sizeof(pk_t));
    if (NULL == pk) {
        return -1;
    }
    #else
    pk_t _pk;
    pk_t *pk = &_pk;
    #endif

    #if _GFSIZE == 16  && (defined(_BLAS_NEON_) || defined(_BLAS_M4F_))
    uint8_t xi[_PUB_N];
    for (int i = 0; i < _PUB_N; i++) {
        xi[i] = gfv_get_ele( signature, i );
    }
    expand_pk_predicate( pk, cpk, xi );
    #else
    expand_pk( pk, cpk );
    #endif
    rc = ov_verify( message, mlen, signature, pk );


    #ifdef _MALLOC_
    ov_free(pk, sizeof(pk_t));
    #endif
    return rc;
    #endif
}
#endif


