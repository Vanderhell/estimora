#include <limits.h>
#include "pc_sensor_vote_weighted_i32.h"
int main(void){int32_t o=99;
 if(!pc_sensor_vote_weighted_i32(1,2,1,2,2,1,&o)||o!=1)return 1;
 if(!pc_sensor_vote_weighted_i32(1,UINT32_MAX,2,1,3,0,&o)||o!=1)return 2;
 o=99;if(pc_sensor_vote_weighted_i32(1,1,2,1,3,1,&o)||o!=99)return 3;
 if(!pc_sensor_vote_weighted_i32(INT32_MIN,UINT32_MAX,INT32_MAX,0,INT32_MIN,UINT32_MAX,&o)||o!=INT32_MIN)return 4;
 o=99;if(pc_sensor_vote_weighted_i32(1,0,2,0,3,0,&o)||o!=99)return 5;
 if(pc_sensor_vote_weighted_i32(1,1,1,1,1,1,0))return 6;
 if(!pc_sensor_vote_weighted_i32(INT32_MAX,3,INT32_MIN,1,7,0,&o)||o!=INT32_MAX)return 7;
 return 0;}