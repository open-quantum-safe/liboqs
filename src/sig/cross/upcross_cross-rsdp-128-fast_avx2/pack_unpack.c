/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 2.2 (July 2025)
 *
 * Authors listed in alphabetical order:
 *
 * @author: Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author: Marco Gianvecchio <marco.gianvecchio@mail.polimi.it>
 * @author: Patrick Karl <patrick.karl@tum.de>
 * @author: Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author: Jonas Schupp <jonas.schupp@tum.de>
 *
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#include <stddef.h>
#include <stdint.h>

#include "pack_unpack.h"

/*Implementation of packing and unpacking of 3/7/9 bit elements into byte vectors*/

/* liboqs-edit: generic_pack_3_bit is unused in RSDPG */
/*
 * generic_pack_3_bit()
 *
 * uint8_t *out       :    3 bit inputs, packed in bytes
 * const uint8_t *in  :    uint8_t Vec input, to be packed
 * size_t outlen      :    Length of out
 * size_t in          :    Length of in
 *
 * This function handles the packing of an vector of uint8_t elements with 3 bit of information
 * of arbitrary length
 */
static inline
void generic_pack_3_bit(uint8_t *out, const uint8_t *in,
                        const size_t outlen, const size_t inlen) {
	size_t i;
	for (i = 0; i < outlen; i++) {
		out[i] = 0;
	}
	for (i = 0; i < inlen / 8; i++) {
		out[i * 3]   = in[i * 8] | (in[i * 8 + 1] << 3) | (in[i * 8 + 2] << 6);
		out[i * 3 + 1]  = (in[i * 8 + 2] >> 2) | (in[i * 8 + 3] << 1) | (in[i * 8 + 4] << 4) | (in[i * 8 + 5] << 7);
		out[i * 3 + 2]  = (in[i * 8 + 5] >> 1) | (in[i * 8 + 6] << 2) | (in[i * 8 + 7] << 5);
	}
	const uint8_t n_remainder = inlen & 0x7;
	if (n_remainder == 1) {
		out[i * 3]   = in[i * 8];
	} else if (n_remainder == 2) {
		out[i * 3]   = in[i * 8] | (in[i * 8 + 1] << 3);
	} else if (n_remainder == 3) {
		out[i * 3]   = in[i * 8] | (in[i * 8 + 1] << 3) | (in[i * 8 + 2] << 6);
		out[i * 3 + 1]  = (in[i * 8 + 2] >> 2);
	} else if (n_remainder == 4) {
		out[i * 3]   = in[i * 8] | (in[i * 8 + 1] << 3) | (in[i * 8 + 2] << 6);
		out[i * 3 + 1]  = (in[i * 8 + 2] >> 2)  | (in[i * 8 + 3] << 1);
	} else if (n_remainder == 5) {
		out[i * 3]   = in[i * 8] | (in[i * 8 + 1] << 3) | (in[i * 8 + 2] << 6);
		out[i * 3 + 1]  = (in[i * 8 + 2] >> 2)  | (in[i * 8 + 3] << 1) | (in[i * 8 + 4] << 4);
	} else if (n_remainder == 6) {
		out[i * 3]   = in[i * 8] | (in[i * 8 + 1] << 3) | (in[i * 8 + 2] << 6);
		out[i * 3 + 1]  = (in[i * 8 + 2] >> 2) | (in[i * 8 + 3] << 1) | (in[i * 8 + 4] << 4) | (in[i * 8 + 5] << 7);
		out[i * 3 + 2]  = (in[i * 8 + 5] >> 1);
	} else if (n_remainder == 7) {
		out[i * 3]   = in[i * 8] | (in[i * 8 + 1] << 3) | (in[i * 8 + 2] << 6);
		out[i * 3 + 1]  = (in[i * 8 + 2] >> 2) | (in[i * 8 + 3] << 1) | (in[i * 8 + 4] << 4) | (in[i * 8 + 5] << 7);
		out[i * 3 + 2]  = (in[i * 8 + 5] >> 1) | (in[i * 8 + 6] << 2);
	}
}

/*
 * generic_pack_7_bit()
 *
 * uint8_t *out       :    7 bit inputs packed in bytes
 * const uint8_t *in  :    uint8_t Vec input, to be packed
 * size_t outlen      :    Length of out
 * size_t in          :    Length of in
 *
 * This function handles the packing of an vector of uint8_t elements with 7 bit of information
 * of arbitrary length
 */
static inline
void generic_pack_7_bit(uint8_t *out, const uint8_t *in,
                        const size_t outlen, const size_t inlen) {
	size_t i;
	for (i = 0; i < outlen; i++) {
		out[i] = 0;
	}
	for (i = 0; i < inlen / 8; i++) {
		out[i * 7] |= (in[i * 8]) | (in[i * 8 + 1] << 7);
		out[i * 7 + 1] |= (in[i * 8 + 1] >> 1) | (in[i * 8 + 2] << 6);
		out[i * 7 + 2] |= (in[i * 8 + 2] >> 2) | (in[i * 8 + 3] << 5);
		out[i * 7 + 3] |= (in[i * 8 + 3] >> 3) | (in[i * 8 + 4] << 4);
		out[i * 7 + 4] |= (in[i * 8 + 4] >> 4) | (in[i * 8 + 5] << 3);
		out[i * 7 + 5] |= (in[i * 8 + 5] >> 5) | (in[i * 8 + 6] << 2);
		out[i * 7 + 6] |= (in[i * 8 + 6] >> 6) | (in[i * 8 + 7] << 1);
	}
	const uint8_t n_remainder = inlen & 0x7;
	if (n_remainder == 1) {
		out[i * 7] = (in[i * 8]);
	} else if (n_remainder == 2) {
		out[i * 7] |= (in[i * 8]) | (in[i * 8 + 1] << 7);
		out[i * 7 + 1] = (in[i * 8 + 1] >> 1);
	} else if (n_remainder == 3) {
		out[i * 7] |= (in[i * 8]) | (in[i * 8 + 1] << 7);
		out[i * 7 + 1] |= (in[i * 8 + 1] >> 1) | (in[i * 8 + 2] << 6);
		out[i * 7 + 2] = (in[i * 8 + 2] >> 2);
	} else if (n_remainder == 4) {
		out[i * 7] |= (in[i * 8]) | (in[i * 8 + 1] << 7);
		out[i * 7 + 1] |= (in[i * 8 + 1] >> 1) | (in[i * 8 + 2] << 6);
		out[i * 7 + 2] |= (in[i * 8 + 2] >> 2) | (in[i * 8 + 3] << 5);
		out[i * 7 + 3] |= (in[i * 8 + 3] >> 3);
	} else if (n_remainder == 5) {
		out[i * 7] |= (in[i * 8]) | (in[i * 8 + 1] << 7);
		out[i * 7 + 1] |= (in[i * 8 + 1] >> 1) | (in[i * 8 + 2] << 6);
		out[i * 7 + 2] |= (in[i * 8 + 2] >> 2) | (in[i * 8 + 3] << 5);
		out[i * 7 + 3] |= (in[i * 8 + 3] >> 3) | (in[i * 8 + 4] << 4);
		out[i * 7 + 4] |= (in[i * 8 + 4] >> 4);
	} else if (n_remainder == 6) {
		out[i * 7] |= (in[i * 8]) | (in[i * 8 + 1] << 7);
		out[i * 7 + 1] |= (in[i * 8 + 1] >> 1) | (in[i * 8 + 2] << 6);
		out[i * 7 + 2] |= (in[i * 8 + 2] >> 2) | (in[i * 8 + 3] << 5);
		out[i * 7 + 3] |= (in[i * 8 + 3] >> 3) | (in[i * 8 + 4] << 4);
		out[i * 7 + 4] |= (in[i * 8 + 4] >> 4) | (in[i * 8 + 5] << 3);
		out[i * 7 + 5] |= (in[i * 8 + 5] >> 5);
	} else if (n_remainder == 7) {
		out[i * 7] |= (in[i * 8]) | (in[i * 8 + 1] << 7);
		out[i * 7 + 1] |= (in[i * 8 + 1] >> 1) | (in[i * 8 + 2] << 6);
		out[i * 7 + 2] |= (in[i * 8 + 2] >> 2) | (in[i * 8 + 3] << 5);
		out[i * 7 + 3] |= (in[i * 8 + 3] >> 3) | (in[i * 8 + 4] << 4);
		out[i * 7 + 4] |= (in[i * 8 + 4] >> 4) | (in[i * 8 + 5] << 3);
		out[i * 7 + 5] |= (in[i * 8 + 5] >> 5) | (in[i * 8 + 6] << 2);
		out[i * 7 + 6] |= (in[i * 8 + 6] >> 6);
	}
}

/* liboqs-edit: generic_pack_9_bit is unused in RSDP */

/*
 * generic_pack_fp()
 *
 * uint8_t *out       :    FP packed in bytes
 * const FP_ELEM *in  :    FP Vec input, to be packed
 * size_t outlen      :    Length of out
 * size_t in          :    Length of in
 *
 * This function handles the packing of an vector of el. in FP of arbit. length
 */
static inline
void generic_pack_fp(uint8_t *out, const FP_ELEM *in,
                     const size_t outlen, const size_t inlen) {
	generic_pack_7_bit(out, in, outlen, inlen);

}

/*
 * generic_pack_fz()
 *
 * uint8_t *out      :    FZ packed in bytes
 * const FZ_ELEM *in :    FZ Vec input, to be packed
 * size_t outlen     :    Length of out
 * size_t in         :    Length of in
 *
 * This function handles the packing of an vector of el. in FZ of arbit. length
 */
static inline
void generic_pack_fz(uint8_t *out, const FZ_ELEM *in, const size_t outlen, const size_t inlen) {
	generic_pack_3_bit(out, in, outlen, inlen);

}

/* liboqs-edit: generic_unpack_3_bit is unused in RSDPG */
/*
 * generic_unpack_3_bit()
 *
 * uint8_t *out       :    uint8_t output, unpacked
 * const uint8_t *in  :    3 bit input, packed in bytes
 * size_t outlen      :    Length of out
 * size_t in          :    Length of in
 *
 * This function handles the packing of an vector of uint8_t elements with 3 bit of information
 * of arbitrary length
 */
static inline
uint8_t generic_unpack_3_bit(uint8_t *out, const uint8_t *in,
                             const size_t outlen, const size_t inlen) {
	uint8_t is_packed_padd_ok = 1;
	size_t i;
	for (i = 0; i < outlen; i++) {
		out[i] = 0;
	}
	for (i = 0; i < outlen / 8; i++) {
		out[i * 8]   = in[i * 3] & 0x7;
		out[i * 8 + 1]  = (in[i * 3] >> 3) & 0x7;
		out[i * 8 + 2]  = ((in[i * 3] >> 6) | (in[i * 3 + 1] << 2)) & 0x7;
		out[i * 8 + 3]  = (in[i * 3 + 1] >> 1) & 0x7;
		out[i * 8 + 4]  = (in[i * 3 + 1] >> 4) & 0x7;
		out[i * 8 + 5]  = ((in[i * 3 + 1] >> 7) | (in[i * 3 + 2] << 1)) & 0x7;
		out[i * 8 + 6]  = (in[i * 3 + 2] >> 2) & 0x7;
		out[i * 8 + 7]  = (in[i * 3 + 2] >> 5) & 0x7;
	}
	const uint8_t n_remainder = outlen & 0x7;
	if (n_remainder == 1) {
		out[i * 8]   = in[i * 3] & 0x7;
	} else if (n_remainder == 2) {
		out[i * 8]   = in[i * 3] & 0x7;
		out[i * 8 + 1]  = (in[i * 3] >> 3) & 0x7;
	} else if (n_remainder == 3) {
		out[i * 8]   = in[i * 3] & 0x7;
		out[i * 8 + 1]  = (in[i * 3] >> 3) & 0x7;
		out[i * 8 + 2]  = ((in[i * 3] >> 6) | (in[i * 3 + 1] << 2)) & 0x7;
	} else if (n_remainder == 4) {
		out[i * 8]   = in[i * 3] & 0x7;
		out[i * 8 + 1]  = (in[i * 3] >> 3) & 0x7;
		out[i * 8 + 2]  = ((in[i * 3] >> 6) | (in[i * 3 + 1] << 2)) & 0x7;
		out[i * 8 + 3]  = (in[i * 3 + 1] >> 1) & 0x7;
	} else if (n_remainder == 5) {
		out[i * 8]   = in[i * 3] & 0x7;
		out[i * 8 + 1]  = (in[i * 3] >> 3) & 0x7;
		out[i * 8 + 2]  = ((in[i * 3] >> 6) | (in[i * 3 + 1] << 2)) & 0x7;
		out[i * 8 + 3]  = (in[i * 3 + 1] >> 1) & 0x7;
		out[i * 8 + 4]  = (in[i * 3 + 1] >> 4) & 0x7;
	} else if (n_remainder == 6) {
		out[i * 8]   = in[i * 3] & 0x7;
		out[i * 8 + 1]  = (in[i * 3] >> 3) & 0x7;
		out[i * 8 + 2]  = ((in[i * 3] >> 6) | (in[i * 3 + 1] << 2)) & 0x7;
		out[i * 8 + 3]  = (in[i * 3 + 1] >> 1) & 0x7;
		out[i * 8 + 4]  = (in[i * 3 + 1] >> 4) & 0x7;
		out[i * 8 + 5]  = ((in[i * 3 + 1] >> 7) | (in[i * 3 + 2] << 1)) & 0x7;
	} else if (n_remainder == 7) {
		out[i * 8]   = in[i * 3] & 0x7;
		out[i * 8 + 1]  = (in[i * 3] >> 3) & 0x7;
		out[i * 8 + 2]  = ((in[i * 3] >> 6) | (in[i * 3 + 1] << 2)) & 0x7;
		out[i * 8 + 3]  = (in[i * 3 + 1] >> 1) & 0x7;
		out[i * 8 + 4]  = (in[i * 3 + 1] >> 4) & 0x7;
		out[i * 8 + 5]  = ((in[i * 3 + 1] >> 7) | (in[i * 3 + 2] << 1)) & 0x7;
		out[i * 8 + 6]  = (in[i * 3 + 2] >> 2) & 0x7;
	}
	if (n_remainder > 0) {
		is_packed_padd_ok = ((in[inlen - 1] & (0xFF << (n_remainder * 3) & 0x7)) == 0);
	}

	return is_packed_padd_ok;
}

/*
 * generic_unpack_7_bit()
 *
 * uint8_t *out       :    uint8_t output, unpacked
 * const uint8_t *in  :    7 bit input, packed in bytes
 * size_t outlen      :    Length of out
 * size_t in          :    Length of in
 *
 * This function handles the packing of an vector of uint8_t elements with 7 bit of information
 * of arbitrary length
 */
static inline
uint8_t generic_unpack_7_bit(uint8_t *out, const uint8_t *in,
                             const size_t outlen, const size_t inlen) {
	uint8_t is_packed_padd_ok = 1;
	size_t i;
	for (i = 0; i < outlen; i++) {
		out[i] = 0;
	}
	for (i = 0; i < outlen / 8; i++) {
		out[i * 8]   = in[i * 7] & 0x7F;
		out[i * 8 + 1]  = (in[i * 7] >> 7) | ((in[i * 7 + 1] << 1) & 0x7F);
		out[i * 8 + 2]  = (in[i * 7 + 1] >> 6) | ((in[i * 7 + 2] << 2) & 0x7F);
		out[i * 8 + 3]  = (in[i * 7 + 2] >> 5) | ((in[i * 7 + 3] << 3) & 0x7F);
		out[i * 8 + 4]  = (in[i * 7 + 3] >> 4) | ((in[i * 7 + 4] << 4) & 0x7F);
		out[i * 8 + 5]  = (in[i * 7 + 4] >> 3) | ((in[i * 7 + 5] << 5) & 0x7F);
		out[i * 8 + 6]  = (in[i * 7 + 5] >> 2) | ((in[i * 7 + 6] << 6) & 0x7F);
		out[i * 8 + 7]  = in[i * 7 + 6] >> 1;
	}
	const uint8_t n_remainder = outlen & 0x7;
	if (n_remainder == 1) {
		out[i * 8]   = in[i * 7] & 0x7F;
	} else if (n_remainder == 2) {
		out[i * 8]   = in[i * 7] & 0x7F;
		out[i * 8 + 1]  = (in[i * 7] >> 7) | ((in[i * 7 + 1] << 1) & 0x7F);
	} else if (n_remainder == 3) {
		out[i * 8]   = in[i * 7] & 0x7F;
		out[i * 8 + 1]  = (in[i * 7] >> 7) | ((in[i * 7 + 1] << 1) & 0x7F);
		out[i * 8 + 2]  = (in[i * 7 + 1] >> 6) | ((in[i * 7 + 2] << 2) & 0x7F);
	} else if (n_remainder == 4) {
		out[i * 8]   = in[i * 7] & 0x7F;
		out[i * 8 + 1]  = (in[i * 7] >> 7) | ((in[i * 7 + 1] << 1) & 0x7F);
		out[i * 8 + 2]  = (in[i * 7 + 1] >> 6) | ((in[i * 7 + 2] << 2) & 0x7F);
		out[i * 8 + 3]  = (in[i * 7 + 2] >> 5) | ((in[i * 7 + 3] << 3) & 0x7F);
	} else if (n_remainder == 5) {
		out[i * 8]   = in[i * 7] & 0x7F;
		out[i * 8 + 1]  = (in[i * 7] >> 7) | ((in[i * 7 + 1] << 1) & 0x7F);
		out[i * 8 + 2]  = (in[i * 7 + 1] >> 6) | ((in[i * 7 + 2] << 2) & 0x7F);
		out[i * 8 + 3]  = (in[i * 7 + 2] >> 5) | ((in[i * 7 + 3] << 3) & 0x7F);
		out[i * 8 + 4]  = (in[i * 7 + 3] >> 4) | ((in[i * 7 + 4] << 4) & 0x7F);
	} else if (n_remainder == 6) {
		out[i * 8]   = in[i * 7] & 0x7F;
		out[i * 8 + 1]  = (in[i * 7] >> 7) | ((in[i * 7 + 1] << 1) & 0x7F);
		out[i * 8 + 2]  = (in[i * 7 + 1] >> 6) | ((in[i * 7 + 2] << 2) & 0x7F);
		out[i * 8 + 3]  = (in[i * 7 + 2] >> 5) | ((in[i * 7 + 3] << 3) & 0x7F);
		out[i * 8 + 4]  = (in[i * 7 + 3] >> 4) | ((in[i * 7 + 4] << 4) & 0x7F);
		out[i * 8 + 5]  = (in[i * 7 + 4] >> 3) | ((in[i * 7 + 5] << 5) & 0x7F);
	} else if (n_remainder == 7) {
		out[i * 8]   = in[i * 7] & 0x7F;
		out[i * 8 + 1]  = (in[i * 7] >> 7) | ((in[i * 7 + 1] << 1) & 0x7F);
		out[i * 8 + 2]  = (in[i * 7 + 1] >> 6) | ((in[i * 7 + 2] << 2) & 0x7F);
		out[i * 8 + 3]  = (in[i * 7 + 2] >> 5) | ((in[i * 7 + 3] << 3) & 0x7F);
		out[i * 8 + 4]  = (in[i * 7 + 3] >> 4) | ((in[i * 7 + 4] << 4) & 0x7F);
		out[i * 8 + 5]  = (in[i * 7 + 4] >> 3) | ((in[i * 7 + 5] << 5) & 0x7F);
		out[i * 8 + 6]  = (in[i * 7 + 5] >> 2) | ((in[i * 7 + 6] << 6) & 0x7F);
	}
	if (n_remainder > 0) {
		is_packed_padd_ok = ((in[inlen - 1] & (0xFF << (8 - n_remainder))) == 0);
	}

	return is_packed_padd_ok;
}

/* liboqs-edit: generic_unpack_9_bit is unused in RSDP */

/*
 * generic_unpack_fp()
 *
 * FP_ELEM *out      :    FP output, unpacked
 * const uint8_t *in :    FP Vec input, packed in bytes
 * size_t outlen     :    Length of out
 * size_t in         :    Length of in
 *
 * This function unpacks an vector of el. in FP of arbit. length
 */
static inline
uint8_t generic_unpack_fp(FP_ELEM *out, const uint8_t *in,
                          size_t outlen, size_t inlen) {
	uint8_t is_packed_padd_ok = 1;
	is_packed_padd_ok = generic_unpack_7_bit(out, in, outlen, inlen);

	return is_packed_padd_ok;
}

/*
 * generic_unpack_fz()
 *
 * FZ_ELEM *out      :    FZ output, unpacked
 * const uint8_t *in :    FZ Vec input, packed in bytes
 * size_t outlen     :    Length of out
 * size_t in         :    Length of in
 *
 * This function unpacks an vector of el. in FZ of arbit. length
 */
static inline
uint8_t generic_unpack_fz(FZ_ELEM *out, const uint8_t *in,
                          size_t outlen, size_t inlen) {
	uint8_t is_packed_padd_ok = 1;
	is_packed_padd_ok = generic_unpack_3_bit(out, in, outlen, inlen);

	return is_packed_padd_ok;
}

/*
 * pack_fp_vec()
 *
 * uint8_t out[DENSELY_PACKED_FP_VEC_SIZE]    :    FP packed in bytes
 * const FP_ELEM in[N]                        :    FP Vec input, to be packed
 *
 * This function handles the packing of FP
 */
void pack_fp_vec(uint8_t out[DENSELY_PACKED_FP_VEC_SIZE],
                 const FP_ELEM in[N]) {
	generic_pack_fp(out, in, DENSELY_PACKED_FP_VEC_SIZE, N);
}

/*
 * pack_fp_syn()
 *
 * uint8_t out[DENSELY_PACKED_FP_SYN_SIZE]    :    FP packed in bytes
 * const FP_ELEM in[N-K]                      :    FP Vec input, to be packed
 *
 * This function handles the packing of FP
 */
void pack_fp_syn(uint8_t out[DENSELY_PACKED_FP_SYN_SIZE],
                 const FP_ELEM in[N - K]) {
	generic_pack_fp(out, in, DENSELY_PACKED_FP_SYN_SIZE, N - K);
}

/*
 * pack_fz_vec()
 *
 * uint8_t out[DENSELY_PACKED_FZ_VEC_SIZE]    :    FP packed in bytes
 * const FZ_ELEM in[N]                        :    FP Vec input, to be packed
 *
 * This function handles the packing of FP
 */
void pack_fz_vec(uint8_t out[DENSELY_PACKED_FZ_VEC_SIZE],
                 const FZ_ELEM in[N]) {
	generic_pack_fz(out, in, DENSELY_PACKED_FZ_VEC_SIZE, N);
}

/*
 * pack_fz_rsdp_g_vec()
 *
 * uint8_t out[DENSELY_PACKED_FZ_RSDP_G_VEC_SIZE]    :    FZ packed in bytes
 * const FZ_ELEM in[M]                               :    FZ Vec input, to be packed
 *
 * This function handles the packing of the add. rdsp(g) vector in FZ
 */

/*
 * unpack_fp_vec()
 *
 * FP_ELEM out[N]                               :    FP Vec output
 * const uint8_t in[DENSELY_PACKED_FP_VEC_SIZE] :    FP Byte input, to be unpckd
 *
 * This function handles the unpacking of FP
 */
uint8_t unpack_fp_vec(FP_ELEM out[N],
                      const uint8_t in[DENSELY_PACKED_FP_VEC_SIZE]) {
	return generic_unpack_fp(out, in, N, DENSELY_PACKED_FP_VEC_SIZE);
}

/*
 * unpack_fp_syn()
 *
 * FP_ELEM out[N]                               :    FP Vec output
 * const uint8_t in[DENSELY_PACKED_FP_SYN_SIZE] :    FP Byte input, to be unpckd
 *
 * This function handles the unpacking of FP
 */
uint8_t unpack_fp_syn(FP_ELEM out[N - K],
                      const uint8_t in[DENSELY_PACKED_FP_SYN_SIZE]) {
	return generic_unpack_fp(out, in, N - K, DENSELY_PACKED_FP_SYN_SIZE);
}

/*
 * unpack_fz_vec()
 *
 * FZ_ELEM out[N]                               :    FP Vec output
 * const uint8_t in[DENSELY_PACKED_FZ_VEC_SIZE] :    FP Byte input, to be unpckd
 *
 * This function handles the unpacking of FP
 */
uint8_t unpack_fz_vec(FZ_ELEM out[N],
                      const uint8_t in[DENSELY_PACKED_FZ_VEC_SIZE]) {
	return generic_unpack_fz(out, in, N, DENSELY_PACKED_FZ_VEC_SIZE);
}

/*
 * unpack_fz_rsdp_g_vec()
 *
 * FZ_ELEM out[M]                               :    FZ Vec output
 * const uint8_t in[DENSELY_PACKED_FZ_VEC_SIZE] :    FZ Byte input, to be unpckd
 *
 * This function handles the unpacking of FP
 */
