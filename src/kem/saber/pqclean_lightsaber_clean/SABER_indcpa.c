#include "SABER_indcpa.h"
#include "SABER_params.h"
#include "fips202.h"
#include "pack_unpack.h"
#include "poly.h"
#include "poly_mul.h"
#include "randombytes.h"
#include <stdint.h>
#include <string.h>



/*-----------------------------------------------------------------------------------
    This routine generates a=[Matrix K x K] of 256-coefficient polynomials
-------------------------------------------------------------------------------------*/

#define h1 4 //2^(EQ-EP-1)

#define h2 ( (1<<(SABER_EP-2)) - (1<<(SABER_EP-SABER_ET-1)) + (1<<(SABER_EQ-SABER_EP-1)) )

static void InnerProd(uint16_t pkcl[SABER_K][SABER_N], uint16_t skpv[SABER_K][SABER_N], uint16_t mod, uint16_t res[SABER_N]);
static void MatrixVectorMul(polyvec *a, uint16_t skpv[SABER_K][SABER_N], uint16_t res[SABER_K][SABER_N], uint16_t mod, int16_t transpose);

static void POL2MSG(const uint16_t *message_dec_unpacked, unsigned char *message_dec);

static void GenMatrix(polyvec *a, const unsigned char *seed) {
    unsigned char buf[SABER_K * SABER_K * (13 * SABER_N / 8)];

    uint16_t temp_ar[SABER_N];

    int i, j, k;
    uint16_t mod = (SABER_Q - 1);

    shake128(buf, sizeof(buf), seed, SABER_SEEDBYTES);

    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_K; j++) {
            PQCLEAN_LIGHTSABER_CLEAN_BS2POL(buf + (i * SABER_K + j) * (13 * SABER_N / 8), temp_ar);
            for (k = 0; k < SABER_N; k++) {
                a[i].vec[j].coeffs[k] = (temp_ar[k])& mod ;
            }
        }
    }
}


void PQCLEAN_LIGHTSABER_CLEAN_indcpa_kem_keypair(unsigned char *pk, unsigned char *sk) {
    polyvec a[SABER_K];

    uint16_t skpv[SABER_K][SABER_N];

    unsigned char seed[SABER_SEEDBYTES];
    unsigned char noiseseed[SABER_COINBYTES];
    int32_t i, j;
    uint16_t mod_q = SABER_Q - 1;


    uint16_t res[SABER_K][SABER_N];

    randombytes(seed, SABER_SEEDBYTES);

    // for not revealing system RNG state
    shake128(seed, SABER_SEEDBYTES, seed, SABER_SEEDBYTES);
    randombytes(noiseseed, SABER_COINBYTES);

    GenMatrix(a, seed);   //sample matrix A

    // generate secret from constant-time binomial distribution
    PQCLEAN_LIGHTSABER_CLEAN_GenSecret(skpv, noiseseed);

    // do the matrix vector multiplication and rounding
    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_N; j++) {
            res[i][j] = 0;
        }
    }
    MatrixVectorMul(a, skpv, res, SABER_Q - 1, 1);

    // now rounding
    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_N; j++) {
            // shift right 3 bits
            res[i][j] = (res[i][j] + h1) & (mod_q);
            res[i][j] = (res[i][j] >> (SABER_EQ - SABER_EP));
        }
    }

    // unload and pack sk=3 x (256 coefficients of 14 bits)
    PQCLEAN_LIGHTSABER_CLEAN_POLVEC2BS(sk, skpv, SABER_Q);

    // unload and pack pk=256 bits seed and 3 x (256 coefficients of 11 bits)
    // load the public-key coefficients
    PQCLEAN_LIGHTSABER_CLEAN_POLVEC2BS(pk, res, SABER_P);


    // now load the seedbytes in PK. Easy since seed bytes are kept in byte format.
    for (i = 0; i < SABER_SEEDBYTES; i++) {
        pk[SABER_POLYVECCOMPRESSEDBYTES + i] = seed[i];
    }

}


void PQCLEAN_LIGHTSABER_CLEAN_indcpa_kem_enc(const unsigned char *message_received, unsigned char *noiseseed, const unsigned char *pk, unsigned char *ciphertext) {
    uint32_t i, j, k;
    polyvec a[SABER_K];
    unsigned char seed[SABER_SEEDBYTES];
    // public key of received by the client
    uint16_t pkcl[SABER_K][SABER_N];
    uint16_t skpv1[SABER_K][SABER_N];
    uint16_t message[SABER_KEYBYTES * 8];
    uint16_t res[SABER_K][SABER_N];
    uint16_t mod_p = SABER_P - 1;
    uint16_t mod_q = SABER_Q - 1;
    uint16_t vprime[SABER_N];
    unsigned char msk_c[SABER_SCALEBYTES_KEM];

    // extract the seedbytes from Public Key.
    for (i = 0; i < SABER_SEEDBYTES; i++) {
        seed[i] = pk[ SABER_POLYVECCOMPRESSEDBYTES + i];
    }

    GenMatrix(a, seed);

    // generate secret from constant-time binomial distribution
    PQCLEAN_LIGHTSABER_CLEAN_GenSecret(skpv1, noiseseed);

    // matrix-vector multiplication and rounding
    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_N; j++) {
            res[i][j] = 0;
        }
    }
    MatrixVectorMul(a, skpv1, res, SABER_Q - 1, 0);

    // now rounding
    //shift right 3 bits
    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_N; j++) {
            res[i][j] = ( res[i][j] + h1 ) & mod_q;
            res[i][j] = (res[i][j] >> (SABER_EQ - SABER_EP) );
        }
    }

    PQCLEAN_LIGHTSABER_CLEAN_POLVEC2BS(ciphertext, res, SABER_P);

    // ************client matrix-vector multiplication ends************

    // now calculate the v'
    // unpack the public_key
    // pkcl is the b in the protocol
    PQCLEAN_LIGHTSABER_CLEAN_BS2POLVEC(pk, pkcl, SABER_P);
    for (i = 0; i < SABER_N; i++) {
        vprime[i] = 0;
    }
    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_N; j++) {
            skpv1[i][j] = skpv1[i][j] & (mod_p);
        }
    }

    // vector-vector scalar multiplication with mod p
    InnerProd(pkcl, skpv1, mod_p, vprime);

    // addition of h1 to vprime
    for (i = 0; i < SABER_N; i++) {
        vprime[i] = vprime[i] + h1;
    }

    // unpack message_received;
    for (j = 0; j < SABER_KEYBYTES; j++) {
        for (i = 0; i < 8; i++) {
            message[8 * j + i] = ((message_received[j] >> i) & 0x01);
        }
    }

    // message encoding
    for (i = 0; i < SABER_N; i++) {
        message[i] = (message[i] << (SABER_EP - 1));
    }

    for (k = 0; k < SABER_N; k++) {
        vprime[k] = ( (vprime[k] - message[k]) & (mod_p) ) >> (SABER_EP - SABER_ET);
    }


    PQCLEAN_LIGHTSABER_CLEAN_pack_3bit(msk_c, vprime);

    for (j = 0; j < SABER_SCALEBYTES_KEM; j++) {
        ciphertext[SABER_POLYVECCOMPRESSEDBYTES + j] = msk_c[j];
    }
}


void PQCLEAN_LIGHTSABER_CLEAN_indcpa_kem_dec(const unsigned char *sk, const unsigned char *ciphertext, unsigned char message_dec[]) {
    uint32_t i, j;
    // secret key of the server
    uint16_t sksv[SABER_K][SABER_N];
    uint16_t pksv[SABER_K][SABER_N];
    uint8_t scale_ar[SABER_SCALEBYTES_KEM];
    uint16_t mod_p = SABER_P - 1;
    uint16_t v[SABER_N];
    uint16_t op[SABER_N];

    // sksv is the secret-key
    PQCLEAN_LIGHTSABER_CLEAN_BS2POLVEC(sk, sksv, SABER_Q);
    // pksv is the ciphertext
    PQCLEAN_LIGHTSABER_CLEAN_BS2POLVEC(ciphertext, pksv, SABER_P);

    // vector-vector scalar multiplication with mod p
    for (i = 0; i < SABER_N; i++) {
        v[i] = 0;
    }
    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_N; j++) {
            sksv[i][j] = sksv[i][j] & (mod_p);
        }
    }
    InnerProd(pksv, sksv, mod_p, v);

    //Extraction
    for (i = 0; i < SABER_SCALEBYTES_KEM; i++) {
        scale_ar[i] = ciphertext[SABER_POLYVECCOMPRESSEDBYTES + i];
    }

    PQCLEAN_LIGHTSABER_CLEAN_un_pack3bit(scale_ar, op);

    //addition of h1
    for (i = 0; i < SABER_N; i++) {
        v[i] = ( ( v[i] + h2 - (op[i] << (SABER_EP - SABER_ET)) ) & (mod_p) ) >> (SABER_EP - 1);
    }

    // pack decrypted message
    POL2MSG(v, message_dec);
}
static void MatrixVectorMul(polyvec *a, uint16_t skpv[SABER_K][SABER_N], uint16_t res[SABER_K][SABER_N], uint16_t mod, int16_t transpose) {
    uint16_t acc[SABER_N];
    int32_t i, j, k;

    if (transpose == 1) {
        for (i = 0; i < SABER_K; i++) {
            for (j = 0; j < SABER_K; j++) {
                PQCLEAN_LIGHTSABER_CLEAN_pol_mul((uint16_t *)&a[j].vec[i], skpv[j], acc, SABER_Q, SABER_N);

                for (k = 0; k < SABER_N; k++) {
                    res[i][k] = res[i][k] + acc[k];
                    //reduction mod p
                    res[i][k] = (res[i][k] & mod);
                    //clear the accumulator
                    acc[k] = 0;
                }
            }
        }
    } else {
        for (i = 0; i < SABER_K; i++) {
            for (j = 0; j < SABER_K; j++) {
                PQCLEAN_LIGHTSABER_CLEAN_pol_mul((uint16_t *)&a[i].vec[j], skpv[j], acc, SABER_Q, SABER_N);
                for (k = 0; k < SABER_N; k++) {
                    res[i][k] = res[i][k] + acc[k];
                    // reduction
                    res[i][k] = res[i][k] & mod;
                    // clear the accumulator
                    acc[k] = 0;
                }
            }
        }
    }
}

static void POL2MSG(const uint16_t *message_dec_unpacked, unsigned char *message_dec) {
    int32_t i, j;

    for (j = 0; j < SABER_KEYBYTES; j++) {
        message_dec[j] = 0;
        for (i = 0; i < 8; i++) {
            message_dec[j] = message_dec[j] | (uint8_t) (message_dec_unpacked[j * 8 + i] << i);
        }
    }
}


static void InnerProd(uint16_t pkcl[SABER_K][SABER_N], uint16_t skpv[SABER_K][SABER_N], uint16_t mod, uint16_t res[SABER_N]) {
    uint32_t j, k;
    uint16_t acc[SABER_N];

    // vector-vector scalar multiplication with mod p
    for (j = 0; j < SABER_K; j++) {
        PQCLEAN_LIGHTSABER_CLEAN_pol_mul(pkcl[j], skpv[j], acc, SABER_P, SABER_N);

        for (k = 0; k < SABER_N; k++) {
            res[k] = res[k] + acc[k];
            // reduction
            res[k] = res[k] & mod;
            // clear the accumulator
            acc[k] = 0;
        }
    }
}
