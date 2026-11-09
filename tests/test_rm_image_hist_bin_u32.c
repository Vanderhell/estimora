#include "pc_image_hist_bin_u32.h"
#include <stdint.h>
#include <stdio.h>
#define CHECK(x) do { if (!(x)) return __LINE__; } while (0)
static uint32_t state=UINT32_C(0x3499ab17);
static uint32_t next(void){state^=state<<13;state^=state>>17;state^=state<<5;return state;}
int main(void){uint32_t bins,out=99u;unsigned v,b;CHECK(!pc_image_hist_bin_u32(0u,0u,&out)&&out==99u);CHECK(!pc_image_hist_bin_u32(0u,1u,NULL));
for(v=0;v<256u;++v)for(bins=1u;bins<=256u;++bins){CHECK(pc_image_hist_bin_u32((uint8_t)v,bins,&out));CHECK(out==((uint64_t)v*bins)/256u&&out<bins);}
for(b=0;b<1000000u;++b){uint8_t x=(uint8_t)next();bins=next();if(bins==0u)bins=UINT32_MAX;CHECK(pc_image_hist_bin_u32(x,bins,&out));CHECK(out==((uint64_t)x*bins)/256u&&out<bins);}
CHECK(pc_image_hist_bin_u32(255u,UINT32_MAX,&out)&&out==((uint64_t)255u*UINT32_MAX)/256u);puts("pc_image_hist_bin_u32 experiment: PASS (65,536 exhaustive, 1,000,000 seeded)");return 0;}
