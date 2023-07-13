#if !defined( HASH_H__ )
#define HASH_H__
#include "sha256.h"
#include <stddef.h>
#include <stdbool.h>

/*
 * This defines the hash interface used within HSS.
 * All globals are prefixed with hss_ to avoid name conflicts
 * Gee, C++ namespaces would be nice...
 */

/*
 * Hash types
 */
enum {
    HASH_SHA256 = 1,    /* SHA256 */
};

union hash_context {
    SHA256_CTX sha256;
    /* Any other hash contexts would go here */
};

/* Hash the message */
void hss_hash(void *result, int hash_type,
          const void *message, size_t message_len);

/* Does the same, but with the passed hash context (which isn't zeroized) */
/* This is here to save time; let the caller use the same ctx for multiple */
/* hashes, and then finally zeroize it if necessary */
void hss_hash_ctx(void *result, int hash_type, union hash_context *ctx,
          const void *message, size_t message_len);

/*
 * This is a debugging flag; turning this on will cause the system to dump
 * the inputs and the outputs of all hash functions.  It only works if
 * debugging is allowed in hash.c (it's off by default), and it is *real*
 * chatty; however sometimes you really need it for debugging
 */
extern bool hss_verbose;

/*
 * This constant has migrated to common_defs.h
 */
/* #define MAX_HASH   32 */  /* Length of the largest hash we support */

unsigned hss_hash_length(int hash_type);
unsigned hss_hash_blocksize(int hash_type);

void hss_init_hash_context( int h, union hash_context *ctx );
void hss_update_hash_context( int h, union hash_context *ctx,
                          const void *msg, size_t len_msg );
void hss_finalize_hash_context( int h, union hash_context *ctx,
                          void *buffer);

#endif /* HASH_H__  */
