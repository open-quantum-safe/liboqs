#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include <oqs/sha2.h>

const unsigned char plaintext[113] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";

const unsigned char expected_224[28] = {
    0xc9, 0x7c, 0xa9, 0xa5, 0x59, 0x85, 0x0c, 0xe9, 0x7a, 0x04, 0xa9, 0x6d,
    0xef, 0x6d, 0x99, 0xa9, 0xe0, 0xe0, 0xe2, 0xab, 0x14, 0xe6, 0xb8, 0xdf,
    0x26, 0x5f, 0xc0, 0xb3
};

const unsigned char expected_256[32] = {
    0xcf, 0x5b, 0x16, 0xa7, 0x78, 0xaf, 0x83, 0x80, 0x03, 0x6c, 0xe5, 0x9e,
    0x7b, 0x04, 0x92, 0x37, 0x0b, 0x24, 0x9b, 0x11, 0xe8, 0xf0, 0x7a, 0x51,
    0xaf, 0xac, 0x45, 0x03, 0x7a, 0xfe, 0xe9, 0xd1
};

const unsigned char expected_384[48] = {
    0x09, 0x33, 0x0c, 0x33, 0xf7, 0x11, 0x47, 0xe8, 0x3d, 0x19, 0x2f, 0xc7,
    0x82, 0xcd, 0x1b, 0x47, 0x53, 0x11, 0x1b, 0x17, 0x3b, 0x3b, 0x05, 0xd2,
    0x2f, 0xa0, 0x80, 0x86, 0xe3, 0xb0, 0xf7, 0x12, 0xfc, 0xc7, 0xc7, 0x1a,
    0x55, 0x7e, 0x2d, 0xb9, 0x66, 0xc3, 0xe9, 0xfa, 0x91, 0x74, 0x60, 0x39
};

const unsigned char expected_512[64] = {
    0x8e, 0x95, 0x9b, 0x75, 0xda, 0xe3, 0x13, 0xda, 0x8c, 0xf4, 0xf7, 0x28,
    0x14, 0xfc, 0x14, 0x3f, 0x8f, 0x77, 0x79, 0xc6, 0xeb, 0x9f, 0x7f, 0xa1,
    0x72, 0x99, 0xae, 0xad, 0xb6, 0x88, 0x90, 0x18, 0x50, 0x1d, 0x28, 0x9e,
    0x49, 0x00, 0xf7, 0xe4, 0x33, 0x1b, 0x99, 0xde, 0xc4, 0xb5, 0x43, 0x3a,
    0xc7, 0xd3, 0x29, 0xee, 0xb6, 0xdd, 0x26, 0x54, 0x5e, 0x96, 0xe5, 0x5b,
    0x87, 0x4b, 0xe9, 0x09
};

static int test_sha256_incremental(void) {
    unsigned char output[32];
    uint8_t state[40];
    int i = 0;

    OQS_SHA2_sha256_inc_init(state);
    OQS_SHA2_sha256_inc_blocks(state, plaintext, 1);
    OQS_SHA2_sha256_inc_finalize(output, state, plaintext + 64, 112 - 64);

    if (memcmp(expected_256, output, 32)) {
        printf("ERROR sha256 incremental did not match sha256.\n");
        printf("  Expected: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", expected_256[i]);
        }
        printf("\n");
        printf("  Received: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", output[i]);
        }
        printf("\n");
        return 1;
    }

    return 0;
}

static int test_sha224(void) {
    unsigned char output[28];
    int i = 0;

    OQS_SHA2_sha224(output, plaintext, 112);

    if (memcmp(expected_224, output, 28)) {
        printf("ERROR sha224 output did not match test vector.\n");
        printf("Expected: ");
        for (i = 0; i < 28; i++) {
            printf("%02X", expected_224[i]);
        }
        printf("\n");
        printf("Received: ");
        for (i = 0; i < 28; i++) {
            printf("%02X", output[i]);
        }
        printf("\n");
        return 1;
    }

    return 0;
}

static int test_sha256(void) {
    unsigned char output[32];
    int i = 0;

    OQS_SHA2_sha256(output, plaintext, 112);

    if (memcmp(expected_256, output, 32)) {
        printf("ERROR sha256 output did not match test vector.\n");
        printf("Expected: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", expected_256[i]);
        }
        printf("\n");
        printf("Received: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", output[i]);
        }
        printf("\n");
        return 1;
    }

    return 0;
}

static int test_sha384(void) {
    unsigned char output[48];
    int i = 0;

    OQS_SHA2_sha384(output, plaintext, 112);

    if (memcmp(expected_384, output, 48)) {
        printf("ERROR sha384 output did not match test vector.\n");
        printf("Expected: ");
        for (i = 0; i < 48; i++) {
            printf("%02X", expected_384[i]);
        }
        printf("\n");
        printf("Received: ");
        for (i = 0; i < 48; i++) {
            printf("%02X", output[i]);
        }
        printf("\n");
        return 1;
    }

    return 0;
}

static int test_sha512(void) {
    unsigned char output[64];
    int i = 0;

    OQS_SHA2_sha512(output, plaintext, 112);

    if (memcmp(expected_512, output, 64)) {
        printf("ERROR sha512 output did not match test vector.\n");
        printf("Expected: ");
        for (i = 0; i < 64; i++) {
            printf("%02X", expected_512[i]);
        }
        printf("\n");
        printf("Received: ");
        for (i = 0; i < 64; i++) {
            printf("%02X", output[i]);
        }
        printf("\n");
        return 1;
    }

    return 0;
}

int main(void) {
    int result = 0;
    result += test_sha224();
    result += test_sha256();
    result += test_sha256_incremental();
    result += test_sha384();
    result += test_sha512();

    if (result != 0) {
        puts("Errors occurred");
    }
    return result;
}
