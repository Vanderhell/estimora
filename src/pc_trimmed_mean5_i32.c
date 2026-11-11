#include "pc_trimmed_mean5_i32.h"
#include "pc_mean3_i32.h"

static void sort5(int32_t x[5])
{
    unsigned i, j;
    for (i = 1; i < 5; ++i) {
        int32_t value = x[i];
        j = i;
        while (j > 0 && x[j - 1] > value) {
            x[j] = x[j - 1];
            --j;
        }
        x[j] = value;
    }
}

int32_t pc_trimmed_mean5_i32(int32_t a, int32_t b, int32_t c,
                             int32_t d, int32_t e)
{
    int32_t x[5] = {a, b, c, d, e};
    sort5(x);
    return pc_mean3_i32(x[1], x[2], x[3]);
}
