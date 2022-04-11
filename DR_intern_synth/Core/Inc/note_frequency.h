/*
 * tone_frequency_table.h
 *
 *  Created on: Feb 18, 2022
 *      Author: sfs
 */

#ifndef INC_NOTE_FREQUENCY_H_
#define INC_NOTE_FREQUENCY_H_

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#define N_OCTAVES 			9// +1 accounts for 0th octave
#define N_NOTES				7 //full notes
#define N_FLATS				5
#define N_SEMITONES 		12
#define OCTAVE_STEP 		1.0594 //octaves are divided into 12 semitones, 2^(1/12)
#define NOTE_C0_440HZ		16.352 // use as basis for building 440hz lookup table
#define NOTE_Db0			NOTE_C0_440HZ * OCTAVE_STEP
#define NOTE_D0				NOTE_Db0 * OCTAVE_STEP
#define NOTE_Eb0			NOTE_D0 * OCTAVE_STEP

#define NOTE_C0_PSC 		13690

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

//nf_pair nf_lookup_table_440hz[N_OCTAVES * N_SEMITONES];

void nf_map_init_440(note_t* nf_lookup_table_440hz);

float nf_get_f440hz(uint8_t octave, uint8_t semitone, note_t* nf_table);
float nf_lookup_440hz_get_f(char* note, uint8_t octave, note_t* nf_lookup_table_440hz);
note_t nf_lookup_440hz_get_note(uint16_t f, note_t* nf_lookup_table_440hz);



//#define NF_PAIR_INIT(t, f)

#endif /* INC_NOTE_FREQUENCY_H_ */
