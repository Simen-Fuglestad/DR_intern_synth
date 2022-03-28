/*
 * trig_utils.c
 *
 *  Created on: Mar 18, 2022
 *      Author: sfs
 */
#include "math_utils.h"
#include <math.h>

uint32_t math_utils_exp(uint32_t x, uint8_t n) {
    uint32_t result = 1;
    for (uint8_t i = 0; i < n; i++) {
        result *= x;
    }
    return result;
}

float math_utils_sine_quad_approx(float x) {
	float c1 = (4.0f * x)/(M_PI * M_PI);
	float c2 = M_PI * c1;
	float c3 = c1 * x;
//	float r = (4 * x/(M_PI * M_PI)) * (M_PI - x);
	return c2 - c3;
}

float math_utils_sine_lsf_approx(float x) {
	if (x < M_PI) {
		return -(0.4176977570064684f * x * x) + 1.3122362048324556f * x - 0.05046549777845089f;

	} else {
		x = 2*M_PI - x;
		return -(-(0.4176977570064684f * x * x) + 1.3122362048324556f * x - 0.05046549777845089f);
	}
}

float math_utils_sine_bhaskara(float x) {
#define M_PI_SQUARED 5* M_PI * M_PI

	float nom = 16 * x * (M_PI - x);
	float denom = M_PI_SQUARED - 4 * x * (M_PI - x);
	return nom/denom;
}

float math_utils_linear_sin(float x) {

}

uint32_t math_utils_factorial(uint32_t x) {
    if (x == 0) return 1;
    return (x * math_utils_factorial(x - 1));
}
