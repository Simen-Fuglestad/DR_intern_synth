/*
 * LFO.c
 *
 *  Created on: Jun 1, 2022
 *      Author: sfs
 */
#include "LFO.h"
#include "wavetable.h"
#include "mixer.h"

static const float LFO_LIM_STEP_SZ 	= 2; //2~C0, 4~C1, 8~C2 etc
static float LFO_tracker			= 0;
static const uint16_t LFO_ref 		= MIXER_DIGI_REF;

static const float VIB_LIM_HIGH 	= 1.122 - 0.122; //offset to avoid clipping at 4095
static const float VIB_LIM_LOW 		= 0.890 - 0.122;
static float vib_tracker   			= (VIB_LIM_HIGH - VIB_LIM_LOW)/2;
static bool vib_rising				= true;
//static const

uint16_t LFO_apply(uint16_t mixer_in, uint16_t out_val, uint16_t* wt, LFO_mode_enum mode) {

	uint16_t lfo_val = wt[(uint16_t)LFO_tracker];
	float lfo_scaled = (float)lfo_val/LFO_ref;

	if(mode == LFO_TREMOLO) {

		out_val = out_val * lfo_scaled;

	} else if (mode == LFO_PITCH) {
		out_val = (out_val + lfo_val)/2;

	} else if (mode == LFO_VIB) {
//		out_val = out_val *

		if (vib_rising) {
			vib_tracker+=(lfo_scaled/100);
			if (vib_tracker >= VIB_LIM_HIGH) {
				vib_rising = false;
			}
		} else {
			vib_tracker-=(lfo_scaled/100);
			if (vib_tracker <= VIB_LIM_LOW) {
				vib_rising = true;
			}
		}
		out_val *= vib_tracker;
	}
	else {
		return out_val;
	}
	LFO_tracker+= LFO_LIM_STEP_SZ * (float)mixer_in/LFO_ref;
	if (LFO_tracker > N_WT_SAMPLES) {
		LFO_tracker -= N_WT_SAMPLES;
	}

	return out_val;
}
