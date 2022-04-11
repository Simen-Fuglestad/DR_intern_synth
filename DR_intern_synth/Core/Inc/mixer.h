/*
 * mixer_input_handler.h
 *
 *  Created on: Apr 1, 2022
 *      Author: sfs
 */

#ifndef INC_MIXER_H_
#define INC_MIXER_H_

#define MIXER_CHANNELS 		9 //==number of conversions in ADC

#define FILTER_LOW_CHANNEL 	0
#define GAIN_CHANNEL 		1
#define TREMOLO_CHANNEL 	2
#define ATTACK_CHANNEL 		3
#define DECAY_CHANNEL		4
#define SUSTAIN_CHANNEL		5
#define RELEASE_CHANNEL		6
#define LFO_CHANNEL			7
#define FILTER_HIGH_CHANNEL	8

#include "main.h"
#include <stdbool.h>


void mixer_init(ADC_HandleTypeDef*  adc_handle);
void mixer_DMA_start();

uint16_t mixer_get_filter_fc_low();
uint16_t mixer_get_filter_fc_high();
uint16_t mixer_get_gain();
uint16_t mixer_get_tremolo();
uint16_t mixer_get_attack();
uint16_t mixer_get_decay();
uint16_t mixer_get_sustain();
uint16_t mixer_get_release();
uint16_t mixer_get_LFO();

bool get_mixer_update_flag();


#endif /* INC_MIXER_H_ */
