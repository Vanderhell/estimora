#include <stdint.h>
#include <limits.h>
#include <stddef.h>
#include "../include/pc_anomaly_zscore_q.h"

#define CHECK(x) do { if (!(x)) return __LINE__; } while (0)

int main(void)
{
    int32_t out = 123;

    CHECK(pc_anomaly_zscore_q(15, 10, 5, 4u, &out) && out == 16);
    CHECK(pc_anomaly_zscore_q(-15, -10, 5, 4u, &out) && out == -16);
    CHECK(pc_anomaly_zscore_q(1, 0, 2, 0u, &out) && out == 0);
    CHECK(pc_anomaly_zscore_q(3, 0, 2, 0u, &out) && out == 2);
    CHECK(pc_anomaly_zscore_q(INT32_MAX, INT32_MIN, INT32_MAX, 0u, &out));
    CHECK(out == 2);
    CHECK(pc_anomaly_zscore_q(INT32_MIN, INT32_MAX, INT32_MAX, 0u, &out));
    CHECK(out == -2);
    CHECK(pc_anomaly_zscore_q(INT32_MAX, 0, 1, 0u, &out));
    CHECK(out == INT32_MAX);
    CHECK(pc_anomaly_zscore_q(INT32_MIN, 0, 1, 0u, &out));
    CHECK(out == INT32_MIN);

    out = 123;
    CHECK(!pc_anomaly_zscore_q(1, 0, 0, 8u, &out) && out == 123);
    CHECK(!pc_anomaly_zscore_q(1, 0, -1, 8u, &out) && out == 123);
    CHECK(!pc_anomaly_zscore_q(1, 0, 1, 32u, &out) && out == 123);
    CHECK(!pc_anomaly_zscore_q(1, 0, 1, 31u, &out) && out == 123);
    CHECK(!pc_anomaly_zscore_q(1, 0, 1, 0u, NULL));
    return 0;
}