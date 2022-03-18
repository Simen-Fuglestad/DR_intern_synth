/*
 * fm_modulator.c
 *
 *  Created on: Mar 15, 2022
 *      Author: sfs
 */

#include "modulator.h"

static const float F_DELTA_DEFAULT = 0.1;

void fm_modulator_init(
		fm_modulator_t* mod, float fc, float fm,
		float f_delta, uint16_t ac, uint16_t am, uint16_t nsm) {

	mod->fc = fc;
	mod->fm = fm;
	mod->f_delta = F_DELTA_DEFAULT;
	mod->am = am;
	mod->ac = ac;
	mod->nsm = nsm;
}

uint16_t fm_modulator_update(fm_modulator_t* mod, uint16_t x, uint16_t index) {
//	uint16_t y = mod->ac * cos(2 * M_PI * fc * (index/mod->nsm) + (mod->f_delta/mod->fm) * (sin(x *)))
}
