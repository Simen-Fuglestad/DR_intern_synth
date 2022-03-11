/*
 * wavetable.c
 *
 *  Created on: Feb 17, 2022
 *      Author: sfs
 */


#include "wavetable.h"
#include <math.h>
//#include <stdio.h>
#include <string.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

void wavetable_create(
		waveshape_t waveshape, uint16_t* out, uint16_t ref_v,
		uint16_t ns, float amp, uint16_t offset) {
	memset(out, 0, ns);

	switch (waveshape) {
	case SINE:
		wavetable_create_sine(out, ref_v, ns, amp, offset);
		break;
	case SQUARE:
		wavetable_create_square(out, ref_v, ns, amp, offset);
		break;
	case TRIANGLE:
		wavetable_create_triangle(out, ref_v, ns, amp, offset);
		break;
	case SAWTOOTH:
		wavetable_create_sawtooth(out, ref_v, ns, amp, offset);
		break;
	default:
		return;
	}
}

void wavetable_create_sine(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp, uint16_t offset) {

	for (int i = 0; i < ns; i++) {
        float sine = sin((i * (2*M_PI)/ns) + 1);
        float scaled = sine * ((ref_v + 1)/2);

        uint16_t s = (uint16_t)(scaled + ref_v/2);
		out[i] = s * amp;
	}
}

void wavetable_create_square(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp, uint16_t offset) {
	for (int i = 0; i < ns/2; i++) {
		out[i] = ref_v * amp;
	}

	for (int i = ns/2; i < ns; i++) {
		out[i] = 0;
	}
}

void wavetable_create_triangle(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp, uint16_t offset) {
	for (int i = 0; i < ns; i++) {
		float tri = ((float)2/M_PI) * asin(sin(((2 * M_PI)/ns) * i));

		float scaled = tri * ((ref_v + 1)/2);

		uint16_t s = (uint16_t)(scaled + ref_v/2);
		out[i] = s * amp;
	}
}

void wavetable_create_sawtooth(
		uint16_t* out, uint16_t ref_v, uint16_t ns, float amp, uint16_t offset) {
	for (int i = 0; i < ns; i++) {		float saw = 2 * ((float)i/ns - floor(0.5 + (float)i/ns));
		float scaled = saw * ((ref_v + 1)/2) * amp;
		uint16_t s = (uint16_t)(scaled + ref_v/2);
		out[i] = s * amp;
	}
}

uint16_t wavetable_compute_sine(uint16_t i, uint16_t ns, uint16_t ref_v) {
    float sine = sin((i * (2*M_PI)/ns) + 1);
    float scaled = sine * ((ref_v + 1)/2);

    uint16_t s = (uint16_t)(scaled + ref_v/2);
    return s;
}

void wavetable_copy(uint16_t* dest, uint32_t* src, uint16_t n) {
	memcpy(dest, src, n);
}
