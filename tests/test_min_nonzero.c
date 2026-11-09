#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/min_nonzero.h"
int main(void)
{
    const uint32_t mixed[]={0u,9u,1u,4u,1u};
    const uint32_t max_only[]={0u,UINT32_MAX};
    const uint32_t empty[]={0u,0u,0u};
    uint32_t out=77u;
    assert(pc_histogram_min_nonzero_u32_experiment(mixed,5u,&out)&&out==1u);
    assert(pc_histogram_min_nonzero_u32_experiment(max_only,2u,&out)&&out==UINT32_MAX);
    out=77u;
    assert(!pc_histogram_min_nonzero_u32_experiment(empty,3u,&out)&&out==77u);
    assert(!pc_histogram_min_nonzero_u32_experiment(mixed,0u,&out)&&out==77u);
    assert(!pc_histogram_min_nonzero_u32_experiment(NULL,1u,&out)&&out==77u);
    assert(!pc_histogram_min_nonzero_u32_experiment(mixed,5u,NULL));
    puts("Minimum positive histogram count and empty/error tests: PASS");
    return 0;
}