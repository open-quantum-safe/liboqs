#include "xof.h"

#include <stdlib.h>




/* Section 2.1.1 of Specification
 * - key_len is the length in BYTES of the key
 */
int InitXOF(XOF_t* xof, const uint8_t* key, byte_length key_len,uint8_t data)
{
  BitSequence D[1];
  D[0]=data;
   
  if (KMAC256_Initialize(xof,(BitSequence*) key,(BitLength)(key_len<<3),0,D,8))
     { return 1; }

  if (KMAC256_Final(xof,NULL))
     { return 1; }

  return 0;
}



int XOF(XOF_t* xof,uint8_t* output,byte_length output_len)
{
  return KMAC256_Squeeze(xof,(BitSequence*) output,(BitLength)(output_len<<3));
}


int KDF(const uint8_t* k, byte_length k_len, uint8_t* key, byte_length key_len)
{
  BitSequence D[1],empty[1];
  D[0]=0;
 
  return KMAC256(k,(BitLength)(k_len<<3),empty,0,(BitSequence*) key,(BitLength)(key_len<<3),D,8);
}



int get_random_field_element(XOF_t* xof, modp_context_t* ctx)
{
   int i,a;
   uint8_t ss[8];
   if (XOF(xof,ss,2*(ctx->log256_q))) { abort(); }

   uint64_t s=0;
   for (i=0; i<2*ctx->log256_q; i++)
     { s = s << 8; 
       s=s+ss[i];
     }
  a=s%ctx->q;
  a=to_mod_q(ctx,a);
  return a;
}


void get_random_field_elements(XOF_t* xof, modp_context_t* ctx, int n, int a[])
{
    int i;
    for (i = 0; i<n; i++) a[i] = get_random_field_element(xof, ctx);
}



int generate_gaussian_noise(XOF_t* xof)
{
    int i,j,s=0;
    uint8_t ss[5];
    if (XOF(xof, ss, 5)) { abort(); }

    for (i=0; i<5; i++)
    {
        for (j = 0; j < 4; j++)
        { s=s+(int) (ss[i]&1);
          ss[i]=ss[i]>>1;
          s=s-(int) (ss[i]&1);
          ss[i]=ss[i]>>1;
        }
    }
    return s;
}

