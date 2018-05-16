#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include "poly_mul.h"

int32_t glob_i=0;

void print_poly2(int16_t *a, int64_t n, uint64_t p){

	printf("-----------------------\n");
	int i;
	for (i = n - 1; i >= 0; i--){
		if (a[i] != 0){
			if(i!=0)
				printf("  Mod(%d,%lu)*x^%d + ", a[i], p,i);
			else
				printf("  Mod(%d,%lu)*x^%d ", a[i], p,i);

			}
	}

	printf("\n-----------------------\n");
}

void pol_mul(uint16_t* a, uint16_t* b, uint16_t* res, uint16_t p, uint32_t n,uint32_t start)

{ 
	// Polynomial multiplication using the schoolbook method, c[x] = a[x]*b[x] 
	// SECURITY NOTE: TO BE USED FOR TESTING ONLY.  

	uint32_t i;
	uint32_t j,  mask = 2*n - 1;


//-------------------normal multiplication-----------------

	
	
	uint16_t *c;
	c=(uint16_t *)malloc((mask+1)*sizeof(uint16_t));
	for (i = 0; i < mask+1; i++) c[i] = 0;

	for (i = start; i < start+n; i++) {
		for (j = start; j < start+n; j++) {
			c[i+j-2*start]=( c[i+j-2*start] + (a[i] * b[j]) )&(p-1);
		}
	}

	//---------------reduction-------
	for(i=n;i<2*n-1;i++){
		c[i-n]=(c[i-n]-c[i])&(p-1);
	}
	
	c[n]=0; //256th coefficient=0;
//----------------------copy result back----------------

	for(i=0; i<n; i++){
            res[i] = c[i];
	}
		

	free(c);

}

