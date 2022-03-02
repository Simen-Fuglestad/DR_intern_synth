/*
 * DSP_utils.h
 *
 *  Created on: Feb 28, 2022
 *      Author: sfs
 */

#ifndef INC_DSP_UTILS_H_
#define INC_DSP_UTILS_H_

#include <stdint.h>

typedef enum {
	LOWPASS, HIGHPASS, BANDPASS, BANDSTOP, NOTCH
} filter_enum;

void dsp_utils_idft_odd(float* x, float* y, uint16_t N);

//void dsp_utils_window_sampling(filter_enum, float* wc, uint16_t N);



void dsp_utils_window_sampling_lp(float fc, uint16_t N, float* out);
void dps_utils_window_sampling_hp(float fc, uint16_t N, uint16_t* out);
void dsp_utils_apply_filter(float* filter, uint16_t filter_len, uint16_t* out, uint16_t ref_v);
void dsp_utils_set_lp(float fc, uint16_t N, uint16_t* out_wave, uint16_t ref_v);
void dsp_utils_idft_odd(float* x, float* y, uint16_t N);
#endif /* INC_DSP_UTILS_H_ */

