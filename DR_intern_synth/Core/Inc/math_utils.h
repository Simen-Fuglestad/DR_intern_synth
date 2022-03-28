/*
 * trig_utils.h
 *
 *  Created on: Mar 18, 2022
 *      Author: sfs
 */

#ifndef INC_MATH_UTILS_H_
#define INC_MATH_UTILS_H_

#include <stdint.h>

#endif /* INC_MATH_UTILS_H_ */
float math_utils_faster_sin(float x);
uint32_t math_utils_factorial(uint32_t x);
uint32_t math_utils_exp(uint32_t x, uint8_t n);
float math_utils_sine_quad_approx(float x);
float math_utils_sine_lsf_approx(float x);
float math_utils_sine_bhaskara(float x);

#define SINE_C1 1.0f/(3*2)
#define SINE_C2 1.0f/(5 * 4 * 3 * 2)
#define SINE_C3 1.0f/(7 * 6 * 5 * 4 * 3 * 2)
#define SINE_C4 1.0f/(9 * 8 * 7 * 6 * 5 * 4 * 3 * 2)
#define SINE_C5 1.0f/(11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2)


