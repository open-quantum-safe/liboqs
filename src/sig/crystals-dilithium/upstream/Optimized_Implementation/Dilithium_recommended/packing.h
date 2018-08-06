#ifndef PACKING_H
#define PACKING_H

#include "polyvec.h"

void pack_pk(unsigned char pk[PK_SIZE_PACKED],
             const unsigned char rho[SEEDBYTES], const polyveck *t1);
void pack_sk(unsigned char pk[PK_SIZE_PACKED],
             const unsigned char rho[SEEDBYTES],
             const unsigned char key[SEEDBYTES],
             const unsigned char tr[CRHBYTES],
             const polyvecl *s1,
             const polyveck *s2,
             const polyveck *t0);
void pack_sig(unsigned char sig[SIG_SIZE_PACKED],
              const polyvecl *z, const polyveck *h, const poly *c);

void unpack_pk(unsigned char rho[SEEDBYTES], polyveck *t1,
               const unsigned char sk[SK_SIZE_PACKED]);
void unpack_sk(unsigned char rho[SEEDBYTES],
               unsigned char key[SEEDBYTES],
               unsigned char tr[CRHBYTES],
               polyvecl *s1,
               polyveck *s2,
               polyveck *t0,
               const unsigned char sk[SK_SIZE_PACKED]);
void unpack_sig(polyvecl *z, polyveck *h, poly *c,
                const unsigned char sig[SIG_SIZE_PACKED]);

#endif
