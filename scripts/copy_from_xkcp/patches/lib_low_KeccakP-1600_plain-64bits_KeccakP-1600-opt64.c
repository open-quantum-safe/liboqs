--- upstream/lib/low/KeccakP-1600/plain-64bits/KeccakP-1600-opt64.c
+++ upstream-patched/lib/low/KeccakP-1600/plain-64bits/KeccakP-1600-opt64.c
@@ -26,6 +26,7 @@
 #include <string.h>
 #include <stdlib.h>
 #include "brg_endian.h"
+#include "KeccakP-1600-SnP.h"
 #include "KeccakP-1600-opt64-config.h"
 
 #if defined(KeccakP1600_useLaneComplementing)
@@ -82,6 +83,10 @@
 
 /* ---------------------------------------------------------------- */
 
+void KeccakP1600_StaticInitialize(void) { }
+
+/* ---------------------------------------------------------------- */
+
 void KeccakP1600_Initialize(void *state)
 {
     memset(state, 0, 200);
@@ -137,27 +142,27 @@
     {
       /* Otherwise... */
       for( ; (i+8)<=laneCount; i+=8) {
-          ((uint64_t*)state)[i+0] ^= ((uint64_t*)data)[i+0];
-          ((uint64_t*)state)[i+1] ^= ((uint64_t*)data)[i+1];
-          ((uint64_t*)state)[i+2] ^= ((uint64_t*)data)[i+2];
-          ((uint64_t*)state)[i+3] ^= ((uint64_t*)data)[i+3];
-          ((uint64_t*)state)[i+4] ^= ((uint64_t*)data)[i+4];
-          ((uint64_t*)state)[i+5] ^= ((uint64_t*)data)[i+5];
-          ((uint64_t*)state)[i+6] ^= ((uint64_t*)data)[i+6];
-          ((uint64_t*)state)[i+7] ^= ((uint64_t*)data)[i+7];
+          ((uint64_t*)state)[i+0] ^= ((const uint64_t*)data)[i+0];
+          ((uint64_t*)state)[i+1] ^= ((const uint64_t*)data)[i+1];
+          ((uint64_t*)state)[i+2] ^= ((const uint64_t*)data)[i+2];
+          ((uint64_t*)state)[i+3] ^= ((const uint64_t*)data)[i+3];
+          ((uint64_t*)state)[i+4] ^= ((const uint64_t*)data)[i+4];
+          ((uint64_t*)state)[i+5] ^= ((const uint64_t*)data)[i+5];
+          ((uint64_t*)state)[i+6] ^= ((const uint64_t*)data)[i+6];
+          ((uint64_t*)state)[i+7] ^= ((const uint64_t*)data)[i+7];
       }
       for( ; (i+4)<=laneCount; i+=4) {
-          ((uint64_t*)state)[i+0] ^= ((uint64_t*)data)[i+0];
-          ((uint64_t*)state)[i+1] ^= ((uint64_t*)data)[i+1];
-          ((uint64_t*)state)[i+2] ^= ((uint64_t*)data)[i+2];
-          ((uint64_t*)state)[i+3] ^= ((uint64_t*)data)[i+3];
+          ((uint64_t*)state)[i+0] ^= ((const uint64_t*)data)[i+0];
+          ((uint64_t*)state)[i+1] ^= ((const uint64_t*)data)[i+1];
+          ((uint64_t*)state)[i+2] ^= ((const uint64_t*)data)[i+2];
+          ((uint64_t*)state)[i+3] ^= ((const uint64_t*)data)[i+3];
       }
       for( ; (i+2)<=laneCount; i+=2) {
-          ((uint64_t*)state)[i+0] ^= ((uint64_t*)data)[i+0];
-          ((uint64_t*)state)[i+1] ^= ((uint64_t*)data)[i+1];
+          ((uint64_t*)state)[i+0] ^= ((const uint64_t*)data)[i+0];
+          ((uint64_t*)state)[i+1] ^= ((const uint64_t*)data)[i+1];
       }
       if (i<laneCount) {
-          ((uint64_t*)state)[i+0] ^= ((uint64_t*)data)[i+0];
+          ((uint64_t*)state)[i+0] ^= ((const uint64_t*)data)[i+0];
       }
     }
 #else
@@ -179,7 +184,11 @@
 
 /* ---------------------------------------------------------------- */
 
-#if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
+#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
+void KeccakP1600_AddByte(void *state, unsigned char byte, unsigned int offset) {
+	((unsigned char*)state)[offset] ^= byte;
+}
+#else
 void KeccakP1600_AddByte(void *state, unsigned char byte, unsigned int offset)
 {
     uint64_t lane = byte;
@@ -235,11 +244,11 @@
 #ifdef KeccakP1600_useLaneComplementing
     unsigned int lanePosition;
 
-    for(lanePosition=0; lanePosition<laneCount; lanePosition++)
+    for(lanePosition=0; lanePosition<laneCount; lanePosition++) {
+        memcpy(((uint64_t*)state)+lanePosition, data, sizeof(uint64_t));
         if ((lanePosition == 1) || (lanePosition == 2) || (lanePosition == 8) || (lanePosition == 12) || (lanePosition == 17) || (lanePosition == 20))
-            ((uint64_t*)state)[lanePosition] = ~((const uint64_t*)data)[lanePosition];
-        else
-            ((uint64_t*)state)[lanePosition] = ((const uint64_t*)data)[lanePosition];
+            ((uint64_t*)state)[lanePosition] = ~((uint64_t*)state)[lanePosition];
+    }
 #else
     memcpy(state, data, laneCount*8);
 #endif
@@ -282,7 +291,7 @@
 
     for(lanePosition=0; lanePosition<byteCount/8; lanePosition++)
         if ((lanePosition == 1) || (lanePosition == 2) || (lanePosition == 8) || (lanePosition == 12) || (lanePosition == 17) || (lanePosition == 20))
-            ((uint64_t*)state)[lanePosition] = ~0;
+            ((uint64_t*)state)[lanePosition] = ~(uint64_t)0;
         else
             ((uint64_t*)state)[lanePosition] = 0;
     if (byteCount%8 != 0) {
@@ -371,7 +380,7 @@
 
 void KeccakP1600_ExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
 {
-    uint64_t lane = ((uint64_t*)state)[lanePosition];
+    uint64_t lane = ((const uint64_t*)state)[lanePosition];
 #ifdef KeccakP1600_useLaneComplementing
     if ((lanePosition == 1) || (lanePosition == 2) || (lanePosition == 8) || (lanePosition == 12) || (lanePosition == 17) || (lanePosition == 20))
         lane = ~lane;
@@ -409,30 +418,30 @@
 #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
     memcpy(data, state, laneCount*8);
 #else
-    unsigned int i;
-
-    for(i=0; i<laneCount; i++)
+    for(unsigned int i=0; i<laneCount; i++)
         fromWordToBytes(data+(i*8), ((const uint64_t*)state)[i]);
 #endif
 #ifdef KeccakP1600_useLaneComplementing
+    #define COMPL(lane) for(unsigned int i=0; i<8; i++) data[8*lane+i] = ~data[8*lane+i]
     if (laneCount > 1) {
-        ((uint64_t*)data)[ 1] = ~((uint64_t*)data)[ 1];
+        COMPL(1);
         if (laneCount > 2) {
-            ((uint64_t*)data)[ 2] = ~((uint64_t*)data)[ 2];
+            COMPL(2);
             if (laneCount > 8) {
-                ((uint64_t*)data)[ 8] = ~((uint64_t*)data)[ 8];
+                COMPL(8);
                 if (laneCount > 12) {
-                    ((uint64_t*)data)[12] = ~((uint64_t*)data)[12];
+                    COMPL(12);
                     if (laneCount > 17) {
-                        ((uint64_t*)data)[17] = ~((uint64_t*)data)[17];
+                        COMPL(17);
                         if (laneCount > 20) {
-                            ((uint64_t*)data)[20] = ~((uint64_t*)data)[20];
+                            COMPL(20);
                         }
                     }
                 }
             }
         }
     }
+    #undef COMPL
 #endif
 }
 
@@ -447,7 +456,7 @@
 
 void KeccakP1600_ExtractAndAddBytesInLane(const void *state, unsigned int lanePosition, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
 {
-    uint64_t lane = ((uint64_t*)state)[lanePosition];
+    uint64_t lane = ((const uint64_t*)state)[lanePosition];
 #ifdef KeccakP1600_useLaneComplementing
     if ((lanePosition == 1) || (lanePosition == 2) || (lanePosition == 8) || (lanePosition == 12) || (lanePosition == 17) || (lanePosition == 20))
         lane = ~lane;
@@ -455,10 +464,8 @@
 #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
     {
         unsigned int i;
-        uint64_t lane1[1];
-        lane1[0] = lane;
         for(i=0; i<length; i++)
-            output[i] = input[i] ^ ((uint8_t*)lane1)[offset+i];
+            output[i] = input[i] ^ ((uint8_t*)&lane)[offset+i];
     }
 #else
     unsigned int i;
@@ -478,11 +485,15 @@
 #if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
     unsigned char temp[8];
     unsigned int j;
+#else
+    uint64_t lane;
 #endif
 
     for(i=0; i<laneCount; i++) {
 #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
-        ((uint64_t*)output)[i] = ((uint64_t*)input)[i] ^ ((const uint64_t*)state)[i];
+        memcpy(&lane, input+8*i, sizeof(uint64_t));
+        lane ^= ((const uint64_t*)state)[i];
+        memcpy(output+8*i, &lane, sizeof(uint64_t));
 #else
         fromWordToBytes(temp, ((const uint64_t*)state)[i]);
         for(j=0; j<8; j++)
@@ -490,24 +501,26 @@
 #endif
     }
 #ifdef KeccakP1600_useLaneComplementing
+    #define COMPL(lane) for(i=0; i<8; i++) output[8*lane+i] = ~output[8*lane+i]
     if (laneCount > 1) {
-        ((uint64_t*)output)[ 1] = ~((uint64_t*)output)[ 1];
+        COMPL(1);
         if (laneCount > 2) {
-            ((uint64_t*)output)[ 2] = ~((uint64_t*)output)[ 2];
+            COMPL(2);
             if (laneCount > 8) {
-                ((uint64_t*)output)[ 8] = ~((uint64_t*)output)[ 8];
+                COMPL(8);
                 if (laneCount > 12) {
-                    ((uint64_t*)output)[12] = ~((uint64_t*)output)[12];
+                    COMPL(12);
                     if (laneCount > 17) {
-                        ((uint64_t*)output)[17] = ~((uint64_t*)output)[17];
+                        COMPL(17);
                         if (laneCount > 20) {
-                            ((uint64_t*)output)[20] = ~((uint64_t*)output)[20];
+                            COMPL(20);
                         }
                     }
                 }
             }
         }
     }
+    #undef COMPL
 #endif
 }
 
@@ -528,13 +541,12 @@
     unsigned int i;
     #endif
     uint64_t *stateAsLanes = (uint64_t*)state;
-    uint64_t *inDataAsLanes = (uint64_t*)data;
 
     copyFromState(A, stateAsLanes)
     while(dataByteLen >= laneCount*8) {
-        addInput(A, inDataAsLanes, laneCount)
+        addInput(A, data, laneCount)
         rounds24
-        inDataAsLanes += laneCount;
+        data += laneCount * 8;
         dataByteLen -= laneCount*8;
     }
     copyToState(stateAsLanes, A)
@@ -551,13 +563,12 @@
     unsigned int i;
     #endif
     uint64_t *stateAsLanes = (uint64_t*)state;
-    uint64_t *inDataAsLanes = (uint64_t*)data;
 
     copyFromState(A, stateAsLanes)
     while(dataByteLen >= laneCount*8) {
-        addInput(A, inDataAsLanes, laneCount)
+        addInput(A, data, laneCount)
         rounds12
-        inDataAsLanes += laneCount;
+        data += laneCount * 8;
         dataByteLen -= laneCount*8;
     }
     copyToState(stateAsLanes, A)

