/*
 * filter_highpass.c
 *
 *  Created on: Mar 11, 2022
 *      Author: sfs
 */

/*
 * NOTE: OBSOLETE. This filter uses old unsigned implementation method and will not work as is
 */

#include "filter.h"

static const float C1_DEFAULT = 6E-6;

static filter_hp_RC1_t hp_filter;
static bool hp_active;

uint16_t filter_hp_compute_fc(float R, float C);
float filter_hp_compute_R(float fc, float C);
float filter_hp_compute_C(float fc, float R);

void filter_hp_RC_init(float gain, float delta_t) {
	if (delta_t >= 1 || delta_t <= 0)
		hp_filter.delta_t = DELTA_T_DEFAULT;
	else
		hp_filter.delta_t = delta_t;

	hp_filter.prev_y = 0;
	hp_filter.gain = gain;
	hp_active = true;
}

void filter_hp_update() {
	static float fc;
	fc = mixer_get_filter_fc_high() >> 4; //caps cutoff at 0xFFF >> 4 == 511Hz
	hp_filter.fc = fc;
	filter_hp_set_alpha(fc);
}

void filter_hp_RC_deinit() {
	memset(&hp_filter, 0, sizeof(hp_filter));
}

uint16_t filter_hp_RC_get_next(uint16_t x) {
//	if (hp_filter.fc > 15) {
		float c1 = hp_filter.alpha * (float)hp_filter.prev_y;
		float c2 = hp_filter.alpha * (float)(x - hp_filter.prev_x);
//		uint16_t y = hp_filter.alpha * hp_filter.prev_y + hp_filter.alpha * (x - hp_filter.prev_x);

		uint16_t y = c1 + c2;
		hp_filter.prev_x = x;
		hp_filter.prev_y = y;

		return y;
//	}
//	return x;
}

void filter_hp_RC_set_R(float R) {
	hp_filter.R = R;
}

void filter_hp_RC_set_C(float C){
	hp_filter.C = C;
}

void filter_hp_set_alpha(uint16_t fc) {
	hp_filter.alpha = 1.0f/(2.0f * M_PI * hp_filter.delta_t*((float)fc) + 1.0f);
//	hp_filter.alpha = 2 * M_PI * fc * hp_filter.delta_t/(2* M_PI * fc * hp_filter.delta_t + 1);
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
