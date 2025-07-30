#ifdef __APPLE__
#define CAT(A, B) _CAT(A, B)
#define _CAT(A, B) A##B
#undef fp_add
#undef fp_sub
#undef fp_mul
#undef fp_sqr
#undef fp2_mul_c0
#undef fp2_mul_c1
#undef fp2_sq_c0
#undef fp2_sq_c1
#define p2 CAT(_, p2)
#define p CAT(_, p)
#define fp_add CAT(_, SQISIGN_NAMESPACE(fp_add))
#define fp_sub CAT(_, SQISIGN_NAMESPACE(fp_sub))
#define fp_mul CAT(_, SQISIGN_NAMESPACE(fp_mul))
#define fp_sqr CAT(_, SQISIGN_NAMESPACE(fp_sqr))
#define fp2_mul_c0 CAT(_, SQISIGN_NAMESPACE(fp2_mul_c0))
#define fp2_mul_c1 CAT(_, SQISIGN_NAMESPACE(fp2_mul_c1))
#define fp2_sq_c0 CAT(_, SQISIGN_NAMESPACE(fp2_sq_c0))
#define fp2_sq_c1 CAT(_, SQISIGN_NAMESPACE(fp2_sq_c1))
#endif
