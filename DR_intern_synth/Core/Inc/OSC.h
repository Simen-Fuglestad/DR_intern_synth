/*
 * LFO.h
 *
 *  Created on: Jun 1, 2022
 *      Author: sfs
 */

#ifndef INC_OSC_H_
#define INC_OSC_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	LFO_TREMOLO, LFO_PITCH, LFO_FLUTTER
} OSC_mode_enum;

uint32_t OSC_apply(uint16_t mixer_in, uint32_t sample_in, uint16_t out_val_ind, uint16_t* wt, OSC_mode_enum mode);
#endif /* INC_OSC_H_ */
