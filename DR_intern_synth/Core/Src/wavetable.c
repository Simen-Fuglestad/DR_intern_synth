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
		waveshape_enum waveshape, uint16_t* out, uint16_t ref_v,
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
        float scaled = sine * ((ref_v)/2);

        uint16_t s = (uint16_t)(scaled + ref_v/2) * amp;

		out[i] = s;
	}
}

//void wavetable_create_sine(uint16_t* out, uint16_t ref_v, uint16_t ns, float amp) {
//
//	for (int i = 0; i < ns; i++) {
////		float sine = math_utils_sine_lsf_approx((i * (2*M_PI)/ns));
//        float sine = sin((i * (2*M_PI)/ns));
//        float scaled = sine * ((ref_v)/2);
//
//        uint16_t s = (uint16_t)(scaled + ref_v/2) * amp;
//
//		out[i] = s;
//	}
//}

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

		float scaled = tri * ((ref_v + 1)/2);

		uint16_t s = (uint16_t)(scaled + ref_v/2) * amp;
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

void wavetable_period_copy(uint16_t* out, uint16_t* in, uint16_t out_len, uint16_t in_len) {
	uint16_t in_index = 0;
	for (uint16_t i = 0; i < out_len; i ++) {
		out = in[in_index];
		in_index++;
		if (in_index > in_len) {
			in_index = 0;
		}
	}
}

//uint16_t compute_sine(uint16_t ns, uint16_t ref_v) {
//    float sine = sin((i * (2*M_PI)/ns));
//    float scaled = sine * ((ref_v)/2);
//
//    uint16_t s = (uint16_t)(scaled + ref_v/2);
//    return s;
//}

float wavetable_outwave_update(
		uint16_t* out, uint16_t out_start, uint16_t out_len, uint16_t* wavetable, float in_start, float index_step) {
	float next_index = in_start;
	for (uint16_t i = out_start; i < out_len; i ++) {
		out[i] = wavetable[(uint16_t)next_index];
		next_index+=index_step;
		if (next_index > MAX_SAMPLE_SIZE) {
			next_index = next_index - MAX_SAMPLE_SIZE;
		}
	}
	return next_index;
}


//void wavetable_create_sine(uint16_t* out, uint16_t ref_v, uint16_t ns_pp, uint16_t ns, float amp) {
//	for (int i = 0; i < ns; i++) {
////		float sine = math_utils_sine_lsf_approx((i * (2*M_PI)/ns));
//        float sine = sin((i * (2*M_PI)/ns_pp));
//        float scaled = sine * ((ref_v)/2);
//
//        uint16_t s = (uint16_t)(scaled + ref_v/2) * amp;
//
//		out[i] = s;
//	}
//}

void wavetable_set_f(uint16_t* out, float index_step, uint16_t len) {
//	uint16_t next;
	uint16_t tmp_out[len];
	memcpy(tmp_out, out, len * sizeof(uint16_t));
	float index_next = 0;
	for (uint16_t i = 0; i < len; i++) {
		out[i] = tmp_out[(uint16_t)index_next];
		index_next += index_step;
		if (index_next > len) {
			index_next = index_next - len;
		}
	}
}
