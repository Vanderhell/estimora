#include <stdint.h>
#include "pc_sensor_triple_median_i32.h"
int main(void){static const int32_t x[]={INT32_MIN,INT32_MIN+1,-1,0,1,INT32_MAX-1,INT32_MAX};unsigned i,j,k;for(i=0;i<7;i++)for(j=0;j<7;j++)for(k=0;k<7;k++){int32_t got=pc_sensor_triple_median_i32(x[i],x[j],x[k]);if(got<x[i]&&got<x[j]&&got<x[k])return 1;if(got>x[i]&&got>x[j]&&got>x[k])return 2;if(got!=x[i]&&got!=x[j]&&got!=x[k])return 3;if(got!=pc_sensor_triple_median_i32(x[k],x[j],x[i]))return 4;}return 0;}
