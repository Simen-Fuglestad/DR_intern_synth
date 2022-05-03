/*
 * filter.c
 *
 *  Created on: Mar 11, 2022
 *      Author: sfs
 */
#include "filter.h"

void filter_update_all() {
	filter_lowpass_update();
//	filter_highpass_update();
}
