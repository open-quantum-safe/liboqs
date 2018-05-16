For Keccak and AES we are using open-source code developed by authors listed in comments on top of the respective files.

Source codes of the algorithms for "LightSaber", "Saber", "FireSaber" 
  - All reference implementations inside "Reference_Implementation_KEM" directory. 
  - All optimized implementations inside "Optimized_Implementation_KEM" directory. 

KAT files present inside "KAT" directory.
  - KAT files for "LightSaber" inside KAT/light_saber 
     -- PQCkemKAT_1568.req : input for Known Answer Test
     -- PQCkemKAT_1568.rsp : output for Known Answer Test
  - KAT files for "Saber" inside KAT/saber
     -- PQCkemKAT_2304.req : input for Known Answer Test
     -- PQCkemKAT_2304.rsp : output for Known Answer Test
  - KAT files for "FireSaber" inside KAT/fire_saber
     -- PQCkemKAT_3040.req : input for Known Answer Test
     -- PQCkemKAT_3040.rsp : output for Known Answer Test

Compilation instruction: 
  - Use 'make clean' command to clean executable files
  - Use 'make all' command to compile the source codes 
  - Use "./test/test_kex" to run KEM operations in loop for 'repeat' number of iterations
  - Use "./test/PQCgenKAT_kem" to generate KAT files
  - NOTE : Performance measurements were done using gcc-7 on a Dell laptop with a Intel i7-6600U CPU running at 2.60 GHz disabling hyoerthreading and turboboost and using only one core. 

Inside any source code directory the following files are available:

api.h: parameters and algorithm names of the implementation
cbd.c: part of portable code for sampling from binomial distribution
cbd.h: part of portable code for sampling from binomial distribution
cpucycles.c: used to measure cycle counts
cpucycles.h: heder file for cpucycles.c
fips202.c: Part of hash function
fips202.h: Header file
fips202.o: Object file
kem.c: CCA secure implementation of the Key encapsulation scheme.
kem.h: Header file for CCA secure implementation
SABER_indcpa.c: CPA secure implementation of Key encapsulation scheme
SABER_indcpa.h: Header file for CPA secure implementation
SABER_params.h: Declaration of parameters
poly.c: Part of polynomial arithmetic functions
poly.h: Part of polynomial arithmetic functions header file
polymul.c: Polynomial multiplication algorithm
polymul.h: Header file for polynomial multiplication
verify.c: Code used for testing purpose
verify.h: Code used for testing purpose header file
recon.c: Code for reconciliation scheme 
recon.h: Code for reconciliation scheme  header file
recon_LightSaber.c: Code for reconciliation scheme for LightSaber
recon_Saber.c: Code for reconciliation scheme for Saber
recon_FireSaber.c: Code for reconciliation scheme for FireSaber
rng.c: Random number generator as provided by NIST
rng.h: Random number generator as provided by NIST header file
pack_unpack.c: part of portable code for packing/unpacking data/bytes 
pack_unpack.h: header file


test/PQCgenKAT_kem.c: NIST provided KAT file generation
test/test_kex.c: To test the Key-encapsulation mechanism



