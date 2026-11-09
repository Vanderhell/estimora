#include "../include/pc_queue_age_mean_u32.h"
#include <stdint.h>
#include <stdio.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)

static uint32_t nearest_even_mean(const uint32_t *ages, size_t count)
{
    uint64_t sum = 0u;
    for (size_t i = 0u; i < count; ++i) sum += ages[i];
    uint64_t q = sum / count;
    const uint64_t r = sum % count;
    if (2u * r > count || (2u * r == count && (q & 1u) != 0u)) ++q;
    return (uint32_t)q;
}

int main(void)
{
    uint32_t result = UINT32_C(0xA5A5A5A5);
    const uint32_t tie_even[] = {0u, UINT32_MAX};
    const uint32_t tie_odd[] = {UINT32_MAX, UINT32_MAX - 1u};
    CHECK(pc_queue_age_mean_u32(0u, tie_even, 2u, &result) && result == 0u);
    CHECK(pc_queue_age_mean_u32(0u, tie_odd, 2u, &result) && result == 2u);
    CHECK(!pc_queue_age_mean_u32(0u, NULL, 1u, &result));
    CHECK(!pc_queue_age_mean_u32(0u, tie_even, 0u, &result));
    CHECK(!pc_queue_age_mean_u32(0u, (const uint32_t[]){UINT32_C(0x80000000)}, 1u, &result));
    CHECK(result == 2u);
    CHECK(pc_queue_age_mean_u32(0u, (const uint32_t[]){UINT32_C(0x80000001)}, 1u, &result));
    CHECK(result == UINT32_C(0x7FFFFFFF));
#if SIZE_MAX > UINT32_MAX
    CHECK(!pc_queue_age_mean_u32(0u, tie_even, (size_t)UINT32_MAX + 1u, &result));
    CHECK(result == UINT32_C(0x7FFFFFFF));
#endif
    for (uint32_t a = 0u; a < 16u; ++a) {
        for (uint32_t b = 0u; b < 16u; ++b) {
            for (uint32_t c = 0u; c < 16u; ++c) {
                const uint32_t ages[] = {a, b, c};
                const uint32_t ticks[] = {0u - a, 0u - b, 0u - c};
                CHECK(pc_queue_age_mean_u32(0u, ticks, 3u, &result));
                CHECK(result == nearest_even_mean(ages, 3u));
            }
        }
    }
    uint32_t state = UINT32_C(0x1751A6E);
    uint32_t ages[16], ticks[16];
    for (uint32_t trial = 0u; trial < UINT32_C(100000); ++trial) {
        state ^= state << 13u; state ^= state >> 17u; state ^= state << 5u;
        const uint32_t now = state;
        const size_t count = (size_t)(trial % 16u) + 1u;
        for (size_t i = 0u; i < count; ++i) {
            state ^= state << 13u; state ^= state >> 17u; state ^= state << 5u;
            ages[i] = state & UINT32_C(0x7FFFFFFF);
            ticks[i] = now - ages[i];
        }
        CHECK(pc_queue_age_mean_u32(now, ticks, count, &result));
        CHECK(result == nearest_even_mean(ages, count));
    }
    return 0;
}
