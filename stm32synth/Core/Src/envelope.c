/*
 * envelope.c
 *
 *  Created on: Jul 18, 2022
 *      Author: sfs
 */
#include <stdint.h>
#include <stdbool.h>

#include "envelope.h"
#include "mixer.h"
#include "MIDI_application.h"

#define ENV_NS_MAX 64

static const uint16_t ADSR_FACTOR = 2048;

static float atc;
static float dec;
static float sus;
static float rel;

static env_t envelopes[MIDI_CODE_RANGE];

static uint8_t env_index_map[MIDI_CODE_RANGE];

float slope_compute_next(float prev, float target, float gain);

void draw_slope(float* slope, int slope_len, float start_val, float target, float gain);
void draw_lin_slope(float* slope, int len, float start, float target);

void atc_update();
void dec_update();
void sus_update();
void rel_update();

void draw_slope(float* slope, int slope_len, float start_val, float target, float gain) {
	slope[0] = start_val;
	for (int i = 1; i < slope_len; i++) {
		slope[i] = slope_compute_next(slope[i-1], target, gain);
	}
}

void draw_lin_slope(float* slope, int len, float start, float target) {
	float step_size = (target - start)/len;
	for (uint16_t i = 0; i < len; ++i) {
		slope[i] = start + step_size * i;
	}
}

void env_ADSR_init() {
	atc_update();
	dec_update();
	sus_update();
	rel_update();
}

int env_update_ADSR() {
	if (mixer_get_updated()) {
		atc_update();
		dec_update();
		sus_update();
		rel_update();
		return 0;
	}
	return -1;
}

void atc_update() {
	// atc = (float)mixer_get_attack()/MIXER_DREF;
	atc = (float)mixer_get_attack()/(MIXER_DREF * ADSR_FACTOR);
}

void dec_update() {
	dec = (float)mixer_get_decay()/(MIXER_DREF * ADSR_FACTOR);
}

void sus_update() {
	sus = (float)mixer_get_sustain()/MIXER_DREF;
}

void rel_update() {
	rel = (float)mixer_get_release()/(MIXER_DREF * ADSR_FACTOR);
}

void env_create(uint8_t index, uint8_t midicode) {
	env_index_map[index] = midicode;
	envelopes[midicode].env_stage = ATTACK;

	envelopes[midicode].rel_rdy = false;
	envelopes[midicode].scaler = 0;

	envelopes[midicode].MIDI_code = midicode;
	envelopes[midicode].input_index = index;
}

env_t* env_map_get(uint8_t index) {
	uint16_t i = env_index_map[index];
	return &envelopes[i];
}

void env_release(uint8_t midi_index) {
	envelopes[midi_index].rel_rdy = true;
	env_index_map[midi_index] = 0;
}

void env_process_update() {
	static uint8_t index;
	env_process(index);
	index++;
	if (index >= 10) { //nr of inputs
		index = 0;
	}
}

void env_process_by_index(uint8_t index) {
	env_t* env = &envelopes[env_index_map[index]];
	env_process(env);
}

void env_process(env_t* env) {
	if (env->rel_rdy) {
		env->env_stage = RELEASE;
	}

	switch(env->env_stage) {

	case ENV_IDLE:
		break;

	case ATTACK:
		if (env->scaler >= 1) {
			env->scaler = 1;
			env->env_stage = DECAY;
		} else {
			env->scaler+=atc;
		}

		break;

	case DECAY:
		if (env->scaler <= sus) {
			env->scaler = sus;
			env->env_stage = SUSTAIN;
		} else {
			env->scaler -= dec;
		}

		break;

	case SUSTAIN:
		env->scaler = sus;
		break;

	case RELEASE:
		if (env->scaler <= 0) {
			env->scaler = 0;
			env->env_stage = ENV_DONE;
			env->rel_rdy = false;
			MIDI_note_disable(env->MIDI_code, env->input_index);
		} else {
			env->rel_rdy = false;
			env->scaler -= rel;
		}

		break;

	case ENV_DONE:
		env_index_map[env->input_index] = 0;
		env->input_index = 0;
//		env->scaler = 0;
		env->MIDI_code = 0;
		env->env_stage = ENV_IDLE;

		break;
	default:
		env->env_stage = ENV_DONE;
		break;
	}
}

float slope_compute_next(float prev, float target, float gain) {
	return target * gain + (1.0f - gain) * prev;
}




