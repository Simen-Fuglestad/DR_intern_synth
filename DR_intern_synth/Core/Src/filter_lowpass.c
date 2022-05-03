/*
 * filter_lowpass.c
 *
 *  Created on: Feb 21, 2022
 *      Author: sfs
 */

#include "filter.h"
#include "mixer.h"

static filter_lp_RC1_t lp_filter;
static bool lp_active;
static const float C1_DEFAULT = 6E-7;

void filter_lowpass_RC_init(float fc, float gain) {
	lp_filter.C = C1_DEFAULT;
	lp_filter.R = filter_lowpass_compute_R(fc, C1_DEFAULT);
	filter_lowpass_RC_set_coeffs(fc);
	lp_filter.gain = gain;
	lp_active = true;
}

void filter_lowpass_update() {
	float fc = mixer_get_filter_fc_low();
	lp_filter.fc = fc;
	filter_lowpass_RC_set_coeffs(fc);
}

void filter_lowpass_RC_deinit() {
	lp_filter.c1 = 0;
	lp_filter.c2 = 0;
	lp_filter.C = 0;
	lp_filter.R = 0;
	lp_filter.fc = 0;
	lp_filter.gain = 0;
}

uint16_t filter_lowpass_RC_get_next(uint16_t x) {
	if (lp_active) {
		uint16_t filtered = lp_filter.c1 * x + lp_filter.c2 * lp_filter.prev_y;
		lp_filter.prev_y = filtered;

		return filtered;
	}
	return x;
}

uint16_t filter_lowpass_RC_get_fc() {
	return lp_filter.fc;
}

void filter_lowpass_RC_set_fc(uint16_t fc) {
	lp_filter.fc = fc;
	filter_lowpass_RC_set_coeffs(fc);
}

void filter_lowpass_RC_set_R(float R) {
	lp_filter.R = R;
//	float new_fc = filter_lowpass_compute_fc(R, filter->C[0]);
//	if (new_fc > 0) {
//		filter->R[0] = R;
//		filter->fc = new_fc;
//		filter_lowpass_RC_set_coeffs(filter, filter->delta_t, R, filter->C[0]);
//	}
}

void filter_lowpass_RC_set_C(float C) {

	lp_filter.C = C;
//	float new_fc = filter_lowpass_compute_fc(filter->R[0], C);
//	if (new_fc > 0) {
//		filter->C[0] = C;
//		filter->fc = new_fc;
//		filter_lowpass_RC_set_coeffs(filter, filter->delta_t, filter->R[0], C);
//	}
}

void filter_lowpass_RC_set_coeffs(uint16_t fc) {
//	lp_filter.c1 = delta_t/(delta_t + R*C);
	lp_filter.c1 = 2 * M_PI * fc * DELTA_T_DEFAULT/(2* M_PI * fc * DELTA_T_DEFAULT + 1);
	lp_filter.c2 = (1 - lp_filter.c1);
}

void filter_lowpass_set_active(bool b) {
	lp_active = b;
}

bool filter_lowpass_get_active() {
	return lp_active;
}

uint16_t filter_lowpass_compute_fc(float R, float C) {
	return 1.0f/(2 * M_PI * R * C);
}

float filter_lowpass_compute_R(float fc, float C) {
	return 1.0f/(2.0f * M_PI * fc * C);
}

float filter_lowpass_compute_C(float fc, float R) {
	return 1.0f/(2.0f * M_PI * fc * R);
}
