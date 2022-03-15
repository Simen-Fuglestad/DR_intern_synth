/*
 * modulator.c
 *
 *  Created on: Mar 14, 2022
 *      Author: sfs
 */

#include "modulator.h"


void am_modulator_init(
		modulator_t* mod, waveshape_enum waveshape, uint16_t ref_v, float f, float gain) {
	uint16_t nsm = round((2*I2S_SAMPLE_RATE)/f);
	static uint16_t wavetable[MAX_SAMPLE_SIZE];
	wavetable_create(waveshape, wavetable, ref_v, nsm, gain);

	mod->wavetable = wavetable;
	mod->mod_enum = AM;
	mod->waveshape = waveshape;
	mod->nsm = nsm;
	mod->current_index = 0;

}

uint16_t am_modulator_update(modulator_t* mod, uint16_t in) {
	uint16_t out = in - mod->wavetable[mod->current_index];
	mod->current_index++;
	if(mod->current_index >= mod->nsm) mod->current_index = 0;
	return out;
}
