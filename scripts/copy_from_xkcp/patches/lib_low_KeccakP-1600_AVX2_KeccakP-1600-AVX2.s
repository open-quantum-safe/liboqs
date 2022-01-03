--- upstream/lib/low/KeccakP-1600/AVX2/KeccakP-1600-AVX2.s
+++ upstream-patched/lib/low/KeccakP-1600/AVX2/KeccakP-1600-AVX2.s
@@ -16,6 +16,34 @@
 # Adaptations for macOS by Stéphane Léon.
 # Adaptations for mingw-w64 (changes macOS too) by Jorrit Jongma.
 
+#ifdef ADD_SYMBOL_SUFFIX
+#define KeccakP1600_Initialize KeccakP1600_Initialize_avx2
+#define KeccakP1600_AddByte KeccakP1600_AddByte_avx2
+#define KeccakP1600_AddBytes KeccakP1600_AddBytes_avx2
+#define KeccakP1600_OverwriteBytes KeccakP1600_OverwriteBytes_avx2
+#define KeccakP1600_OverwriteWithZeroes KeccakP1600_OverwriteWithZeroes_avx2
+#define KeccakP1600_Permute_Nrounds KeccakP1600_Permute_Nrounds_avx2
+#define KeccakP1600_Permute_12rounds KeccakP1600_Permute_12rounds_avx2
+#define KeccakP1600_Permute_24rounds KeccakP1600_Permute_24rounds_avx2
+#define KeccakP1600_ExtractBytes KeccakP1600_ExtractBytes_avx2
+#define KeccakP1600_ExtractAndAddBytes KeccakP1600_ExtractAndAddBytes_avx2
+#define KeccakF1600_FastLoop_Absorb KeccakF1600_FastLoop_Absorb_avx2
+#define KeccakP1600_12rounds_FastLoop_Absorb KeccakP1600_12rounds_FastLoop_Absorb_avx2
+
+#define _KeccakP1600_Initialize _KeccakP1600_Initialize_avx2
+#define _KeccakP1600_AddByte _KeccakP1600_AddByte_avx2
+#define _KeccakP1600_AddBytes _KeccakP1600_AddBytes_avx2
+#define _KeccakP1600_OverwriteBytes _KeccakP1600_OverwriteBytes_avx2
+#define _KeccakP1600_OverwriteWithZeroes _KeccakP1600_OverwriteWithZeroes_avx2
+#define _KeccakP1600_Permute_Nrounds _KeccakP1600_Permute_Nrounds_avx2
+#define _KeccakP1600_Permute_12rounds _KeccakP1600_Permute_12rounds_avx2
+#define _KeccakP1600_Permute_24rounds _KeccakP1600_Permute_24rounds_avx2
+#define _KeccakP1600_ExtractBytes _KeccakP1600_ExtractBytes_avx2
+#define _KeccakP1600_ExtractAndAddBytes _KeccakP1600_ExtractAndAddBytes_avx2
+#define _KeccakF1600_FastLoop_Absorb _KeccakF1600_FastLoop_Absorb_avx2
+#define _KeccakP1600_12rounds_FastLoop_Absorb _KeccakP1600_12rounds_FastLoop_Absorb_avx2
+#endif
+
 .text
 
 # -----------------------------------------------------------------------------
@@ -24,24 +52,24 @@
 #
 .globl  KeccakP1600_Initialize
 .globl _KeccakP1600_Initialize
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_Initialize,@function
-.endif
+#endif
 KeccakP1600_Initialize:
 _KeccakP1600_Initialize:
 .balign 32
     vpxor       %ymm0,%ymm0,%ymm0
-    vmovdqu     %ymm0,0*32(%rdi)
-    vmovdqu     %ymm0,1*32(%rdi)
-    vmovdqu     %ymm0,2*32(%rdi)
-    vmovdqu     %ymm0,3*32(%rdi)
-    vmovdqu     %ymm0,4*32(%rdi)
-    vmovdqu     %ymm0,5*32(%rdi)
+    vmovdqa     %ymm0,0*32(%rdi)
+    vmovdqa     %ymm0,1*32(%rdi)
+    vmovdqa     %ymm0,2*32(%rdi)
+    vmovdqa     %ymm0,3*32(%rdi)
+    vmovdqa     %ymm0,4*32(%rdi)
+    vmovdqa     %ymm0,5*32(%rdi)
     movq        $0,6*32(%rdi)
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_Initialize,.-KeccakP1600_Initialize
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -50,9 +78,9 @@
 #
 .globl  KeccakP1600_AddByte
 .globl _KeccakP1600_AddByte
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_AddByte,@function
-.endif
+#endif
 KeccakP1600_AddByte:
 _KeccakP1600_AddByte:
 .balign 32
@@ -65,9 +93,9 @@
     add         %rax, %rdi
     xorb        %sil, (%rdi)
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_AddByte,.-KeccakP1600_AddByte
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -76,9 +104,9 @@
 #
 .globl  KeccakP1600_AddBytes
 .globl _KeccakP1600_AddBytes
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_AddBytes,@function
-.endif
+#endif
 KeccakP1600_AddBytes:
 _KeccakP1600_AddBytes:
 .balign 32
@@ -130,9 +158,9 @@
     jnz         KeccakP1600_AddBytes_LastIncompleteLaneLoop
 KeccakP1600_AddBytes_Exit:
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_AddBytes,.-KeccakP1600_AddBytes
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -141,9 +169,9 @@
 #
 .globl  KeccakP1600_OverwriteBytes
 .globl _KeccakP1600_OverwriteBytes
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_OverwriteBytes,@function
-.endif
+#endif
 KeccakP1600_OverwriteBytes:
 _KeccakP1600_OverwriteBytes:
 .balign 32
@@ -195,9 +223,9 @@
     jnz         KeccakP1600_OverwriteBytes_LastIncompleteLaneLoop
 KeccakP1600_OverwriteBytes_Exit:
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_OverwriteBytes,.-KeccakP1600_OverwriteBytes
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -206,9 +234,9 @@
 #
 .globl  KeccakP1600_OverwriteWithZeroes
 .globl _KeccakP1600_OverwriteWithZeroes
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_OverwriteWithZeroes,@function
-.endif
+#endif
 KeccakP1600_OverwriteWithZeroes:
 _KeccakP1600_OverwriteWithZeroes:
 .balign 32
@@ -236,9 +264,9 @@
     jnz         KeccakP1600_OverwriteWithZeroes_LastIncompleteLaneLoop
 KeccakP1600_OverwriteWithZeroes_Exit:
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_OverwriteWithZeroes,.-KeccakP1600_OverwriteWithZeroes
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -247,9 +275,9 @@
 #
 .globl  KeccakP1600_ExtractBytes
 .globl _KeccakP1600_ExtractBytes
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_ExtractBytes,@function
-.endif
+#endif
 KeccakP1600_ExtractBytes:
 _KeccakP1600_ExtractBytes:
 .balign 32
@@ -304,9 +332,9 @@
 KeccakP1600_ExtractBytes_Exit:
     pop         %rbx
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_ExtractBytes,.-KeccakP1600_ExtractBytes
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -315,9 +343,9 @@
 #
 .globl  KeccakP1600_ExtractAndAddBytes
 .globl _KeccakP1600_ExtractAndAddBytes
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_ExtractAndAddBytes,@function
-.endif
+#endif
 KeccakP1600_ExtractAndAddBytes:
 _KeccakP1600_ExtractAndAddBytes:
 .balign 32
@@ -380,17 +408,17 @@
     pop         %r10
     pop         %rbx
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_ExtractAndAddBytes,.-KeccakP1600_ExtractAndAddBytes
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
 # internal    
 #
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type    __KeccakF1600,@function
-.endif
+#endif
 .balign 32
 __KeccakF1600:
 .Loop_avx2:
@@ -530,9 +558,9 @@
     dec         %eax
     jnz         .Loop_avx2
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   __KeccakF1600,.-__KeccakF1600
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -541,9 +569,9 @@
 #
 .globl  KeccakP1600_Permute_24rounds
 .globl _KeccakP1600_Permute_24rounds
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_Permute_24rounds,@function
-.endif
+#endif
 KeccakP1600_Permute_24rounds:
 _KeccakP1600_Permute_24rounds:
 .balign 32
@@ -570,9 +598,9 @@
     vmovdqu         %ymm6,8+32*5-96(%rdi)
     vzeroupper
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_Permute_24rounds,.-KeccakP1600_Permute_24rounds
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -581,9 +609,9 @@
 #
 .globl  KeccakP1600_Permute_12rounds
 .globl _KeccakP1600_Permute_12rounds
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_Permute_12rounds,@function
-.endif
+#endif
 KeccakP1600_Permute_12rounds:
 _KeccakP1600_Permute_12rounds:
 .balign 32
@@ -610,9 +638,9 @@
     vmovdqu         %ymm6,8+32*5-96(%rdi)
     vzeroupper
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_Permute_12rounds,.-KeccakP1600_Permute_12rounds
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -621,9 +649,9 @@
 #
 .globl  KeccakP1600_Permute_Nrounds
 .globl _KeccakP1600_Permute_Nrounds
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_Permute_Nrounds,@function
-.endif
+#endif
 KeccakP1600_Permute_Nrounds:
 _KeccakP1600_Permute_Nrounds:
 .balign 32
@@ -652,9 +680,9 @@
     vmovdqu         %ymm6,8+32*5-96(%rdi)
     vzeroupper
     ret
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_Permute_Nrounds,.-KeccakP1600_Permute_Nrounds
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -663,9 +691,9 @@
 #
 .globl  KeccakF1600_FastLoop_Absorb
 .globl _KeccakF1600_FastLoop_Absorb
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakF1600_FastLoop_Absorb,@function
-.endif
+#endif
 KeccakF1600_FastLoop_Absorb:
 _KeccakF1600_FastLoop_Absorb:
 .balign 32
@@ -819,11 +847,11 @@
     push            %rsi
     push            %rdx
     push            %rcx
-.ifdef no_plt
+#ifdef no_plt
     call            KeccakP1600_Permute_24rounds
-.else
+#else
     call            KeccakP1600_Permute_24rounds@PLT
-.endif
+#endif
     pop             %rcx
     pop             %rdx
     pop             %rsi
@@ -831,9 +859,9 @@
     cmp             %rsi, %rcx
     jae             KeccakF1600_FastLoop_Absorb_Not17Lanes
     jmp             KeccakF1600_FastLoop_Absorb_Exit
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakF1600_FastLoop_Absorb,.-KeccakF1600_FastLoop_Absorb
-.endif
+#endif
 
 # -----------------------------------------------------------------------------
 #
@@ -842,9 +870,9 @@
 #
 .globl  KeccakP1600_12rounds_FastLoop_Absorb
 .globl _KeccakP1600_12rounds_FastLoop_Absorb
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .type   KeccakP1600_12rounds_FastLoop_Absorb,@function
-.endif
+#endif
 KeccakP1600_12rounds_FastLoop_Absorb:
 _KeccakP1600_12rounds_FastLoop_Absorb:
 .balign 32
@@ -998,11 +1026,11 @@
     push            %rsi
     push            %rdx
     push            %rcx
-.ifdef no_plt
+#ifdef no_plt
     call            KeccakP1600_Permute_12rounds
-.else
+#else
     call            KeccakP1600_Permute_12rounds@PLT
-.endif
+#endif
     pop             %rcx
     pop             %rdx
     pop             %rsi
@@ -1010,9 +1038,9 @@
     cmp             %rsi, %rcx
     jae             KeccakP1600_12rounds_FastLoop_Absorb_Not17Lanes
     jmp             KeccakP1600_12rounds_FastLoop_Absorb_Exit
-.ifndef old_gas_syntax
+#ifndef old_gas_syntax
 .size   KeccakP1600_12rounds_FastLoop_Absorb,.-KeccakP1600_12rounds_FastLoop_Absorb
-.endif
+#endif
 
 .equ    ALLON,        0xFFFFFFFFFFFFFFFF
 

