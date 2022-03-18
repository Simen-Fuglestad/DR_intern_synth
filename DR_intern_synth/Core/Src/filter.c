/*
 * filter.c
 *
 *  Created on: Mar 11, 2022
 *      Author: sfs
 */
#include "filter.h"

uint16_t filter_apply(filter_f** filter_functions, filter_t* filters, uint16_t inp, uint8_t nf) {
	uint16_t out = inp;
	for (uint8_t i = 0; i < nf; i++) {
		out = (*filter_functions)(&filters[i], out);
	}
	return out;
}
