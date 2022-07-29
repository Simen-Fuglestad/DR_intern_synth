/*
 * output_handler.c
 *
 *  Created on: Apr 22, 2022
 *      Author: sfs
 */

#include <stdint.h>
#include <math.h>
#include <OSC.h>
#include "output_handler.h"
#include "mixer.h"
#include "envelope.h"

static uint8_t poly_inputs;

#define VOL_RMS_SCALER 4095.0f/sqrt(2)

static float trackers[MAX_VOICES];
static float steps[MAX_VOICES];

uint32_t apply_effects(uint32_t sample, uint16_t out_val_ind, uint16_t* wavetable);
uint32_t apply_filters(uint32_t sample);

void output_handler_init(uint8_t MIDI_in_voices) {
	if (MIDI_in_voices <= MAX_VOICES) {
		poly_inputs = MIDI_in_voices;
	}
}

void output_handler_outwave_update(uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable) {

	static uint16_t out_val;

//	static uint8_t tracker_sync;

	float c0_baseline = 16.352;
	float fs = 44000;

//	float fm = c0_baseline/2;
	float fm = (float)mixer_get_fm()/1000;
//	float fm = 0.216;
	float df = (float)mixer_get_df()/1000;
//	float df = 0.18;
//	float fm = (float)mixer_get_fm()/(c0_baseline * 100);
//	float df = (float)mixer_get_df()/(c0_baseline * 10);

	static float fm_steps[10];

	for (size_t i = 0; i < poly_inputs; ++i) {
		fm_steps[i] += steps[i] * fm;
//		OCTAVE_STEP_DOWN
		if (fm_steps[i] >= N_WT_SAMPLES) {
			fm_steps[i] -= N_WT_SAMPLES;
		}
	}

	float k;
	if (fm > 0.01)
		k = (df * fs) / (fm * 2 * M_PI * 2048);
	else
		k = 0;

	static float modulator;
	static float modulator2;

	float m;
	float m2;
	int idx;

	for (uint16_t i = out_start; i < out_len - 1; i+=2) {

		uint8_t active_voices = 0;
		uint32_t out_sample = 0;

		modulator += fm;
		if (modulator >= N_WT_SAMPLES) {
			modulator -= N_WT_SAMPLES;
		}

		m = (wavetable[(uint16_t)modulator] - 2048) * k;


		for (uint8_t j = 0; j < poly_inputs; ++j) {
			if (!steps[j]) {
//				trackers[j] = trackers[tracker_sync];
				trackers[j] = 0;
				continue;
			}

			idx = (int)(trackers[j] + m) % N_WT_SAMPLES;
			if (idx < 0) {
				idx += N_WT_SAMPLES;
			}


			if (trackers[j] > N_WT_SAMPLES*((float)mixer_get_PWM()/MIXER_DREF)) {
				float scaler = env_map_get(j)->scaler;
//				float os = (float)wavetable[(uint16_t)idx];
				out_sample += ((float)wavetable[(uint16_t)idx]) * scaler;
				env_process(j);
//				out_sample += (wavetable[(uint16_t)idx]);
			}

			trackers[j] += steps[j];

			if(trackers[j] >= N_WT_SAMPLES) {
				trackers[j] = trackers[j] - N_WT_SAMPLES;
			}

			active_voices++;


		}
		if (active_voices) {
			out_sample = apply_effects(out_sample, i, wavetable);
			out_sample = apply_filters(out_sample);
			out_val = (out_sample)/active_voices;
		}
		float vol = ((float)mixer_get_volume()/MIXER_DREF);
//		out[i] = out_val * ((float)mixer_get_volume()/MIXER_DREF);
//		out_val = out_val + (active_voices * VOL_RMS_SCALER);
		out[i] = out_val * vol;
		out[i+1] = out[i];
	}
}

uint32_t apply_effects(uint32_t sample_in, uint16_t out_val_ind, uint16_t* wavetable) {
	if (mixer_get_OSC1() > 0) {
		sample_in = OSC_apply(mixer_get_OSC1(), sample_in, out_val_ind, wavetable, mixer_get_OSC1_mode());
	}

	if (mixer_get_OSC2() > 0) {
		sample_in = OSC_apply(mixer_get_OSC2(), sample_in, out_val_ind, wavetable, mixer_get_OSC2_mode());
	}

	return sample_in;
}

uint32_t apply_filters(uint32_t sample_in) {
	sample_in = filter_lp_RC_get_next(sample_in);
	sample_in = filter_hp_RC_get_next(sample_in);

	return sample_in;
}


float* output_handler_get_steps() {
	return steps;
}
