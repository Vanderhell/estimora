#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/cross_entropy.h"
int main(void)
{
    const uint32_t pure[]={65536u};
    const uint32_t pure4[]={65536u,0u,0u,0u};
    const uint32_t balanced[]={32768u,32768u};
    const uint32_t uniform4[]={16384u,16384u,16384u,16384u};
    const uint32_t mismatch[]={65536u,0u};
    const uint32_t zero_model[]={0u,65536u};
    const uint32_t invalid[]={1u,1u};
    uint32_t out=17u;
    assert(pc_cross_entropy_q_experiment(pure,pure,1u,&out)&&out==0u);
    assert(pc_cross_entropy_q_experiment(balanced,balanced,2u,&out)&&out==65536u);
    assert(pc_cross_entropy_q_experiment(pure4,uniform4,4u,&out)&&out==131072u);
    assert(!pc_cross_entropy_q_experiment(mismatch,zero_model,2u,&out)&&out==131072u);
    assert(!pc_cross_entropy_q_experiment(invalid,invalid,2u,&out)&&out==131072u);
    assert(!pc_cross_entropy_q_experiment(pure,pure,0u,&out)&&out==131072u);
    assert(!pc_cross_entropy_q_experiment(NULL,pure,1u,&out));
    assert(!pc_cross_entropy_q_experiment(pure,pure,1u,NULL));
    puts("Categorical cross-entropy boundaries, support and invalid inputs: PASS");
    return 0;
}
