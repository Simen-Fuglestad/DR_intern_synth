/*
 * output_handler.c
 *
 *  Created on: Apr 22, 2022
 *      Author: sfs
 */

#include <stdint.h>
#include "output_handler.h"
#include "mixer.h"

static uint8_t n_voices;

static float trackers[MAX_VOICES];
static float steps[MAX_VOICES];

static float prev_index;
static uint16_t prev_amp;

bool pwm_update(float, float, uint8_t);
bool pwm_update2(float f);

uint16_t output_handler_apply_filters(uint16_t in) {
	in = filter_lp_RC_get_next(in);
	//	in = filter_hp_RC_get_next(in);
	return in;
}

void output_handler_init(uint8_t MIDI_in_voices) {
	if (MIDI_in_voices <= MAX_VOICES) {
		n_voices = MIDI_in_voices;
		prev_index = 0;
		prev_amp = 0;
	}
}

//uint16_t output_handler_outwave_update(
//		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable,
//		float* trackers, float* steps, uint16_t n_voices, uint16_t prev_out
//) {
////
//	uint16_t out_val = prev_out;
//	uint8_t div_count = 0;
//	uint8_t tracker_sync = 0; //make sure next keypress start on same index as previous
//
//	uint16_t out_tmp = prev_out;
//
//	for (uint16_t i = out_start; i < out_len; i ++) {
//		for (uint16_t j = 0; j < n_voices; j++) {
//
//			if (steps[j] == 0) {
//				trackers[j] = trackers[tracker_sync];
//				continue;
//			}
//			trackers[j] += steps[j];
//			if(trackers[j] >= N_WT_SAMPLES) {
//				trackers[j] = trackers[j] - N_WT_SAMPLES;
//			}
////			out_val = (out_val + wavetable[(uint16_t)trackers[j]])/2;
//			out_val += wavetable[(uint16_t)trackers[j]];
//			tracker_sync = j;
//			div_count++;
//
//		}
//		out[i] = out_val/div_count;
//		div_count = 0;
//	}
////	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);
//
//	return out_val;
//}


/*
 * Best AM (or RM?) so far
 */
void output_handler_outwave_AM_update(uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable) {

	static uint16_t out_val;

	static uint8_t tracker_sync; //make sure next keypress start on same index as previous

	for (uint16_t i = out_start; i < out_len; i ++) {
		for (uint16_t j = 0; j < n_voices; j++) {

			if (steps[j] == 0) {
				trackers[j] = trackers[tracker_sync];
				continue;
			}

			trackers[j] += steps[j];
			if(trackers[j] >= N_WT_SAMPLES) {
				trackers[j] = trackers[j] - N_WT_SAMPLES;
			}

			if (mixer_is_PWM_enabled()) {
				if (trackers[j] > N_WT_SAMPLES*((float)mixer_get_PWM()/4095.0f)) {
					out_val = (out_val + wavetable[(uint16_t)trackers[j]])/2;
				}
			} else {
				out_val = (out_val + wavetable[(uint16_t)trackers[j]])/2;
			}

			tracker_sync = j;
		}

		if (mixer_get_filter_enabled())
			out_val = output_handler_apply_filters(out_val);

		out[i] = out_val;

	}
	//	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);

}

//float output_handler_outwave_fupdate(
//		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable,
//		float* trackers, float* steps, uint16_t n_voices, float prev_index) {
//
//	uint8_t tracker_sync = 0; //make sure next keypress start on same index as previous
//	uint8_t period_tracker = 0;
//	float index_cnt = prev_index;
//
//	for (uint16_t i = out_start; i < out_len; i ++) {
//		for (uint16_t j = 0; j < n_voices; j++) {
//
//			if (steps[j] == 0) {
//				continue;
//			}
//
//
//			index_cnt = index_cnt + steps[j];
//			if (index_cnt > N_WT_SAMPLES) {
//				index_cnt = index_cnt - N_WT_SAMPLES;
//				period_tracker++;
//				if (period_tracker > n_voices)
//					period_tracker = 0;
//			}
//
//			if (period_tracker == 0) {
//				index_cnt += trackers[0];
//			} else if (period_tracker == 1) {
//				index_cnt += trackers[1];
//			}
//
//
//		}
//		out[i] = wavetable[(uint16_t)index_cnt];
//	}
////	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);
//
//	return index_cnt;
//}

void output_handler_outwave_FM_update(
		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable) {
	//
	static uint16_t out_val;

	uint8_t tracker_sync = 0; //make sure next keypress start on same index as previous

	for (uint16_t i = out_start; i < out_len; i ++) {
		for (uint16_t j = 0; j < n_voices; j++) {

			if (steps[j] == 0) {
				trackers[j] = trackers[tracker_sync];
				continue;
			}

			trackers[j] += steps[j];
			if(trackers[j] >= N_WT_SAMPLES) {
				trackers[j] = trackers[j] - N_WT_SAMPLES;
			}
			out_val = (out_val + wavetable[(uint16_t)trackers[j]])/2;
			prev_amp = out_val;
			tracker_sync = j;

		}
		out[i] = out_val;
	}
	//	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);
}



/*
 * Weirdness
 */
void output_handler_outwave_fupdate(
		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable) {

	uint8_t tracker_sync = 0; //make sure next keypress start on same index as previous

	uint8_t active_voice = 0;

	float index_cnt = prev_index;

	for (uint16_t i = out_start; i < out_len; i ++) {
		for (uint16_t j = 0; j < n_voices; j++) {

			if (steps[j] == 0) {
				trackers[j] = trackers[tracker_sync];
				continue;
			}

			trackers[j] += steps[j];
			if(trackers[j] >= N_WT_SAMPLES) {
				trackers[j] = trackers[j] - N_WT_SAMPLES;
				//				active_voice = j;
			}

			tracker_sync = j;

			//			Fun sci-fi
			//			index_cnt = index_cnt + steps[j];
			//			if (index_cnt > N_WT_SAMPLES) {
			//				index_cnt = index_cnt - N_WT_SAMPLES;
			//			}

		}
		index_cnt = index_cnt + steps[active_voice];
		if (index_cnt > N_WT_SAMPLES) {
			index_cnt = index_cnt - N_WT_SAMPLES;
		}
		//		out[i] = filter_lp_RC_get_next(wavetable[(uint16_t)index_cnt]);
		out[i] = filter_hp_RC_get_next(wavetable[(uint16_t)index_cnt]);
		//		out[i] = wavetable[(uint16_t)index_cnt];
	}
	//	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);
	prev_index = index_cnt;
}

/*
 * WARNING: LOUD
 * This produces a really wet sound at lower frequencies (not good, just wet) and terrible static at >2nd octave
 */
//float output_handler_outwave_fupdate(
//		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable,
//		float* trackers, float* steps, uint16_t n_voices, float prev_index) {
//
//	uint8_t tracker_sync = 0; //make sure next keypress start on same index as previous
//
//	float index = prev_index;
//
//	for (uint16_t i = out_start; i < out_len; i ++) {
//		for (uint16_t j = 0; j < n_voices; j++) {
//
//			if (steps[j] == 0) {
//				trackers[j] = trackers[tracker_sync];
//				continue;
//			}
//			trackers[j] += steps[j];
//			if(trackers[j] >= N_WT_SAMPLES) {
//				trackers[j] = trackers[j] - N_WT_SAMPLES;
//			}
//////			out_val = (out_val + wavetable[(uint16_t)trackers[j]])/2;
////			out_val = wavetable[(uint16_t)t]
////			prev_out = out_val;
////			tracker_sync = j;
//
//			index = index + trackers[j];
//			if (index > N_WT_SAMPLES) {
//				index = index - N_WT_SAMPLES;
//			}
//
//		}
//		out[i] = wavetable[(uint16_t)index];
//	}
////	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);
//
//	return index;
//
//}


bool pwm_update(float step_val, float pwm, uint8_t voice_n) {

	/*
	 * OBSOLETE, use exisiting trackers to decide PWM.
	 * Keeping this for future reference
	 */
	static float pwm_cnts[MAX_VOICES];

	pwm_cnts[voice_n] += step_val;

	if (pwm_cnts[voice_n] >= N_WT_SAMPLES) {
		pwm_cnts[voice_n] = 0;
	}
	else if (pwm_cnts[voice_n] > (float)N_WT_SAMPLES * pwm) {
		return false;
	}
	else {
		return true;
	}

	// early version, not really PWM in the music sense
//	static uint16_t pwm_val;
//	static bool pwm_out;
//	static float pwm_count;
//	pwm_val = mixer_get_PWM();
//
//	pwm_count += 10;
//	//		pwm_count += mixer_get_mod();
//	if (pwm_count > pwm_val) {
//		pwm_out = !pwm_out;
//		pwm_count = 0;
//	}
//	return pwm_out;
}


float* output_handler_get_steps() {
	return steps;
}
