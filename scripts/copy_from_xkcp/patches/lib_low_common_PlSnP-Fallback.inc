--- upstream/lib/low/common/PlSnP-Fallback.inc
+++ upstream-patched/lib/low/common/PlSnP-Fallback.inc
@@ -25,46 +25,51 @@
 /* expect prefix */
 /* expect SnP_* */
 
-#define JOIN0(a, b)                     a ## b
-#define JOIN(a, b)                      JOIN0(a, b)
+#if defined(ADD_SYMBOL_SUFFIX)
+#define JOIN0(a, b, c)                  a ## _ ## b ## _ ## c
+#define JOIN(a, b, c)                   JOIN0(a, b, c)
+#else
+#define JOIN0(a, b)                     a ## _ ## b
+#define JOIN(a, b, c)                   JOIN0(a, b)
+#endif
 
-#define PlSnP_StaticInitialize          JOIN(prefix, _StaticInitialize)
-#define PlSnP_InitializeAll             JOIN(prefix, _InitializeAll)
-#define PlSnP_AddByte                   JOIN(prefix, _AddByte)
-#define PlSnP_AddBytes                  JOIN(prefix, _AddBytes)
-#define PlSnP_AddLanesAll               JOIN(prefix, _AddLanesAll)
-#define PlSnP_OverwriteBytes            JOIN(prefix, _OverwriteBytes)
-#define PlSnP_OverwriteLanesAll         JOIN(prefix, _OverwriteLanesAll)
-#define PlSnP_OverwriteWithZeroes       JOIN(prefix, _OverwriteWithZeroes)
-#define PlSnP_ExtractBytes              JOIN(prefix, _ExtractBytes)
-#define PlSnP_ExtractLanesAll           JOIN(prefix, _ExtractLanesAll)
-#define PlSnP_ExtractAndAddBytes        JOIN(prefix, _ExtractAndAddBytes)
-#define PlSnP_ExtractAndAddLanesAll     JOIN(prefix, _ExtractAndAddLanesAll)
+#define PlSnP_StaticInitialize          JOIN(prefix, StaticInitialize, suffix)
+#define PlSnP_InitializeAll             JOIN(prefix, InitializeAll, suffix)
+#define PlSnP_AddByte                   JOIN(prefix, AddByte, suffix)
+#define PlSnP_AddBytes                  JOIN(prefix, AddBytes, suffix)
+#define PlSnP_AddLanesAll               JOIN(prefix, AddLanesAll, suffix)
+#define PlSnP_OverwriteBytes            JOIN(prefix, OverwriteBytes, suffix)
+#define PlSnP_OverwriteLanesAll         JOIN(prefix, OverwriteLanesAll, suffix)
+#define PlSnP_OverwriteWithZeroes       JOIN(prefix, OverwriteWithZeroes, suffix)
+#define PlSnP_ExtractBytes              JOIN(prefix, ExtractBytes, suffix)
+#define PlSnP_ExtractLanesAll           JOIN(prefix, ExtractLanesAll, suffix)
+#define PlSnP_ExtractAndAddBytes        JOIN(prefix, ExtractAndAddBytes, suffix)
+#define PlSnP_ExtractAndAddLanesAll     JOIN(prefix, ExtractAndAddLanesAll, suffix)
 
 #if (PlSnP_baseParallelism == 1)
-    #define SnP_stateSizeInBytes            JOIN(SnP, _stateSizeInBytes)
-    #define SnP_stateAlignment              JOIN(SnP, _stateAlignment)
+    #define SnP_stateSizeInBytes            JOIN(SnP_prefix, stateSizeInBytes, SnP_suffix)
+    #define SnP_stateAlignment              JOIN(SnP_prefix, stateAlignment, SnP_suffix)
 #else
-    #define SnP_stateSizeInBytes            JOIN(SnP, _statesSizeInBytes)
-    #define SnP_stateAlignment              JOIN(SnP, _statesAlignment)
+    #define SnP_stateSizeInBytes            JOIN(SnP_prefix, statesSizeInBytes, SnP_suffix)
+    #define SnP_stateAlignment              JOIN(SnP_prefix, statesAlignment, SnP_suffix)
 #endif
 #define PlSnP_factor ((PlSnP_targetParallelism)/(PlSnP_baseParallelism))
 #define SnP_stateOffset (((SnP_stateSizeInBytes+(SnP_stateAlignment-1))/SnP_stateAlignment)*SnP_stateAlignment)
 #define stateWithIndex(i) ((unsigned char *)states+((i)*SnP_stateOffset))
 
-#define SnP_StaticInitialize            JOIN(SnP, _StaticInitialize)
-#define SnP_Initialize                  JOIN(SnP, _Initialize)
-#define SnP_InitializeAll               JOIN(SnP, _InitializeAll)
-#define SnP_AddByte                     JOIN(SnP, _AddByte)
-#define SnP_AddBytes                    JOIN(SnP, _AddBytes)
-#define SnP_AddLanesAll                 JOIN(SnP, _AddLanesAll)
-#define SnP_OverwriteBytes              JOIN(SnP, _OverwriteBytes)
-#define SnP_OverwriteLanesAll           JOIN(SnP, _OverwriteLanesAll)
-#define SnP_OverwriteWithZeroes         JOIN(SnP, _OverwriteWithZeroes)
-#define SnP_ExtractBytes                JOIN(SnP, _ExtractBytes)
-#define SnP_ExtractLanesAll             JOIN(SnP, _ExtractLanesAll)
-#define SnP_ExtractAndAddBytes          JOIN(SnP, _ExtractAndAddBytes)
-#define SnP_ExtractAndAddLanesAll       JOIN(SnP, _ExtractAndAddLanesAll)
+#define SnP_StaticInitialize            JOIN(SnP_prefix, StaticInitialize, SnP_suffix)
+#define SnP_Initialize                  JOIN(SnP_prefix, Initialize, SnP_suffix)
+#define SnP_InitializeAll               JOIN(SnP_prefix, InitializeAll, SnP_suffix)
+#define SnP_AddByte                     JOIN(SnP_prefix, AddByte, SnP_suffix)
+#define SnP_AddBytes                    JOIN(SnP_prefix, AddBytes, SnP_suffix)
+#define SnP_AddLanesAll                 JOIN(SnP_prefix, AddLanesAll, SnP_suffix)
+#define SnP_OverwriteBytes              JOIN(SnP_prefix, OverwriteBytes, SnP_suffix)
+#define SnP_OverwriteLanesAll           JOIN(SnP_prefix, OverwriteLanesAll, SnP_suffix)
+#define SnP_OverwriteWithZeroes         JOIN(SnP_prefix, OverwriteWithZeroes, SnP_suffix)
+#define SnP_ExtractBytes                JOIN(SnP_prefix, ExtractBytes, SnP_suffix)
+#define SnP_ExtractLanesAll             JOIN(SnP_prefix, ExtractLanesAll, SnP_suffix)
+#define SnP_ExtractAndAddBytes          JOIN(SnP_prefix, ExtractAndAddBytes, SnP_suffix)
+#define SnP_ExtractAndAddLanesAll       JOIN(SnP_prefix, ExtractAndAddLanesAll, SnP_suffix)
 
 void PlSnP_StaticInitialize( void )
 {
@@ -214,7 +219,7 @@
     #endif
 }
 
-void PlSnP_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
+void PlSnP_ExtractLanesAll(void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
 {
     unsigned int i;
 
@@ -237,7 +242,7 @@
     #endif
 }
 
-void PlSnP_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset)
+void PlSnP_ExtractAndAddLanesAll(void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset)
 {
     unsigned int i;
 

