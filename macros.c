#include <stdio.h>

int main(void) {
#if __APPLE__
        printf("__APPLE__\n");
#else
        printf("no __APPLE__\n");
#endif
#if __ARM_FEATURE_CRYPTO
        printf("__ARM_FEATURE_CRYPTO\n");
#else
        printf("no __ARM_FEATURE_CRYPTO\n");
#endif
#if __ARM_FEATURE_SHA3
        printf("__ARM_FEATURE_SHA3\n");
#else
        printf("no __ARM_FEATURE_SHA3\n");
#endif
}
