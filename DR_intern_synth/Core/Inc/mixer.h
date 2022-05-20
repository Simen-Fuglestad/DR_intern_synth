/*
 * mixer_input_handler.h
 *
 *  Created on: Apr 1, 2022
 *      Author: sfs
 */

#ifndef INC_MIXER_H_
#define INC_MIXER_H_

#include "main.h"
#include <stdbool.h>
#include "wavetable.h"

#define ADC_RES				12 //adc resolution in bits
#define ADC_DIGI_REF		4095 //maximum digital value of ADC
#define ADC_VREF			2.9 //same as system vref

#define MIXER_ADC1_CHANNELS 13 //==number of conversions in ADC1

#define FILTER_LOW_CHANNEL	0
#define FILTER_HIGH_CHANNEL	1
#define GAIN_CHANNEL 		2
#define TREMOLO_CHANNEL 	3
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
uint16_t mixer_get_gain(void);
uint16_t mixer_get_tremolo(void);
uint16_t mixer_get_attack(void);
uint16_t mixer_get_decay(void);
uint16_t mixer_get_sustain(void);
uint16_t mixer_get_release(void);
uint16_t mixer_get_duty_cycle(void);
uint16_t mixer_get_PWM(void);
uint16_t mixer_get_mod(void);
uint16_t mixer_get_LFO(void);

wave_shape_enum mixer_get_waveshape_out(void);
wave_out_mode_enum mixer_get_wave_out_mode(void);

bool mixer_get_filter_en(void);
bool mixer_get_PWM_en(void);
bool mixer_get_mono_en(void);

#endif /* INC_MIXER_H_ */
