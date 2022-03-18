/*
 * modulator.c
 *
 *  Created on: Mar 14, 2022
 *      Author: sfs
 */

#include "modulator.h"
#include <string.h>

void am_mod_storage_init() {
	am_mod_storage[0] = am_mod1;
	am_mod_storage[1] = am_mod2;
	am_mod_storage[2] = am_mod3;

	am_mod_wtable_storage[0] = am_mod1_wt;
	am_mod_wtable_storage[1] = am_mod2_wt;
	am_mod_wtable_storage[2] = am_mod3_wt;
}

void am_mod_init(
		am_modulator_t* mod, uint16_t mod_n, waveshape_enum waveshape, uint16_t ref_v, float f, float gain) {
	uint16_t nsm = round((2*I2S_SAMPLE_RATE)/f);

	wavetable_create(waveshape, am_mod_wtable_storage[mod_n], ref_v, nsm, gain);

	mod->wtable_ptr = am_mod_wtable_storage[mod_n];

	mod->storage_n = mod_n;
	mod->carrier_waveshape = waveshape;
	mod->nsm = nsm;
	mod->current_index = 0;
}

uint16_t am_mod_update(am_modulator_t* mod, uint16_t in) {
	uint16_t out = in - mod->wtable_ptr[mod->current_index];
	mod->current_index++;
	if(mod->current_index >= mod->nsm) mod->current_index = 0;
	return out;
}

void am_mod_set_f(am_modulator_t* mod, float f) {
	uint16_t nsm = round((2*I2S_SAMPLE_RATE)/f);
	mod->nsm = nsm;
	memset(am_mod_wtable_storage[mod->storage_n], 0, MAX_SAMPLE_SIZE * sizeof(uint16_t));
	wavetable_create(mod->carrier_waveshape, am_mod_wtable_storage[mod->storage_n], 0xFFF, nsm, 1);
}
