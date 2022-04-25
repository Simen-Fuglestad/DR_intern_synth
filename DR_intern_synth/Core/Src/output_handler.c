/*
 * output_handler.c
 *
 *  Created on: Apr 22, 2022
 *      Author: sfs
 */

#include <stdint.h>
#include "main.h"

#include "output_handler.h"

void output_handler_outwave_update(
		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable,
		float input_index_trackers[], float* input_index_steps, uint16_t keys_pressed
		) {
	float index;
	uint16_t out_val;

	for (uint16_t i = out_start; i < out_len; i ++) {
		out_val = 0;
		for (uint16_t j = 0; j < keys_pressed; j++) {
			if (input_index_steps[j] >= 1) {
				index = input_index_trackers[j];
				out_val = (out_val + wavetable[(uint16_t)index])/2;

				input_index_trackers[j] += input_index_steps[j];
				if(input_index_trackers[j]  >= MAX_SAMPLE_SIZE) {
					input_index_trackers[j] = input_index_trackers[j] - MAX_SAMPLE_SIZE;
				}
			}
		}
		out[i] = out_val;
	}
}
