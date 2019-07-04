#include "pack_unpack.h"

void PQCLEAN_LIGHTSABER_CLEAN_pack_3bit(uint8_t *bytes, const uint16_t *data) {
    uint32_t j;
    uint32_t offset_data, offset_byte;

    for (j = 0; j < SABER_N / 8; j++) {
        offset_byte = 3 * j;
        offset_data = 8 * j;
        bytes[offset_byte + 0] = (data[offset_data + 0] & 0x7) |
                                 ((data[offset_data + 1] & 0x7) << 3) |
                                 ((data[offset_data + 2] & 0x3) << 6);
        bytes[offset_byte + 1] = ((data[offset_data + 2] >> 2 ) & 0x01)  |
                                 ((data[offset_data + 3] & 0x7) << 1) |
                                 ((data[offset_data + 4] & 0x7) << 4) |
                                 (((data[offset_data + 5]) & 0x01) << 7);
        bytes[offset_byte + 2] = ((data[offset_data + 5] >> 1 ) & 0x03) |
                                 ((data[offset_data + 6] & 0x7) << 2) |
                                 ((data[offset_data + 7] & 0x7) << 5);
    }
}

void PQCLEAN_LIGHTSABER_CLEAN_un_pack3bit(const uint8_t *bytes, uint16_t *data) {
    uint32_t j;
    uint32_t offset_data, offset_byte;

    for (j = 0; j < SABER_N / 8; j++) {
        offset_byte = 3 * j;
        offset_data = 8 * j;
        data[offset_data + 0] = (bytes[offset_byte + 0]) & 0x07;
        data[offset_data + 1] = ((bytes[offset_byte + 0]) >> 3 ) & 0x07;
        data[offset_data + 2] = (((bytes[offset_byte + 0]) >> 6 ) & 0x03) |
                                (((bytes[offset_byte + 1]) & 0x01) << 2);
        data[offset_data + 3] = ((bytes[offset_byte + 1]) >> 1 ) & 0x07;
        data[offset_data + 4] = ((bytes[offset_byte + 1]) >> 4 ) & 0x07;
        data[offset_data + 5] = (((bytes[offset_byte + 1]) >> 7 ) & 0x01) |
                                (((bytes[offset_byte + 2]) & 0x03) << 1);
        data[offset_data + 6] = ((bytes[offset_byte + 2] >> 2) & 0x07);
        data[offset_data + 7] = ((bytes[offset_byte + 2] >> 5) & 0x07);
    }
}

void PQCLEAN_LIGHTSABER_CLEAN_pack_4bit(uint8_t *bytes, const uint16_t *data) {
    uint32_t j;
    uint32_t offset_data;

    for (j = 0; j < SABER_N / 2; j++) {
        offset_data = 2 * j;
        bytes[j] = (data[offset_data] & 0x0f) |
                   ((data[offset_data + 1] & 0x0f) << 4);
    }
}

void PQCLEAN_LIGHTSABER_CLEAN_un_pack4bit(const unsigned char *bytes, uint16_t *ar) {
    uint32_t j;
    uint32_t offset_data;

    for (j = 0; j < SABER_N / 2; j++) {
        offset_data = 2 * j;
        ar[offset_data] = bytes[j] & 0x0f;
        ar[offset_data + 1] = (bytes[j] >> 4) & 0x0f;
    }
}

void PQCLEAN_LIGHTSABER_CLEAN_pack_6bit(uint8_t *bytes, const uint16_t *data) {
    uint32_t j;
    uint32_t offset_data, offset_byte;

    for (j = 0; j < SABER_N / 4; j++) {
        offset_byte = 3 * j;
        offset_data = 4 * j;
        bytes[offset_byte + 0] = (data[offset_data + 0] & 0x3f) |
                                 ((data[offset_data + 1] & 0x03) << 6);
        bytes[offset_byte + 1] = ((data[offset_data + 1] >> 2) & 0x0f) |
                                 ((data[offset_data + 2] & 0x0f) << 4);
        bytes[offset_byte + 2] = ((data[offset_data + 2] >> 4) & 0x03) |
                                 ((data[offset_data + 3] & 0x3f) << 2);
    }
}


void PQCLEAN_LIGHTSABER_CLEAN_un_pack6bit(const unsigned char *bytes, uint16_t *data) {
    uint32_t j;
    uint32_t offset_data, offset_byte;

    for (j = 0; j < SABER_N / 4; j++) {
        offset_byte = 3 * j;
        offset_data = 4 * j;
        data[offset_data + 0] = bytes[offset_byte + 0] & 0x3f;
        data[offset_data + 1] = ((bytes[offset_byte + 0] >> 6) & 0x03) |
                                ((bytes[offset_byte + 1] & 0x0f) << 2);
        data[offset_data + 2] = ((bytes[offset_byte + 1] & 0xff) >> 4) |
                                ((bytes[offset_byte + 2] & 0x03) << 4);
        data[offset_data + 3] = ((bytes[offset_byte + 2] & 0xff) >> 2);
    }
}


static void POLVECp2BS(uint8_t *bytes, uint16_t data[SABER_K][SABER_N]) {
    uint32_t i, j;
    uint32_t offset_data, offset_byte, offset_byte1;

    for (i = 0; i < SABER_K; i++) {
        offset_byte1 = i * (SABER_N * 10) / 8;
        for (j = 0; j < SABER_N / 4; j++) {
            offset_byte = offset_byte1 + 5 * j;
            offset_data = 4 * j;
            bytes[offset_byte + 0] = (data[i][offset_data + 0] & (0xff));
            bytes[offset_byte + 1] = ((data[i][offset_data + 0] >> 8) & 0x03) |
                                     ((data[i][offset_data + 1] & 0x3f) << 2);
            bytes[offset_byte + 2] = ((data[i][offset_data + 1] >> 6) & 0x0f) |
                                     ((data[i][offset_data + 2] & 0x0f) << 4);
            bytes[offset_byte + 3] = ((data[i][offset_data + 2] >> 4) & 0x3f) |
                                     ((data[i][offset_data + 3] & 0x03) << 6);
            bytes[offset_byte + 4] = ((data[i][offset_data + 3] >> 2) & 0xff);
        }
    }
}

static void BS2POLVECp(const unsigned char *bytes, uint16_t data[SABER_K][SABER_N]) {
    uint32_t i, j;
    uint32_t offset_data, offset_byte, offset_byte1;

    for (i = 0; i < SABER_K; i++) {
        offset_byte1 = i * (SABER_N * 10) / 8;
        for (j = 0; j < SABER_N / 4; j++) {
            offset_byte = offset_byte1 + 5 * j;
            offset_data = 4 * j;
            data[i][offset_data + 0] = (bytes[offset_byte + 0] & (0xff)) |
                                       ((bytes[offset_byte + 1] & 0x03) << 8);
            data[i][offset_data + 1] = ((bytes[offset_byte + 1] >> 2) & (0x3f)) |
                                       ((bytes[offset_byte + 2] & 0x0f) << 6);
            data[i][offset_data + 2] = ((bytes[offset_byte + 2] >> 4) & (0x0f)) |
                                       ((bytes[offset_byte + 3] & 0x3f) << 4);
            data[i][offset_data + 3] = ((bytes[offset_byte + 3] >> 6) & (0x03)) |
                                       ((bytes[offset_byte + 4] & 0xff) << 2);
        }
    }
}



static void POLVECq2BS(uint8_t *bytes, uint16_t data[SABER_K][SABER_N]) {
    uint32_t i, j;
    uint32_t offset_data, offset_byte, offset_byte1;

    for (i = 0; i < SABER_K; i++) {
        offset_byte1 = i * (SABER_N * 13) / 8;
        for (j = 0; j < SABER_N / 8; j++) {
            offset_byte = offset_byte1 + 13 * j;
            offset_data = 8 * j;
            bytes[offset_byte + 0] = (data[i][offset_data + 0] & (0xff));
            bytes[offset_byte + 1] = ((data[i][offset_data + 0] >> 8) & 0x1f) |
                                     ((data[i][offset_data + 1] & 0x07) << 5);
            bytes[offset_byte + 2] = ((data[i][offset_data + 1] >> 3) & 0xff);
            bytes[offset_byte + 3] = ((data[i][offset_data + 1] >> 11) & 0x03) |
                                     ((data[i][offset_data + 2] & 0x3f) << 2);
            bytes[offset_byte + 4] = ((data[i][offset_data + 2] >> 6) & 0x7f) |
                                     ((data[i][offset_data + 3] & 0x01) << 7);
            bytes[offset_byte + 5] = ((data[i][offset_data + 3] >> 1) & 0xff);
            bytes[offset_byte + 6] = ((data[i][offset_data + 3] >> 9) & 0x0f) |
                                     ((data[i][offset_data + 4] & 0x0f) << 4);
            bytes[offset_byte + 7] = ((data[i][offset_data + 4] >> 4) & 0xff);
            bytes[offset_byte + 8] = ((data[i][offset_data + 4] >> 12) & 0x01) |
                                     ((data[i][offset_data + 5] & 0x7f) << 1);
            bytes[offset_byte + 9] = ((data[i][offset_data + 5] >> 7) & 0x3f) |
                                     ((data[i][offset_data + 6] & 0x03) << 6);
            bytes[offset_byte + 10] = ((data[i][offset_data + 6] >> 2) & 0xff);
            bytes[offset_byte + 11] = ((data[i][offset_data + 6] >> 10) & 0x07) |
                                      ((data[i][offset_data + 7] & 0x1f) << 3);
            bytes[offset_byte + 12] = ((data[i][offset_data + 7] >> 5) & 0xff);
        }
    }
}

static void BS2POLVECq(const unsigned char *bytes, uint16_t data[SABER_K][SABER_N]) {
    uint32_t i, j;
    uint32_t offset_data, offset_byte, offset_byte1;

    for (i = 0; i < SABER_K; i++) {
        offset_byte1 = i * (SABER_N * 13) / 8;
        for (j = 0; j < SABER_N / 8; j++) {
            offset_byte = offset_byte1 + 13 * j;
            offset_data = 8 * j;
            data[i][offset_data + 0] = (bytes[offset_byte + 0] & (0xff)) |
                                       ((bytes[offset_byte + 1] & 0x1f) << 8);
            data[i][offset_data + 1] = (bytes[offset_byte + 1] >> 5 & (0x07)) |
                                       ((bytes[offset_byte + 2] & 0xff) << 3) |
                                       ((bytes[offset_byte + 3] & 0x03) << 11);
            data[i][offset_data + 2] = (bytes[offset_byte + 3] >> 2 & (0x3f)) |
                                       ((bytes[offset_byte + 4] & 0x7f) << 6);
            data[i][offset_data + 3] = (bytes[offset_byte + 4] >> 7 & (0x01)) |
                                       ((bytes[offset_byte + 5] & 0xff) << 1) |
                                       ((bytes[offset_byte + 6] & 0x0f) << 9);
            data[i][offset_data + 4] = (bytes[offset_byte + 6] >> 4 & (0x0f)) |
                                       ((bytes[offset_byte + 7] & 0xff) << 4) |
                                       ((bytes[offset_byte + 8] & 0x01) << 12);
            data[i][offset_data + 5] = (bytes[offset_byte + 8] >> 1 & (0x7f)) |
                                       ((bytes[offset_byte + 9] & 0x3f) << 7);
            data[i][offset_data + 6] = (bytes[offset_byte + 9] >> 6 & (0x03)) |
                                       ((bytes[offset_byte + 10] & 0xff) << 2) |
                                       ((bytes[offset_byte + 11] & 0x07) << 10);
            data[i][offset_data + 7] = (bytes[offset_byte + 11] >> 3 & (0x1f)) |
                                       ((bytes[offset_byte + 12] & 0xff) << 5);
        }
    }
}

//only BS2POLq no BS2POLp
void PQCLEAN_LIGHTSABER_CLEAN_BS2POL(const unsigned char *bytes, uint16_t data[SABER_N]) {
    uint32_t j;
    uint32_t offset_data, offset_byte;

    for (j = 0; j < SABER_N / 8; j++) {
        offset_byte = 13 * j;
        offset_data = 8 * j;
        data[offset_data + 0] = (bytes[offset_byte + 0] & (0xff)) |
                                ((bytes[offset_byte + 1] & 0x1f) << 8);
        data[offset_data + 1] = (bytes[offset_byte + 1] >> 5 & (0x07)) |
                                ((bytes[offset_byte + 2] & 0xff) << 3) |
                                ((bytes[offset_byte + 3] & 0x03) << 11);
        data[offset_data + 2] = (bytes[offset_byte + 3] >> 2 & (0x3f)) |
                                ((bytes[offset_byte + 4] & 0x7f) << 6);
        data[offset_data + 3] = (bytes[offset_byte + 4] >> 7 & (0x01)) |
                                ((bytes[offset_byte + 5] & 0xff) << 1) |
                                ((bytes[offset_byte + 6] & 0x0f) << 9);
        data[offset_data + 4] = (bytes[offset_byte + 6] >> 4 & (0x0f)) |
                                ((bytes[offset_byte + 7] & 0xff) << 4) |
                                ((bytes[offset_byte + 8] & 0x01) << 12);
        data[offset_data + 5] = (bytes[offset_byte + 8] >> 1 & (0x7f)) |
                                ((bytes[offset_byte + 9] & 0x3f) << 7);
        data[offset_data + 6] = (bytes[offset_byte + 9] >> 6 & (0x03)) |
                                ((bytes[offset_byte + 10] & 0xff) << 2) |
                                ((bytes[offset_byte + 11] & 0x07) << 10);
        data[offset_data + 7] = (bytes[offset_byte + 11] >> 3 & (0x1f)) |
                                ((bytes[offset_byte + 12] & 0xff) << 5);
    }
}

void PQCLEAN_LIGHTSABER_CLEAN_POLVEC2BS(uint8_t *bytes, uint16_t data[SABER_K][SABER_N], uint16_t modulus) {
    if (modulus == 1024) {
        POLVECp2BS(bytes, data);
    } else if (modulus == 8192) {
        POLVECq2BS(bytes, data);
    }
}

void PQCLEAN_LIGHTSABER_CLEAN_BS2POLVEC(const unsigned char *bytes, uint16_t data[SABER_K][SABER_N], uint16_t modulus) {
    if (modulus == 1024) {
        BS2POLVECp(bytes, data);
    } else if (modulus == 8192) {
        BS2POLVECq(bytes, data);
    }
}
