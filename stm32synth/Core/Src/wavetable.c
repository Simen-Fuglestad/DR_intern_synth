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

static float sine[N_WT_SAMPLES];
static float square[N_WT_SAMPLES];
static float triangle[N_WT_SAMPLES];
static float sawtooth[N_WT_SAMPLES];

void wavetable_init_all() {
	wavetable_create_sine(sine, N_WT_SAMPLES, 1);
	wavetable_create_square(square, N_WT_SAMPLES, 1);
	wavetable_create_triangle(triangle, N_WT_SAMPLES, 1);
	wavetable_create_sawtooth(sawtooth, N_WT_SAMPLES, 1);
}

float* wavetable_get_ptr(ws_enum wave) {
	switch(wave) {
	case SINE:
		return sine;
	case SQUARE:
		return square;
	case TRIANGLE:
		return triangle;
	case SAWTOOTH:
		return sawtooth;
	default:
		break;
	}
}

void wavetable_create_sine(float* out, uint16_t ns, float amp) {

	for (int i = 0; i < ns; i++) {
	    float sine = sin((i * (2.0f*M_PI)/(float)ns));
        //float scaled = sine * ((ref_v)/2); //divide by 2 to dampen SPL

        //uint16_t s = (uint16_t)(scaled + ref_v/2) * amp;

		out[i] = sine;
	}
}

void wavetable_create_square(float* out, uint16_t ns, float amp) {
	for (int i = 0; i < ns/2; i++) {
		out[i] = 1 * amp;
	}

	for (int i = ns/2; i < ns; i++) {
		out[i] = -1;
	}
}

void wavetable_create_triangle(float* out, uint16_t ns, float amp) {
	for (int i = 0; i < ns; i++) {
		float tri = 2.0f * fabs((float)i/ns - floor((float)i/ns + 0.5));
		//float scaled = tri * (ref_v);

		//uint16_t s = (uint16_t)(scaled) * amp;
		out[i] = 2*tri - 1;
	}
}

void wavetable_create_sawtooth(float* out, uint16_t ns, float amp) {
	for (int i = 0; i < ns; i++) {
		float saw = 2 * ((float)i/ns - floor(0.5 + (float)i/ns));
		//float scaled = saw * ((ref_v + 1)/2);
		//uint16_t s = (uint16_t)(scaled + ref_v/2) * amp;
		out[i] = saw;
	}
}

void wavetable_create_bowsaw(float* out, uint16_t ns, float amp, uint16_t split_i) {
	float tmp_sin[ns - split_i];
	wavetable_create_sine(tmp_sin, ns - split_i, amp);

	float tmp_square[ns];
	wavetable_create_square(tmp_square, ns, amp);

	float tmp_tri[ns];
	wavetable_create_triangle(tmp_tri, ns, amp);

	float tmp_saw[split_i];
	wavetable_create_sawtooth(tmp_saw, split_i, amp);

	float tmp_saw2[split_i/2];
	wavetable_create_sawtooth(tmp_saw2, split_i/2, amp);

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
