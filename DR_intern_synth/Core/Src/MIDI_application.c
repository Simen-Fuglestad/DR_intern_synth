/*
 * MIDI_application.c
 *
 *  First created on: 6 dec. 2014
 *      Author: Xavier Halgand
 */

/* Includes ------------------------------------------------------------------*/

//#include "main.h"
#include "MIDI_application.h"
#include "usb_host.h"
#include "mixer.h"

/* Private define ------------------------------------------------------------*/

#define RX_BUFF_SIZE 64 /* USB MIDI buffer : max received data 64 bytes */


uint8_t MIDI_RX_Buffer[RX_BUFF_SIZE]; // MIDI reception buffer


#define POLY_INPUTS 10
static uint8_t MIDI_input_keys[POLY_INPUTS];
static uint8_t next_key_index = 0;
static uint8_t input_key_tmp;
static uint8_t keys_pressed = 0;

/* Private function prototypes -----------------------------------------------*/
void ProcessReceivedMidiData(uint8_t midi_code, uint8_t n);

/*-----------------------------------------------------------------------------*/
/**
 * @brief  Main routine for MIDI application, looped in main.c
 * @param  None
 * @retval none
 */
void MIDI_Application(void)
{
	if (Appli_state == MIDI_APPLICATION_READY)
	{
		USBH_MIDI_Receive(&hUsbHostFS, MIDI_RX_Buffer, RX_BUFF_SIZE); // just once at the beginning, start the first reception
		Appli_state = APPLICATION_RUNNING;
	}
	if (Appli_state == APPLICATION_RUNNING)
	{
		//....pffff......grrrrr......
	}

	if (Appli_state == MIDI_APPLICATION_DISCONNECT)
	{
		Appli_state = MIDI_APPLICATION_IDLE;
		USBH_MIDI_Stop(&hUsbHostFS);
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief  MIDI data receive callback.
 * @param  phost: Host handle
 * @retval None
 */
void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost)
{
	for (uint8_t i = 0; i < RX_BUFF_SIZE; i+=MIDI_MSG_LEN) {
		ProcessReceivedMidiData(MIDI_RX_Buffer[i], i);
	}

	USBH_MIDI_Receive(&hUsbHostFS, MIDI_RX_Buffer, RX_BUFF_SIZE); // start a new reception
}

uint8_t* MIDI_get_input_keys(void) {
	return MIDI_input_keys;
}

void MIDI_update_input_f(float* f_steps, float f_base) {
	for (uint8_t i = 0; i < POLY_INPUTS; i++) {
		float f = MIDI_key2f(MIDI_input_keys[i])/f_base;
		if (f < 1)
			f = 0;
		f_steps[i] = f;
	}
}

float MIDI_key2f(uint8_t key) {
	float exponent = (float)key - 69;
	exponent = exponent/12;
	return 440 * powf(2, exponent);

}

uint8_t MIDI_get_n_voices() {
	return POLY_INPUTS;
}

void ProcessReceivedMidiData(uint8_t midi_code, uint8_t n) {
	switch(midi_code) {
	case MIDI_CODE_NOTE_OFF:
		if (keys_pressed > 0) {
			input_key_tmp = MIDI_RX_Buffer[n+2];
			for (uint8_t k = 0; k < POLY_INPUTS; k++) {
				if (MIDI_input_keys[k] == input_key_tmp) {
					MIDI_input_keys[k] = 0;
					keys_pressed--;
				}
			}
		}
		break;

	case MIDI_CODE_NOTE_ON:
		if (keys_pressed < POLY_INPUTS) {
			while(MIDI_input_keys[next_key_index] != 0) {
				next_key_index++;
			}

			MIDI_input_keys[next_key_index] = MIDI_RX_Buffer[n+2];
			keys_pressed++;

			next_key_index = 0;
		}
		break;
	case MIDI_CODE_POLY_KEY_PRESS: ; //currently not registering any from microkey S25
	int test_poly_br = 10;
	break;
	case MIDI_CODE_CTRL_CHANGE: ;
	//from sustain btn and north/south joystick movement on KORG microkey s25
	int test_ctrl_br = 10;
	break;
	case MIDI_CODE_PITCH_BEND: ;
	int test_pitch_br = 10;
	default:
		break;
	}
}
