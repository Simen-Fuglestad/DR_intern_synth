/*
 * LFO.h
 *
 *  Created on: Jun 1, 2022
 *      Author: sfs
 */

#ifndef INC_LFO_H_
#define INC_LFO_H_

#include <stdint.h>

typedef enum {
	LFO_VOLUME, LFO_PITCH
} LFO_mode_enum;

uint16_t LFO_apply(uint16_t mixer_in, uint16_t out_val, uint16_t* wt, LFO_mode_enum mode);
#endif /* INC_LFO_H_ */
