#include "trimmed_mean5_i32.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>

static uint32_t state=UINT32_C(0x1815beef);
static uint32_t next_word(void){state^=state<<13;state^=state>>17;state^=state<<5;return state;}
static int32_t next_i32(void){uint32_t u=next_word();return u<=(uint32_t)INT32_MAX?(int32_t)u:INT32_MIN+(int32_t)(u-UINT32_C(0x80000000));}
static int32_t oracle(int32_t a,int32_t b,int32_t c,int32_t d,int32_t e){
    int64_t x[5]={a,b,c,d,e},sum=0,t,q,r;int i,j;
    for(i=0;i<5;++i)for(j=i+1;j<5;++j)if(x[i]>x[j]){t=x[i];x[i]=x[j];x[j]=t;}
    for(i=1;i<4;++i)sum+=x[i];
    q=sum/3;r=sum%3;
    if(r>=2)++q;else if(r<=-2)--q;
    return (int32_t)q;
}
int main(void){int32_t a,b,c,d,e,i,j,k,l,m;
    assert(pc_trimmed_mean5_i32(INT32_MIN,INT32_MIN,0,INT32_MAX,INT32_MAX)==0);
    assert(pc_trimmed_mean5_i32(0,1,2,3,100)==2);
    assert(pc_trimmed_mean5_i32(-100,0,1,2,3)==1);
    for(i=-4;i<=4;++i)for(j=-4;j<=4;++j)for(k=-4;k<=4;++k)
    for(l=-4;l<=4;++l)for(m=-4;m<=4;++m)
        assert(pc_trimmed_mean5_i32(i,j,k,l,m)==oracle(i,j,k,l,m));
    for(i=0;i<200000;++i){a=next_i32();b=next_i32();c=next_i32();d=next_i32();e=next_i32();
        assert(pc_trimmed_mean5_i32(a,b,c,d,e)==oracle(a,b,c,d,e));}
    return 0;
}
