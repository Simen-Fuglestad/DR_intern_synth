/*
 * output_handler.c
 *
 *  Created on: Apr 22, 2022
 *      Author: sfs
 */

#include <math.h>
#include "usb_host.h"
#include "MIDI_application.h"
#include "wavetable.h"
#include "filter.h"
#include "output_handler.h"
#include "mixer.h"
#include "envelope.h"
#include "modulator.h"

static float squares[MODULATOR_MAX_VOICES];

static float trackers[MODULATOR_MAX_VOICES];
static float steps[MODULATOR_MAX_VOICES];

static uint16_t phase_shift_buffer[N_WT_SAMPLES];

#define PHASE_SHIFT_MAX 12 //getting the phase shifter to sound nice require careful tuning on the mixer
static float phase_shift_idx;
static const float DELAY_LIMIT = (float)PHASE_SHIFT_MAX/0xFFF;

float apply_filters(float sample);

void output_handler_init() {
	squares[0] = sqrt(2);
	for (uint8_t i = 1; i < MODULATOR_MAX_VOICES; i++) {
		squares[i] = squares[i-1] * sqrt(2);
	}
}

void output_handler_outwave_update(uint16_t* out, uint16_t out_start, uint16_t out_stop, float* wavetable) {
	/*
	 * Combine modulation signals to single output
	 * Note that we are skipping 2 entries in buffer for performance, so the frequency of "slowest" tone
	 * is divided by 2. Since N_WT_SAMPLES = 2692 and sampling at fs = 22Khz in stereo, then base tone is 16.352/2 = 8.176Hz
	 */

	static float out_val;

	static int bit_crush_cnt;

	modulator_update_df();
	modulator_update_fm();

	modulator_update_beta();
	modulator_pmf_update();

	float* mod_wt_fm = wavetable_get_ptr(mixer_get_OSC_ws(1));
	float* mod_wt_pm1 = wavetable_get_ptr(mixer_get_OSC_ws(2));

	float mf = 0;

	float mp = 0;

	int idx = 0;

//	bit_crush_cnt += mixer_get_OSC2_FM();
//
//	static int bit_crush;
//	if (bit_crush_cnt >= N_WT_SAMPLES) {
//		bit_crush+=2;
//		if (bit_crush>= 16) {
//			bit_crush = 0;
//		}
//		bit_crush_cnt = 0;
//	}

	for (uint16_t i = out_start; i < out_stop; i+=2) {

		uint8_t active_voices = 0;
		float out_sample = 0;

		modulator_update_fm_osc();

		for (uint8_t j = 0; j < MODULATOR_MAX_VOICES; ++j) {
			if (!steps[j]) {
//				trackers[j] = 0;
//				trackers2[j] = N_WT_SAMPLES >> 1;
				continue;
			}

			mf = modulator_get_next(modulator_get_fm_osc(1), mod_wt_fm) * modulator_get_df();

			mp = modulator_get_next_pm(steps[j], mod_wt_pm1, j) * env_map_get(j)->scaler;;

			int mod_step = (mp + mf) * N_WT_SAMPLES;

			idx = ((int)(trackers[j] + mod_step)) % N_WT_SAMPLES;

			if (idx < 0) {
				idx += N_WT_SAMPLES;
			}

			float pwm = N_WT_SAMPLES*((float)mixer_get_PWM()/MIXER_DREF);

			if (trackers[j] > pwm) {
				env_process_by_index(j);
				float scaler = env_map_get(j)->scaler;

				out_sample += wavetable[idx] * scaler;
			}

			trackers[j] += steps[j];

			if(trackers[j] >= N_WT_SAMPLES) {
				trackers[j] = trackers[j] - N_WT_SAMPLES;
			}

			active_voices++;
		}

		if (active_voices) {
			//NOTE: phase shifting as a feature is incomplete
			uint16_t phase = phase_shift_buffer[(int)phase_shift_idx];
			float phase_mix = (float)mixer_get_phase_shift() * DELAY_LIMIT;
			phase_shift_idx += phase_mix;
			if (phase_shift_idx >= N_WT_SAMPLES) {
				phase_shift_idx -= N_WT_SAMPLES;
			}
			out_val = out_sample / squares[active_voices - 1];
			out_val = apply_filters(out_val);
			out_val = out_val * 2047 + 2048;

			if (phase_mix > 0.1) {
				out_val = (out_val + phase) / squares[0];
			}
		}

		float vol = (float)mixer_get_volume()/MIXER_DREF;

		out[i] = out_val * vol;
		out[i+1] = out[i];

		phase_shift_buffer[i] = out[i];
		phase_shift_buffer[i+1] = out[i];
	}
}

float apply_filters(float sample) {
	//add other filters here as desired
	sample = filter_res_update(sample);
	return sample;
}


float* output_handler_get_steps() {
	return steps;
}
