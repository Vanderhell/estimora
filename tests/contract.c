#include <stdint.h>
#include "roundmath/pc_interval_mean_u32.h"
#define CHECK(x) do { if (!(x)) return __LINE__; } while (0)
int main(void) {
 uint32_t out=99u; const uint32_t a[]={1u,2u}; const uint32_t b[]={UINT32_MAX,UINT32_MAX,0u};
 CHECK(pc_interval_mean_u32(a,2u,&out)&&out==1u);
 CHECK(pc_interval_mean_u32(b,3u,&out)&&out==UINT32_C(2863311530));
 CHECK(!pc_interval_mean_u32(a,0u,&out)&&out==UINT32_C(2863311530));
 CHECK(!pc_interval_mean_u32(NULL,1u,&out)&&out==UINT32_C(2863311530));
 CHECK(!pc_interval_mean_u32(a,2u,NULL));
 return 0;
}
