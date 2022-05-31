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

void wavetable_create(
		wave_shape_enum waveshape, uint16_t* out, uint16_t ref_v,
		uint16_t ns, float amp) {
	switch (waveshape) {
	case SINE:
		wavetable_create_sine(out, ref_v, ns, amp);
		break;
	case SQUARE:
		wavetable_create_square(out, ref_v, ns, amp);
		break;
	case TRIANGLE:
		wavetable_create_triangle(out, ref_v, ns, amp);
		break;
	case SAWTOOTH:
		wavetable_create_sawtooth(out, ref_v, ns, amp);
		break;
	default:
		return;
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
