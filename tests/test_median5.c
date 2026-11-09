#include "median_network5_i32.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t state=UINT32_C(0x1834feed);
static uint32_t next_word(void){state^=state<<13;state^=state>>17;state^=state<<5;return state;}
static int32_t next_i32(void){uint32_t u=next_word();return u<=(uint32_t)INT32_MAX?(int32_t)u:INT32_MIN+(int32_t)(u-UINT32_C(0x80000000));}
static int compare_i32(const void*l,const void*r){int32_t a=*(const int32_t*)l,b=*(const int32_t*)r;return(a>b)-(a<b);}
static int32_t oracle(int32_t a,int32_t b,int32_t c,int32_t d,int32_t e){int32_t x[5]={a,b,c,d,e};qsort(x,5,sizeof x[0],compare_i32);return x[2];}
int main(void){int32_t a,b,c,d,e;uint32_t n;
    assert(pc_median_network5_i32(INT32_MIN,INT32_MAX,0,INT32_MIN,INT32_MAX)==0);
    assert(pc_median_network5_i32(9,9,9,-1,100)==9);
    for(a=-6;a<=6;++a)for(b=-6;b<=6;++b)for(c=-6;c<=6;++c)for(d=-6;d<=6;++d)for(e=-6;e<=6;++e)
        assert(pc_median_network5_i32(a,b,c,d,e)==oracle(a,b,c,d,e));
    for(n=0;n<200000;++n){a=next_i32();b=next_i32();c=next_i32();d=next_i32();e=next_i32();assert(pc_median_network5_i32(a,b,c,d,e)==oracle(a,b,c,d,e));}
    return 0;}
