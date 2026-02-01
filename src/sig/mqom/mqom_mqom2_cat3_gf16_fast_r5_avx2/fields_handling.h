#ifndef __FIELDS_HANDLING_H__
#define __FIELDS_HANDLING_H__

/* Needed for primitive types definitions */
#include <stdint.h>

/* ==== Packing primitives ====== */
static inline void field_gf2_vect_pack(uint8_t elt, uint8_t *packed_elt, uint32_t index) {
	packed_elt[index / 8] &= ~(1 << (index % 8));
	packed_elt[index / 8] |= (elt << (index % 8));
	return;
}
static inline void field_gf4_vect_pack(uint8_t elt, uint8_t *packed_elt, uint32_t index) {
	packed_elt[index / 4] &= ~(0x03 << (2 * (index % 4)));
	packed_elt[index / 4] |= (elt << (2 * (index % 4)));
	return;
}
static inline void field_gf16_vect_pack(uint8_t elt, uint8_t *packed_elt, uint32_t index) {
	packed_elt[index / 2] &= ~(0x0F << (4 * (index % 2)));
	packed_elt[index / 2] |= (elt << (4 * (index % 2)));
	return;
}
static inline void field_gf256_vect_pack(uint8_t elt, uint8_t *packed_elt, uint32_t index) {
	packed_elt[index] = elt;
	return;
}
static inline void field_gf256to2_vect_pack(uint16_t elt, uint16_t *packed_elt, uint32_t index) {
	packed_elt[index] = elt;
	return;
}

/* ==== Unpacking primitives ====== */
static inline uint8_t field_gf2_vect_unpack(const uint8_t *packed_elt, uint32_t index) {
	return (packed_elt[index / 8] >> (index % 8)) & 1;
}
static inline uint8_t field_gf4_vect_unpack(const uint8_t *packed_elt, uint32_t index) {
	return (packed_elt[index / 4] >> (2 * (index % 4))) & 0x03;
}
static inline uint8_t field_gf16_vect_unpack(const uint8_t *packed_elt, uint32_t index) {
	return (packed_elt[index / 2] >> (4 * (index % 2))) & 0x0F;
}
static inline uint8_t field_gf256_vect_unpack(const uint8_t *packed_elt, uint32_t index) {
	return packed_elt[index];
}
static inline uint16_t field_gf256to2_vect_unpack(const uint16_t *packed_elt, uint32_t index) {
	return packed_elt[index];
}

/* ==== String parsing primitives ====== */
static inline void field_gf2_parse(const uint8_t *in_string, uint32_t num_bits, uint8_t *elements) {
	uint32_t num_bytes = num_bits / 8;
	uint8_t leftover = (uint8_t)(num_bits % 8);

	/* Memcpy as much elements as we can */
	memcpy(elements, in_string, num_bytes);
	/* Deal with the possible last left over bits */
	if (leftover) {
		uint8_t mask = ((1 << leftover) - 1);
		elements[num_bytes] = in_string[num_bytes] & mask;
	}
	return;
}
static inline void field_gf4_parse(const uint8_t *in_string, uint32_t num, uint8_t *elements) {
	uint32_t num_bytes = num / 4;
	uint8_t leftover = 2 * ((uint8_t)(num % 4));

	/* Memcpy as much elements as we can */
	memcpy(elements, in_string, num_bytes);
	/* Deal with the possible last left over bits */
	if (leftover) {
		uint8_t mask = ((1 << leftover) - 1);
		elements[num_bytes] = in_string[num_bytes] & mask;
	}
	return;
}
static inline void field_gf16_parse(const uint8_t *in_string, uint32_t num, uint8_t *elements) {
	uint32_t num_bytes = num / 2;
	uint8_t leftover = 4 * ((uint8_t)(num % 2));

	/* Memcpy as much elements as we can */
	memcpy(elements, in_string, num_bytes);
	/* Deal with the possible last left over bits */
	if (leftover) {
		uint8_t mask = ((1 << leftover) - 1);
		elements[num_bytes] = in_string[num_bytes] & mask;
	}
	return;
}
static inline void field_gf256_parse(const uint8_t *in_string, uint32_t num, uint8_t *elements) {
	/* Since we map bytes, this is simply a memcpy */
	memcpy(elements, in_string, num);
	return;
}
static inline void field_gf256to2_parse(const uint8_t *in_string, uint32_t num, uint16_t *elements) {
	if (!arch_is_big_endian()) {
		/* For little endian platforms, this is a memcpy */
		memcpy(elements, in_string, (2 * num));
	} else {
		/* For big endian platforms, we have to reverse the bytes */
		uint32_t i;
		for (i = 0; i < (2 * num); i += 2) {
			elements[i / 2] = (in_string[i + 1] << 8) | in_string[i];
		}
	}
	return;
}


/* ==== Serialization primitives ====== */
static inline void field_gf2_serialize(const uint8_t *elements, uint32_t num, uint8_t *out_string) {
	/* This is simply a memcpy
	     * NOTE: in the serialization cases, we only deal with number of bits multiple of bytes
	 * */
	memcpy(out_string, elements, (num / 8));
	return;
}
static inline void field_gf4_serialize(const uint8_t *elements, uint32_t num, uint8_t *out_string) {
	/* This is simply a memcpy
	     * NOTE: in the serialization cases, we only deal with number of bits multiple of bytes
	 * */
	memcpy(out_string, elements, (num / 4));
	return;
}
static inline void field_gf16_serialize(const uint8_t *elements, uint32_t num, uint8_t *out_string) {
	/* This is simply a memcpy
	     * NOTE: in the serialization cases, we only deal with number of bits multiple of bytes
	 * */
	memcpy(out_string, elements, (num / 2));
	return;
}
static inline void field_gf256_serialize(const uint8_t *elements, uint32_t num, uint8_t *out_string) {
	/* This is simply a memcpy */
	memcpy(out_string, elements, num);
	return;
}
static inline void field_gf256to2_serialize(const uint16_t *elements, uint32_t num, uint8_t *out_string) {
	if (!arch_is_big_endian()) {
		/* For little endian platforms, this is a memcpy */
		memcpy(out_string, elements, (2 * num));
	} else {
		/* For big endian platforms, we have to reverse the bytes */
		uint32_t i;
		for (i = 0; i < (2 * num); i += 2) {
			out_string[i]   = (elements[i / 2] & 0xff);
			out_string[i + 1] = (elements[i / 2] >> 8);
		}
	}
	return;
}

#endif /* __FIELDS_HANDLING_H__ */
