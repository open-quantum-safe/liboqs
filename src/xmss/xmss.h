#ifndef XMSS_INT_H
#define XMSS_INT_H

#define NID_xmss 958

// The order of the following includes matters.

#include "../../../../crypto/crypto.h"

#include <openssl/asn1t.h>
#include <openssl/cms.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

#include "../../../../crypto/evp/evp_locl.h"
#include "../../../../crypto/asn1/asn1_locl.h"

#include "./xmss_draft.h"

void load_xmss_all();

/* == internal state/context for EVP XMSS
* Possibly contains a secret key, public key, signature, parameters
* during runtime
*/
typedef struct
	{
		wots_param *para;
		xmss_sk *sk;
		xmss_pk *pk;
		xmss_sig *sig;
		unsigned char *seed;
		int seedlength;
		int references;
		EVP_MD *md;
		int h;
		int w;
		unsigned char adrs[32];
	} XMSS_PKEY_CTX;

int EVP_PKEY_set1_XMSS(EVP_PKEY *pkey, XMSS_PKEY_CTX *key);

#endif
