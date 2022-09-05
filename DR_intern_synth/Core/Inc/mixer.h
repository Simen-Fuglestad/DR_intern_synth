/*
 * mixer_input_handler.h
 *
 *  Created on: Apr 1, 2022
 *      Author: sfs
 */

#ifndef INC_MIXER_H_
#define INC_MIXER_H_


#include <OSC.h>
#include <stdbool.h>
#include "main.h"
#include "wavetable.h"

#define MIXER_RES			12 //adc resolution in bits
#define MIXER_DREF			4095 //maximum digital value of ADC
#define MIXER_VREF			2.9 //same as system vref

#define MIXER_POTM_VAL		10000
#define MIXER_POTM_ERR		0.20f
#define MIXER_SOFT_CAP		250 //caps values read by adc, prevents reading wrong values when sliders are down

#define MIXER_ADC1_CHANNELS 13 //number of conversions in ADC1

#define FILTER_LOW_CHANNEL	0
#define FILTER_HIGH_CHANNEL	1
#define VOLUME_CHANNEL 		2
#define OSC2_FM_CHANNEL		3
#define ATTACK_CHANNEL 		4
#define DECAY_CHANNEL		5
#define SUSTAIN_CHANNEL		6
#define RELEASE_CHANNEL		7
#define OSC_DF_CHANNEL		8
#define PWM_CHANNEL			9
#define OSC1_FM_CHANNEL		10
#define PM1_BETA_CHANNEL	11
#define PM2_BETA_CHANNEL	12

void mixer_init(ADC_HandleTypeDef*  adc_handle, TIM_HandleTypeDef* htim);

void mixer_update(void);
bool mixer_get_updated(void);

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
uint16_t mixer_get_OSC1_FM(void);
uint16_t mixer_get_df(void);
uint16_t mixer_get_OSC2_FM(void);
uint16_t mixer_get_pm_beta2(void);
uint16_t mixer_get_pm_beta(void);

float mixer_get_PM_mult();
void mixer_PM_mult_cycle();

ws_enum mixer_get_OSC_ws(uint8_t n);
ws_enum mixer_get_waveshape_out(void);
wave_out_mode_enum mixer_get_wave_out_mode(void);

OSC_mode_enum mixer_get_OSC1_mode(void);
OSC_mode_enum mixer_get_OSC2_mode(void);
OSC_mode_enum mixer_get_OSC3_mode();

bool mixer_get_filter_en(void);
bool mixer_is_PWM_en(void);
bool mixer_is_LFO_en(void);

#endif /* INC_MIXER_H_ */
