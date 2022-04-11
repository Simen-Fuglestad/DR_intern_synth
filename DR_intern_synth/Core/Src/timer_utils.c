/*
 * timer_utils.c
 *
 *  Created on: Feb 18, 2022
 *      Author: sfs
 */

#include "timer_utils.h"
#include <math.h>

static uint32_t WAVEGEN_CLK;

void timer_utils_init_wavegen_clk() {
	WAVEGEN_CLK = HAL_RCC_GetSysClockFreq(); //set wave gen = SYSCLK
}

void timer_utils_set_PSC(TIM_HandleTypeDef* htim, uint16_t PSC) {
	htim->Instance->PSC = PSC - 1;
}
void timer_utils_set_ARR(TIM_HandleTypeDef* htim, uint32_t ARR) {
	htim->Instance->ARR = ARR - 1;
}

void timer_utils_set_f_wave(TIM_HandleTypeDef* htim, float f_out, uint16_t ns) {

	float trig_f = f_out * ns;

	uint16_t psc = get_timer_PSC(htim);

	float f_sys_clk = (float)WAVEGEN_CLK;

	float arr = f_sys_clk/(psc * trig_f);

	arr = (uint32_t)round(arr);

	timer_utils_set_ARR(htim, arr);
}

void set_timer_frequency_max(TIM_HandleTypeDef* htim) {
	timer_utils_set_PSC(htim, 0);
	timer_utils_set_ARR(htim, 1);
}

uint16_t get_timer_PSC(TIM_HandleTypeDef* htim) {
	return htim->Instance->PSC + 1; //note that PSC register holds PSC-1
}
uint16_t get_timer_ARR(TIM_HandleTypeDef* htim) {
	return htim->Instance->ARR + 1;
}

uint32_t get_timer_frequency(TIM_HandleTypeDef* timer) {
	return WAVEGEN_CLK/(get_timer_PSC(timer) * get_timer_ARR(timer));
}




