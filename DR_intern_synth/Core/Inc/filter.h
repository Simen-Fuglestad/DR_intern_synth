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

//defines

//type defs
typedef enum {
	LOWPASS, HIGHPASS, BANDPASS, NOTCH
} filter_response_t;

static const float DELTA_T_DEFAULT = 1.0f/10000;

typedef struct {
	uint16_t prev_y;
	float fc;
	float delta_t;
	float R;
	float C;
	float gain;
	float c1;
	float c2;
} filter_lp_RC1_t;

typedef struct {
	uint16_t prev_y;
	uint16_t prev_x;
	float fc;
	float delta_t;
	float R;
	float C;
	float gain;
	float c1;
	float c2;
} filter_hp_RC1_t;

typedef struct {
	filter_response_t response;
	uint8_t order;
	uint16_t* prev_y;
	uint16_t*prev_x;
	float fc;
	float delta_t;
	float gain;
	float* R;
	float* C;
	float* coeff;
} filter_t;

typedef uint16_t (filter_f)(filter_t*, uint16_t);

uint16_t filter_apply(filter_f** filter_functions, filter_t* filters, uint16_t inp, uint8_t nf);
void filter_init(filter_t* filter, filter_response_t response, uint8_t order, float fc, float gain);

// Low pass

void filter_lowpass_RC_init(filter_t* filter, float fc, float gain);

uint16_t filter_lowpass_RC_get_next(filter_t* filter, uint16_t x);

void filter_lowpass_RC_set_R(filter_t* filter,  float R);
void filter_lowpass_RC_set_C(filter_t* filter, float C);
void filter_lowpass_RC_set_coeffs(filter_t* filter, float delta_t, float R, float C);

float filter_lowpass_RC_get_fc(filter_t* filter);
float filter_lowpass_compute_fc(float R, float C);
float filter_lowpass_compute_R(float fc, float C);
float filter_lowpass_compute_C(float fc, float R);

filter_lp_RC1_t filter_lowpass_RC_create(float fc, float gain);

// High pass

void filter_highpass_RC_init(filter_t* filter, float fc, float gain);

uint16_t filter_highpass_RC_get_next(filter_t* filter, uint16_t x);

void filter_highpass_RC_set_R(filter_t* filter,  float R);
void filter_highpass_RC_set_C(filter_t* filter, float C);
void filter_highpass_RC_set_coeffs(filter_t* filter, float delta_t, float R, float C);

float filter_highpass_RC_get_fc(filter_t* filter);
float filter_highpass_compute_fc(float R, float C);
float filter_highpass_compute_R(float fc, float C);
float filter_highpass_compute_C(float fc, float R);

#endif /* INC_FILTER_H_ */
