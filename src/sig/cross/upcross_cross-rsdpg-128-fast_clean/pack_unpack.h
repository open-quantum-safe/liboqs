/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 1.1 (March 2023)
 *
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author Jonas Schupp <jonas.schupp@tum.de>
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

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "parameters.h"

/* compact Z_z/F_q vector encoding functions */

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_pack_fq_vec(uint8_t out[DENSELY_PACKED_FQ_VEC_SIZE],
        const FQ_ELEM in[N]);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_pack_fq_syn(uint8_t out[DENSELY_PACKED_FQ_SYN_SIZE],
        const FQ_ELEM in[N - K]);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_pack_fz_vec(uint8_t out[DENSELY_PACKED_FZ_VEC_SIZE],
        const FZ_ELEM in[N]);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_pack_fz_rsdp_g_vec(uint8_t out[DENSELY_PACKED_FZ_RSDP_G_VEC_SIZE],
        const FZ_ELEM in[M]);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_generic_pack_fq(uint8_t *out, const FQ_ELEM *in,
        size_t outlen, size_t inlen);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_generic_pack_fz(uint8_t *out, const FZ_ELEM *in,
        size_t outlen, size_t inlen);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_unpack_fq_vec(FQ_ELEM out[N],
        const uint8_t in[DENSELY_PACKED_FQ_VEC_SIZE]);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_unpack_fq_syn(FQ_ELEM out[N - K],
        const uint8_t in[DENSELY_PACKED_FQ_SYN_SIZE]);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_unpack_fz_vec(FZ_ELEM out[N],
        const uint8_t in[DENSELY_PACKED_FZ_VEC_SIZE]);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_unpack_fz_rsdp_g_vec(FZ_ELEM out[M],
        const uint8_t in[DENSELY_PACKED_FZ_RSDP_G_VEC_SIZE]);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_generic_unpack_fq(FQ_ELEM *out, const uint8_t *in,
        size_t outlen, size_t inlen);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_generic_unpack_fz(FZ_ELEM *out, const uint8_t *in,
        size_t outlen);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_generic_uint16_t_unpack(uint16_t *out, const uint8_t *in,
        size_t outlen, size_t inlen, uint8_t btr);

void PQCLEAN_CROSSRSDPG128FAST_CLEAN_generic_uint16_t_pack(uint8_t *out, const uint16_t *in,
        size_t outlen, size_t inlen, uint8_t btr);
