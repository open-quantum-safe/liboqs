--- upstream/lib/low/common/PlSnP-Fallback.inc
+++ upstream-patched/lib/low/common/PlSnP-Fallback.inc
@@ -214,7 +214,7 @@
     #endif
 }
 
-void PlSnP_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
+void PlSnP_ExtractLanesAll(void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
 {
     unsigned int i;
 
@@ -237,7 +237,7 @@
     #endif
 }
 
-void PlSnP_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset)
+void PlSnP_ExtractAndAddLanesAll(void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset)
 {
     unsigned int i;
 

