--- upstream/lib/low/KeccakP-1600-times4/AVX2/KeccakP-1600-times4-SIMD256.c
+++ upstream-patched/lib/low/KeccakP-1600-times4/AVX2/KeccakP-1600-times4-SIMD256.c
@@ -45,16 +45,15 @@
 
 #if defined(KeccakP1600times4_useAVX2)
     #define ANDnu256(a, b)          _mm256_andnot_si256(a, b)
-    #define CONST256(a)             _mm256_load_si256((const V256 *)&(a))
-    #define CONST256_64(a)          _mm256_set1_epi64x(a)
+    #define CONST256_64(a)          _mm256_set1_epi64x((long long) a)
     #define LOAD256(a)              _mm256_load_si256((const V256 *)&(a))
     #define LOAD256u(a)             _mm256_loadu_si256((const V256 *)&(a))
-    #define LOAD4_64(a, b, c, d)    _mm256_set_epi64x((uint64_t)(a), (uint64_t)(b), (uint64_t)(c), (uint64_t)(d))
+    #define LOAD4_64(a, b, c, d)    _mm256_set_epi64x((long long)(a), (long long)(b), (long long)(c), (long long)(d))
     #define ROL64in256(d, a, o)     d = _mm256_or_si256(_mm256_slli_epi64(a, o), _mm256_srli_epi64(a, 64-(o)))
-    #define ROL64in256_8(d, a)      d = _mm256_shuffle_epi8(a, CONST256(rho8))
-    #define ROL64in256_56(d, a)     d = _mm256_shuffle_epi8(a, CONST256(rho56))
-static const uint64_t rho8[4] = {0x0605040302010007, 0x0E0D0C0B0A09080F, 0x1615141312111017, 0x1E1D1C1B1A19181F};
-static const uint64_t rho56[4] = {0x0007060504030201, 0x080F0E0D0C0B0A09, 0x1017161514131211, 0x181F1E1D1C1B1A19};
+    #define ROL64in256_8(d, a)      d = _mm256_shuffle_epi8(a, rho8.v)
+    #define ROL64in256_56(d, a)     d = _mm256_shuffle_epi8(a, rho56.v)
+static const union { uint64_t i[4]; V256 v; } rho8 = {{0x0605040302010007, 0x0E0D0C0B0A09080F, 0x1615141312111017, 0x1E1D1C1B1A19181F}};
+static const union { uint64_t i[4]; V256 v; } rho56 = {{0x0007060504030201, 0x080F0E0D0C0B0A09, 0x1017161514131211, 0x181F1E1D1C1B1A19}};
     #define STORE256(a, b)          _mm256_store_si256((V256 *)&(a), b)
     #define STORE256u(a, b)         _mm256_storeu_si256((V256 *)&(a), b)
     #define STORE2_128(ah, al, v)   _mm256_storeu2_m128i(&(ah), &(al), v)
@@ -87,9 +86,30 @@
 
 #define SnP_laneLengthInBytes 8
 
+static inline uint64_t load64(const unsigned char *x)
+{
+    return (uint64_t) x[0]         \
+         | (uint64_t) x[1] << 0x08 \
+         | (uint64_t) x[2] << 0x10 \
+         | (uint64_t) x[3] << 0x18 \
+         | (uint64_t) x[4] << 0x20 \
+         | (uint64_t) x[5] << 0x28 \
+         | (uint64_t) x[6] << 0x30 \
+         | (uint64_t) x[7] << 0x38;
+}
+
+static void store64(unsigned char *out, uint64_t in)
+{
+    memcpy(out, &in, sizeof(uint64_t));
+}
+
 void KeccakP1600times4_InitializeAll(void *states)
 {
-    memset(states, 0, KeccakP1600times4_statesSizeInBytes);
+    memset(states, 0, KeccakP1600times4_statesSizeInBytes_avx2);
+}
+
+void KeccakP1600times4_AddByte(void *states, unsigned int instanceIndex, unsigned char byte, unsigned int offset) {
+    ((unsigned char*)states)[instanceIndex*8 + (offset/8)*4*8 + offset%8] ^= byte;
 }
 
 void KeccakP1600times4_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
@@ -97,14 +117,16 @@
     unsigned int sizeLeft = length;
     unsigned int lanePosition = offset/SnP_laneLengthInBytes;
     unsigned int offsetInLane = offset%SnP_laneLengthInBytes;
+    unsigned int bytesInLane;
     const unsigned char *curData = data;
     uint64_t *statesAsLanes = (uint64_t *)states;
+    uint64_t lane;
 
     if ((sizeLeft > 0) && (offsetInLane != 0)) {
-        unsigned int bytesInLane = SnP_laneLengthInBytes - offsetInLane;
-        uint64_t lane = 0;
+        bytesInLane = SnP_laneLengthInBytes - offsetInLane;
         if (bytesInLane > sizeLeft)
             bytesInLane = sizeLeft;
+        lane = 0;
         memcpy((unsigned char*)&lane + offsetInLane, curData, bytesInLane);
         statesAsLanes[laneIndex(instanceIndex, lanePosition)] ^= lane;
         sizeLeft -= bytesInLane;
@@ -113,7 +135,7 @@
     }
 
     while(sizeLeft >= SnP_laneLengthInBytes) {
-        uint64_t lane = *((const uint64_t*)curData);
+        lane = load64(curData);
         statesAsLanes[laneIndex(instanceIndex, lanePosition)] ^= lane;
         sizeLeft -= SnP_laneLengthInBytes;
         lanePosition++;
@@ -121,7 +143,7 @@
     }
 
     if (sizeLeft > 0) {
-        uint64_t lane = 0;
+        lane = 0;
         memcpy(&lane, curData, sizeLeft);
         statesAsLanes[laneIndex(instanceIndex, lanePosition)] ^= lane;
     }
@@ -131,18 +153,21 @@
 {
     V256 *stateAsLanes = (V256 *)states;
     unsigned int i;
-    const uint64_t *curData0 = (const uint64_t *)data;
-    const uint64_t *curData1 = (const uint64_t *)(data+laneOffset*SnP_laneLengthInBytes);
-    const uint64_t *curData2 = (const uint64_t *)(data+laneOffset*2*SnP_laneLengthInBytes);
-    const uint64_t *curData3 = (const uint64_t *)(data+laneOffset*3*SnP_laneLengthInBytes);
+    const unsigned char *curData0 = data;
+    const unsigned char *curData1 = data+laneOffset*SnP_laneLengthInBytes;
+    const unsigned char *curData2 = data+laneOffset*2*SnP_laneLengthInBytes;
+    const unsigned char *curData3 = data+laneOffset*3*SnP_laneLengthInBytes;
     V256    lanes0, lanes1, lanes2, lanes3, lanesL01, lanesL23, lanesH01, lanesH23;
 
-    #define Xor_In( argIndex )  XOReq256(stateAsLanes[argIndex], LOAD4_64(curData3[argIndex], curData2[argIndex], curData1[argIndex], curData0[argIndex]))
-
-    #define Xor_In4( argIndex ) lanes0 = LOAD256u( curData0[argIndex]),\
-                                lanes1 = LOAD256u( curData1[argIndex]),\
-                                lanes2 = LOAD256u( curData2[argIndex]),\
-                                lanes3 = LOAD256u( curData3[argIndex]),\
+    #define Xor_In( argIndex )  ((uint64_t *)states)[4*argIndex+0] ^= load64(curData0+8*argIndex),\
+                                ((uint64_t *)states)[4*argIndex+1] ^= load64(curData1+8*argIndex),\
+                                ((uint64_t *)states)[4*argIndex+2] ^= load64(curData2+8*argIndex),\
+                                ((uint64_t *)states)[4*argIndex+3] ^= load64(curData3+8*argIndex)
+
+    #define Xor_In4( argIndex ) lanes0 = LOAD256u(curData0[8*argIndex]),\
+                                lanes1 = LOAD256u(curData1[8*argIndex]),\
+                                lanes2 = LOAD256u(curData2[8*argIndex]),\
+                                lanes3 = LOAD256u(curData3[8*argIndex]),\
                                 INTLEAVE(),\
                                 XOReq256( stateAsLanes[argIndex+0], lanes0 ),\
                                 XOReq256( stateAsLanes[argIndex+1], lanes1 ),\
@@ -191,7 +216,7 @@
     }
 
     while(sizeLeft >= SnP_laneLengthInBytes) {
-        uint64_t lane = *((const uint64_t*)curData);
+        uint64_t lane = load64(curData);
         statesAsLanes[laneIndex(instanceIndex, lanePosition)] = lane;
         sizeLeft -= SnP_laneLengthInBytes;
         lanePosition++;
@@ -207,23 +232,26 @@
 {
     V256 *stateAsLanes = (V256 *)states;
     unsigned int i;
-    const uint64_t *curData0 = (const uint64_t *)data;
-    const uint64_t *curData1 = (const uint64_t *)(data+laneOffset*SnP_laneLengthInBytes);
-    const uint64_t *curData2 = (const uint64_t *)(data+laneOffset*2*SnP_laneLengthInBytes);
-    const uint64_t *curData3 = (const uint64_t *)(data+laneOffset*3*SnP_laneLengthInBytes);
+    const unsigned char *curData0 = data;
+    const unsigned char *curData1 = data+laneOffset*SnP_laneLengthInBytes;
+    const unsigned char *curData2 = data+laneOffset*2*SnP_laneLengthInBytes;
+    const unsigned char *curData3 = data+laneOffset*3*SnP_laneLengthInBytes;
     V256    lanes0, lanes1, lanes2, lanes3, lanesL01, lanesL23, lanesH01, lanesH23;
 
-    #define OverWr( argIndex )  STORE256(stateAsLanes[argIndex], LOAD4_64(curData3[argIndex], curData2[argIndex], curData1[argIndex], curData0[argIndex]))
-
-    #define OverWr4( argIndex )     lanes0 = LOAD256u( curData0[argIndex]),\
-                                    lanes1 = LOAD256u( curData1[argIndex]),\
-                                    lanes2 = LOAD256u( curData2[argIndex]),\
-                                    lanes3 = LOAD256u( curData3[argIndex]),\
-                                    INTLEAVE(),\
-                                    STORE256( stateAsLanes[argIndex+0], lanes0 ),\
-                                    STORE256( stateAsLanes[argIndex+1], lanes1 ),\
-                                    STORE256( stateAsLanes[argIndex+2], lanes2 ),\
-                                    STORE256( stateAsLanes[argIndex+3], lanes3 )
+    #define OverWr( argIndex )  ((uint64_t *)states)[4*argIndex+0] = load64(curData0 + 8*argIndex),\
+                                ((uint64_t *)states)[4*argIndex+1] = load64(curData1 + 8*argIndex),\
+                                ((uint64_t *)states)[4*argIndex+2] = load64(curData2 + 8*argIndex),\
+                                ((uint64_t *)states)[4*argIndex+3] = load64(curData3 + 8*argIndex)
+
+    #define OverWr4( argIndex ) lanes0 = LOAD256u(curData0[8*argIndex]),\
+                                lanes1 = LOAD256u(curData1[8*argIndex]),\
+                                lanes2 = LOAD256u(curData2[8*argIndex]),\
+                                lanes3 = LOAD256u(curData3[8*argIndex]),\
+                                INTLEAVE(),\
+                                STORE256( stateAsLanes[argIndex+0], lanes0 ),\
+                                STORE256( stateAsLanes[argIndex+1], lanes1 ),\
+                                STORE256( stateAsLanes[argIndex+2], lanes2 ),\
+                                STORE256( stateAsLanes[argIndex+3], lanes3 )
 
     if ( laneCount >= 16 )  {
         OverWr4( 0 );
@@ -277,14 +305,14 @@
         unsigned int bytesInLane = SnP_laneLengthInBytes - offsetInLane;
         if (bytesInLane > sizeLeft)
             bytesInLane = sizeLeft;
-        memcpy( curData, ((unsigned char *)&statesAsLanes[laneIndex(instanceIndex, lanePosition)]) + offsetInLane, bytesInLane);
+        memcpy( curData, ((const unsigned char *)&statesAsLanes[laneIndex(instanceIndex, lanePosition)]) + offsetInLane, bytesInLane);
         sizeLeft -= bytesInLane;
         lanePosition++;
         curData += bytesInLane;
     }
 
     while(sizeLeft >= SnP_laneLengthInBytes) {
-        *(uint64_t*)curData = statesAsLanes[laneIndex(instanceIndex, lanePosition)];
+        store64(curData, statesAsLanes[laneIndex(instanceIndex, lanePosition)]);
         sizeLeft -= SnP_laneLengthInBytes;
         lanePosition++;
         curData += SnP_laneLengthInBytes;
@@ -297,20 +325,20 @@
 
 void KeccakP1600times4_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
 {
-    uint64_t *curData0 = (uint64_t *)data;
-    uint64_t *curData1 = (uint64_t *)(data+laneOffset*1*SnP_laneLengthInBytes);
-    uint64_t *curData2 = (uint64_t *)(data+laneOffset*2*SnP_laneLengthInBytes);
-    uint64_t *curData3 = (uint64_t *)(data+laneOffset*3*SnP_laneLengthInBytes);
+    unsigned char *curData0 = data;
+    unsigned char *curData1 = data+laneOffset*1*SnP_laneLengthInBytes;
+    unsigned char *curData2 = data+laneOffset*2*SnP_laneLengthInBytes;
+    unsigned char *curData3 = data+laneOffset*3*SnP_laneLengthInBytes;
 
     const V256 *stateAsLanes = (const V256 *)states;
     const uint64_t *stateAsLanes64 = (const uint64_t*)states;
     V256    lanes0, lanes1, lanes2, lanes3, lanesL01, lanesL23, lanesH01, lanesH23;
     unsigned int i;
 
-    #define Extr( argIndex )    curData0[argIndex] = stateAsLanes64[4*(argIndex)],      \
-                                curData1[argIndex] = stateAsLanes64[4*(argIndex)+1],    \
-                                curData2[argIndex] = stateAsLanes64[4*(argIndex)+2],    \
-                                curData3[argIndex] = stateAsLanes64[4*(argIndex)+3]
+    #define Extr( argIndex )    store64(curData0+8*argIndex, stateAsLanes64[4*(argIndex)+0]),\
+                                store64(curData1+8*argIndex, stateAsLanes64[4*(argIndex)+1]),\
+                                store64(curData2+8*argIndex, stateAsLanes64[4*(argIndex)+2]),\
+                                store64(curData3+8*argIndex, stateAsLanes64[4*(argIndex)+3])
 
     #define Extr4( argIndex )   lanes0 = LOAD256( stateAsLanes[argIndex+0] ),           \
                                 lanes1 = LOAD256( stateAsLanes[argIndex+1] ),           \
@@ -368,7 +396,7 @@
     }
 
     while(sizeLeft >= SnP_laneLengthInBytes) {
-        *((uint64_t*)curOutput) = *((uint64_t*)curInput) ^ statesAsLanes[laneIndex(instanceIndex, lanePosition)];
+        store64(curOutput, load64(curInput) ^ statesAsLanes[laneIndex(instanceIndex, lanePosition)]);
         sizeLeft -= SnP_laneLengthInBytes;
         lanePosition++;
         curInput += SnP_laneLengthInBytes;
@@ -386,25 +414,24 @@
 
 void KeccakP1600times4_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset)
 {
-    const uint64_t *curInput0 = (uint64_t *)input;
-    const uint64_t *curInput1 = (uint64_t *)(input+laneOffset*1*SnP_laneLengthInBytes);
-    const uint64_t *curInput2 = (uint64_t *)(input+laneOffset*2*SnP_laneLengthInBytes);
-    const uint64_t *curInput3 = (uint64_t *)(input+laneOffset*3*SnP_laneLengthInBytes);
-    uint64_t *curOutput0 = (uint64_t *)output;
-    uint64_t *curOutput1 = (uint64_t *)(output+laneOffset*1*SnP_laneLengthInBytes);
-    uint64_t *curOutput2 = (uint64_t *)(output+laneOffset*2*SnP_laneLengthInBytes);
-    uint64_t *curOutput3 = (uint64_t *)(output+laneOffset*3*SnP_laneLengthInBytes);
+    const unsigned char *curInput0 = input;
+    const unsigned char *curInput1 = input+laneOffset*1*SnP_laneLengthInBytes;
+    const unsigned char *curInput2 = input+laneOffset*2*SnP_laneLengthInBytes;
+    const unsigned char *curInput3 = input+laneOffset*3*SnP_laneLengthInBytes;
+    unsigned char *curOutput0 = output;
+    unsigned char *curOutput1 = output+laneOffset*1*SnP_laneLengthInBytes;
+    unsigned char *curOutput2 = output+laneOffset*2*SnP_laneLengthInBytes;
+    unsigned char *curOutput3 = output+laneOffset*3*SnP_laneLengthInBytes;
 
     const V256 *stateAsLanes = (const V256 *)states;
     const uint64_t *stateAsLanes64 = (const uint64_t*)states;
     V256    lanes0, lanes1, lanes2, lanes3, lanesL01, lanesL23, lanesH01, lanesH23;
     unsigned int i;
 
-    #define ExtrXor( argIndex ) \
-                                curOutput0[argIndex] = curInput0[argIndex] ^ stateAsLanes64[4*(argIndex)],\
-                                curOutput1[argIndex] = curInput1[argIndex] ^ stateAsLanes64[4*(argIndex)+1],\
-                                curOutput2[argIndex] = curInput2[argIndex] ^ stateAsLanes64[4*(argIndex)+2],\
-                                curOutput3[argIndex] = curInput3[argIndex] ^ stateAsLanes64[4*(argIndex)+3]
+    #define ExtrXor( argIndex ) store64(curOutput0+8*argIndex, load64(curInput0+8*argIndex) ^ stateAsLanes64[4*argIndex+0]),\
+                                store64(curOutput1+8*argIndex, load64(curInput1+8*argIndex) ^ stateAsLanes64[4*argIndex+1]),\
+                                store64(curOutput2+8*argIndex, load64(curInput2+8*argIndex) ^ stateAsLanes64[4*argIndex+2]),\
+                                store64(curOutput3+8*argIndex, load64(curInput3+8*argIndex) ^ stateAsLanes64[4*argIndex+3])
 
     #define ExtrXor4( argIndex ) \
                                     lanes0 = LOAD256( stateAsLanes[argIndex+0] ),\
@@ -692,7 +719,7 @@
     E##su = XOR256(Bsu, ANDnu256(Bsa, Bse)); \
 \
 
-static ALIGN(KeccakP1600times4_statesAlignment) const uint64_t KeccakF1600RoundConstants[24] = {
+static ALIGN(KeccakP1600times4_statesAlignment_avx2) const uint64_t KeccakF1600RoundConstants[24] = {
     0x0000000000000001ULL,
     0x0000000000008082ULL,
     0x800000000000808aULL,
@@ -900,19 +927,17 @@
         }
         return (const unsigned char *)curData0 - dataStart;
 #else
-        unsigned int i;
-        const unsigned char *dataStart = data;
-        const uint64_t *curData0 = (const uint64_t *)data;
-        const uint64_t *curData1 = (const uint64_t *)(data+laneOffsetParallel*1*SnP_laneLengthInBytes);
-        const uint64_t *curData2 = (const uint64_t *)(data+laneOffsetParallel*2*SnP_laneLengthInBytes);
-        const uint64_t *curData3 = (const uint64_t *)(data+laneOffsetParallel*3*SnP_laneLengthInBytes);
+        const unsigned char *curData0 = data;
+        const unsigned char *curData1 = data+laneOffsetParallel*1*SnP_laneLengthInBytes;
+        const unsigned char *curData2 = data+laneOffsetParallel*2*SnP_laneLengthInBytes;
+        const unsigned char *curData3 = data+laneOffsetParallel*3*SnP_laneLengthInBytes;
         V256 *statesAsLanes = (V256 *)states;
         declareABCDE
 
         copyFromState(A, statesAsLanes)
         while(dataByteLen >= (laneOffsetParallel*3 + laneCount)*8) {
             #define XOR_In( Xxx, argIndex ) \
-                XOReq256(Xxx, LOAD4_64(curData3[argIndex], curData2[argIndex], curData1[argIndex], curData0[argIndex]))
+                XOReq256(Xxx, LOAD4_64(load64(curData3+8*argIndex), load64(curData2+8*argIndex), load64(curData1+8*argIndex), load64(curData0+8*argIndex)))
             XOR_In( Aba, 0 );
             XOR_In( Abe, 1 );
             XOR_In( Abi, 2 );
@@ -943,11 +968,10 @@
             dataByteLen -= laneOffsetSerial*8;
         }
         copyToState(statesAsLanes, A)
-        return (const unsigned char *)curData0 - dataStart;
+        return (size_t)(curData0 - data);
 #endif
     }
     else {
-        unsigned int i;
         const unsigned char *dataStart = data;
 
         while(dataByteLen >= (laneOffsetParallel*3 + laneCount)*8) {
@@ -956,7 +980,7 @@
             data += laneOffsetSerial*8;
             dataByteLen -= laneOffsetSerial*8;
         }
-        return data - dataStart;
+        return (size_t)(data - dataStart);
     }
 }
 
@@ -1002,19 +1026,17 @@
         }
         return (const unsigned char *)curData0 - dataStart;
 #else
-        unsigned int i;
-        const unsigned char *dataStart = data;
-        const uint64_t *curData0 = (const uint64_t *)data;
-        const uint64_t *curData1 = (const uint64_t *)(data+laneOffsetParallel*1*SnP_laneLengthInBytes);
-        const uint64_t *curData2 = (const uint64_t *)(data+laneOffsetParallel*2*SnP_laneLengthInBytes);
-        const uint64_t *curData3 = (const uint64_t *)(data+laneOffsetParallel*3*SnP_laneLengthInBytes);
+        const unsigned char *curData0 = data;
+        const unsigned char *curData1 = data+laneOffsetParallel*1*SnP_laneLengthInBytes;
+        const unsigned char *curData2 = data+laneOffsetParallel*2*SnP_laneLengthInBytes;
+        const unsigned char *curData3 = data+laneOffsetParallel*3*SnP_laneLengthInBytes;
         V256 *statesAsLanes = states;
         declareABCDE
 
         copyFromState(A, statesAsLanes)
         while(dataByteLen >= (laneOffsetParallel*3 + laneCount)*8) {
             #define XOR_In( Xxx, argIndex ) \
-                XOReq256(Xxx, LOAD4_64(curData3[argIndex], curData2[argIndex], curData1[argIndex], curData0[argIndex]))
+                XOReq256(Xxx, LOAD4_64(load64(curData3+8*argIndex), load64(curData2+8*argIndex), load64(curData1+8*argIndex), load64(curData0+8*argIndex)))
             XOR_In( Aba, 0 );
             XOR_In( Abe, 1 );
             XOR_In( Abi, 2 );
@@ -1045,11 +1067,10 @@
             dataByteLen -= laneOffsetSerial*8;
         }
         copyToState(statesAsLanes, A)
-        return (const unsigned char *)curData0 - dataStart;
+        return (size_t)(curData0 - data);
 #endif
     }
     else {
-        unsigned int i;
         const unsigned char *dataStart = data;
 
         while(dataByteLen >= (laneOffsetParallel*3 + laneCount)*8) {
@@ -1058,10 +1079,11 @@
             data += laneOffsetSerial*8;
             dataByteLen -= laneOffsetSerial*8;
         }
-        return data - dataStart;
+        return (size_t)(data - dataStart);
     }
 }
 
+#if 0
 /* ------------------------------------------------------------------------- */
 
 #define UNINTLEAVEa(lanes0, lanes1, lanes2, lanes3)                                         \
@@ -1319,3 +1341,4 @@
 #undef Kravatte_Roll
 #undef UNINTLEAVEa
 #undef INTLEAVEa
+#endif

