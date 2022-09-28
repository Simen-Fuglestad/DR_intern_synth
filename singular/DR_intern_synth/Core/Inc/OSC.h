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

uint32_t OSC_apply(uint16_t mixer_in, float sample_in, OSC_mode_enum mode);

uint16_t OSC_res_update();
#endif /* INC_OSC_H_ */
