#include "../include/pc_circular_weighted_mean_q.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#define CHECK(condition) do { if (!(condition)) return __LINE__; } while (0)

static uint32_t phase_distance(uint32_t a, uint32_t b)
{
    uint32_t d = a - b;
    return d < UINT32_C(0x80000000) ? d : UINT32_C(0) - d;
}

int main(void)
{
    uint32_t phases[RM_CIRCULAR_WEIGHTED_MEAN_Q_MAX_COUNT] = { 0u };
    uint16_t weights[RM_CIRCULAR_WEIGHTED_MEAN_Q_MAX_COUNT] = { 0u };
    pc_circular_weighted_mean_q_result out = { 17u, 19u };
    unsigned i;
    uint32_t state = UINT32_C(0x91e10da5);

    CHECK(!pc_circular_weighted_mean_q(NULL, weights, 1u, 0u, &out));
    CHECK(!pc_circular_weighted_mean_q(phases, NULL, 1u, 0u, &out));
    CHECK(!pc_circular_weighted_mean_q(phases, weights, 0u, 0u, &out));
    CHECK(!pc_circular_weighted_mean_q(phases, weights, 33u, 0u, &out));
    CHECK(!pc_circular_weighted_mean_q(phases, weights, 1u, UINT32_C(2147483649), &out));
    CHECK(!pc_circular_weighted_mean_q(phases, weights, 1u, 0u, NULL));
    CHECK(out.phase_q0_32 == 17u && out.resultant_q1_31 == 19u);

    phases[0] = 0u;
    weights[0] = 1u;
    CHECK(pc_circular_weighted_mean_q(phases, weights, 1u, 0u, &out));
    CHECK(phase_distance(out.phase_q0_32, 0u) <= 64u);
    CHECK(out.resultant_q1_31 >= UINT32_C(2147480000));
    CHECK(out.resultant_q1_31 <= UINT32_C(2147483648));

    phases[0] = 0u;
    phases[1] = UINT32_C(0x80000000);
    weights[0] = 1u;
    weights[1] = 1u;
    out.phase_q0_32 = 123u;
    out.resultant_q1_31 = 456u;
    CHECK(!pc_circular_weighted_mean_q(phases, weights, 2u, 0u, &out));
    CHECK(out.phase_q0_32 == 123u && out.resultant_q1_31 == 456u);

    phases[0] = 0u;
    phases[1] = UINT32_C(0x40000000);
    phases[2] = UINT32_C(0x80000000);
    phases[3] = UINT32_C(0xc0000000);
    weights[0] = weights[1] = weights[2] = weights[3] = 1u;
    out.phase_q0_32 = 123u;
    out.resultant_q1_31 = 456u;
    CHECK(!pc_circular_weighted_mean_q(phases, weights, 4u, 64u, &out));
    CHECK(out.phase_q0_32 == 123u && out.resultant_q1_31 == 456u);

    phases[0] = 0u;
    phases[1] = UINT32_C(0x40000000);
    weights[0] = 3u;
    weights[1] = 1u;
    CHECK(pc_circular_weighted_mean_q(phases, weights, 2u, 0u, &out));
    CHECK(phase_distance(out.phase_q0_32, UINT32_C(219937506)) < 300u);
    {
        pc_circular_weighted_mean_q_result scaled;
        weights[0] = 300u;
        weights[1] = 100u;
        CHECK(pc_circular_weighted_mean_q(phases, weights, 2u, 0u, &scaled));
        CHECK(phase_distance(out.phase_q0_32, scaled.phase_q0_32) < 8u);
        CHECK(out.resultant_q1_31 == scaled.resultant_q1_31);
    }

    for (i = 0u; i < 10000u; ++i) {
        uint8_t count = (uint8_t)(1u + i % RM_CIRCULAR_WEIGHTED_MEAN_Q_MAX_COUNT);
        uint8_t j;
        uint32_t rotation;
        pc_circular_weighted_mean_q_result base;
        pc_circular_weighted_mean_q_result moved;
        for (j = 0u; j < count; ++j) {
            state = state * UINT32_C(1664525) + UINT32_C(1013904223);
            phases[j] = state;
            state = state * UINT32_C(1664525) + UINT32_C(1013904223);
            weights[j] = (uint16_t)(1u + state % UINT32_C(65535));
        }
        rotation = i * UINT32_C(2654435761);
        CHECK(pc_circular_weighted_mean_q(phases, weights, count, 0u, &base));
        for (j = 0u; j < count; ++j) phases[j] += rotation;
        CHECK(pc_circular_weighted_mean_q(phases, weights, count, 0u, &moved));
        CHECK(phase_distance(moved.phase_q0_32, base.phase_q0_32 + rotation) < 500u);
        CHECK(moved.resultant_q1_31 <= UINT32_C(2147483648));
    }

    {
        uint32_t opposite[2] = { 0u, UINT32_C(0x80000000) };
        uint16_t equal[2] = { 9u, 9u };
        out.phase_q0_32 = 123u;
        out.resultant_q1_31 = 456u;
        CHECK(!pc_circular_weighted_mean_q(opposite, equal, 2u, 0u, &out));
        CHECK(out.phase_q0_32 == 123u && out.resultant_q1_31 == 456u);
        equal[0] = 0u;
        equal[1] = 0u;
        CHECK(!pc_circular_weighted_mean_q(opposite, equal, 2u, 0u, &out));
        CHECK(out.phase_q0_32 == 123u && out.resultant_q1_31 == 456u);
    }
    return 0;
}