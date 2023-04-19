/*
 * modulator.h
 *
 *  Created on: Mar 14, 2022
 *      Author: sfs
 */

#ifndef INC_MODULATOR_H_
#define INC_MODULATOR_H_

#include <stdint.h>

#define MODULATOR_MAX_VOICES 	4 		// Nr of "voices", aka simultaneous notes that can be played. More voices imply higher performance requirements
#define FREQ_BASE 				8.176 	// c-1
#define NOTE_F_UP				1.059 	// 2^(1/12), each successive note is 1.059 times faster than the previous
#define NOTE_F_DOWN				0.943

float modulator_get_next(float mod_step, float *mod_table);

void modulator_update_fm_osc();
void modulator_update_df();
void modulator_update_fm();
float modulator_compute_k(int kn);

float modulator_get_fm_osc();
float modulator_get_df();

void modulator_update_beta();

float modulator_get_next_pm(float mod, float *mod_table, uint16_t ind);
void modulator_pmf_update();

#endif /* INC_MODULATOR_H_ */
