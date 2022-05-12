/*
 * oscillator.c
 *
 *  Created on: Mar 16, 2022
 *      Author: sfs
 */
#include "oscillator.h"

void osc_init(oscillator_t* osc, uint16_t* wavetable, wave_shape_enum osc_shape, float f) {
	osc->osc_shape = osc_shape;
	osc->inc_size = f/16.35;
	osc->wavetable_ptr = wavetable;
	osc->ns = round((2*I2S_SAMPLE_RATE)/16.35);
	osc->current_index = 0;
}

uint16_t osc_get_next(oscillator_t* osc) {
	uint16_t y = osc->wavetable_ptr[(uint16_t)round(osc->current_index)];

	osc->current_index += osc->inc_size;
	if (osc->current_index >= osc->ns) osc->current_index = 0;
	return y;
}

uint16_t osc_next_sine(oscillator_t* osc, uint16_t ref_v, float amp) {
    float sine = sinf((osc->current_index * (2*M_PI)/osc->ns) + 1);
    uint16_t y = ref_v * (sine/2 + 0.5) * amp;
//    int i = osc->current_index;
//    int ns = osc->ns;
//	float sine = sin((i * (2*M_PI)/ns) + 1);
//    float scaled = sine * ((ref_v + 1)/2);

//    uint16_t y = (uint16_t)(scaled + ref_v/2) * amp;
    osc->current_index++;
    if (osc->current_index > osc->ns) osc->current_index = 0;

    return y;
}

uint16_t osc_next_square(oscillator_t* osc, uint16_t ref_v, float amp) {
	uint16_t y;
	if (osc->current_index > osc->ns/2) {
		y = ref_v * amp;
	} else {
		y = 0;
	}
	osc->current_index++;
	if (osc->current_index > osc->ns) osc->current_index = 0;
	return y;
}
