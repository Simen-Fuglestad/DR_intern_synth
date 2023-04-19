#include <math.h>
#include <stdbool.h>
#include  "modulator.h"
#include "mixer.h"

// FM PARAMETERS
static float fm_osc;									// FM modulator value
static float df; 										// FM modulation depth
static float fm; 										// FM modulation frequency of fm_osc
static float fm_speed;									// FM modulation frequency of fm_osc2
static float k1;										// kf = delta_f/amplitude, see wikipage on fm modulation
static float k2;

static const float FS = 22050; 							// sample rate
static const float FM_VIB_MAX = 3.0f/MIXER_DREF;		// FM vibrato threshold=2xf0
static const float FM_BOUND = 0.01; 					// Ignore fm below

// GENERAL PARAMTERES
static const float TEMPER_RATIO = 1.0f/12.0f;			//12 tone temperament
// PM PARAMETERS
static float beta; 										// PM modulation depth for pm_osc1
static float beta_scaler; 								// PM modulation depth scaler
static float pmf;										// PM modulation sync frequency

static const float BETA_DS = MIXER_DREF >> 1;			// Depth scaling beta
						
static float pm_mods[MODULATOR_MAX_VOICES];
			
float modulator_get_next(float mod_step, float *mod_table) {
	float m = mod_table[(uint16_t)mod_step];
	return m;
}

void modulator_update_fm_osc() {
	fm_osc += fm;
	if (fm_osc >= N_WT_SAMPLES) {
		fm_osc -= N_WT_SAMPLES;
	}
}

uint16_t modulator_modu16(uint16_t modulator, uint16_t *carrier, uint16_t ref) {
	uint16_t m = carrier[modulator] - ref;
	return m;
}

void modulator_update_df() {
	df = (float)mixer_get_df()/MIXER_DREF;
}

void modulator_update_fm() {
	fm = (float)mixer_get_OSC1_FM() * FM_VIB_MAX;
//	fm_speed = (float)mixer_get_OSC2_FM()/MIXER_DREF;
}

float modulator_compute_k(int kn) {
	/*
	 * DEPRECATED: Might be useful if converting to fixed point presentation
	 * Premise: If amplitude is given digitally, we need to recompute sensitivity k for a given frequency deviation
	 *
	 */
	float *k;
	float *fmx;
	switch(kn) {
		case 1:
			k = &k1;
			fmx = &fm;
			break;
		case 2:
			k = &k2;
			fmx = &fm_speed;
			break;
		default:
			return -1;
			break;
	}
	if (*fmx > FM_BOUND) {
		*k = (df * FS) / (*fmx * 2 * M_PI);
	}
	else {
		*k = 0;
	}
	return *k;
}

void modulator_update_beta() {
	beta  = (float)mixer_get_pm_beta1()/BETA_DS;
	beta_scaler = (float)mixer_get_pm_beta2()/MIXER_DREF;
}

float modulator_update_beta1() {
	return beta;
}

float modulator_update_beta2() {
	return beta_scaler;
}

void modulator_pmf_update() {
	float ipmf = (float)mixer_get_pmf()/MIXER_DREF;
	float pmf_scaler = roundf(ipmf/TEMPER_RATIO); // scale in steps of 12
	pmf = powf(NOTE_F_UP, pmf_scaler) + TEMPER_RATIO;
}

float modulator_get_fm_osc() {
	return fm_osc;
}

float modulator_get_df() {
	return df;
}

float modulator_get_next_pm(float mod, float *wt, uint16_t ind) {
	bool pmf_en = mixer_get_sync();
	if (pmf_en) {
		pm_mods[ind] += mod * pmf;
	} else {
		pm_mods[ind] += mod;
	}
	if (pm_mods[ind] >= N_WT_SAMPLES) {
		pm_mods[ind] -= N_WT_SAMPLES;
	}
	float m = wt[(uint16_t)pm_mods[ind]];
	float next = m * beta * beta_scaler;
	return next;
}
