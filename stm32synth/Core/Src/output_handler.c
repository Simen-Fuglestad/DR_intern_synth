/*
 * output_handler.c
 *
 *  Created on: Apr 22, 2022
 *      Author: sfs
 */

#include <math.h>
#include "wavetable.h"
#include "filter.h"
#include "OSC.h"
#include "output_handler.h"
#include "mixer.h"
#include "envelope.h"
#include "modulator.h"

static float squares[MODULATOR_MAX_VOICES];

static float trackers[MODULATOR_MAX_VOICES];
static float steps[MODULATOR_MAX_VOICES];

uint32_t apply_effects(uint32_t sample);
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

	modulator_update_df();
	modulator_update_fm();

	modulator_update_beta();
	modulator_pmf_update();

	float* mod_wt_fm = wavetable_get_ptr(mixer_get_OSC_ws(1));
	float* mod_wt_pm1 = wavetable_get_ptr(mixer_get_OSC_ws(2));
	float* mod_wt_pm2 = wavetable_get_ptr(mixer_get_OSC_ws(3));

	float mf1;
	float mf2;

	float mp1;
	float mp2;

	int idx;

	for (uint16_t i = out_start; i < out_stop; i+=2) {
		uint8_t active_voices = 0;
		float out_sample = 0;

		modulator_update_fm_osc(1);
		modulator_update_fm_osc(2);

		for (uint8_t j = 0; j < MODULATOR_MAX_VOICES; ++j) {
			if (!steps[j]) {
				trackers[j] = 0;
				continue;
			}

			mf1 = modulator_get_next(modulator_get_fm_osc(1), mod_wt_fm) * modulator_get_df();
			mf2 = modulator_get_next(modulator_get_fm_osc(2), mod_wt_fm) * modulator_get_df();

			mp1 = modulator_get_next_pm(steps[j], mod_wt_pm1, j, 1) * env_map_get(j)->scaler;;
			mp2 = modulator_get_next_pm(steps[j], mod_wt_pm2, j, 2) * env_map_get(j)->scaler;;

			idx = ((int)(trackers[j] + mp1 + mp2 + mf1 + mf2)) % N_WT_SAMPLES;

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
//			out_sample = apply_effects(out_sample);
//			out_sample = apply_filters(out_sample);

			out_val = (out_sample)/squares[active_voices - 1];
//			out_val = apply_filters(out_val);
//			out_val = out_sample/active_voices;
			out_val = out_val * 2047 + 2048;

			//out_val = apply_filters(out_val);

		}
		float vol = (float)mixer_get_volume()/MIXER_DREF;

		out[i] = out_val * vol;
		out[i+1] = out[i];
	}
}

uint32_t apply_effects(uint32_t sample_in) {
	if (mixer_get_OSC2_FM() > 0) {
		sample_in = OSC_apply(mixer_get_OSC2_FM(), sample_in, mixer_get_OSC1_mode());
	}

	if (mixer_get_pm_beta2() > 0) {
		sample_in = OSC_apply(mixer_get_pm_beta2(), sample_in, mixer_get_OSC2_mode());
	}

	return sample_in;
}

float apply_filters(float sample) {
	sample = filter_res_update(sample);
//	sample = filter_lp_RC_get_next(sample);

	return sample;
}


float* output_handler_get_steps() {
	return steps;
}
