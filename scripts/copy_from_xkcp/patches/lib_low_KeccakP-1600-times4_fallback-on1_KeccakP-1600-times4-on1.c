--- upstream/lib/low/KeccakP-1600-times4/fallback-on1/KeccakP-1600-times4-on1.c
+++ upstream-patched/lib/low/KeccakP-1600-times4/fallback-on1/KeccakP-1600-times4-on1.c
@@ -22,10 +22,28 @@
 #include "KeccakP-1600-SnP.h"
 
 #define prefix                          KeccakP1600times4
+#define suffix                          serial
 #define PlSnP_baseParallelism           1
 #define PlSnP_targetParallelism         4
 #define SnP_laneLengthInBytes           8
-#define SnP                             KeccakP1600
+#define SnP_prefix                      KeccakP1600
+#define SnP_suffix                      KECCAK_SYMBOL_SUFFIX
+
+#if defined(ADD_SYMBOL_SUFFIX)
+#define JOIN0(a,b,c) a##_##b##_##c
+#define JOIN(a,b)   JOIN0(a,b,c)
+
+#define SnP_Permute                     JOIN(SnP_prefix, Permute_24rounds, SnP_suffix)
+#define SnP_Permute_12rounds            JOIN(SnP_prefix, Permute_12rounds, SnP_suffix)
+#define SnP_Permute_Nrounds             JOIN(SnP_prefix, Permute_Nrounds, SnP_suffix)
+#define PlSnP_PermuteAll                JOIN(prefix, PermuteAll_24rounds, suffix)
+#define PlSnP_PermuteAll_12rounds       JOIN(prefix, PermuteAll_12rounds, suffix)
+#define PlSnP_PermuteAll_6rounds        JOIN(prefix, PermuteAll_6rounds, suffix)
+#define PlSnP_PermuteAll_4rounds        JOIN(prefix, PermuteAll_4rounds, suffix)
+
+#undef JOIN0
+#undef JOIN
+#else
 #define SnP_Permute                     KeccakP1600_Permute_24rounds
 #define SnP_Permute_12rounds            KeccakP1600_Permute_12rounds
 #define SnP_Permute_Nrounds             KeccakP1600_Permute_Nrounds
@@ -33,5 +51,6 @@
 #define PlSnP_PermuteAll_12rounds       KeccakP1600times4_PermuteAll_12rounds
 #define PlSnP_PermuteAll_6rounds        KeccakP1600times4_PermuteAll_6rounds
 #define PlSnP_PermuteAll_4rounds        KeccakP1600times4_PermuteAll_4rounds
+#endif
 
 #include "PlSnP-Fallback.inc"

