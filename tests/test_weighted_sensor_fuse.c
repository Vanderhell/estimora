#include "pc_weighted_sensor_fuse_i32.h"
#include <limits.h>
#include <stdint.h>
#define CHECK(e) do { if (!(e)) return __LINE__; } while (0)
int main(void){int32_t out=777,x0,x1;uint32_t w0,w1;
 CHECK(!pc_weighted_sensor_fuse_i32(1,0,2,0,&out)&&out==777);
 CHECK(!pc_weighted_sensor_fuse_i32(1,1,2,1,0));
 CHECK(!pc_weighted_sensor_fuse_i32(1,UINT32_MAX,2,1,&out)&&out==777);
 CHECK(pc_weighted_sensor_fuse_i32(INT32_MIN,0,INT32_MAX,1,&out)&&out==INT32_MAX);
 CHECK(pc_weighted_sensor_fuse_i32(INT32_MIN,1,INT32_MAX,0,&out)&&out==INT32_MIN);
 CHECK(pc_weighted_sensor_fuse_i32(-1,1,0,1,&out)&&out==-1);
 CHECK(pc_weighted_sensor_fuse_i32(0,1,1,1,&out)&&out==1);
 CHECK(pc_weighted_sensor_fuse_i32(INT32_MIN,INT32_MAX,INT32_MAX,INT32_MAX,&out)&&out==-1);
 CHECK(pc_weighted_sensor_fuse_i32(INT32_MIN,INT32_MAX,INT32_MIN,INT32_MAX,&out)&&out==INT32_MIN);
 for(x0=-16;x0<=16;x0++)for(x1=-16;x1<=16;x1++)for(w0=0;w0<=16;w0++)for(w1=0;w1<=16;w1++){
  int32_t a,b,expected;int64_t n,d,q,r;
  if(w0==0&&w1==0){a=777;CHECK(!pc_weighted_sensor_fuse_i32(x0,w0,x1,w1,&a)&&a==777);continue;}
  n=(int64_t)x0*w0+(int64_t)x1*w1;d=(int64_t)w0+w1;q=n/d;r=n%d;
  if(r>=0){if(r*2>=d)++q;}else if(-r*2>=d)--q;
  expected=(int32_t)q;
  CHECK(pc_weighted_sensor_fuse_i32(x0,w0,x1,w1,&a)&&a==expected);
  CHECK(pc_weighted_sensor_fuse_i32(x1,w1,x0,w0,&b)&&b==a);
 }
 return 0;
}
#include <stddef.h>
