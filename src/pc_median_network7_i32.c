#include "pc_median_network7_i32.h"

#define ASC(i, j) do { if (x[i] > x[j]) { int32_t t=x[i]; x[i]=x[j]; x[j]=t; } } while (0)

int32_t pc_median_network7_i32(int32_t a, int32_t b, int32_t c,
                               int32_t d, int32_t e, int32_t f, int32_t g)
{
    int32_t x[7] = {a,b,c,d,e,f,g};
    ASC(0,4); ASC(1,5); ASC(2,6); ASC(0,2); ASC(1,3); ASC(4,6); ASC(2,4);
    ASC(3,5); ASC(0,1); ASC(2,3); ASC(4,5); ASC(1,4); ASC(3,6); ASC(3,4);
    return x[3];
}

#undef ASC