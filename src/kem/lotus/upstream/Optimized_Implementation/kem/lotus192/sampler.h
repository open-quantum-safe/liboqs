/**
 * @file sampler.h
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 */

#ifndef _SAMPLER_H
#define _SAMPLER_H

#include "type.h"

void sampler_init();
void sampler_set_seed(const U8 *seed);
void sample_uniform(U16 *ret_array, const U32 len);
void sample_discrete_gaussian(U16 *ret_array, const U32 len);

#endif
