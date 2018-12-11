/** Optimized ThreeBears implementation */
#include "common.h"
#include "shake.h"
#include "params.h"
#include "ring.h"
#include "threebears.h"

#define FEC_BYTES ((FEC_BITS+7)/8)
#define ENC_BITS  (ENC_SEED_BYTES*8 + FEC_BITS)
#if FEC_BITS
#include "melas_fec.h"
#endif

/* Securely erase size bytes from s */
#ifdef __STDC_LIB_EXT1__
void WEAK secure_bzero (void *s,size_t size) { memset_s(s,size,0,size); }
#else
void WEAK __attribute__((noinline)) secure_bzero (void *s,size_t size) { memset(s,0,size); }
#endif

enum { HASH_PURPOSE_UNIFORM=0, HASH_PURPOSE_KEYGEN=1, HASH_PURPOSE_ENCAPS=2 };

/** Initialize the hash function with a given purpose */
static void threebears_hash_init(hash_ctx_t ctx, uint8_t purpose) {
    threebears_cshake_init(ctx);
    const uint8_t pblock[15] ={
         VERSION, PRIVATE_KEY_BYTES, MATRIX_SEED_BYTES, ENC_SEED_BYTES,
         IV_BYTES, SHARED_SECRET_BYTES, LGX, DIGITS&0xFF, DIGITS>>8, DIM,
         VAR_TIMES_128-1, LPR_BITS, FEC_BITS, CCA, 0 /* padding */
    };
    hash_update(ctx,(const uint8_t*)pblock,sizeof(pblock));
    hash_update(ctx,&purpose,1);
}

/** Return at least 8 bits of a starting at the b'th bit */
static inline limb_t bits_starting_at(const gf_t a, unsigned b) {
    unsigned pos = b/LBITS;
    limb_t ret = a[pos] >> (b%LBITS);
    if (pos < NLIMBS-1) ret |= a[pos+1] << (LBITS-(b%LBITS));
    return ret;
}

/** Sample n gf_t's uniformly from a seed */
static void uniform(
    gf_t *matrix,
    const uint8_t *seed,
    uint8_t iv,
    unsigned n
) {
    hash_ctx_t ctx;
    threebears_hash_init(ctx,HASH_PURPOSE_UNIFORM);
    hash_update(ctx,seed,MATRIX_SEED_BYTES);
    uint8_t *c = (uint8_t *) (&matrix[0][NLIMBS]) - GF_BYTES;
    hash_times_n(c,GF_BYTES,sizeof(gf_t),ctx,iv,n);
    for (unsigned i=0; i<n; i++) {
        expand(matrix[i],(uint8_t *)(&matrix[i][NLIMBS]) - GF_BYTES);
    }
}

/** The three bears error distribution */
static slimb_t psi(uint8_t ci) {
    int sample=0, var=VAR_TIMES_128;
    for (; var > 64; var -= 64, ci<<=2) {
        sample += ((ci+64)>>8) + ((ci-64)>>8);
    }
    return sample + ((ci+var)>>8) + ((ci-var)>>8);
}

/** Sample a vector of n noise elements */
static void noise(
    gf_t *vector,
    const hash_ctx_t ctx,
    unsigned iv,
    uint8_t n
) {
    uint8_t *c0 = (uint8_t *)(&vector[0][NLIMBS]) - DIGITS;
    hash_times_n(c0,DIGITS,sizeof(gf_t),ctx,iv,n);

    for (unsigned k=0; k<n; k++) {
        limb_t *l = vector[k];
        uint8_t *c = (uint8_t *)(&l[NLIMBS]) - DIGITS;

        unsigned i,j=0,s=0;
        slimb_t buffer=0;
        for (i=0; i<DIGITS; i++) {
            buffer += psi(c[i])<<s;
            s += LGX;
            if (s >= LBITS) {
                l[j] = buffer + modulus(j);
                buffer = 0;
                j++;
                s -= LBITS;
            }
        }
    }
}

void get_pubkey(uint8_t *pk, const uint8_t *seed) {
    hash_ctx_t ctx;
    threebears_hash_init(ctx,HASH_PURPOSE_KEYGEN);
    hash_update(ctx,seed,PRIVATE_KEY_BYTES);
    
    {
        hash_ctx_t ctx2;
        memcpy(ctx2,ctx,sizeof(ctx2));
        hash_output(ctx2,pk,MATRIX_SEED_BYTES);
        hash_destroy(ctx2);
    }
    
    {
    #if VECLEN > 1
        gf_t sk_expanded[DIM*2],matrix[DIM*DIM];
        noise(sk_expanded,ctx,0,2*DIM);
        uniform(matrix,pk,0,DIM*DIM);
        for (unsigned i=0; i<DIM; i++) {
            for (unsigned j=0; j<DIM; j++) {
                mac(sk_expanded[DIM+i],matrix[i+DIM*j],sk_expanded[j]);
            }
            contract(&pk[MATRIX_SEED_BYTES+i*GF_BYTES], sk_expanded[DIM+i]);
        }
        hash_destroy(ctx);
    #else
        gf_t sk_expanded[DIM],b,c;
        for (unsigned i=0; i<DIM; i++) {
            noise(&sk_expanded[i],ctx,i,1);
        }
        for (unsigned i=0; i<DIM; i++) {
            noise(&c,ctx,i+DIM,1);
            for (unsigned j=0; j<DIM; j++) {
                uniform(&b,pk,i+DIM*j,1);
                mac(c,b,sk_expanded[j]);
            }
            contract(&pk[MATRIX_SEED_BYTES+i*GF_BYTES], c);
        }
        hash_destroy(ctx);
        secure_bzero(b,sizeof(b));
        secure_bzero(c,sizeof(c));
    #endif
    }
}

void encapsulate(
    uint8_t *shared_secret,
    uint8_t *capsule,
    const uint8_t *pk,
    const uint8_t *seed
) {
    uint8_t *lpr = &capsule[GF_BYTES*DIM];
#if IV_BYTES
    uint8_t *iv = &lpr[(ENC_BITS*LPR_BITS+7)/8];
    memcpy(iv,&seed[ENC_SEED_BYTES],IV_BYTES);
#endif
    
    hash_ctx_t ctx;
    threebears_hash_init(ctx,HASH_PURPOSE_ENCAPS);
    hash_update(ctx,pk,MATRIX_SEED_BYTES);
    hash_update(ctx,seed,ENC_SEED_BYTES + IV_BYTES);
    
#if VECLEN > 1
    gf_t sk_expanded[2*DIM+1],matrix[DIM*DIM];
    limb_t *b=sk_expanded[2*DIM-1], *c=sk_expanded[2*DIM];
    noise(sk_expanded,ctx,0,2*DIM+1);
    uniform(matrix,pk,0,DIM*DIM);
    for (unsigned i=0; i<DIM; i++) {
        for (unsigned j=0; j<DIM; j++) {
            mac(sk_expanded[DIM+i],matrix[j+DIM*i],sk_expanded[j]);
        }
        contract(&capsule[i*GF_BYTES], sk_expanded[DIM+i]);
    }
#else
    gf_t sk_expanded[DIM],b,c;
    for (unsigned i=0; i<DIM; i++) {
        noise(&sk_expanded[i],ctx,i,1);
    }
    for (unsigned i=0; i<DIM; i++) {
        noise(&c,ctx,i+DIM,1);
        for (unsigned j=0; j<DIM; j++) {
            uniform(&b,pk,j+DIM*i,1);
            mac(c,b,sk_expanded[j]);
        }
        contract(&capsule[i*GF_BYTES], c);
    }
    noise(&c,ctx,2*DIM,1);
#endif
    
    /* Calculate approximate shared secret */
    for (unsigned i=0; i<DIM; i++) {
        expand(b, &pk[MATRIX_SEED_BYTES+i*GF_BYTES]);
        mac(c,b,sk_expanded[i]);
    }
    canon(c);
    
#if !CCA
    uint8_t *seed1 = &lpr[(ENC_BITS*LPR_BITS+7)/8-ENC_SEED_BYTES];
    hash_output(ctx,seed1,ENC_SEED_BYTES);
    seed = seed1;
    threebears_hash_init(ctx,HASH_PURPOSE_ENCAPS);
    hash_update(ctx,pk,MATRIX_SEED_BYTES);
    hash_update(ctx,seed,ENC_SEED_BYTES);
#if IV_BYTES
    hash_update(ctx,iv,IV_BYTES);
#endif
#endif

#if FEC_BITS
    uint8_t fec[MELAS_FEC_BYTES];
    melas_fec_set(fec,seed,ENC_SEED_BYTES);
#endif
    
    /* Export with rounding */
    for (unsigned i=0; i<ENC_BITS; i+=2) {
#if FEC_BITS
        limb_t h = ((i/8<ENC_SEED_BYTES) ? seed[i/8] : fec[i/8-ENC_SEED_BYTES]) >> (i%8);
#else
        limb_t h = seed[i/8] >> (i%8);
#endif
        unsigned rlimb0 = bits_starting_at(c,(i/2+1)      * LGX-LPR_BITS) + (h<<3);
        unsigned rlimb1 = bits_starting_at(c,(DIGITS-i/2) * LGX-LPR_BITS) + ((h>>1)<<3);
        lpr[i/2] = (rlimb0 & 0xF) | rlimb1<<4;
    }

    hash_output(ctx,shared_secret,SHARED_SECRET_BYTES);
    
    /* Clean up */
    hash_destroy(ctx);
    secure_bzero(sk_expanded,sizeof(sk_expanded));
#if VECLEN == 1
    secure_bzero(b,sizeof(b));
    secure_bzero(c,sizeof(c));
#endif
}

static void decrypt_seed(uint8_t *seed, gf_t residue, const uint8_t *lpr) {
#if FEC_BITS
    uint8_t fec[MELAS_FEC_BYTES];
#endif
    /* Add in LPR data */
    canon(residue);
    unsigned rounding = 1<<(LPR_BITS-1), out=0;
    for (signed i=ENC_BITS-1; i>=0; i--) {
        unsigned j = (i&1) ? (int)(DIGITS-i/2) : i/2+1;
        unsigned our_rlimb = bits_starting_at(residue,j*LGX-LPR_BITS-1);
        unsigned their_rlimb = lpr[i*LPR_BITS/8] >> ((i*LPR_BITS) % 8);
        unsigned delta =  their_rlimb*2 - our_rlimb + rounding;
        out |= ((delta>>LPR_BITS) & 1)<<(i%8);
        if (i%8==0) {
#if FEC_BITS
            if ((unsigned)i/8<ENC_SEED_BYTES) {
                seed[i/8] = out;
            } else {
                fec[i/8-ENC_SEED_BYTES] = out;
            }
#else
            seed[i/8] = out;
#endif
            out = 0;
        }
    }
#if FEC_BITS
    melas_fec_correct(seed,ENC_SEED_BYTES,fec);
#endif
}

#if CCA
static unsigned memeq (
   const void *data1_,
   const void *data2_,
   size_t size
) {
    const unsigned char *data1 = (const unsigned char *)data1_;
    const unsigned char *data2 = (const unsigned char *)data2_;
    unsigned char ret = 0;
    for (; size; size--, data1++, data2++) {
        ret |= *data1 ^ *data2;
    }
    return ((int)ret - 1) >> 8;
}

int decapsulate(
    uint8_t *shared_secret,
    const uint8_t *capsule,
    const uint8_t *sk
) { 
    const uint8_t *lpr = &capsule[GF_BYTES*DIM];
    
    /* Calculate approximate shared secret */
    hash_ctx_t ctx, ctx2;
    threebears_hash_init(ctx,HASH_PURPOSE_KEYGEN);
    hash_update(ctx,sk,PRIVATE_KEY_BYTES);
    
    union { gf_t gf; uint8_t buf[GF_BYTES]; } a;
#if VECLEN>1
    gf_t ska[(DIM+1)*DIM],skb[2*DIM+1],*matrix = &ska[DIM];
    limb_t *b=skb[2*DIM-1], *d = skb[2*DIM], *c=d;
    memset(d,0,sizeof(gf_t));
    noise(ska,ctx,0,2*DIM);
    for (unsigned i=0; i<DIM; i++) {
        expand(b,&capsule[i*GF_BYTES]);
        mac(d,ska[i],b);
    }
#else
    gf_t ska[DIM],skb[DIM],b,c,d;
    memset(c,0,sizeof(gf_t));
    noise(ska,ctx,0,DIM);
    for (unsigned i=0; i<DIM; i++) {
        expand(b,&capsule[i*GF_BYTES]);
        mac(c,ska[i],b);
    }
#endif

    assert(ENC_SEED_BYTES <= SHARED_SECRET_BYTES);
    uint8_t *seed = shared_secret;
    decrypt_seed(seed,c,lpr);
    unsigned ok=-(unsigned)1;
    
    /* Derive the matrix seed */
    uint8_t ms[MATRIX_SEED_BYTES];
    memcpy(ctx2,ctx,sizeof(ctx));
    hash_output(ctx,ms,sizeof(ms));
    threebears_hash_init(ctx,HASH_PURPOSE_ENCAPS);
    hash_update(ctx,ms,sizeof(ms));
    hash_update(ctx,seed,ENC_SEED_BYTES);
#if IV_BYTES
    const uint8_t *iv = &lpr[(ENC_BITS*LPR_BITS+7)/8];
    hash_update(ctx,iv,IV_BYTES);
#endif

#if VECLEN > 1
    noise(skb,ctx,0,2*DIM+1);
    for (unsigned j=0; j<DIM; j++) {
        mac(d,ska[DIM+j],skb[j]);
    }
    uniform(matrix,ms,0,DIM*DIM);
    for (unsigned i=0; i<DIM; i++) {
        memset(a.gf,0,sizeof(a.gf));
        for (unsigned j=0; j<DIM; j++) {
            mac(a.gf,matrix[j+DIM*i],skb[j]);
        }

        mac(d,ska[i],a.gf);
        for (unsigned j=0; j<NLIMBS; j++) a.gf[j] += skb[DIM+i][j];
        contract(a.buf,a.gf);
        ok &= memeq(a.buf,&capsule[i*GF_BYTES],GF_BYTES);
    }
#else
    noise(&d,ctx,2*DIM,1);
    noise(skb,ctx,0,DIM);
    for (unsigned j=0; j<DIM; j++) {
        noise(&b,ctx2,j+DIM,1);
        mac(d,b,skb[j]);
    }
    for (unsigned i=0; i<DIM; i++) {
        memset(a.gf,0,sizeof(a.gf));
        for (unsigned j=0; j<DIM; j++) {
            uniform(&b,ms,j+DIM*i,1);
            mac(a.gf,b,skb[j]);
        }
        mac(d,ska[i],a.gf);
        noise(&b,ctx,i+DIM,1);
        for (unsigned j=0; j<NLIMBS; j++) a.gf[j] += b[j];
        contract(a.buf,a.gf);
        ok &= memeq(a.buf,&capsule[i*GF_BYTES],GF_BYTES);
    }
#endif

    /* Calculate approximate shared secret */
    canon(d);
#if FEC_BITS
    uint8_t fec[MELAS_FEC_BYTES];
    melas_fec_set(fec,seed,ENC_SEED_BYTES);
#endif
    /* Check export with rounding */
    uint8_t wrong=0;
    for (unsigned i=0; i<ENC_BITS; i+=2) {
#if FEC_BITS
        limb_t h = (i/8<ENC_SEED_BYTES ? seed[i/8] : fec[i/8-ENC_SEED_BYTES]) >> (i%8);
#else
        limb_t h = seed[i/8] >> (i%8);
#endif
        unsigned rlimb0 = bits_starting_at(d,(i/2+1)      * LGX-LPR_BITS) + (h<<3);
        unsigned rlimb1 = bits_starting_at(d,(DIGITS-i/2) * LGX-LPR_BITS) + ((h>>1)<<3);
        unsigned char c = (rlimb0 & 0xF) | rlimb1<<4;
        wrong |= c^lpr[i/2];
    }
    ok &= ((int)wrong - 1) >> 8;

    hash_output(ctx,shared_secret,SHARED_SECRET_BYTES);
    for (unsigned i=0; i<SHARED_SECRET_BYTES; i++) {
        shared_secret[i] &= ok;
    }

    /* Clean up */
    secure_bzero(&a,sizeof(a));
#if VECLEN>1
    secure_bzero(ska,sizeof(ska));
    secure_bzero(skb,sizeof(skb));
#else
    secure_bzero(ska,sizeof(ska));
    secure_bzero(skb,sizeof(skb));
    secure_bzero(b,sizeof(b));
    secure_bzero(c,sizeof(c));
    secure_bzero(d,sizeof(d));
#endif
    hash_destroy(ctx);
    hash_destroy(ctx2);
    return ~ok;
}
#else /* !CCA */
int decapsulate(
    uint8_t *shared_secret,
    const uint8_t *capsule,
    const uint8_t *sk
) {
    const uint8_t *lpr = &capsule[GF_BYTES*DIM];
    
    /* Calculate approximate shared secret */
    hash_ctx_t ctx;
    threebears_hash_init(ctx,HASH_PURPOSE_KEYGEN);
    hash_update(ctx,sk,PRIVATE_KEY_BYTES);
    
    gf_t c={0};
    {
#if VECLEN == 1
        gf_t ska,b;
        for (unsigned i=0; i<DIM; i++) {
            expand(b,&capsule[i*GF_BYTES]);
            noise(&ska,ctx,i,1);
            mac(c,ska,b);
        }
#else
        gf_t ska[DIM],b;
        noise(ska,ctx,0,DIM);
        for (unsigned i=0; i<DIM; i++) {
            expand(b,&capsule[i*GF_BYTES]);
            mac(c,ska[i],b);
        }
#endif
        secure_bzero(ska,sizeof(ska));
        secure_bzero(b,sizeof(b));
    }

    assert(MATRIX_SEED_BYTES <= SHARED_SECRET_BYTES);
    uint8_t *seed = shared_secret;
    hash_output(ctx,seed,MATRIX_SEED_BYTES);
    threebears_hash_init(ctx,HASH_PURPOSE_ENCAPS);
    hash_update(ctx,seed,MATRIX_SEED_BYTES);

    assert(ENC_SEED_BYTES <= SHARED_SECRET_BYTES);
    decrypt_seed(seed,c,lpr);
    hash_update(ctx,seed,ENC_SEED_BYTES); 
#if IV_BYTES
    hash_update(ctx,&lpr[(ENC_BITS*LPR_BITS+7)/8],IV_BYTES);
#endif
    hash_output(ctx,shared_secret,SHARED_SECRET_BYTES);

    hash_destroy(ctx);
    secure_bzero(c,sizeof(c));
    return 0;
}
#endif

