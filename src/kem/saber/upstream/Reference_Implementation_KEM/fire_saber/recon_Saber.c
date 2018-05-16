#include <stdio.h>
#include "recon.h"

uint16_t floor_special(uint16_t a)
{

	int sign;
	uint16_t b;
	int16_t abs;
	int16_t sign2, temp;


	sign = (a>>15) & 0x01;
	
        abs = sign*(0-a) + (1-sign)*a;      // absolute value of a in constant time;

		
	//b = (abs>>9); 			   
	b = abs; 			    

	
	// when a is -ve, we need to do more computation (but in constant time)
	temp = abs & 0x01ff;	// least 9 bits of a;

	temp = -temp;		// if temp was 0, then result is 0; otherwise -ve. so the sign bit tells it.

	

	sign2 = (temp>>15) & 0x01;
	
	
	abs = abs + sign*sign2;

	b = sign*(0-abs) + (1-sign)*b;

	b = (b>>9) & 0x01;

	
	return(b);
}


// generates K1 and recon_data from b_avx
void recon_gen(uint16_t b, uint16_t *K1, uint16_t *recon_data)
{
    uint16_t temp;

    *K1 = (b>>9);    // K1 = b>>9; hence 1 bit per coefficient
    temp = (b & 0x01ff);    // keeps the least 9 bits of temp
    *recon_data = (temp>>6);    // recon_data = [8:6] bits

}



// generates binary K2 from a_avx and recon_data
void recon(uint16_t a, uint16_t recon_data, uint16_t *K2)
{
    uint16_t temp;

    //temp = a - (recon_data<<6) - 2^5 + 2^8;
    temp = (recon_data << 6);   // temp = recon_data<<6
    temp = (a - temp);        	// temp = a - (recon_data<<7)
    temp = (temp - 32);        	// temp = a - (recon_data<<7) - 2^5
    temp = (temp + 256);        // temp = a - (recon_data<<7) - 2^5 + 2^8

    //K2 = floor(temp/2^10);
    *K2 = floor_special(temp);

}

// generates ciphertext from input
void ciphertext_gen(uint16_t b, uint16_t *ciphertext)
{
 
    *ciphertext = (b >> 6);    // ciphertext = b>>6; hence 4 bits per coefficient
 
}

void ReconDataGen(uint16_t *vprime, unsigned char *rec_c){


	uint16_t ciphertext_temp[SABER_N];
	int32_t i;

	for(i=0;i<SABER_N;i++)
		ciphertext_gen(vprime[i], ciphertext_temp+i);

	SABER_pack_4bit(rec_c, ciphertext_temp);



}

void Recon(uint16_t *recon_data,unsigned char *recon_ar,uint16_t *message_dec_unpacked){


	int32_t i;
	uint16_t op[SABER_N];	

	SABER_un_pack4bit(recon_ar, op);

	for(i=0;i<SABER_N;i++){
		    recon(recon_data[i], op[i], message_dec_unpacked+i);
	}


}

