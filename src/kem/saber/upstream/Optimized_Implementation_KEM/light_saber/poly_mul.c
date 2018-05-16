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

void toom_cook_4way(uint16_t* a1,uint16_t* b1, uint16_t* result, uint64_t p_mod, uint16_t n){

	int16_t i;
	int16_t small_len=SABER_N/4;
	
	
	int16_t result_final[2*SABER_N];
	uint64_t p_mod_or;	

	p_mod_or=p_mod;
	p_mod=p_mod*8;

//----------------array declaration to hold smaller arrays--------------------

	int16_t a[small_len],b[small_len];
	int16_t th_a[small_len],t_h_a[small_len];
	int16_t th_b[small_len],t_h_b[small_len];
	int16_t temp1[2*small_len];

//----------------array declaration to hold smaller arrays ends--------------------

//----------------array declaration to hold results--------------------
	int16_t w1[2*small_len],w2[2*small_len],w3[2*small_len],w4[2*small_len],w5[2*small_len],w6[2*small_len],w7[2*small_len];

//----------------array declaration to hold results ends--------------------
	//--------------------these data are created for place holding---------

	int16_t a1_ph[small_len],b1_ph[small_len];
	int16_t a2_ph[small_len],b2_ph[small_len];
	int16_t a3_ph[small_len],b3_ph[small_len];
	int16_t a4_ph[small_len],b4_ph[small_len];
	int16_t a5_ph[small_len],b5_ph[small_len];
	int16_t a6_ph[small_len],b6_ph[small_len];

	//--------------------these data are created for place holding ends---------

	int16_t inv3=43691;
	int16_t inv9=36409;
	int16_t inv15=61167;

	int16_t int45=45;
	int16_t int30=30;
	int16_t int0=0;


	//do the partial products
	
	//-------------------t0--------------------

	//create a(0)*b(0)

	for(i=0;i<small_len;i++){
		a1_ph[i]=a1[i+0];
		b1_ph[i]=b1[i+0];
	}

	//-------------------t0 ends------------------

	//-------------------th and t_h. th <-a(1/2)*b(1/2). t_h <- a(-1/2)*b(-1/2) ---------------------
	//create partial sum for th and t_h
	
	for(i=0;i<small_len;i++){//th_x contains 4*x[0]
		th_a[i]= a1_ph[i]<<2;
		th_b[i]= b1_ph[i]<<2;
	
	//th_x contains 4*x[0]+x[2]
		th_a[i]= th_a[i]+a1[small_len*2+i];
		th_b[i]= th_b[i]+b1[small_len*2+i];
	
	//th_x_avx contains 8*x[0]+2*x[2]
		th_a[i]= th_a[i]<<1;
		th_b[i]= th_b[i]<<1;
	
	//t_h_x_avx contains x[1]
		t_h_a[i]= a1[small_len*1+i];
		t_h_b[i]= b1[small_len*1+i];
	
	//t_h_x_avx contains 4*x[1]
		t_h_a[i]= t_h_a[i]<<2;
		t_h_b[i]= t_h_b[i]<<2;

	//th_x_avx contains 4*x[1]+x[3]
		t_h_a[i]= t_h_a[i]+a1[small_len*3+i];
		t_h_b[i]= t_h_b[i]+b1[small_len*3+i];
	}

	//create th

	for(i=0;i<small_len;i++){
		a2_ph[i]= th_a[i]+t_h_a[i];
		b2_ph[i]= th_b[i]+t_h_b[i];
	}

	//pol_mul_avx(a_avx, b_avx, w5_avx, p_mod, small_len, 0);	

	//create t_h

	for(i=0;i<small_len;i++){
		a3_ph[i]= th_a[i]-t_h_a[i];
		b3_ph[i]= th_b[i]-t_h_b[i];
	}

	//pol_mul_avx(a_avx, b_avx, w6_avx, p_mod, small_len, 0);

	//-------------------t1 and t_1. t1 <-a(1)*b(1). t_1 <- a(-1)*b(-1) ---------------------

	//create partial sum for t_1 and t1
	
	for(i=0;i<small_len;i++){//th_x_avx contains x[2]+x[0]
		th_a[i]= a1[small_len*2+i]+a1[small_len*0+i];
		th_b[i]= b1[small_len*2+i]+b1[small_len*0+i];
		
		//th_x_avx contains x[3]+x[1]
		t_h_a[i]= a1[small_len*3+i]+a1[small_len*1+i];
		t_h_b[i]= b1[small_len*3+i]+b1[small_len*1+i];
	}	
	

	//create t1
	for(i=0;i<small_len;i++){// x[0]+x[1]+x[2]+x[3]
		a4_ph[i]= th_a[i]+t_h_a[i];
		b4_ph[i]= th_b[i]+t_h_b[i];
	}

	//pol_mul_avx(a_avx, b_avx, w3_avx, p_mod, small_len, 0);	

	//create t_1
	for(i=0;i<small_len;i++){//-x[3]+x[2]-x[1]+x[0]
		a5_ph[i]= th_a[i]-t_h_a[i];
		b5_ph[i]= th_b[i]-t_h_b[i];
	}

	//pol_mul_avx(a_avx, b_avx, w4_avx, p_mod, small_len, 0);	


	//------------------t_inf------------------------------	
	//create t_inf

	for(i=0;i<small_len;i++){//x_avx contains x[3]
		a6_ph[i]= a1[small_len*3+i];
		b6_ph[i]= b1[small_len*3+i];
	}	

	//pol_mul_avx(a_avx, b_avx, w1_avx, p_mod, small_len, 0);

	//-------------------t_inf ends----------------------
	
	//-------------------t2-------------------------
	for(i=0;i<small_len;i++){// 2*x[3]
		a[i]= a6_ph[i]+a1[small_len*3+i];
		b[i]= b6_ph[i]+b1[small_len*3+i];

		// 2*x[3]+x[2]
		a[i]= a[i]+a1[small_len*2+i];
		b[i]= b[i]+b1[small_len*2+i];
		
		// 4*x[3]+2*x[2]
		a[i]= a[i]<<1;
		b[i]= b[i]<<1;
		
		// 4*x[3]+2*x[2]+x[1]
		a[i]= a[i]+a1[small_len*1+i];
		b[i]= b[i]+b1[small_len*1+i];
		
		// 8*x[3]+4*x[2]+2*x[1]
		a[i]= a[i]<<1;
		b[i]= b[i]<<1;
		
		// 8*x[3]+8*x[2]+2*x[1]+x[0]
		a[i]= a[i]+a1[small_len*0+i];
		b[i]= b[i]+b1[small_len*0+i];
	}

	//-----first multiplication
	pol_mul_sb(a1_ph, b1_ph, w7, p_mod, small_len, 0);
	//-----second multiplication
	pol_mul_sb(a2_ph, b2_ph, w5, p_mod, small_len, 0);
	//-----Third multiplication
	pol_mul_sb(a3_ph, b3_ph, w6, p_mod, small_len, 0);
	//-----Fourth multilication
	pol_mul_sb(a4_ph, b4_ph, w3, p_mod, small_len, 0);	
	//-----Fifth Multiplication
	pol_mul_sb(a5_ph, b5_ph, w4, p_mod, small_len, 0);	
	//-----Sixth Multiplication
	pol_mul_sb(a6_ph, b6_ph, w1, p_mod, small_len, 0);	
	//-----Seventh Multiplication
	pol_mul_sb(a, b, w2, p_mod, small_len, 0);


	/*	--------------------------------------------
		---------------Solution starts--------------
		--------------------------------------------

	*/
	

	for(i=0;i<2*small_len;i++){
			
		w2[i]= w2[i]+w5[i];//w2 <- w2+w5
		w6[i]= w6[i]-w5[i];// w6 <- w6-w5
		w4[i]= w4[i]-w3[i];// w4 <- w4-w3
		
		w5[i]= w5[i]-w1[i];// w5 <- w5-w1
		temp1[i] = w7[i]<<6; //temp <- 64*w7
		w5[i]= w5[i]-temp1[i];// w5 <- w5-64*w7

		w4[i] = w4[i]>>1; //w4 <- w4/2
		w3[i] = w3[i]+w4[i];//w3 <- w3+w4

		temp1[i] = w5[i]<<1; //temp <- 2*w5
		w5[i]= w6[i]+temp1[i];//w5 <- 2*w5+w6

		temp1[i] = w3[i]<<6; //temp <- 64*w3
		temp1[i] = w3[i]+temp1[i]; //temp <- 65*w3
		w2[i]= w2[i]-temp1[i];// w2 <- w2-65*w3

		w3[i]= w3[i]-w7[i];// w3 <- w3-w7
		w3[i]= w3[i]-w1[i];// w3 <- w3-w1

		temp1[i] = w3[i]*int45; //temp <- 45*w3
		w2[i] = w2[i]+temp1[i]; //w2 <- w2+45*w3

		temp1[i] = w3[i]<<3; //temp <- 8*w3
		w5[i]= w5[i]-temp1[i];//w5 <- w5-8*w3

		w5[i] = w5[i]*inv3; //w5 <- w5*1/3
		w5[i] = w5[i]>>3; //w5 <- w5*1/8 ---> w5=w5/24

		w6[i] = w2[i]+w6[i]; //w6 <- w6+w2
		temp1[i] = w4[i]<<4; //temp <- 16*w4
		w2[i] = w2[i]+temp1[i]; //w2 <- w2+16*w4

		w2[i] = w2[i]*inv9; //w2 <- w2*1/9
		w2[i] = w2[i]>>1; //w2 <- w2*1/2 ---> w2=w2/18

		w3[i]= w3[i]-w5[i];//w3 <- w3-w5
		
		w4[i] = w4[i]+w2[i]; //w4 <- w4+w2

		w4[i] = int0-w4[i]; //w4 <- -(w4+w2)

		temp1[i] = w2[i]*int30; //temp <- w2*30
		w6[i]= temp1[i]-w6[i];//w6 <- 30*w2-w6

		w6[i] = w6[i]*inv15; //w6 <- w6*1/15
		w6[i] = w6[i]>>2; //w6 <- w6*1/4 ---> w6=w6/60

		w2[i]= w2[i]-w6[i];//w2 <- w2-w6

	}


	for(i=0; i<2*SABER_N; i++){
		result_final[i] = 0;
	}	

	for(i=0;i<2*small_len-1;i++){	
		result_final[0*small_len+i]= result_final[0*small_len+i]+ w7[i];
		result_final[1*small_len+i]= result_final[1*small_len+i]+ w6[i];
		result_final[2*small_len+i]= result_final[2*small_len+i]+ w5[i];
		result_final[3*small_len+i]= result_final[3*small_len+i]+ w4[i];
		result_final[4*small_len+i]= result_final[4*small_len+i]+ w3[i];
		result_final[5*small_len+i]= result_final[5*small_len+i]+ w2[i];
		result_final[6*small_len+i]= result_final[6*small_len+i]+ w1[i];
	}		
	
	//---------------reduction-------
	for(i=n;i<2*n-1;i++){
		result_final[i-n]=(result_final[i-n]-result_final[i]);
	}
	
	result_final[n]=0; //256th coefficient=0;
	//----------------------copy result back----------------

	for(i=0; i<n; i++){
            result[i] = reduce(result_final[i],p_mod_or);;
	}


}




void pol_mul_sb(int16_t* a, int16_t* b, int16_t* res, uint16_t p, uint32_t n,uint32_t start) //simple school book
{ // Polynomial multiplication using the schoolbook method, c[x] = a[x]*b[x] 
	// SECURITY NOTE: TO BE USED FOR TESTING ONLY.  


	uint32_t i, j,mask = 2*n - 1;
	
	//int16_t *c;
	//c=(int16_t *)malloc(mask*sizeof(int16_t));
	int16_t c[2*n-1];
	for (i = 0; i < mask; i++) c[i] = 0;

	for (i = start; i < start+n; i++) {
		for (j = start; j < start+n; j++) {
			c[i+j-2*start]=reduce(c[i+j-2*start] + (a[i] * b[j]), p);
			//printf("i+j : %u,i:%u,j:%u,a[i]=%lu,b[j]=%lu,c[i+j]=%lu\n",i+j,i,j,a[i],b[j],c[i+j]);
		}
	}
	for (i = 0; i < mask; i++){
		res[i] = (res[i]^res[i])+c[i];
		//res[i] = reduce(res[i]+c[i],p);
	}

	//free(c);

}

static inline int16_t reduce(int16_t a, int64_t p)
{
    return a&(p-1);
}

