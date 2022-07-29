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

static const float ADSR_FACTOR = 500;

static float atc;
static float dec;
static float sus;
static float rel;
#define ENV_N 0x7f
static env_t envelopes[ENV_N];

static uint8_t env_index_map[ENV_N];

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


void env_update_ADSR() {
	if (mixer_get_updated()) {
		atc_update();
		dec_update();
		sus_update();
		rel_update();
	}
}

void atc_update() {
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
}

void env_process(uint8_t index) {
	env_t* env = &envelopes[env_index_map[index]];
	if (env->rel_rdy) {
		env->env_stage = RELEASE;
	}

	switch(env->env_stage) {
	case ATTACK:
		env->scaler+=atc;
		if (env->scaler >= 1) {
			env->scaler = 1;
			env->env_stage = DECAY;
		}

		break;

	case DECAY:
		env->scaler -= dec;
		if (env->scaler <= sus) {
			env->scaler = sus;
			env->env_stage = SUSTAIN;

		}

		break;

	case SUSTAIN:
		//wait for release
		break;

	case RELEASE:
		env->scaler -= rel;

		if (env->scaler <= 0) {
			env->scaler = 0;
			env->env_stage = ENV_DONE;
			env->rel_rdy = false;
		}

		break;

	case ENV_DONE:
		MIDI_note_disable(env->input_index);
		env->MIDI_code = 0;
		env->env_stage = ENV_IDLE;

		break;
	default:
		break;
	}
}

float slope_compute_next(float prev, float target, float gain) {
	return target * gain + (1.0f - gain) * prev;
}




