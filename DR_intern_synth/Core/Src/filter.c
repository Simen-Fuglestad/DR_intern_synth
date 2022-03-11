/*
 * filter.c
 *
 *  Created on: Feb 21, 2022
 *      Author: sfs
 */

#include "filter.h"
#include <math.h>
#include <string.h>
#include "wavetable.h"

uint16_t (*test_f[2])(uint16_t) = {
		filter_RC_lp_get_next,
};

uint16_t filter_apply_all(filter_f** filter_functions, filter_t* filters, uint16_t inp, uint8_t nf) {
	uint16_t out = inp;
	for (uint8_t i = 0; i < nf; i++) {
		out = (*filter_functions)(&filters[i], out);
	}
	return out;
}

uint16_t filter_RC_lp_get_next(filter_t* filter, uint16_t x) {
	uint16_t y = filter->coeff[0] * x + filter->coeff[1] * filter->prev_y;
	filter->prev_y = y;

	return y;
}

void filter_init_RC_lowpass(
		filter_t* filter, float delta_t, float fc, float gain) {
	static float R[0xFF];
	static float C[0xFF];
	static float coeff[0xFF];

	//add check to order of filter here

	float C1 = 6E-5;
	float R1 = filter_compute_R(LOWPASS, 1, fc, C1);
	R[0] = R1;
	C[0] = C1;

	filter->response = LOWPASS;
	filter->prev_y = 0;
	filter->R = R;
	filter->C = C;
	filter->gain = gain;

	coeff[0] = delta_t/(delta_t + R1*C1);

	coeff[1] = (R1 * C1) / (delta_t + R1 * C1);

	filter->coeff = coeff;

	filter->fc = 1/(2 * M_PI * R1 * C1);
	filter->delta_t = delta_t;
}

float filter_get_fc(filter_t* filter) {
	return filter->fc;
}

void filter_set_R_lp(filter_t* filter,  float R, uint8_t ri) {
	float new_fc = filter_compute_fc_lp(R, filter->C[0]);
	if (new_fc > 0) {
		filter->R[ri] = R;
		filter->fc = new_fc;
		filter_set_coeffs_lp(filter, filter->delta_t, R, filter->C[0]);
	}
}

void filter_set_C_lp(filter_t* filter, float C, uint8_t ci) {
	float new_fc = filter_compute_fc_lp(filter->R[0], C);
	if (new_fc > 0) {
		filter->C[ci] = C;
		filter->fc = new_fc;
		filter_set_coeffs_lp(filter, filter->delta_t, filter->R[0], C);
	}
}
void filter_set_coeffs_lp(filter_t* filter, float delta_t, float R, float C) {
	filter->coeff[0] = delta_t/(delta_t + R*C);
	filter->coeff[1] = (R * C) / (delta_t + R * C);
}

float filter_compute_fc_lp(float R, float C) {
	return 1.0f/(2 * M_PI * R * C);
}

float filter_compute_R_lp(float fc, float C) {
	return 1.0f/(2.0f * M_PI * fc * C);
}

float filter_compute_C_lp(float fc, float R) {
	return 1.0f/(2.0f * M_PI * fc * R);
}

float filter_compute_R(filter_response_t ft, uint8_t order, float fc, float C) {
	float R = -1;
	switch (ft) {
	case LOWPASS:
		if (order == 1) {
			R = filter_compute_R_lp(fc, C);
		}
		break;

	case HIGHPASS:
		break;

	case BANDPASS:
		break;

	case NOTCH:
		break;

	default:
		break;
	}
	return R;
}

