/*
 * filter.c
 *
 *  Created on: Mar 11, 2022
 *      Author: sfs
 */
#include "filter.h"


void filter_update_all() {
	if (mixer_get_update_flag()) {
		filter_lp_update();
		filter_hp_update();
	}
}
