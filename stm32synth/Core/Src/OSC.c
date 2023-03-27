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

static const float VIB_LIM_HIGH 	= 1.122 - 0.122; //offset to avoid clipping at 4095
static const float VIB_LIM_LOW 		= 0.890 - 0.122;
static float vib_tracker   			= (VIB_LIM_HIGH - VIB_LIM_LOW)/2;
static bool vib_rising				= true;

//static const

float OSC_apply(uint16_t speed, float sample_in, OSC_mode_enum mode) {
	float* wt = wavetable_get_ptr(mixer_get_OSC_ws(1));
	float osc_val = wt[(uint16_t)OSC_tracker];

	float out_val = 0;


	if(mode == LFO_TREMOLO) {
		out_val = sample_in * osc_val;

	} else if (mode == LFO_PITCH) {
		out_val = (out_val + osc_val)/2;
//		out_sample = in + lfo_val;

	} else if (mode == LFO_FLUTTER) {
		//fun
		if (vib_rising) {
			vib_tracker+=(osc_val);
			if (vib_tracker >= VIB_LIM_HIGH) {
				vib_tracker = VIB_LIM_HIGH;
				vib_rising = false;
			}
		} else {
			vib_tracker-=(osc_val);
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
	OSC_tracker+= OSC_LIM_STEP_SZ * (float)speed/OSC_ref;
	if (OSC_tracker > N_WT_SAMPLES) {
		OSC_tracker -= N_WT_SAMPLES;
	}

//	if (out_val <= 0) out_val = 1;
	return out_val;
}

static float res_tracker;

static float res_step_sz;

uint16_t OSC_res_update() {
	res_step_sz = mixer_get_pmf()/4.095;
	uint16_t depth = mixer_get_filter_fc_high()/2 - 1;
	uint16_t center = mixer_get_filter_fc_low()/2 - 1;

	int start = center - depth;
	if (start < 0) start = 0;
	int stop = center + depth;
	if (stop > 0xFFF) stop = 0xFFF;

	uint16_t* wt = wavetable_get_ptr(mixer_get_OSC_ws(1));

	uint16_t osc_val = wt[(uint16_t)res_tracker];
	float osc_scaled = (float)osc_val/OSC_ref;

	float f_start = (float)start/0xFFF;
	float f_stop = (float)stop/0xFFF;

	uint16_t out_val = OSC_ref * (f_start + osc_scaled*f_stop);

	res_tracker+= res_step_sz;
	if (res_tracker > N_WT_SAMPLES) {
		res_tracker -= N_WT_SAMPLES;
	}

	return out_val;
}
