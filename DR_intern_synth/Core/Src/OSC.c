/*
 * LFO.c
 *
 *  Created on: Jun 1, 2022
 *      Author: sfs
 */

#include <OSC.h>
#include "wavetable.h"
#include "mixer.h"

static const float OSC_LIM_STEP_SZ 	= 16; //2~C0, 4~C1, 8~C2 etc
static float OSC_tracker			= 0;
static const uint16_t OSC_ref 		= MIXER_DREF;

static uint32_t LFO_carr_sample		= 0;

static const float VIB_LIM_HIGH 	= 1.122 - 0.122; //offset to avoid clipping at 4095
static const float VIB_LIM_LOW 		= 0.890 - 0.122;
static float vib_tracker   			= (VIB_LIM_HIGH - VIB_LIM_LOW)/2;
static bool vib_rising				= true;

//static const

uint32_t OSC_apply(uint16_t mixer_in, uint32_t sample_in, uint16_t out_val_ind, OSC_mode_enum mode) {
	uint16_t* wt = wavetable_get_ptr(mixer_get_OSC_ws());
	uint16_t osc_val = wt[(uint16_t)OSC_tracker];
	float osc_scaled = (float)osc_val/OSC_ref;

	static uint32_t out_sample;
	uint16_t out_val = sample_in;


	if(mode == LFO_TREMOLO) {
		out_val = sample_in * osc_scaled;

	} else if (mode == LFO_PITCH) {
		out_val = (out_val + osc_val)/2;
//		out_sample = in + lfo_val;

	} else if (mode == LFO_FLUTTER) {
		//fun
		if (vib_rising) {
			vib_tracker+=(osc_scaled);
			if (vib_tracker >= VIB_LIM_HIGH) {
				vib_tracker = VIB_LIM_HIGH;
				vib_rising = false;
			}
		} else {
			vib_tracker-=(osc_scaled);
			if (vib_tracker <= VIB_LIM_LOW) {
				vib_tracker = VIB_LIM_LOW;
				vib_rising = true;
			}
		}
		out_val *= vib_tracker;
	}
	else {
		return out_val;
	}
	OSC_tracker+= OSC_LIM_STEP_SZ * (float)mixer_in/OSC_ref;
	if (OSC_tracker > N_WT_SAMPLES) {
		OSC_tracker -= N_WT_SAMPLES;
	}

	return out_val;
}
