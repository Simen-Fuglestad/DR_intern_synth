/*
 * envelope.h
 *
 *  Created on: Jul 18, 2022
 *      Author: sfs
 */

#ifndef INC_ENVELOPE_H_
#define INC_ENVELOPE_H_


#include <stdbool.h>
#include "main.h"

typedef enum {
	ENV_IDLE, ATTACK, DECAY, SUSTAIN, RELEASE, ENV_DONE
} env_stage_enum_t;

typedef struct {
	env_stage_enum_t env_stage;
	float scaler;

	bool rel_rdy;

	uint8_t MIDI_code;
	uint8_t input_index;
} env_t;

void env_ADSR_init();

void env_create(uint8_t output_ind, uint8_t midicode);
env_t* env_map_get(uint8_t index);
void env_release(uint8_t index);
void env_process(uint8_t index);

void env_update_ADSR();

void env_timer_init(TIM_HandleTypeDef* htim);

#endif /* INC_ENVELOPE_H_ */
