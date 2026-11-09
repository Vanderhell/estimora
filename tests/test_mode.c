#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/histogram_mode.h"
int main(void)
{
    const uint32_t tied[]={0u,5u,5u,1u};
    const uint32_t singleton[]={UINT32_MAX};
    const uint32_t empty[]={0u,0u,0u};
    uint32_t mode=77u;
    assert(pc_histogram_mode_u32_experiment(tied,4u,&mode)&&mode==1u);
    assert(pc_histogram_mode_u32_experiment(singleton,1u,&mode)&&mode==0u);
    mode=77u;
    assert(!pc_histogram_mode_u32_experiment(empty,3u,&mode)&&mode==77u);
    assert(!pc_histogram_mode_u32_experiment(tied,0u,&mode)&&mode==77u);
    assert(!pc_histogram_mode_u32_experiment(NULL,1u,&mode)&&mode==77u);
    assert(!pc_histogram_mode_u32_experiment(tied,4u,NULL));
    puts("Histogram mode, first-tie policy and empty input tests: PASS");
    return 0;
}