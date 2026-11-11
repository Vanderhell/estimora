#ifdef NDEBUG
#undef NDEBUG
#endif
#include "pc_mean5_i32.h"
#include <assert.h>
#include <stdint.h>
int main(void) {
    int32_t a,b,c,d,e;
    assert(pc_mean5_i32(INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN)==INT32_MIN);
    assert(pc_mean5_i32(INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX)==INT32_MAX);
    assert(pc_mean5_i32(0,0,0,0,3)==1 && pc_mean5_i32(0,0,0,0,-3)==-1);
    for(a=-4;a<=4;++a) for(b=-4;b<=4;++b) for(c=-4;c<=4;++c) for(d=-4;d<=4;++d) for(e=-4;e<=4;++e) {
        int64_t sum=(int64_t)a+b+c+d+e;
        int32_t x=pc_mean5_i32(a,b,c,d,e);
        int64_t rem=(int64_t)5*x-sum;
        assert(rem>=-2 && rem<=2);
        assert(pc_mean5_i32(-a,-b,-c,-d,-e)==-x);
    }
    return 0;
}
#include <stddef.h>
