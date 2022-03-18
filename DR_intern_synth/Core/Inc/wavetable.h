/*
 * wavetable.h
 *
 *  Created on: Feb 17, 2022
 *      Author: sfs
 */

#ifndef INC_WAVETABLE_H_
#define INC_WAVETABLE_H_

#define I2S_SAMPLE_RATE 48000
#define MAX_SAMPLE_SIZE (2*I2S_SAMPLE_RATE)/(uint16_t)16.35
#include <stdint.h>
#include "note_frequency.h"

typedef enum {
	SINE, SQUARE, TRIANGLE, SAWTOOTH
} waveshape_enum;

typedef struct {
	uint16_t* wave_data;
	note_t note;
} wavetable_tone_t;

wavetable_tone_t wavetable_tone_bank[N_OCTAVES * N_SEMITONES];

uint16_t wavetable_sine[MAX_SAMPLE_SIZE];
uint16_t wavetable_square[MAX_SAMPLE_SIZE];
uint16_t wavetable_tri[MAX_SAMPLE_SIZE];
uint16_t wavetable_saw[MAX_SAMPLE_SIZE];

void wavetable_init(uint16_t* wt_sine, uint16_t* wt_square, uint16_t* wt_tri, uint16_t* wt_saw, uint16_t ref_v);

//Wave gen functions, create a single period T of ns samples
void wavetable_create(
		waveshape_enum waveshape, uint16_t* out, uint16_t ref_v,
		uint16_t ns, float amp);

void wavetable_create_sine(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp);

void wavetable_create_square(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp);

void wavetable_create_triangle(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp);

void wavetable_create_sawtooth(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp);

#endif /* INC_WAVETABLE_H_ */
