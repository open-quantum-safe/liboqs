/* makepk_evp.c
 * XMSS EVP public key creation
 * Takes an XMSS public key as input 
 */

#include "xmss.c"
#include <openssl/err.h>

int main(int argc, char **argv)
{
	BIO *bio_err;
	EVP_PKEY *pkey = NULL;
	bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
	ERR_load_crypto_strings();

	load_xmss_all(); // Load XMSS integration suite

	if (argc < 2) {
		fprintf(stderr,"[makepk_evp.c] Error: no file name given for the XMSS secret key! argc was %d\n",argc);
		fprintf(stderr,"[makepk_evp.c] Usage: ./makepk_evp privkey\n");
		abort();
	}
	FILE *fp = fopen(argv[1],"r");
	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(NID_xmss, NULL);
	pkey=EVP_PKEY_new();
	EVP_PKEY_set1_XMSS(pkey, (XMSS_PKEY_CTX*) ctx);

	if(!PEM_read_PrivateKey(fp, &pkey, 0, 0)){
		fprintf(stderr, "[makepk_evp.c] PEM_read_PrivateKey() failed!\n");
		ERR_print_errors_fp(stderr);
		abort();
	}
	
	PEM_write_PUBKEY(stdout, pkey);
	CRYPTO_cleanup_all_ex_data();
	CRYPTO_mem_leaks(bio_err);
	BIO_free(bio_err);
	fprintf(stderr, "[makepk_evp.c] makepk_evp done\n");
	return(0);
}
