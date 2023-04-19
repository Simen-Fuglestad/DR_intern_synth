/*
 * MIDI_application.h
 *
 *  First created on: 6 dec. 2014
 *      Author: Xavier Halgand
 */

#ifndef MIDI_APPLICATION_H_
#define MIDI_APPLICATION_H_


#define MIDI_MSG_LEN				0x04

//MIDI Code Index Classifications

#define MIDI_CODE_2B_MSG			0x02
#define MIDI_CODE_3B_MSG			0x03
#define MIDI_CODE_SYSEX_SC			0x04 //start/continue
#define MIDI_CODE_1BSCM				0x05 //Single-byte system common message
#define MIDI_CODE_SYSEX_END_B		0x05 //SysEx ends with following single byte
#define MIDI_CODE_SYSEX_END_2B		0x06 //SysEx ends with following 2 bytes
#define MIDI_CODE_SYSEX_END_3B		0x07 //SysEx ends with following 3 bytes
#define MIDI_CODE_NOTE_OFF			0x08
#define MIDI_CODE_NOTE_ON			0x09
#define MIDI_CODE_POLY_KEY_PRESS	0x0A
#define MIDI_CODE_CTRL_CHANGE		0x0B
#define MIDI_CODE_PROGRAM_CHANGE	0x0C
#define MIDI_CODE_CH_PRESSURE		0x0D
#define MIDI_CODE_PITCH_BEND		0x0E
#define MIDI_CODE_1B				0x0F //Single byte

#define MIDI_CODE_RANGE				0x7F


/* Includes ------------------------------------------------------------------*/

#include "stdio.h"
#include "usbh_core.h"
#include "usbh_MIDI.h"

#include <math.h>
#include <stdint.h>

/*------------------------------------------------------------------------------*/
typedef enum
{
	MIDI_APPLICATION_IDLE = 0,
	MIDI_APPLICATION_START,
	MIDI_APPLICATION_READY,
	APPLICATION_RUNNING,
	MIDI_APPLICATION_DISCONNECT
}
MIDI_ApplicationTypeDef;

/*------------------------------------------------------------------------------*/

extern USBH_HandleTypeDef hUsbHostFS;
extern MIDI_ApplicationTypeDef Appli_state;
extern int8_t currentNote;
extern int8_t velocity;

/* Exported functions ------------------------------------------------------- */
void MIDI_Application(void);

uint8_t* MIDI_get_input_keys(void);
void MIDI_update_input(float* f_steps);
float MIDI_key2f(uint8_t);
uint8_t MIDI_get_n_voices();
void MIDI_note_disable(uint8_t midi_code, int index);

/*------------------------------------------------------------------------------*/
#endif /* MIDI_APPLICATION_H_ */
