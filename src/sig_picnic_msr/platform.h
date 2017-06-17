/*! @file platform.h
 *  @brief Platform-specific defines.   
 *
 *  This file is part of the reference implementation of the Picnic and Fish
 *  signature schemes, described in the paper:
 *
 *  Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives
 *  Melissa Chase and David Derler and Steven Goldfeder and Claudio Orlandi and
 *  Sebastian Ramacher and Christian Rechberger and Daniel Slamanig and Greg
 *  Zaverucha
 *  Cryptology ePrint Archive: Report 2017/279
 *  http://eprint.iacr.org/2017/279
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *
 */

/* __FUNCTION__ generates a warning on Linux with -Wpedantic and newer versions
 * of GCC (tested with 5.4).  So we use __func__ in all source and define it on
 * Windows. 
 */
#if defined(__WINDOWS__)
#define __func__ __FUNCTION__
#endif
