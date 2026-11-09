#include "mad5_i32.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>

static uint32_t state=UINT32_C(0x1814abcd);
static uint32_t next_word(void){state^=state<<13;state^=state>>17;state^=state<<5;return state;}
static int32_t next_i32(void){uint32_t u=next_word();return u<=(uint32_t)INT32_MAX?(int32_t)u:INT32_MIN+(int32_t)(u-UINT32_C(0x80000000));}
static uint32_t oracle(int32_t a,int32_t b,int32_t c,int32_t d,int32_t e){
    int64_t x[5]={a,b,c,d,e},r[5],t;int i,j;
    for(i=0;i<5;++i)for(j=i+1;j<5;++j)if(x[i]>x[j]){t=x[i];x[i]=x[j];x[j]=t;}
    for(i=0;i<5;++i){r[i]=x[i]-x[2];if(r[i]<0)r[i]=-r[i];}
    for(i=0;i<5;++i)for(j=i+1;j<5;++j)if(r[i]>r[j]){t=r[i];r[i]=r[j];r[j]=t;}
    return (uint32_t)r[2];
}
int main(void){int32_t a,b,c,d,e,i,j,k,l,m;uint32_t got;
    assert(!pc_mad5_i32(0,1,2,3,4,NULL));
    assert(pc_mad5_i32(INT32_MIN,INT32_MIN,0,INT32_MAX,INT32_MAX,&got));assert(got==(uint32_t)INT32_MAX);
    assert(pc_mad5_i32(4,4,4,4,100,&got)&&got==0);
    for(i=-3;i<=3;++i)for(j=-3;j<=3;++j)for(k=-3;k<=3;++k)
    for(l=-3;l<=3;++l)for(m=-3;m<=3;++m){assert(pc_mad5_i32(i,j,k,l,m,&got));assert(got==oracle(i,j,k,l,m));}
    for(i=0;i<200000;++i){a=next_i32();b=next_i32();c=next_i32();d=next_i32();e=next_i32();
        assert(pc_mad5_i32(a,b,c,d,e,&got));assert(got==oracle(a,b,c,d,e));}
    return 0;
}
