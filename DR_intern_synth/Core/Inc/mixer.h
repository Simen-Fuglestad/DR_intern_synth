/*
 * mixer_input_handler.h
 *
 *  Created on: Apr 1, 2022
 *      Author: sfs
 */

#ifndef INC_MIXER_H_
#define INC_MIXER_H_


#include <stdbool.h>
#include "main.h"
#include "wavetable.h"
#include "LFO.h"

#define MIXER_RES			12 //adc resolution in bits
#define MIXER_DIGI_REF		4095 //maximum digital value of ADC
#define MIXER_VREF			2.9 //same as system vref

#define MIXER_POTM_VAL		10000
#define MIXER_POTM_ERR		500
#define MIXER_SOFT_CAP		((float)MIXER_POTM_ERR/MIXER_POTM_VAL) * MIXER_DIGI_REF + 1 //correction value to offset errors in potmeter accuracy

#define MIXER_ADC1_CHANNELS 13 //number of conversions in ADC1

#define FILTER_LOW_CHANNEL	0
#define FILTER_HIGH_CHANNEL	1
#define VOLUME_CHANNEL 		2
#define LFO2_CHANNEL 		3
#define ATTACK_CHANNEL 		4
#define DECAY_CHANNEL		5
#define SUSTAIN_CHANNEL		6
#define RELEASE_CHANNEL		7
#define DUTY_CYCLE_CHANNEL	8
#define PWM_CHANNEL			9
#define MOD_CHANNEL			10
#define LFO_CHANNEL			11
#define PITCH_CHANNEL		12

void mixer_init(ADC_HandleTypeDef*  adc_handle, TIM_HandleTypeDef* htim);

bool mixer_update(void);

uint16_t mixer_get_filter_fc_low(void);
uint16_t mixer_get_filter_fc_high(void);
uint16_t mixer_get_volume(void);
uint16_t mixer_get_tremolo(void);
uint16_t mixer_get_attack(void);
uint16_t mixer_get_decay(void);
uint16_t mixer_get_sustain(void);
uint16_t mixer_get_release(void);
uint16_t mixer_get_duty_cycle(void);
uint16_t mixer_get_PWM(void);
uint16_t mixer_get_mod(void);
uint16_t mixer_get_LFO(void);
uint16_t mixer_get_LFO2(void);

wave_shape_enum mixer_get_waveshape_out(void);
wave_out_mode_enum mixer_get_wave_out_mode(void);

LFO_mode_enum mixer_get_LFO_mode(void);
LFO_mode_enum mixer_get_LFO2_mode(void);

bool mixer_get_filter_en(void);
bool mixer_is_PWM_en(void);
bool mixer_is_LFO_en(void);

#endif /* INC_MIXER_H_ */
