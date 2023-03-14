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
uint32_t apply_filters(uint32_t sample);

void output_handler_init() {
	for (uint8_t i = 0; i < MODULATOR_MAX_VOICES; i++) {
		squares[i] = sqrt(i + 1);
	}
}

void output_handler_outwave_update(uint16_t* out, uint16_t out_start, uint16_t out_stop, uint16_t* wavetable) {
	/*
	 * Combine modulation signals to single output
	 * Note that we are skipping 4 entries in buffer for performance, which makes the frequency of wavetable note
	 * is divided by 4. Since N_WT_SAMPLES = 5384 and sampling f = 44Khz then base tone is 16.352/4 = 4.088Hz
	 */
#define REF_CENTER 2048
	static uint32_t out_val;

	modulator_update_df();
	modulator_update_fm();

	modulator_update_beta();
	modulator_pmf_update();

	modulator_compute_k(1);
	modulator_compute_k(2);

	uint16_t* mod_wt1 = wavetable_get_ptr(mixer_get_OSC_ws(1));
	uint16_t* mod_wt2 = wavetable_get_ptr(mixer_get_OSC_ws(2));
	uint16_t* mod_wt3 = wavetable_get_ptr(mixer_get_OSC_ws(3));

	float mf1;
	float mf2;

	float mp1;
	float mp2;

	int idx;

	for (uint16_t i = out_start; i < out_stop - 3; i+=4) {
		uint8_t active_voices = 0;
		uint32_t out_sample = 0;

		modulator_update_fm_osc(1);
		modulator_update_fm_osc(2);

		for (uint8_t j = 0; j < MODULATOR_MAX_VOICES; ++j) {
			if (!steps[j]) {
				trackers[j] = 0;
				continue;
			}
			mf1 = modulator_get_next_fm(modulator_get_fm_osc(1), mod_wt1, modulator_get_k(1));

			mf2 = modulator_get_next_fm(modulator_get_fm_osc(2), mod_wt2, modulator_get_k(2));

			mp1 = modulator_get_next_pm(steps[j], mod_wt2, j, 1);
			mp2 = modulator_get_next_pm(steps[j], mod_wt3, j, 2);

			idx = ((int)(trackers[j] + mp1 + mp2 + mf1 + mf2)) % N_WT_SAMPLES;

			if (idx < 0) {
				idx += N_WT_SAMPLES;
			}

			if (trackers[j] > N_WT_SAMPLES*((float)mixer_get_PWM()/MIXER_DREF)) {
				env_process(j);
				float scaler = env_map_get(j)->scaler;

				uint16_t next_sample = (float)wavetable[idx];
				if (next_sample > 0xFFF) {
					next_sample -= 0xFFF;
				}
				out_sample += next_sample * scaler;
			}

			trackers[j] += steps[j];

			if(trackers[j] >= N_WT_SAMPLES) {
				trackers[j] = trackers[j] - N_WT_SAMPLES;
			}

			active_voices++;
		}

		if (active_voices) {
			//out_sample = apply_effects(out_sample);
			// out_sample = apply_filters(out_sample);

			out_val = (out_sample)/squares[active_voices - 1];
			out_val = apply_filters(out_val);

		}
		float vol = (float)mixer_get_volume()/MIXER_DREF;

		out[i] = out_val * vol;
		out[i+1] = out[i];
		out[i+2] = out[i];
		out[i+3] = out[i];
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

uint32_t apply_filters(uint32_t sample) {
	//sample = filter_res_update(sample);
	sample = filter_lp_RC_get_next(sample);

	return sample;
}


float* output_handler_get_steps() {
	return steps;
}
