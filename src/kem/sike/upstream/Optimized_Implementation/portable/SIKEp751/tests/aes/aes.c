/********************************************************************************************
* Functions for AES256
*********************************************************************************************/

#include <assert.h>
#include <string.h>
#include "aes.h"
#include "aes_local.h"


void AES256_load_schedule(const uint8_t *key, void *schedule) {
    aes256_load_schedule_c(key, schedule);
}
     

void AES256_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
    assert(plaintext_len % 16 == 0);
    for (size_t block = 0; block < plaintext_len / 16; block++) {
        aes256_enc_c(plaintext + (16 * block), schedule, ciphertext + (16 * block));
    }
}


void AES256_free_schedule(void *schedule) {
    memset(schedule, 0, 16*15);
}