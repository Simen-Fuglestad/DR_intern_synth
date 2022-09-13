/*
 * wavetable.c
 *
 *  Created on: Feb 17, 2022
 *      Author: sfs
 */


#include "wavetable.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

static uint16_t sine[N_WT_SAMPLES];
static uint16_t square[N_WT_SAMPLES];
static uint16_t triangle[N_WT_SAMPLES];
static uint16_t sawtooth[N_WT_SAMPLES];
static uint16_t bowsaw[N_WT_SAMPLES];

void wavetable_init_all() {
	wavetable_create_sine(sine, 0xFFF, N_WT_SAMPLES, 1);
	wavetable_create_square(square, 0xFFF, N_WT_SAMPLES, 1);
	wavetable_create_triangle(triangle, 0xFFF, N_WT_SAMPLES, 1);
	wavetable_create_sawtooth(sawtooth, 0xFFF, N_WT_SAMPLES, 1);
	wavetable_create_bowsaw(bowsaw, 0xFFF, N_WT_SAMPLES, 1, 1000);
}

uint16_t* wavetable_get_ptr(ws_enum wave) {
	switch(wave) {
	case SINE:
		return sine;
	case SQUARE:
		return square;
	case TRIANGLE:
		return triangle;
	case SAWTOOTH:
		return sawtooth;
	case BOWSAW:
		return bowsaw;
	default:
		break;
	}
}

void wavetable_create_sine(uint16_t* out, uint16_t ref_v, uint16_t ns, float amp) {

	for (int i = 0; i < ns; i++) {
        float sine = sin((i * (2*M_PI)/ns));
        float scaled = sine * ((ref_v)/2); //divide by 2 to dampen SPL

        uint16_t s = (uint16_t)(scaled + ref_v/2) * amp;

		out[i] = s;
	}
}

void wavetable_create_square(uint16_t* out, uint16_t ref_v, uint16_t ns, float amp) {
	uint16_t high_val = ref_v * amp;
	for (int i = 0; i < ns/2; i++) {
		out[i] = high_val;
	}

	for (int i = ns/2; i < ns; i++) {
		out[i] = 0;
	}
}

void wavetable_create_triangle(uint16_t* out, uint16_t ref_v, uint16_t ns, float amp) {
	for (int i = 0; i < ns; i++) {
		float tri = 2.0f * fabs((float)i/ns - floor((float)i/ns + 0.5));
		float scaled = tri * (ref_v);

		uint16_t s = (uint16_t)(scaled) * amp;
		out[i] = s;
	}
}

void wavetable_create_sawtooth(uint16_t* out, uint16_t ref_v, uint16_t ns, float amp) {
	for (int i = 0; i < ns; i++) {
		float saw = 2 * ((float)i/ns - floor(0.5 + (float)i/ns));
		float scaled = saw * ((ref_v + 1)/2);
		uint16_t s = (uint16_t)(scaled + ref_v/2) * amp;
		out[i] = s;
	}
}

void wavetable_create_bowsaw(uint16_t* out, uint16_t ref_v, uint16_t ns, float amp, uint16_t split_i) {
	uint16_t tmp_sin[ns - split_i];
	wavetable_create_sine(tmp_sin, ref_v, ns - split_i, amp);

	uint16_t tmp_square[ns];
	wavetable_create_square(tmp_square, ref_v, ns, amp);

	uint16_t tmp_tri[ns];
	wavetable_create_triangle(tmp_tri, ref_v, ns, amp);

	uint16_t tmp_saw[split_i];
	wavetable_create_sawtooth(tmp_saw, ref_v, split_i, amp);

	uint16_t tmp_saw2[split_i/2];
	wavetable_create_sawtooth(tmp_saw2, ref_v, split_i/2, amp);

	for (uint16_t i = 0; i < split_i; ++i) {
		out[i] = tmp_saw[i];
	}

//	for (uint16_t i = split_i/2; i < split_i; ++i) {
//		out[i] = tmp_saw2[i];
//	}

	for (uint16_t i = split_i; i < ns-split_i; ++i) {
		out[i] = tmp_sin[i];
	}
}
