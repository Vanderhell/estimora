#include <assert.h>
#include <stdint.h>
#include "../include/range.h"

int main(void)
{
    const uint32_t sparse[] = {0u, 9u, 0u, 1u, 0u, 4u};
    const uint32_t singleton[] = {0u, UINT32_MAX, 0u};
    const uint32_t zeroes[] = {0u, 0u};
    uint32_t out = 77u;
    assert(pc_histogram_range_u32_experiment(sparse, 6u, &out) && out == 4u);
    assert(pc_histogram_range_u32_experiment(singleton, 3u, &out) && out == 0u);
    out = 77u;
    assert(!pc_histogram_range_u32_experiment(zeroes, 2u, &out) && out == 77u);
    assert(!pc_histogram_range_u32_experiment(sparse, 0u, &out) && out == 77u);
    assert(!pc_histogram_range_u32_experiment(NULL, 1u, &out) && out == 77u);
    assert(!pc_histogram_range_u32_experiment(sparse, 6u, NULL));
    return 0;
}
