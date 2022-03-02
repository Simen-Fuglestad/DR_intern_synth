/*
 * key_map.c
 *
 *  Created on: Feb 21, 2022
 *      Author: sfs
 */

#include <input_handler.h>
#include "main.h"

void key_map_set(uint16_t n, key_map_t* key_map, note_t* nf_map) {
	key_map_t k;
	for (uint16_t i = 0; i < n; i++) {
		k.key = nf_map[i];
//		k.adc_channel = i + 1;
		key_map[i] = k;
	}
}

void input_handler_key_map_init(key_map_t* key_map, uint16_t n_keys) {
	key_map_t km;
	for (uint16_t i = 0; i < n_keys; i++) {
//		key_map
	}
}

uint32_t poll_key_pressed(key_map_t* key_map) {
//	if (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin)) {

//	}
}
