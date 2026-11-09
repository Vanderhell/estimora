#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/binary_entropy.h"

int main(void)
{
    uint32_t out=77u,prev=0u;
    assert(pc_binary_entropy_q_experiment(0u,&out)&&out==0u);
    assert(pc_binary_entropy_q_experiment(65536u,&out)&&out==0u);
    assert(pc_binary_entropy_q_experiment(32768u,&out)&&out==65536u);
    assert(pc_binary_entropy_q_experiment(1u,&out)&&out>0u);
    assert(pc_binary_entropy_q_experiment(65535u,&out)&&out>0u);
    out=77u;
    assert(!pc_binary_entropy_q_experiment(65537u,&out)&&out==77u);
    assert(!pc_binary_entropy_q_experiment(0u,NULL));
    for(uint32_t p=0u;p<=65536u;++p){
        uint32_t x=0u,y=0u;
        assert(pc_binary_entropy_q_experiment(p,&x)&&x<=65536u);
        assert(pc_binary_entropy_q_experiment(65536u-p,&y)&&x==y);
        if(p<=32768u){assert(x>=prev);prev=x;}
    }
    puts("Binary entropy exhaustive domain, symmetry, monotonicity and boundaries: PASS");
    return 0;
}
