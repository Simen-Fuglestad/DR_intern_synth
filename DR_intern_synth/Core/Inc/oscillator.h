/*
 * oscillator.h
 *
 *  Created on: Mar 16, 2022
 *      Author: sfs
 */

#ifndef INC_OSCILLATOR_H_
#define INC_OSCILLATOR_H_

#include <math.h>
#include <stdint.h>
#include "wavetable.h"

//typedef enum {
//	SINE, SQUARE, TRIANGLE, SAWTOOTH
//} osc_shape_enum;

typedef struct {
	wave_shape_enum osc_shape;
	uint16_t* wavetable_ptr;
	float inc_size;
	uint16_t ns;
	float current_index;
} oscillator_t;

void osc_init(oscillator_t* osc, uint16_t* wavetable, wave_shape_enum osc_shape, float f);
uint16_t osc_next_sine(oscillator_t* osc, uint16_t ref_v, float amp);
uint16_t osc_next_square(oscillator_t* osc, uint16_t ref_v, float amp);

#endif /* INC_OSCILLATOR_H_ */
