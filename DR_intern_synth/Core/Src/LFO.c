/*
 * LFO.c
 *
 *  Created on: Jun 1, 2022
 *      Author: sfs
 */
#include "LFO.h"
#include "wavetable.h"
#include "mixer.h"

static const float LFO_LIM_STEP_SZ = 4; //2~C0, 4~C1, 8~C2 etc
static float LFO_tracker = 0;
static const uint16_t LFO_ref = MIXER_DIGI_REF;

uint16_t LFO_apply(uint16_t mixer_in, uint16_t out_val, uint16_t* wt, LFO_mode_enum mode) {
	uint16_t lfo_val;
	float scaled;

	lfo_val = wt[(uint16_t)LFO_tracker];

	if(mode == LFO_VOLUME) {
		float lfo_scaled = (float)lfo_val/LFO_ref;
		out_val = out_val * lfo_scaled;

	} else if (mode == LFO_PITCH) {
		out_val = (out_val + wt[(uint16_t)LFO_tracker])/2;
	} else {
		return out_val;
	}

	scaled = (float)mixer_in/LFO_ref;
	LFO_tracker+= LFO_LIM_STEP_SZ * scaled;
	if (LFO_tracker > N_WT_SAMPLES) {
		LFO_tracker -= N_WT_SAMPLES;
	}

	return out_val;
}
