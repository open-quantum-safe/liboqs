#include"SABER_params.h"

void pol_mul(uint16_t* a, uint16_t* b, uint16_t* res, uint16_t p, uint32_t n,uint32_t start);

void pol_mul_sb(int16_t* a, int16_t* b, int16_t* res, uint16_t p, uint32_t n,uint32_t start);

void toom_cook_4way(uint16_t* a1,uint16_t* b1, uint16_t* result, uint64_t p_mod, uint16_t n);

static inline int16_t reduce(int16_t a, int64_t p);
