/*
 * output_handler.c
 *
 *  Created on: Apr 22, 2022
 *      Author: sfs
 */

#include <stdint.h>
#include "output_handler.h"

static uint8_t n_voices;

static float trackers[MAX_VOICES];
static float steps[MAX_VOICES];

static float prev_index;
static uint16_t prev_amp;

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
//
	uint16_t out_val = prev_amp;

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
			tracker_sync = j;

		}
		out[i] = filter_lp_RC_get_next(out_val);
		out[i] = filter_hp_RC_get_next(out[i]);


//		out[i] = out_val;
	}
//	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);

	prev_amp = out_val;
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
	uint16_t out_val = prev_amp;

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

	prev_amp = out_val;
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

			trackers[j] += steps[active_voice];
			if(trackers[j] >= N_WT_SAMPLES) {
				trackers[j] = trackers[j] - N_WT_SAMPLES;
				active_voice = j;
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

float* output_handler_get_steps() {
	return steps;
}
