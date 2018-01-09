/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: Key Encapsulation Mechanism (KEM) based on Frodo
*********************************************************************************************/

#include <string.h>
#include "sha3/fips202.h"
#include "random/random.h"


int crypto_kem_keypair(unsigned char* pk, unsigned char* sk)
{ // Frodo-KEM's key generation
  // Outputs: public key pk (               BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 bytes) 
  //          secret key sk (CRYPTO_BYTES + BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + 2*PARAMS_N*PARAMS_NBAR bytes)
    uint16_t B[PARAMS_N*PARAMS_NBAR] = {0}, E[PARAMS_N*PARAMS_NBAR] = {0}, S[PARAMS_N*PARAMS_NBAR] = {0};
    uint8_t *randomness = sk;
          
    // Generate the secret value s, the seed for S and E, and the seed for A. Add seed_A to the public key
    randombytes(randomness, 2*CRYPTO_BYTES + BYTES_SEED_A);
    cshake(pk, BYTES_SEED_A, 0, randomness + 2*CRYPTO_BYTES, (unsigned long long)(BYTES_SEED_A));

    // Generate S and E, and compute B = A*S + E. Generate A on-the-fly
    frodo_sample_n(S, PARAMS_N*PARAMS_NBAR, randomness + CRYPTO_BYTES, CRYPTO_BYTES, 1);
    frodo_sample_n(E, PARAMS_N*PARAMS_NBAR, randomness + CRYPTO_BYTES, CRYPTO_BYTES, 2);
    frodo_mul_add_as_plus_e(B, S, E, pk);

    // Encode the second part of the public key
    frodo_pack(pk+BYTES_SEED_A, CRYPTO_PUBLICKEYBYTES - BYTES_SEED_A, B, PARAMS_N*PARAMS_NBAR, PARAMS_LOGQ);

    // Add pk and S to the secret key
    memcpy(&sk[CRYPTO_BYTES], pk, CRYPTO_PUBLICKEYBYTES);
    memcpy(&sk[CRYPTO_BYTES+CRYPTO_PUBLICKEYBYTES], S, 2*PARAMS_N*PARAMS_NBAR);

// Cleanup:
    clear_words((void*)S, PARAMS_N*PARAMS_NBAR/2);
    clear_words((void*)E, PARAMS_N*PARAMS_NBAR/2);
    return 0; 
}


int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk)
{ // Frodo-KEM's key encapsulation
    unsigned char randomness[BYTES_MU];
    uint16_t B[PARAMS_N*PARAMS_NBAR] = {0}, Bp[PARAMS_N*PARAMS_NBAR] = {0}, Ep[PARAMS_N*PARAMS_NBAR] = {0};
    uint16_t Epp[PARAMS_NBAR*PARAMS_NBAR] = {0}, V[PARAMS_NBAR*PARAMS_NBAR]= {0}, C[PARAMS_NBAR*PARAMS_NBAR] = {0};
    ALIGN_HEADER(32) uint16_t Sp[PARAMS_N*PARAMS_NBAR] ALIGN_FOOTER(32) = {0}; 
    uint8_t temp[CRYPTO_CIPHERTEXTBYTES+CRYPTO_BYTES], G[3*CRYPTO_BYTES];
    
    // temp <- pk||mu, and generate (r||KK||d) = G(pk||mu)
    randombytes(randomness, BYTES_MU);
    memcpy(temp, pk, CRYPTO_PUBLICKEYBYTES);
    memcpy(&temp[CRYPTO_PUBLICKEYBYTES], randomness, BYTES_MU);
    cshake(G, 3*CRYPTO_BYTES, 3, temp, (unsigned long long)(CRYPTO_PUBLICKEYBYTES + BYTES_MU));
    
    // Generate Sp and Ep, and compute Bp = Sp*A + Ep. Generate A on-the-fly
    frodo_sample_n(Sp, PARAMS_N*PARAMS_NBAR, G, CRYPTO_BYTES, 4);
    frodo_sample_n(Ep, PARAMS_N*PARAMS_NBAR, G, CRYPTO_BYTES, 5);
    frodo_mul_add_sa_plus_e(Bp, Sp, Ep, temp);    
    frodo_pack(ct, (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8, Bp, PARAMS_N*PARAMS_NBAR, PARAMS_LOGQ);
    
    // Generate Epp, and compute V = Sp*B + Epp
    frodo_sample_n(Epp, PARAMS_NBAR*PARAMS_NBAR, G, CRYPTO_BYTES, 6);
    frodo_unpack(B, PARAMS_N*PARAMS_NBAR, temp+BYTES_SEED_A, CRYPTO_PUBLICKEYBYTES - BYTES_SEED_A, PARAMS_LOGQ);
    frodo_mul_add_sb_plus_e(V, B, Sp, Epp);
    
    // Encode mu, and compute C = V + enc(mu) (mode q)
    frodo_key_encode(C, (uint16_t*)(temp+CRYPTO_PUBLICKEYBYTES));          
    frodo_add(C, V, C);  
    frodo_pack(ct+(PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8, (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8, C, PARAMS_NBAR*PARAMS_NBAR, PARAMS_LOGQ); 
    
    // Compute ss = F(ct||KK||d) and the ciphertext CT = ct||d
    memcpy(temp, ct, CRYPTO_CIPHERTEXTBYTES-CRYPTO_BYTES);
    memcpy(&temp[CRYPTO_CIPHERTEXTBYTES-CRYPTO_BYTES], &G[CRYPTO_BYTES], 2*CRYPTO_BYTES);
    cshake(ss, CRYPTO_BYTES, 7, temp, (unsigned long long)(CRYPTO_CIPHERTEXTBYTES + CRYPTO_BYTES)); 
    memcpy(&ct[CRYPTO_CIPHERTEXTBYTES-CRYPTO_BYTES], &G[2*CRYPTO_BYTES], CRYPTO_BYTES);
   
// Cleanup:
    clear_words((void*)Sp, PARAMS_N*PARAMS_NBAR/2);
    clear_words((void*)Ep, PARAMS_N*PARAMS_NBAR/2);
    clear_words((void*)Epp, PARAMS_NBAR*PARAMS_NBAR/2);
    clear_words((void*)V, PARAMS_NBAR*PARAMS_NBAR/2);
    clear_words((void*)G, CRYPTO_BYTES/2);
    clear_words((void*)(temp+CRYPTO_CIPHERTEXTBYTES-CRYPTO_BYTES), CRYPTO_BYTES/2);
    return 0;
}


int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) 
{ // Frodo-KEM's key decapsulation
    uint16_t B[PARAMS_N*PARAMS_NBAR] = {0}, Bp[PARAMS_N*PARAMS_NBAR] = {0}, BBp[PARAMS_N*PARAMS_NBAR] = {0};
    uint16_t Ep[PARAMS_N*PARAMS_NBAR] = {0}, Epp[PARAMS_NBAR*PARAMS_NBAR] = {0}, W[PARAMS_NBAR*PARAMS_NBAR] = {0};
    uint16_t C[PARAMS_NBAR*PARAMS_NBAR] = {0}, CC[PARAMS_NBAR*PARAMS_NBAR] = {0};
    uint16_t *S = (uint16_t*)(sk+CRYPTO_BYTES+CRYPTO_PUBLICKEYBYTES);
    ALIGN_HEADER(32) uint16_t Sp[PARAMS_N*PARAMS_NBAR] ALIGN_FOOTER(32) = {0};
    uint8_t temp[CRYPTO_CIPHERTEXTBYTES+CRYPTO_BYTES], G[3*CRYPTO_BYTES], *seed_A = temp;
    
    // temp <- pk
    memcpy(temp, &sk[CRYPTO_BYTES], CRYPTO_PUBLICKEYBYTES);
    
    // Compute W = C - Bp*S (mod q), and decode the randomness mu
    frodo_unpack(Bp, PARAMS_N*PARAMS_NBAR, ct, (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8, PARAMS_LOGQ);
    frodo_unpack(C, PARAMS_NBAR*PARAMS_NBAR, ct+(PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8, (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8, PARAMS_LOGQ);
    frodo_mul_bs(W, Bp, S);
    frodo_sub(W, C, W);
    frodo_key_decode((uint16_t*)(temp+CRYPTO_PUBLICKEYBYTES), W);

    // Generate (r||KK||d) = G(pk||mu)
    cshake(G, 3*CRYPTO_BYTES, 3, temp, (unsigned long long)(CRYPTO_PUBLICKEYBYTES + BYTES_MU)); 
    
    // Generate Sp and Ep, and compute BBp = Sp*A + Ep. Generate A on-the-fly
    frodo_sample_n(Sp, PARAMS_N*PARAMS_NBAR, G, CRYPTO_BYTES, 4);
    frodo_sample_n(Ep, PARAMS_N*PARAMS_NBAR, G, CRYPTO_BYTES, 5);
    frodo_mul_add_sa_plus_e(BBp, Sp, Ep, seed_A);    

    // Generate Epp, and compute W = Sp*B + Epp
    frodo_sample_n(Epp, PARAMS_NBAR*PARAMS_NBAR, G, CRYPTO_BYTES, 6);
    frodo_unpack(B, PARAMS_N*PARAMS_NBAR, temp+BYTES_SEED_A, CRYPTO_PUBLICKEYBYTES - BYTES_SEED_A, PARAMS_LOGQ);
    frodo_mul_add_sb_plus_e(W, B, Sp, Epp);
    
    // Encode mu, and compute CC = W + enc(mu) (mode q)
    frodo_key_encode(CC, (uint16_t*)(temp+CRYPTO_PUBLICKEYBYTES));          
    frodo_add(CC, W, CC);   

    // temp <- ct
    memcpy(temp, ct, CRYPTO_CIPHERTEXTBYTES-CRYPTO_BYTES);

    // Reducing BBp modulo q
    for (int i = 0; i < PARAMS_N*PARAMS_NBAR; i++) BBp[i] = BBp[i] & ((1 << PARAMS_LOGQ)-1);
    
    // Is (dd == d & Bp == BBp & C == CC) = true
    if (memcmp(G+2*CRYPTO_BYTES, ct+CRYPTO_CIPHERTEXTBYTES-CRYPTO_BYTES, CRYPTO_BYTES) == 0 && 
        memcmp(Bp, BBp, 2*PARAMS_N*PARAMS_NBAR) == 0 &&
        memcmp(C, CC, 2*PARAMS_NBAR*PARAMS_NBAR) == 0) {  // Load (KK || d) to do ss = F(ct||KK||d)
        memcpy(&temp[CRYPTO_CIPHERTEXTBYTES-CRYPTO_BYTES], &G[CRYPTO_BYTES], 2*CRYPTO_BYTES);  
    } else {  // Load (s || d) to do ss = F(ct||s||d)
        memcpy(&temp[CRYPTO_CIPHERTEXTBYTES-CRYPTO_BYTES], sk, CRYPTO_BYTES); 
        memcpy(&temp[CRYPTO_CIPHERTEXTBYTES], &G[2*CRYPTO_BYTES], CRYPTO_BYTES);  
    }
    cshake(ss, CRYPTO_BYTES, 7, temp, (unsigned long long)(CRYPTO_CIPHERTEXTBYTES + CRYPTO_BYTES));

// Cleanup:
    clear_words((void*)Sp, PARAMS_N*PARAMS_NBAR/2);
    clear_words((void*)Ep, PARAMS_N*PARAMS_NBAR/2);
    clear_words((void*)Epp, PARAMS_NBAR*PARAMS_NBAR/2);
    clear_words((void*)W, PARAMS_NBAR*PARAMS_NBAR/2);
    clear_words((void*)G, CRYPTO_BYTES/2);
    clear_words((void*)(temp+CRYPTO_CIPHERTEXTBYTES-CRYPTO_BYTES), CRYPTO_BYTES/2);
    return 0;
}


