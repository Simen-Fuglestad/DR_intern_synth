/*
 * filter_lowpass.c
 *
 *  Created on: Feb 21, 2022
 *      Author: sfs
 */

#include "filter.h"

static const float C1_DEFAULT = 6E-7;

static filter_lp_RC1_t lp_filter;
static bool lp_active;


void filter_lp_RC_init(float gain, float delta_t) {
	if (delta_t >= 1 || delta_t <= 0)
		lp_filter.delta_t = delta_t;
	else
		lp_filter.delta_t = DELTA_T_DEFAULT;

	lp_filter.prev_y = 0;
	lp_filter.gain = gain;
	lp_active = true;
}
void filter_lp_update() {
	static float fc;
	fc = mixer_get_filter_fc_low();//flip scale for more intuitive slider behavior
	lp_filter.fc = fc;
	filter_lp_set_alpha(fc);
}

void filter_lp_RC_deinit() {
	memset(&lp_filter, 0, sizeof(lp_filter));
}

uint16_t filter_lp_RC_get_next(uint16_t x) {
	uint16_t y = lp_filter.alpha * x + (1 - lp_filter.alpha) * lp_filter.prev_y;
	lp_filter.prev_y = y;

	return y;
}

uint16_t filter_lp_RC_get_fc() {
	return lp_filter.fc;
}

void filter_lowpass_RC_set_fc(uint16_t fc) {
	lp_filter.fc = fc;
	filter_lp_set_alpha(fc);
}

void filter_lp_RC_set_R(float R) {
	lp_filter.R = R;
}

void filter_lp_RC_set_C(float C) {
	lp_filter.C = C;
}

void filter_lp_set_alpha(uint16_t fc) {

	lp_filter.alpha = 2 * M_PI * fc * lp_filter.delta_t/(2* M_PI * fc * lp_filter.delta_t + 1);
}

void filter_lp_set_active(bool b) {
	lp_active = b;
}

bool filter_lp_get_active() {
	return lp_active;
}

uint16_t filter_lp_compute_fc(float R, float C) {
	return 1.0f/(2 * M_PI * R * C);
}

float filter_lp_compute_R(float fc, float C) {
	return 1.0f/(2.0f * M_PI * fc * C);
}

float filter_lp_compute_C(float fc, float R) {
	return 1.0f/(2.0f * M_PI * fc * R);
}
