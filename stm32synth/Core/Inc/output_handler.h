/*
 * output_handler.h
 *
 *  Created on: Apr 22, 2022
 *      Author: sfs
 */

#ifndef INC_OUTPUT_HANDLER_H_
#define INC_OUTPUT_HANDLER_H_

#include <stdint.h>

void output_handler_init();

void output_handler_outwave_update(uint16_t* out, uint16_t out_start, uint16_t out_len, float* wavetable);

float* output_handler_get_steps();

#endif /* INC_OUTPUT_HANDLER_H_ */
