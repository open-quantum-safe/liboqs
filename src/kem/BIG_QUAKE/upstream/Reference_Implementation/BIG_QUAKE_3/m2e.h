#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "sizes.h"
#include "api.h"

typedef uint16_t index_t;

void swap_m2e(int * permutation, int a, int b);
void init_hash(IN unsigned char *m);
unsigned char * hash_trunc(int s); 
int uniform_m2e(int s, int module);
int m2error(IN unsigned char *m, OUT int * error);




