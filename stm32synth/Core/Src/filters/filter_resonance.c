#include "filter.h"
#include <math.h>
#include "modulator.h"
#include "envelope.h"

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

static env_t filter_env;

env_t filter_get_env() {
	return filter_env;
}

void shift(float inp, float* buff, uint16_t buff_len);

void filter_res_init() {
    fs = 22050/2; //output updates every 2 samples
    Ts = 1.0f/fs;
    r = 0.9; //tight resonance peak that still yields frequencies in close proximity
    fc = 1000;
}

void shift(float inp, float* buff, uint16_t buff_len) {
    for (uint8_t i = buff_len - 1; i >= 1; i--) {
        buff[i] = buff[i-1];
    }
    buff[0] = inp;
}

float filter_res_update(float in) {
    uint16_t mixer_fc = mixer_get_filter_fc_low();
    float sweep = (float)mixer_get_filter_fc_high()/0xff;

    float sweepf = modulator_get_next(sweep, wavetable_get_ptr(SINE)) * N_WT_SAMPLES;
    float* wt = wavetable_get_ptr(SINE);
    LFO_tracker += sweepf;
    if (LFO_tracker >= N_WT_SAMPLES) {
    	LFO_tracker -= N_WT_SAMPLES;
    } else if (LFO_tracker < 0) {
    	LFO_tracker += N_WT_SAMPLES;
    }
    fc = mixer_fc * wt[(uint16_t)LFO_tracker];

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

    a1 = -2.0f * r * cosf(2.0f * 3.141f * fc * Ts);
    a2 = r*r;

    b0 = (1.0f - r*r)/2.0f;
    b1 = 0;
    b2 = -b0;
}
