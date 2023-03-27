/*
 * modulator.h
 *
 *  Created on: Mar 14, 2022
 *      Author: sfs
 */

#ifndef INC_MODULATOR_H_
#define INC_MODULATOR_H_

#include <stdint.h>

#define MODULATOR_MAX_VOICES 5 //Nr of "voices", aka simultaneous notes that can be played. More voices imply higher performance requirements

float modulator_get_next(float mod_step, float *mod_table);
float modulator_updatef(float mod, float upd, float ref);

uint16_t modu16(uint16_t modulator, uint16_t carrier);
uint16_t modu_16_update(uint16_t modulator, uint16_t ref);

float modulator_update_fm_osc(int osc_n);
float modulator_update_df();
void modulator_update_fm();
float modulator_get_k(int kn);
float modulator_compute_k(int kn);

float modulator_get_fm_osc(int fm_osc_n);
float modulator_get_fm_osc1();
float modulator_get_fm_osc2();
float modulator_get_df();
float modulator_get_fm(int fnm);

float modulator_update_ipmf();
void modulator_update_beta();

float modulator_get_next_pm(float mod, float *mod_table, uint16_t ind, int pmn);
float modulator_get_beta(int beta_n);
float modulator_pmf_update();
float modulator_get_beta1_ds();
float modulator_get_beta2_ds();

float modulator_get_TEMPER_RATIO();
float modulator_get_NOTE_F_UP();
float modulator_get_NOTE_F_DOWN();

#endif /* INC_MODULATOR_H_ */
