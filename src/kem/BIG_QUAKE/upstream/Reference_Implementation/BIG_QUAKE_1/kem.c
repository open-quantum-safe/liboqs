#include <stdlib.h>
#include <string.h>
#include "kem.h"
#include "rng.h"
#include "api.h"
#include "goppa.h"
#include "m2e.h"

/* Keccac function */
void FIPS202_SHA3_256(const unsigned char *input, unsigned int inputByteLen, unsigned char *output);

/**
   Store in a the bitwise xor of vectors a and b.
 */
void xor(unsigned char * a, unsigned char * b, int length) {
    int i;
    for (i = 0; i < length; i++) 
        a[i] ^= b[i]; 
}

/**
   Shift a column of the parity-check matrix d times. The input column is 'in'; 
   the output 'out'.
 */
void shift_cols(unsigned char * in, unsigned char * out, int d) {
    int i, i_out;
    
    memset(out, 0, BITS_TO_BYTES(CODIMENSION));
        
    for(i = 0; i < CODIMENSION ; i++) {
	    if(ind(in, i)){
	        i_out = ((i/ORDER)*ORDER + (i + d)%ORDER);
	        out[i_out/8] ^= (1 << (i_out % 8));
	    }
    }
}

/**
   Store in 'syndrome' an encypted version of 'e' under public key 'pk',
   using Niederreiter cryptosysystem.
 */
int encrypt_nied(OUT unsigned char *syndrome, IN int * e, unsigned char * pk) {

    int i;
    unsigned char *perm_cols, *syn_aux;
    
    perm_cols = malloc(BITS_TO_BYTES(CODIMENSION)*sizeof(unsigned char));
    syn_aux = malloc(BITS_TO_BYTES(CODIMENSION)*sizeof(unsigned char));
    

    memset(perm_cols, 0, BITS_TO_BYTES(CODIMENSION));
    memset(syn_aux, 0, BITS_TO_BYTES(CODIMENSION));
    
    for (i = 0; i < NB_ERRORS; ++i) {

        if (e[i] >= DIMENSION) {
            syn_aux[(e[i]-DIMENSION) / 8]^= (1 << ((e[i]-DIMENSION) % 8));
        }
        else {
        	shift_cols(pk + (e[i]/ORDER)*BITS_TO_BYTES(CODIMENSION), perm_cols, e[i]%ORDER);
        	addto(syn_aux, perm_cols);
        }
    }
    
    memcpy(syndrome, syn_aux, SYNDROME_BYTES);

    free(perm_cols);
    free(syn_aux);
    
    return SUCCESS;
}


/**
   Store in 'error' a decryption of 'syndrome', using secret key 'sk'.
 */
int decrypt_nied(IN unsigned char *syndrome, OUT int * error, IN unsigned char * sk) {
    int i;
    goppa_t gamma;
    
    //Initialisation of gamma from sk
    gf_init(EXT_DEGREE);
    
    //Support L
    gfelt_t * L = (gfelt_t *) sk;
    
    //Polynome g
    sk += LENGTH * sizeof (gfelt_t);    
    poly_t g = poly_alloc_from_string(NB_ERRORS, sk);
    poly_set_deg(g, NB_ERRORS);
    
    //Polynome sqrtzmod
    sk += (NB_ERRORS + 1) * sizeof (gfelt_t);
    poly_t sqrtzmod = poly_alloc_from_string(NB_ERRORS - 1, sk);
    poly_calcule_deg(sqrtzmod);
    
    //Goppa code gamma
    gamma = goppa_init(L, g, LENGTH, NB_ERRORS, ORDER, sqrtzmod);

    //Decode
    i = goppa_decode(syndrome, error, gamma);
    
    free(gamma->Linv);
    free(gamma);
    free(sqrtzmod);
    free(g);

    if (i<0)
       return FAIL;
    return SUCCESS;
}


/**
   Key generation
     - pk is the public key,
     - sk is the private key.
*/
int crypto_kem_keypair(OUT unsigned char *pk, OUT unsigned char *sk) {

    goppa_t gamma;

    memset(pk, 0, PUBLICKEY_BYTES);
    gamma = goppa_keygen_rand(LENGTH, ORDER, EXT_DEGREE, NB_ERRORS, &pk);
    gamma->sqrtzmod = poly_sqrtmod(gamma->g);

    memcpy(sk, gamma->L, LENGTH * sizeof (gfelt_t));
    sk += LENGTH * sizeof (gfelt_t);
    
    memcpy(sk, gamma->g->coeff, (NB_ERRORS + 1) * sizeof (gfelt_t));
    sk += (NB_ERRORS + 1) * sizeof (gfelt_t);

    memcpy(sk, gamma->sqrtzmod->coeff, NB_ERRORS * sizeof (gfelt_t));
    sk += NB_ERRORS * sizeof (gfelt_t);

    free_goppa(gamma);
    
    
    return SUCCESS;
}



/**
   Encapsulation: 
     - pk is the public key,
     - ct is a key encapsulation message (ciphertext),
     - ss is the shared secret.
*/
int crypto_kem_enc(OUT unsigned char *ct, OUT unsigned char *ss, IN unsigned char *pk) {

    int i;
    
    //Random bits sequence m
    unsigned char* m = malloc(RANDOM_BYTES * sizeof(unsigned char));
    randombytes(m, RANDOM_BYTES);
    
    //Construct an error e from m
    int *error = (int *) malloc(NB_ERRORS*sizeof(int));
    m2error(m, error);
    
    //Encrypt e (Niederrieter)
    unsigned char *syndrome = malloc(SYNDROME_BYTES*sizeof(unsigned char));
    encrypt_nied(syndrome, error, (unsigned char *) pk);
    
    //m XOR Hash(e)
    unsigned char * e = calloc(BITS_TO_BYTES(LENGTH),sizeof(unsigned char));
    for (i = 0; i < NB_ERRORS; i++) {
        e[error[i]/8] ^= (1 << (error[i]%8));
    }
    
    unsigned char * m_xor_hash_e = malloc(HASH_SIZE *sizeof(unsigned char));    
    FIPS202_SHA3_256(e, BITS_TO_BYTES(LENGTH), m_xor_hash_e);
    xor(m_xor_hash_e, m, RANDOM_BYTES); //$Elise : Tronquer les hashs?
    
    //Hash(m)
    unsigned char * hash_m = malloc(HASH_SIZE *sizeof(unsigned char));
    FIPS202_SHA3_256(m, RANDOM_BYTES, hash_m); //$Elise : Tronquer les hashs?
    
    //Copy into ct
    memcpy(ct, m_xor_hash_e, HASH_SIZE);
    memcpy(ct + HASH_SIZE, syndrome, SYNDROME_BYTES);
    memcpy(ct + HASH_SIZE + SYNDROME_BYTES, hash_m, HASH_SIZE);
        
        
    // Create shared secret
    unsigned char *m_ct = malloc((RANDOM_BYTES + CIPHERTEXT_BYTES)* sizeof(unsigned char));
    
    memcpy(m_ct, m, RANDOM_BYTES);
    memcpy(m_ct + RANDOM_BYTES, ct, CIPHERTEXT_BYTES);        
    FIPS202_SHA3_256(m_ct, RANDOM_BYTES + CIPHERTEXT_BYTES, ss);
    
    
    //Free
    free(m);
    free(syndrome);
    free(e);
    free(m_xor_hash_e);
    free(hash_m);
    free(m_ct);
    free(error);
    
    return SUCCESS;
}



/**
  Decapsulation:
    - ct is a key encapsulation message (ciphertext),
    - sk is the private key,
    - ss is the shared secret
*/
int crypto_kem_dec(OUT unsigned char *ss, IN unsigned char *ct, IN unsigned char *sk) {
    
    int i;
    int error[NB_ERRORS], error_bis[NB_ERRORS];
    int testing = TRUE;
    
    // Copy ct into ct_aux
    unsigned char * ct_aux = malloc(CIPHERTEXT_BYTES* sizeof(unsigned char));
    memcpy(ct_aux, ct, CIPHERTEXT_BYTES);
    
    // Split ct_aux into 3 parts
    unsigned char * c1 = malloc(HASH_SIZE*sizeof(unsigned char));
    unsigned char * c2 = malloc (SYNDROME_BYTES * sizeof(unsigned char));
    unsigned char * c3 = malloc (HASH_SIZE * sizeof(unsigned char));
    
    memcpy(c1, ct_aux, HASH_SIZE);
    memcpy(c2, ct_aux + HASH_SIZE, SYNDROME_BYTES);
    memcpy(c3, ct_aux + HASH_SIZE + SYNDROME_BYTES, HASH_SIZE);
    
    // Decrypt c2 (syndrome)    
    decrypt_nied(c2, error, sk); 
    
    // c1 XOR Hash(e) (= m)
    unsigned char * e = calloc(BITS_TO_BYTES(LENGTH),sizeof(unsigned char));
    for (i = 0; i < NB_ERRORS; i++) {
        e[error[i]/8] ^= (1 << (error[i] % 8));
    }
    
    unsigned char * c1_xor_hash_e = malloc(HASH_SIZE *sizeof(unsigned char));
    FIPS202_SHA3_256(e, BITS_TO_BYTES(LENGTH), c1_xor_hash_e);
    xor(c1_xor_hash_e, c1, RANDOM_BYTES);
    
    unsigned char * m = malloc(RANDOM_BYTES*sizeof(unsigned char));
    memcpy(m, c1_xor_hash_e, RANDOM_BYTES);

    // Test correctness of 'e'    
    m2error(m, error_bis);
    
    unsigned char * e_bis = calloc(BITS_TO_BYTES(LENGTH),sizeof(unsigned char));
    for (i = 0; i < NB_ERRORS; i++) {
        e_bis[error[i]/8] ^= (1 << (error[i] % 8));
    }
    
    
    for (i = 0; i < BITS_TO_BYTES(LENGTH) && testing; i++)
        testing = testing && (e[i] == e_bis[i]);

    // Test correctness of 'm'
    unsigned char * hash_m = malloc(HASH_SIZE *sizeof(unsigned char));
    FIPS202_SHA3_256(m, RANDOM_BYTES, hash_m);
      
    for (i = 0; i < HASH_SIZE && testing; i++)
        testing = testing && (hash_m[i] == c3[i]);
    
    // Construct the shared secret
    if (testing){
        unsigned char * m_ct = malloc((RANDOM_BYTES + CIPHERTEXT_BYTES)* sizeof(unsigned char));
        
       memcpy(m_ct, m, RANDOM_BYTES);
       memcpy(m_ct + RANDOM_BYTES, ct_aux, CIPHERTEXT_BYTES);
       FIPS202_SHA3_256(m_ct, RANDOM_BYTES + CIPHERTEXT_BYTES, ss);
			 free(m_ct);
    }
    

    // Free
    free(m);
    free(c1);
    free(c2);
    free(c3);
    free(ct_aux);
    free(e);
    free(e_bis);
    free(c1_xor_hash_e);
    free(hash_m);
           
    return testing ? SUCCESS: FAIL;

}
