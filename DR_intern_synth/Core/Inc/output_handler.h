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

#define MAX_VOICES	16

static uint8_t n_voices; //5

static float trackers[MAX_VOICES];
static float steps[MAX_VOICES];

static float prev_index;
static uint16_t prev_amp;

void output_handler_init(uint8_t n_voices);

void output_handler_outwave_AM_update(
		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable);

void output_handler_outwave_FM_update(
		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable);


void output_handler_outwave_fupdate(
		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable);

float* output_handler_get_steps();

#endif /* INC_OUTPUT_HANDLER_H_ */
