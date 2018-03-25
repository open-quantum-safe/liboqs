#ifndef API_H
#define API_H

#define GF_EXT_DEGREE 18
#define LENGTH 7410
#define NB_ERRORS 152
#define ORDER 19

#include "sizes.h"

//For clarity of the code.
#define IN const 
//no modified by the function
#define OUT
//modified by the function

//$Elise : À remplir correctement
#define CRYPTO_SECRETKEYBYTES SECRETKEY_BYTES
//sizeof secret key
#define CRYPTO_PUBLICKEYBYTES PUBLICKEY_BYTES
//sizeof public key
#define CRYPTO_CIPHERTEXTBYTES CIPHERTEXT_BYTES
//sizeof cipher text
#define CRYPTO_BYTES HASH_SIZE       
//sizeof shared secret

#define CRYPTO_ALGNAME "BIG_QUAKE_3"


#define CHECK_STATUS(stat) {if(stat != SUCCESS) {goto EXIT;}}
//$Elise : Ajouter le types d'erreurs ici
enum _status
{
    SUCCESS  = 0,
    FAIL = -1,
};

typedef enum _status status_t;

////////////////////////////////////////////////////////////////
//Below three APIs (keygen, encaps, decaps) are defined by NIST:
////////////////////////////////////////////////////////////////
//Keygenerate - pk is the public key,
//              sk is the private key,
int crypto_kem_keypair(OUT unsigned char *pk, OUT unsigned char *sk);

//Encapsulate - pk is the public key,
//              ct is a key encapsulation message (ciphertext),
//              ss is the shared secret.
int crypto_kem_enc(OUT unsigned char *ct,
                   OUT unsigned char *ss,
                   IN unsigned char *pk);

//Decapsulate - ct is a key encapsulation message (ciphertext),
//              sk is the private key,
//              ss is the shared secret
int crypto_kem_dec(OUT unsigned char *ss,
                   IN unsigned char *ct,
                   IN unsigned char *sk);

#endif
