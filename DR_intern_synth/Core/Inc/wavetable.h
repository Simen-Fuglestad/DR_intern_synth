/*
 * wavetable.h
 *
 *  Created on: Feb 17, 2022
 *      Author: sfs
 */

#ifndef INC_WAVETABLE_H_
#define INC_WAVETABLE_H_

/*
 * Collection of methods and helper constants to create wave tables of various shapes.
 */
#define REF_V_DIGITAL_HEADPHONE 0xFFF
#define REF_V_DIGITAL_SPEAKER 0xFFFF
#define I2S_SAMPLE_RATE 48000
#define MAX_SAMPLE_SIZE_F (I2S_SAMPLE_RATE)/16.352

//#define N_WT_SAMPLES 3493

//#define N_WT_SAMPLES 2935 	//48khz mono
//#define N_WT_SAMPLES 5872 	//48khz stereo
//#define MAX_SAMPLE_SIZE 2691 	//44khz mono
#define N_WT_SAMPLES 5384 	//44Khz stereo

#include <stdint.h>
#include "note_frequency.h"

typedef enum {
	SINE, SQUARE, TRIANGLE, SAWTOOTH
} ws_enum;

typedef enum {
	AM, FM, PM
} wave_out_mode_enum;

typedef struct {
	uint16_t* wave_data;
	note_t note;
} wavetable_tone_t;

void wavetable_init_all();
uint16_t* wavetable_get_ptr(ws_enum wave);

//Wave gen functions, create a single period T of ns samples
void wavetable_create(
		ws_enum waveshape, uint16_t* out, uint16_t ref_v,
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
