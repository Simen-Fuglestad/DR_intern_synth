/*
 * filter_lowpass.c
 *
 *  Created on: Feb 21, 2022
 *      Author: sfs
 */

#include "filter.h"
#include <math.h>
#include <string.h>
#include "wavetable.h"

static const float C1_DEFAULT = 6E-5;

uint16_t filter_lowpass_RC_get_next(filter_t* filter, uint16_t x) {
	uint16_t y = filter->coeff[0] * x + filter->coeff[1] * filter->prev_y;
	filter->prev_y = y;

	return y;
}

void filter_lowpass_RC_init(
		filter_t* filter, float delta_t, float fc, float gain) {
	static float R[0xFF];
	static float C[0xFF];
	static float coeff[0xFF];

	//add check to order of filter here
	float R1 = filter_lowpass_compute_R(fc, C1_DEFAULT);
	R[0] = R1;
	C[0] = C1_DEFAULT;

	filter->response = LOWPASS;
	filter->prev_y = 0;
	filter->R = R;
	filter->C = C;
	filter->gain = gain;

	coeff[0] = delta_t/(delta_t + R1*C1_DEFAULT);

	coeff[1] = (R1 * C1_DEFAULT) / (delta_t + R1 * C1_DEFAULT);

	filter->coeff = coeff;

	filter->fc = 1/(2 * M_PI * R1 * C1_DEFAULT);
	filter->delta_t = delta_t;
}

float filter_lowpass_RC_get_fc(filter_t* filter) {
	return filter->fc;
}

void filter_lowpass_RC_set_R(filter_t* filter,  float R, uint8_t ri) {
	float new_fc = filter_lowpass_compute_fc(R, filter->C[0]);
	if (new_fc > 0) {
		filter->R[ri] = R;
		filter->fc = new_fc;
		filter_lowpass_RC_set_coeffs(filter, filter->delta_t, R, filter->C[0]);
	}
}

void filter_lowpass_RC_set_C(filter_t* filter, float C, uint8_t ci) {
	float new_fc = filter_lowpass_compute_fc(filter->R[0], C);
	if (new_fc > 0) {
		filter->C[ci] = C;
		filter->fc = new_fc;
		filter_lowpass_RC_set_coeffs(filter, filter->delta_t, filter->R[0], C);
	}
}

void filter_lowpass_RC_set_coeffs(filter_t* filter, float delta_t, float R, float C) {
	filter->coeff[0] = delta_t/(delta_t + R*C);
	filter->coeff[1] = (R * C) / (delta_t + R * C);
}

float filter_lowpass_compute_fc(float R, float C) {
	return 1.0f/(2 * M_PI * R * C);
}

float filter_lowpass_compute_R(float fc, float C) {
	return 1.0f/(2.0f * M_PI * fc * C);
}

float filter_lowpass_compute_C(float fc, float R) {
	return 1.0f/(2.0f * M_PI * fc * R);
}
