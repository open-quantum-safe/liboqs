/* These functions test various subroutines against 
 * KAT values defined in the KAT values subdirectory
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xof.h"
#include "fft.h"
#include "LimaAPI.h"

void print_string(FILE* fp,uint8_t* seed,int len)
{ for (int k = 0; k<len; k++) { fprintf(fp,"%02x", seed[k]); } }



// Print in hex-human readable form
void print(FILE* fp,modp_context_t* ctx, int x)
{
  int y=to_int(ctx,x);
  fprintf(fp,"%0*x",2*ctx->log256_q,y);
}



void Test_XOF()
{
  FILE *fp;
  fp=fopen("../KAT/Test-XOF-KAT.txt","w+");
  
  fprintf(fp,"Test vectors for the XOF routines\n");
  fprintf(fp,"\nEach four line batch corresponds to the following values\n");
  fprintf(fp,"\tLine 1: The seed passed into InitXOF\n");
  fprintf(fp,"\tLine 2: The data value passed into InitXOF\n");
  fprintf(fp,"\tLine 3: The first 32 bytes of output of the XOF (first byte ont he left)\n");
  fprintf(fp,"\tLine 4: The first 3 field elements produced from the byte string output in the fields.\n");
  fprintf(fp,"\t\t F_{133121}, F_{44802049} and F_{133121}\n");
  fprintf(fp,"\tLine 5: The first six Gaussian values produced from that byte string\n");

  fprintf(fp,"\nExplanation/Mathematical check of the first five lines\n");

  fprintf(fp,"\nFor the fourth line\n");
  fprintf(fp,"\tlog_{256}{133121}=2.11   and  log_{256}{44802049}=3.17\n");
  fprintf(fp,"So for elements in the small field we take 6 bytes, and for elements in the big field we take 8 bytes\n");

  fprintf(fp,"0x919fe8a0e77c     = 160115988686716\n");
  fprintf(fp,"\t160115988686716     %% 133121  = 109366  = 0x01ab36\n");
  fprintf(fp,"0x68390e6888217902 = 7510049695740295426\n");
  fprintf(fp,"\t7510049695740295426 %% 44802049 = 5621370 = 0x0055c67a\n");
  fprintf(fp,"0xf87441e7e28f     = 273178205610639\n");
  fprintf(fp,"\t273178205610639     %% 133121 = 69961   = 0x011149\n");

  fprintf(fp,"\nFor the first three elements on the fifth line\n");
  fprintf(fp,"0x919fe8a0e7 = 0b1001000110011111111010001010000011100111\n");
  fprintf(fp,"\tEven Pos  0b10001011111011001101 <- Hwt = 12\n");
  fprintf(fp,"\tOdd  Pos  0b01010111100000001011 <- Hwt =  9\n");
  fprintf(fp,"\tAnswer = 9-12 = -3\n");
  fprintf(fp,"0x7c68390e68 = 0b0111110001101000001110010000111001101000\n");
  fprintf(fp,"\tEven Pos  0b01100110011000110110 <- Hwt = 10\n");
  fprintf(fp,"\tOdd Pos   0b11101000010100101000 <- Hwt =  8\n");
  fprintf(fp,"\tAnswer = 8-10 = -2\n");
  fprintf(fp,"0x88217902f8 = 0b1000100000100001011110010000001011111000\n");
  fprintf(fp,"\tEvenPos   0b10100100011000011110 <- Hwt =  9\n");
  fprintf(fp,"\tOdd Pos   0b00000001110100001100 <- Hwt =  6\n");
  fprintf(fp,"\tAnswer = 6-9 = -3\n");

  fprintf(fp,"\n----------------------------------------------------------------\n\n");

  XOF_t xof;

  uint8_t seed[70],output[32];
  int i,len,a;

  for (i=0; i<70; i++) { seed[i]=i; }

  for (len=32; len<70; len++)
    { fprintf(fp,"Seed  : "); 
      print_string(fp,seed,len);
      fprintf(fp,"\n");

      fprintf(fp,"Data  : 01\n");

      fprintf(fp,"Output: ");
      InitXOF(&xof, seed, len,1); 
      XOF(&xof,output,32);
      print_string(fp,output,32);
      fprintf(fp,"\n");

      // Initialise again to get a fresh PRNG
      fprintf(fp,"modp  : ");
      InitXOF(&xof, seed, len,1);
      a=get_random_field_element(&xof,&mod133121);
      print(fp,&mod133121,a); fprintf(fp," ");
      a = get_random_field_element(&xof, &mod44802049);
      print(fp,&mod44802049,a);  fprintf(fp," ");
      a = get_random_field_element(&xof, &mod133121);
      print(fp,&mod133121,a); fprintf(fp," ");
      fprintf(fp,"\n");

      // Initialise again to gt a fresh PRNG
      fprintf(fp,"Gauss : ");
      InitXOF(&xof, seed, len,1);
      for (i=0; i<6; i++)
         { a=generate_gaussian_noise(&xof);
           fprintf(fp,"%d ",a);
         }
      fprintf(fp,"\n");
      
      fprintf(fp,"\n");
    }
  fclose(fp);
}



void Test_FFT_Sub(FILE *fp,lima_params_t* params)
{
  int a[LIMA_MAX_N],fa[LIMA_MAX_N],a2[2*LIMA_MAX_N+1],fa2[LIMA_MAX_N],b[LIMA_MAX_N];
  int i,j,k,N=params->N,v,t;
  
  // Compute FFT of all one vector
  for (i=0; i<N; i++)
    { a[i]=to_mod_q(params->ctx,1); }
  fft(params,a,fa);
  
  // Print FFT as KAT
  fprintf(fp,"FFT All Ones Vector:\n");
  for (i=0; i<N; i++)
    { print(fp,params->ctx,fa[i]); fprintf(fp," "); }
  fprintf(fp,"\n");
  
  // Invert it for a quick check
  fft_inv(params,fa,b);
  for (i=0; i<N; i++)
    { if (b[i] != a[i]) { abort(); } }
  
  // Square both a and fa
  for (i=0; i<N; i++)
    { fa2[i]=mul_mod_q(params->ctx,fa[i],fa[i]); }

  if (params->mode==LIMA_2P)
    { for (i=0; i<N; i++)
        { a2[i]=0;
          for (j = 0; j <= i; j++)
            { a2[i]=mul_add_mod_q(params->ctx,a[j],a[i-j],a2[i]); }
          for (j = i+1; j < N; j++)
            { t= mul_mod_q(params->ctx, a[j], a[N + i - j]); 
              a2[i] = sub_mod_q(params->ctx,a2[i],t);
            }
        }
    }
  else
    { for (i=0; i<2*N; i++) { a2[i]=0; }
      for (i = 0; i<N; i++)
        { for (j=0; j<N; j++) 
            { t=mul_mod_q(params->ctx,a[i],a[j]);
              v=i+j;
              a2[v]=add_mod_q(params->ctx,a2[v],t);
            }
        }
      for (i = 2 * N - 1; i >= N; i--)
        { t=a2[i];
          for (k=0; k<=N; k++)
            { a2[i+k-N]=sub_mod_q(params->ctx,a2[i+k-N],t); }
        }
    }

  // Now invert the fft version and check
  fft_inv(params,fa2,b);
  for (i=0; i<N; i++)
    { if (b[i]!=a2[i]) { abort(); } }
}


void Test_FFT()
{ 
  FILE *fp;
  fp=fopen("../KAT/Test-FFT-KAT.txt","w+");

  fprintf(fp,"Testing FFT\n\n");
  fprintf(fp,"lima_2p_1024\n");
  Test_FFT_Sub(fp,&lima_2p_1024);
  fprintf(fp,"lima_2p_2048\n");
  Test_FFT_Sub(fp,&lima_2p_2048);
  fprintf(fp,"lima_sp_1018\n");
  Test_FFT_Sub(fp,&lima_sp_1018);
  fprintf(fp,"lima_sp_1306\n");
  Test_FFT_Sub(fp,&lima_sp_1306);
  fprintf(fp,"lima_sp_1822\n");
  Test_FFT_Sub(fp,&lima_sp_1822);
  fprintf(fp,"lima_sp_2062\n");
  Test_FFT_Sub(fp,&lima_sp_2062);
  fclose(fp);
}

void Test_Enc_Sub(lima_params_t* params)
{
  
  lima_pk_t pk;
  lima_sk_t sk;
  lima_ciphertext_t c;

  uint8_t seedkg[32],seedenc[32],seedencap[64],mess[10],dmess[10],key[32],dkey[32];
  int i;
  byte_length len;
  
  for (i=0; i<32; i++)
    { seedkg[i]=i; seedenc[i]=i+40; 
      seedencap[i]=i; seedencap[i+32]=i+32;
    }

  for (i=0; i<10; i++)
    { mess[i]=i*25; }

  int not_ok;
  printf("Doing KeyGen\n");
  not_ok=lima_key_gen(params,seedkg,32,&pk,&sk);
  if (not_ok) { printf("Error\n"); abort(); }

  printf("Doing Enc CPA\n");
  not_ok=lima_enc_cpa(params,mess,10,&pk,seedenc,32,&c);
  if (not_ok) { printf("Error\n"); abort(); }

  printf("Doing Dec CPA\n");
  not_ok=lima_dec_cpa(params,&c,&sk,dmess,&len);
  if (not_ok) { printf("Error\n"); abort(); }

  if (len!=10) { printf("Length wrong\n"); abort(); }
  for (i=0; i<10; i++)
    { if (mess[i]!=dmess[i]) { abort(); } }

  printf("Doing Enc CCA\n");
  not_ok = lima_enc_cca(params, mess, 10, &pk, seedenc, &c);
  if (not_ok) { printf("Error\n"); abort(); }

  printf("Doing Dec CCA\n");
  not_ok = lima_dec_cca(params, &c, &sk, dmess, &len);
  if (not_ok) { printf("Error\n"); abort(); }

  if (len != 10) { printf("Length wrong\n"); abort(); }
  for (i = 0; i<10; i++)
  {
      if (mess[i] != dmess[i]) { abort(); }
  }

  printf("Doing Encap CPA\n");
  not_ok = lima_encap_cpa(params, &pk, seedencap, 64, key, 32, &c);
  if (not_ok) { printf("Error\n"); abort(); }

  printf("Doing Decap CPA\n");
  not_ok = lima_decap_cpa(params, &c, &sk, dkey, &len);
  if (not_ok) { printf("Error\n"); abort(); }

  if (len != 32) { printf("Length wrong\n"); abort(); }
  for (i = 0; i<32; i++)
  {
      if (key[i] != dkey[i]) { abort(); }
  }

  printf("Doing Encap CCA\n");
  not_ok = lima_encap_cca(params, &pk, seedencap, 64, key, 32, &c);
  if (not_ok) { printf("Error\n"); abort(); }

  printf("Doing Decap CCA\n");
  not_ok = lima_decap_cca(params, &c, &sk, dkey, 32);
  if (not_ok) { printf("Error\n"); abort(); }

  for (i = 0; i<32; i++)
  {
      if (key[i] != dkey[i]) { abort(); }
  }

  printf("All OK\n\n");
}



void Test_Encryption(int sklen,int pklen)
{
  uint8_t pks[20000],sks[30000],cts[17000];
  uint8_t seed[48],mess[32],messo[32],seed2[32],seed3[80],key[32],keyo[32];
  byte_length clen,mlen,klen;


  for (int i=0; i<48; i++) { seed[i]=i; }
  for (int i=0; i<32; i++) { seed2[i]=i; }
  for (int i=0; i<80; i++) { seed3[i]=i; }

  if (Key_Generation(pks,pklen,sks,sklen,seed,48)) { abort(); }

  for (int i=0; i<32; i++) { mess[i]=255-i; }
  if (Enc_CPA(cts,&clen,mess,32,seed,48,pks)) { abort(); }
  
  if (Dec_CPA(messo,&mlen,cts,sks)) { abort(); }
  if (mlen!=32)  { abort(); }
  for (int i=0; i<32; i++) { if (mess[i]!=messo[i]) { abort(); } }
  
  for (int i=0; i<32; i++) { mess[i]=255-i; }
  if (Enc_CCA(cts,&clen,mess,32,seed2,pks)) { abort(); }

  if (Dec_CCA(messo,&mlen,cts,sks)) { abort(); }
  if (mlen!=32)  { abort(); }
  for (int i=0; i<32; i++) { if (mess[i]!=messo[i]) { abort(); } }

  if (Encap_CPA(cts,&clen,key,32,seed3,80,pks)) { abort(); }

  if (Decap_CPA(keyo,&klen,cts,sks)) { abort(); }
  if (klen!=32)  { abort(); }
  for (int i=0; i<32; i++) { if (key[i]!=keyo[i]) { abort(); } }

  if (Encap_CCA(cts,&clen,key,32,seed,48,pks)) { abort(); }

  if (Decap_CCA(keyo,32,cts,sks)) { abort(); }
  for (int i=0; i<32; i++) { if (key[i]!=keyo[i]) { abort(); } }
}



void Test_Enc()
{
    printf("Testing Enc internal routines\n");
    printf("lima_2p_1024\n");
    Test_Enc_Sub(&lima_2p_1024);
    printf("lima_2p_2048\n");
    Test_Enc_Sub(&lima_2p_2048);
    printf("lima_sp_1018\n");
    Test_Enc_Sub(&lima_sp_1018);
    printf("lima_sp_1306\n");
    Test_Enc_Sub(&lima_sp_1306);
    printf("lima_sp_1822\n");
    Test_Enc_Sub(&lima_sp_1822);
    printf("lima_sp_2062\n");
    Test_Enc_Sub(&lima_sp_2062);
}


void Test_ENC()
{
  int sk_sz[6]={9217,18433,9163,15673,21865,24745};
  int pk_sz[6]={6145,12289,6109,10449,14577,16497};

  for (int i=0; i<6; i++)
    { Test_Encryption(sk_sz[i],pk_sz[i]); }
}


int main(int argc,char* argv[])
{
  InitLima();

  int done=0;
  if (argc == 1 || strcmp(argv[1], "All") == 0 || strcmp(argv[1], "XOF") == 0)
    { Test_XOF(); done=1; }
  if (argc == 1 || strcmp(argv[1], "All") == 0 || strcmp(argv[1], "FFT") == 0)
    { Test_FFT(); done = 1; }
  if (argc == 1 || strcmp(argv[1], "All") == 0 || strcmp(argv[1], "enc") == 0)
    { Test_Enc(); done = 1; }
  if (argc == 1 || strcmp(argv[1], "All") == 0 || strcmp(argv[1], "ENC") == 0)
    { Test_ENC(); done = 1; }
  if (!done)
    { printf("Either use with no arguments or arguments \n");
      printf("This basically produces the data for the KAT files in the KAT subdirectory\n");
      printf("\t All\n");
      printf("\t XOF\n");
      printf("\t FFT\n");
      printf("\t enc (Internal Checking)\n");
      printf("\t ENC (External Checking)\n");
      printf("There is no test for the MODP module, as that is tested by the FFT routines\n");
    }
  
  printf("Finished\n");
  printf("For XOF and FFT testing now run diff in the KAT directory between the Test files and the given files\n");
  return 0;
}


