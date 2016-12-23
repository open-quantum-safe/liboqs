// This will take as input a certificate and a public key and check validity.
// Alternatively, do this using just certificates.

#include "xmss.c"
#include <openssl/err.h>

void load_xmss_all();

int mkcert(X509 **x509p, EVP_PKEY **pkeyp, EVP_PKEY **pubkey);
int add_ext(X509 *cert, int nid, char *value);

EVP_PKEY * read_pubkey(char *path) {
        FILE *fp = fopen(path,"r");
        if (!fp) {
                fprintf(stderr,"UNABLE TO OPEN FILE!\n");
                abort();
        }
        EVP_PKEY *pkey=NULL;
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(NID_xmss, NULL);
        pkey=EVP_PKEY_new();
        EVP_PKEY_set1_XMSS(pkey, (XMSS_PKEY_CTX*) ctx);
        if(!PEM_read_PUBKEY(fp, &pkey,0,0)){
                fprintf(stderr,"PEM_read_PrivateKey() failed!");
                ERR_print_errors_fp(stderr);
                abort();    
        }    
        fclose(fp);
        return pkey;
}

X509 *read_certificate(char *path) {
        FILE *fp = fopen(path,"r"); 
        if (!fp) {  
                fprintf(stderr,"UNABLE TO OPEN FILE!\n");
                abort();            
        }           
	X509 *x509 = NULL;
	PEM_read_X509(fp, &x509, NULL, NULL);
	return x509;
}


int main(int c, char **v) {
	if (c < 3) {
                fprintf(stderr,"NEED MORE ARGUMENTS!\n");
                fprintf(stderr,"usage: ./verifycert pubkey certificate!\n");
                abort();            	
	}
	load_xmss_all();
	EVP_PKEY *pub = read_pubkey(v[1]);
	PEM_write_PUBKEY(stdout,pub);
	X509 *x509 = read_certificate(v[2]);
	PEM_write_X509(stdout,x509);


	int verify_return =  X509_verify(x509, pub);

	printf("===========================\n");
	
	printf("X509_verify('%s','%s') = %d \n",v[2],v[1],verify_return);
	if (verify_return == 1) fprintf(stderr, "Certificate verification succeeds.\n");
		else fprintf(stderr, "Certificate verification fails!\n");
	printf("===========================\n");


	return 0;
}
