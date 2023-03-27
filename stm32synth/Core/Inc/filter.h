/*
 * filter.h
 *
 *  Created on: Feb 15, 2022
 *      Author: sfs
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "mixer.h"

static const float DELTA_T_DEFAULT = 1.0f/(48000);

typedef struct {
	float prev_y;
	float fc;
	float delta_t;
	float R;
	float C;
	float gain;
	float alpha;
} filter_lp_RC1_t;

typedef struct {
	uint16_t prev_y;
	uint16_t prev_x;
	uint16_t fc;
	float delta_t;
	float R;
	float C;
	float gain;
	float alpha;
} filter_hp_RC1_t;

// Generic

void filter_update(void);

// Low pass

void filter_lp_RC_init(float gain, float delta_t);
void filter_lp_RC_deinit(void);

void filter_lp_update(void);

void filter_lp_RC_set_R(float R);
void filter_lp_RC_set_C(float C);
void filter_lp_set_alpha(float fc);
void filter_lp_set_active(bool b);

float filter_lp_RC_get_next(float x);
float filter_lp_RC_get_fc(void);
bool filter_lp_get_active(void);

float filter_lp_compute_fc(float R, float C);
float filter_lp_compute_R(float fc, float C);
float filter_lp_compute_C(float fc, float R);

// High pass

void filter_hp_RC_init(float gain, float delta_t);
void filter_hp_RC_deinit(void);

void filter_hp_update(void);

void filter_hp_RC_set_R(float R);
void filter_hp_RC_set_C(float C);
void filter_hp_set_alpha(uint16_t fc);
void filter_hp_set_active(bool b);

uint16_t filter_hp_RC_get_next(uint16_t x);
uint16_t filter_hp_RC_get_fc(void);
bool filter_hp_RC_get_active(void);

//resonant

void filter_res_init();
float filter_res_update(float in);
void filter_res_coeff_update();
float filter_res_get_next();
#endif /* INC_FILTER_H_ */
