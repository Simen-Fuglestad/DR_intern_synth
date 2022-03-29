/*
 * modulator.c
 *
 *  Created on: Mar 14, 2022
 *      Author: sfs
 */

#include "modulator.h"

void am_mod_init(
		am_modulator_t* mod, uint16_t* out_wave, semitone_t st, uint8_t oct) {
	mod->out_wave = out_wave;
	float step = pow(OCTAVE_STEP, 12 *oct + st);
	mod->index_step = step;
	mod->current_index = 0.0f;
}

uint16_t am_modulate(am_modulator_t* mod, uint16_t in, float ns) {
	uint16_t result = (in + mod->out_wave[(uint16_t)floor(mod->current_index)])/2;
	float index = mod->current_index + mod->index_step;
	if (index >= ns) {
		mod->current_index = index - ns;
	}
	else {
		mod->current_index = mod->current_index + mod->index_step;
	}

	return result;
}

void am_mod_set_tone(am_modulator_t* mod, semitone_t st, uint8_t oct) {
	mod->index_step = pow(OCTAVE_STEP, 12*oct + st);
}
