/*
 * filter.h
 *
 *  Created on: Feb 15, 2022
 *      Author: sfs
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

#include <stdint.h>

//defines

//type defs

typedef struct {

};

//Function prototypes
float dsp_utils_compute_fc_lowpass(float R, float C);
float dsp_utils_compute_R_lowpass(float fc, float C);
float dsp_utils_compute_C_lowpass(float fc, float R);

void filter_1st_order_lowpass(uint16_t* out_wave, uint16_t out_len, float R, float C, float T);

#endif /* INC_FILTER_H_ */
