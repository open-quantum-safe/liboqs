/* expect-fail: EVP return value not checked via OQS_OPENSSL_GUARD */
#include <openssl/evp.h>
void bad_example(EVP_MD_CTX *ctx, const EVP_MD *md) {
    EVP_DigestInit_ex(ctx, md, NULL);
}
