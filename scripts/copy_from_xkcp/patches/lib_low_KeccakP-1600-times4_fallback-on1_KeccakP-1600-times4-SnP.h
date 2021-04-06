--- upstream/lib/low/KeccakP-1600-times4/fallback-on1/KeccakP-1600-times4-SnP.h
+++ upstream-patched/lib/low/KeccakP-1600-times4/fallback-on1/KeccakP-1600-times4-SnP.h
@@ -20,26 +20,63 @@
 
 #include "KeccakP-1600-SnP.h"
 
+#if defined(ADD_SYMBOL_SUFFIX)
+#define KECCAKTIMES4_NAMESPACE(x) KeccakP1600times4_##x##_serial
+#else
+#define KECCAKTIMES4_NAMESPACE(x) KeccakP1600times4_##x
+#endif
+
 #define KeccakP1600times4_implementation        "fallback on serial implementation (" KeccakP1600_implementation ")"
-#define KeccakP1600times4_statesSizeInBytes     (((KeccakP1600_stateSizeInBytes+(KeccakP1600_stateAlignment-1))/KeccakP1600_stateAlignment)*KeccakP1600_stateAlignment*4)
-#define KeccakP1600times4_statesAlignment       KeccakP1600_stateAlignment
 #define KeccakP1600times4_isFallback
+#define KeccakP1600times4_statesAlignment       KeccakP1600_stateAlignment
+#define KeccakP1600times4_statesSizeInBytes     (((KeccakP1600_stateSizeInBytes+(KeccakP1600_stateAlignment-1))/KeccakP1600_stateAlignment)*KeccakP1600_stateAlignment*4)
 
+#define KeccakP1600times4_StaticInitialize KECCAKTIMES4_NAMESPACE(KeccakP1600times4_StaticInitialize)
 void KeccakP1600times4_StaticInitialize( void );
+
+#define KeccakP1600times4_InitializeAll KECCAKTIMES4_NAMESPACE(KeccakP1600times4_InitializeAll)
 void KeccakP1600times4_InitializeAll(void *states);
+
+#define KeccakP1600times4_AddByte KECCAKTIMES4_NAMESPACE(KeccakP1600times4_AddByte)
 void KeccakP1600times4_AddByte(void *states, unsigned int instanceIndex, unsigned char data, unsigned int offset);
+
+#define KeccakP1600times4_AddBytes KECCAKTIMES4_NAMESPACE(KeccakP1600times4_AddBytes)
 void KeccakP1600times4_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
+
+#define KeccakP1600times4_AddLanesAll KECCAKTIMES4_NAMESPACE(KeccakP1600times4_AddLanesAll)
 void KeccakP1600times4_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
+
+#define KeccakP1600times4_OverwriteBytes KECCAKTIMES4_NAMESPACE(KeccakP1600times4_OverwriteBytes)
 void KeccakP1600times4_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
+
+#define KeccakP1600times4_OverwriteLanesAll KECCAKTIMES4_NAMESPACE(KeccakP1600times4_OverwriteLanesAll)
 void KeccakP1600times4_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
+
+#define KeccakP1600times4_OverwriteWithZeroes KECCAKTIMES4_NAMESPACE(KeccakP1600times4_OverwriteWithZeroes)
 void KeccakP1600times4_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount);
+
+#define KeccakP1600times4_PermuteAll_4rounds KECCAKTIMES4_NAMESPACE(KeccakP1600times4_PermuteAll_4rounds)
 void KeccakP1600times4_PermuteAll_4rounds(void *states);
+
+#define KeccakP1600times4_PermuteAll_6rounds KECCAKTIMES4_NAMESPACE(KeccakP1600times4_PermuteAll_6rounds)
 void KeccakP1600times4_PermuteAll_6rounds(void *states);
+
+#define KeccakP1600times4_PermuteAll_12rounds KECCAKTIMES4_NAMESPACE(KeccakP1600times4_PermuteAll_12rounds)
 void KeccakP1600times4_PermuteAll_12rounds(void *states);
+
+#define KeccakP1600times4_PermuteAll_24rounds KECCAKTIMES4_NAMESPACE(KeccakP1600times4_PermuteAll_24rounds)
 void KeccakP1600times4_PermuteAll_24rounds(void *states);
+
+#define KeccakP1600times4_ExtractBytes KECCAKTIMES4_NAMESPACE(KeccakP1600times4_ExtractBytes)
 void KeccakP1600times4_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
+
+#define KeccakP1600times4_ExtractLanesAll KECCAKTIMES4_NAMESPACE(KeccakP1600times4_ExtractLanesAll)
 void KeccakP1600times4_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
+
+#define KeccakP1600times4_ExtractAndAddBytes KECCAKTIMES4_NAMESPACE(KeccakP1600times4_ExtractAndAddBytes)
 void KeccakP1600times4_ExtractAndAddBytes(const void *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
+
+#define KeccakP1600times4_ExtractAndAddLanesAll KECCAKTIMES4_NAMESPACE(KeccakP1600times4_ExtractAndAddLanesAll)
 void KeccakP1600times4_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);
 
 #endif

