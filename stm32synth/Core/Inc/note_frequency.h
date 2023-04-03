/*
 * tone_frequency_table.h
 *
 *  Created on: Feb 18, 2022
 *      Author: sfs
 */

#ifndef INC_NOTE_FREQUENCY_H_
#define INC_NOTE_FREQUENCY_H_

/*
 * Music-specific constants and lookup table for notes
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#define N_OCTAVES 		9			// +1 accounts for 0th octave
#define N_SEMITONES 	12
#define ST_STEP_UP 		1.059463 	//octaves are divided into 12 semitones, 2^(1/12)
#define ST_STEP_DOWN	0.943874
#define ST_DIFF			ST_STEP_UP - ST_STEP_DOWN
#define NOTE_C0_440HZ	16.352 		// use as basis for building 440hz lookup table

#define FREQ_BASE 		8.176// c-1

typedef enum {
	SEMITONE_C, SEMITONE_C_SHARP, SEMITONE_D, SEMITONE_D_SHARP, SEMITONE_E,
	SEMITONE_F, SEMITONE_F_SHARP, SEMITONE_G, SEMITONE_G_SHARP, SEMITONE_A,
	SEMITONE_A_SHARP, SEMITONE_B
} semitone_t;

typedef struct {
	semitone_t note;
	uint8_t octave;
    float f;
    uint32_t timer_psc;
} note_t;

typedef struct {
	note_t note;
	double f; //millihertz
} nf_pair;

void nf_map_init_440(note_t* nf_lookup_table_440hz);

float nf_get_f440hz(uint8_t octave, uint8_t semitone, note_t* nf_table);
float nf_lookup_440hz_get_f(char* note, uint8_t octave, note_t* nf_lookup_table_440hz);
note_t nf_lookup_440hz_get_note(uint16_t f, note_t* nf_lookup_table_440hz);


#endif /* INC_NOTE_FREQUENCY_H_ */
