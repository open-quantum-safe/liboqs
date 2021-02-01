#include "aes256ctr.h"
#include "symmetric.h"
#include <stdint.h>

void PQCLEAN_DILITHIUM2AES_CLEAN_dilithium_aes256ctr_init(aes256ctr_ctx *state,
        const uint8_t key[32],
        uint16_t nonce) {
    uint8_t expnonce[12] = {0};
    expnonce[0] = (uint8_t) nonce;
    expnonce[1] = (uint8_t) (nonce >> 8);
    PQCLEAN_DILITHIUM2AES_CLEAN_aes256ctr_init(state, key, expnonce);
}
