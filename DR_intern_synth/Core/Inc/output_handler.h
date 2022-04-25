/*
 * output_handler.h
 *
 *  Created on: Apr 22, 2022
 *      Author: sfs
 */

#ifndef INC_OUTPUT_HANDLER_H_
#define INC_OUTPUT_HANDLER_H_

/*
 * Parsing and processing input from MIDI device
 */

#include "filter.h"
#include "modulator.h"
#include "wavetable.h"

#endif /* INC_OUTPUT_HANDLER_H_ */

void output_handler_outwave_update(
		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable,
		float* input_index_trackers, float* input_index_steps, uint16_t keys_pressed);
