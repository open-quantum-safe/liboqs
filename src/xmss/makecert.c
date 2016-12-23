/* XMSS Certificate creation. Demonstrates some certificate-related
 * operations.
 */

// TODO: take as argument a secret key, output the *corresponding* public key.

#include "xmss.c"
#include <openssl/x509v3.h>
#include <openssl/err.h>

EVP_PKEY * read_pubkey(char *path)
{
	FILE *fp = fopen(path,"r");
	if (!fp)
	{
		fprintf(stderr,"[makecert.c] Error: Unable to open file!\n");
		abort();
	}
	EVP_PKEY *pkey = NULL;
	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(NID_xmss, NULL);
	pkey=EVP_PKEY_new();
	EVP_PKEY_set1_XMSS(pkey, (XMSS_PKEY_CTX*) ctx);
	if(!PEM_read_PUBKEY(fp, &pkey, 0, 0))
	{
                fprintf(stderr, "[makecert.c] PEM_read_PUBKEY() failed!");
                ERR_print_errors_fp(stderr);
                abort();    
        }    
	fclose(fp);
	return pkey;
}

/* Add extension using V3 code: we can set the config file as NULL
 * because we wont reference any other sections.
 */

int add_ext(X509 *cert, int nid, char *value)
{
	X509_EXTENSION *ex;
	X509V3_CTX ctx;
	/* This sets the 'context' of the extensions. */
	/* No configuration database */
	X509V3_set_ctx_nodb(&ctx);
	/* Issuer and subject certs: both the target since it is self signed,
	 * no request and no CRL
	 */
	X509V3_set_ctx(&ctx, cert, cert, NULL, NULL, 0);
	ex = X509V3_EXT_conf_nid(NULL, &ctx, nid, value);
	if (!ex) return 0;

	X509_add_ext(cert, ex, -1);
	X509_EXTENSION_free(ex);
	return 1;
}

int mkcert(X509 **x509p, EVP_PKEY **pkeyp, EVP_PKEY **pubkey)
{
	fprintf(stderr, "[makecert.c] Entered mkcert()\n");
	int serial = 1;
	int days = 10;
	X509 *x;
	EVP_PKEY *pk;
	X509_NAME *name=NULL;
	
	if ((pkeyp == NULL) || (*pkeyp == NULL))
	{
		if ((pk=EVP_PKEY_new()) == NULL)
			{
			abort(); 
			return(0);
			}
	}
	else pk= *pkeyp;

	if ((x509p == NULL) || (*x509p == NULL))
	{
		if ((x=X509_new()) == NULL) goto err;
	}
	else x= *x509p;

	fprintf(stderr,"[makecert.c] mkcert(): EVP_PKEY_keygen() done\n");

	X509_set_version(x, 2);
	ASN1_INTEGER_set(X509_get_serialNumber(x), serial);
	X509_gmtime_adj(X509_get_notBefore(x), 0);
	X509_gmtime_adj(X509_get_notAfter(x), (long)60*60*24*days);
	X509_set_pubkey(x, *pubkey);

	name=X509_get_subject_name(x);

	/* This function creates and adds the entry, working out the
	 * correct string type and performing checks on its length.
	 * Normally we'd check the return value for errors...
	 */
	X509_NAME_add_entry_by_txt(name,"C",
				MBSTRING_ASC, (unsigned char*)"UK", -1, -1, 0);
	X509_NAME_add_entry_by_txt(name,"CN",
				MBSTRING_ASC, (unsigned char *)"OpenSSL Group", -1, -1, 0);

	/* Its self signed so set the issuer name to be the same as the
 	 * subject.
	 */
	X509_set_issuer_name(x,name);

	/* Add various extensions: standard extensions */
	add_ext(x, NID_basic_constraints, "critical,CA:TRUE");
	add_ext(x, NID_key_usage, "critical,keyCertSign,cRLSign,keyAgreement,digitalSignature,nonRepudiation");

	add_ext(x, NID_subject_key_identifier, "hash");

	/* Some Netscape specific extensions */
	add_ext(x, NID_netscape_cert_type, "sslCA");

	add_ext(x, NID_netscape_comment, "Signed with XMSS");

	int nid;
	nid = OBJ_create("1 3 6 1 4 1 8301 3 1 3 3 1", "XMSS-SHA256", "xmssWithSHA256");
	X509V3_EXT_add_alias(nid, NID_xmss);
	add_ext(x, nid, "example comment alias");
	
	fprintf(stderr, "[makecert.c] mkcert(): about to call X509_sign()\n");
	
	if (!X509_sign(x,pk,EVP_sha256())) {
		fprintf(stderr, "[makecert.c] X509_sign failed!\n");
		abort();
		goto err;
	}
	fprintf(stderr, "[makecert.c] mkcert(): X509_sign() done\n");

	*x509p=x;
	*pkeyp=pk;
	return(1);
err:
	return(0);
}

int main(int argc, char **argv)
{
	BIO *bio_err;
	X509 *x509 = NULL;
	EVP_PKEY *pkey = NULL;
	EVP_PKEY *pubkey = NULL;

	bio_err=BIO_new_fp(stderr, BIO_NOCLOSE);
	load_xmss_all(); // Load XMSS integration suite

        if (argc < 3)
	{
           fprintf(stderr,"[makecert.c] Missing inputs! argc was %d\n",argc);
           fprintf(stderr,"[makecert.c] Usage: ./makecert privkey pubkey\n");
           abort();
        }

        FILE *fp = fopen(argv[1],"r");
        fprintf(stderr,"[makecert.c] fp is %p \n", (void *)fp);
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(NID_xmss, NULL);
        pkey=EVP_PKEY_new();
        EVP_PKEY_set1_XMSS(pkey, (XMSS_PKEY_CTX*) ctx);

        if(!PEM_read_PrivateKey(fp, &pkey,0,0))
	{
                fprintf(stderr,"[makecert.c] PEM_read_PrivateKey() failed!");
                ERR_print_errors_fp(stderr);
                abort();
        }

	pubkey = read_pubkey(argv[2]);
	mkcert(&x509,&pkey,&pubkey);
	
	X509_print_fp(stdout,x509);
	PEM_write_X509(stdout,x509);

	CRYPTO_cleanup_all_ex_data();
	CRYPTO_mem_leaks(bio_err);
	BIO_free(bio_err);
	return(0);
}
