/*
 * output_handler.c
 *
 *  Created on: Apr 22, 2022
 *      Author: sfs
 */

#include <stdint.h>

#include "output_handler.h"

uint16_t sum_trackers(uint16_t* wavetable, float* trackers, uint16_t fs_len) {
	uint16_t result = 0;
	uint8_t div_count = 1;
	for (uint16_t i = 0; i < fs_len; i++) {
		uint16_t wave_point = wavetable[(uint16_t)trackers[i]];
		if (wave_point > 0) div_count++;
		result += wave_point;
	}

	return result/div_count;
}

void output_handler_outwave_update(
		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable,
		float* trackers, float* steps, uint16_t n_voices
) {
	uint16_t out_val;

	for (uint16_t i = out_start; i < out_len; i ++) {
		out_val = 0;

		for (uint16_t j = 0; j < n_voices; j++) {
			if (steps[j] == 0) {
//				out_val = wavetable[(uint16_t)trackers[j]];
				trackers[j] = 0;
				continue;
			}
			trackers[j] += steps[j];
			if(trackers[j] >= MAX_SAMPLE_SIZE) {
				trackers[j] = trackers[j] - MAX_SAMPLE_SIZE;
			}

			out_val = (out_val + wavetable[(uint16_t)trackers[j]])/2;

//			out_val = sum_trackers(wavetable, trackers, n_voices);

//			if (steps[j] >= 1) {
//				out_val = (out_val + wavetable[(uint16_t)trackers[j]])/2;
//			}

		}
		out[i] = out_val;
	}
}
