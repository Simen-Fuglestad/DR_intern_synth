/*
 * DSP_utils.c
 *
 *  Created on: Feb 28, 2022
 *      Author: sfs
 */

#include "DSP_utils.h"
#include <math.h>

float dsp_utils_compute_fc_lowpass(float R, float C) {
	float denom = 2.0f * M_PI * R * C;
	return 1.0f/(2.0f * M_PI * R * C);
}

float dsp_utils_compute_R_lowpass(float fc, float C) {
	return 1.0f/(2 * M_PI * fc * C);
}

float dsp_utils_compute_C_lowpass(float fc, float R) {
	return 1.0f/(2 * M_PI * R * fc);
}

void dsp_utils_set_lp(float fc, uint16_t N, uint16_t* out_wave, uint16_t ref_v) {
	float filter_coeffs[N];
	dsp_utils_window_sampling_lp(fc, N, filter_coeffs);

	float filter_idft[N];
	dsp_utils_idft_odd(filter_coeffs, filter_idft, N);
	dsp_utils_apply_filter(filter_idft, N, out_wave, ref_v);
}

void dsp_utils_idft_odd(float* x, float* y, uint16_t N) {
    float alpha = (N - 1)/2;
    for (int n = 0; n < N; n++) {
        float v = 0;
        for (int k = 1; k <= alpha; k++) {
            v = v + x[k] * cos(2 * M_PI * k * (n - alpha)/N);
        }
        v = (2*v + x[0])/N;
        y[n] = v;
    }
}

void dsp_utils_window_sampling_lp(float fc, uint16_t N, float* out) {
    float fs = 2 * 2 * fc;
    float f_res = fs/(float)N;
	for (int i = 0; i < N; i++) {
		if (f_res * i < fc) {
			out[i] = 1;
		} else {
			out[i] = 0;
		}
	}
}

void dps_utils_window_sampling_hp(float wc, uint16_t N, uint16_t* out) {

}

void dsp_utils_apply_filter(float* filter, uint16_t filter_len, uint16_t* out, uint16_t ref_v) {
    for (int i = 0; i < filter_len; i++) {
        float scaled_val = filter[i] * ((ref_v + 1)/2);
        uint16_t new_val = (uint16_t)(scaled_val + ref_v/2);
        out[i] = new_val;
    }
}
