#ifdef NDEBUG
#undef NDEBUG
#endif
#include "pc_hist_entropy_norm_q.h"
#include <stddef.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t h[256];
static uint32_t state = UINT32_C(0x4456E17A);
static long double max_abs_error;

static uint32_t next_u32(void)
{
    state ^= state << 13; state ^= state >> 17; state ^= state << 5;
    return state;
}

static long double exact_reference(void)
{
    long double total = 0.0L, entropy = 0.0L;
    for (size_t i = 0u; i < 256u; ++i) total += (long double)h[i];
    for (size_t i = 0u; i < 256u; ++i) {
        if (h[i] != 0u) {
            long double p = (long double)h[i] / total;
            entropy -= p * log2l(p);
        }
    }
    return entropy / 8.0L;
}

static void compare(unsigned f)
{
    uint32_t q = UINT32_MAX;
    assert(pc_hist_entropy_norm_q(h, f, &q));
    long double value = (long double)q / (long double)(UINT64_C(1) << f);
    long double tolerance = 1.0L / 262144.0L +
                            0.5L / (long double)(UINT64_C(1) << f) + 1.0e-12L;
    long double error = fabsl(value - exact_reference());
    if (f == 31u && error > max_abs_error) max_abs_error = error;
    assert(error <= tolerance);
    assert(q <= (UINT32_C(1) << f));
}

int main(void)
{
    uint32_t out = UINT32_C(0xDEADBEEF);
    assert(!pc_hist_entropy_norm_q(NULL, 15u, &out));
    assert(!pc_hist_entropy_norm_q(h, 15u, NULL));
    assert(!pc_hist_entropy_norm_q(h, 32u, &out));
    assert(out == UINT32_C(0xDEADBEEF));
    assert(!pc_hist_entropy_norm_q(h, 15u, &out));
    assert(out == UINT32_C(0xDEADBEEF));

    h[7] = UINT32_MAX;
    compare(31u);
    assert(pc_hist_entropy_norm_q(h, 31u, &out) && out == 0u);
    for (size_t i = 0u; i < 256u; ++i) h[i] = 1u;
    compare(31u);
    assert(pc_hist_entropy_norm_q(h, 31u, &out) && out == (UINT32_C(1) << 31u));

    for (size_t k = 2u; k <= 256u; ++k) {
        for (size_t i = 0u; i < 256u; ++i) h[i] = (i < k) ? 1u : 0u;
        compare(15u);
    }
    for (size_t i = 0u; i < 256u; ++i) h[i] = 0u;
    h[0] = 1u; h[128] = 2u; h[255] = 7u;
    compare(31u);
    uint32_t original;
    assert(pc_hist_entropy_norm_q(h, 24u, &original));
    for (size_t i = 0u; i < 256u; ++i) h[i] *= 100u;
    uint32_t scaled;
    assert(pc_hist_entropy_norm_q(h, 24u, &scaled));
    assert(original <= scaled + 2u && scaled <= original + 2u);

    for (size_t k = 0u; k < 2000u; ++k) {
        for (size_t i = 0u; i < 256u; ++i)
            h[i] = (next_u32() % 4u == 0u) ? 0u : next_u32();
        if (k % 4u == 0u) h[k % 256u] = UINT32_MAX;
        compare((unsigned)(k % 32u));
    }
    (void)printf("PASS: maximum observed normalized entropy error %.9e\n",
                 (double)max_abs_error);
    return 0;
}
