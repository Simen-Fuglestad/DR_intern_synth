/*
 * mix_handler.c
 *
 *  Created on: Apr 1, 2022
 *      Author: sfs
 */

#include "mixer.h"

volatile static uint16_t mixer_DMA[MIXER_CHANNELS];
static ADC_HandleTypeDef* adc_ptr;
static bool mixer_update_flag;

void mixer_init(ADC_HandleTypeDef* adc_handle) {
	adc_ptr = adc_handle;
	mixer_update_flag = false;
}

void mixer_DMA_start(TIM_HandleTypeDef* htim) {
	if (adc_ptr) {
		HAL_TIM_Base_Start(htim);
		HAL_ADC_Start_DMA(adc_ptr, (uint32_t*)mixer_DMA, MIXER_CHANNELS);
	}
}

uint16_t mixer_get_filter_fc_low() {
	return mixer_DMA[FILTER_LOW_CHANNEL];
}

uint16_t mixer_get_filter_fc_high() {
	return mixer_DMA[FILTER_HIGH_CHANNEL];
}

uint16_t mixer_get_gain() {
	return mixer_DMA[GAIN_CHANNEL];
}

uint16_t mixer_get_tremolo() {
	return mixer_DMA[TREMOLO_CHANNEL];
}

uint16_t mixer_get_attack() {
	return mixer_DMA[ATTACK_CHANNEL];
}

uint16_t mixer_get_decay() {
	return mixer_DMA[DECAY_CHANNEL];
}

uint16_t mixer_get_sustain() {
	return mixer_DMA[SUSTAIN_CHANNEL];
}

uint16_t mixer_get_release() {
	return mixer_DMA[RELEASE_CHANNEL];
}

uint16_t mixer_get_LFO() {
	return mixer_DMA[LFO_CHANNEL];
}

bool get_mixer_update_flag() {
	return mixer_update_flag;
}

void mixer_update(uint16_t* fc_lp) {
//	fc_lp =
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if (hadc == adc_ptr) {
		mixer_update_flag = true;
	}
}


