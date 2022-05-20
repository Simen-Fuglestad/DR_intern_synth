/*
 * mix_handler.c
 *
 *  Created on: Apr 1, 2022
 *      Author: sfs
 */

#include "mixer.h"

volatile static uint16_t mixer_DMA[MIXER_ADC1_CHANNELS];
static ADC_HandleTypeDef* adc_ptr;

static wave_shape_enum waveshape_out;
static wave_out_mode_enum wave_out_mode;

static bool mixer_adc_update_flag;
static bool mixer_btn_pressed_flag;

static bool mixer_filter_en;
static bool mixer_mono_en;
static bool mixer_LFO_en;
static bool mixer_PWM_en;

static const uint8_t debounce_limit = 0xFF;
static bool btn_rdy = true;
static uint8_t debounce_cnt = 0;
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

bool mixer_update() {
	bool tmp = mixer_adc_update_flag;
	if (mixer_adc_update_flag) {
		mixer_adc_update_flag = false;
	}

	if (!btn_rdy) {
		debounce_cnt++;
		if (debounce_cnt >= debounce_limit) {
			btn_rdy = true;
			debounce_cnt = 0;
		}
	}
	return tmp;
}


uint16_t mixer_get_filter_fc_low() {
	return mixer_DMA[FILTER_LOW_CHANNEL];
}

uint16_t mixer_get_filter_fc_high() {
	if (mixer_DMA[FILTER_HIGH_CHANNEL] > 20) {
		return mixer_DMA[FILTER_HIGH_CHANNEL];
	}
	else {
		return 0;
	}
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

uint16_t mixer_get_duty_cycle(void) {
	return mixer_DMA[DUTY_CYCLE_CHANNEL];
}

uint16_t mixer_get_PWM() {
	return mixer_DMA[PWM_CHANNEL];
}

uint16_t mixer_get_mod() {
	return mixer_DMA[MOD_CHANNEL];
}

uint16_t mixer_get_LFO() {
	return mixer_DMA[LFO_CHANNEL];
}


wave_shape_enum mixer_get_waveshape_out(void) {
	return waveshape_out;
}

wave_out_mode_enum mixer_get_wave_out_mode(void) {
	return wave_out_mode;
}

bool mixer_get_filter_en() {
	return mixer_filter_en;
}

bool mixer_get_PWM_en() {
	return mixer_PWM_en;
}

bool mixer_get_mono_en() {
	return mixer_mono_en;
}

void mixer_cycle_wave() {
	switch (waveshape_out) {
	case SINE:
		waveshape_out = SQUARE;
		break;
	case SQUARE:
		waveshape_out = TRIANGLE;
		break;
	case TRIANGLE:
		waveshape_out = SAWTOOTH;
		break;
	default:
		waveshape_out = SINE;
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
		//PM coming soon!
		break;
	}
}

void mixer_LFO_toggle() {
	mixer_LFO_en = !mixer_LFO_en;
}

void mixer_PWM_toggle() {
	mixer_PWM_en = !mixer_PWM_en;
}

void mixer_mono_toggle() {
	mixer_mono_en = !mixer_mono_en;
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
			mixer_cycle_wave();
			break;
		case BUTTON_WAVE_MODE_Pin:
			mixer_cycle_mode();
			break;
		case BUTTON_LFO_ENABLE_Pin:
			mixer_LFO_toggle();
			break;
		case BUTTON_PWM_ENABLE_Pin:
			mixer_PWM_toggle();
			break;
		case BUTTON_MONO_TOGGLE_Pin:
			mixer_mono_toggle();
			break;
		case BUTTON_FILTER_ENABLE_Pin:
			mixer_filter_toggle();
			break;
		default:
			break;
		}
	}
}


uint16_t scale_mixer_val(uint16_t in, float start, float end) {

}


