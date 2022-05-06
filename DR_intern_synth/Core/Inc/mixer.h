/*
 * mixer_input_handler.h
 *
 *  Created on: Apr 1, 2022
 *      Author: sfs
 */

#ifndef INC_MIXER_H_
#define INC_MIXER_H_

#define ADC_RES				12 //adc resolution in bits
#define ADC_DIGI_REF		4095 //maximum digital value of ADC
#define ADC_VREF			2.9 //same as system vref

#define MIXER_CHANNELS 		12 //==number of conversions in ADC

#define FILTER_LOW_CHANNEL	0
#define FILTER_HIGH_CHANNEL	1
#define GAIN_CHANNEL 		2
#define TREMOLO_CHANNEL 	3
#define ATTACK_CHANNEL 		4
#define DECAY_CHANNEL		5
#define SUSTAIN_CHANNEL		6
#define RELEASE_CHANNEL		7
#define OUT_WAVE_CHANNEL	8
#define OUT_MODE_CHANNEL	9
#define MOD_CHANNEL			10
#define LFO_CHANNEL			11
#define PITCH_CHANNEL		12

#include "main.h"
#include <stdbool.h>

void mixer_init(ADC_HandleTypeDef*  adc_handle, TIM_HandleTypeDef* htim);

uint16_t mixer_get_filter_fc_low();
uint16_t mixer_get_filter_fc_high();
uint16_t mixer_get_gain();
uint16_t mixer_get_tremolo();
uint16_t mixer_get_attack();
uint16_t mixer_get_decay();
uint16_t mixer_get_sustain();
uint16_t mixer_get_release();
uint16_t mixer_get_LFO();

bool mixer_get_update_flag();


#endif /* INC_MIXER_H_ */
