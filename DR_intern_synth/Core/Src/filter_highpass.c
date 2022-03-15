/*
 * filter_highpass.c
 *
 *  Created on: Mar 11, 2022
 *      Author: sfs
 */


#include "filter.h"
static const float C1_DEFAULT = 6E-5;

void filter_highpass_RC_init(filter_t* filter, float delta_t, float fc, float gain) {
	static uint16_t y[0xFF];
	static uint16_t x[0xFF];
	static float R[0xFF];
	static float C[0xFF];
	static float coeff[0xFF];

	float R1 = filter_highpass_compute_R(fc, C1_DEFAULT);
	R[0] = R1;
	C[0] = C1_DEFAULT;

	filter->response = HIGHPASS;
	filter->prev_y = y;
	filter->prev_x = x;

	filter->R = R;
	filter->C = C;
	filter->gain = gain;

	filter->coeff = coeff;
	filter_highpass_RC_set_coeffs(filter, delta_t, R1, C1_DEFAULT);

	filter->fc = fc;
	filter->delta_t = delta_t;
}

uint16_t filter_highpass_RC_get_next(filter_t* filter, uint16_t x) {
	uint16_t y = filter->coeff[0] * filter->prev_y[0] + filter->coeff[1] * (x - filter->prev_x[0]);
	filter->prev_y[0] = y;
	filter->prev_x[0] = x;
	return y;
}

void filter_highpass_RC_set_R(filter_t* filter,  float R) {
	float new_fc = filter_highpass_compute_fc(R, filter->C[0]);
	if (new_fc > 0) {
		filter->R[0] = R;
		filter->fc = new_fc;
		filter_highpass_RC_set_coeffs(filter, filter->delta_t, R, filter->C[0]);
	}

}
void filter_highpass_RC_set_C(filter_t* filter, float C){
	float new_fc = filter_highpass_compute_fc(filter->R[0], C);
	if (new_fc > 0) {
		filter->C[0] = C;
		filter->fc = new_fc;
		filter_highpass_RC_set_coeffs(filter, filter->delta_t, filter->R[0], C);
	}
}

void filter_highpass_RC_set_coeffs(filter_t* filter, float delta_t, float R, float C) {
	filter->coeff[0] = (R*C)/(R*C + delta_t);
	filter->coeff[1] = filter->coeff[0];
}

float filter_highpass_RC_get_fc(filter_t* filter) {
	return filter->fc;
}

float filter_highpass_compute_fc(float R, float C) {
	return 1.0f/(2 * M_PI * R * C);

}
float filter_highpass_compute_R(float fc, float C) {
	return 1.0f/(2.0f * M_PI * fc * C);
}

float filter_highpass_compute_C(float fc, float R) {
	return 1.0f/(2.0f * M_PI * fc * R);
}