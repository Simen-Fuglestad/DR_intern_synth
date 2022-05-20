/*
 * filter.c
 *
 *  Created on: Mar 11, 2022
 *      Author: sfs
 */
#include "filter.h"


void filter_update() {
	if (mixer_update()) {
		filter_lp_update();
		filter_hp_update();
	}
}
