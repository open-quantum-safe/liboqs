/* ****************************** *
 * Titanium_CCA_med               *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Modulo reduction               *
 * ****************************** */
 
#ifndef FASTMODULO_H
#define FASTMODULO_H

#include "param.h"
#include <stdint.h>

/* Montgomery reduction
 * Input: x < Q*R, where R=2^k and Q<R
 * Output: m = x*R^{-1} % Q
 * 
 * b = -Q^{-1} % R
 * t = ((x % R)*b) % R
 * m = (x + t * Q) / R */

#define MONTGOMERY_FACTOR 4010184703
#define MONTGOMERY_SHIFT 32

inline uint32_t montgomery(uint64_t t)
{
	uint32_t x, y;
	
	x = t;
	y = ((uint64_t)x) * MONTGOMERY_FACTOR;
	
	return (t + ((uint64_t)y) * Q) >> MONTGOMERY_SHIFT;
}

/* Input: x < 2^k
 * Output m = x % Q in [0, 2Q)
 * 
 * b = floor(2^k/Q)
 * t = floor((x * b) / 2^k), where t is an estimation of x / Q
 * m = x - t * Q */

#define BARRETT_BITSHIFT_SHORT 32
#define BARRETT_FACTOR_SHORT 9986

#define BARRETT_BITSHIFT_4Q2 40 
#define BARRETT_FACTOR_4Q2 2556522

#define BARRETT_BITSHIFT_8Q2 41 
#define BARRETT_FACTOR_8Q2 5113044

#define BARRETT_BITSHIFT_ZQ (ZQ_BYTES * 8) 
#define BARRETT_FACTOR_ZQ 39

inline uint32_t barrett_short(uint64_t t)
{
	return t - (((t * BARRETT_FACTOR_SHORT) >> BARRETT_BITSHIFT_SHORT) * Q);
}

inline uint32_t barrett_4q2(uint64_t t)
{
	return t - (((t * BARRETT_FACTOR_4Q2) >> BARRETT_BITSHIFT_4Q2) * Q);
}

inline uint32_t barrett_8q2(uint64_t t)
{
	return t - (((t * BARRETT_FACTOR_8Q2) >> BARRETT_BITSHIFT_8Q2) * Q);
}

#endif
