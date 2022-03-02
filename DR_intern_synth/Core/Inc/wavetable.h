/*
 * wavetable.h
 *
 *  Created on: Feb 17, 2022
 *      Author: sfs
 */

#ifndef INC_WAVETABLE_H_
#define INC_WAVETABLE_H_

#include <stdint.h>
#include "wave_util.h"

//Wave gen functions, create a single period T of ns samples
void wavetable_create(
		waveshape_t waveshape, uint16_t* out, uint16_t ref_v,
		uint16_t ns, float amp, uint16_t offset);
void wavetable_create_sine(uint16_t* out, uint16_t ref_v, uint16_t ns, float amp, uint16_t offset);
void wavetable_create_square(uint16_t* out, uint16_t ref_v, uint16_t ns, float amp, uint16_t offset);
void wavetable_create_triangle(uint16_t* out, uint16_t ref_v, uint16_t ns, float amp, uint16_t offset);
void wavetable_create_sawtooth(uint16_t* out, uint16_t ref_v, uint16_t ns, float amp, uint16_t offset);
void wavetable_copy(uint16_t* dest, uint32_t* src, uint16_t n);

void wavetable_i2s_create_sine(uint16_t* out_buff, uint16_t f);
//Helper functions


#endif /* INC_WAVETABLE_H_ */
