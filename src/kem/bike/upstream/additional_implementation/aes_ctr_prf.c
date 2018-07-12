/***************************************************************************
* Additional implementation of "BIKE: Bit Flipping Key Encapsulation". 
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#include "aes_ctr_prf.h"
#include "string.h"
#include "stdio.h"
#include "utilities.h"

status_t init_aes_ctr_prf_state(OUT aes_ctr_prf_state_t* s,
                                IN const uint32_t maxInvokations,
                                IN const seed_t* seed)
{
    if (maxInvokations == 0)
    {
        return E_AES_CTR_PRF_INIT_FAIL;
    }
    
    //Set the Key schedule (from seed).
    AES_256_Key_Expansion (s->key, seed->u.raw);
 
    //Initialize buffer and counter
    s->ctr.u.qw[0] = 0;
    s->ctr.u.qw[1] = 0;

    s->pos = AES256_BLOCK_SIZE;
    s->rem_invokations = maxInvokations;
    
    SEDMSG("    Init aes_prf_ctr state:\n");
    SEDMSG("      s.pos = %d\n", s->pos); 
    SEDMSG("      s.rem_invokations = %u\n", s->rem_invokations); 
    SEDMSG("      s.ctr = 0x"); //print(s->ctr.u.qw, sizeof(s->ctr)*8);
    
    return SUCCESS;
}

_INLINE_ status_t perform_aes(OUT uint8_t* ct, IN OUT aes_ctr_prf_state_t* s)
{
    if(s->rem_invokations == 0)
    {
        return E_AES_OVER_USED;
    }

    AES256_Enc_Intrinsic(ct, s->ctr.u.bytes, s->key);

    s->ctr.u.qw[0]++;
    s->rem_invokations--;
    
    return SUCCESS;
}

status_t aes_ctr_prf(OUT uint8_t* a,
                     IN aes_ctr_prf_state_t* s,
                     IN const uint32_t len)
{
    status_t res = SUCCESS;

    //When Len i smaller then whats left in the buffer 
    //No need in additional AES.
    if ((len + s->pos) <= AES256_BLOCK_SIZE)
    {
        memcpy(a, &s->buffer.u.bytes[s->pos], len);
        s->pos += len;
        
        return res;
    }

    //if s.pos != AES256_BLOCK_SIZE then copy whats left in the buffer.
    //else copy zero bytes.
    uint32_t idx = AES256_BLOCK_SIZE - s->pos;
    memcpy(a, &s->buffer.u.bytes[s->pos], idx);
    
    //Init s.pos;
    s->pos = 0;
    
    //Copy full AES blocks.
    while((len - idx) >= AES256_BLOCK_SIZE)
    {
        res = perform_aes(&a[idx], s);                    CHECK_STATUS(res);
        idx += AES256_BLOCK_SIZE;
    }
    
    res = perform_aes(s->buffer.u.bytes, s);                CHECK_STATUS(res);

    //Copy the tail.
    s->pos = len - idx;
    memcpy(&a[idx], s->buffer.u.bytes, s->pos);

EXIT:
    return res;
}

