/*
 * wavetable.h
 *
 *  Created on: Feb 17, 2022
 *      Author: sfs
 */

#ifndef INC_WAVETABLE_H_
#define INC_WAVETABLE_H_

#define N_WT_SAMPLES 2692

#include <stdint.h>

typedef enum {
	SINE, SQUARE, TRIANGLE, SAWTOOTH
} ws_enum;

void wavetable_init_all();
float* wavetable_get_ptr(ws_enum wave);

//Wave gen functions, create a single period T of ns samples
void wavetable_create_sine(
		float* out, uint16_t ns, float amp);

void wavetable_create_square(
		float* out, uint16_t ns, float amp);

void wavetable_create_triangle(
		float* out, uint16_t ns, float amp);

void wavetable_create_sawtooth(
		float* out, uint16_t ns, float amp);

void wavetable_create_bowsaw(
		float* out, uint16_t ns, float amp, uint16_t split_i);

#endif /* INC_WAVETABLE_H_ */
