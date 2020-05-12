//************************************************************************************
// qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
//
// Abstract: optimized polynomial multiplication for case N = 1024 written in 
//           x64 assembly with AVX2 support for Linux 
//************************************************************************************

.intel_syntax noprefix 

// Registers that are used for parameter passing:
#define reg_p1  rdi
#define reg_p2  rsi
#define reg_p3  rdx

#define PARAM_BARR_DIV 30


.text
//**********************************************************************************
//  Forward NTT, case N = 1024
//  Operation: c [reg_p1] <- NTT(a) [reg_p2], 
//             [reg_p3] points to table with the NTT constants. 
//  Output c is in extended form. 
//
//  NTT implementation is an optimized variant of the approach by G. Seiler, 
//  "Faster AVX2 optimized NTT multiplication for Ring-LWE lattice cryptography",
//  https://eprint.iacr.org/2018/039.pdf.
//  See also https://eprint.iacr.org/2017/633.pdf by L. Ducas, E. Kiltz, T. Lepoint, 
//  V. Lyubashevsky, P. Schwabe and G. Seiler, that reports an implementation of the 
//  approach.
//********************************************************************************** 
.global oqs_sig_qteslapi_poly_ntt_asm
oqs_sig_qteslapi_poly_ntt_asm:
  xor          rax, rax 
  xor          r11, r11 
  vmovdqa      ymm15, YMMWORD PTR [oqs_sig_qteslapi_PARAM_Qx4+rip]
  vmovdqa      ymm14, YMMWORD PTR [oqs_sig_qteslapi_PARAM_QINVx4+rip]
  vbroadcastss ymm12, DWORD PTR [reg_p3]              // Load twiddle factor W     
  mov          r10, 32                                
loop_ntt123:
// Round 1  
  vpmovsxdq    ymm4, XMMWORD PTR [reg_p2+rax+2048]    // a[j+k]           
  vpmovsxdq    ymm5, XMMWORD PTR [reg_p2+rax+2560]      
  vpmovsxdq    ymm6, XMMWORD PTR [reg_p2+rax+3072] 
  vpmovsxdq    ymm7, XMMWORD PTR [reg_p2+rax+3584]
  vpmovsxdq    ymm0, XMMWORD PTR [reg_p2+rax]         // a[j] 
  vpmovsxdq    ymm1, XMMWORD PTR [reg_p2+rax+512]  
  vpmovsxdq    ymm2, XMMWORD PTR [reg_p2+rax+1024]
  vpmovsxdq    ymm3, XMMWORD PTR [reg_p2+rax+1536] 
  vpmuldq      ymm4, ymm4, ymm12                      // W.a[j+k]
  vpmuldq      ymm5, ymm5, ymm12                    
  vpmuldq      ymm6, ymm6, ymm12                    
  vpmuldq      ymm7, ymm7, ymm12   
  vpmuldq      ymm8, ymm4, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm9, ymm5, ymm14                    
  vpmuldq      ymm10, ymm6, ymm14 
  vpmuldq      ymm11, ymm7, ymm14
  vpmuludq     ymm8, ymm8, ymm15  
  vpmuludq     ymm9, ymm9, ymm15 
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15
  vpaddq       ymm4, ymm4, ymm8                      
  vpaddq       ymm5, ymm5, ymm9                     
  vpaddq       ymm6, ymm6, ymm10                                               
  vpaddq       ymm7, ymm7, ymm11  
  vpsrlq       ymm8, ymm4, 32                        
  vpsrlq       ymm9, ymm5, 32                       
  vpsrlq       ymm10, ymm6, 32                                      
  vpsrlq       ymm11, ymm7, 32 
  vpsubd       ymm4, ymm0, ymm8                       // a[j+k] = a[j] - temp
  vpsubd       ymm5, ymm1, ymm9                     
  vpsubd       ymm6, ymm2, ymm10
  vpsubd       ymm7, ymm3, ymm11
  vpaddd       ymm0, ymm0, ymm8                       // a[j] = a[j] + temp    
  vpaddd       ymm1, ymm1, ymm9                     
  vpaddd       ymm2, ymm2, ymm10  
  vpaddd       ymm3, ymm3, ymm11  
  vpsrad       ymm8, ymm4, 31                         // If result < 0 then add q                   
  vpsrad       ymm9, ymm5, 31                       
  vpsrad       ymm10, ymm6, 31                       
  vpsrad       ymm11, ymm7, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                    
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm4, ymm4, ymm8                      
  vpaddd       ymm5, ymm5, ymm9                     
  vpaddd       ymm6, ymm6, ymm10                     
  vpaddd       ymm7, ymm7, ymm11 
  vpsubd       ymm0, ymm0, ymm15                      // If result >= q then subtract q    
  vpsubd       ymm1, ymm1, ymm15                     
  vpsubd       ymm2, ymm2, ymm15                      
  vpsubd       ymm3, ymm3, ymm15    
  vpsrad       ymm8, ymm0, 31                 
  vpsrad       ymm9, ymm1, 31                       
  vpsrad       ymm10, ymm2, 31                       
  vpsrad       ymm11, ymm3, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                   
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm0, ymm0, ymm8                      
  vpaddd       ymm1, ymm1, ymm9                     
  vpaddd       ymm2, ymm2, ymm10                      
  vpaddd       ymm3, ymm3, ymm11  
  
  vbroadcastss ymm10, DWORD PTR [reg_p3+4]            // Load twiddle factor W
  vbroadcastss ymm13, DWORD PTR [reg_p3+8]                   
                               
// Round 2
  vpmuldq      ymm2, ymm2, ymm10                      // W.a[j+k]
  vpmuldq      ymm3, ymm3, ymm10                    
  vpmuldq      ymm6, ymm6, ymm13                    
  vpmuldq      ymm7, ymm7, ymm13
  vpmuldq      ymm8, ymm2, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm9, ymm3, ymm14                    
  vpmuldq      ymm10, ymm6, ymm14  
  vpmuldq      ymm11, ymm7, ymm14
  vpmuludq     ymm8, ymm8, ymm15 
  vpmuludq     ymm9, ymm9, ymm15 
  vpmuludq     ymm10, ymm10, ymm15                                 
  vpmuludq     ymm11, ymm11, ymm15   
  vpaddq       ymm2, ymm2, ymm8                      
  vpaddq       ymm3, ymm3, ymm9                     
  vpaddq       ymm6, ymm6, ymm10                                
  vpaddq       ymm7, ymm7, ymm11      
  vpsrlq       ymm8, ymm2, 32                        
  vpsrlq       ymm9, ymm3, 32                       
  vpsrlq       ymm10, ymm6, 32                                   
  vpsrlq       ymm11, ymm7, 32
  vpsubd       ymm2, ymm0, ymm8                       // a[j+k] = a[j] - temp 
  vpsubd       ymm3, ymm1, ymm9                    
  vpsubd       ymm6, ymm4, ymm10
  vpsubd       ymm7, ymm5, ymm11
  vpaddd       ymm0, ymm0, ymm8                       // a[j] = a[j] + temp 
  vpaddd       ymm1, ymm1, ymm9                     
  vpaddd       ymm4, ymm4, ymm10 
  vpaddd       ymm5, ymm5, ymm11  
  vpsrad       ymm8, ymm2, 31                         // If result < 0 then add q                   
  vpsrad       ymm9, ymm3, 31                       
  vpsrad       ymm10, ymm6, 31                       
  vpsrad       ymm11, ymm7, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                    
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm2, ymm2, ymm8                      
  vpaddd       ymm3, ymm3, ymm9                     
  vpaddd       ymm6, ymm6, ymm10                     
  vpaddd       ymm7, ymm7, ymm11 
  vpsubd       ymm0, ymm0, ymm15                      // If result >= q then subtract q    
  vpsubd       ymm1, ymm1, ymm15                     
  vpsubd       ymm4, ymm4, ymm15                      
  vpsubd       ymm5, ymm5, ymm15    
  vpsrad       ymm8, ymm0, 31                 
  vpsrad       ymm9, ymm1, 31                       
  vpsrad       ymm10, ymm4, 31                       
  vpsrad       ymm11, ymm5, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                   
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm0, ymm0, ymm8                      
  vpaddd       ymm1, ymm1, ymm9                     
  vpaddd       ymm4, ymm4, ymm10                      
  vpaddd       ymm5, ymm5, ymm11  

  vbroadcastss ymm8, DWORD PTR [reg_p3+12]            // Load twiddle factor W
  vbroadcastss ymm9, DWORD PTR [reg_p3+16]            
  vbroadcastss ymm10, DWORD PTR [reg_p3+20]         
  vbroadcastss ymm13, DWORD PTR [reg_p3+24]                                 

// Round 3
  vpmuldq      ymm1, ymm1, ymm8                       // W.a[j+k]
  vpmuldq      ymm3, ymm3, ymm9                     
  vpmuldq      ymm5, ymm5, ymm10                    
  vpmuldq      ymm7, ymm7, ymm13
  vpmuldq      ymm8, ymm1, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm9, ymm3, ymm14                    
  vpmuldq      ymm10, ymm5, ymm14  
  vpmuldq      ymm11, ymm7, ymm14
  vpmuludq     ymm8, ymm8, ymm15
  vpmuludq     ymm9, ymm9, ymm15 
  vpmuludq     ymm10, ymm10, ymm15                             
  vpmuludq     ymm11, ymm11, ymm15
  vpaddq       ymm1, ymm1, ymm8                     
  vpaddq       ymm3, ymm3, ymm9                     
  vpaddq       ymm5, ymm5, ymm10                                     
  vpaddq       ymm7, ymm7, ymm11   
  vpsrlq       ymm8, ymm1, 32                           
  vpsrlq       ymm9, ymm3, 32                       
  vpsrlq       ymm10, ymm5, 32                                 
  vpsrlq       ymm11, ymm7, 32
  vpsubd       ymm1, ymm0, ymm8                       // a[j+k] = a[j] - temp
  vpsubd       ymm3, ymm2, ymm9                    
  vpsubd       ymm5, ymm4, ymm10
  vpsubd       ymm7, ymm6, ymm11
  vpaddd       ymm0, ymm0, ymm8                       // a[j] = a[j] + temp
  vpaddd       ymm2, ymm2, ymm9                      
  vpaddd       ymm4, ymm4, ymm10                               
  vpaddd       ymm6, ymm6, ymm11   
  vpsrad       ymm8, ymm1, 31                         // If result < 0 then add q                   
  vpsrad       ymm9, ymm3, 31                       
  vpsrad       ymm10, ymm5, 31                       
  vpsrad       ymm11, ymm7, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                    
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm1, ymm1, ymm8                      
  vpaddd       ymm3, ymm3, ymm9                     
  vpaddd       ymm5, ymm5, ymm10                     
  vpaddd       ymm7, ymm7, ymm11 
  vpsubd       ymm0, ymm0, ymm15                      // If result >= q then subtract q    
  vpsubd       ymm2, ymm2, ymm15                     
  vpsubd       ymm4, ymm4, ymm15                      
  vpsubd       ymm6, ymm6, ymm15    
  vpsrad       ymm8, ymm0, 31                 
  vpsrad       ymm9, ymm2, 31                       
  vpsrad       ymm10, ymm4, 31                       
  vpsrad       ymm11, ymm6, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                   
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm0, ymm0, ymm8                      
  vpaddd       ymm2, ymm2, ymm9                     
  vpaddd       ymm4, ymm4, ymm10                      
  vpaddd       ymm6, ymm6, ymm11   
  vmovdqa      [reg_p1+r11+1024], ymm1                // Store in temporary storage        
  vmovdqa      [reg_p1+r11+3072], ymm3
  vmovdqa      [reg_p1+r11+5120], ymm5
  vmovdqa      [reg_p1+r11+7168], ymm7
  vmovdqa      [reg_p1+r11], ymm0                     
  vmovdqa      [reg_p1+r11+2048], ymm2
  vmovdqa      [reg_p1+r11+4096], ymm4
  vmovdqa      [reg_p1+r11+6144], ymm6
  add          rax, 16
  add          r11, 32
  dec          r10
  jnz          loop_ntt123
                                                    
  xor          r11, r11 
  xor          rax, rax
round4_loop:
  vbroadcastss ymm12, DWORD PTR [reg_p3+rax+28]       // Load twiddle factor W
  vbroadcastss ymm9, DWORD PTR [reg_p3+rax+32]                   
  vbroadcastss ymm10, DWORD PTR [reg_p3+rax+36]         
  vbroadcastss ymm11, DWORD PTR [reg_p3+rax+40] 
  mov          r10, 16                               
loop_ntt4:
// Round 4   
  vmovdqa      ymm4, YMMWORD PTR [reg_p1+r11+512]     // Load a[j+k] from temporary storage 
  vmovdqa      ymm5, YMMWORD PTR [reg_p1+r11+1536]  
  vmovdqa      ymm6, YMMWORD PTR [reg_p1+r11+2560] 
  vmovdqa      ymm7, YMMWORD PTR [reg_p1+r11+3584]
  vmovdqa      ymm0, YMMWORD PTR [reg_p1+r11]         // Load a[j] from temporary storage
  vmovdqa      ymm1, YMMWORD PTR [reg_p1+r11+1024]      
  vmovdqa      ymm2, YMMWORD PTR [reg_p1+r11+2048]    
  vmovdqa      ymm3, YMMWORD PTR [reg_p1+r11+3072] 
  vpmuldq      ymm4, ymm4, ymm12                      // W.a[j+k]
  vpmuldq      ymm5, ymm5, ymm9                     
  vpmuldq      ymm6, ymm6, ymm10                    
  vpmuldq      ymm7, ymm7, ymm11  

  vpmuldq      ymm8, ymm4, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm13, ymm5, ymm14
  vpmuludq     ymm8, ymm8, ymm15  
  vpmuludq     ymm13, ymm13, ymm15
  vpaddq       ymm4, ymm4, ymm8                     
  vpaddq       ymm5, ymm5, ymm13
  vpsrlq       ymm8, ymm4, 32                        
  vpsrlq       ymm13, ymm5, 32
  vpsubd       ymm4, ymm0, ymm8                       // a[j+k] = a[j] - temp
  vpsubd       ymm5, ymm1, ymm13                    
  vpaddd       ymm0, ymm0, ymm8                       // a[j] = a[j] + temp   
  vpaddd       ymm1, ymm1, ymm13    
  vpsrad       ymm8, ymm4, 31                         // If result < 0 then add q                   
  vpsrad       ymm13, ymm5, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm13, ymm13, ymm15  
  vpaddd       ymm4, ymm4, ymm8                      
  vpaddd       ymm5, ymm5, ymm13 
  vpsubd       ymm0, ymm0, ymm15                      // If result >= q then subtract q    
  vpsubd       ymm1, ymm1, ymm15   
  vpsrad       ymm8, ymm0, 31                 
  vpsrad       ymm13, ymm1, 31 
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm13, ymm13, ymm15  
  vpaddd       ymm0, ymm0, ymm8                      
  vpaddd       ymm1, ymm1, ymm13                     

  vpmuldq      ymm8, ymm6, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm13, ymm7, ymm14
  vpmuludq     ymm8, ymm8, ymm15                             
  vpmuludq     ymm13, ymm13, ymm15
  vpaddq       ymm6, ymm6, ymm8                                    
  vpaddq       ymm7, ymm7, ymm13
  vpsrlq       ymm8, ymm6, 32                                      
  vpsrlq       ymm13, ymm7, 32
  vpsubd       ymm6, ymm2, ymm8                       // a[j+k] = a[j] - temp
  vpsubd       ymm7, ymm3, ymm13
  vpaddd       ymm2, ymm2, ymm8                       // a[j] = a[j] + temp                          
  vpaddd       ymm3, ymm3, ymm13                    
  vpsrad       ymm8, ymm6, 31                         // If result < 0 then add q                       
  vpsrad       ymm13, ymm7, 31                      
  vpand        ymm8, ymm8, ymm15                    
  vpand        ymm13, ymm13, ymm15                 
  vpaddd       ymm6, ymm6, ymm8                     
  vpaddd       ymm7, ymm7, ymm13                     
  vpsubd       ymm2, ymm2, ymm15                      // If result >= q then subtract q                      
  vpsubd       ymm3, ymm3, ymm15                    
  vpsrad       ymm8, ymm2, 31                       
  vpsrad       ymm13, ymm3, 31                        
  vpand        ymm8, ymm8, ymm15                   
  vpand        ymm13, ymm13, ymm15                 
  vpaddd       ymm2, ymm2, ymm8                      
  vpaddd       ymm3, ymm3, ymm13  
  vmovdqa      [reg_p1+r11], ymm0                     // Store in temporary storage
  vmovdqa      [reg_p1+r11+512], ymm4
  vmovdqa      [reg_p1+r11+1024], ymm1
  vmovdqa      [reg_p1+r11+1536], ymm5
  vmovdqa      [reg_p1+r11+2048], ymm2
  vmovdqa      [reg_p1+r11+2560], ymm6
  vmovdqa      [reg_p1+r11+3072], ymm3
  vmovdqa      [reg_p1+r11+3584], ymm7
  add          r11, 32
  dec          r10
  jnz          loop_ntt4  
  add          rax, 16 
  mov          r11, 4096
  cmp          rax, 32
  jne          round4_loop
                                                     
  xor          r9, r9                            
  xor          r11, r11 
  xor          rax, rax
round5_loop:
  vbroadcastss ymm12, DWORD PTR [reg_p3+rax+60]       // Load twiddle factor W
  vbroadcastss ymm9, DWORD PTR [reg_p3+rax+64]                   
  vbroadcastss ymm10, DWORD PTR [reg_p3+rax+68]         
  vbroadcastss ymm11, DWORD PTR [reg_p3+rax+72] 
  mov          r10, 8                                
loop_ntt5:
// Round 5   
  vmovdqa      ymm4, YMMWORD PTR [reg_p1+r11+256]     // Load a[j+k] from temporary storage 
  vmovdqa      ymm5, YMMWORD PTR [reg_p1+r11+768]  
  vmovdqa      ymm6, YMMWORD PTR [reg_p1+r11+1280] 
  vmovdqa      ymm7, YMMWORD PTR [reg_p1+r11+1792]
  vmovdqa      ymm0, YMMWORD PTR [reg_p1+r11]         // Load a[j] from temporary storage 
  vmovdqa      ymm1, YMMWORD PTR [reg_p1+r11+512]       
  vmovdqa      ymm2, YMMWORD PTR [reg_p1+r11+1024]    // a[j+k]
  vmovdqa      ymm3, YMMWORD PTR [reg_p1+r11+1536] 
  vpmuldq      ymm4, ymm4, ymm12                      // W.a[j+k]
  vpmuldq      ymm5, ymm5, ymm9                     
  vpmuldq      ymm6, ymm6, ymm10                    
  vpmuldq      ymm7, ymm7, ymm11  

  vpmuldq      ymm8, ymm4, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm13, ymm5, ymm14
  vpmuludq     ymm8, ymm8, ymm15  
  vpmuludq     ymm13, ymm13, ymm15
  vpaddq       ymm4, ymm4, ymm8                     
  vpaddq       ymm5, ymm5, ymm13                     
  vpsrlq       ymm8, ymm4, 32                       
  vpsrlq       ymm13, ymm5, 32                       
  vpsubd       ymm4, ymm0, ymm8                       // a[j+k] = a[j] - temp
  vpsubd       ymm5, ymm1, ymm13
  vpaddd       ymm0, ymm0, ymm8                       // a[j] = a[j] + temp   
  vpaddd       ymm1, ymm1, ymm13         
  vpsrad       ymm8, ymm4, 31                         // If result < 0 then add q                   
  vpsrad       ymm13, ymm5, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm13, ymm13, ymm15  
  vpaddd       ymm4, ymm4, ymm8                      
  vpaddd       ymm5, ymm5, ymm13 
  vpsubd       ymm0, ymm0, ymm15                      // If result >= q then subtract q    
  vpsubd       ymm1, ymm1, ymm15   
  vpsrad       ymm8, ymm0, 31                 
  vpsrad       ymm13, ymm1, 31 
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm13, ymm13, ymm15  
  vpaddd       ymm0, ymm0, ymm8                      
  vpaddd       ymm1, ymm1, ymm13                                   

  vpmuldq      ymm8, ymm6, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm13, ymm7, ymm14
  vpmuludq     ymm8, ymm8, ymm15                             
  vpmuludq     ymm13, ymm13, ymm15
  vpaddq       ymm6, ymm6, ymm8                                    
  vpaddq       ymm7, ymm7, ymm13
  vpsrlq       ymm8, ymm6, 32                                      
  vpsrlq       ymm13, ymm7, 32
  vpsubd       ymm6, ymm2, ymm8                       // a[j+k] = a[j] - temp
  vpsubd       ymm7, ymm3, ymm13
  vpaddd       ymm2, ymm2, ymm8                       // a[j] = a[j] + temp                           
  vpaddd       ymm3, ymm3, ymm13                   
  vpsrad       ymm8, ymm6, 31                         // If result < 0 then add q                       
  vpsrad       ymm13, ymm7, 31                      
  vpand        ymm8, ymm8, ymm15                    
  vpand        ymm13, ymm13, ymm15                 
  vpaddd       ymm6, ymm6, ymm8                     
  vpaddd       ymm7, ymm7, ymm13                     
  vpsubd       ymm2, ymm2, ymm15                      // If result >= q then subtract q                      
  vpsubd       ymm3, ymm3, ymm15                    
  vpsrad       ymm8, ymm2, 31                       
  vpsrad       ymm13, ymm3, 31                        
  vpand        ymm8, ymm8, ymm15                   
  vpand        ymm13, ymm13, ymm15                 
  vpaddd       ymm2, ymm2, ymm8                      
  vpaddd       ymm3, ymm3, ymm13  
  vmovdqa      [reg_p1+r11], ymm0                     // Store in temporary storage
  vmovdqa      [reg_p1+r11+256], ymm4
  vmovdqa      [reg_p1+r11+512], ymm1
  vmovdqa      [reg_p1+r11+768], ymm5
  vmovdqa      [reg_p1+r11+1024], ymm2
  vmovdqa      [reg_p1+r11+1280], ymm6
  vmovdqa      [reg_p1+r11+1536], ymm3
  vmovdqa      [reg_p1+r11+1792], ymm7
  add          r11, 32
  dec          r10
  jnz          loop_ntt5  
  add          rax, 16 
  add          r9, 2048
  mov          r11, r9
  cmp          rax, 64
  jne          round5_loop

  xor          rax, rax               
  xor          r11, r11                            
  mov          r10, 32                               
loop_ntt678910:
// Round 6
  vbroadcastss ymm12, DWORD PTR [reg_p3+rax+124]      // Load twiddle factor W
  vmovdqa      ymm0, YMMWORD PTR [reg_p1+r11]         // Load a[j] from temporary storage 
  vmovdqa      ymm4, YMMWORD PTR [reg_p1+r11+128]     // Load a[j+k] from temporary storage   
  vmovdqa      ymm1, YMMWORD PTR [reg_p1+r11+32]       
  vmovdqa      ymm5, YMMWORD PTR [reg_p1+r11+160] 
  vmovdqa      ymm2, YMMWORD PTR [reg_p1+r11+64]        
  vmovdqa      ymm6, YMMWORD PTR [reg_p1+r11+192] 
  vmovdqa      ymm3, YMMWORD PTR [reg_p1+r11+96] 
  vmovdqa      ymm7, YMMWORD PTR [reg_p1+r11+224]
  vpmuldq      ymm4, ymm4, ymm12                      // W.a[j+k]
  vpmuldq      ymm5, ymm5, ymm12                    
  vpmuldq      ymm6, ymm6, ymm12                    
  vpmuldq      ymm7, ymm7, ymm12
  vpmuldq      ymm8, ymm4, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm9, ymm5, ymm14                    
  vpmuldq      ymm10, ymm6, ymm14 
  vpmuldq      ymm11, ymm7, ymm14 
  vpmuludq     ymm8, ymm8, ymm15   
  vpmuludq     ymm9, ymm9, ymm15  
  vpmuludq     ymm10, ymm10, ymm15                                                 
  vpmuludq     ymm11, ymm11, ymm15
  vpaddq       ymm4, ymm4, ymm8                     
  vpaddq       ymm5, ymm5, ymm9                     
  vpaddq       ymm6, ymm6, ymm10                
  vpaddq       ymm7, ymm7, ymm11  
  vpsrlq       ymm8, ymm4, 32                          
  vpsrlq       ymm9, ymm5, 32                       
  vpsrlq       ymm10, ymm6, 32                                    
  vpsrlq       ymm11, ymm7, 32
  vpsubd       ymm4, ymm0, ymm8                       // a[j+k] = a[j] - temp
  vpsubd       ymm5, ymm1, ymm9                     
  vpsubd       ymm6, ymm2, ymm10
  vpsubd       ymm7, ymm3, ymm11
  vpaddd       ymm0, ymm0, ymm8                       // a[j] = a[j] + temp
  vpaddd       ymm1, ymm1, ymm9                     
  vpaddd       ymm2, ymm2, ymm10
  vpaddd       ymm3, ymm3, ymm11   
  vpsrad       ymm8, ymm4, 31                         // If result < 0 then add q                   
  vpsrad       ymm9, ymm5, 31                       
  vpsrad       ymm10, ymm6, 31                       
  vpsrad       ymm11, ymm7, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                    
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm4, ymm4, ymm8                      
  vpaddd       ymm5, ymm5, ymm9                     
  vpaddd       ymm6, ymm6, ymm10                     
  vpaddd       ymm7, ymm7, ymm11 
  vpsubd       ymm0, ymm0, ymm15                      // If result >= q then subtract q    
  vpsubd       ymm1, ymm1, ymm15                     
  vpsubd       ymm2, ymm2, ymm15                      
  vpsubd       ymm3, ymm3, ymm15    
  vpsrad       ymm8, ymm0, 31                 
  vpsrad       ymm9, ymm1, 31                       
  vpsrad       ymm10, ymm2, 31                       
  vpsrad       ymm11, ymm3, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                   
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm0, ymm0, ymm8                      
  vpaddd       ymm1, ymm1, ymm9                     
  vpaddd       ymm2, ymm2, ymm10                      
  vpaddd       ymm3, ymm3, ymm11

  vbroadcastss ymm10, DWORD PTR [reg_p3+2*rax+252]    // Load twiddle factor W
  vbroadcastss ymm13, DWORD PTR [reg_p3+2*rax+256]
  
// Round 7 
  vpmuldq      ymm2, ymm2, ymm10                      // W.a[j+k]
  vpmuldq      ymm3, ymm3, ymm10                    
  vpmuldq      ymm6, ymm6, ymm13                    
  vpmuldq      ymm7, ymm7, ymm13
  vpmuldq      ymm8, ymm2, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm9, ymm3, ymm14                    
  vpmuldq      ymm10, ymm6, ymm14  
  vpmuldq      ymm11, ymm7, ymm14 
  vpmuludq     ymm8, ymm8, ymm15  
  vpmuludq     ymm9, ymm9, ymm15
  vpmuludq     ymm10, ymm10, ymm15                                
  vpmuludq     ymm11, ymm11, ymm15 
  vpaddq       ymm2, ymm2, ymm8                       
  vpaddq       ymm3, ymm3, ymm9                     
  vpaddq       ymm6, ymm6, ymm10                               
  vpaddq       ymm7, ymm7, ymm11   
  vpsrlq       ymm8, ymm2, 32                        
  vpsrlq       ymm9, ymm3, 32                       
  vpsrlq       ymm10, ymm6, 32                                      
  vpsrlq       ymm11, ymm7, 32 
  vpsubd       ymm2, ymm0, ymm8                       // a[j+k] = a[j] - temp 
  vpsubd       ymm3, ymm1, ymm9                    
  vpsubd       ymm6, ymm4, ymm10
  vpsubd       ymm7, ymm5, ymm11
  vpaddd       ymm0, ymm0, ymm8                       // a[j] = a[j] + temp
  vpaddd       ymm1, ymm1, ymm9                     
  vpaddd       ymm4, ymm4, ymm10
  vpaddd       ymm5, ymm5, ymm11 
  vpsrad       ymm8, ymm2, 31                         // If result < 0 then add q                   
  vpsrad       ymm9, ymm3, 31                       
  vpsrad       ymm10, ymm6, 31                       
  vpsrad       ymm11, ymm7, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                    
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm2, ymm2, ymm8                      
  vpaddd       ymm3, ymm3, ymm9                     
  vpaddd       ymm6, ymm6, ymm10                     
  vpaddd       ymm7, ymm7, ymm11 
  vpsubd       ymm0, ymm0, ymm15                      // If result >= q then subtract q    
  vpsubd       ymm1, ymm1, ymm15                     
  vpsubd       ymm4, ymm4, ymm15                      
  vpsubd       ymm5, ymm5, ymm15    
  vpsrad       ymm8, ymm0, 31                 
  vpsrad       ymm9, ymm1, 31                       
  vpsrad       ymm10, ymm4, 31                       
  vpsrad       ymm11, ymm5, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                   
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm0, ymm0, ymm8                      
  vpaddd       ymm1, ymm1, ymm9                     
  vpaddd       ymm4, ymm4, ymm10                      
  vpaddd       ymm5, ymm5, ymm11 

  vbroadcastss ymm8, DWORD PTR [reg_p3+4*rax+508]     // Load twiddle factor W
  vbroadcastss ymm9, DWORD PTR [reg_p3+4*rax+512]    
  vbroadcastss ymm10, DWORD PTR [reg_p3+4*rax+516]         
  vbroadcastss ymm13, DWORD PTR [reg_p3+4*rax+520]   
  
// Round 8
  vpmuldq      ymm1, ymm1, ymm8                       // W.a[j+k]
  vpmuldq      ymm3, ymm3, ymm9                     
  vpmuldq      ymm5, ymm5, ymm10                    
  vpmuldq      ymm7, ymm7, ymm13
  vpmuldq      ymm8, ymm1, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm9, ymm3, ymm14                    
  vpmuldq      ymm10, ymm5, ymm14                   
  vpmuldq      ymm11, ymm7, ymm14
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15
  vpaddq       ymm1, ymm1, ymm8                      
  vpaddq       ymm3, ymm3, ymm9                     
  vpaddq       ymm5, ymm5, ymm10
  vpaddq       ymm7, ymm7, ymm11 
  vpsrlq       ymm8, ymm1, 32                        
  vpsrlq       ymm9, ymm3, 32                       
  vpsrlq       ymm10, ymm5, 32                          
  vpsrlq       ymm11, ymm7, 32
  vpsubd       ymm1, ymm0, ymm8                       // a[j+k] = a[j] - temp 
  vpsubd       ymm3, ymm2, ymm9                     
  vpsubd       ymm5, ymm4, ymm10
  vpsubd       ymm7, ymm6, ymm11 
  vpaddd       ymm0, ymm0, ymm8                       // a[j] = a[j] + temp               
  vpaddd       ymm2, ymm2, ymm9                     
  vpaddd       ymm4, ymm4, ymm10
  vpaddd       ymm6, ymm6, ymm11   
  vpsrad       ymm8, ymm1, 31                         // If result < 0 then add q                   
  vpsrad       ymm9, ymm3, 31                       
  vpsrad       ymm10, ymm5, 31                       
  vpsrad       ymm11, ymm7, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                    
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm1, ymm1, ymm8                      
  vpaddd       ymm3, ymm3, ymm9                     
  vpaddd       ymm5, ymm5, ymm10                     
  vpaddd       ymm7, ymm7, ymm11 
  vpsubd       ymm0, ymm0, ymm15                      // If result >= q then subtract q    
  vpsubd       ymm2, ymm2, ymm15                     
  vpsubd       ymm4, ymm4, ymm15                      
  vpsubd       ymm6, ymm6, ymm15    
  vpsrad       ymm8, ymm0, 31                 
  vpsrad       ymm9, ymm2, 31                       
  vpsrad       ymm10, ymm4, 31                       
  vpsrad       ymm11, ymm6, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                   
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm0, ymm0, ymm8                      
  vpaddd       ymm2, ymm2, ymm9                     
  vpaddd       ymm4, ymm4, ymm10                      
  vpaddd       ymm6, ymm6, ymm11
  
  vbroadcastss ymm8, DWORD PTR [reg_p3+8*rax+1020]    // Load twiddle factor W
  vbroadcastss ymm10, DWORD PTR [reg_p3+8*rax+1024]  
  vbroadcastss ymm9, DWORD PTR [reg_p3+8*rax+1028]  
  vbroadcastss ymm13, DWORD PTR [reg_p3+8*rax+1032] 
  vbroadcastss ymm12, DWORD PTR [reg_p3+8*rax+1036]                         

// Round 9      
  vpblendd     ymm8, ymm8, ymm10, 0xF0      
  vpblendd     ymm9, ymm9, ymm13, 0xF0      
  vbroadcastss ymm10, DWORD PTR [reg_p3+8*rax+1040] 
  vbroadcastss ymm11, DWORD PTR [reg_p3+8*rax+1044]
  vbroadcastss ymm13, DWORD PTR [reg_p3+8*rax+1048] 
  vpblendd     ymm10, ymm12, ymm10, 0xF0 
  vpblendd     ymm11, ymm11, ymm13, 0xF0
  vperm2i128   ymm12, ymm0, ymm1, 0x20              
  vperm2i128   ymm0, ymm0, ymm1, 0x31               
  vperm2i128   ymm1, ymm2, ymm3, 0x20
  vperm2i128   ymm2, ymm2, ymm3, 0x31
  vperm2i128   ymm3, ymm4, ymm5, 0x20
  vperm2i128   ymm4, ymm4, ymm5, 0x31
  vperm2i128   ymm5, ymm6, ymm7, 0x20
  vperm2i128   ymm6, ymm6, ymm7, 0x31    
  vpmuldq      ymm0, ymm0, ymm8                       // W.a[j+k]
  vpmuldq      ymm2, ymm2, ymm9                     
  vpmuldq      ymm4, ymm4, ymm10                    
  vpmuldq      ymm6, ymm6, ymm11
  vpmuldq      ymm8, ymm0, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm9, ymm2, ymm14                    
  vpmuldq      ymm10, ymm4, ymm14                   
  vpmuldq      ymm11, ymm6, ymm14
  vpmuludq     ymm8, ymm8, ymm15                
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15
  vpaddq       ymm0, ymm0, ymm8                      
  vpaddq       ymm2, ymm2, ymm9                     
  vpaddq       ymm4, ymm4, ymm10
  vpaddq       ymm6, ymm6, ymm11 
  vpsrlq       ymm8, ymm0, 32                       
  vpsrlq       ymm9, ymm2, 32                       
  vpsrlq       ymm10, ymm4, 32                         
  vpsrlq       ymm11, ymm6, 32
  vpsubd       ymm0, ymm12, ymm8                      // a[j+k] = a[j] - temp
  vpsubd       ymm2, ymm1, ymm9                    
  vpsubd       ymm4, ymm3, ymm10
  vpsubd       ymm6, ymm5, ymm11
  vpaddd       ymm12, ymm12, ymm8                     // a[j] = a[j] + temp
  vpaddd       ymm1, ymm1, ymm9                     
  vpaddd       ymm3, ymm3, ymm10
  vpaddd       ymm5, ymm5, ymm11    
  vpsrad       ymm8, ymm0, 31                         // If result < 0 then add q                   
  vpsrad       ymm9, ymm2, 31                       
  vpsrad       ymm10, ymm4, 31                       
  vpsrad       ymm11, ymm6, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                    
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm0, ymm0, ymm8                      
  vpaddd       ymm2, ymm2, ymm9                     
  vpaddd       ymm4, ymm4, ymm10                     
  vpaddd       ymm6, ymm6, ymm11 
  vpsubd       ymm12, ymm12, ymm15                    // If result >= q then subtract q    
  vpsubd       ymm1, ymm1, ymm15                     
  vpsubd       ymm3, ymm3, ymm15                      
  vpsubd       ymm5, ymm5, ymm15    
  vpsrad       ymm8, ymm12, 31                 
  vpsrad       ymm9, ymm1, 31                       
  vpsrad       ymm10, ymm3, 31                       
  vpsrad       ymm11, ymm5, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                   
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm12, ymm12, ymm8                      
  vpaddd       ymm1, ymm1, ymm9                     
  vpaddd       ymm3, ymm3, ymm10                      
  vpaddd       ymm5, ymm5, ymm11

  vpshufd      ymm8, ymm12, 0x4E
  vpshufd      ymm9, ymm0, 0x4E
  vpshufd      ymm10, ymm1, 0x4E
  vpshufd      ymm13, ymm2, 0x4E                          
  
// Round 10
  lea          r9, [reg_p3+8*rax+2044]
  vpblendd     ymm12, ymm12, ymm9, 0xCC
  vpblendd     ymm0, ymm8, ymm0, 0xCC
  vpblendd     ymm1, ymm1, ymm13, 0xCC
  vpblendd     ymm2, ymm10, ymm2, 0xCC
  vpshufd      ymm8, ymm3, 0x4E
  vpshufd      ymm9, ymm4, 0x4E
  vpshufd      ymm10, ymm5, 0x4E
  vpshufd      ymm11, ymm6, 0x4E
  vpblendd     ymm4, ymm8, ymm4, 0xCC
  vpblendd     ymm3, ymm3, ymm9, 0xCC
  vpblendd     ymm6, ymm10, ymm6, 0xCC
  vpblendd     ymm5, ymm5, ymm11, 0xCC
  vpmovzxdq    ymm8, XMMWORD PTR [r9+8*rax]           // Load twiddle factor W
  vpmovzxdq    ymm9, XMMWORD PTR [r9+8*rax+16]
  vpmovzxdq    ymm10, XMMWORD PTR [r9+8*rax+32]
  vpmovzxdq    ymm11, XMMWORD PTR [r9+8*rax+48]  
  vpmuldq      ymm0, ymm0, ymm8                       // W.a[j+k]
  vpmuldq      ymm2, ymm2, ymm9                     
  vpmuldq      ymm4, ymm4, ymm10                    
  vpmuldq      ymm6, ymm6, ymm11     
  vpmuldq      ymm8, ymm0, ymm14                      // temp = reduce(W.a[j+k])
  vpmuldq      ymm9, ymm2, ymm14                    
  vpmuldq      ymm10, ymm4, ymm14                   
  vpmuldq      ymm11, ymm6, ymm14  
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15     
  vpaddq       ymm8, ymm0, ymm8                                   
  vpaddq       ymm9, ymm2, ymm9                     
  vpaddq       ymm10, ymm4, ymm10                   
  vpaddq       ymm11, ymm6, ymm11
  vpsrlq       ymm8, ymm8, 32 
  vpsrlq       ymm9, ymm9, 32  
  vpsrlq       ymm10, ymm10, 32  
  vpsrlq       ymm11, ymm11, 32
  vpsubd       ymm0, ymm12, ymm8                      // a[j+k] = a[j] - temp                      
  vpsubd       ymm2, ymm1, ymm9                    
  vpsubd       ymm4, ymm3, ymm10
  vpsubd       ymm6, ymm5, ymm11                      
  vpaddd       ymm7, ymm5, ymm11                      // a[j] = a[j] + temp
  vpaddd       ymm5, ymm3, ymm10
  vpaddd       ymm3, ymm1, ymm9                     
  vpaddd       ymm1, ymm12, ymm8      
  vpsrad       ymm8, ymm0, 31                         // If result < 0 then add q                   
  vpsrad       ymm9, ymm2, 31                       
  vpsrad       ymm10, ymm4, 31                       
  vpsrad       ymm11, ymm6, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                    
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm0, ymm0, ymm8                      
  vpaddd       ymm2, ymm2, ymm9                     
  vpaddd       ymm4, ymm4, ymm10                     
  vpaddd       ymm6, ymm6, ymm11 
  vpsubd       ymm1, ymm1, ymm15                    // If result >= q then subtract q    
  vpsubd       ymm3, ymm3, ymm15                     
  vpsubd       ymm5, ymm5, ymm15                      
  vpsubd       ymm7, ymm7, ymm15    
  vpsrad       ymm8, ymm1, 31                 
  vpsrad       ymm9, ymm3, 31                       
  vpsrad       ymm10, ymm5, 31                       
  vpsrad       ymm11, ymm7, 31   
  vpand        ymm8, ymm8, ymm15                                             
  vpand        ymm9, ymm9, ymm15                       
  vpand        ymm10, ymm10, ymm15                   
  vpand        ymm11, ymm11, ymm15
  vpaddd       ymm1, ymm1, ymm8                      
  vpaddd       ymm3, ymm3, ymm9                     
  vpaddd       ymm5, ymm5, ymm10                      
  vpaddd       ymm7, ymm7, ymm11 
  vmovdqa      [reg_p1+r11], ymm0                     // Store result
  vmovdqa      [reg_p1+r11+32], ymm2
  vmovdqa      [reg_p1+r11+64], ymm4
  vmovdqa      [reg_p1+r11+96], ymm6
  vmovdqa      [reg_p1+r11+128], ymm1                      
  vmovdqa      [reg_p1+r11+160], ymm3
  vmovdqa      [reg_p1+r11+192], ymm5
  vmovdqa      [reg_p1+r11+224], ymm7
  add          rax, 4
  add          r11, 256
  dec          r10
  jnz          loop_ntt678910
ret   
                  

//**********************************************************************************
//  Pointwise multiplication, case N = 1024
//  Operation: c [reg_p1] <- a [reg_p2] x b [reg_p3].
//  Input b and output c are in extended form. 
//********************************************************************************** 
.global oqs_sig_qteslapi_poly_pmul_asm
oqs_sig_qteslapi_poly_pmul_asm:
  xor          rax, rax               
  xor          r11, r11 
  vmovdqa      ymm15, YMMWORD PTR [oqs_sig_qteslapi_PARAM_Qx4+rip]
  vmovdqa      ymm14, YMMWORD PTR [oqs_sig_qteslapi_PARAM_QINVx4+rip]                           
  mov          r10, 32                               
loop_pmul:   
  vmovdqa      ymm0, [reg_p3+r11]                     // Load a[j]
  vmovdqa      ymm2, [reg_p3+r11+32]
  vmovdqa      ymm4, [reg_p3+r11+64]
  vmovdqa      ymm6, [reg_p3+r11+96]
  vmovdqa      ymm1, [reg_p3+r11+128]                      
  vmovdqa      ymm3, [reg_p3+r11+160]
  vmovdqa      ymm5, [reg_p3+r11+192]
  vmovdqa      ymm7, [reg_p3+r11+224]                                
  vmovdqa      ymm8, YMMWORD PTR [reg_p2]             // Load b[j]
  vmovdqa      ymm9, YMMWORD PTR [reg_p2+32]
  vmovdqa      ymm10, YMMWORD PTR [reg_p2+64]
  vmovdqa      ymm11, YMMWORD PTR [reg_p2+96]
  vpmuldq      ymm1, ymm1, ymm8                       // a.b
  vpmuldq      ymm3, ymm3, ymm9 
  vpmuldq      ymm5, ymm5, ymm10
  vpmuldq      ymm7, ymm7, ymm11
  vpmuldq      ymm12, ymm1, ymm14                     // temp = reduce(a.b) 
  vpmuldq      ymm13, ymm3, ymm14                     
  vpmuludq     ymm12, ymm12, ymm15     
  vpmuludq     ymm13, ymm13, ymm15   
  vpaddq       ymm1, ymm1, ymm12                    
  vpaddq       ymm3, ymm3, ymm13                   
  vpmuldq      ymm12, ymm5, ymm14                     
  vpmuldq      ymm13, ymm7, ymm14                        
  vpmuludq     ymm12, ymm12, ymm15   
  vpmuludq     ymm13, ymm13, ymm15      
  vpaddq       ymm5, ymm5, ymm12                    
  vpaddq       ymm7, ymm7, ymm13
  vpsrlq       ymm1, ymm1, 32                                     
  vpsrlq       ymm3, ymm3, 32                       
  vpsrlq       ymm5, ymm5, 32                   
  vpsrlq       ymm7, ymm7, 32
    
  vpsrlq       ymm8, ymm8, 32                                       
  vpsrlq       ymm9, ymm9, 32                       
  vpsrlq       ymm10, ymm10, 32                   
  vpsrlq       ymm11, ymm11, 32
  vpmuldq      ymm0, ymm0, ymm8                       // a.b
  vpmuldq      ymm2, ymm2, ymm9 
  vpmuldq      ymm4, ymm4, ymm10
  vpmuldq      ymm6, ymm6, ymm11
  vpmuldq      ymm8, ymm0, ymm14                      // temp = reduce(a.b) 
  vpmuldq      ymm9, ymm2, ymm14                      
  vpmuldq      ymm10, ymm4, ymm14                     
  vpmuldq      ymm11, ymm6, ymm14                      
  vpmuludq     ymm8, ymm8, ymm15       
  vpmuludq     ymm9, ymm9, ymm15        
  vpmuludq     ymm10, ymm10, ymm15       
  vpmuludq     ymm11, ymm11, ymm15       
  vpaddq       ymm0, ymm0, ymm8                   
  vpaddq       ymm2, ymm2, ymm9                   
  vpaddq       ymm4, ymm4, ymm10                    
  vpaddq       ymm6, ymm6, ymm11                    
  vpsrlq       ymm0, ymm0, 32 
  vpsrlq       ymm2, ymm2, 32  
  vpsrlq       ymm4, ymm4, 32  
  vpsrlq       ymm6, ymm6, 32 
  vmovdqa      [reg_p1+r11], ymm1                     // Store result
  vmovdqa      [reg_p1+r11+32], ymm3
  vmovdqa      [reg_p1+r11+64], ymm5
  vmovdqa      [reg_p1+r11+96], ymm7
  vmovdqa      [reg_p1+r11+128], ymm0                      
  vmovdqa      [reg_p1+r11+160], ymm2
  vmovdqa      [reg_p1+r11+192], ymm4
  vmovdqa      [reg_p1+r11+224], ymm6
  add          rax, 4
  add          r11, 256
  add          reg_p2, 128 
  dec          r10
  jnz          loop_pmul 
ret     
                  

//**********************************************************************************
//  Inverse NTT, case N = 1024
//  Operation: c [reg_p1] <- INTT(a) [reg_p2], 
//             [reg_p3] points to table with the NTT constants.
//  Input a is in extended form. 
//********************************************************************************** 
.global oqs_sig_qteslapi_poly_intt_asm
oqs_sig_qteslapi_poly_intt_asm:
  xor          rax, rax               
  xor          r11, r11 
  vmovdqa      ymm15, YMMWORD PTR [oqs_sig_qteslapi_PARAM_Qx4+rip]
  vmovdqa      ymm14, YMMWORD PTR [oqs_sig_qteslapi_PARAM_QINVx4+rip]                           
  mov          r10, 32                               
loop_intt12345:                           
// Round 1         
  vmovdqa      ymm1, [reg_p2+r11]                      // Load a[j]
  vmovdqa      ymm3, [reg_p2+r11+32] 
  vmovdqa      ymm5, [reg_p2+r11+64] 
  vmovdqa      ymm7, [reg_p2+r11+96] 
  vmovdqa      ymm0, [reg_p2+r11+128]                       
  vmovdqa      ymm2, [reg_p2+r11+160] 
  vmovdqa      ymm4, [reg_p2+r11+192] 
  vmovdqa      ymm6, [reg_p2+r11+224]              
  lea          r9, [reg_p3+8*rax]                
  vpsubd       ymm8, ymm1, ymm0                       // temp - a[j+k]
  vpsubd       ymm9, ymm3, ymm2
  vpsubd       ymm10, ymm5, ymm4
  vpsubd       ymm11, ymm7, ymm6
  vpaddd       ymm1, ymm1, ymm0                       // a[j] = temp + a[j+k] 
  vpaddd       ymm3, ymm3, ymm2                  
  vpaddd       ymm5, ymm5, ymm4                    
  vpaddd       ymm7, ymm7, ymm6       
  vmovdqa      ymm13, YMMWORD PTR [oqs_sig_qteslapi_PARAM_BARRx4+rip]  // Load PARAM_BARR_MULT
  vpmuldq      ymm0, ymm1, ymm13                      // a[j] = barr_reduce(a[j])
  vpmuldq      ymm2, ymm3, ymm13                    
  vpmuldq      ymm4, ymm5, ymm13                   
  vpmuldq      ymm6, ymm7, ymm13
  vpsrlq       ymm0, ymm0, PARAM_BARR_DIV                                        
  vpsrlq       ymm2, ymm2, PARAM_BARR_DIV                       
  vpsrlq       ymm4, ymm4, PARAM_BARR_DIV                   
  vpsrlq       ymm6, ymm6, PARAM_BARR_DIV  
  vpmuludq     ymm0, ymm0, ymm15                   
  vpmuludq     ymm2, ymm2, ymm15                   
  vpmuludq     ymm4, ymm4, ymm15                   
  vpmuludq     ymm6, ymm6, ymm15  
  vpsubd       ymm1, ymm1, ymm0                      
  vpsubd       ymm3, ymm3, ymm2
  vpsubd       ymm5, ymm5, ymm4
  vpsubd       ymm7, ymm7, ymm6              
  vpmovzxdq    ymm0, XMMWORD PTR [r9+8*rax]           // Load twiddle factor W
  vpmovzxdq    ymm2, XMMWORD PTR [r9+8*rax+16]
  vpmovzxdq    ymm4, XMMWORD PTR [r9+8*rax+32]
  vpmovzxdq    ymm6, XMMWORD PTR [r9+8*rax+48]  
  vpmuldq      ymm0, ymm0, ymm8                       // W.(temp - a[j+k])  
  vpmuldq      ymm2, ymm2, ymm9 
  vpmuldq      ymm4, ymm4, ymm10 
  vpmuldq      ymm6, ymm6, ymm11        
  vpmuldq      ymm8, ymm0, ymm14                      // temp = reduce(W.X)
  vpmuldq      ymm9, ymm2, ymm14                    
  vpmuldq      ymm10, ymm4, ymm14                   
  vpmuldq      ymm11, ymm6, ymm14  
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15     
  vpaddq       ymm0, ymm0, ymm8                                   
  vpaddq       ymm2, ymm2, ymm9                     
  vpaddq       ymm4, ymm4, ymm10                   
  vpaddq       ymm6, ymm6, ymm11
  vpsrlq       ymm0, ymm0, 32                         // a[j+k]                
  vpsrlq       ymm2, ymm2, 32                       
  vpsrlq       ymm4, ymm4, 32                   
  vpsrlq       ymm6, ymm6, 32   
  
// Round 2
  vpshufd      ymm8, ymm1, 0xC6
  vpshufd      ymm9, ymm0, 0xC6
  vpshufd      ymm10, ymm3, 0xC6
  vpshufd      ymm11, ymm2, 0xC6
  vpblendd     ymm1, ymm1, ymm9, 0xCC
  vpblendd     ymm8, ymm8, ymm0, 0xCC
  vpblendd     ymm3, ymm3, ymm11, 0xCC
  vpblendd     ymm9, ymm10, ymm2, 0xCC
  vpshufd      ymm0, ymm5, 0xC6
  vpshufd      ymm10, ymm4, 0xC6
  vpshufd      ymm2, ymm7, 0xC6
  vpshufd      ymm11, ymm6, 0xC6
  vpblendd     ymm5, ymm5, ymm10, 0xCC
  vpblendd     ymm10, ymm0, ymm4, 0xCC
  vpblendd     ymm7, ymm7, ymm11, 0xCC
  vpblendd     ymm11, ymm2, ymm6, 0xCC  
  vpaddd       ymm0, ymm1, ymm8                       // a[j] = temp + a[j+k] 
  vpaddd       ymm2, ymm3, ymm9                  
  vpaddd       ymm4, ymm5, ymm10                   
  vpaddd       ymm6, ymm7, ymm11                   
  vpsubd       ymm1, ymm1, ymm8                       // temp - a[j+k]
  vpsubd       ymm3, ymm3, ymm9
  vpsubd       ymm5, ymm5, ymm10
  vpsubd       ymm7, ymm7, ymm11
  vpmuldq      ymm8, ymm0, ymm13                      // a[j] = barr_reduce(a[j])
  vpmuldq      ymm9, ymm2, ymm13                    
  vpmuldq      ymm10, ymm4, ymm13                   
  vpmuldq      ymm11, ymm6, ymm13
  vpsrlq       ymm8, ymm8, PARAM_BARR_DIV                                        
  vpsrlq       ymm9, ymm9, PARAM_BARR_DIV                       
  vpsrlq       ymm10, ymm10, PARAM_BARR_DIV                   
  vpsrlq       ymm11, ymm11, PARAM_BARR_DIV  
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15  
  vpsubd       ymm0, ymm0, ymm8                      
  vpsubd       ymm2, ymm2, ymm9
  vpsubd       ymm4, ymm4, ymm10
  vpsubd       ymm6, ymm6, ymm11                 
  vbroadcastss ymm8, DWORD PTR [reg_p3+8*rax+2048]    // Load twiddle factor W
  vbroadcastss ymm10, DWORD PTR [reg_p3+8*rax+2052]  
  vbroadcastss ymm9, DWORD PTR [reg_p3+8*rax+2056]    
  vbroadcastss ymm11, DWORD PTR [reg_p3+8*rax+2060] 
  vbroadcastss ymm12, DWORD PTR [reg_p3+8*rax+2064] 
  vbroadcastss ymm13, DWORD PTR [reg_p3+8*rax+2068]   
  vpblendd     ymm8, ymm8, ymm10, 0xF0
  vpblendd     ymm9, ymm9, ymm11, 0xF0
  vbroadcastss ymm11, DWORD PTR [reg_p3+8*rax+2072]   
  vpblendd     ymm10, ymm12, ymm13, 0xF0      
  vbroadcastss ymm12, DWORD PTR [reg_p3+8*rax+2076] 
  vpblendd     ymm11, ymm11, ymm12, 0xF0  
  vpmuldq      ymm1, ymm1, ymm8                       // W.(temp - a[j+k])  
  vpmuldq      ymm3, ymm3, ymm9 
  vpmuldq      ymm5, ymm5, ymm10 
  vpmuldq      ymm7, ymm7, ymm11        
  vpmuldq      ymm8, ymm1, ymm14                      // temp = reduce(W.X)
  vpmuldq      ymm9, ymm3, ymm14                    
  vpmuldq      ymm10, ymm5, ymm14                   
  vpmuldq      ymm11, ymm7, ymm14  
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15     
  vpaddq       ymm1, ymm1, ymm8                                  
  vpaddq       ymm3, ymm3, ymm9                     
  vpaddq       ymm5, ymm5, ymm10                   
  vpaddq       ymm7, ymm7, ymm11
  vpsrlq       ymm1, ymm1, 32                                       
  vpsrlq       ymm3, ymm3, 32                      
  vpsrlq       ymm5, ymm5, 32                   
  vpsrlq       ymm7, ymm7, 32   
  
// Round 3
  vperm2i128   ymm12, ymm0, ymm1, 0x20              
  vperm2i128   ymm0, ymm0, ymm1, 0x31               
  vperm2i128   ymm1, ymm2, ymm3, 0x20
  vperm2i128   ymm2, ymm2, ymm3, 0x31
  vperm2i128   ymm3, ymm4, ymm5, 0x20
  vperm2i128   ymm4, ymm4, ymm5, 0x31
  vperm2i128   ymm5, ymm6, ymm7, 0x20
  vperm2i128   ymm6, ymm6, ymm7, 0x31  
  vpsubd       ymm8, ymm12, ymm0                      // temp - a[j+k]
  vpsubd       ymm9, ymm1, ymm2
  vpsubd       ymm10, ymm3, ymm4
  vpsubd       ymm11, ymm5, ymm6  
  vpaddd       ymm12, ymm12, ymm0                     // a[j] = temp + a[j+k] 
  vpaddd       ymm1, ymm1, ymm2                  
  vpaddd       ymm3, ymm3, ymm4                   
  vpaddd       ymm5, ymm5, ymm6       
  vmovdqa      ymm13, YMMWORD PTR [oqs_sig_qteslapi_PARAM_BARRx4+rip]  // Load PARAM_BARR_MULT
  vpmuldq      ymm0, ymm12, ymm13                     // a[j] = barr_reduce(a[j])
  vpmuldq      ymm2, ymm1, ymm13                    
  vpmuldq      ymm4, ymm3, ymm13                   
  vpmuldq      ymm6, ymm5, ymm13
  vpsrlq       ymm0, ymm0, PARAM_BARR_DIV                                        
  vpsrlq       ymm2, ymm2, PARAM_BARR_DIV                       
  vpsrlq       ymm4, ymm4, PARAM_BARR_DIV                   
  vpsrlq       ymm6, ymm6, PARAM_BARR_DIV  
  vpmuludq     ymm0, ymm0, ymm15                   
  vpmuludq     ymm2, ymm2, ymm15                   
  vpmuludq     ymm4, ymm4, ymm15                   
  vpmuludq     ymm6, ymm6, ymm15  
  vpsubd       ymm12, ymm12, ymm0                      
  vpsubd       ymm1, ymm1, ymm2
  vpsubd       ymm3, ymm3, ymm4
  vpsubd       ymm5, ymm5, ymm6            
  vbroadcastss ymm0, DWORD PTR [reg_p3+4*rax+3072]    // Load twiddle factor W
  vbroadcastss ymm2, DWORD PTR [reg_p3+4*rax+3076]   
  vbroadcastss ymm4, DWORD PTR [reg_p3+4*rax+3080]         
  vbroadcastss ymm6, DWORD PTR [reg_p3+4*rax+3084]
  vpmuldq      ymm0, ymm0, ymm8                       // W.(temp - a[j+k])  
  vpmuldq      ymm2, ymm2, ymm9 
  vpmuldq      ymm4, ymm4, ymm10 
  vpmuldq      ymm6, ymm6, ymm11        
  vpmuldq      ymm8, ymm0, ymm14                      // temp = reduce(W.X)
  vpmuldq      ymm9, ymm2, ymm14                    
  vpmuldq      ymm10, ymm4, ymm14                   
  vpmuldq      ymm11, ymm6, ymm14  
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15     
  vpaddq       ymm0, ymm0, ymm8                                   
  vpaddq       ymm2, ymm2, ymm9                     
  vpaddq       ymm4, ymm4, ymm10                   
  vpaddq       ymm6, ymm6, ymm11
  vpsrlq       ymm0, ymm0, 32                                        
  vpsrlq       ymm2, ymm2, 32                       
  vpsrlq       ymm4, ymm4, 32                   
  vpsrlq       ymm6, ymm6, 32   
  
// Round 4  
  vpsubd       ymm8, ymm12, ymm1                      // temp - a[j+k]
  vpsubd       ymm9, ymm0, ymm2
  vpsubd       ymm10, ymm3, ymm5
  vpsubd       ymm11, ymm4, ymm6 
  vpaddd       ymm12, ymm12, ymm1                     // a[j] = temp + a[j+k] 
  vpaddd       ymm0, ymm0, ymm2                  
  vpaddd       ymm3, ymm3, ymm5                    
  vpaddd       ymm4, ymm4, ymm6  
  vpmuldq      ymm1, ymm12, ymm13                      // a[j] = barr_reduce(a[j])
  vpmuldq      ymm2, ymm0, ymm13                    
  vpmuldq      ymm5, ymm3, ymm13                   
  vpmuldq      ymm6, ymm4, ymm13
  vpsrlq       ymm1, ymm1, PARAM_BARR_DIV                                        
  vpsrlq       ymm2, ymm2, PARAM_BARR_DIV                       
  vpsrlq       ymm5, ymm5, PARAM_BARR_DIV                   
  vpsrlq       ymm6, ymm6, PARAM_BARR_DIV  
  vpmuludq     ymm1, ymm1, ymm15                   
  vpmuludq     ymm2, ymm2, ymm15                   
  vpmuludq     ymm5, ymm5, ymm15                   
  vpmuludq     ymm6, ymm6, ymm15  
  vpsubd       ymm12, ymm12, ymm1                      
  vpsubd       ymm0, ymm0, ymm2
  vpsubd       ymm3, ymm3, ymm5
  vpsubd       ymm4, ymm4, ymm6                             
  vbroadcastss ymm2, DWORD PTR [reg_p3+2*rax+3584]    // Load twiddle factor W
  vbroadcastss ymm6, DWORD PTR [reg_p3+2*rax+3588]  
  vpmuldq      ymm1, ymm8, ymm2                       // W.(temp - a[j+k])  
  vpmuldq      ymm2, ymm9, ymm2
  vpmuldq      ymm5, ymm10, ymm6
  vpmuldq      ymm6, ymm11, ymm6        
  vpmuldq      ymm8, ymm1, ymm14                      // temp = reduce(W.X)
  vpmuldq      ymm9, ymm2, ymm14                    
  vpmuldq      ymm10, ymm5, ymm14                   
  vpmuldq      ymm11, ymm6, ymm14  
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15     
  vpaddq       ymm1, ymm1, ymm8                                   
  vpaddq       ymm2, ymm2, ymm9                     
  vpaddq       ymm5, ymm5, ymm10                   
  vpaddq       ymm6, ymm6, ymm11
  vpsrlq       ymm1, ymm1, 32                                       
  vpsrlq       ymm2, ymm2, 32                       
  vpsrlq       ymm5, ymm5, 32                   
  vpsrlq       ymm6, ymm6, 32   
  
// Round 5  
  vbroadcastss ymm7, DWORD PTR [reg_p3+rax+3840]      // Load twiddle factor W
  vpsubd       ymm8, ymm12, ymm3                      // temp - a[j+k]
  vpsubd       ymm9, ymm0, ymm4
  vpsubd       ymm10, ymm1, ymm5
  vpsubd       ymm11, ymm2, ymm6
  vpaddd       ymm12, ymm12, ymm3                     // a[j] = temp + a[j+k] 
  vpaddd       ymm0, ymm0, ymm4                  
  vpaddd       ymm1, ymm1, ymm5                    
  vpaddd       ymm2, ymm2, ymm6
  vpmuldq      ymm3, ymm12, ymm13                     // a[j] = barr_reduce(a[j])
  vpmuldq      ymm4, ymm0, ymm13                    
  vpmuldq      ymm5, ymm1, ymm13                   
  vpmuldq      ymm6, ymm2, ymm13
  vpsrlq       ymm3, ymm3, PARAM_BARR_DIV                                        
  vpsrlq       ymm4, ymm4, PARAM_BARR_DIV                       
  vpsrlq       ymm5, ymm5, PARAM_BARR_DIV                   
  vpsrlq       ymm6, ymm6, PARAM_BARR_DIV  
  vpmuludq     ymm3, ymm3, ymm15                   
  vpmuludq     ymm4, ymm4, ymm15                   
  vpmuludq     ymm5, ymm5, ymm15                   
  vpmuludq     ymm6, ymm6, ymm15  
  vpsubd       ymm12, ymm12, ymm3                      
  vpsubd       ymm0, ymm0, ymm4
  vpsubd       ymm1, ymm1, ymm5
  vpsubd       ymm2, ymm2, ymm6                    
  vpmuldq      ymm3, ymm8, ymm7                       // W.(temp - a[j+k])  
  vpmuldq      ymm4, ymm9, ymm7
  vpmuldq      ymm5, ymm10, ymm7 
  vpmuldq      ymm6, ymm11, ymm7
  vpmuldq      ymm8, ymm3, ymm14                      // temp = reduce(W.X)
  vpmuldq      ymm9, ymm4, ymm14                    
  vpmuldq      ymm10, ymm5, ymm14                   
  vpmuldq      ymm11, ymm6, ymm14  
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15     
  vpaddq       ymm3, ymm3, ymm8                                   
  vpaddq       ymm4, ymm4, ymm9                     
  vpaddq       ymm5, ymm5, ymm10                   
  vpaddq       ymm6, ymm6, ymm11
  vpsrlq       ymm3, ymm3, 32                                        
  vpsrlq       ymm4, ymm4, 32                       
  vpsrlq       ymm5, ymm5, 32                   
  vpsrlq       ymm6, ymm6, 32
  vmovdqa      [reg_p2+r11], ymm12                    // Store in temporary storage
  vmovdqa      [reg_p2+r11+32], ymm0
  vmovdqa      [reg_p2+r11+64], ymm1
  vmovdqa      [reg_p2+r11+96], ymm2
  vmovdqa      [reg_p2+r11+128], ymm3                      
  vmovdqa      [reg_p2+r11+160], ymm4
  vmovdqa      [reg_p2+r11+192], ymm5
  vmovdqa      [reg_p2+r11+224], ymm6
  add          rax, 4
  add          r11, 256
  dec          r10
  jnz          loop_intt12345  
                                                                                    
  xor          r9, r9 
  xor          r11, r11 
  xor          rax, rax
round6_loop:
  vbroadcastss ymm9, DWORD PTR [reg_p3+rax+3968]      // Load twiddle factor W
  vbroadcastss ymm10, DWORD PTR [reg_p3+rax+3972]                   
  vbroadcastss ymm11, DWORD PTR [reg_p3+rax+3976]         
  vbroadcastss ymm12, DWORD PTR [reg_p3+rax+3980]
  mov          r10, 8                                
loop_intt6:
// Round 6 
  vmovdqa      ymm0, YMMWORD PTR [reg_p2+r11]         // a[j]   
  vmovdqa      ymm4, YMMWORD PTR [reg_p2+r11+256]      
  vmovdqa      ymm1, YMMWORD PTR [reg_p2+r11+512]     
  vmovdqa      ymm5, YMMWORD PTR [reg_p2+r11+768] 
  vmovdqa      ymm2, YMMWORD PTR [reg_p2+r11+1024]    // a[j+k] 
  vmovdqa      ymm6, YMMWORD PTR [reg_p2+r11+1280] 
  vmovdqa      ymm3, YMMWORD PTR [reg_p2+r11+1536] 
  vmovdqa      ymm7, YMMWORD PTR [reg_p2+r11+1792]
  vpsubd       ymm8, ymm0, ymm4                       // temp - a[j+k]
  vpaddd       ymm0, ymm0, ymm4                       // a[j] = temp + a[j+k]
  vpmuldq      ymm4, ymm0, ymm13                      // a[j] = barr_reduce(a[j])
  vpsrlq       ymm4, ymm4, PARAM_BARR_DIV   
  vpmuludq     ymm4, ymm4, ymm15   
  vpsubd       ymm0, ymm0, ymm4  
  vpmuldq      ymm4, ymm8, ymm9                       // W.(temp - a[j+k]) 
  vpmuldq      ymm8, ymm4, ymm14                      // temp = reduce(W.X) 
  vpmuludq     ymm8, ymm8, ymm15 
  vpaddq       ymm4, ymm4, ymm8                    
  vpsrlq       ymm4, ymm4, 32                                
  
  vpsubd       ymm8, ymm1, ymm5
  vpaddd       ymm1, ymm1, ymm5  
  vpmuldq      ymm5, ymm1, ymm13                                      
  vpsrlq       ymm5, ymm5, PARAM_BARR_DIV                   
  vpmuludq     ymm5, ymm5, ymm15                      
  vpsubd       ymm1, ymm1, ymm5                   
  vpmuldq      ymm5, ymm8, ymm10
  vpmuldq      ymm8, ymm5, ymm14                    
  vpmuludq     ymm8, ymm8, ymm15  
  vpaddq       ymm5, ymm5, ymm8                     
  vpsrlq       ymm5, ymm5, 32                       
  
  vpsubd       ymm8, ymm2, ymm6
  vpaddd       ymm2, ymm2, ymm6  
  vpmuldq      ymm6, ymm2, ymm13                        
  vpsrlq       ymm6, ymm6, PARAM_BARR_DIV                     
  vpmuludq     ymm6, ymm6, ymm15    
  vpsubd       ymm2, ymm2, ymm6             
  vpmuldq      ymm6, ymm8, ymm11
  vpmuldq      ymm8, ymm6, ymm14    
  vpmuludq     ymm8, ymm8, ymm15 
  vpaddq       ymm6, ymm6, ymm8 
  vpsrlq       ymm6, ymm6, 32     
  
  vpsubd       ymm8, ymm3, ymm7  
  vpaddd       ymm3, ymm3, ymm7            
  vpmuldq      ymm7, ymm3, ymm13                  
  vpsrlq       ymm7, ymm7, PARAM_BARR_DIV                    
  vpmuludq     ymm7, ymm7, ymm15 
  vpsubd       ymm3, ymm3, ymm7                   
  vpmuldq      ymm7, ymm8, ymm12                         
  vpmuldq      ymm8, ymm7, ymm14                                                   
  vpmuludq     ymm8, ymm8, ymm15                                  
  vpaddq       ymm7, ymm7, ymm8                              
  vpsrlq       ymm7, ymm7, 32
  vmovdqa      [reg_p2+r11], ymm0                     // Store in temporary storage
  vmovdqa      [reg_p2+r11+256], ymm4
  vmovdqa      [reg_p2+r11+512], ymm1
  vmovdqa      [reg_p2+r11+768], ymm5
  vmovdqa      [reg_p2+r11+1024], ymm2
  vmovdqa      [reg_p2+r11+1280], ymm6
  vmovdqa      [reg_p2+r11+1536], ymm3
  vmovdqa      [reg_p2+r11+1792], ymm7
  add          r11, 32
  dec          r10
  jnz          loop_intt6  
  add          rax, 16  
  add          r9, 2048
  mov          r11, r9
  cmp          rax, 64
  jne          round6_loop

  xor          r11, r11 
  xor          rax, rax
round7_loop:
  vbroadcastss ymm9, DWORD PTR [reg_p3+rax+4032]      // Load twiddle factor W
  vbroadcastss ymm10, DWORD PTR [reg_p3+rax+4036]                   
  vbroadcastss ymm11, DWORD PTR [reg_p3+rax+4040]         
  vbroadcastss ymm12, DWORD PTR [reg_p3+rax+4044] 
  mov          r10, 16                                
loop_intt7:
// Round 7 
  vmovdqa      ymm0, YMMWORD PTR [reg_p2+r11]         // a[j]   
  vmovdqa      ymm4, YMMWORD PTR [reg_p2+r11+512]      
  vmovdqa      ymm1, YMMWORD PTR [reg_p2+r11+1024]     
  vmovdqa      ymm5, YMMWORD PTR [reg_p2+r11+1536] 
  vmovdqa      ymm2, YMMWORD PTR [reg_p2+r11+2048]    // a[j+k] 
  vmovdqa      ymm6, YMMWORD PTR [reg_p2+r11+2560] 
  vmovdqa      ymm3, YMMWORD PTR [reg_p2+r11+3072] 
  vmovdqa      ymm7, YMMWORD PTR [reg_p2+r11+3584]
  vpsubd       ymm8, ymm0, ymm4                       // temp - a[j+k]
  vpaddd       ymm0, ymm0, ymm4                       // a[j] = temp + a[j+k]
  vpmuldq      ymm4, ymm0, ymm13                      // a[j] = barr_reduce(a[j])
  vpsrlq       ymm4, ymm4, PARAM_BARR_DIV   
  vpmuludq     ymm4, ymm4, ymm15   
  vpsubd       ymm0, ymm0, ymm4  
  vpmuldq      ymm4, ymm8, ymm9                       // W.(temp - a[j+k]) 
  vpmuldq      ymm8, ymm4, ymm14                      // temp = reduce(W.X) 
  vpmuludq     ymm8, ymm8, ymm15 
  vpaddq       ymm4, ymm4, ymm8                    
  vpsrlq       ymm4, ymm4, 32                                
  
  vpsubd       ymm8, ymm1, ymm5
  vpaddd       ymm1, ymm1, ymm5  
  vpmuldq      ymm5, ymm1, ymm13                                      
  vpsrlq       ymm5, ymm5, PARAM_BARR_DIV                   
  vpmuludq     ymm5, ymm5, ymm15                      
  vpsubd       ymm1, ymm1, ymm5                   
  vpmuldq      ymm5, ymm8, ymm10
  vpmuldq      ymm8, ymm5, ymm14                    
  vpmuludq     ymm8, ymm8, ymm15  
  vpaddq       ymm5, ymm5, ymm8                     
  vpsrlq       ymm5, ymm5, 32                       
  
  vpsubd       ymm8, ymm2, ymm6
  vpaddd       ymm2, ymm2, ymm6  
  vpmuldq      ymm6, ymm2, ymm13                        
  vpsrlq       ymm6, ymm6, PARAM_BARR_DIV                     
  vpmuludq     ymm6, ymm6, ymm15    
  vpsubd       ymm2, ymm2, ymm6             
  vpmuldq      ymm6, ymm8, ymm11
  vpmuldq      ymm8, ymm6, ymm14    
  vpmuludq     ymm8, ymm8, ymm15 
  vpaddq       ymm6, ymm6, ymm8 
  vpsrlq       ymm6, ymm6, 32     
  
  vpsubd       ymm8, ymm3, ymm7  
  vpaddd       ymm3, ymm3, ymm7            
  vpmuldq      ymm7, ymm3, ymm13                  
  vpsrlq       ymm7, ymm7, PARAM_BARR_DIV                    
  vpmuludq     ymm7, ymm7, ymm15 
  vpsubd       ymm3, ymm3, ymm7                   
  vpmuldq      ymm7, ymm8, ymm12                         
  vpmuldq      ymm8, ymm7, ymm14                                                   
  vpmuludq     ymm8, ymm8, ymm15                                  
  vpaddq       ymm7, ymm7, ymm8                              
  vpsrlq       ymm7, ymm7, 32
  vmovdqa      [reg_p2+r11], ymm0                     // Store in temporary storage
  vmovdqa      [reg_p2+r11+512], ymm4
  vmovdqa      [reg_p2+r11+1024], ymm1
  vmovdqa      [reg_p2+r11+1536], ymm5
  vmovdqa      [reg_p2+r11+2048], ymm2
  vmovdqa      [reg_p2+r11+2560], ymm6
  vmovdqa      [reg_p2+r11+3072], ymm3
  vmovdqa      [reg_p2+r11+3584], ymm7
  add          r11, 32
  dec          r10
  jnz          loop_intt7  
  add          rax, 16 
  mov          r11, 4096
  cmp          rax, 32
  jne          round7_loop
    
  xor          rax, rax
  xor          r11, r11
  vmovdqa      ymm12, YMMWORD PTR [oqs_sig_qteslapi_PERM_MASK+rip]
  mov          r10, 32                               
loop_intt8910:
// Round 8
  vmovdqa      ymm0, YMMWORD PTR [reg_p2+r11]         // a[j]   
  vmovdqa      ymm4, YMMWORD PTR [reg_p2+r11+1024]       
  vmovdqa      ymm1, YMMWORD PTR [reg_p2+r11+2048]       
  vmovdqa      ymm5, YMMWORD PTR [reg_p2+r11+3072] 
  vmovdqa      ymm2, YMMWORD PTR [reg_p2+r11+4096]    // a[j+k] 
  vmovdqa      ymm6, YMMWORD PTR [reg_p2+r11+5120] 
  vmovdqa      ymm3, YMMWORD PTR [reg_p2+r11+6144] 
  vmovdqa      ymm7, YMMWORD PTR [reg_p2+r11+7168]  
  vpsubd       ymm8, ymm0, ymm4                       // temp - a[j+k]
  vpsubd       ymm9, ymm1, ymm5
  vpsubd       ymm10, ymm2, ymm6
  vpsubd       ymm11, ymm3, ymm7
  vpaddd       ymm0, ymm0, ymm4                       // a[j] = temp + a[j+k] 
  vpaddd       ymm1, ymm1, ymm5                  
  vpaddd       ymm2, ymm2, ymm6                    
  vpaddd       ymm3, ymm3, ymm7       
  vmovdqa      ymm13, YMMWORD PTR [oqs_sig_qteslapi_PARAM_BARRx4+rip]  // Load PARAM_BARR_MULT
  vpmuldq      ymm4, ymm0, ymm13                      // a[j] = barr_reduce(a[j])
  vpmuldq      ymm5, ymm1, ymm13                    
  vpmuldq      ymm6, ymm2, ymm13                   
  vpmuldq      ymm7, ymm3, ymm13
  vpsrlq       ymm4, ymm4, PARAM_BARR_DIV                                        
  vpsrlq       ymm5, ymm5, PARAM_BARR_DIV                       
  vpsrlq       ymm6, ymm6, PARAM_BARR_DIV                   
  vpsrlq       ymm7, ymm7, PARAM_BARR_DIV  
  vpmuludq     ymm4, ymm4, ymm15                   
  vpmuludq     ymm5, ymm5, ymm15                   
  vpmuludq     ymm6, ymm6, ymm15                   
  vpmuludq     ymm7, ymm7, ymm15  
  vpsubd       ymm0, ymm0, ymm4                      
  vpsubd       ymm1, ymm1, ymm5
  vpsubd       ymm2, ymm2, ymm6
  vpsubd       ymm3, ymm3, ymm7
  vbroadcastss ymm4, DWORD PTR [reg_p3+4064]          // Load twiddle factor W     
  vbroadcastss ymm5, DWORD PTR [reg_p3+4068]                   
  vbroadcastss ymm6, DWORD PTR [reg_p3+4072]         
  vbroadcastss ymm7, DWORD PTR [reg_p3+4076]  
  vpmuldq      ymm4, ymm4, ymm8                       // W.(temp - a[j+k])  
  vpmuldq      ymm5, ymm5, ymm9
  vpmuldq      ymm6, ymm6, ymm10 
  vpmuldq      ymm7, ymm7, ymm11        
  vpmuldq      ymm8, ymm4, ymm14                      // temp = reduce(W.X)
  vpmuldq      ymm9, ymm5, ymm14                    
  vpmuldq      ymm10, ymm6, ymm14                   
  vpmuldq      ymm11, ymm7, ymm14  
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15     
  vpaddq       ymm4, ymm4, ymm8                                   
  vpaddq       ymm5, ymm5, ymm9                     
  vpaddq       ymm6, ymm6, ymm10                   
  vpaddq       ymm7, ymm7, ymm11
  vpsrlq       ymm4, ymm4, 32                                       
  vpsrlq       ymm5, ymm5, 32                       
  vpsrlq       ymm6, ymm6, 32                   
  vpsrlq       ymm7, ymm7, 32
    
// Round 9   
  vpsubd       ymm8, ymm0, ymm1                       // temp - a[j+k]
  vpsubd       ymm9, ymm4, ymm5
  vpsubd       ymm10, ymm2, ymm3
  vpsubd       ymm11, ymm6, ymm7
  vpaddd       ymm0, ymm0, ymm1                       // a[j] = temp + a[j+k] 
  vpaddd       ymm4, ymm4, ymm5                  
  vpaddd       ymm2, ymm2, ymm3                    
  vpaddd       ymm6, ymm6, ymm7   
  vpmuldq      ymm1, ymm0, ymm13                      // a[j] = barr_reduce(a[j])
  vpmuldq      ymm5, ymm4, ymm13                    
  vpmuldq      ymm3, ymm2, ymm13                   
  vpmuldq      ymm7, ymm6, ymm13
  vpsrlq       ymm1, ymm1, PARAM_BARR_DIV                                        
  vpsrlq       ymm5, ymm5, PARAM_BARR_DIV                       
  vpsrlq       ymm3, ymm3, PARAM_BARR_DIV                   
  vpsrlq       ymm7, ymm7, PARAM_BARR_DIV  
  vpmuludq     ymm1, ymm1, ymm15                   
  vpmuludq     ymm5, ymm5, ymm15                   
  vpmuludq     ymm3, ymm3, ymm15                   
  vpmuludq     ymm7, ymm7, ymm15  
  vpsubd       ymm0, ymm0, ymm1                      
  vpsubd       ymm4, ymm4, ymm5
  vpsubd       ymm2, ymm2, ymm3
  vpsubd       ymm6, ymm6, ymm7
  vbroadcastss ymm5, DWORD PTR [reg_p3+4080]          // Load twiddle factor W
  vbroadcastss ymm7, DWORD PTR [reg_p3+4084]                   
  vpmuldq      ymm1, ymm8, ymm5                       // W.(temp - a[j+k])  
  vpmuldq      ymm5, ymm9, ymm5
  vpmuldq      ymm3, ymm10, ymm7 
  vpmuldq      ymm7, ymm11, ymm7        
  vpmuldq      ymm8, ymm1, ymm14                      // temp = reduce(W.X)
  vpmuldq      ymm9, ymm5, ymm14                    
  vpmuldq      ymm10, ymm3, ymm14                   
  vpmuldq      ymm11, ymm7, ymm14  
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15     
  vpaddq       ymm1, ymm1, ymm8                                   
  vpaddq       ymm5, ymm5, ymm9                     
  vpaddq       ymm3, ymm3, ymm10                   
  vpaddq       ymm7, ymm7, ymm11
  vpsrlq       ymm1, ymm1, 32                                       
  vpsrlq       ymm5, ymm5, 32                       
  vpsrlq       ymm3, ymm3, 32                   
  vpsrlq       ymm7, ymm7, 32
    
// Round 10   
  vpsubd       ymm8, ymm0, ymm2                       // temp - a[j+k]
  vpsubd       ymm9, ymm4, ymm6
  vpsubd       ymm10, ymm1, ymm3
  vpsubd       ymm11, ymm5, ymm7
  vpaddd       ymm0, ymm0, ymm2                       // a[j] = temp + a[j+k] 
  vpaddd       ymm4, ymm4, ymm6                  
  vpaddd       ymm1, ymm1, ymm3                    
  vpaddd       ymm5, ymm5, ymm7   
  vpmuldq      ymm2, ymm0, ymm13                      // a[j] = barr_reduce(a[j])
  vpmuldq      ymm6, ymm4, ymm13                    
  vpmuldq      ymm3, ymm1, ymm13                   
  vpmuldq      ymm7, ymm5, ymm13
  vpsrlq       ymm2, ymm2, PARAM_BARR_DIV                                        
  vpsrlq       ymm6, ymm6, PARAM_BARR_DIV                       
  vpsrlq       ymm3, ymm3, PARAM_BARR_DIV                   
  vpsrlq       ymm7, ymm7, PARAM_BARR_DIV  
  vpmuludq     ymm2, ymm2, ymm15                   
  vpmuludq     ymm6, ymm6, ymm15                   
  vpmuludq     ymm3, ymm3, ymm15                   
  vpmuludq     ymm7, ymm7, ymm15  
  vpsubd       ymm0, ymm0, ymm2                      
  vpsubd       ymm4, ymm4, ymm6
  vpsubd       ymm1, ymm1, ymm3
  vpsubd       ymm5, ymm5, ymm7
  vpsllq       ymm0, ymm0, 32                                       
  vpsllq       ymm4, ymm4, 32                       
  vpsllq       ymm1, ymm1, 32                   
  vpsllq       ymm5, ymm5, 32
  vbroadcastss ymm13, DWORD PTR [reg_p3+4088]         // Load twiddle factor W
  vpmuldq      ymm2, ymm8, ymm13                      // W.(temp - a[j+k])  
  vpmuldq      ymm6, ymm9, ymm13
  vpmuldq      ymm3, ymm10, ymm13 
  vpmuldq      ymm7, ymm11, ymm13        
  vpmuldq      ymm8, ymm2, ymm14                      // temp = reduce(W.X)
  vpmuldq      ymm9, ymm6, ymm14                    
  vpmuldq      ymm10, ymm3, ymm14                   
  vpmuldq      ymm11, ymm7, ymm14  
  vpmuludq     ymm8, ymm8, ymm15                   
  vpmuludq     ymm9, ymm9, ymm15                   
  vpmuludq     ymm10, ymm10, ymm15                   
  vpmuludq     ymm11, ymm11, ymm15     
  vpaddq       ymm2, ymm2, ymm8                                
  vpaddq       ymm6, ymm6, ymm9                     
  vpaddq       ymm3, ymm3, ymm10                   
  vpaddq       ymm7, ymm7, ymm11                        
  vpermd       ymm2, ymm12, ymm2
  vpermd       ymm6, ymm12, ymm6
  vpermd       ymm3, ymm12, ymm3
  vpermd       ymm7, ymm12, ymm7
  vpermd       ymm0, ymm12, ymm0
  vpermd       ymm4, ymm12, ymm4
  vpermd       ymm1, ymm12, ymm1
  vpermd       ymm5, ymm12, ymm5
  vmovdqa      XMMWORD PTR [reg_p1+rax+2048], xmm2    // Store result
  vmovdqa      XMMWORD PTR [reg_p1+rax+2560], xmm6
  vmovdqa      XMMWORD PTR [reg_p1+rax+3072], xmm3
  vmovdqa      XMMWORD PTR [reg_p1+rax+3584], xmm7
  vmovdqa      XMMWORD PTR [reg_p1+rax], xmm0
  vmovdqa      XMMWORD PTR [reg_p1+rax+512], xmm4
  vmovdqa      XMMWORD PTR [reg_p1+rax+1024], xmm1
  vmovdqa      XMMWORD PTR [reg_p1+rax+1536], xmm5
  add          r11, 32
  add          rax, 16  
  dec          r10
  jnz          loop_intt8910
ret
