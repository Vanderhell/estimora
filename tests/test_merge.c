#include <assert.h>
#include <stdint.h>
#include "../include/merge.h"

int main(void)
{
    const uint32_t left[] = {1u, 0u, UINT32_MAX};
    const uint32_t right[] = {2u, 7u, 0u};
    const uint32_t overflow_left[] = {3u, UINT32_MAX};
    const uint32_t overflow_right[] = {4u, 1u};
    uint32_t out[] = {9u, 9u, 9u};
    uint32_t inplace[] = {1u, 2u};
    uint32_t inplace_overflow[] = {UINT32_MAX, 1u};
    const uint32_t one[] = {1u, 0u};
    const uint32_t addend[] = {3u, 4u};
    assert(pc_histogram_merge_u32_experiment(left, right, 3u, out));
    assert(out[0] == 3u && out[1] == 7u && out[2] == UINT32_MAX);
    out[0] = 9u; out[1] = 9u;
    assert(!pc_histogram_merge_u32_experiment(overflow_left, overflow_right, 2u, out));
    assert(out[0] == 9u && out[1] == 9u);
    assert(pc_histogram_merge_u32_experiment(inplace, addend, 2u, inplace));
    assert(inplace[0] == 4u && inplace[1] == 6u);
    assert(!pc_histogram_merge_u32_experiment(inplace_overflow, one, 2u, inplace_overflow));
    assert(inplace_overflow[0] == UINT32_MAX && inplace_overflow[1] == 1u);
    assert(pc_histogram_merge_u32_experiment(NULL, NULL, 0u, out));
    assert(!pc_histogram_merge_u32_experiment(NULL, right, 1u, out));
    assert(!pc_histogram_merge_u32_experiment(left, right, 1u, NULL));
    return 0;
}