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

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "namespace.h"
#include "parameters.h"

/* compact Z_z/F_q vector encoding functions */

void pack_fp_vec(uint8_t out[DENSELY_PACKED_FP_VEC_SIZE],
                 const FP_ELEM in[N]);

void pack_fp_syn(uint8_t out[DENSELY_PACKED_FP_SYN_SIZE],
                 const FP_ELEM in[N - K]);

void pack_fz_vec(uint8_t out[DENSELY_PACKED_FZ_VEC_SIZE],
                 const FZ_ELEM in[N]);


uint8_t unpack_fp_vec(FP_ELEM out[N],
                      const uint8_t in[DENSELY_PACKED_FP_VEC_SIZE]);

uint8_t unpack_fp_syn(FP_ELEM out[N - K],
                      const uint8_t in[DENSELY_PACKED_FP_SYN_SIZE]);

uint8_t unpack_fz_vec(FZ_ELEM out[N],
                      const uint8_t in[DENSELY_PACKED_FZ_VEC_SIZE]);

