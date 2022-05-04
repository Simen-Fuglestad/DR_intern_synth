/*
 * filter_highpass.c
 *
 *  Created on: Mar 11, 2022
 *      Author: sfs
 */


#include "filter.h"

static const float C1_DEFAULT = 6E-6;

static filter_hp_RC1_t hp_filter;
static bool hp_active;

uint16_t filter_hp_compute_fc(float R, float C);
float filter_hp_compute_R(float fc, float C);
float filter_hp_compute_C(float fc, float R);

void filter_hp_RC_init(float fc, float gain, float delta_t) {
	hp_filter.C = C1_DEFAULT;
	hp_filter.R = filter_hp_compute_R(fc, C1_DEFAULT);

	if (delta_t >= 1 || delta_t <= 0)
		hp_filter.delta_t = DELTA_T_DEFAULT;
	else
		hp_filter.delta_t = delta_t;


	filter_hp_set_alpha(fc);
	hp_filter.prev_y = 0;
	hp_filter.gain = gain;
	hp_active = true;
}

void filter_hp_update() {
	static float fc;
	fc = mixer_get_filter_fc_high();
	filter_hp_set_alpha(fc);
}

void filter_hp_RC_deinit() {
	memset(&hp_filter, 0, sizeof(hp_filter));
}

uint16_t filter_hp_RC_get_next(uint16_t x) {
	uint16_t y = hp_filter.alpha * hp_filter.prev_y + hp_filter.alpha * (x - hp_filter.prev_x);

	hp_filter.prev_x = x;
	hp_filter.prev_y = y;

	return y;
}

void filter_hp_RC_set_R(float R) {
	hp_filter.R = R;
}

void filter_hp_RC_set_C(float C){
	hp_filter.C = C;
}

void filter_hp_set_alpha(uint16_t fc) {
	static float tmp;
	tmp = 2 * M_PI * fc * hp_filter.delta_t;
	hp_filter.alpha = tmp/(tmp + 1);
}

uint16_t filter_hp_RC_get_fc() {
	return hp_filter.fc;
}

uint16_t filter_hp_compute_fc(float R, float C) {
	return 1.0f/(2 * M_PI * R * C);

}
float filter_hp_compute_R(float fc, float C) {
	return 1.0f/(2.0f * M_PI * fc * C);
}

float filter_hp_compute_C(float fc, float R) {
	return 1.0f/(2.0f * M_PI * fc * R);
}
