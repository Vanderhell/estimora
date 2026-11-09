#include "../include/pc_circular_weighted_mean_q.h"

#include <inttypes.h>
#include <stdio.h>

int main(void)
{
    unsigned count;
    while (scanf("%u", &count) == 1) {
        uint32_t phases[RM_CIRCULAR_WEIGHTED_MEAN_Q_MAX_COUNT];
        uint16_t weights[RM_CIRCULAR_WEIGHTED_MEAN_Q_MAX_COUNT];
        unsigned i;
        pc_circular_weighted_mean_q_result result;
        int ok;
        if (count == 0u || count > RM_CIRCULAR_WEIGHTED_MEAN_Q_MAX_COUNT) return 2;
        for (i = 0u; i < count; ++i) {
            unsigned weight;
            if (scanf("%" SCNu32 " %u", &phases[i], &weight) != 2 || weight > UINT16_MAX) return 3;
            weights[i] = (uint16_t)weight;
        }
        result.phase_q0_32 = UINT32_C(0xdeadbeef);
        result.resultant_q1_31 = UINT32_C(0xcafebabe);
        ok = pc_circular_weighted_mean_q(phases, weights, (uint8_t)count,
                                         UINT32_C(64), &result);
        printf("%d %" PRIu32 " %" PRIu32 "\n", ok,
               result.phase_q0_32, result.resultant_q1_31);
    }
    return 0;
}