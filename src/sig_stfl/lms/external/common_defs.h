#if !defined( COMMON_DEFS_H_ )
#define COMMON_DEFS_H_

/*
 * These are defintions for the LMS implementation that are common throughout
 * the system (and so are collected in one place)
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_HASH   32 /* Length of the largest hash we support */

/* The I (Merkle tree identifier) value is 16 bytes long */
#define I_LEN               16

/* The maximum height of a Merkle tree */
#define MAX_MERKLE_HEIGHT       25

/* The mininum height of a Merkle tree.  Some of our update logic assumes */
/* this isn't too small */
#define MIN_MERKLE_HEIGHT        5

/* The minimum/maximum number of levels of Merkle trees within an HSS trees */
#define MIN_HSS_LEVELS 1    /* Minumum levels we allow */
#define MAX_HSS_LEVELS 8    /* Maximum levels we allow */

/* This is the length of our internal seed values */
#define SEED_LEN 32         /* Enough to make Grover's infeasible */

/* Here are some internal types used within the code.  They are listed more */
/* for documentation ("this is what this variable is expected to be") rather */
/* than to let the compiler do any sort of type checking */

    /* This is an index into a Merkle tree */
    /* Used for both the leaf index (0..N-1) and the node number (1..2*N-1), */
    /* where N is the size 2**h of the tre */
#if MAX_MERKLE_HEIGHT > 31
    /* We need to express more than 32 bits in this type */
typedef uint_fast64_t merkle_index_t;
#error We need to extend the id we place within a hash to more than 4 bytes
#else
typedef uint_fast32_t merkle_index_t;
#endif

    /* This is the name of a parameter set */
    /* Used for both an OTS parameter set or an LM parameter set */
    /* Both are 32 bits */
typedef uint_fast32_t param_set_t;

    /* This is a sequence number over an HSS tree */
    /* This means we can never generate more than 2**64 signatures from a */
    /* private key (even if the parameter set would, in theory, allow us */
    /* to do more) */
typedef uint_fast64_t sequence_t;

/* Defined LM parameter sets */
#define LMS_SHA256_N32_H5  0x00000005
#define LMS_SHA256_N32_H10 0x00000006
#define LMS_SHA256_N32_H15 0x00000007
#define LMS_SHA256_N32_H20 0x00000008
#define LMS_SHA256_N32_H25 0x00000009

/* LM-OTS registry */
#define LMOTS_SHA256_N32_W1 0x00000001
#define LMOTS_SHA256_N32_W2 0x00000002
#define LMOTS_SHA256_N32_W4 0x00000003
#define LMOTS_SHA256_N32_W8 0x00000004

/*
 * Internal formats of various hashes
 *
 * We do a number of different hashes as a part of this package; some
 * specified by the draft, some specific to us.
 * For each such hash, we list the values being hashed, and the offset
 * from the start where they go.  We treat them as indicies into unsigned char
 * arrays, and not structs, to avoid any potential padding issues with structs
 *
 * For a hash of type XXXX, XXXX_Z is the offset where component Z goes,
 * XXXX_LEN(hash_len) is the length being hashed (assuming that hash length),
 * XXXX_MAXLEN is the maximum length it can be (for allocation), and D_XXXX
 * is the hash distinguisher (the value that makes it different from any other
 * hash)
 */

/* The initial message hashing */
#define MESG_I    0
#define MESG_Q   16
#define MESG_D   20   /* The fixed D_MESG value */
#define MESG_C   22
#define MESG_PREFIX_LEN(n) (MESG_C + (n))  /* Length not counting the actual */
                                         /* message being signed */
#define MESG_PREFIX_MAXLEN MESG_PREFIX_LEN(MAX_HASH)
#define D_MESG 0x8181

/* The Winternitz iteration hashes */
#define ITER_I    0
#define ITER_Q   16
#define ITER_K   20   /* The RFC uses i here */
#define ITER_J   22
#define ITER_PREV 23  /* Hash from previous iteration; RFC uses tmp */
#define ITER_LEN(hash_len) (ITER_PREV + (hash_len))
#define ITER_MAX_LEN ITER_LEN(MAX_HASH)

/* Hashing the OTS public key */
#define PBLC_I    0
#define PBLC_Q   16
#define PBLC_D   20   /* The fixed D_PBLC value */
#define PBLC_PREFIX_LEN 22   /* Not counting the OTS public keys */
#define D_PBLC 0x8080

/* Hashing Merkle tree leaf nodes */
#define LEAF_I    0
#define LEAF_R   16
#define LEAF_D   20
#define LEAF_PK  22
#define LEAF_LEN(root_len) (LEAF_PK + (root_len))
#define LEAF_MAX_LEN LEAF_LEN(MAX_HASH)
#define D_LEAF 0x8282

/* Hashing Merkle tree internal nodes */
#define INTR_I    0
#define INTR_R   16
#define INTR_D   20
#define INTR_PK  22
#define INTR_LEN(root_len) (INTR_PK + 2 * (root_len))
#define INTR_MAX_LEN INTR_LEN(MAX_HASH)
#define D_INTR 0x8383

/* The determanistic key generation */
/* Also used to generate subkeys in the j-tree hierarchy */
/* As we'll always do either one or the other, we can reuse the structure */
/* for both purposes */
#define PRG_I     0
#define PRG_Q    16
#define PRG_J    20
#define PRG_FF   22   /* A fixed 0xff goes here */
#define PRG_SEED 23
#define PRG_LEN(seed_len) (23 + (seed_len))
#define PRG_MAX_LEN PRG_LEN(MAX_HASH)

/* The below are hash formats that the draft does not list, but we */
/* implement ourselves (largely because we need to be determanistic */
/* based on the seed) */

/* Hash used to generate subkeys in the q tree hierarchy */
#define QTREE_I    0
#define QTREE_Q   16
#define QTREE_D   20   /* D_QTREE goes here */
#define QTREE_SEED 22
#define QTREE_LEN (22 + 32)       /* We assume a fixed length seed */
#define QTREE_MAX_LEN QTREE_LEN
#define D_QTREE 0xffff

/* Hash used to generate the master seed for the top level Merkle tree */
#define TOPSEED_I    0  /* 16 0's here (we don't have an I value) */
#define TOPSEED_Q   16  /* 0's here (as we don't have a Q value) */
#define TOPSEED_D   20  /* D_TOPSEED */
#define TOPSEED_WHICH 22 /* 0 -> Gen Master seed (used as seed for */
                         /*      the next two) */
                         /* 1 -> Create top level seed */
                         /* 2 -> Create top level I */
#define TOPSEED_SEED 23  /* 32 bytes long */
#define TOPSEED_LEN (TOPSEED_SEED + 32)
#define D_TOPSEED 0xfefe

/* Hash used to generate the key used for the authenticating the aux values */
#define DAUX_I    0   /* 16 0's here (no I value) */
#define DAUX_Q   16   /* 4 more 0's here (no Q value) */
#define DAUX_D   20   /* D_AUX_SEED_DERIVE */
#define DAUX_PREFIX_LEN 22  /* Not counting the seed value */
#define D_DAUX 0xfdfd

/* Macro to set the D_XXXX value to the XXXX_D offset */
#define SET_D(p, value) (void)(((p)[0] = (value) >> 8),   \
                               ((p)[1] = (value) & 0xff))

#endif /* COMMON_DEFS_H_ */
