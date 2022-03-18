/*
 * modulator.h
 *
 *  Created on: Mar 14, 2022
 *      Author: sfs
 */

#ifndef INC_MODULATOR_H_
#define INC_MODULATOR_H_

#include <stdint.h>
#include <math.h>
#include "wavetable.h"

typedef enum {
	FM, AM, LFO
} modulator_enum;

typedef struct {
	uint16_t* wtable_ptr;
	waveshape_enum carrier_waveshape;
	uint16_t nsm;
	uint16_t current_index;
	uint8_t storage_n;
} am_modulator_t;

am_modulator_t am_mod1;
uint16_t am_mod1_wt[MAX_SAMPLE_SIZE];

am_modulator_t am_mod2;
uint16_t am_mod2_wt[MAX_SAMPLE_SIZE];

am_modulator_t am_mod3;
uint16_t am_mod3_wt[MAX_SAMPLE_SIZE];

am_modulator_t am_mod_storage[0x0F];
uint16_t* am_mod_wtable_storage[0x0F];

typedef struct {
	float fc;
	float fm;
	float f_delta;
	uint16_t ac;
	uint16_t am;
	uint16_t nsm;
	float kf; //sensitivity factor
} fm_modulator_t;

void am_mod_storage_init();

void am_mod_init(
		am_modulator_t* mod, uint16_t mod_n, waveshape_enum mod_enum, uint16_t ref_v, float f, float gain);
uint16_t am_mod_update(am_modulator_t* mod, uint16_t in);
void am_mod_set_f(am_modulator_t* mod, float f);
#endif /* INC_MODULATOR_H_ */
