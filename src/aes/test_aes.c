#include "aes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t mtc_enc_key[] = {0x13, 0x9a, 0x35, 0x42, 0x2f, 0x1d, 0x61, 0xde, 0x3c, 0x91, 0x78, 0x7f, 0xe0, 0x50, 0x7a, 0xfd};
uint8_t mtc_enc_plain[] = {0xb9, 0x14, 0x5a, 0x76, 0x8b, 0x7d, 0xc4, 0x89, 0xa0, 0x96, 0xb5, 0x46, 0xf4, 0x3b, 0x23, 0x1f};

uint8_t mtc_dec_key[] =
{0x0c, 0x60, 0xe7, 0xbf, 0x20, 0xad, 0xa9, 0xba, 0xa9, 0xe1, 0xdd, 0xf0, 0xd1, 0x54, 0x07, 0x26};
uint8_t mtc_dec_cipher[] =
{0xb0, 0x8a, 0x29, 0xb1, 0x1a, 0x50, 0x0e, 0xa3, 0xac, 0xa4, 0x2c, 0x36, 0x67, 0x5b, 0x97, 0x85};

static void xor_bytes(uint8_t *a, uint8_t *b) {
	for (size_t i = 0; i < 16; i++) {
		a[i] ^= b[i];
	}
}


static void print_bytes(uint8_t *bytes, size_t num_bytes) {
	for (size_t i = 0; i < num_bytes; i++) {
		printf("%02x", (unsigned)bytes[i]);
	}
}

void monte_test_encrypt(uint8_t *plain_text, uint8_t *key) {
	uint8_t schedule[20 * 16];
	uint8_t cipher_text[16];
	for (size_t i = 0; i < 100; i++) {
		printf ("\nCOUNT = %d", (int)i );
		printf ("\nKEY = ");
		print_bytes(key, 16);
		printf ("\nPLAINTEXT = ");
		print_bytes(plain_text, 16);
		OQS_AES128_load_schedule(key, schedule);
		for (size_t i = 0; i < 1000; i++) {
			OQS_AES128_enc(plain_text, schedule, cipher_text);
			memcpy(plain_text, cipher_text, sizeof(cipher_text));
		}
		printf ("\nCIPHERTEXT = ");
		print_bytes(cipher_text, 16);
		printf ("\n");
		xor_bytes(key, cipher_text);
	}
}

void monte_test_decrypt(uint8_t *cipher_text, uint8_t *key) {
	uint8_t schedule[20 * 16];
	uint8_t plain_text[16];
	for (size_t i = 0; i < 100; i++) {
		printf ("\nCOUNT = %d", (int)i );
		printf ("\nKEY = ");
		print_bytes(key, 16);
		printf ("\nCIPHERTEXT = ");
		print_bytes(cipher_text, 16);
		OQS_AES128_load_schedule(key, schedule);
		for (size_t i = 0; i < 1000; i++) {
			OQS_AES128_dec(cipher_text, schedule, plain_text);
			memcpy(cipher_text, plain_text, sizeof(plain_text));
		}
		printf ("\nPLAINTEXT = ");
		print_bytes(plain_text, 16);
		printf ("\n");
		xor_bytes(key, cipher_text);
	}
}

int main() {
	printf("\n[ENCRYPT]\n");
	monte_test_encrypt(mtc_enc_plain, mtc_enc_key);
	printf("\n[DECRYPT]\n");
	monte_test_decrypt(mtc_dec_cipher, mtc_dec_key);
	return 0;
}
