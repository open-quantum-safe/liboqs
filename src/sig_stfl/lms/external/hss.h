// SPDX-License-Identifier: MIT
#if !defined(HSS_H_)
#define HSS_H_

#include <stdlib.h>
#include <stddef.h>
#include "common_defs.h"
#include "lms_namespace.h"

/*
 * This is intended to be a usable (nontoy) implementation of the LMS
 * signature scheme.  The public data (public keys, signatures) are
 * precisely the same as the standard LMS implmentation; however it
 * strives to be more usable, in the following ways:
 *
 * - During signature generation time, it incrementally computes the next
 *   trees; that means that it doesn't need to generate the next Merkle tree
 *   from scratch on the 1025th signature.
 * - It doesn't try to hold the entire Merkle tree in memory; hence a level
 *   25 Merkle tree doesn't need to save 2**25 internal node values.  This
 *   does increase the time to generate the next siganture (as we will need
 *   to recompute some internal nodes); however by only a small constant factor
 * - It divides the private key into three parts, only one of which needs to
 *   be kept secret, and updated dynamically; the other parts are a working
 *   copy (that can be kept in RAM, and can be dynamically regenerated as
 *   needed), and some optional static (nonprivate) data (which can speed up
 *   the regeneration process)
 * - API to explicitly reserve the next N signatures (so that we don't need
 *   to update the secure storage copy quite as often)
 *
 *
 * We use a nonflat memory structure for the working_key.  Part of the reason
 * we use a flat representation elsewhere is so that they can be written (and
 * later read) to/from disk as required; we specifically assume that the
 * working_key is never written to disk. And, being able to use C structures
 * makes this rather nontrivial structure a bit more transparent
 *
 * Here is the intended order of usage:
 * Step 1: generate the private/public keypair:
 *   The API to do this is hss_generate_private_key; this is done once per
 *   private key; and you should write the private key to secure storage
 *   (which the passed update_private_key function could do)
 *
 * Step 2: (which you can do per restart):
 *   Load the private keypair into memory: hss_load_private_key
 *
 * Step 3: generate signatures (which you can do lots of time after you've
 *      loaded the key into memory):
 *   The API to do this is hss_generate_signature.  Note that this needs
 *   to update the private key state; the update_private_key function pointer
 *   can be useful here
 *
 * Step 4: (when you're done with the loaded private key; optional)
 *   Free the ephemeral copy (hss_free_working_key).  Note that this is not
 *   required for correctness; this just does a free()
 *
 *
 * One can also verify signatures at any time; all that needs is a public
 * key, a signature and a message; it's not a part of the intended order
 * of usage
 */

struct hss_extra_info;

/*
 * This will generate a fresh (unadorned) private key, with the selected
 * parameter set, the corresponding public key, and (optionally) the aux_data
 * that is associated with the private key.
 *
 * The generate_random function will be called when this function needs
 * random values; it is assumed to generate cryptographically secure ones.
 * We ask you to pass a function, rather than an array of random values,
 * to emphasize that we really do need fresh random data here; the security
 * of this entire system depends on it.
 *
 * levels, lm_type, lm_ots_type is the parameter set for the new key.
 * levels is the number of levels in the HSS hierarchy (1-8), while
 * lm_type[], lm_ots_type[] are arrays giving the parameter set of each
 * individual level; level i of the hierarchy will have LMS parameter set
 * lm_type[i] and OTS parameter set lm_ots_type[i] (where i=0 is the topmost
 * Merkle tree.
 *
 * The update_private_key function will be called when the private key is
 * generated; it is expected to write the private key to secure storage (and
 * the context pointer is a value that is passed to the update_private_key
 * function; it can be used to tell the update_private_key function where
 * in the secure storage to place the key).  If the passed update_private_key
 * function pointer is NULL, the private will will be written to the context
 * pointer (which is expected to hold 48 bytes of data)
 *
 * public_key is where the freshly generated public key will be placed, and
 * len_public_key is the size of the array (and this will generate an error
 * if the public key is larger than the array).  See the hss_get_public_key_len
 * function for the expected length of the public key
 *
 * aux_data is where to place internal nodes of the Merkle tree, and
 * len_aux_data is the length of the provided buffer.  This aux_data
 * is optional (pass in a NULL if it's not being used), but does significantly
 * speed the generate_working_key process.  It's envisioned use is to write
 * this aux_data to disk, and reread it when it's time to regenerate the
 * ephemeral key; it need not be kept in secure storage; revealing it doesn't
 * help an attacker to generate forgeries, and if an attacker does manage to
 * corrupt it, the regeneration process will detect the corruption and ignore
 * it.  Also, even if writing it to disk is not possible, passing in a
 * small array here and passing that to the initial regeneration call will
 * speed that up (and later ones can omit it; those will go slow, but at
 * least you got the speed up benefit the first time).
 *
 * One slightly tricky thing about aux data is that the required length of the
 * aux data; there are several different possible time/memory trade-offs.
 * Depending on the length, we'll automatically pick the fastest option that
 * fits. If we have N bytes available total, see hss_get_aux_data_len for
 * the amount of data we'll actually use (and so the amount you need to write
 * to disk)
 */
bool hss_generate_private_key(
    bool (*generate_random)(void *output, size_t length),
    unsigned levels,
    const param_set_t *lm_type, const param_set_t *lm_ots_type,
    bool (*update_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
        void *context,
    unsigned char *public_key, size_t len_public_key,
    unsigned char *aux_data, size_t len_aux_data,
    struct hss_extra_info *info);

/*
 * This is the routine to load a private key into memory, and
 * initialize the working data structures; these data structures
 * allow us to generate signtures quickly
 *
 * The read_private_key is a function to read the private key from secure
 * storage, with context being a value passed to that function.
 * If the read_private_key pointer is NULL, we assume that the context
 * pointer points to the private key.
 * This assumes that the key has already been generated by
 * hss_generate_private_key
 *
 * memory_target is a value which gives a goal for the amount of memory (in
 * bytes) that this structure should take up.  There are a number of
 * time/memory trade-offs possible; the function uses this parameter as a
 * guide as to what trade-offs it should take.  This structure tries to
 * allocate no more than memory_target bytes; however it is considered
 * advisatory; this function will never fail beccause memory_target was too
 * small (so passing 0 will work, and will minimize the memory used)
 *
 * aux_data points to a buffer containing the auxiliary data generated
 * during the key generation process, with len_aux_data being the length
 * of the buffer.  Passing it a NULL means that we're not providing that
 * data (which is fine; it just means this will take longer)
 *
 * On success, this malloc's the ephemeral key (struct hss_working_key*) and
 * retursn it.  Because it mallocs it, it asssumes that the caller will
 * eventually free it (via the hss_free_working_key function, don't try
 * calling free() yourself)
 */
struct hss_working_key;
struct hss_working_key *hss_load_private_key(
    bool (*read_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
        void *context,
    size_t memory_target,
    const unsigned char *aux_data, size_t len_aux_data, /* Optional */
    struct hss_extra_info *info);

/*
 * Corresponding function to free the working key
 */
void hss_free_working_key( struct hss_working_key * );

/*
 * This will actually generate a signature
 *
 * working_key is the key that has been allocated by allocate_working_key and
 * initialied by hss_generate_working_key
 *
 * The update_private_key function will be called when the private key is
 * updated; it is expected to write the private key to secure storage (and the
 * context pointer is a value that is passed to the update_private_key
 * function; it can be used to tell the update_private_key function where
 * in the secure storage to place the key).  And, if it is NULL, the context
 * is expected to point to a copy of the private_key in RAM.
 * One distinction is that, on an update, len_private_key will be 8;
 * the update_private_key can choose to update only the first 8 bytes
 * of the private key (the rest will be unchanged), or write all
 * 48 bytes (private_key will point to the full 48 byte value)
 *
 * message, message_len are the message being signed
 *
 * signature is where the signature will be written, with signature_len being
 * the length of the buffer.  See the hss_get_signature_len function for the
 * expected signature length for this parameter set; if signature_len is too
 * short for the signature to fit, this will fail.
 */
bool hss_generate_signature(
    struct hss_working_key *working_key,
    bool (*update_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
    void *context,
    const void *message, size_t message_len,
    unsigned char *signature, size_t signature_len,
    struct hss_extra_info *info);

/*
 * See hss_verify.h for the signature verfication routine; it's in a
 * separate file for those programs that only need to verify a signature
 */
#include "hss_verify.h"

/*
 * Lower level routines to allocate and initialize a working key.
 *
 * hss_load_working_key will do the work of the below routines; these are
 * provided separately in case you need more control (e.g. reuse an already
 * allocated working key)
 *
 * First, the routine to allocate (but not initialize) a working key.
 *
 * The levels/lm_type/lm_ots_type are the same parameter sets as in the
 * generate public/private keypair call; the parameter set must match the
 * values for the private key.
 *
 * memory_target is a value which gives a goal for the amount of memory that
 * this structure should take up.  There are a number of time/memory trade-offs
 * possible; the function uses this parameter as a guide as to what trade-offs
 * it should take.  This structure tries to allocate no more than memory_target
 * bytes; however it is considered advisatory; this function will never fail
 * beccause memory_target was too small (so passing 0 will work, and will
 * minimize the memory used)
 */
struct hss_working_key *allocate_working_key(
    unsigned levels,
    const param_set_t *lm_type, const param_set_t *lm_ots_type,
    size_t memory_target,
    struct hss_extra_info *info);

/*
 * This is called on reload (or initial key generation), it'll take the
 * working key that's been allocated by allocate_working_key, and initialize
 * it based on the private key; this working key is what we need to actually
 * generate signatures.
 *
 * The read_private_key is a function to read the private key from secure
 * storage, with context being a value passed to that function.
 * If NULL, we assume that the context pointer points to the private key
 *
 * aux_data points to a buffer containing the auxiliary data generated
 * during the key generation process, with len_aux_data being the length
 * of the buffer.  Passing it a NULL means that we're not providing that
 * data (which is fine; it just means this will take longer)
 *
 * working_key is a pointer to the allocated working key
 */
bool hss_generate_working_key(
    bool (*read_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
        void *context,
    const unsigned char *aux_data, size_t len_aux_data,  /* Optional */
    struct hss_working_key *working_key,
    struct hss_extra_info *info);

/*
 * This will make sure that (at least) N signatures are reserved; that is, we
 * won't need to actually call the update function for the next N signatures
 * generated
 *
 * This can be useful if the update_private_key function is expensive.
 *
 * Note that if, N (or more) signatures are already reserved, this won't do
 * anything.
 */
bool hss_reserve_signature(
    struct hss_working_key *w,
    bool (*update_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
    void *context,
    unsigned sigs_to_reserve,
    struct hss_extra_info *info);

/*
 * This will set the autoreserve, so that when the signing process runs out,
 * it will automatically reserve N more signatures (in addition to the one
 * that is being used for the current signature)
 *
 * This can be useful if the update_private_key function is expensive,
 * setting sigs_to_autoreserve=99 means will actually update the private
 * key once every 100 signatures
 */
bool hss_set_autoreserve(
    struct hss_working_key *w,
    unsigned sigs_to_autoreserve,
    struct hss_extra_info *info);

/*
 * This returns the required lengths for the various objects we export
 *
 * This is the length of the private key (which is written to secure storage)
 */
size_t hss_get_private_key_len(unsigned levels,
                   const param_set_t *lm_type,
                   const param_set_t *lm_ots_type);
#define HSS_MAX_PRIVATE_KEY_LEN (8 + 8 + SEED_LEN + 16)

/*
 * This include file has the functions that contains the lengths of the other
 * public objects
 */
#include "hss_common.h"

/*
 * Get the signature length.  We don't put this in hss_common because we
 * assume we have a loaded private key
 * Returns 0 on error
 */
size_t hss_get_signature_len_from_working_key(
                                        struct hss_working_key *working_key);

/*
 * This returns the amount of aux data we use
 * This is slightly different from the above routines; given the bound on the
 * amount of data the aux_data is allowed to take (max_length), this returns
 * the amount of data we'll actually use
 */
size_t hss_get_aux_data_len(size_t max_length,
                   unsigned levels,
                   const param_set_t *lm_type,
                   const param_set_t *lm_ots_type);

/*
 * This returns the parameter set for a given private key.
 * This is here to solve a chicken-and-egg problem: the hss_working_key
 * must be initialized to the same parameter set as the private key,
 * but (other than this function, or somehow remembering it) there's
 * no way to retreive the parameter set.
 *
 * read_private_key/context will read the private key (if read_private_key is
 * NULL, context is assumed to point to the private key)
 *
 * On success, *levels will be set to the number of levels, and lm_type[]
 * and lm_ots_type[] will be set to the lm/ots parameter sets
 *
 * On success, this returns true; on failure (can't read the private key, or
 * the * private key is invalid), returns false
 */
bool hss_get_parameter_set( unsigned *levels,
                           param_set_t lm_type[ MAX_HSS_LEVELS ],
                           param_set_t lm_ots_type[ MAX_HSS_LEVELS ],
                           bool (*read_private_key)(unsigned char *private_key,
                                       size_t len_private_key, void *context),
                           void *context);

enum hss_error_code {
    hss_error_none = 0,      /* I don't know nothing about any error */

    hss_range_normal_failures, /* There errors happen during normal use */
                             /* of the signature scheme */
    hss_error_bad_signature, /* Invalid signature */
    hss_error_private_key_expired, /* This private key has generated all */
                             /* the signatures it is allowed */
    hss_error_not_that_many_sigs_left, /* Reservation request failed */
                             /* because the key couldn't do that many  */
                             /* signatures */

    hss_range_bad_parameters, /* These errors are cause by the */
                          /* application passing in a bad parameter */
    hss_error_no_randomness, /* No RNG supplied */
    hss_error_bad_param_set, /* Application asked for an illegal parmaeter */
                             /* set */
    hss_error_buffer_overflow, /* Buffer provide not big enough */
    hss_error_got_null,      /* Application passed in a NULL pointer */
    hss_error_bad_aux,       /* Error with provided aux buffer */
    hss_error_no_private_buffer, /* Application didn't provide a place */
                             /* to put the private key */
    hss_error_incompatible_param_set, /* The parameter set of the working */
                             /* set didn't agree with the private key */
    hss_error_key_uninitialized, /* The working key used had never been */
                             /* initialized with a private key */
    hss_error_key_mismatch,  /* The working set and the private key */
                             /* do not correspond */
    hss_error_ctx_uninitialized, /* The incremental ctx wasn't initialized */
                             /* properly */
    hss_error_ctx_already_used, /* The ctx has already been used */
    hss_error_bad_public_key, /* Somehow, we got an invalid public key */

    hss_range_processing_error, /* These errors are cause by an */
                             /* error while processing */
    hss_error_bad_randomness, /* The RNG claimed failure */
    hss_error_private_key_write_failed, /* The write of the private key */
                             /* to NVRAM failed */
    hss_error_private_key_read_failed, /* The read of the private key */
                             /* from NVRAM failed */
    hss_error_out_of_memory, /* A malloc failure caused us to fail */

    hss_range_my_problem,    /* These are caused by internal errors */
                             /* within the HSS implementation */
    hss_error_internal,      /* Some internal assertion failed (should */
                             /* never happen) */
};

/*
 * This is the structure that allows us to pass noncritical information
 * to and from the above routines (without requiring us to add each
 * one as an additional parameter
 */
struct hss_extra_info {
    int num_threads;     /* Number of threads we're allowed to ues */
    bool last_signature; /* Set if we just signed the last signature */
                         /* allowed by this private key */
    enum hss_error_code error_code; /* The more recent error detected */
};

/* Accessor APIs in case someone doesn't feel comfortable about reaching */
/* into the structure */
void hss_init_extra_info( struct hss_extra_info * );
void hss_extra_info_set_threads( struct hss_extra_info *, int );
bool hss_extra_info_test_last_signature( struct hss_extra_info * );
enum hss_error_code hss_extra_info_test_error_code( struct hss_extra_info * );

#endif /* HSS_H_ */
