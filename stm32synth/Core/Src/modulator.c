#include <math.h>
#include <stdbool.h>
#include  "modulator.h"
#include "mixer.h"

// FM PARAMETERS
static float fm_osc1;							// FM modulator value	
static float fm_osc2;  							// FM modulator value
static float df; 							// FM modulation depth
static float fm1; 							// FM modulation frequency of fm_osc1
static float fm2;							// FM modulation frequency of fm_osc2
static float k1;							// kf = delta_f/amplitude, see wikipage on fm modulation
static float k2;

static const float FS = 44050; 						// sample rate
static const float FM_TS2 = 3.0f/MIXER_DREF;				// FM vibrato threshold=2xf0
static const float FM_TS05 = 0.5f/MIXER_DREF;				// FM vibrato f=0.5xfx0
static const float FM_BOUND = 0.01; 					// Ignore fm below
static const float F_C0 = 16.352;					// Lowest permitted tone output in Hz
static const float DF_RANGE = 1.0f/MIXER_DREF;				// Modulation depth

// GENERAL PARAMTERES
static const float TEMPER_RATIO = 1.0f/12.0f;		//12 tone temparement
static const float NOTE_F_UP = 1.059;					//2^(1/12), each successive note is 1.059 times faster than the previous
static const float NOTE_F_DOWN = 0.943;					//2^(-1/12), descending notes
static uint16_t REF_CENTER = MIXER_DREF / 2;

// PM PARAMETERS
static float beta1; 							// PM modulation depth for pm_osc1
static float beta2; 							// PM modulation depth for pm_osc2
static float pmf;								// PM modulation sync frequency

static const float BETA1_DS = MIXER_DREF;				// Depth scaling beta1
static const float BETA2_DS = MIXER_DREF / 10;				// Depth scaling beta2
						
static float pm1_mods[MODULATOR_MAX_VOICES];
static float pm2_mods[MODULATOR_MAX_VOICES];
			
float modulator_get_next_fm(float mod_step, uint16_t *mod_table, float k) {
	uint16_t m = mod_table[(uint16_t)mod_step];
	float f = (mod_table[m] - REF_CENTER) * k;
	return f;	
}

float modulator_update_fm_osc(int osc_n) {
	float* f_osc;
	float* f_step;
	switch(osc_n) {
		case 1:
			f_osc = &fm_osc1;
			f_step = &fm1;
			break;
		case 2:
			f_osc = &fm_osc2;
			f_step = &fm2;
			break;
		default:
			return -1;
	}
	*f_osc += *f_step;
	if (*f_osc >= N_WT_SAMPLES) {
		*f_osc -= N_WT_SAMPLES;
	}
     return *f_osc;
}	


uint16_t modulator_modu16(uint16_t modulator, uint16_t *carrier, uint16_t ref) {
	uint16_t m = carrier[modulator] - ref;
	return m;
}

float modulator_update_df() {
	df = (float)mixer_get_df() * DF_RANGE; //get a separate slider for DF_DIV?
	return df;
}

void modulator_update_fm() {
	fm1 = (float)mixer_get_OSC1_FM() * FM_TS05;
	fm2 = (float)mixer_get_OSC2_FM() * FM_TS2;
}

float modulator_compute_k(int kn) {
	float *k;
	float *fmx;
	switch(kn) {
		case 1:
			k = &k1;
			fmx = &fm1;
			break;
		case 2:
			k = &k2;
			fmx = &fm2;
			break;
		default:
			return -1;
			break;
	}
	if (*fmx > FM_BOUND) {
		*k = (df * FS) / (*fmx * 2 * M_PI * REF_CENTER);
	}
	else {
		*k = 0;
	}
	return *k;
}

void modulator_update_beta() {
	beta1  = (float)mixer_get_pm_beta1()/BETA1_DS;
	beta2 = (float)mixer_get_pm_beta2()/BETA2_DS;
}

float modulator_update_beta1() {
	return beta1;
}

float modulator_update_beta2() {
	return beta2;
}

float modulator_pmf_update() {
	float ipmf = (float)mixer_get_pmf()/MIXER_DREF;
	float pmf_scaler = roundf(ipmf/TEMPER_RATIO); // scale in steps of 12
	pmf = powf(NOTE_F_UP, pmf_scaler) + TEMPER_RATIO;
	return pmf;
}

float modulator_get_fm_osc(int fm_osc_n) {
	switch (fm_osc_n) {
		case 1:
			return fm_osc1;
		case 2:
			return fm_osc2;
		default:
			return -1;
	}
}

float modulator_get_df() {
	return df;
}

float modulator_get_fm(int fmn) {
	switch(fmn) {
		case 1:
			return fm1;
		case 2:
			return fm2;
		default:
			return -1;
	}
}

float modulator_get_next_pm(float mod, uint16_t *wt, uint16_t ind, int pmn) {
	float *pm_mods;
	float *beta;
	switch(pmn) {
		case 1:
			pm_mods = pm1_mods;
			beta = &beta1;
			break;
		case 2:
			pm_mods = pm2_mods;
			beta = &beta2;
			break;
		default:
			return -1;
	}

	bool pmf_en = mixer_get_sync();
	if (pmf_en) {
		pm_mods[ind] += mod * pmf;
	} else {
		pm_mods[ind] += mod;
	}
	if (pm_mods[ind] >= N_WT_SAMPLES) {
		pm_mods[ind] -= N_WT_SAMPLES;
	}
	uint16_t m = wt[(uint16_t)pm_mods[ind]];
	float next = (m - REF_CENTER) * (*beta);
	return next;
}


float modulator_get_beta(int beta_n) {
	switch (beta_n) {
		case 1:
			return beta1;
		case 2:
			return beta2;
		default:
			return -1;
	}
}

float modulator_get_beta_ds(int beta_dsn) {
	switch(beta_dsn) {
		case 1:
			return BETA1_DS;
		case 2:
			return BETA2_DS;
		default:
			return -1;
	}
}

float modulator_get_k(int kn) {
	switch (kn) {
		case 1:
			return k1;
		case 2:
			return k2;
		default:
			return -1;
	}
}

float modulator_get_TEMPER_RATIO() {
	return TEMPER_RATIO; 
}

float modulator_get_NOTE_F_UP() {
	return NOTE_F_UP;
}

float modulator_get_NOTE_F_DOWN() {
	return NOTE_F_DOWN;
}
