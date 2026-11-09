#include "mad3_i32.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>

static uint32_t state=UINT32_C(0x1813abcd);
static uint32_t next_word(void){state^=state<<13;state^=state>>17;state^=state<<5;return state;}
static int32_t next_i32(void){uint32_t u=next_word();return u<=(uint32_t)INT32_MAX?(int32_t)u:INT32_MIN+(int32_t)(u-UINT32_C(0x80000000));}
static uint32_t oracle(int32_t a,int32_t b,int32_t c){
    int64_t x[3]={a,b,c}, d[3],t; int i,j;
    for(i=0;i<3;++i)for(j=i+1;j<3;++j)if(x[i]>x[j]){t=x[i];x[i]=x[j];x[j]=t;}
    for(i=0;i<3;++i){d[i]=x[i]-x[1];if(d[i]<0)d[i]=-d[i];}
    for(i=0;i<3;++i)for(j=i+1;j<3;++j)if(d[i]>d[j]){t=d[i];d[i]=d[j];d[j]=t;}
    return (uint32_t)d[1];
}
int main(void){int32_t a,b,c,i,j,k;uint32_t got;
    assert(!pc_mad3_i32(0,1,2,NULL));
    assert(pc_mad3_i32(INT32_MIN,0,INT32_MAX,&got)&&got==INT32_MAX);
    assert(pc_mad3_i32(0,0,100,&got)&&got==0);
    assert(pc_mad3_i32(0,10,11,&got)&&got==1);
    for(i=-20;i<=20;++i)for(j=-20;j<=20;++j)for(k=-20;k<=20;++k){assert(pc_mad3_i32(i,j,k,&got));assert(got==oracle(i,j,k));}
    for(i=0;i<200000;++i){a=next_i32();b=next_i32();c=next_i32();assert(pc_mad3_i32(a,b,c,&got));assert(got==oracle(a,b,c));}
    return 0;
}