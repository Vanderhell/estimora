#include "pc_running_median3_i32.h"
#include <limits.h>
#define CHECK(e) do { if (!(e)) return __LINE__; } while (0)
int main(void){pc_running_median3_i32_state s={{0,0,0},0,0,0},old;
 CHECK(pc_running_median3_i32(INT32_MIN,&s)&&s.median==INT32_MIN&&s.count==1);
 CHECK(pc_running_median3_i32(INT32_MAX,&s)&&s.median==INT32_MAX&&s.count==2);
 CHECK(pc_running_median3_i32(0,&s)&&s.median==0&&s.count==3);
 CHECK(pc_running_median3_i32(1,&s)&&s.median==1);
 CHECK(pc_running_median3_i32(2,&s)&&s.median==1);
 old=s; CHECK(!pc_running_median3_i32(3,0)); CHECK(s.next==old.next&&s.count==old.count&&s.median==old.median);
 {pc_running_median3_i32_state bad={{0,0,0},99,2,1},before=bad;CHECK(!pc_running_median3_i32(1,&bad));CHECK(bad.next==before.next&&bad.count==before.count&&bad.median==before.median);}
 {pc_running_median3_i32_state bad={{0,0,0},0,0,4};CHECK(!pc_running_median3_i32(1,&bad));}
 {pc_running_median3_i32_state bad={{0,0,0},0,3,3};CHECK(!pc_running_median3_i32(1,&bad));}
 return 0;}
#include <stddef.h>
