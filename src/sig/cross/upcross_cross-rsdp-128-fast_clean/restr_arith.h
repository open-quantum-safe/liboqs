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

#include "parameters.h"

#define FZRED_SINGLE(x)   (((x) & 0x07) + ((x) >> 3))
#define FZRED_OPPOSITE(x) ((x) ^ 0x07)
#define FZ_DOUBLE_ZERO_NORM(x) (((x) + (((x) + 1) >> 3)) & 0x07)



static inline
void fz_dz_norm_n(FZ_ELEM v[N]) {
	for (int i = 0; i < N; i++) {
		v[i] = FZ_DOUBLE_ZERO_NORM(v[i]);
	}
}

/* Elements of the restricted subgroups are represented as the exponents of
 * the generator */
static inline
void fz_vec_sub_n(FZ_ELEM res[N],
                  const FZ_ELEM a[N],
                  const FZ_ELEM b[N]) {
	for (int i = 0; i < N; i++) {
		res[i] = FZRED_SINGLE( a[i] + FZRED_OPPOSITE(b[i]) );
	}
}

static inline
int is_fz_vec_in_restr_group_n(const FZ_ELEM in[N]) {
	int is_in_ok = 1;
	for (int i = 0; i < N; i++) {
		is_in_ok = is_in_ok && (in[i] < Z);
	}
	return is_in_ok;
}

