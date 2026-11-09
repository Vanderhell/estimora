/* Held-out cross-family validation source; dependency is outside this product. */
#include <assert.h>
#include <limits.h>
#include <stdint.h>
static int32_t oracle(int32_t a,int32_t b,int32_t c){int32_t lo=a<b?a:b,hi=a>b?a:b;int32_t mid=hi<c?hi:c;return lo>mid?lo:mid;}
int main(void){int a,b,c;int32_t vals[]={INT32_MIN,INT32_MIN+1,-1,0,1,INT32_MAX-1,INT32_MAX};unsigned i,j,k;
 for(a=-128;a<128;++a)for(b=-128;b<128;++b)for(c=-128;c<128;++c){int32_t got=pc_predict_median3_i32(a,b,c);assert(got==oracle(a,b,c));assert(got==pc_predict_median3_i32(c,a,b));}
 for(i=0;i<7;++i)for(j=0;j<7;++j)for(k=0;k<7;++k)assert(pc_predict_median3_i32(vals[i],vals[j],vals[k])==oracle(vals[i],vals[j],vals[k]));
 return 0;}