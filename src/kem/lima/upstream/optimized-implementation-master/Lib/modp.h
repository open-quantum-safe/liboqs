#ifndef _modp
#define _modp

/* 
 * This file defines the functions to perform
 * arithmetic mod q.
 * 
 * This is the Montgomery implementation. In all
 * cases we use R=2^32
 *
 * To keep the API the same we therefore pass each
 * arithmetic operation a ctx pointer which defines
 * various constants for arithmetic mod q. In the reference
 * implementation this is (pretty) redundant.
 */

#include <stdint.h>

typedef struct tag_modp_context_t
{
    int q;
    int log256_q;                    // Number of bytes in q
    uint32_t R,R2,qinv;              // Montgomery data R mod q, R^2 mod q, -1/q mod R
} modp_context_t;


/*  Function to convert the integer x to a mod q integer 
 */
int to_mod_q(modp_context_t* ctx, int x);

/* Function to convert the internal mod q representation
 *   into a normal integer
 *   - Needed as in optimized variant the representations
 *     are different, really a NOP for the reference 
 *     implementation
 */
int to_int(modp_context_t* ctx, int x);


/* Returns the Montgomery representation of one,
 *   i.e. R
 */
int assign_one(modp_context_t* ctx);

/*  Function to get  c=a*b mod q
 */
int mul_mod_q(modp_context_t* ctx, int a, int b);

/* As above but does not do the final mod reduction
 */
int mul_mod_q_tag(modp_context_t* ctx, int a, int b);

/*  Function to get c=a+b mod q
 */
int add_mod_q(modp_context_t* ctx, int a, int b);

/*  Function to get d=a*b+c mod q
 */
int mul_add_mod_q(modp_context_t* ctx, int a, int b, int c);

/*  Function to get c=a-b mod q
 */
int sub_mod_q(modp_context_t* ctx, int a, int b);

/* Function to get c=base**exp mod q
   - Only need naive version as it is just used for set up purposes
 */
int exp_mod_q(modp_context_t* ctx, int base, int exp);

#endif
