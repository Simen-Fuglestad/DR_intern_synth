#include "filter.h"
#include <math.h>
#include "OSC.h"

static float fc;
static uint16_t fs;

static float Ts;
static float r;

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
    fs = 44100;
    Ts = 1.0f/fs;
    fc = 2047; //needed to init LFO
}

void shift(float inp, float* buff, uint16_t buff_len) {
    for (uint8_t i = buff_len - 1; i >= 1; i--) {
        buff[i] = buff[i-1];
    }
    buff[0] = inp;
}

uint16_t filter_res_update(uint32_t in) {
    static float fin;
    fin = ((float)in - 2048)/2048;
    shift(fin, x_buff, X_LEN);
    float next = filter_res_get_next();
    shift(next, y_buff, Y_LEN);

    uint16_t out = next * 2048 + 2048;
    return out;
}

float filter_res_get_next() {
    float x0 = b0 * x_buff[0];
    float x1 = b1 * x_buff[1];
    float x2 = b2 * x_buff[2];

    float y1 = a1 * y_buff[0];

    float y2 = a2 * y_buff[1];

    static volatile float out;
    out = x0 + x1 + x2 - y1 - y2;

    return out;
}

void filter_res_coeff_update() {
    static float r_scaler;
    static float r_mix;

    static float r_index[11] = {
        0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1
    };

    // uint16_t mixer_fc = mixer_get_filter_fc_low();

    // fc = OSC_apply(mixer_fc, 1000, LFO_TREMOLO);
    fc = OSC_res_update();

    // fc = 440;
    r_mix = (float)mixer_get_filter_fc_high()/0xFFF;

    r_scaler = roundf(r_mix/0.1);

    r = r_index[(int)r_scaler];
    if (r > 1) r = 1;

    r = 0.99;

    a1 = -2.0f * r * cosf(2.0f * 3.1415 * fc/44100);
    a2 = r*r;

    b0 = (1 - r*r)/2;
    // b0 = 1;
    b1 = 0;
    b2 = -b0;
}