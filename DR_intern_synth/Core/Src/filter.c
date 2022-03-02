/*
 * filter.c
 *
 *  Created on: Feb 21, 2022
 *      Author: sfs
 */

#include "filter.h"
#include <math.h>

void filter_1st_order_lowpass(uint16_t* out_wave, uint16_t out_len, float R, float C, float T) {
	float c1 = T/(T + R*C);
	float c2 = (R*C)/(T + R*C);
	out_wave[0] = c1 * out_wave[0];
	for (int i = 1; i < out_len; i++) {
		out_wave[i] = c1 * out_wave[i] + c2 * out_wave[i - 1];
	}
}
