/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 1.1 (March 2023)
 *
 * @author Jonas Schupp <Jonas.Schupp@tum.de>
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

/*Implementation of packing of 1 to 16 bits value vectors in 8 bit vectors,
 * generic inputs from 1 to 16 bit possible*/

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_pack_fq_vec()
 *
 * uint8_t out[DENSELY_PACKED_FQ_VEC_SIZE]    : FQ packed in bytes
 * const FQ_ELEM in[N]                        :   FQ Vec input, to be packed
 *
 * This function handles the packing of FQ
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_pack_fq_vec(uint8_t out[DENSELY_PACKED_FQ_VEC_SIZE],
        const FQ_ELEM in[N]) {
	PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_pack_fq(out, in, DENSELY_PACKED_FQ_VEC_SIZE, N);
}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_pack_fq_syn()
 *
 * uint8_t out[DENSELY_PACKED_FQ_SYN_SIZE]    :   FQ packed in bytes
 * const FQ_ELEM in[N-K]                      :   FQ Vec input, to be packed
 *
 * This function handles the packing of FQ
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_pack_fq_syn(uint8_t out[DENSELY_PACKED_FQ_SYN_SIZE],
        const FQ_ELEM in[N - K]) {
	PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_pack_fq(out, in, DENSELY_PACKED_FQ_SYN_SIZE, N - K);
}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_pack_fz_vec()
 *
 * uint8_t out[DENSELY_PACKED_FZ_VEC_SIZE]    :   FQ packed in bytes
 * const FZ_ELEM in[N]                        :   FQ Vec input, to be packed
 *
 * This function handles the packing of FQ
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_pack_fz_vec(uint8_t out[DENSELY_PACKED_FZ_VEC_SIZE],
        const FZ_ELEM in[N]) {
	PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_pack_fz(out, in, DENSELY_PACKED_FZ_VEC_SIZE, N);
}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_pack_fz_rsdp_g_vec()
 *
 * uint8_t out[DENSELY_PACKED_FZ_RSDP_G_VEC_SIZE]    :   Zz packed in bytes
 * const FZ_ELEM in[M]                               :   Zz Vec input, to be packed
 *
 * This function handles the packing of the add. rdsp(g) vector in Zz
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_pack_fz_rsdp_g_vec(uint8_t out[DENSELY_PACKED_FZ_RSDP_G_VEC_SIZE],
        const FZ_ELEM in[M]) {
	PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_pack_fz(out, in, DENSELY_PACKED_FZ_RSDP_G_VEC_SIZE, M);
}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_pack_fq()
 *
 * uint8_t *out      :   FQ packed in bytes
 * const FQ_ELEM *in :   FQ Vec input, to be packed
 * size_t outlen     :   Length of out
 * size_t in         :   Length of in
 *
 * This function handles the packing of an vector of el. in FQ of arbit. length
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_pack_fq(uint8_t *out, const FQ_ELEM *in,
        const size_t outlen, const size_t inlen) {
	size_t i;
	for (i = 0; i < outlen; i++) {
		out[i] = 0;
	}
	for (i = 0; i < inlen / 8; i++) {
		out[i * 9] = (in[i * 8] >> 1);
		out[i * 9 + 1] |= (in[i * 8] << 7) | (in[i * 8 + 1] >> 2);
		out[i * 9 + 2] |= (in[i * 8 + 1] << 6) | (in[i * 8 + 2] >> 3);
		out[i * 9 + 3] |= (in[i * 8 + 2] << 5) | (in[i * 8 + 3] >> 4);
		out[i * 9 + 4] |= (in[i * 8 + 3] << 4) | (in[i * 8 + 4] >> 5);
		out[i * 9 + 5] |= (in[i * 8 + 4] << 3) | (in[i * 8 + 5] >> 6);
		out[i * 9 + 6] |= (in[i * 8 + 5] << 2) | (in[i * 8 + 6] >> 7);
		out[i * 9 + 7] |= (in[i * 8 + 6] << 1) | (in[i * 8 + 7] >> 8);
		out[i * 9 + 8] |= (in[i * 8 + 7]);
	}
	const uint8_t n_remainder = inlen % 8;
	if (n_remainder == 1) {
		out[i * 9] = (in[i * 8] >> 1);
		out[i * 9 + 1] = (in[i * 8] << 7);
	} else if (n_remainder == 2) {
		out[i * 9] = (in[i * 8] >> 1);
		out[i * 9 + 1] |= (in[i * 8] << 7) | (in[i * 8 + 1] >> 2);
		out[i * 9 + 2] = (in[i * 8 + 1] << 6);
	} else if (n_remainder == 3) {
		out[i * 9] = (in[i * 8] >> 1);
		out[i * 9 + 1] |= (in[i * 8] << 7) | (in[i * 8 + 1] >> 2);
		out[i * 9 + 2] |= (in[i * 8 + 1] << 6) | (in[i * 8 + 2] >> 3);
		out[i * 9 + 3] = (in[i * 8 + 2] << 5);
	} else if (n_remainder == 4) {
		out[i * 9] = (in[i * 8] >> 1);
		out[i * 9 + 1] |= (in[i * 8] << 7) | (in[i * 8 + 1] >> 2);
		out[i * 9 + 2] |= (in[i * 8 + 1] << 6) | (in[i * 8 + 2] >> 3);
		out[i * 9 + 3] |= (in[i * 8 + 2] << 5) | (in[i * 8 + 3] >> 4);
		out[i * 9 + 4] = (in[i * 8 + 3] << 4);
	} else if (n_remainder == 5) {
		out[i * 9] = (in[i * 8] >> 1);
		out[i * 9 + 1] |= (in[i * 8] << 7) | (in[i * 8 + 1] >> 2);
		out[i * 9 + 2] |= (in[i * 8 + 1] << 6) | (in[i * 8 + 2] >> 3);
		out[i * 9 + 3] |= (in[i * 8 + 2] << 5) | (in[i * 8 + 3] >> 4);
		out[i * 9 + 4] |= (in[i * 8 + 3] << 4) | (in[i * 8 + 4] >> 5);
		out[i * 9 + 5] = (in[i * 8 + 4] << 3);
	} else if (n_remainder == 6) {
		out[i * 9] = (in[i * 8] >> 1);
		out[i * 9 + 1] |= (in[i * 8] << 7) | (in[i * 8 + 1] >> 2);
		out[i * 9 + 2] |= (in[i * 8 + 1] << 6) | (in[i * 8 + 2] >> 3);
		out[i * 9 + 3] |= (in[i * 8 + 2] << 5) | (in[i * 8 + 3] >> 4);
		out[i * 9 + 4] |= (in[i * 8 + 3] << 4) | (in[i * 8 + 4] >> 5);
		out[i * 9 + 5] |= (in[i * 8 + 4] << 3) | (in[i * 8 + 5] >> 6);
		out[i * 9 + 6] = (in[i * 8 + 5] << 2);
	} else if (n_remainder == 7) {
		out[i * 9] = (in[i * 8] >> 1);
		out[i * 9 + 1] |= (in[i * 8] << 7) | (in[i * 8 + 1] >> 2);
		out[i * 9 + 2] |= (in[i * 8 + 1] << 6) | (in[i * 8 + 2] >> 3);
		out[i * 9 + 3] |= (in[i * 8 + 2] << 5) | (in[i * 8 + 3] >> 4);
		out[i * 9 + 4] |= (in[i * 8 + 3] << 4) | (in[i * 8 + 4] >> 5);
		out[i * 9 + 5] |= (in[i * 8 + 4] << 3) | (in[i * 8 + 5] >> 6);
		out[i * 9 + 6] |= (in[i * 8 + 5] << 2) | (in[i * 8 + 6] >> 7);
		out[i * 9 + 7] = (in[i * 8 + 6] << 1);
	}

}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_pack_fz()
 *
 * uint8_t *out      :   Zz packed in bytes
 * const FZ_ELEM *in :   Zz Vec input, to be packed
 * size_t outlen     :   Length of out
 * size_t in         :   Length of in
 *
 * This function handles the packing of an vector of el. in Zz of arbit. length
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_pack_fz(uint8_t *out, const FZ_ELEM *in, const size_t outlen, const size_t inlen) {
	size_t i;
	for (i = 0; i < outlen; i++) {
		out[i] = 0;
	}
	for (i = 0; i < inlen / 8; i++) {
		out[i * 7] |= (in[i * 8] << 1) | (in[i * 8 + 1] >> 6);
		out[i * 7 + 1] |= (in[i * 8 + 1] << 2) | (in[i * 8 + 2] >> 5);
		out[i * 7 + 2] |= (in[i * 8 + 2] << 3) | (in[i * 8 + 3] >> 4);
		out[i * 7 + 3] |= (in[i * 8 + 3] << 4) | (in[i * 8 + 4] >> 3);
		out[i * 7 + 4] |= (in[i * 8 + 4] << 5) | (in[i * 8 + 5] >> 2);
		out[i * 7 + 5] |= (in[i * 8 + 5] << 6) | (in[i * 8 + 6] >> 1);
		out[i * 7 + 6] |= (in[i * 8 + 6] << 7) | (in[i * 8 + 7]);
	}
	const uint8_t n_remainder = inlen % 8;
	if (n_remainder == 1) {
		out[i * 7] = (in[i * 8] << 1);
	} else if (n_remainder == 2) {
		out[i * 7] |= (in[i * 8] << 1) | (in[i * 8 + 1] >> 6);
		out[i * 7 + 1] = (in[i * 8 + 1] << 2);
	} else if (n_remainder == 3) {
		out[i * 7] |= (in[i * 8] << 1) | (in[i * 8 + 1] >> 6);
		out[i * 7 + 1] |= (in[i * 8 + 1] << 2) | (in[i * 8 + 2] >> 5);
		out[i * 7 + 2] = (in[i * 8 + 2] << 3);
	} else if (n_remainder == 4) {
		out[i * 7] |= (in[i * 8] << 1) | (in[i * 8 + 1] >> 6);
		out[i * 7 + 1] |= (in[i * 8 + 1] << 2) | (in[i * 8 + 2] >> 5);
		out[i * 7 + 2] |= (in[i * 8 + 2] << 3) | (in[i * 8 + 3] >> 4);
		out[i * 7 + 3] |= (in[i * 8 + 3] << 4);
	} else if (n_remainder == 5) {
		out[i * 7] |= (in[i * 8] << 1) | (in[i * 8 + 1] >> 6);
		out[i * 7 + 1] |= (in[i * 8 + 1] << 2) | (in[i * 8 + 2] >> 5);
		out[i * 7 + 2] |= (in[i * 8 + 2] << 3) | (in[i * 8 + 3] >> 4);
		out[i * 7 + 3] |= (in[i * 8 + 3] << 4) | (in[i * 8 + 4] >> 3);
		out[i * 7 + 4] = (in[i * 8 + 4] << 5);
	} else if (n_remainder == 6) {
		out[i * 7] |= (in[i * 8] << 1) | (in[i * 8 + 1] >> 6);
		out[i * 7 + 1] |= (in[i * 8 + 1] << 2) | (in[i * 8 + 2] >> 5);
		out[i * 7 + 2] |= (in[i * 8 + 2] << 3) | (in[i * 8 + 3] >> 4);
		out[i * 7 + 3] |= (in[i * 8 + 3] << 4) | (in[i * 8 + 4] >> 3);
		out[i * 7 + 4] |= (in[i * 8 + 4] << 5) | (in[i * 8 + 5] >> 2);
		out[i * 7 + 5] = (in[i * 8 + 5] << 6);
	} else if (n_remainder == 7) {
		out[i * 7] |= (in[i * 8] << 1) | (in[i * 8 + 1] >> 6);
		out[i * 7 + 1] |= (in[i * 8 + 1] << 2) | (in[i * 8 + 2] >> 5);
		out[i * 7 + 2] |= (in[i * 8 + 2] << 3) | (in[i * 8 + 3] >> 4);
		out[i * 7 + 3] |= (in[i * 8 + 3] << 4) | (in[i * 8 + 4] >> 3);
		out[i * 7 + 4] |= (in[i * 8 + 4] << 5) | (in[i * 8 + 5] >> 2);
		out[i * 7 + 5] |= (in[i * 8 + 5] << 6) | (in[i * 8 + 6] >> 1);
		out[i * 7 + 6] |= (in[i * 8 + 6] << 7);
	}

}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_unpack_fq_vec()
 *
 * FQ_ELEM out[N]                               :   FQ Vec output
 * const uint8_t in[DENSELY_PACKED_FQ_VEC_SIZE] :   FQ Byte input, to be unpckd
 *
 * This function handles the unpacking of FQ
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_unpack_fq_vec(FQ_ELEM out[N],
        const uint8_t in[DENSELY_PACKED_FQ_VEC_SIZE]) {
	PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_unpack_fq(out, in, N, DENSELY_PACKED_FQ_VEC_SIZE);
}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_unpack_fq_syn()
 *
 * FQ_ELEM out[N]                               :   FQ Vec output
 * const uint8_t in[DENSELY_PACKED_FQ_SYN_SIZE] :   FQ Byte input, to be unpckd
 *
 * This function handles the unpacking of FQ
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_unpack_fq_syn(FQ_ELEM out[N - K],
        const uint8_t in[DENSELY_PACKED_FQ_SYN_SIZE]) {
	PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_unpack_fq(out, in, N - K, DENSELY_PACKED_FQ_SYN_SIZE);
}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_unpack_fz_vec()
 *
 * FZ_ELEM out[N]                               :   FQ Vec output
 * const uint8_t in[DENSELY_PACKED_FZ_VEC_SIZE] :   FQ Byte input, to be unpckd
 *
 * This function handles the unpacking of FQ
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_unpack_fz_vec(FZ_ELEM out[N],
        const uint8_t in[DENSELY_PACKED_FZ_VEC_SIZE]) {
	PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_unpack_fz(out, in, N);
}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_unpack_fz_rsdp_g_vec()
 *
 * FZ_ELEM out[M]                               :   FZ Vec output
 * const uint8_t in[DENSELY_PACKED_FZ_VEC_SIZE] :   FZ Byte input, to be unpckd
 *
 * This function handles the unpacking of FQ
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_unpack_fz_rsdp_g_vec(FZ_ELEM out[M],
        const uint8_t in[DENSELY_PACKED_FZ_RSDP_G_VEC_SIZE]) {
	PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_unpack_fz(out, in, M);
}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_unpack_fq()
 *
 * FQ_ELEM *out      :   FQ output, unpacked
 * const uint8_t *in :   FQ Vec input, packed in bytes
 * size_t outlen     :   Length of out
 * size_t in         :   Length of in
 *
 * This function unpacks an vector of el. in FQ of arbit. length
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_unpack_fq(FQ_ELEM *out, const uint8_t *in,
        size_t outlen, size_t inlen) {

	size_t i;
	for (i = 0; i < outlen; i++) {
		out[i] = 0;
	}
	for (i = 0; i < inlen / 9; i++) {
		out[i * 8]    = (in[i * 9] << 1);
		out[i * 8]   |= (in[i * 9 + 1] >> 7);
		out[i * 8 + 1]  = ((in[i * 9 + 1] << 2) & 0x1FF);
		out[i * 8 + 1] |= (in[i * 9 + 2] >> 6);
		out[i * 8 + 2]  = ((in[i * 9 + 2] << 3) & 0x1FF);
		out[i * 8 + 2] |= (in[i * 9 + 3] >> 5);
		out[i * 8 + 3]  = ((in[i * 9 + 3] << 4) & 0x1FF);
		out[i * 8 + 3] |= (in[i * 9 + 4] >> 4);
		out[i * 8 + 4]  = ((in[i * 9 + 4] << 5) & 0x1FF);
		out[i * 8 + 4] |= (in[i * 9 + 5] >> 3);
		out[i * 8 + 5]  = ((in[i * 9 + 5] << 6) & 0x1FF);
		out[i * 8 + 5] |= (in[i * 9 + 6] >> 2);
		out[i * 8 + 6]  = ((in[i * 9 + 6] << 7) & 0x1FF);
		out[i * 8 + 6] |= (in[i * 9 + 7] >> 1);
		out[i * 8 + 7]  = ((in[i * 9 + 7] << 8) & 0x1FF);
		out[i * 8 + 7] |= (in[i * 9 + 8]);
	}
	const uint8_t n_remainder = outlen % 8;
	if (n_remainder == 1) {
		out[i * 8]    = (in[i * 9] << 1);
		out[i * 8]   |= (in[i * 9 + 1] >> 7);
	}
	if (n_remainder == 2) {
		out[i * 8]    = (in[i * 9] << 1);
		out[i * 8]   |= (in[i * 9 + 1] >> 7);
		out[i * 8 + 1]  = ((in[i * 9 + 1] << 2) & 0x1FF);
		out[i * 8 + 1] |= (in[i * 9 + 2] >> 6);
	}
	if (n_remainder == 3) {
		out[i * 8]    = (in[i * 9] << 1);
		out[i * 8]   |= (in[i * 9 + 1] >> 7);
		out[i * 8 + 1]  = ((in[i * 9 + 1] << 2) & 0x1FF);
		out[i * 8 + 1] |= (in[i * 9 + 2] >> 6);
		out[i * 8 + 2]  = ((in[i * 9 + 2] << 3) & 0x1FF);
		out[i * 8 + 2] |= (in[i * 9 + 3] >> 5);
	}
	if (n_remainder == 4) {
		out[i * 8]    = (in[i * 9] << 1);
		out[i * 8]   |= (in[i * 9 + 1] >> 7);
		out[i * 8 + 1]  = ((in[i * 9 + 1] << 2) & 0x1FF);
		out[i * 8 + 1] |= (in[i * 9 + 2] >> 6);
		out[i * 8 + 2]  = ((in[i * 9 + 2] << 3) & 0x1FF);
		out[i * 8 + 2] |= (in[i * 9 + 3] >> 5);
		out[i * 8 + 3]  = ((in[i * 9 + 3] << 4) & 0x1FF);
		out[i * 8 + 3] |= (in[i * 9 + 4] >> 4);
	}
	if (n_remainder == 5) {
		out[i * 8]    = (in[i * 9] << 1);
		out[i * 8]   |= (in[i * 9 + 1] >> 7);
		out[i * 8 + 1]  = ((in[i * 9 + 1] << 2) & 0x1FF);
		out[i * 8 + 1] |= (in[i * 9 + 2] >> 6);
		out[i * 8 + 2]  = ((in[i * 9 + 2] << 3) & 0x1FF);
		out[i * 8 + 2] |= (in[i * 9 + 3] >> 5);
		out[i * 8 + 3]  = ((in[i * 9 + 3] << 4) & 0x1FF);
		out[i * 8 + 3] |= (in[i * 9 + 4] >> 4);
		out[i * 8 + 4]  = ((in[i * 9 + 4] << 5) & 0x1FF);
		out[i * 8 + 4] |= (in[i * 9 + 5] >> 3);
	}
	if (n_remainder == 6) {
		out[i * 8]    = (in[i * 9] << 1);
		out[i * 8]   |= (in[i * 9 + 1] >> 7);
		out[i * 8 + 1]  = ((in[i * 9 + 1] << 2) & 0x1FF);
		out[i * 8 + 1] |= (in[i * 9 + 2] >> 6);
		out[i * 8 + 2]  = ((in[i * 9 + 2] << 3) & 0x1FF);
		out[i * 8 + 2] |= (in[i * 9 + 3] >> 5);
		out[i * 8 + 3]  = ((in[i * 9 + 3] << 4) & 0x1FF);
		out[i * 8 + 3] |= (in[i * 9 + 4] >> 4);
		out[i * 8 + 4]  = ((in[i * 9 + 4] << 5) & 0x1FF);
		out[i * 8 + 4] |= (in[i * 9 + 5] >> 3);
		out[i * 8 + 5]  = ((in[i * 9 + 5] << 6) & 0x1FF);
		out[i * 8 + 5] |= (in[i * 9 + 6] >> 2);
	}
	if (n_remainder == 7) {
		out[i * 8]    = (in[i * 9] << 1);
		out[i * 8]   |= (in[i * 9 + 1] >> 7);
		out[i * 8 + 1]  = ((in[i * 9 + 1] << 2) & 0x1FF);
		out[i * 8 + 1] |= (in[i * 9 + 2] >> 6);
		out[i * 8 + 2]  = ((in[i * 9 + 2] << 3) & 0x1FF);
		out[i * 8 + 2] |= (in[i * 9 + 3] >> 5);
		out[i * 8 + 3]  = ((in[i * 9 + 3] << 4) & 0x1FF);
		out[i * 8 + 3] |= (in[i * 9 + 4] >> 4);
		out[i * 8 + 4]  = ((in[i * 9 + 4] << 5) & 0x1FF);
		out[i * 8 + 4] |= (in[i * 9 + 5] >> 3);
		out[i * 8 + 5]  = ((in[i * 9 + 5] << 6) & 0x1FF);
		out[i * 8 + 5] |= (in[i * 9 + 6] >> 2);
		out[i * 8 + 6]  = ((in[i * 9 + 6] << 7) & 0x1FF);
		out[i * 8 + 6] |= (in[i * 9 + 7] >> 1);
	}

}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_unpack_fz()
 *
 * FZ_ELEM *out      :   Zz output, unpacked
 * const uint8_t *in :   Zz Vec input, packed in bytes
 * size_t outlen     :   Length of out
 *
 * This function unpacks an vector of el. in Zz of arbit. length
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_unpack_fz(FZ_ELEM *out, const uint8_t *in,
        size_t outlen) {

	size_t i;
	for (i = 0; i < outlen; i++) {
		out[i] = 0;
	}
	for (i = 0; i < outlen / 8; i++) {
		out[i * 8]    = (in[i * 7] >> 1);
		out[i * 8 + 1]  = ((in[i * 7] << 6) & 0x7F);
		out[i * 8 + 1] |= (in[i * 7 + 1] >> 2);
		out[i * 8 + 2]  = ((in[i * 7 + 1] << 5) & 0x7F);
		out[i * 8 + 2] |= (in[i * 7 + 2] >> 3);
		out[i * 8 + 3]  = ((in[i * 7 + 2] << 4) & 0x7F);
		out[i * 8 + 3] |= (in[i * 7 + 3] >> 4);
		out[i * 8 + 4]  = ((in[i * 7 + 3] << 3) & 0x7F);
		out[i * 8 + 4] |= (in[i * 7 + 4] >> 5);
		out[i * 8 + 5]  = ((in[i * 7 + 4] << 2) & 0x7F);
		out[i * 8 + 5] |= (in[i * 7 + 5] >> 6);
		out[i * 8 + 6]  = ((in[i * 7 + 5] << 1) & 0x7F);
		out[i * 8 + 6] |= (in[i * 7 + 6] >> 7);
		out[i * 8 + 7]  = (in[i * 7 + 6] & 0x7F);
	}
	const uint8_t n_remainder = outlen % 8;
	if (n_remainder == 1) {
		out[i * 8]    = (in[i * 7] >> 1);
	} else if (n_remainder == 2) {
		out[i * 8]    = (in[i * 7] >> 1);
		out[i * 8 + 1]  = ((in[i * 7] << 6) & 0x7F);
		out[i * 8 + 1] |= (in[i * 7 + 1] >> 2);
	} else if (n_remainder == 3) {
		out[i * 8]    = (in[i * 7] >> 1);
		out[i * 8 + 1]  = ((in[i * 7] << 6) & 0x7F);
		out[i * 8 + 1] |= (in[i * 7 + 1] >> 2);
		out[i * 8 + 2]  = ((in[i * 7 + 1] << 5) & 0x7F);
		out[i * 8 + 2] |= (in[i * 7 + 2] >> 3);
	} else if (n_remainder == 4) {
		out[i * 8]    = (in[i * 7] >> 1);
		out[i * 8 + 1]  = ((in[i * 7] << 6) & 0x7F);
		out[i * 8 + 1] |= (in[i * 7 + 1] >> 2);
		out[i * 8 + 2]  = ((in[i * 7 + 1] << 5) & 0x7F);
		out[i * 8 + 2] |= (in[i * 7 + 2] >> 3);
		out[i * 8 + 3]  = ((in[i * 7 + 2] << 4) & 0x7F);
		out[i * 8 + 3] |= (in[i * 7 + 3] >> 4);
	} else if (n_remainder == 5) {
		out[i * 8]    = (in[i * 7] >> 1);
		out[i * 8 + 1]  = ((in[i * 7] << 6) & 0x7F);
		out[i * 8 + 1] |= (in[i * 7 + 1] >> 2);
		out[i * 8 + 2]  = ((in[i * 7 + 1] << 5) & 0x7F);
		out[i * 8 + 2] |= (in[i * 7 + 2] >> 3);
		out[i * 8 + 3]  = ((in[i * 7 + 2] << 4) & 0x7F);
		out[i * 8 + 3] |= (in[i * 7 + 3] >> 4);
		out[i * 8 + 4]  = ((in[i * 7 + 3] << 3) & 0x7F);
		out[i * 8 + 4] |= (in[i * 7 + 4] >> 5);
	} else if (n_remainder == 6) {
		out[i * 8]    = (in[i * 7] >> 1);
		out[i * 8 + 1]  = ((in[i * 7] << 6) & 0x7F);
		out[i * 8 + 1] |= (in[i * 7 + 1] >> 2);
		out[i * 8 + 2]  = ((in[i * 7 + 1] << 5) & 0x7F);
		out[i * 8 + 2] |= (in[i * 7 + 2] >> 3);
		out[i * 8 + 3]  = ((in[i * 7 + 2] << 4) & 0x7F);
		out[i * 8 + 3] |= (in[i * 7 + 3] >> 4);
		out[i * 8 + 4]  = ((in[i * 7 + 3] << 3) & 0x7F);
		out[i * 8 + 4] |= (in[i * 7 + 4] >> 5);
		out[i * 8 + 5]  = ((in[i * 7 + 4] << 2) & 0x7F);
		out[i * 8 + 5] |= (in[i * 7 + 5] >> 6);
	} else if (n_remainder == 7) {
		out[i * 8]    = (in[i * 7] >> 1);
		out[i * 8 + 1]  = ((in[i * 7] << 6) & 0x7F);
		out[i * 8 + 1] |= (in[i * 7 + 1] >> 2);
		out[i * 8 + 2]  = ((in[i * 7 + 1] << 5) & 0x7F);
		out[i * 8 + 2] |= (in[i * 7 + 2] >> 3);
		out[i * 8 + 3]  = ((in[i * 7 + 2] << 4) & 0x7F);
		out[i * 8 + 3] |= (in[i * 7 + 3] >> 4);
		out[i * 8 + 4]  = ((in[i * 7 + 3] << 3) & 0x7F);
		out[i * 8 + 4] |= (in[i * 7 + 4] >> 5);
		out[i * 8 + 5]  = ((in[i * 7 + 4] << 2) & 0x7F);
		out[i * 8 + 5] |= (in[i * 7 + 5] >> 6);
		out[i * 8 + 6]  = ((in[i * 7 + 5] << 1) & 0x7F);
		out[i * 8 + 6] |= (in[i * 7 + 6] >> 7);
	}
}

void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_uint16_t_pack(uint8_t *out, const uint16_t *in,
        size_t outlen, size_t inlen, uint8_t btr) {
	size_t i;
	size_t in_i = 0;
	uint8_t left, right;
	uint8_t skip = 0;
	if (btr <= 8) {
		left = 8 - btr;
		right = 0;
		for (i = 0; i < outlen; i++) {
			out[i] = 0;
		}
		for (i = 0; i < outlen; i++) {
			while (right < 8 && in_i < inlen) {
				out[i] |= (in[in_i] << (left)) >> right;
				right += 8 - left;
				left = 8 - btr;
				in_i++;
			}
			if (right != 8) {
				in_i--;
				left = 2 * 8 - right;
			} else {
				left = 8 - btr;
			}
			right = 0;
		}
	} else if (btr == 8) {
		for (i = 0; i < outlen; i++) {
			out[i] = in[i];
		}
	} else if ((btr > 8) && (btr <= 16)) {
		left = 0;
		right = btr - 8;
		skip = 0;
		for (i = 0; i < outlen; i++) {
			out[i] = 0;
		}
		for (i = 0; i < outlen; i++) {
			skip = 0;
			while (skip == 0 && in_i < inlen) {
				out[i] |= (uint8_t)((in[in_i] << left) >> right);
				// First case: left == 0,
				// i.e.: the current value covers the LSBs of the packed value
				if (left == 0) {
					skip = 1;
					//Case 0: This value was complete,
					//i.e. a new one needs to be started
					if (right == 0) {
						in_i++;
						left = 0;
						right = btr - 8;
					}
					//Case 1: These were not the LSBs of this value,
					//i.e. its LSBs need to be stored in the next block
					else if (right <= 8) {
						left = 8 - right;
						right = 0;
					} else {
						right -= 8;
					}

				}
				// Second case: right == 0 and left > 0,
				// i.e.: the current value is complete its LSBs
				// are stored in the upper bits
				else if (right == 0 && left > 0) {

					right = btr - left;
					left = 0;
					in_i++;

				}
			}
		}
	}
	/* PQClean-edit: unused parameter */
	(void)skip;
}

void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generic_uint16_t_unpack(uint16_t *out, const uint8_t *in,
        size_t outlen, size_t inlen, uint8_t btr) {
	size_t i;
	uint8_t skip = 0;
	uint8_t right, left;
	size_t out_i;
	if (btr <= 8) {
		out_i = 0;
		right = 8 - btr;
		left = 0;
		uint8_t mask = (1 << (btr)) - 1;
		for (i = 0; i < outlen; i++) {
			out[i] = 0;
		}
		for (i = 0; i < inlen; i++) {
			skip = 0;
			while (skip == 0 && out_i < outlen) {
				out[out_i] |= (((in[i] >> (right)) << left) & mask);
				//Case 1: right > 0, i.e. the current word is done
				//and there is a new one waiting in the same input
				if (right > 0) {
					out_i++;
					if (right >= btr) {
						right -= btr;
					} else {
						left = btr - right;
						right = 0;
					}
				}
				//Case 2: right == 0 and left > BITS_TO_REPRESENT(Q-1),
				//i.e. the current word continues in the next input
				else if (right == 0) {
					skip = 1;
					right = 8 - left;
					left = 0;
					if (right == 0) {
						right = 8 - btr;
					}
				}
			}
		}
	} else if (btr == 8) {
		for (i = 0; i < outlen; i++) {
			out[i] = in[i];
		}
	} else if ((btr > 8) && (btr <= 16)) {
		left = btr - 8;
		right = 0;
		out_i = 0;
		skip = 0;
		uint16_t mask = (1 << (btr)) - 1;
		for (i = 0; i < outlen; i++) {
			out[i] = 0;
		}
		for (i = 0; i < inlen; i++) {
			skip = 0;
			while (skip == 0 && out_i < outlen) {
				// Shift some value of packed poly to correct position
				// and OR it to the target value
				out[out_i] |= ((((uint16_t)in[i] << left) >> right) & mask);
				// First case: the value was incomplete,
				// i.e. the LSBs are in the next block
				// This means that left > 0 and right == 0.
				if (left > 0 && right == 0) {
					// Right shift of next element for remaining bits
					if (left <= 8) {
						right = 8 - left;
						left = 0;
					} else {
						right = 0;
						left -= 8;
					}
					skip = 1;
				}
				// Second case: the value was complete, i.e. the LSBs are now stored
				else if (left == 0) {
					if (right == 0) {
						skip = 1;
						left = btr - 8;
					} else {
						left = btr - right;
					}
					right = 0;
					out_i++;
				}
				if (left == (btr)) {
					left = 1;
				}
			}
		}
	}
	/* PQClean-edit: unused parameter */
	(void)skip;
}
