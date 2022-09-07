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

static const float squares[10] = {
		sqrt(1), sqrt(2), sqrt(3), sqrt(4), sqrt(5), sqrt(6), sqrt(7), sqrt(8), sqrt(9), sqrt(10)
};

static float trackers[MAX_VOICES];
static float steps[MAX_VOICES];

uint32_t apply_effects(uint32_t sample, uint16_t out_val_ind);
uint32_t apply_filters(uint32_t sample);

void output_handler_init(uint8_t MIDI_in_voices) {
	if (MIDI_in_voices <= MAX_VOICES) {
		poly_inputs = MIDI_in_voices;
	}
}

void output_handler_outwave_update(uint16_t* out, uint16_t out_start, uint16_t out_stop, uint16_t* wavetable) {
#define REF_CENTER 2048
	static uint32_t out_val;

	float c0_baseline = 16.352;
	float fs = 22000;


	float df = (float)mixer_get_df()/(c0_baseline * 1000);

	float fm = (float)mixer_get_OSC1_FM()/(20000);

	float fm2 = (float)mixer_get_OSC2_FM()/1000;

	float beta = (float)mixer_get_pm_beta()/4095; //0-1
	float beta2 = (float)mixer_get_pm_beta2()/409.5; //0-10

	float ipmf  = (float)mixer_get_pmf()/0xFFF;
//	pmf = roundf(pmf * 10)/10;

#define octave_ratio 0.0833
	float pmf_scaler = roundf(ipmf/octave_ratio);

	float pmf = powf(1.059, pmf_scaler) + octave_ratio;

	float k1;
	float k2;

	if (fm > 0.01) {
		k1 = (df * fs) / (fm * 2 * M_PI * REF_CENTER);
	}
	else {
		k1 = 0;
	}

	if (fm2 > 0.01) {
		k2 = (df * fs) / (fm2 * 2 * M_PI * REF_CENTER);
	}
	else {
		k2 = 0;
	}


	uint16_t* mod_wt1 = wavetable_get_ptr(mixer_get_OSC_ws(1));
	uint16_t* mod_wt2 = wavetable_get_ptr(mixer_get_OSC_ws(2));
	uint16_t* mod_wt3 = wavetable_get_ptr(mixer_get_OSC_ws(3));

	static float fm_modulator;
	static float fm_modulator2;

	float mf1;
	float mf2;

	static float pm_modulators[10];

	static float pm_modulators2[10];

	float mp;
	float mp2;

	int idx;

	for (uint16_t i = out_start; i < out_stop - 3; i+=4) {
		uint8_t active_voices = 0;
		uint32_t out_sample = 0;

		fm_modulator += fm;
		if (fm_modulator >= N_WT_SAMPLES) {
			fm_modulator -= N_WT_SAMPLES;
		}

		fm_modulator2 += fm2;
		if (fm_modulator2 >= N_WT_SAMPLES) {
			fm_modulator2 -= N_WT_SAMPLES;
		}

		for (uint8_t j = 0; j < poly_inputs; ++j) {
			if (!steps[j]) {
				trackers[j] = 0;
				continue;
			}

			mf1 = (mod_wt1[(uint16_t)(fm_modulator)] - REF_CENTER) * k1;
			mf2 = (mod_wt2[(uint16_t)fm_modulator2] - REF_CENTER) * k2;

//			float mult = mixer_get_PM_mult();
			bool sync = mixer_get_sync();

			if (sync) {
				pm_modulators[j] += steps[j] * pmf;
			} else {
				pm_modulators[j] += steps[j];
			}

			if (pm_modulators[j] >= N_WT_SAMPLES) {
				pm_modulators[j] -= N_WT_SAMPLES;
			}
			mp = (mod_wt2[(uint16_t)pm_modulators[j]] - REF_CENTER) * beta;



			if (sync) {
				pm_modulators2[j] += steps[j] * pmf;
			} else {
				pm_modulators2[j] += steps[j];
			}

			if (pm_modulators2[j] >= N_WT_SAMPLES) {
				pm_modulators2[j] -= N_WT_SAMPLES;
			}
			mp2 = (mod_wt3[(uint16_t)pm_modulators2[j]] - REF_CENTER) * beta2;

			idx = ((int)(trackers[j] + mp + mp2 + mf1 + mf2)) % N_WT_SAMPLES;

			if (idx < 0) {
				idx += N_WT_SAMPLES;
			}


			if (trackers[j] > N_WT_SAMPLES*((float)mixer_get_PWM()/MIXER_DREF)) {
				env_process(j);
				float scaler = env_map_get(j)->scaler;

				int tmp = idx;
				if (tmp >= N_WT_SAMPLES) {
					tmp -= N_WT_SAMPLES;
				}
				uint16_t next_sample = (float)wavetable[tmp];
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
//			out_sample = apply_effects(out_sample, i);
			out_sample = apply_filters(out_sample);

			out_val = (out_sample)/squares[active_voices - 1];


		}
		float vol = (float)mixer_get_volume()/MIXER_DREF;
//		out_val *= (vol*out_val + active_voices * sqrt(2)); //super loud but fun (TURN DOWN VOLUME!)

		out[i] = out_val * vol;
		out[i+1] = out[i];
		out[i+2] = out[i];
		out[i+3] = out[i];
	}
}

uint32_t apply_effects(uint32_t sample_in, uint16_t out_val_ind) {
	if (mixer_get_OSC2_FM() > 0) {
		sample_in = OSC_apply(mixer_get_OSC2_FM(), sample_in, out_val_ind, mixer_get_OSC1_mode());
	}

	if (mixer_get_pm_beta2() > 0) {
		sample_in = OSC_apply(mixer_get_pm_beta2(), sample_in, out_val_ind, mixer_get_OSC2_mode());
	}

	return sample_in;
}

uint32_t apply_filters(uint32_t sample_in) {
	sample_in = filter_lp_RC_get_next(sample_in);
//	sample_in = filter_hp_RC_get_next(sample_in);

	return sample_in;
}


float* output_handler_get_steps() {
	return steps;
}
