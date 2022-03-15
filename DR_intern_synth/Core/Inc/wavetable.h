/*
 * wavetable.h
 *
 *  Created on: Feb 17, 2022
 *      Author: sfs
 */

#ifndef INC_WAVETABLE_H_
#define INC_WAVETABLE_H_

#define I2S_SAMPLE_RATE 48000
#define MAX_SAMPLE_SIZE (2*I2S_SAMPLE_RATE)/(uint16_t)16.35
#include <stdint.h>

typedef enum {
	SINE, SQUARE, TRIANGLE, SAWTOOTH
} waveshape_enum;

//Wave gen functions, create a single period T of ns samples
void wavetable_create(
		waveshape_enum waveshape, uint16_t* out, uint16_t ref_v,
		uint16_t ns, float amp);

void wavetable_create_sine(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp);

void wavetable_create_square(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp);

void wavetable_create_triangle(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp);

void wavetable_create_sawtooth(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp);

#endif /* INC_WAVETABLE_H_ */
