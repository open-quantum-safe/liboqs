#ifndef __FIELDS_BITSLICED_H__
#define __FIELDS_BITSLICED_H__

#include "mqom2_parameters.h"
#include "fields.h"
typedef uint32_t felt_ext_elt_bitsliced_t;
#define BITSLICED_PACKING(n, s) (n*((s+31)>>5)*FIELD_EXT_LOG2_CARD)

#ifndef _concat3
#define _concat3(a, b, c) a ## b ## c
#endif
#ifndef concat3
#define concat3(a, b, c) _concat3(a, b, c)
#endif
#ifndef _concat5
#define _concat5(a, b, c, d, e) a ## b ## c ## d ## e
#endif
#ifndef concat5
#define concat5(a, b, c, d, e) _concat5(a, b, c, d, e)
#endif

#if defined(FIELDS_BITSLICE_COMPOSITE)
#include "fields_bitsliced_branchconst_composite.h"
#define FIELD_BITSLICE_IMPLEMENTATION_SUFFIX composite
#if defined(FIELDS_BITSLICE_PUBLIC_JUMP)
static const char fields_bitslice_conf[] = "Fields bitsliced composite (public jump)";
#else
static const char fields_bitslice_conf[] = "Fields bitsliced composite";
#endif
#else
#include "fields_bitsliced_branchconst.h"
#define FIELD_BITSLICE_IMPLEMENTATION_SUFFIX default
#if defined(FIELDS_BITSLICE_PUBLIC_JUMP)
static const char fields_bitslice_conf[] = "Fields bitsliced default (public jump)";
#else
static const char fields_bitslice_conf[] = "Fields bitsliced default";
#endif
#endif

/* Bitslice packing operations */
#define _field_ext_bitslice32_vect_pack_post concat3(FIELD_EXT_PREFIX, _bitslice32_vect_pack_post_, FIELD_BITSLICE_IMPLEMENTATION_SUFFIX)
#define _field_ext_bitslice32_vect_pack concat5(FIELD_EXT_PREFIX, _bitslice32_vect_pack_, FIELD_EXT_PREFIX, _, FIELD_BITSLICE_IMPLEMENTATION_SUFFIX)
#define _field_ext_bitslice32_vect_pack_base concat5(FIELD_EXT_PREFIX, _bitslice32_vect_pack_, FIELD_BASE_PREFIX, _, FIELD_BITSLICE_IMPLEMENTATION_SUFFIX)
#define _field_ext_bitslice32_vect_unpack_pre concat3(FIELD_EXT_PREFIX, _bitslice32_vect_unpack_pre_, FIELD_BITSLICE_IMPLEMENTATION_SUFFIX)
#define _field_ext_bitslice32_vect_unpack concat5(FIELD_EXT_PREFIX, _bitslice32_vect_unpack_, FIELD_EXT_PREFIX, _, FIELD_BITSLICE_IMPLEMENTATION_SUFFIX)

/* Direct field operations */
#define _field_ext_bitslice32_add concat3(FIELD_EXT_PREFIX, _bitslice32_add_, FIELD_BITSLICE_IMPLEMENTATION_SUFFIX)
#define _field_ext_bitslice32_mult concat3(FIELD_EXT_PREFIX, _bitslice32_mult_, FIELD_BITSLICE_IMPLEMENTATION_SUFFIX)
#define _field_ext_bitslice32_mult_hybrid concat3(FIELD_EXT_PREFIX, _bitslice32_mult_hybrid_, FIELD_BITSLICE_IMPLEMENTATION_SUFFIX)
#define _field_ext_bitslice32_mult_hybrid_base concat5(FIELD_EXT_PREFIX, _bitslice32_mult_hybrid_, FIELD_BASE_PREFIX, _, FIELD_BITSLICE_IMPLEMENTATION_SUFFIX)
#define _field_ext_bitslice32_mult_hybrid_public concat3(FIELD_EXT_PREFIX, _bitslice32_mult_hybrid_public_, FIELD_BITSLICE_IMPLEMENTATION_SUFFIX)

static inline uint32_t *field_ext_bitslice_vect_get(felt_ext_elt_bitsliced_t* x_bitsliced, uint32_t pos, uint32_t nb_bitsliced) {
	uint32_t scale32 = FIELD_EXT_LOG2_CARD * ((nb_bitsliced + 31) >> 5);
	return x_bitsliced + scale32 * pos;
}

static inline const felt_ext_elt_bitsliced_t *field_ext_bitslice_vect_get_const(const felt_ext_elt_bitsliced_t* x_bitsliced, uint32_t pos, uint32_t nb_bitsliced) {
	uint32_t scale32 = FIELD_EXT_LOG2_CARD * ((nb_bitsliced + 31) >> 5);
	return x_bitsliced + scale32 * pos;
}

static inline void field_ext_bitslice_vect_pack_pre(felt_ext_elt_bitsliced_t* x_bitsliced, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t scale32 = FIELD_EXT_LOG2_CARD * ((nb_bitsliced + 31) >> 5);
	memset(x_bitsliced, 0, sizeof(uint32_t)*size * scale32);
}

static inline void field_ext_bitslice_vect_pack_post(felt_ext_elt_bitsliced_t* x_bitsliced, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t nb_packs = ((nb_bitsliced + 31) >> 5);
	for (uint32_t i = 0; i < (nb_packs * size); i++) {
		_field_ext_bitslice32_vect_pack_post(&x_bitsliced[FIELD_EXT_LOG2_CARD * i]);
	}
}

static inline void field_ext_bitslice_vect_pack(const field_ext_elt* x, felt_ext_elt_bitsliced_t* x_bitsliced, uint32_t index, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t index32 = index & 0x1F;
	uint32_t offset32 = FIELD_EXT_LOG2_CARD * (index >> 5);
	uint32_t scale32 = FIELD_EXT_LOG2_CARD * ((nb_bitsliced + 31) >> 5);
	uint32_t *pos = x_bitsliced + offset32;
	for (uint32_t j = 0; j < size; j++) {
		field_ext_elt elt = field_ext_vect_unpack(x, j);
		_field_ext_bitslice32_vect_pack(elt, pos, index32);
		pos += scale32;
	}
}

static inline void field_ext_bitslice_vect_pack_base(const field_base_elt* x, felt_ext_elt_bitsliced_t* x_bitsliced, uint32_t index, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t index32 = index & 0x1F;
	uint32_t offset32 = FIELD_EXT_LOG2_CARD * (index >> 5);
	uint32_t scale32 = FIELD_EXT_LOG2_CARD * ((nb_bitsliced + 31) >> 5);
	uint32_t *pos = x_bitsliced + offset32;
	for (uint32_t j = 0; j < size; j++) {
		field_base_elt elt = field_base_vect_unpack(x, j);
		_field_ext_bitslice32_vect_pack_base(elt, pos, index32);
		pos += scale32;
	}
}

static inline void field_ext_bitslice_vect_unpack_pre(felt_ext_elt_bitsliced_t* x_bitsliced, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t nb_packs = ((nb_bitsliced + 31) >> 5);
	for (uint32_t i = 0; i < (nb_packs * size); i++) {
		_field_ext_bitslice32_vect_unpack_pre(&x_bitsliced[FIELD_EXT_LOG2_CARD * i]);
	}
}

static inline void field_ext_bitslice_vect_unpack(const felt_ext_elt_bitsliced_t* x_bitsliced, field_ext_elt* x, uint32_t index, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t index32 = index & 0x1F;
	uint32_t offset32 = FIELD_EXT_LOG2_CARD * (index >> 5);
	uint32_t scale32 = FIELD_EXT_LOG2_CARD * ((nb_bitsliced + 31) >> 5);
	const uint32_t *pos = x_bitsliced + offset32;
	for (uint32_t j = 0; j < size; j++) {
		field_ext_elt elt = _field_ext_bitslice32_vect_unpack(pos, index32);
		field_ext_vect_pack(elt, x, j);
		pos += scale32;
	}
}

static inline void field_ext_bitslice_add(felt_ext_elt_bitsliced_t* z_bitsliced, const felt_ext_elt_bitsliced_t* x_bitsliced, const felt_ext_elt_bitsliced_t* y_bitsliced, uint32_t nb_bitsliced) {
	uint32_t scale32 = FIELD_EXT_LOG2_CARD * ((nb_bitsliced + 31) >> 5);
	for (uint32_t i = 0; i < scale32; i++) {
		z_bitsliced[i] = x_bitsliced[i] ^ y_bitsliced[i];
	}
}

static inline void field_ext_bitslice_vect_add(felt_ext_elt_bitsliced_t* z_bitsliced, const felt_ext_elt_bitsliced_t* x_bitsliced, const felt_ext_elt_bitsliced_t* y_bitsliced, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t scale32 = FIELD_EXT_LOG2_CARD * ((nb_bitsliced + 31) >> 5);
	for (uint32_t i = 0; i < (scale32 * size); i++) {
		z_bitsliced[i] = x_bitsliced[i] ^ y_bitsliced[i];
	}
}

static inline void field_ext_bitslice_mult(felt_ext_elt_bitsliced_t* res, const felt_ext_elt_bitsliced_t* x, const felt_ext_elt_bitsliced_t* y, uint32_t nb_bitsliced) {
	uint32_t nb_packs = ((nb_bitsliced + 31) >> 5);
	for (uint32_t i = 0; i < nb_packs; i++) {
		_field_ext_bitslice32_mult(&res[FIELD_EXT_LOG2_CARD * i], &x[FIELD_EXT_LOG2_CARD * i], &y[FIELD_EXT_LOG2_CARD * i]);
	}
}

static inline void field_ext_bitslice_vect_mult_hybrid(felt_ext_elt_bitsliced_t* res, const field_ext_elt* x, const felt_ext_elt_bitsliced_t* y, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t i, j;
	uint32_t tmp[FIELD_EXT_LOG2_CARD];
	uint32_t nb_packs = ((nb_bitsliced + 31) >> 5);

	memset(res, 0, sizeof(uint32_t[FIELD_EXT_LOG2_CARD])*nb_packs);
	for (j = 0; j < nb_packs; j++) {
		for (i = 0; i < size; i++) {
			field_ext_elt elt = field_ext_vect_unpack(x, i);
			_field_ext_bitslice32_mult_hybrid(tmp, elt, &y[FIELD_EXT_LOG2_CARD * (nb_packs * i + j)]);
			_field_ext_bitslice32_add(&res[FIELD_EXT_LOG2_CARD * j], &res[FIELD_EXT_LOG2_CARD * j], tmp);
		}
	}
}

static inline void field_ext_bitslice_vect_mult_hybrid_public(felt_ext_elt_bitsliced_t* res, const field_ext_elt* x, const felt_ext_elt_bitsliced_t* y, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t i, j;
	uint32_t tmp[FIELD_EXT_LOG2_CARD];
	uint32_t nb_packs = ((nb_bitsliced + 31) >> 5);

	memset(res, 0, sizeof(uint32_t[FIELD_EXT_LOG2_CARD])*nb_packs);
	for (j = 0; j < nb_packs; j++) {
		for (i = 0; i < size; i++) {
			field_ext_elt elt = field_ext_vect_unpack(x, i);
			_field_ext_bitslice32_mult_hybrid_public(tmp, elt, &y[FIELD_EXT_LOG2_CARD * (nb_packs * i + j)]);
			_field_ext_bitslice32_add(&res[FIELD_EXT_LOG2_CARD * j], &res[FIELD_EXT_LOG2_CARD * j], tmp);
		}
	}
}

static inline void field_ext_bitslice_const_vect_mult_hybrid(felt_ext_elt_bitsliced_t* res, const felt_ext_elt_bitsliced_t* sc, const field_ext_elt* x, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t i, j;
	uint32_t nb_packs = ((nb_bitsliced + 31) >> 5);

	for (j = 0; j < nb_packs; j++) {
		for (i = 0; i < size; i++) {
			field_ext_elt elt = field_ext_vect_unpack(x, i);
			_field_ext_bitslice32_mult_hybrid(&res[FIELD_EXT_LOG2_CARD * (nb_packs * i + j)], elt, &sc[FIELD_EXT_LOG2_CARD * j]);
		}
	}
}

static inline void field_ext_bitslice_vect_mult_hybrid_base(felt_ext_elt_bitsliced_t* res, const field_base_elt* x, const felt_ext_elt_bitsliced_t* y, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t i, j;
	uint32_t tmp[FIELD_EXT_LOG2_CARD];
	uint32_t nb_packs = ((nb_bitsliced + 31) >> 5);

	memset(res, 0, sizeof(uint32_t[FIELD_EXT_LOG2_CARD])*nb_packs);
	for (j = 0; j < nb_packs; j++) {
		for (i = 0; i < size; i++) {
			field_base_elt elt = field_base_vect_unpack(x, i);
			_field_ext_bitslice32_mult_hybrid_base(tmp, elt, &y[FIELD_EXT_LOG2_CARD * (nb_packs * i + j)]);
			_field_ext_bitslice32_add(&res[FIELD_EXT_LOG2_CARD * j], &res[FIELD_EXT_LOG2_CARD * j], tmp);
		}
	}
}

static inline void field_ext_bitslice_const_vect_mult(felt_ext_elt_bitsliced_t* res, const felt_ext_elt_bitsliced_t* sc, const felt_ext_elt_bitsliced_t* x, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t i, j;
	uint32_t nb_packs = ((nb_bitsliced + 31) >> 5);

	for (j = 0; j < nb_packs; j++) {
		for (i = 0; i < size; i++) {
			_field_ext_bitslice32_mult(&res[FIELD_EXT_LOG2_CARD * (nb_packs * i + j)], &x[FIELD_EXT_LOG2_CARD * (nb_packs * i + j)], &sc[FIELD_EXT_LOG2_CARD * j]);
		}
	}
}

static inline void field_ext_bitslice_vect_mult(uint32_t* res, const uint32_t* x, const uint32_t* y, uint32_t size, uint32_t nb_bitsliced) {
	uint32_t i, j;
	uint32_t tmp[FIELD_EXT_LOG2_CARD];
	uint32_t nb_packs = ((nb_bitsliced + 31) >> 5);

	memset(res, 0, sizeof(uint32_t[FIELD_EXT_LOG2_CARD])*nb_packs);
	for (j = 0; j < nb_packs; j++) {
		for (i = 0; i < size; i++) {
			_field_ext_bitslice32_mult(tmp, &x[FIELD_EXT_LOG2_CARD * (nb_packs * i + j)], &y[FIELD_EXT_LOG2_CARD * (nb_packs * i + j)]);
			_field_ext_bitslice32_add(&res[FIELD_EXT_LOG2_CARD * j], &res[FIELD_EXT_LOG2_CARD * j], tmp);
		}
	}
}

#endif /* __FIELDS_BITSLICED_H__ */
