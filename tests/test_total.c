#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/histogram_total.h"
int main(void)
{
    const uint32_t values[]={1u,2u,3u};
    const uint32_t maximum[]={UINT32_MAX};
    const uint32_t overflow[]={UINT32_MAX,1u};
    uint32_t total=77u;
    assert(pc_histogram_total_u32_experiment(values,3u,&total)&&total==6u);
    assert(pc_histogram_total_u32_experiment(maximum,1u,&total)&&total==UINT32_MAX);
    total=77u;
    assert(!pc_histogram_total_u32_experiment(overflow,2u,&total)&&total==77u);
    assert(pc_histogram_total_u32_experiment(NULL,0u,&total)&&total==0u);
    assert(!pc_histogram_total_u32_experiment(NULL,1u,&total)&&total==0u);
    assert(!pc_histogram_total_u32_experiment(values,3u,NULL));
    puts("Checked histogram total and overflow atomicity: PASS");
    return 0;
}
