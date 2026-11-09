#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/histogram_sparsity.h"
int main(void)
{
    const uint32_t all_empty[]={0u,0u,0u,0u};
    const uint32_t half[]={3u,1u,0u,0u};
    const uint32_t none[]={1u,2u,3u,4u};
    uint32_t out=77u;
    assert(pc_histogram_sparsity_q_experiment(all_empty,4u,&out)&&out==65536u);
    assert(pc_histogram_sparsity_q_experiment(half,4u,&out)&&out==32768u);
    assert(pc_histogram_sparsity_q_experiment(none,4u,&out)&&out==0u);
    assert(!pc_histogram_sparsity_q_experiment(half,0u,&out)&&out==0u);
    assert(!pc_histogram_sparsity_q_experiment(half,65u,&out)&&out==0u);
    assert(!pc_histogram_sparsity_q_experiment(NULL,4u,&out)&&out==0u);
    assert(!pc_histogram_sparsity_q_experiment(half,4u,NULL));
    puts("Histogram empty-bin fraction and invalid-input tests: PASS");
    return 0;
}
