#include "median_network7_i32.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t state=UINT32_C(0x1835feed);
static uint32_t next_word(void){state^=state<<13;state^=state>>17;state^=state<<5;return state;}
static int32_t next_i32(void){uint32_t u=next_word();return u<=(uint32_t)INT32_MAX?(int32_t)u:INT32_MIN+(int32_t)(u-UINT32_C(0x80000000));}
static int compare_i32(const void*l,const void*r){int32_t a=*(const int32_t*)l,b=*(const int32_t*)r;return(a>b)-(a<b);}
static int32_t oracle(const int32_t *v){int32_t x[7];unsigned i;for(i=0;i<7;++i)x[i]=v[i];qsort(x,7,sizeof x[0],compare_i32);return x[3];}
int main(void){int32_t v[7];uint32_t n;unsigned mask;
    for(mask=0;mask<128;++mask){for(unsigned i=0;i<7;++i)v[i]=(mask&(1u<<i))?1:0;assert(pc_median_network7_i32(v[0],v[1],v[2],v[3],v[4],v[5],v[6])==oracle(v));}
    v[0]=INT32_MIN;v[1]=INT32_MAX;v[2]=0;v[3]=INT32_MIN;v[4]=INT32_MAX;v[5]=-1;v[6]=1;
    assert(pc_median_network7_i32(v[0],v[1],v[2],v[3],v[4],v[5],v[6])==oracle(v));
    for(int32_t a=-3;a<=3;++a)for(int32_t b=-3;b<=3;++b)for(int32_t c=-3;c<=3;++c)for(int32_t d=-3;d<=3;++d)
    for(int32_t e=-3;e<=3;++e)for(int32_t f=-3;f<=3;++f)for(int32_t g=-3;g<=3;++g){v[0]=a;v[1]=b;v[2]=c;v[3]=d;v[4]=e;v[5]=f;v[6]=g;assert(pc_median_network7_i32(a,b,c,d,e,f,g)==oracle(v));}
    for(n=0;n<200000;++n){for(unsigned i=0;i<7;++i)v[i]=next_i32();assert(pc_median_network7_i32(v[0],v[1],v[2],v[3],v[4],v[5],v[6])==oracle(v));}return 0;}
