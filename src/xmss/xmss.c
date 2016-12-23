#include "xmss.h"

// Comment next line out to remove information about entered functions
#define DEBUGFUNC 1

/* Increases the reference count for OpenSSL such that 
 *  OpenSSL will only free if the count is down to 1
 *  (the context that is using it to free() right now)
 */

int XMSS_up_ref(XMSS_PKEY_CTX *key)
{
	int i = CRYPTO_add(&key->references, 1, CRYPTO_LOCK_XMSS);
	return ((i > 1) ? 1 : 0);
}

/* Takes a EVP_PKEY and sets it up to be used with XMSS
 *  setting a constant and the XMSS_PKEY_CTX context inside a union
 *  within the pkey
 */
int EVP_PKEY_set1_XMSS(EVP_PKEY *pkey, XMSS_PKEY_CTX *key)
{             
        int ret = EVP_PKEY_assign(pkey, NID_xmss, key);
        if(ret) XMSS_up_ref(key);
        return ret;
}             

/* Allocate/initialize memory in XMSS_PKEY_CTX 
 *  but only if it hasn't been done yet
 */
void xmss_init(XMSS_PKEY_CTX *xmss)
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered xmss_init()\n");
	#endif
	xmss->para = 0;
	xmss->pk = 0;
	xmss->sk = 0;
	xmss->sig = 0;
	xmss->references = 0;
	xmss->seed=0;
	xmss->seedlength=0;
	xmss->w=0;
	memset(xmss->adrs, 0, 32);
}

/* == int pkey_xmss_init(EVP_PKEY_CTX *ctx)
* call xmss_init() and modify context in a meaningful way
*/
int pkey_xmss_init(EVP_PKEY_CTX *ctx)
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered pkey_xmss_init()\n");
	#endif
	XMSS_PKEY_CTX *xmss  = OPENSSL_malloc(sizeof(XMSS_PKEY_CTX));
	if (!xmss) return 0;
	xmss_init(xmss);
	ctx->data = xmss;
	return 1;
}

/* Makes a copy of the pointer to the context
 * OpenSSL seems to copy the state before making signatures/verification
 */

int pkey_xmss_copy(EVP_PKEY_CTX *dst, EVP_PKEY_CTX *src)
{
	XMSS_PKEY_CTX *dctx, *sctx;
	if (!pkey_xmss_init(dst))
		return 0;
	sctx = src->data;
	dctx = dst->data;
	dctx->references = sctx->references;
	dctx->md = sctx->md;
	return 1;	
}

// Frees memory such that no memleaks occurs
void pkey_xmss_cleanup(EVP_PKEY_CTX *ctx)
{
	#ifdef DEBUGFUNC
	fprintf(stderr,"[xmss.c] Entered pkey_xmss_cleanup\n");
	#endif
	XMSS_PKEY_CTX *xmss = ctx->data;
	if (xmss)
	        OPENSSL_free(xmss);
}


/* Check whether context is in order for signing and
 * call xmss_sig_init()
 */
int pkey_xmss_sign_init(EVP_PKEY_CTX *ctx)
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered pkey_xmss_sign_init()\n");
	#endif
	XMSS_PKEY_CTX *xmss = (XMSS_PKEY_CTX*) ctx->pkey->pkey.ptr;
	if (!xmss->sk) {
		fprintf(stderr,"pkey_xmss_sign_init: Error: xmss->sk is NULL\n");
		abort();
	}
	xmss->sig=OPENSSL_malloc(sizeof(xmss_sig));
	xmss_sig_init(xmss->sig, xmss->para, xmss->h);
	return 1;
}

/* Make an actual signature with the given context/msg
 *	tbs (to be signed/input)
 *	sig (output)
 *  We don't actually use siglen, but it's required by pmeth
 */

int pkey_xmss_sign(EVP_PKEY_CTX *ctx, unsigned char *sig, size_t *siglen,
					const unsigned char *tbs, size_t tbslen)
{
	(void)(siglen); // Silence unused variable warning
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered pkey_xmss_sign()\n");
	#endif
	XMSS_PKEY_CTX *xmss = (XMSS_PKEY_CTX*) ctx->pkey->pkey.ptr;
	if (xmss_signing(xmss->sk,(unsigned char*)tbs,tbslen,sig,xmss->adrs)) {
		fprintf(stderr,"pkey_xmss_sign: Error calling xmss_signing!\n");  
		abort();
	}
	// TODO: Write secret key to disk here!
	return 1;
}

// Verify a signature
// We don't actually use siglen, but it's required by pmeth
int pkey_xmss_verify(EVP_PKEY_CTX *ctx,
					const unsigned char *sig, size_t siglen,
					const unsigned char *tbs, size_t tbslen)
{
	(void)(siglen); // Silence unused variable warning
	XMSS_PKEY_CTX *xmss = (XMSS_PKEY_CTX*) ctx->pkey->pkey.ptr;
	unsigned char *pk_all = OPENSSL_malloc(xmss->para->n * 2);
	xmssPKtoString(pk_all,xmss->pk,xmss->para->n);
	int ver = xmss_verification(pk_all,(unsigned char*)sig, (unsigned char*)tbs,tbslen, xmss->para, xmss->h);
	free(pk_all);
	return !ver;
}

int pkey_xmss_keygen_init(EVP_PKEY_CTX *ctx)
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered pkey_xmss_keygen_init()\n");
	#endif
	XMSS_PKEY_CTX *x=ctx->data;
	x->sk = OPENSSL_malloc(sizeof(xmss_sk));
	x->para = OPENSSL_malloc(sizeof(wots_param));
	x->pk = OPENSSL_malloc(sizeof(xmss_pk));
	x->h = 5; // Tree height
	x->w = 16; // Winternitz parameter
	unsigned int n = 32; // Hash function output length in bytes
	wots_param_init(x->para, n, x->w);
	x->seedlength = x->para->n;
	return 1;
}

// Keygen that right now uses fixed parameters (needs to be exchanged)
int pkey_xmss_keygen(EVP_PKEY_CTX *ctx, EVP_PKEY *pkey)
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered pkey_xmss_keygen()\n");
	#endif
	XMSS_PKEY_CTX *xmss = (XMSS_PKEY_CTX*) ctx->data;
	unsigned char *pk = OPENSSL_malloc(xmss->para->n*2);
	memset(pk, 0, xmss->para->n*2);
	xmss_sk_init(xmss->sk, xmss->para, xmss->h);
	xmss_pk_init(xmss->pk, xmss->para->n);
	xmss_keyGen(xmss->sk, pk, xmss->para, xmss->h);
	xmssStringToPK(xmss->pk, pk, xmss->para->n);
	pkey->pkey.ptr = (void*) xmss;
	EVP_PKEY_set1_XMSS(pkey,xmss);
	OPENSSL_free(pk);
	return 1;
}

/* Manipulate context e.g. change Winternitz parameter or hash size
 * not really implemented only so much as that OpenSSL doesn't fail on it
 * TODO: Clean this up
 */
static int pkey_xmss_ctrl(EVP_PKEY_CTX *ctx, int type, int p1, void *p2)
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered pkey_xmss_ctrl()\n");
	fprintf(stderr,"[xmss.c] pkey_xmss_ctrl: Arguments are %d %d %p\n", type, p1, p2);
	#endif
	XMSS_PKEY_CTX *xmss = ctx->data;
	switch(type)
	{
                case EVP_PKEY_CTRL_MD:
                if (EVP_MD_type((const EVP_MD *)p2) != NID_sha256 &&
                    EVP_MD_type((const EVP_MD *)p2) != NID_sha384 &&
                    EVP_MD_type((const EVP_MD *)p2) != NID_sha512)
                    {
                    	fprintf(stderr,"[pkey_xmss_ctrl] Error: invalid digest type!\n");
                    	return 0;
                    }
                xmss->md = (EVP_MD *) p2;
                return 1;
                case EVP_PKEY_CTRL_DIGESTINIT:
                case EVP_PKEY_CTRL_PKCS7_SIGN:
                case EVP_PKEY_CTRL_CMS_SIGN:
                return 1; 
		case EVP_PKEY_CTRL_PEER_KEY:
			fprintf(stderr, "[xmss.c] pkey_xmss_ctrl: Error: unsupported EVP operation for XMSS keytype!\n");
                        return -2;
                default:
                return -2;
	}
	abort();
	return 0;
}

// Insert new EVP pkey method functionality
void load_pmeth()
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered load_pmeth()\n");
	#endif
	static EVP_PKEY_METHOD x =
	{
		NID_xmss,
		EVP_PKEY_FLAG_AUTOARGLEN,
		pkey_xmss_init,
		pkey_xmss_copy,
		pkey_xmss_cleanup,
		0, // paramgen_init
		0, //paramgen
		pkey_xmss_keygen_init,
		pkey_xmss_keygen,
		pkey_xmss_sign_init,
		pkey_xmss_sign,
		0, //verify_init
		pkey_xmss_verify,
		0, //verify_recover_init
		0, //verify_recover
		0, //signctx_init
		0, //signctx
		0, //verifyctx_init
		0, //verifyctx
		0, //encrypt_init
		0, //encrypt
		0, //decrypt_init
		0, //decrypt
		0, //derive_init
		0, //derive
		pkey_xmss_ctrl, // defined above
		0 // pkey_xmss_ctrl_str
	};
	EVP_PKEY_meth_add0(&x);
}

/* === BEGIN ASN.1 PART === */

// ASN.1 definitions for encoding secret key

// W-OTS+
ASN1_SEQUENCE(wots_param) = {
	ASN1_SIMPLE(wots_param,n,LONG),
	ASN1_SIMPLE(wots_param,w,LONG),
	ASN1_SIMPLE(wots_param,len,LONG),
	ASN1_SIMPLE(wots_param,len_1,LONG),
	ASN1_SIMPLE(wots_param,len_2,LONG)
} ASN1_SEQUENCE_END(wots_param)
DECLARE_ASN1_FUNCTIONS(wots_param)
// Secret key
typedef struct {
	long idx;
	long n;
	wots_param *param;
	long h;
	ASN1_OCTET_STRING *PRF;
	ASN1_OCTET_STRING *root;
	ASN1_OCTET_STRING *seed;
	ASN1_OCTET_STRING *nodes;
	ASN1_OCTET_STRING *wots_keys;
	ASN1_OCTET_STRING *adrs;
} xmssasn1sk;
// Public key
typedef struct {
	long wots_n;
	long wots_w;
	long wots_len;
	long wots_len_1;
	long wots_len_2;
	long h;
	ASN1_OCTET_STRING *root;	
	ASN1_OCTET_STRING *seed;	
} xmssasn1pk;

ASN1_SEQUENCE(xmssasn1sk) ={
	ASN1_SIMPLE(xmssasn1sk,idx,LONG),
	ASN1_SIMPLE(xmssasn1sk,n,LONG),
	ASN1_SIMPLE(xmssasn1sk,param,wots_param),
	ASN1_SIMPLE(xmssasn1sk,h,LONG),
	ASN1_SIMPLE(xmssasn1sk,PRF,ASN1_OCTET_STRING),
	ASN1_SIMPLE(xmssasn1sk,root,ASN1_OCTET_STRING),
	ASN1_SIMPLE(xmssasn1sk,seed,ASN1_OCTET_STRING),
	ASN1_SIMPLE(xmssasn1sk,nodes,ASN1_OCTET_STRING),
	ASN1_SIMPLE(xmssasn1sk,wots_keys,ASN1_OCTET_STRING),
	ASN1_SIMPLE(xmssasn1sk,adrs,ASN1_OCTET_STRING)
}  ASN1_SEQUENCE_END(xmssasn1sk)
DECLARE_ASN1_FUNCTIONS(xmssasn1sk)

ASN1_SEQUENCE(xmssasn1pk) ={
	ASN1_SIMPLE(xmssasn1pk,wots_n,LONG),
	ASN1_SIMPLE(xmssasn1pk,wots_w,LONG),
	ASN1_SIMPLE(xmssasn1pk,wots_len,LONG),
	ASN1_SIMPLE(xmssasn1pk,wots_len_1,LONG),
	ASN1_SIMPLE(xmssasn1pk,wots_len_2,LONG),
	ASN1_SIMPLE(xmssasn1pk,h,LONG),
	ASN1_SIMPLE(xmssasn1pk,root,ASN1_OCTET_STRING),
	ASN1_SIMPLE(xmssasn1pk,seed,ASN1_OCTET_STRING)
}  ASN1_SEQUENCE_END(xmssasn1pk)
DECLARE_ASN1_FUNCTIONS(xmssasn1pk)


IMPLEMENT_ASN1_FUNCTIONS(wots_param)
IMPLEMENT_ASN1_FUNCTIONS(xmssasn1sk)
IMPLEMENT_ASN1_FUNCTIONS(xmssasn1pk)

// OCTET_STRING constructor
ASN1_OCTET_STRING *string_from( unsigned char *d, int len)
{
	ASN1_OCTET_STRING *a = ASN1_OCTET_STRING_new();
        int x = ASN1_OCTET_STRING_set(a, d, len);
	return a;
}

/* Serializes all the XMSS nodes without height information
 * since it can be recovered from the index
 */
ASN1_OCTET_STRING *getNodes(xmss_sk *sk)
{
	ASN1_OCTET_STRING *ret;
	int nmem = ((2 << sk->h)) - 1;
	int meml = sk->n;

	char *tmp = OPENSSL_malloc(meml*nmem);
	int h;
	for (h = 0; h < nmem ; h++)
	{
		memcpy(tmp+(meml*h),sk->nodes[h].node,meml);
	}
	ret = string_from((unsigned char*)tmp,nmem*meml);
	OPENSSL_free(tmp);
	return ret;
}

/* Implements the mapping of 
 *	1 2 3 4 5 6 7 8 9 ... 
 *  to   0 0 1 0 0 1 2 0 0 ...
 * needed to recover height information from 
 */
int getHeight(int i, int l, int h)
{
	if (i <= 2) return 0; // trivial case
	if (2 * l + 1 == i) return h; // 
	if (2 * l + 1 > i) return getHeight(i - l, 1, 1); // if it can't be 2*l+1 we cut of the first tree
	return getHeight(i, 2 * l + 1, h + 1); // l=2*l+1 h=h+1
}

/* == void retrieveNodes(xmss_sk *sk, unsigned char* data)
* Supposed to be used for reconstructing the xmss_node array 
* in the secret key
**/
void retrieveNodes(xmss_sk *sk, unsigned char* data)
{
	int nmem = ((2 << sk->h)) - 1;
	int meml = sk->n;
	int h;
	sk->nodes = OPENSSL_malloc(sizeof(xmss_node)*nmem);
	for (h = 0; h < nmem ; h++)
	{
		memcpy(sk->nodes[h].node, data+(meml*h), meml);
		sk->nodes[h].height = getHeight(h+1, 1, 1);
	}
}

// Encodes public key as a string and puts it in a X509_PUBKEY struct
static int xmss_pub_encode(X509_PUBKEY *pk, const EVP_PKEY *pkey)
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered xmss_pub_encode()\n");
	#endif
	XMSS_PKEY_CTX *xmss = (XMSS_PKEY_CTX*) pkey->pkey.ptr;
	void *pval = NULL;
	int ptype = V_ASN1_UNDEF;
	ASN1_STRING *penc=ASN1_STRING_new();
	#ifdef DEBUGFUNC
	fprintf(stderr,"[xmss.c] xmss_pub_encode: sk=%p pk=%p sig=%p\n",(void *)xmss->sk, (void *)xmss->pk, (void *)xmss->sig);
	#endif
	if (!xmss->pk) {
		if (!xmss->sk) {
			fprintf(stderr,"pk=%p pkey= %p\n", (void *)pk, (void *)pkey);
			abort();
		}
	}
	xmssasn1pk asn1;

	// The five next lines assign wots_param values to the ASN.1 structure
	asn1.wots_n=xmss->para->n;
	asn1.wots_w=xmss->para->w;
	asn1.wots_len=xmss->para->len;
	asn1.wots_len_1=xmss->para->len_1;
	asn1.wots_len_2=xmss->para->len_2;

	asn1.h = xmss->h;
	asn1.root=string_from(xmss->pk->root,xmss->para->n);
	asn1.seed=string_from(xmss->pk->seed,xmss->para->n);
	// i2d_TYPE converts an ASN.1 object in an internal standardized form
	// to its DER encoding and stuffs it into a character string
	penc->length = i2d_xmssasn1pk(&asn1,&penc->data); // encoding the mimic structure
	if (X509_PUBKEY_set0_param(pk, OBJ_nid2obj(NID_xmss),
                                ptype, pval, penc->data, penc->length)) {
               	return 1;
        }
	free(penc);
	return 0;
}

/* Decodes the private key into the EVP context from some internal structure that basically has 
 * the ASN.1 encoded string of the secret key
 */

static int xmss_priv_decode(EVP_PKEY *pkey, PKCS8_PRIV_KEY_INFO *p8)
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered xmss_priv_decode\n");
	#endif
	const unsigned char *p=NULL;
	int plen;
	PKCS8_pkey_get0(NULL, &p, &plen, NULL, p8);

	wots_param wp;
	xmssasn1sk a;
	xmssasn1sk *asn1=&a;
	a.PRF = ASN1_OCTET_STRING_new();
	a.wots_keys = ASN1_OCTET_STRING_new();
	a.nodes = ASN1_OCTET_STRING_new();
	a.root = ASN1_OCTET_STRING_new();
	a.seed = ASN1_OCTET_STRING_new();
	a.adrs = ASN1_OCTET_STRING_new();
	a.param=&wp;
	#ifdef DEBUGFUNC
	fprintf(stderr,"[xmss.c] xmss_priv_decode: p=%p plen=%d\n",p,plen);
	#endif
	// d2i_TYPE converts an ASN.1 object from its DER encoded form to its
	// internal standardized form. 
	// Next line: DER to internal (d2i) --> gives us ASN1
	d2i_xmssasn1sk(&asn1,(const unsigned char**)&p,plen);

	XMSS_PKEY_CTX *xmss = (XMSS_PKEY_CTX*) OPENSSL_malloc(sizeof(XMSS_PKEY_CTX));
	xmss_init(xmss);
	xmss->sk=OPENSSL_malloc(sizeof(xmss_sk));
        xmss->para=&xmss->sk->para;
        xmss->sk->root=OPENSSL_malloc(asn1->n);
        xmss->sk->seed=OPENSSL_malloc(asn1->n);
        xmss->sk->h = asn1->h;
        xmss->h = asn1->h;
        xmss->sk->idx = asn1->idx;
        xmss->sk->n = asn1->n;
        memcpy(xmss->para,&wp,sizeof(wots_param)); 
	memcpy(xmss->adrs, asn1->adrs->data, 32);

	xmss_sk_init(xmss->sk,&wp,5); 
	xmss->sk->SK_PRF=OPENSSL_malloc(asn1->n);
	memcpy(xmss->sk->SK_PRF,asn1->PRF->data,asn1->n);
	memcpy(xmss->sk->seed,asn1->seed->data,asn1->n);	
	memcpy(xmss->sk->root,asn1->root->data,asn1->n);
	memcpy(xmss->para,asn1->param,sizeof(wots_param)); 
	memcpy(&xmss->sk->para,asn1->param,sizeof(wots_param)); 
	xmss->sk->wots_keys=OPENSSL_malloc(              (1<<xmss->h)*xmss->para->len*xmss->para->n);
	memcpy(xmss->sk->wots_keys,asn1->wots_keys->data,(1<<xmss->h)*xmss->para->len*xmss->para->n);
	retrieveNodes(xmss->sk,asn1->nodes->data);

	xmss->pk = OPENSSL_malloc(sizeof(xmss_pk));
	xmss_pk_init(xmss->pk,xmss->para->n);
	memcpy(xmss->pk->root,xmss->sk->root,xmss->para->n);
	memcpy(xmss->pk->seed,xmss->sk->seed,xmss->para->n);

	EVP_PKEY_set1_XMSS(pkey,xmss);
	return 1;
}


/* Decode public key
 * since it is a string this is comperatively easy
 */
static int xmss_pub_decode(EVP_PKEY *pkey, X509_PUBKEY *pubkey)
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered xmss_pub_decode()\n");
	#endif
	unsigned char *p;
	int pklen;
	X509_ALGOR *palg;

	if (!X509_PUBKEY_get0_param(NULL,(const unsigned char**) &p, &pklen, &palg, pubkey))
	{
		fprintf(stderr,"X509_PUBKEY_get0_param() errored out %s:%d\n",__FILE__,__LINE__);  
		return 0;
	}

	xmssasn1pk a;
	xmssasn1pk *asn1=&a;
	a.root = ASN1_OCTET_STRING_new();
	a.seed = ASN1_OCTET_STRING_new();
	d2i_xmssasn1pk(&asn1,(const unsigned char **)&p,pklen);

	XMSS_PKEY_CTX *xmss = OPENSSL_malloc(sizeof(XMSS_PKEY_CTX));
	xmss_init(xmss);
	xmss->pk = OPENSSL_malloc(sizeof(xmss_pk));
	xmss->para = OPENSSL_malloc(sizeof(wots_param));
	wots_param_init(xmss->para, asn1->wots_n, asn1->wots_w);
	xmss->h	= asn1->h;
	xmss_pk_init(xmss->pk,xmss->para->n);
	memcpy(xmss->pk->seed,asn1->seed->data,asn1->wots_n);	
	memcpy(xmss->pk->root,asn1->root->data,asn1->wots_n);

	return EVP_PKEY_set1_XMSS(pkey,xmss);
}

// Encodes the secret key as a string and puts it into the pkcs8 structure
static int xmss_priv_encode(PKCS8_PRIV_KEY_INFO *p8, const EVP_PKEY *pkey)
{
	#ifdef DEBUGFUNC
	fprintf(stderr, "[xmss.c] Entered xmss_priv_encode\n");
	#endif
	ASN1_INTEGER *params = NULL;
        ASN1_STRING *prkey  = NULL;

	XMSS_PKEY_CTX *xmss = (XMSS_PKEY_CTX*)pkey->pkey.ptr;
        if (!xmss || !xmss->sk)
        {
		if (xmss) xmss_init(xmss);
		else fprintf(stderr,"ERROR! %s:%d %p\n",__FILE__,__LINE__, (void *)xmss); 
        }

	// Filling our mimic structures for ASN1
	xmssasn1sk asn1;
	asn1.h=xmss->sk->h;
	asn1.idx=xmss->sk->idx;
	asn1.n=xmss->sk->n;
	asn1.param=&xmss->sk->para;
	asn1.PRF=string_from(xmss->sk->SK_PRF, asn1.n);
	asn1.root=string_from(xmss->sk->root, asn1.n);
	asn1.seed=string_from(xmss->sk->seed, asn1.n);
	asn1.adrs=string_from(xmss->adrs, 32);
	asn1.wots_keys=string_from(xmss->sk->wots_keys,(1<<asn1.h)*xmss->para->len*asn1.n);
	asn1.nodes=getNodes(xmss->sk);

	prkey=ASN1_STRING_new();
	prkey->length = i2d_xmssasn1sk(&asn1,&prkey->data); // Encoding the mimic structure
	if (prkey->length <= 0)
	{
		fprintf(stderr,"ERROR! %s:%d\n",__FILE__,__LINE__); 
		abort();
	}

	params=ASN1_INTEGER_new();
	ASN1_INTEGER_set(params,NID_xmss);
	if (!PKCS8_pkey_set0(p8, OBJ_nid2obj(NID_xmss), 0,
                                V_ASN1_NULL,0, prkey->data, prkey->length))
	{
		fprintf(stderr,"[xmss.c] Error: xmss_priv_encode() not done yet! %s:%d\n",__FILE__,__LINE__); 
		return 0;
        }

	return 1;
}

// Returns number of bytes per signatures as per the reference implementation
static int int_xmss_size(const EVP_PKEY *pkey)
{
	XMSS_PKEY_CTX *xmss = (XMSS_PKEY_CTX*)pkey->pkey.ptr;
	int n, len, h, w;
	n = xmss->para->n;
	h = xmss->h;
	w = xmss->para->w;
	unsigned int len1 = calcLen1(w, n);
	len = calcLen(len1, calcLen2(w, len1));
	int siglen = 4 + n + (n * len) + (h * n);
	#ifdef DEBUGFUNC
	fprintf (stderr,"[xmss.c] int_xmss_size: xmss_size=%d n=%d,len=%d,h=%d,w=%d\n", siglen, n, len, h, w);
	#endif
	return siglen;
}

// A destructor
static void int_xmss_free(EVP_PKEY *pkey)
{
// TODO: implement this to fix memleaks
//		XMSS_PKEY_CTX *xmss = (XMSS_PKEY_CTX*)pkey->pkey.ptr;
//		if (xmss->sk)	xmss_sk_free(xmss->sk);
//		if (xmss->pk)   xmss_pk_free(xmss->pk);
//		if (xmss->sig) 	xmss_sig_free(xmss->sig);
//		OPENSSL_free(xmss);
		return;
}

// This function is called from the X509 context and prints the public key
// as we know them from x509 certs xx:xx:xx:xx...
// Parameter pctx not used, but required by ameth
static int xmss_pub_print(BIO *bp, const EVP_PKEY *pkey, int indent,
							ASN1_PCTX *ctx)
{
	(void)(ctx); // Silence unused variable warning
	XMSS_PKEY_CTX *xmss = (XMSS_PKEY_CTX*)pkey->pkey.ptr;
	int n = 32;
	if (xmss->para) 
		n = xmss->para->n;
	else {
		fprintf(stderr,"xmss->para not set! %s:%d",__FILE__,__LINE__); 
		abort();
	}
	unsigned char *pk = OPENSSL_malloc(xmss->para->n*2);
	xmssPKtoString(pk, xmss->pk, n);
	int i;
	for (i = 0; i < n * 2; i++){
		if (i % 16 == 0) {
			if (i) BIO_write(bp, "\n", 1);
			int j;
			for (j=0; j < indent; j++)
				BIO_write(bp, " ", 1);
		}
		char buf[10];
		sprintf(buf, "%02x:", pk[i]);
		BIO_write(bp, buf, 3);
	}
	BIO_write(bp,"\n",1);
	OPENSSL_free(pk);
	return 1;	
}

// Print the signature for the X.509 certificate as in openssl x509 -text
// Parameters sigalg and pctx not used, but required by ameth
static int xmss_sig_print(BIO *bp, const X509_ALGOR *sigalg,
			const ASN1_STRING *sig, int indent, ASN1_PCTX *pctx)
{
	(void)(sigalg); // Silence unused variable warning
	(void)(pctx); // Silence unused variable warning
	if (!sig)
	{
        	if (BIO_puts(bp, "\n") <= 0) return 0; else return 1;
	}

	ASN1_STRING *ssig = (ASN1_STRING*) sig;

	if (BIO_write(bp, "\n", 1) != 1)
	{
		fprintf(stderr,"IMPLEMENT %s:%d",__FILE__,__LINE__); 
		abort();
	}
	int i = 0;
	fprintf(stderr, "[xmss.c] xmss_sig_print: ssig->length = %d\n", ssig->length);
	for (i = 0; (i < ssig->length); i++) {
		if (i % 32 == 0) {
			if (i) BIO_write(bp, "\n", 1);
			int j = 0;
			for (j = 0; j < indent; j++)
				BIO_write(bp, " ", 1);
		}
		char buf[4];
		sprintf(buf, "%02x:", ssig->data[i]);
		BIO_write(bp, buf, 3);
	}
	BIO_write(bp,"\n",1);
	return 1;
}

int xmss_pub_cmp(const EVP_PKEY *a, const EVP_PKEY *b)
{
	fprintf(stderr,"xmss_pub_cmp called in %s:%d\n",__FILE__,__LINE__);
	XMSS_PKEY_CTX *xmssa = (XMSS_PKEY_CTX*)a->pkey.ptr;
	XMSS_PKEY_CTX *xmssb = (XMSS_PKEY_CTX*)b->pkey.ptr;
	return memcmp(xmssa->pk->root,xmssb->pk->root,xmssa->para->n) == 0;
}

// Loads ASN.1 methods at runtime
void load_ameth()
{
	static EVP_PKEY_ASN1_METHOD x=
	{
		NID_xmss, // pkey_id
		NID_xmss, // pkey_base_id

		0, // pkey_flags

		"xmssWithSHA256", // pem_str
		"OpenSSL XMSS method", // info

		xmss_pub_decode, // pub_decode
		xmss_pub_encode, // pub_encode
	        xmss_pub_cmp, // pub_cmp
		xmss_pub_print, // pub_print
		xmss_priv_decode, // priv_decode
		xmss_priv_encode, // priv_encode
		0, // priv_print

		int_xmss_size, // pkey_size
		0, // pkey_bits

		0, // param_decode
		0, // param_encode
		0, // param_missing
		0, // param_copy
		0, // param_cmp
		0, // param_print
		xmss_sig_print, // sig_print

		int_xmss_free, // pkey_free
		0, // pkey_ctrl
    /* Legacy functions for old PEM */
		0, // old_priv_decode
		0, // old_priv_encode
    /* Custom ASN1 signature verification */
		0, // item_verify
		0 // item_sign
	};
	EVP_PKEY_asn1_add0(&x);
}

// This adds the functionality at runtime
void load_xmss_all()
{
	OpenSSL_add_all_algorithms();
// Removed in order to be able to hook it
	load_ameth();
	load_pmeth();
	// A few OBJ such that x509 cert doesn't throw errors when printed
int newNID = OBJ_create("1 3 6 1 4 1 8301 3 1 3 3 1", "xmssWithSHA256", "xmssWithSHA256");
	if(!newNID)
	{
		fprintf(stderr,"[xmss.c] ERROR! %s:%d\n",__FILE__,__LINE__);  
		abort();
	}

	fprintf(stderr, "[xmss.c] OBJ_create returned this NID: %d\n", newNID);
	
	if(!OBJ_add_sigid(NID_xmss, NID_sha256, NID_xmss))
	{
		fprintf(stderr,"[xmss.c] ERROR! %s:%d\n",__FILE__,__LINE__);
		abort();
	}
}
