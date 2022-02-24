/*
 * note_frequency.c
 *
 *  Created on: Feb 18, 2022
 *      Author: sfs
 */

#include "note_frequency.h"

char* CHROMA_SCALE[N_SEMITONES] = {
    "C\0", "C#\0", "D\0", "D#\0", "E\0", "F\0", "F#\0", "G\0", "G#\0", "A\0", "A#\0", "B\0"
};

void nf_map_init_440(note_t* nf_lookup_table_440hz) {
	note_t note;
    const double p = (double)1/12;
	const double F_STEP = pow(2, p);
	float current_f = NOTE_C0_440HZ;

	for (uint16_t oct = 0; oct < N_OCTAVES; oct++) {
		for (uint16_t st = 0; st < N_SEMITONES; st++) {
			semitone_t semitone = st;
			note.note = semitone;
			note.octave = oct;
            note.f = current_f;

            current_f = current_f * F_STEP;

			nf_lookup_table_440hz[oct*N_SEMITONES + st] = note;
		}
	}
}

float nf_get_f440hz(semitone_t semitone, uint8_t octave, note_t* nf_table) {
	return nf_table[octave * 12 + semitone].f;
}
