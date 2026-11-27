#include <stdint.h>
#include <stdio.h>
#include <estimora/statistics/histogram_total_u32.h>
#include <estimora/statistics/mean4_i32.h>

int main(void)
{
    const uint32_t histogram[4] = { 2u, 5u, 3u, 1u };
    uint32_t total = 0u;
    const int32_t mean = estimora_mean4_i32(-2, 1, 2, 5);

    if (!estimora_histogram_total_u32(histogram, 4u, &total)) {
        return 1;
    }
    printf("total=%u mean=%ld\n", (unsigned)total, (long)mean);
    return total == 11u && mean == 2 ? 0 : 1;
}
