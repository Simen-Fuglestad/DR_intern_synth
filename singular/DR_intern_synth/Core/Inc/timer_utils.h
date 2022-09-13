/*
 * signal_proc_utils.h
 *
 *  Created on: Feb 18, 2022
 *      Author: sfs
 */

#ifndef INC_TIMER_UTILS_H_
#define INC_TIMER_UTILS_H_

#include <main.h>

void timer_utils_init_wavegen_clk();

void init_timer(TIM_HandleTypeDef* htim);

void timer_utils_set_PSC(TIM_HandleTypeDef* timer, uint16_t PSC);
void timer_utils_set_ARR(TIM_HandleTypeDef* timer, uint32_t ARR);

void timer_utils_set_f_wave(TIM_HandleTypeDef* htim, float frequency, uint16_t ns);

void set_timer_frequency_max(TIM_HandleTypeDef* htim);

uint16_t get_timer_PSC(TIM_HandleTypeDef* timer);
uint16_t get_timer_ARR(TIM_HandleTypeDef* timer);
uint32_t get_timer_frequency(TIM_HandleTypeDef* timer);

#endif /* INC_TIMER_UTILS_H_ */
