/*
 * key_map.h
 *
 *  Created on: Feb 21, 2022
 *      Author: sfs
 */

#ifndef INC_INPUT_HANDLER_H_
#define INC_INPUT_HANDLER_H_

//MIDI code index numbers

#define NOTE_OFF 	0x08
#define NOTE_ON		0x09
#define POLY_KEY	0x0A

#include "note_frequency.h"
#include "main.h"

#define N_KEYS //number of input keys

typedef struct {
	note_t key;
	GPIO_TypeDef* GPIO_port;
	uint16_t GPIO_pin;
} key_map_t;

void input_handler_key_map_init(key_map_t* key_map, uint16_t n_keys);
void key_map_set(uint16_t n, key_map_t* key_map, note_t* nf_map);
uint32_t poll_key_pressed(key_map_t* key_map);
#endif /* INC_INPUT_HANDLER_H_ */
