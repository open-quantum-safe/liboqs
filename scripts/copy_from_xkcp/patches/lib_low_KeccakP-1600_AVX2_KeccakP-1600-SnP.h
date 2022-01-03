--- upstream/lib/low/KeccakP-1600/AVX2/KeccakP-1600-SnP.h
+++ upstream-patched/lib/low/KeccakP-1600/AVX2/KeccakP-1600-SnP.h
@@ -29,24 +29,59 @@
 #define FORCE_SYSV
 #endif
 
-#define KeccakP1600_implementation      "AVX2 optimized implementation"
-#define KeccakP1600_stateSizeInBytes    200
-#define KeccakP1600_stateAlignment      8
-#define KeccakF1600_FastLoop_supported
-#define KeccakP1600_12rounds_FastLoop_supported
+#define KeccakP1600_implementation_avx2      "AVX2 optimized implementation"
+#define KeccakP1600_stateSizeInBytes_avx2    200
+#define KeccakP1600_stateAlignment_avx2      32
+#define KeccakF1600_FastLoop_supported_avx2
 
-#define KeccakP1600_StaticInitialize()
+#if defined(ADD_SYMBOL_SUFFIX)
+#define KECCAK_SYMBOL_SUFFIX avx2
+#define KECCAK_IMPL_NAMESPACE(x) x##_avx2
+#else
+#define KECCAK_IMPL_NAMESPACE(x) x
+#define KeccakP1600_implementation KeccakP1600_implementation_avx2
+#define KeccakP1600_stateSizeInBytes KeccakP1600_stateSizeInBytes_avx2
+#define KeccakP1600_stateAlignment KeccakP1600_stateAlignment_avx2
+#define KeccakF1600_FastLoop_supported KeccakF1600_FastLoop_supported_avx2
+#endif
+
+#define KeccakP1600_StaticInitialize KECCAK_IMPL_NAMESPACE(KeccakP1600_StaticInitialize)
+void KeccakP1600_StaticInitialize(void);
+
+#define KeccakP1600_Initialize KECCAK_IMPL_NAMESPACE(KeccakP1600_Initialize)
 FORCE_SYSV void KeccakP1600_Initialize(void *state);
+
+#define KeccakP1600_AddByte KECCAK_IMPL_NAMESPACE(KeccakP1600_AddByte)
 FORCE_SYSV void KeccakP1600_AddByte(void *state, unsigned char data, unsigned int offset);
+
+#define KeccakP1600_AddBytes KECCAK_IMPL_NAMESPACE(KeccakP1600_AddBytes)
 FORCE_SYSV void KeccakP1600_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
+
+#define KeccakP1600_OverwriteBytes KECCAK_IMPL_NAMESPACE(KeccakP1600_OverwriteBytes)
 FORCE_SYSV void KeccakP1600_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
+
+#define KeccakP1600_OverwriteWithZeroes KECCAK_IMPL_NAMESPACE(KeccakP1600_OverwriteWithZeroes)
 FORCE_SYSV void KeccakP1600_OverwriteWithZeroes(void *state, unsigned int byteCount);
+
+#define KeccakP1600_Permute_Nrounds KECCAK_IMPL_NAMESPACE(KeccakP1600_Permute_Nrounds)
 FORCE_SYSV void KeccakP1600_Permute_Nrounds(void *state, unsigned int nrounds);
+
+#define KeccakP1600_Permute_12rounds KECCAK_IMPL_NAMESPACE(KeccakP1600_Permute_12rounds)
 FORCE_SYSV void KeccakP1600_Permute_12rounds(void *state);
+
+#define KeccakP1600_Permute_24rounds KECCAK_IMPL_NAMESPACE(KeccakP1600_Permute_24rounds)
 FORCE_SYSV void KeccakP1600_Permute_24rounds(void *state);
+
+#define KeccakP1600_ExtractBytes KECCAK_IMPL_NAMESPACE(KeccakP1600_ExtractBytes)
 FORCE_SYSV void KeccakP1600_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
+
+#define KeccakP1600_ExtractAndAddBytes KECCAK_IMPL_NAMESPACE(KeccakP1600_ExtractAndAddBytes)
 FORCE_SYSV void KeccakP1600_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
+
+#define KeccakF1600_FastLoop_Absorb KECCAK_IMPL_NAMESPACE(KeccakF1600_FastLoop_Absorb)
 FORCE_SYSV size_t KeccakF1600_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);
+
+#define KeccakP1600_12rounds_FastLoop_Absorb KECCAK_IMPL_NAMESPACE(KeccakP1600_12rounds_FastLoop_Absorb)
 FORCE_SYSV size_t KeccakP1600_12rounds_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);
 
 #endif

