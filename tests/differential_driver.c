#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/pc_anomaly_zscore_q.h"

int main(void)
{
    int32_t value, mean, standard_deviation, out;
    unsigned fractional_bits;
    while (scanf("%" SCNd32 " %" SCNd32 " %" SCNd32 " %u",
                 &value, &mean, &standard_deviation, &fractional_bits) == 4) {
        int ok;
        out = INT32_C(123456789);
        ok = pc_anomaly_zscore_q(value, mean, standard_deviation,
                                 fractional_bits, &out);
        printf("%d %" PRId32 "\n", ok, out);
    }
    return ferror(stdin) || ferror(stdout) ? 1 : 0;
}
