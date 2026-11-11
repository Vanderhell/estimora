#include <Arduino.h>
extern "C" {
#include "pc_hist_cdf_u32.h"
#include "pc_hist_clip_redistribute_u32.h"
#include "pc_hist_dynamic_range_u32.h"
#include "pc_hist_entropy_norm_q.h"
#include "pc_hist_equalize_u8.h"
#include "pc_hist_high_percentile_u32.h"
#include "pc_hist_low_percentile_u32.h"
#include "pc_hist_valley_count_u32.h"
#include "pc_histogram_total_u32.h"
#include "pc_image_hist_bin_u32.h"
#include "pc_mean3_i32.h"
#include "pc_mean4_i32.h"
#include "pc_mean5_i32.h"
#include "pc_median_network5_i32.h"
#include "pc_median_network7_i32.h"
#include "pc_running_median3_i32.h"
#include "pc_sensor_triple_median_i32.h"
#include "pc_trimmed_mean5_i32.h"
#include "pc_weighted_sensor_fuse_i32.h"
}

static uint32_t h[256], valley[256], cdf[256], clipped[256];
static uint8_t lut[256];
static pc_running_median3_i32_state running;
static volatile int32_t sink_i;
static volatile uint32_t sink_u;
static const uint32_t reps=1000;
static bool pass=true;
static void result(uint32_t id, bool ok, uint32_t cycles) {
  Serial.print("API "); Serial.print(id); Serial.print(" cycles="); Serial.print(cycles);
  Serial.println(ok ? " PASS" : " FAIL"); if (!ok) pass=false;
}
#define BENCH(ID, EXPR, OK) do { uint32_t t0=ESP.getCycleCount(); for(uint32_t k=0;k<reps;k++){ EXPR; } uint32_t dt=(uint32_t)(ESP.getCycleCount()-t0)/reps; result(ID,(OK),dt); } while(0)
void setup() {
  Serial.begin(115200); delay(1200);
  h[10]=3; h[20]=1; valley[0]=4; valley[1]=1; valley[2]=4;
  uint32_t u=0; int32_t s=0; bool ok;
  ok=pc_histogram_total_u32(h,256,&u) && u==4; BENCH(1,sink_u=pc_histogram_total_u32(h,256,&u)?u:0, u==4 && sink_u==4);
  ok=pc_hist_cdf_u32(h,256,cdf) && cdf[10]==3 && cdf[20]==4; BENCH(2,sink_u=pc_hist_cdf_u32(h,256,cdf)?cdf[255]:0, ok && sink_u==4);
  ok=pc_hist_clip_redistribute_u32(h,2,clipped) && clipped[0]==1 && clipped[10]==2 && clipped[20]==1; BENCH(3,sink_u=pc_hist_clip_redistribute_u32(h,2,clipped)?clipped[10]:0, ok && sink_u==2);
  uint32_t range=0; ok=pc_hist_dynamic_range_u32(h,&range) && range==10; BENCH(4,sink_u=pc_hist_dynamic_range_u32(h,&range)?range:0,ok && sink_u==10);
  uint32_t entropy=0; ok=pc_hist_entropy_norm_q(h,24,&entropy) && entropy>1690000 && entropy<1710000; BENCH(5,sink_u=pc_hist_entropy_norm_q(h,24,&entropy)?entropy:0,ok && sink_u>1690000 && sink_u<1710000);
  ok=pc_hist_equalize_u8(h,lut) && lut[10]==0 && lut[20]==255; BENCH(6,sink_u=pc_hist_equalize_u8(h,lut)?lut[20]:0,ok && sink_u==255);
  uint32_t lo=0,hi=0; ok=pc_hist_low_percentile_u32(h,0,&lo)&&lo==10 && pc_hist_high_percentile_u32(h,0,&hi)&&hi==20;
  BENCH(7,sink_u=pc_hist_low_percentile_u32(h,0,&lo)?lo:999,ok && sink_u==10);
  BENCH(8,sink_u=pc_hist_high_percentile_u32(h,0,&hi)?hi:999,ok && sink_u==20);
  uint32_t valleys=0; ok=pc_hist_valley_count_u32(valley,&valleys)&&valleys==1; BENCH(9,sink_u=pc_hist_valley_count_u32(valley,&valleys)?valleys:999,ok && sink_u==1);
  uint32_t bin=0; ok=pc_image_hist_bin_u32(255,1000,&bin)&&bin==996; BENCH(10,sink_u=pc_image_hist_bin_u32(255,1000,&bin)?bin:0,ok && sink_u==996);
  ok=pc_mean3_i32(1,2,4)==2; BENCH(11,sink_i=pc_mean3_i32(1,2,4),ok && sink_i==2);
  ok=pc_mean4_i32(0,1,2,3)==2; BENCH(12,sink_i=pc_mean4_i32(0,1,2,3),ok && sink_i==2);
  ok=pc_mean5_i32(1,2,3,4,9)==4; BENCH(13,sink_i=pc_mean5_i32(1,2,3,4,9),ok && sink_i==4);
  ok=pc_median_network5_i32(9,-1,3,2,4)==3; BENCH(14,sink_i=pc_median_network5_i32(9,-1,3,2,4),ok && sink_i==3);
  ok=pc_median_network7_i32(9,-1,3,2,4,8,0)==3; BENCH(15,sink_i=pc_median_network7_i32(9,-1,3,2,4,8,0),ok && sink_i==3);
  memset(&running,0,sizeof(running)); pc_running_median3_i32(5,&running); pc_running_median3_i32(1,&running);
  ok=pc_running_median3_i32(3,&running)&&running.median==3; BENCH(16,sink_u=pc_running_median3_i32(3,&running)?(uint32_t)running.median:0,ok && sink_u==3);
  ok=pc_sensor_triple_median_i32(3,1,2)==2; BENCH(17,sink_i=pc_sensor_triple_median_i32(3,1,2),ok && sink_i==2);
  ok=pc_trimmed_mean5_i32(1,2,3,4,100)==3; BENCH(18,sink_i=pc_trimmed_mean5_i32(1,2,3,4,100),ok && sink_i==3);
  ok=pc_weighted_sensor_fuse_i32(-3,1,1,1,&s)&&s==-1; BENCH(19,sink_u=pc_weighted_sensor_fuse_i32(-3,1,1,1,&s)?(uint32_t)s:0,ok && (int32_t)sink_u==-1);
  Serial.println(pass ? "ESTIMORA_ALL19_PASS" : "ESTIMORA_ALL19_FAIL");
}
void loop() { delay(1000); }
