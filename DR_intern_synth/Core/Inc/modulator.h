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
#include "note_frequency.h"
#include <string.h>

typedef enum {
	FM, AM, LFO
} modulator_enum;

typedef struct {
	uint16_t* out_wave;
	float current_index;
	float index_step;
	float gain;
} am_modulator_t;

typedef struct {
	float fc;
	float fm;
	float f_delta;
	uint16_t ac;
	uint16_t am;
	uint16_t nsm;
	float kf; //sensitivity factor
} fm_modulator_t;

uint16_t am_modulate(am_modulator_t* mod, uint16_t in, float ns);
void am_mod_init(
		am_modulator_t* mod, uint16_t* out_wave, semitone_t st, uint8_t oct);
void am_mod_set_tone(am_modulator_t* mod, semitone_t st, uint8_t octave);
#endif /* INC_MODULATOR_H_ */
