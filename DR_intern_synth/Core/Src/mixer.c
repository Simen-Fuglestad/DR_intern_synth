/*
 * mix_handler.c
 *
 *  Created on: Apr 1, 2022
 *      Author: sfs
 */

#include "mixer.h"
#include <stdlib.h>

volatile static uint16_t mixer_DMA[MIXER_ADC1_CHANNELS];
static uint16_t mixer_tmp[MIXER_ADC1_CHANNELS];

static ADC_HandleTypeDef* adc_ptr;

static wave_shape_enum waveshape_out;
static wave_out_mode_enum wave_out_mode;

static bool mixer_adc_update_flag;
static bool mixer_btn_pressed_flag;

static bool mixer_filter_en;

static bool mixer_OSC1_en;
static bool mixer_OSC2_en;

static OSC_mode_enum mixer_OSC1_mode;
static OSC_mode_enum mixer_OSC2_mode;

static bool mixer_PWM_en;

static const uint8_t debounce_limit = 0xFF;
static bool btn_rdy = true;
static uint8_t debounce_cnt = 20;

void mixer_LFO_toggle(void);
void mixer_LFO2_toggle(void);

static const uint16_t ADC_HYSTERESIS_MAX_LIM = 20;

static int adc_hystereis_adapter = 0;

static const uint16_t ADC_BITMASK = 0xFE0; // remove 5 LSB to handle fluctuations on ADC

uint16_t adc_hysteresis(uint16_t next, uint16_t prev, uint16_t lim, uint16_t i);


void mixer_init(ADC_HandleTypeDef* adc_handle, TIM_HandleTypeDef* htim) {
	adc_ptr = adc_handle;

	if (adc_ptr) {
		HAL_StatusTypeDef tim_init = HAL_TIM_Base_Start_IT(htim);
		HAL_StatusTypeDef adc_init = HAL_ADC_Start_DMA(adc_ptr, (uint32_t*)mixer_DMA, MIXER_ADC1_CHANNELS);

		waveshape_out = SINE;
		mixer_adc_update_flag = false;
		mixer_btn_pressed_flag = false;
		mixer_filter_en = false;
	}
}

uint16_t adc_hysteresis(uint16_t next, uint16_t prev, uint16_t lim, uint16_t i) {
	int a = abs(next - prev);

	if (lim < a) {
		return next;
	} else {
		return prev;
	}
}

void mixer_update() {
	if (mixer_adc_update_flag) {
		mixer_adc_update_flag = false;
		for (uint16_t i = 0; i < MIXER_ADC1_CHANNELS; i++) {
			mixer_tmp[i] = adc_hysteresis(mixer_DMA[i], mixer_tmp[i], ADC_HYSTERESIS_MAX_LIM, i);
//			mixer_tmp[i] = mixer_DMA[i] & ADC_BITMASK;
		}
	}

	if (!btn_rdy) {
		debounce_cnt++;
		if (debounce_cnt >= debounce_limit) {
			btn_rdy = true;
			debounce_cnt = 0;
		}
	}
}

bool mixer_get_updated() {
	return mixer_adc_update_flag;
}


uint16_t mixer_get_filter_fc_low() {
	if (mixer_DMA[FILTER_LOW_CHANNEL] >= MIXER_SOFT_CAP) {
		return mixer_DMA[FILTER_LOW_CHANNEL] - MIXER_SOFT_CAP;
	}
	else {
		return 0;
	}
}



uint16_t mixer_get_filter_fc_high() {
	if (mixer_tmp[FILTER_HIGH_CHANNEL] >= MIXER_SOFT_CAP) {
		return mixer_tmp[FILTER_HIGH_CHANNEL] - MIXER_SOFT_CAP;
	}
	else {
		return 0;
	}
}

uint16_t mixer_get_volume() {
	return mixer_tmp[VOLUME_CHANNEL];
}

uint16_t mixer_get_attack() {
	return mixer_tmp[ATTACK_CHANNEL];
}

uint16_t mixer_get_decay() {
	return mixer_tmp[DECAY_CHANNEL];
}

uint16_t mixer_get_sustain() {
	return mixer_tmp[SUSTAIN_CHANNEL];
}

uint16_t mixer_get_release() {
	return mixer_tmp[RELEASE_CHANNEL];
}

uint16_t mixer_get_duty_cycle(void) {
	return mixer_tmp[FM_DF_CHANNEL];
}

uint16_t mixer_round_val(uint16_t val, uint16_t r) {

}

uint16_t mixer_get_PWM() {
	if (mixer_tmp[PWM_CHANNEL] >= MIXER_SOFT_CAP)
		return mixer_tmp[PWM_CHANNEL] - MIXER_SOFT_CAP;
	else
		return 0;
}
uint16_t mixer_get_fm() {
//	static uint16_t threshold;
	if (mixer_tmp[FM_MOD_CHANNEL] >= MIXER_SOFT_CAP) {
//		threshold = MIXER_SOFT_CAP >> 1;
		return mixer_tmp[FM_MOD_CHANNEL] - MIXER_SOFT_CAP;
	} else {
//		threshold = MIXER_SOFT_CAP;
		return 0;
	}
//	return mixer_tmp[FM_MOD_CHANNEL];
}



uint16_t mixer_get_df() {
//	static uint16_t threshold_df;
	if (mixer_tmp[FM_DF_CHANNEL] >= MIXER_SOFT_CAP) {
//		threshold_df = MIXER_SOFT_CAP >> 1;
		return mixer_tmp[FM_DF_CHANNEL] - MIXER_SOFT_CAP;
	} else {
//		threshold_df = MIXER_SOFT_CAP;
		return 0;
	}
//	return mixer_tmp[FM_DF_CHANNEL];
}

uint16_t mixer_get_OSC1() {
	if (mixer_tmp[OSC1_CHANNEL] >= MIXER_SOFT_CAP)
		return mixer_tmp[OSC1_CHANNEL] - MIXER_SOFT_CAP;
	else
		return 0;
//	return mixer_tmp[OSC1_CHANNEL];
}

uint16_t mixer_get_OSC2() {
	if (mixer_tmp[OSC2_CHANNEL] >= MIXER_SOFT_CAP)
		return mixer_tmp[OSC2_CHANNEL] - MIXER_SOFT_CAP;
	else
		return 0;
}

wave_shape_enum mixer_get_waveshape_out() {
	return waveshape_out;
}

wave_out_mode_enum mixer_get_wave_out_mode() {
	return wave_out_mode;
}

OSC_mode_enum mixer_get_OSC1_mode() {
	return mixer_OSC1_mode;
}

OSC_mode_enum mixer_get_OSC2_mode() {
	return mixer_OSC2_mode;
}

bool mixer_get_filter_en() {
	return mixer_filter_en;
}

bool mixer_is_PWM_en() {
	return mixer_PWM_en;
}

bool mixer_is_LFO_en() {
	return mixer_OSC1_en;
}

void mixer_cycle_wave(wave_shape_enum* w_shape_ptr) {
	switch (*w_shape_ptr) {
	case SINE:
		*w_shape_ptr = SQUARE;
		break;
	case SQUARE:
		*w_shape_ptr = TRIANGLE;
		break;
	case TRIANGLE:
		*w_shape_ptr = SAWTOOTH;
		break;
	default:
		*w_shape_ptr = SINE;
		break;
	}
}

void mixer_cycle_mode() {
	switch(wave_out_mode) {
	case AM:
		wave_out_mode = FM;
		break;
	case FM:
		wave_out_mode = AM;
		break;
	case PM:
		wave_out_mode = PM;
		break;
	default:
		break;
	}
}

void mixer_cycle_OSC_mode(OSC_mode_enum* lfo_mode_ptr) {
	switch(*lfo_mode_ptr) {
	case LFO_TREMOLO:
		*lfo_mode_ptr = LFO_PITCH;
		break;
	case LFO_PITCH:
		*lfo_mode_ptr = LFO_FLUTTER;
		break;
	case LFO_FLUTTER:
		*lfo_mode_ptr = LFO_TREMOLO;
	default:
		break;
	}
}

void mixer_LFO_toggle() {
	mixer_OSC1_en = !mixer_OSC1_en;
}

void mixer_LFO2_toggle() {
	mixer_OSC2_en = !mixer_OSC2_en;
}

void mixer_PWM_toggle() {
	mixer_PWM_en = !mixer_PWM_en;
}

void mixer_filter_toggle() {
	mixer_filter_en = !mixer_filter_en;
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc) {
	if (hadc == adc_ptr) {
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if (hadc == adc_ptr) {
		mixer_adc_update_flag = true;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (btn_rdy) {
		btn_rdy = false;
		switch(GPIO_Pin) {

		case BUTTON_WAVE_CYCLE_Pin:
			mixer_cycle_wave(&waveshape_out);
			break;

		case BUTTON_OSC1_MODE_Pin:
			mixer_cycle_OSC_mode(&mixer_OSC1_mode);
			break;

		case BUTTON_OSC2_MODE_Pin:
			mixer_cycle_OSC_mode(&mixer_OSC2_mode);
			break;

		default:
			break;
		}
	}
}


uint16_t scale_mixer_val(uint16_t in, float start, float end) {

}


