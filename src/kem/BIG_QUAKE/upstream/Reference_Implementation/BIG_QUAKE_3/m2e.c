#include <stdio.h>
#include "m2e.h"

/* Keccack function */

void FIPS202_SHA3_256(const unsigned char *input, unsigned int inputByteLen, unsigned char *output);




static unsigned char * buff = NULL;
static int buff_size = 0;

void swap_m2e(int * permutation, int a, int b) {
    int tmp = permutation[a];
    permutation[a] = permutation[b];
    permutation[b] = tmp;
}


void init_hash(IN unsigned char *m) {
    buff_size = HASH_SIZE;
    buff = realloc(buff, buff_size*sizeof(unsigned char));
    FIPS202_SHA3_256(m, RANDOM_BYTES, buff);
}

/*

 */
unsigned char * hash_trunc(int s) {
    
	unsigned char* aux = malloc(HASH_SIZE*sizeof(unsigned char));
	buff_size -= s;
    
	if (buff_size < 0) {
		memcpy(aux, buff, HASH_SIZE);
		init_hash(aux);
		buff_size -= s;

	
		return realloc(aux, s*sizeof(unsigned char));
	}
    
	memcpy(aux, buff + buff_size - 1, s*sizeof(unsigned char));
	return aux;
}


static int ucharToInt(unsigned char * c, int s) {
	int i, res = 0;
    
	for(i=0; i < s; i++){
		res <<= 8;
		res ^= (c[i] & 0xff);
	}
    
	return res;
}


/*
 */
int uniform_m2e(int s, int module) {
	int res;
	unsigned char * aux;
	while(1) {
		aux = hash_trunc(s);
		res = ucharToInt(aux, s);
        
		if (res > ((1 << (s*8)) - ((1 << (s*8)) % module))) {
	    free(aux);
			continue;
		}
		free(aux);
		return res % module;
	}
}


/* generate a constant weight word into e from a random vector m of (F_2)^s*/
int m2error(IN unsigned char *m, OUT int * error) {
    
    int i, j, s = 3; //$Elise : À modifier???
    int permutation[LENGTH];
    unsigned char * aux = malloc(HASH_SIZE*sizeof(unsigned char));
    
    for (i = 0; i < LENGTH; ++i) {
        permutation[i] = i;
    }
    
    init_hash(m);
    /*
    for (int i = 0; i<buff_size; i++)
	    printf("%u", buff[i]);
    */
    for(i = 0; i < NB_ERRORS; i++) {
        j = uniform_m2e(s, LENGTH - i-1);
        //printf("j = %d\n", j);
        swap_m2e(permutation, i, i + j);
        //printf("permutation[%d] = %d\n", i, permutation[i]);
        
        memcpy(aux, buff, HASH_SIZE);
        init_hash(aux);   
    }
    for (i = 0; i < NB_ERRORS; ++i) {
        error[i] = permutation[i];
    }
    
    free(aux);
    //free(buff);
    
    return SUCCESS;
}


