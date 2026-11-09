#include "pc_running_median3_i32.h"
#include <stddef.h>
static int32_t median3(int32_t a, int32_t b, int32_t c)
{
    int32_t t;
    if (a > b) { t=a; a=b; b=t; }
    if (b > c) { t=b; b=c; c=t; }
    if (a > b) { t=a; a=b; b=t; }
    return b;
}
bool rm_running_median3_i32(int32_t sample, rm_running_median3_i32_state *state)
{
    if (state == NULL || state->count > 3u || state->next > 2u ||
        (state->count < 3u && state->next != state->count)) return false;
    state->samples[state->next] = sample;
    if (state->count < 2u) {
        ++state->count;
        ++state->next;
        state->median = sample;
    } else if (state->count == 2u) {
        state->count = 3u;
        state->next = 0u;
        state->median = median3(state->samples[0], state->samples[1], state->samples[2]);
    } else {
        state->next = (uint8_t)((state->next + 1u) % 3u);
        state->median = median3(state->samples[0], state->samples[1], state->samples[2]);
    }
    return true;
}
