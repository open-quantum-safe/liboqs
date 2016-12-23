/* makesk_evp.c
 * XMSS EVP secret key creation
 */

#include "xmss.c"

int mksk(EVP_PKEY **pkeyp)
{
	EVP_PKEY *pk;
	if ((pkeyp == NULL) || (*pkeyp == NULL))
		{
		if ((pk=EVP_PKEY_new()) == NULL)
			{
			abort(); 
			return(0);
			}
		}
	else pk= *pkeyp;

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(NID_xmss, NULL);
	if (!pk) pk = EVP_PKEY_new();	
	EVP_PKEY_keygen_init(ctx);
	if (EVP_PKEY_keygen(ctx,&pk) <= 0){
		fprintf(stderr,"[makesk_evp.c] Error: EVP_PKEY_keygen failed!\n");
		return(0);
	}
	*pkeyp=pk;	
	return(1);
}

int main()
{
	fprintf(stderr,"[makesk_evp.c] Entered main\n");
	BIO *bio_err;
	EVP_PKEY *pkey=NULL;
	// Memory leak detection turned on:
	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
	bio_err=BIO_new_fp(stderr, BIO_NOCLOSE);

	load_xmss_all(); // Load XMSS integration suite
	mksk(&pkey);
	EVP_PKEY_set1_XMSS(pkey, (XMSS_PKEY_CTX*)pkey->pkey.ptr);

	fprintf(stderr, "[makesk_evp.c] OBJ_nid2obj(NID_xmss) =  %p\n",(void *)OBJ_nid2obj(NID_xmss));
	if(!PEM_write_PrivateKey(stdout, pkey, NULL, NULL, 0, NULL, NULL)) {
		fprintf(stdout,"[makesk_evp.c] Error from PEM_write_PrivateKey!\n");
	}

	CRYPTO_cleanup_all_ex_data();
	// Print out the list of memory that has not been free()ed:
	// CRYPTO_mem_leaks(bio_err);
	BIO_free(bio_err);
	fprintf(stderr, "[makesk_evp.c] makesk_evp done\n");
	return(0);
}

