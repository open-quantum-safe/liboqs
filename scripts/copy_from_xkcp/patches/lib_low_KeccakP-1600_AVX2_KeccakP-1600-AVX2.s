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
@@ -31,12 +59,12 @@
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
 .ifndef old_gas_syntax

