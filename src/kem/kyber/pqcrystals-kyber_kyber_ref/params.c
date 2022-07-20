#include <stdint.h>
#include "params.h"


/*
int8_t Kyber_param(char* name ,int8_t security_level){

    if(name == "POLYVECBYTES" || name == "INDCPA_SECRETKEYBYTES"){
        switch (security_level)
        {
        case 512:
            return 786;
        case 768:
            return 1152;
        case 1024:
            return 1536;
        }
    }
    else if(name == "ETA1"){
        switch(security_level)
        {
            case 512:
                return 3;
            case 768:
                return 2;
            case 1024:
                return 2;
        }
    }
    else if(name == "POLYCOMPRESSEDBYTES"){
        switch(security_level)
        {
            case 512:
                return 128;
            case 768:
                return 128;
            case 1024:
                return 160;
        }
    }
    else if(name =="POLYVECCOMPRESSEDBYTES"){
        switch(security_level)
        {
            case 512:
                return 640;
            case 768:
                return 960;
            case 1024:
                return 1408;
        }
    }
    else if(name == "INDCPA_PUBLICKEYBYTES" || name == "PUBLICKEYBYTES"){
        switch (security_level)
        {
        case 512:
            return 25152;
        case 768:
            return 36864;
        case 1024:
            return 49152;
        }
    }
    else if(name == "INDCPA_BYTES" || name == "CIPHERTEXTBYTES"){
        switch(security_level){
            case 512:
                return 81920;
            case 768:
                return 122880;
            case 1024:
                return 225280;
        }
    }
    else if(name == "SECRETKEYBYTES"){
        switch(security_level){
            case 512:
                return Kyber_param("INDCPA_SECRETKEYBYTES",512) + Kyber_param("INDCPA_PUBLICKEYBYTES",512) + 64;
            case 768:
                return Kyber_param("INDCPA_SECRETKEYBYTES",768) + Kyber_param("INDCPA_PUBLICKEYBYTES",768) + 64;
            case 1024:
                return Kyber_param("INDCPA_SECRETKEYBYTES",1024) + Kyber_param("INDCPA_PUBLICKEYBYTES",1024) + 64;
        }
    }
    else printf("name error in Kyber_param");

    return 0;
}
*/

param params[3]={
    
    
    //Kyber Security Level = 512
    {

        2,              /* KYBER_K */
        256,            /* KYBER_N */
        3329,           /* KYBER_Q */
        32,             /* KYBER_SYMBYTES */
        32,             /* KYBER_SSBYTES */
        384,            /* KYBER_POLYBYTES */
        768,            /* KYBER_POLYVECBYTES */
        3,              /* KYBER_ETA1 */
        128,            /* KYBER_POLYCOMPRESSEDBYTES */
        640,            /* KYBER_POLYVECCOMPRESSEDBYTES */
        2,              /* KYBER_ETA2 */
        32,             /* KYBER_INDCPA_MSGBYTES */
        800,            /* KYBER_INDCPA_PUBLICKEYBYTES */
        768,            /* KYBER_INDCPA_SECRETKEYBYTES */
        768,            /* KYBER_INDCPA_BYTES */
        800,            /* KYBER_PUBLICKEYBYTES */
        1632,           /* KYBER_SECRETKEYBYTES */
        768             /* KYBER_CIPHERTEXTBYTES */


    },

    //Kyber Security Level = 768
    {
        3,              /* KYBER_K */
        256,            /* KYBER_N */
        3329,           /* KYBER_Q */
        32,             /* KYBER_SYMBYTES */
        32,             /* KYBER_SSBYTES */
        384,            /* KYBER_POLYBYTES */
        1152,           /* KYBER_POLYVECBYTES */
        2,              /* KYBER_ETA1 */
        128,            /* KYBER_POLYCOMPRESSEDBYTES */
        960,            /* KYBER_POLYVECCOMPRESSEDBYTES */
        2,              /* KYBER_ETA2 */
        32,             /* KYBER_INDCPA_MSGBYTES */
        1184,           /* KYBER_INDCPA_PUBLICKEYBYTES */
        1152,           /* KYBER_INDCPA_SECRETKEYBYTES */
        1088,            /* KYBER_INDCPA_BYTES */
        1184,           /* KYBER_PUBLICKEYBYTES */
        2400,           /* KYBER_SECRETKEYBYTES */
        1088             /* KYBER_CIPHERTEXTBYTES */
    },
    
    //Kyber Security Level = 1024
    {
        4,              /* KYBER_K */
        256,            /* KYBER_N */
        3329,           /* KYBER_Q */
        32,             /* KYBER_SYMBYTES */
        32,             /* KYBER_SSBYTES */
        384,            /* KYBER_POLYBYTES */
        1536,           /* KYBER_POLYVECBYTES */
        2,              /* KYBER_ETA1 */
        160,            /* KYBER_POLYCOMPRESSEDBYTES */
        1408,           /* KYBER_POLYVECCOMPRESSEDBYTES */
        2,              /* KYBER_ETA2 */
        32,             /* KYBER_INDCPA_MSGBYTES */
        1568,           /* KYBER_INDCPA_PUBLICKEYBYTES */
        1536,           /* KYBER_INDCPA_SECRETKEYBYTES */
        1568,            /* KYBER_INDCPA_BYTES */
        1568,           /* KYBER_PUBLICKEYBYTES */
        3168,           /* KYBER_SECRETKEYBYTES */
        1568             /* KYBER_CIPHERTEXTBYTES */
    }

};