#ifndef COMMON_H
#define COMMON_H

int PQCLEAN_FRODOKEM640AES_CLEAN_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
int PQCLEAN_FRODOKEM640AES_CLEAN_mul_add_sa_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
void PQCLEAN_FRODOKEM640AES_CLEAN_sample_n(uint16_t *s, size_t n);
void PQCLEAN_FRODOKEM640AES_CLEAN_mul_bs(uint16_t *out, const uint16_t *b, const uint16_t *s);
void PQCLEAN_FRODOKEM640AES_CLEAN_mul_add_sb_plus_e(uint16_t *out, const uint16_t *b, const uint16_t *s, const uint16_t *e);
void PQCLEAN_FRODOKEM640AES_CLEAN_add(uint16_t *out, const uint16_t *a, const uint16_t *b);
void PQCLEAN_FRODOKEM640AES_CLEAN_sub(uint16_t *out, const uint16_t *a, const uint16_t *b);
void PQCLEAN_FRODOKEM640AES_CLEAN_key_encode(uint16_t *out, const uint16_t *in);
void PQCLEAN_FRODOKEM640AES_CLEAN_key_decode(uint16_t *out, const uint16_t *in);
void PQCLEAN_FRODOKEM640AES_CLEAN_pack(uint8_t *out, size_t outlen, const uint16_t *in, size_t inlen, uint8_t lsb);
void PQCLEAN_FRODOKEM640AES_CLEAN_unpack(uint16_t *out, size_t outlen, const uint8_t *in, size_t inlen, uint8_t lsb);
void PQCLEAN_FRODOKEM640AES_CLEAN_clear_bytes(uint8_t *mem, size_t n);
uint16_t PQCLEAN_FRODOKEM640AES_CLEAN_LE_TO_UINT16(uint16_t n);
uint16_t PQCLEAN_FRODOKEM640AES_CLEAN_UINT16_TO_LE(uint16_t n);

#endif
