#include <sys/types.h>
#if defined(WINDOWS)
#include <windows.h>
#include <Wincrypt.h>
#else
#include <sys/uio.h>
#include <unistd.h>
#include <strings.h>
#endif
#include <string.h> //memcpy
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>

#include <oqs/rand.h>
#include <oqs/rand_urandom_aesctr.h>
#include <oqs/aes.h>

typedef struct OQS_RAND_urandom_aesctr_ctx {
	union {
		uint8_t ba[16];
		uint64_t ui[2];
	} ctr;
	void *schedule;
	uint8_t cache[64];
	size_t cache_next_byte;
} OQS_RAND_urandom_aesctr_ctx;

static OQS_RAND_urandom_aesctr_ctx *OQS_RAND_urandom_aesctr_ctx_new() {
	int fd = 0;
	OQS_RAND_urandom_aesctr_ctx *rand_ctx = NULL;
	rand_ctx = (OQS_RAND_urandom_aesctr_ctx *) malloc(sizeof(OQS_RAND_urandom_aesctr_ctx));
	if (rand_ctx == NULL) {
		goto err;
	}
	uint8_t key[16];
#if defined(WINDOWS)
	HCRYPTPROV   hCryptProv;
	if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) ||
	        !cryptgenrandom(hcryptprov, 16, key) ||
	        !cryptgenrandom(hcryptprov, 8, &rand_ctx->ctr[8]) ) {
		goto err;
	}
#else
	fd = open("/dev/urandom", O_RDONLY);
	if (fd <= 0) {
		goto err;
	}
	int r = read(fd, key, 16);
	if (r != 16) {
		goto err;
	}
	r = read(fd, &rand_ctx->ctr.ba[8], 8);
	if (r != 8) {
		goto err;
	}
#endif
	OQS_AES128_load_schedule(key, &rand_ctx->schedule);
	rand_ctx->cache_next_byte = 64; // cache is empty
	goto okay;
err:
	if (rand_ctx) {
		free(rand_ctx);
	}
#if !defined(WINDOWS)
	if (fd > 0) {
		close(fd);
	}
#endif
	return NULL;
okay:
#if !defined(WINDOWS)
	close(fd);
#endif
	return rand_ctx;
}

static void OQS_RAND_urandom_aesctr_fill_cache(OQS_RAND *r) {
	OQS_RAND_urandom_aesctr_ctx *rand_ctx = (OQS_RAND_urandom_aesctr_ctx *) r->ctx;
	for (int i = 0; i < 4; i++) {
		OQS_AES128_enc(rand_ctx->ctr.ba, rand_ctx->schedule, &rand_ctx->cache[i * 16]);
		rand_ctx->ctr.ui[0]++;
	}
	rand_ctx->cache_next_byte = 0;
}

void OQS_RAND_urandom_aesctr_n(OQS_RAND *r, uint8_t *out, size_t n) {
	OQS_RAND_urandom_aesctr_ctx *rand_ctx = (OQS_RAND_urandom_aesctr_ctx *) r->ctx;
	while (n > 0) {
		if (n + rand_ctx->cache_next_byte < 64) {
			memcpy(out, &rand_ctx->cache[rand_ctx->cache_next_byte], n);
			rand_ctx->cache_next_byte += n;
			n = 0;
		} else {
			size_t bytes_left = 64 - rand_ctx->cache_next_byte;
			memcpy(out, &rand_ctx->cache[rand_ctx->cache_next_byte], bytes_left);
			out += bytes_left;
			n -= bytes_left;
			OQS_RAND_urandom_aesctr_fill_cache(r);
		}
	}
}

uint8_t OQS_RAND_urandom_aesctr_8(OQS_RAND *r) {
	uint8_t out;
	OQS_RAND_urandom_aesctr_n(r, (uint8_t *)&out, 1);
	return out;
}


uint32_t OQS_RAND_urandom_aesctr_32(OQS_RAND *r) {
	uint32_t out;
	OQS_RAND_urandom_aesctr_n(r, (uint8_t *)&out, 4);
	return out;
}

uint64_t OQS_RAND_urandom_aesctr_64(OQS_RAND *r) {
	uint64_t out;
	OQS_RAND_urandom_aesctr_n(r, (uint8_t *)&out, 8);
	return out;
}

void OQS_RAND_urandom_aesctr_free(OQS_RAND *r) {
	if (r) {
		OQS_RAND_urandom_aesctr_ctx *rand_ctx = (OQS_RAND_urandom_aesctr_ctx *) r->ctx;
		if (rand_ctx) {
			OQS_AES128_free_schedule(rand_ctx->schedule);
		}
		free(r->ctx);
		free(r->method_name);
	}
	free(r);
}

OQS_RAND *OQS_RAND_urandom_aesctr_new() {
	OQS_RAND *r = malloc(sizeof(OQS_RAND));
	if (r == NULL) {
		return NULL;
	}
	r->method_name = strdup("urandom_aesctr");
	r->ctx = OQS_RAND_urandom_aesctr_ctx_new();
	if (r->ctx == NULL || r->method_name == NULL) {
		OQS_RAND_urandom_aesctr_free(r);
		return NULL;
	}
	r->estimated_classical_security = 128;
	r->estimated_quantum_security = 64; // Grover search
	r->rand_8 = &OQS_RAND_urandom_aesctr_8;
	r->rand_32 = &OQS_RAND_urandom_aesctr_32;
	r->rand_64 = &OQS_RAND_urandom_aesctr_64;
	r->rand_n = &OQS_RAND_urandom_aesctr_n;
	r->free = &OQS_RAND_urandom_aesctr_free;
	return r;
}
