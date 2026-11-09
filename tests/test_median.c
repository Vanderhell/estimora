#include <assert.h>
#include <stdint.h>
#include "../include/median.h"

int main(void)
{
    pc_frugal_median_i32_experiment_state s;
    assert(!pc_frugal_median_i32_experiment_init(NULL));
    assert(pc_frugal_median_i32_experiment_init(&s));
    assert(pc_frugal_median_i32_experiment_step(&s, 0));
    assert(pc_frugal_median_i32_experiment_step(&s, 10));
    assert(s.estimate == 1); /* unlike randomized generic Frugal-1U with r=0 */
    assert(pc_frugal_median_i32_experiment_init(&s));
    assert(pc_frugal_median_i32_experiment_step(&s, INT32_MIN));
    assert(s.estimate == INT32_MIN);
    assert(pc_frugal_median_i32_experiment_step(&s, INT32_MAX));
    assert(s.estimate == INT32_MIN + 1);
    assert(pc_frugal_median_i32_experiment_init(&s));
    assert(pc_frugal_median_i32_experiment_step(&s, INT32_MAX));
    assert(pc_frugal_median_i32_experiment_step(&s, INT32_MIN));
    assert(s.estimate == INT32_MAX - 1);
    assert(!pc_frugal_median_i32_experiment_step(NULL, 0));
    return 0;
}