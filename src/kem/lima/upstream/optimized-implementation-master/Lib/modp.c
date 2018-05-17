

#include <stdint.h>

#include "modp.h"

/* The heart of the Montgomery Routines 
 *    - Assumes R=2^32
 *    - Needs looking at as currently poorly implemented
 */
int MontMult(modp_context_t* ctx, int a, int b)
{
  uint64_t t = (uint64_t)a * b;
  uint32_t u = (uint32_t)t * ctx->qinv;
  uint64_t z = t + (uint64_t)u * ctx->q;
  int zr=z>>32;
  zr=zr-(zr>=ctx->q)*ctx->q;  // Conditional subtraction
  return zr;
}

/* (mod q) x   Assumes input in range 0 <= x < q
 */
int to_mod_q(modp_context_t* ctx, int x)
{
    x=MontMult(ctx,x,ctx->R2);
    return x;
}

/*  (int) x
 *   - Needed as in optimized variant the representations
 *     are different. It uses MontMult
 */
int to_int(modp_context_t* ctx, int x)
{
    x=MontMult(ctx,x,1);
    return x;
}

int assign_one(modp_context_t* ctx)
{ return ctx->R; }

// c=a*b mod q
int mul_mod_q(modp_context_t* ctx, int a, int b)
{
    return MontMult(ctx,a,b);
}

int mul_mod_q_tag(modp_context_t* ctx, int a, int b) 
{
  uint64_t t = (uint64_t)a * b;
  uint32_t u = (uint32_t)t * ctx->qinv;
  uint64_t z = t + (uint64_t)u * ctx->q;
  return z>>32;
}


// c=a+b mod q
int add_mod_q(modp_context_t* ctx, int a, int b)
{
    int x=a+b;
    x=x-(x>=ctx->q)*ctx->q;
    return x;
}

// d=a*b+c mod q
int mul_add_mod_q(modp_context_t* ctx, int a, int b, int c)
{
    int x=MontMult(ctx,a,b)+c;
    x = x - (x>=ctx->q)*ctx->q;
    return x;
}

// c=a-b mod q
int sub_mod_q(modp_context_t* ctx, int a, int b)
{
  int x = a-b;
  return x + (x<0)*ctx->q;
}

/* c=base**exp mod q
 *   - Only need naive version as it is just used for set up purposes
 */
int exp_mod_q(modp_context_t* ctx, int base, int exp)
{
    int value=assign_one(ctx);
    while (exp !=0)
    {
        if (exp & 1) value = mul_mod_q(ctx,value,base);
        base = mul_mod_q(ctx,base,base);
        exp=exp>>1;
    }

    return value;
}



/*
 * q, log_256 q, R, R^2, -1/q mod R
 */
modp_context_t mod133121   = { 133121, 3,  84473, 2766, 3754035199 };
modp_context_t mod184321   = { 184321, 3, 103675, 10831, 386060287 };
modp_context_t mod12521473 = { 12521473, 3, 102057, 10287186, 532615167 };
modp_context_t mod48181249 = { 48181249, 4, 6836135, 13808161, 1507799039 };
modp_context_t mod44802049 = { 44802049, 4, 38772641, 34210394, 3735789567 };
modp_context_t mod16900097 = { 16900097, 4, 2342658, 10405766, 2097274879 };
