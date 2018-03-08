#ifndef _PERMUTATION_H
#define _PERMUTATION_H

#include <stdint.h>

/* We assume all index will fit in 2 bytes */
typedef uint16_t index_t;

void randindex_init(int rand_size);

index_t uniform(index_t module);

void change_permutation (index_t * permutation,
						 index_t start,
						 index_t size_permutation);
void generate_permutation (index_t * permutation,
						   index_t size_permutation);

#endif /*_PERMUTATION_H*/
