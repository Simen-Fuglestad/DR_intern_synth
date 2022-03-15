/*
 * modulator.h
 *
 *  Created on: Mar 14, 2022
 *      Author: sfs
 */

#ifndef INC_MODULATOR_H_
#define INC_MODULATOR_H_

#include <stdint.h>
#include <math.h>
#include "wavetable.h"

typedef enum {
	FM, AM, LFO
} modulator_enum;

typedef struct {
	modulator_enum mod_enum;
	uint16_t* wavetable;
	waveshape_enum waveshape;
	uint16_t nsm;
	uint16_t current_index;
} modulator_t;

void am_modulator_sine(uint16_t* out, float f, float gain, uint16_t ns);
void am_modulator_init(
		modulator_t* mod, waveshape_enum mod_enum, uint16_t ref_v, float f, float gain);
uint16_t am_modulator_update(modulator_t* mod, uint16_t in);
#endif /* INC_MODULATOR_H_ */
