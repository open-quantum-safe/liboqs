/*
 * example_sig.cpp
 *
 * Minimal C++ example of using a post-quantum signature implemented in liboqs.
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>

#include <oqs/oqs.h>

constexpr size_t MESSAGE_LEN = 50;

/* Cleaning up memory etc */
void cleanup_stack(uint8_t *secret_key, size_t secret_key_len);

struct OQSSecureDeleter {
    size_t length;

    explicit OQSSecureDeleter(size_t len) : length(len) {}

    void operator()(uint8_t* ptr) const {
        if (ptr) {
            OQS_MEM_secure_free(ptr, length);
        }
    }
};

struct OQSInsecureDeleter {
	void operator()(uint8_t* ptr) {
		if (ptr) {
			OQS_MEM_insecure_free(ptr);
		}
	}
};

struct OQSSigDeleter {
    void operator()(OQS_SIG* sig) {
        if (sig) {
            OQS_SIG_free(sig);
        }
    }
};


/* This function gives an example of the signing operations
 * using only compile-time macros and allocating variables
 * statically on the stack, calling a specific algorithm's functions
 * directly.
 *
 * The macros OQS_SIG_dilithium_2_length_* and the functions OQS_SIG_dilithium_2_*
 * are only defined if the algorithm dilithium_2 was enabled at compile-time
 * which must be checked using the OQS_ENABLE_SIG_dilithium_2 macro.
 *
 * <oqs/oqsconfig.h>, which is included in <oqs/oqs.h>, contains macros
 * indicating which algorithms were enabled when this instance of liboqs
 * was compiled.
 */
static OQS_STATUS example_stack(void) {

#ifdef OQS_ENABLE_SIG_dilithium_2

	OQS_STATUS rc;

	uint8_t public_key[OQS_SIG_dilithium_2_length_public_key];
	uint8_t secret_key[OQS_SIG_dilithium_2_length_secret_key];
	uint8_t message[MESSAGE_LEN];
	uint8_t signature[OQS_SIG_dilithium_2_length_signature];
	size_t message_len = MESSAGE_LEN;
	size_t signature_len;

	// let's create a random test message to sign
	OQS_randombytes(message, message_len);

	rc = OQS_SIG_dilithium_2_keypair(public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		std::cerr << "ERROR: OQS_SIG_dilithium_2_keypair failed!" << std::endl;
		cleanup_stack(secret_key, OQS_SIG_dilithium_2_length_secret_key);
		return OQS_ERROR;
	}
	rc = OQS_SIG_dilithium_2_sign(signature, &signature_len, message, message_len, secret_key);
	if (rc != OQS_SUCCESS) {
		std::cerr << "ERROR: OQS_SIG_dilithium_2_sign failed!" << std::endl;
		cleanup_stack(secret_key, OQS_SIG_dilithium_2_length_secret_key);
		return OQS_ERROR;
	}
	rc = OQS_SIG_dilithium_2_verify(message, message_len, signature, signature_len, public_key);
	if (rc != OQS_SUCCESS) {
		std::cerr << "ERROR: OQS_SIG_dilithium_2_verify failed!" << std::endl;
		cleanup_stack(secret_key, OQS_SIG_dilithium_2_length_secret_key);
		return OQS_ERROR;
	}

	std::cout << "[example_stack] OQS_SIG_dilithium_2 operations completed" << std::endl;
	cleanup_stack(secret_key, OQS_SIG_dilithium_2_length_secret_key);
	return OQS_SUCCESS; // success!

#else

	std::cout << "[example_stack] OQS_SIG_dilithium_2 was not enabled at compile-time" << std::endl;
	return OQS_SUCCESS;

#endif
}

/* This function gives an example of the signing operations,
 * allocating variables dynamically on the heap and calling the generic
 * OQS_SIG object.
 *
 * This does not require the use of compile-time macros to check if the
 * algorithm in question was enabled at compile-time; instead, the caller
 * must check that the OQS_SIG object returned is not nullptr.
 */
static OQS_STATUS example_heap(void) {

#ifdef OQS_ENABLE_SIG_dilithium_2

	size_t message_len = MESSAGE_LEN;
	size_t signature_len;
	OQS_STATUS rc;

	std::unique_ptr<OQS_SIG, OQSSigDeleter> sig(OQS_SIG_new((OQS_SIG_alg_dilithium_2)));
	if (sig == nullptr) {
		throw std::runtime_error("[example_heap]  OQS_SIG_alg_dilithium_2 was not enabled at compile-time.");
	}
	std::unique_ptr<uint8_t[], OQSInsecureDeleter> public_key(static_cast<uint8_t*>(malloc(sig->length_public_key)));
	std::unique_ptr<uint8_t[], OQSSecureDeleter> secret_key(static_cast<uint8_t*>(malloc(sig->length_secret_key)), OQSSecureDeleter(sig->length_secret_key));
	std::unique_ptr<uint8_t[], OQSInsecureDeleter> message(static_cast<uint8_t*>(malloc(message_len)));
	std::unique_ptr<uint8_t[], OQSInsecureDeleter> signature(static_cast<uint8_t*>(malloc(sig->length_signature)));
	if ((public_key == nullptr) || (secret_key == nullptr) || (message == nullptr) || (signature == nullptr)) {
		throw std::runtime_error("ERROR: malloc failed!");
	}

	// let's create a random test message to sign
	OQS_randombytes(message.get(), message_len);

	rc = OQS_SIG_keypair(sig.get(), public_key.get(), secret_key.get());
	if (rc != OQS_SUCCESS) {
		throw std::runtime_error("ERROR: OQS_SIG_keypair failed!");
	}
	rc = OQS_SIG_sign(sig.get(), signature.get(), &signature_len, message.get(), message_len, secret_key.get());
	if (rc != OQS_SUCCESS) {
		throw std::runtime_error("ERROR: OQS_SIG_sign failed!");
	}
	rc = OQS_SIG_verify(sig.get(), message.get(), message_len, signature.get(), signature_len, public_key.get());
	if (rc != OQS_SUCCESS) {
		throw std::runtime_error("ERROR: OQS_SIG_verify failed!");
	}

	std::cout << "[example_heap]  OQS_SIG_dilithium_2 operations completed." << std::endl;
	return OQS_SUCCESS; // success
#else

	std::cout << "[example_heap] OQS_SIG_dilithium_2 was not enabled at compile-time." << std::endl;
	return OQS_SUCCESS;

#endif
}

int main() {
	OQS_init();
	try {
		example_stack();
		example_heap();
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	return EXIT_SUCCESS;
}

void cleanup_stack(uint8_t *secret_key, size_t secret_key_len) {
	OQS_MEM_cleanse(secret_key, secret_key_len);
}
