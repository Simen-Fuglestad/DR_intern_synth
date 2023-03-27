#include "filter.h"
#include <math.h>
#include "OSC.h"
#include "modulator.h"

static float fc;
static uint16_t fs;

static float Ts;
static float r;

static float LFO_tracker;

static float a1;
static float a2;

static float b0;
static float b1;
static float b2;


#define Y_LEN 2
#define X_LEN 3

static float y_buff[Y_LEN];
static float x_buff[X_LEN];

void shift(float inp, float* buff, uint16_t buff_len);

void filter_res_init() {
    fs = 44100/4; //output updates every 4 samples
    Ts = 1.0f/fs;
    fc = 10000; //needed to init LFO
    r = 0.9;
}

void shift(float inp, float* buff, uint16_t buff_len) {
    for (uint8_t i = buff_len - 1; i >= 1; i--) {
        buff[i] = buff[i-1];
    }
    buff[0] = inp;
}

float filter_res_update(float in) {
    uint16_t mixer_fc = mixer_get_filter_fc_low();
    uint16_t sweep = mixer_get_filter_fc_high()/0xF;

    float sweepf = modulator_get_next(sweep, wavetable_get_ptr(SINE))/0x1FF;
    float* wt = wavetable_get_ptr(SINE);
    LFO_tracker += sweepf;
    if (LFO_tracker >= N_WT_SAMPLES) {
    	LFO_tracker -= N_WT_SAMPLES;
    }
    fc = mixer_fc * wt[(uint16_t)LFO_tracker];
//    float next
//    float* wt = wavetable_get_ptr(SINE);

//    float osc = wt[(uint16_t)LF_TRACKER];
//    fc = fcf * sweepf;

//    fc = modulator_get_next(fabs(fc) * sweepf, wavetable_get_ptr(SINE));
    //fc = OSC_apply(sweep, mixer_fc, LFO_TREMOLO);
    shift(in, x_buff, X_LEN);
    float next = filter_res_get_next();
    shift(next, y_buff, Y_LEN);

    return next;
}

float filter_res_get_next() {
    float x0 = b0 * x_buff[0];
    float x1 = b1 * x_buff[1];
    float x2 = b2 * x_buff[2];

    float y1 = a1 * y_buff[0];

    float y2 = a2 * y_buff[1];

    static float out;
    out = x0 + x1 + x2 - y1 - y2;

    return out;
}

void filter_res_coeff_update() {

    float tmp_fc_ts = fc*Ts;
    a1 = -2.0f * r * cosf(2.0f * 3.141f * fc * Ts);
    a2 = r*r;

    b0 = (1.0f - r*r)/2.0f;
    // b0 = 1;
    b1 = 0;
    b2 = -b0;
}
