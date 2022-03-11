/*
 * filter.h
 *
 *  Created on: Feb 15, 2022
 *      Author: sfs
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

#include <stdint.h>

//defines

//type defs
typedef enum {
	LOWPASS, HIGHPASS, BANDPASS, NOTCH
} filter_response_t;

typedef struct {
	filter_response_t response;
	uint16_t prev_y;
	float fc;
	float delta_t;
	float* R;
	float* C;
	float gain;
	float* coeff;
} filter_t;

typedef uint16_t (filter_f)(filter_t*, uint16_t);

uint16_t filter_RC_lp_get_next(filter_t* filter, uint16_t x);

filter_t filter_RC_lowpass;

void filter_init_RC_lowpass(filter_t* filter, float delta_t, float fc, float gain);

float filter_compute_fc_lp(float R, float C);
float filter_compute_R(filter_response_t ft, uint8_t order, float fc, float C);
void filter_set_R_lp(filter_t* filter,  float R, uint8_t ri);
void filter_set_C_lp(filter_t* filter, float C, uint8_t ci);
void filter_set_coeffs_lp(filter_t* filter, float delta_t, float R, float C);
uint16_t filter_apply_all(filter_f** filter_functions, filter_t* filters, uint16_t inp, uint8_t nf);

void filter_set_gain(filter_t filter, float gain);
#endif /* INC_FILTER_H_ */
