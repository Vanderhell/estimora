#include "primitivecore/statistics/descriptive/pc_median_network5_i32.h"

#define ASC(x, y) do { if ((x) > (y)) { int32_t t=(x); (x)=(y); (y)=t; } } while (0)

int32_t pc_median_network5_i32(int32_t a, int32_t b, int32_t c,
                               int32_t d, int32_t e)
{
    ASC(a,b);
    ASC(c,d);
    ASC(a,c);
    ASC(b,d);
    ASC(b,c);
    ASC(e,c);
    ASC(b,e);
    return e;
}

#undef ASC
