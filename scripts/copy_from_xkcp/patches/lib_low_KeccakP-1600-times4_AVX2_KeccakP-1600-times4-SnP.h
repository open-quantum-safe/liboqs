--- upstream/lib/low/KeccakP-1600-times4/AVX2/KeccakP-1600-times4-SnP.h
+++ upstream-patched/lib/low/KeccakP-1600-times4/AVX2/KeccakP-1600-times4-SnP.h
@@ -49,7 +49,9 @@
 void KeccakP1600times4_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);
 size_t KeccakF1600times4_FastLoop_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);
 size_t KeccakP1600times4_12rounds_FastLoop_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);
+#if 0
 size_t KeccakP1600times4_KravatteCompress(uint64_t *xAccu, uint64_t *kRoll, const unsigned char *input, size_t inputByteLen);
 size_t KeccakP1600times4_KravatteExpand(uint64_t *yAccu, const uint64_t *kRoll, unsigned char *output, size_t outputByteLen);
+#endif
 
 #endif

