#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <xmss.h>

#define MAX_LIMIT 256
#define MAX_FILENAME 32

int main() {

    // Variables needed for the XMSS signing
    unsigned char input[MAX_LIMIT], message[MAX_LIMIT];
    unsigned char *pk, *sk, *sig;
    unsigned long long siglength, mlength;  
    
    unsigned char filename[30];
    FILE *fpt;

    // XMSS-SHA2_16_256
    const uint32_t oid = 0x00000002;

    while (1) {
        printf("Command > ");
        gets(input);

        if (strcmp(input, "gen")) {
            xmss_keypair(pk, sk, oid);
            
        } else if (strcmp(input, "sign")) {

            printf("Message > ");
            gets(message);
            mlength = strlen(message);
            
            printf("File stem to write to > ");
            gets(filename);

            xmss_sign(sk, sig, siglength, message, mlength);

            fpt = fopen(strcat(filename, ".sig"), "w+");
            fputs(sig, fpt);
            fclose(fpt);
            
        } else if (strcmp(input, "verify")) {

            unsigned char filename[30];
            printf("Signature filestem name > ");
            gets(filename);
            fpt = fopen(strcat(filename, ".sig"), "r");

            xmss_sign_open(message, mlength, sig, siglength, pk);
            fclose(fpt);

        } else {
            break;
        }
    }

    free(pk);
    free(sk);
    free(sig);
}