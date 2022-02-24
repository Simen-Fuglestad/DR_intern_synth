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
//#define OCTAVE_STEP 		1.0595 //octaves are divided into 12 semitones, 2^(1/12)
#define NOTE_C0_440HZ		16.352 // use as basis for building 440hz lookup table


typedef enum {
	C, C_SHARP, D, D_SHARP, E, F, F_SHARP, G, G_SHARP, A, A_SHARP, B
} semitone_t;

typedef struct {
	semitone_t note;
	uint8_t octave;
    float f;
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
