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

static uint8_t avg_cnt;
static const uint8_t adc_avg_n = 16;
static uint32_t mixer_avg[MIXER_ADC1_CHANNELS];

static ADC_HandleTypeDef* adc_ptr;

static ws_enum waveshape_out;

static bool mixer_adc_update_flag;
static bool mixer_btn_pressed_flag;

static bool mixer_filter_en;

static bool mixer_OSC1_en;
static bool mixer_OSC2_en;

static OSC_mode_enum mixer_OSC1_mode;
static OSC_mode_enum mixer_OSC2_mode;
static ws_enum mixer_OSC1_ws;
static ws_enum mixer_OSC2_ws;
static ws_enum mixer_OSC3_ws;

static bool mixer_PWM_en;

static const uint8_t debounce_limit = 0xFF;
static bool btn_rdy = true;
static uint8_t debounce_cnt = 20;

void mixer_LFO_toggle(void);
void mixer_LFO2_toggle(void);

#define PM_MULT_LEN 12

static const float MIXER_PM_MULTS[PM_MULT_LEN] = {
		0.0625f, 0.25f, 0.5f, 0.75f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f
};

static bool mixer_sync;

static int pm_mult_idx;

static const uint16_t ADC_HYSTERESIS_MAX_LIM = 20;

uint16_t adc_hysteresis(uint16_t next, uint16_t prev, uint16_t lim);


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

uint16_t adc_hysteresis(uint16_t next, uint16_t prev, uint16_t lim) {
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
		avg_cnt++;
		if (avg_cnt <= adc_avg_n) {
			for (uint16_t i = 0; i < MIXER_ADC1_CHANNELS; i++) {
				mixer_avg[i]+= adc_hysteresis(mixer_DMA[i], mixer_tmp[i], ADC_HYSTERESIS_MAX_LIM);
			}
		} else {
			for (uint16_t i = 0; i < MIXER_ADC1_CHANNELS; i++) {
				mixer_tmp[i] = mixer_avg[i] >> 4; //divide by 16, lowers resolution but reduces noise input
				mixer_avg[i] = 0;
			}
			avg_cnt = 0;
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

uint16_t mixer_get_PWM() {
	if (mixer_tmp[PWM_CHANNEL] >= MIXER_SOFT_CAP)
		return mixer_tmp[PWM_CHANNEL] - MIXER_SOFT_CAP;
	else
		return 0;
}

uint16_t mixer_get_df() {
	if (mixer_tmp[OSC_DF_CHANNEL] >= MIXER_SOFT_CAP) {
		return mixer_tmp[OSC_DF_CHANNEL] - MIXER_SOFT_CAP;
	} else {
		return 0;
	}
}

uint16_t mixer_get_OSC1_FM() {
	if (mixer_tmp[OSC1_FM_CHANNEL] >= MIXER_SOFT_CAP) {
		return mixer_tmp[OSC1_FM_CHANNEL] - MIXER_SOFT_CAP;
	} else {
		return 0;
	}
}

uint16_t mixer_get_OSC2_FM() {
	if (mixer_tmp[OSC2_FM_CHANNEL] >= MIXER_SOFT_CAP)
		return mixer_tmp[OSC2_FM_CHANNEL] - MIXER_SOFT_CAP;
	else
		return 0;
}

uint16_t mixer_get_pm_beta1() {
	if (mixer_tmp[PM1_BETA_CHANNEL] >= MIXER_SOFT_CAP)
		return mixer_tmp[PM1_BETA_CHANNEL] - MIXER_SOFT_CAP;
	else
		return 0;
}


uint16_t mixer_get_pm_beta2() {
	if (mixer_tmp[PM2_BETA_CHANNEL] >= MIXER_SOFT_CAP)
		return mixer_tmp[PM2_BETA_CHANNEL] - MIXER_SOFT_CAP;
	else
		return 0;
}

uint16_t mixer_get_pmf() {
	if (mixer_tmp[PMF_CHANNEL] >= MIXER_SOFT_CAP)
		return mixer_tmp[PMF_CHANNEL] - 100;
	else
		return 0;
}

ws_enum mixer_get_OSC_ws(uint8_t n) {
	switch(n) {
	case 1:
		return mixer_OSC1_ws;
	case 2:
		return mixer_OSC2_ws;
	case 3:
		return mixer_OSC3_ws;

	default:
		return;
	}
}

ws_enum mixer_get_waveshape_out() {
	return waveshape_out;
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

void mixer_cycle_wave(ws_enum* w_shape_ptr) {
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

void mixer_PM_mult_cycle() {
	pm_mult_idx = (pm_mult_idx + 1) % PM_MULT_LEN;
}

float mixer_get_PM_mult() {
	return MIXER_PM_MULTS[pm_mult_idx];
}

bool mixer_get_sync() {
	return mixer_sync;
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

		case BUTTON_OUT_WAVE_CYCLE_Pin:
			mixer_cycle_wave(&waveshape_out);
			break;

		case BUTTON_OSC1_CYCLE_Pin:
			mixer_cycle_wave(&mixer_OSC1_ws);

		case BUTTON_OSC2_CYCLE_Pin:
			mixer_cycle_wave(&mixer_OSC2_ws);
			break;

		case BUTTON_OSC3_CYCLE_Pin:
			mixer_cycle_wave(&mixer_OSC3_ws);
			break;

		case BUTTON_PHASE_MULT_Pin:
//			mixer_PM_mult_cycle();
			mixer_sync = !mixer_sync;
			break;


		default:
			break;
		}
	}
}


