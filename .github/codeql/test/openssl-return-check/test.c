// SPDX-License-Identifier: MIT
// Test cases for queries/openssl-return-check.ql.
// Self-contained: OpenSSL is stubbed so no headers are needed.

typedef struct evp_md_ctx_st EVP_MD_CTX;
typedef struct evp_md_st EVP_MD;

int EVP_DigestInit_ex(EVP_MD_CTX *ctx, const EVP_MD *type, void *impl);
int EVP_DigestUpdate(EVP_MD_CTX *ctx, const void *d, unsigned long cnt);
EVP_MD_CTX *EVP_MD_CTX_new(void);
void EVP_MD_CTX_free(EVP_MD_CTX *ctx);
void exit(int status);

#define OQS_OPENSSL_GUARD(x) \
	do {                     \
		if (1 != (x)) {      \
			exit(1);         \
		}                    \
	} while (0)

void bad_discarded(EVP_MD_CTX *ctx, const EVP_MD *md) {
	EVP_DigestInit_ex(ctx, md, 0); // BAD
	EVP_DigestUpdate(ctx, "x", 1); // BAD
}

void good_guarded(EVP_MD_CTX *ctx, const EVP_MD *md) {
	OQS_OPENSSL_GUARD(EVP_DigestInit_ex(ctx, md, 0));
}

int good_tested(EVP_MD_CTX *ctx, const EVP_MD *md) {
	if (EVP_DigestInit_ex(ctx, md, 0) != 1) {
		return -1;
	}
	int ok = EVP_DigestUpdate(ctx, "x", 1);
	return ok ? 0 : -1;
}

void good_explicitly_discarded(EVP_MD_CTX *ctx) {
	(void)EVP_DigestUpdate(ctx, "x", 1);
}

void good_pointer_and_void_returns(void) {
	EVP_MD_CTX *ctx = EVP_MD_CTX_new();
	EVP_MD_CTX_free(ctx);
}
