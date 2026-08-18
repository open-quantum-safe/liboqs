#ifndef POLY_H
#define POLY_H

#include "params.h"

#include <stddef.h>
#include <stdint.h>

#define MODQ(X) ((X) & (NTRU_Q-1))

typedef struct {
    uint16_t coeffs[NTRU_N];
} poly;

void PQCLEAN_NTRUHRSS701_CLEAN_poly_mod_3_Phi_n(poly *r);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_mod_q_Phi_n(poly *r);

void PQCLEAN_NTRUHRSS701_CLEAN_poly_Sq_tobytes(unsigned char *r, const poly *a);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_Sq_frombytes(poly *r, const unsigned char *a);

void PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_sum_zero_tobytes(unsigned char *r, const poly *a);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_sum_zero_frombytes(poly *r, const unsigned char *a);

void PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_tobytes(unsigned char msg[NTRU_PACK_TRINARY_BYTES], const poly *a);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_frombytes(poly *r, const unsigned char msg[NTRU_PACK_TRINARY_BYTES]);

void PQCLEAN_NTRUHRSS701_CLEAN_poly_Sq_mul(poly *r, const poly *a, const poly *b);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_mul(poly *r, const poly *a, const poly *b);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_mul(poly *r, const poly *a, const poly *b);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_lift(poly *r, const poly *a);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_to_S3(poly *r, const poly *a);

void PQCLEAN_NTRUHRSS701_CLEAN_poly_R2_inv(poly *r, const poly *a);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_inv(poly *r, const poly *a);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_S3_inv(poly *r, const poly *a);

void PQCLEAN_NTRUHRSS701_CLEAN_poly_Z3_to_Zq(poly *r);
void PQCLEAN_NTRUHRSS701_CLEAN_poly_trinary_Zq_to_Z3(poly *r);
#endif
